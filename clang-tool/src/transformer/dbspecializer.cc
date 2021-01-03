#include "dbspecializer.h"
#include "../utils/ast_utils.h"
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Lex/Lexer.h>
#include <clang/Rewrite/Core/Rewriter.h>

DBSpecializer::DBSpecializer(clang::ASTContext &context,
                             clang::Rewriter &rewriter,
                             clang::tooling::Replacements &replacements,
                             DBAnalysisInfoStorage &storage, std::string &file)
    : Transformer(context, rewriter, replacements), infoStorage(storage),
      file(file) {}

void DBSpecializer::start() {
  using namespace clang::ast_matchers;

  filterAccesses();

  MatchFinder getRowMatchFinder;
  auto getRowCall =
      hasDescendant(callExpr(callee(functionDecl(hasName("get_row")))));
  auto getRowAssignmentMatcher =
      binaryOperator(isAssignmentOperator(), getRowCall)
          .bind("getRowAssignment");
  auto getRowDeclMatcher = varDecl(getRowCall).bind("getRowDecl");

  getRowMatchFinder.addMatcher(getRowAssignmentMatcher, this);
  getRowMatchFinder.addMatcher(getRowDeclMatcher, this);
  getRowMatchFinder.matchAST(context);

  MatchFinder matchFinder;
  auto rowClassDeclMatcher =
      cxxRecordDecl(hasName("row_storage"), isDefinition()).bind("rowClass");
  matchFinder.addMatcher(rowClassDeclMatcher, this);
  matchFinder.matchAST(context);
}

void DBSpecializer::genDecl(const DBRow &row, std::vector<DBAccess *> &accesses,
                            clang::raw_ostream &ostream) {
  using namespace clang;

  if (accesses.empty())
    return;

  llvm::SmallSet<std::string, 32> inStruct;

  std::string copyFromFunc, applyToFunc;
  llvm::raw_string_ostream copyFromStream(copyFromFunc),
      applyToStream(applyToFunc);

  copyFromStream << "  void copyFrom(row_t *row) override {\n";
  applyToStream << "  void applyTo(row_t *row) override {\n";

  ostream << "struct " << row.toStorageType() << " : row_storage { \n";
  for (DBAccess *access : accesses) {
    if (inStruct.count(access->columnName)) {
      continue;
    }
    inStruct.insert(access->columnName);

    ostream << "  " << access->cType << " ";

    std::string fieldName = "f_" + access->columnName;
    ostream << fieldName << ";";
    ostream << " /* " << access->secondArg << " */\n";

    copyFromStream << "    row->get_value(" << access->columnName << ", "
                   << fieldName << ");\n";
    applyToStream << "     row->set_value(" << access->columnName << ", "
                  << fieldName << ");\n";
  }
  copyFromStream << "  }";
  applyToStream << "  }";

  ostream << copyFromStream.str() << "\n";
  ostream << applyToStream.str() << "\n";

  ostream << "};\n";
}

std::string DBSpecializer::genStructDecls() {
  std::string out;
  llvm::raw_string_ostream ostream(out);
  ostream << "\n";

  for (auto &IT : filteredRows) {
    genDecl(IT.first, IT.second, ostream);
  }

  return ostream.str();
}

void DBSpecializer::run(
    const clang::ast_matchers::MatchFinder::MatchResult &result) {
  using namespace clang;

  if (const auto *getRowAssignment =
          result.Nodes.getNodeAs<BinaryOperator>("getRowAssignment")) {
    if (Optional<NamedDecl *> decl = extractDecl(getRowAssignment->getLHS())) {
      auto txnName =
          findEnclosingFunction(getRowAssignment, context)->getName().str();
      auto call = getRowAssignment->getRHS();
      call->dumpPretty(context);
      DBGetRow getRow{(*decl)->getBeginLoc(),
                      call->getSourceRange(),
                      {txnName, (*decl)->getName().str()}};
      if (filteredRows.find(getRow.row) != filteredRows.end()) {
        replaceGetRow(getRow);
      }
    }
  }

  if (const auto *getRowDecl = result.Nodes.getNodeAs<VarDecl>("getRowDecl")) {
    auto txnName =
        findEnclosingFunction(getRowDecl->getInit(), context)->getName().str();
    auto call = getRowDecl->getInit();
    call->dumpPretty(context);
    DBGetRow getRow{getRowDecl->getBeginLoc(),
                    call->getSourceRange(),
                    {txnName, getRowDecl->getName().str()}};
    if (filteredRows.find(getRow.row) != filteredRows.end()) {
      replaceGetRow(getRow);
    }
  }

  if (!infoStorage.addedStructDefs) {
    if (const auto *rowClass =
            result.Nodes.getNodeAs<CXXRecordDecl>("rowClass")) {
      std::string decls = genStructDecls();
      SourceLocation loc = Lexer::findLocationAfterToken(
          rowClass->getEndLoc(), tok::TokenKind::semi,
          context.getSourceManager(), context.getLangOpts(), true);
      if (rewriter.InsertTextAfterToken(loc, decls)) {
        llvm::errs() << "location was not rewritable\n";
      }
      loc.dump(context.getSourceManager());
      infoStorage.addedStructDefs = true;
      replaceAccesses();
    }
  }
}

void DBSpecializer::insertText(const clang::SourceLocation &loc,
                               const std::string &str, unsigned len) {
  clang::tooling::Replacement rep(context.getSourceManager(), loc, len, str);
  if (llvm::Error err = replacements.add(rep)) {
    llvm::errs() << err << "\n";
  }
}

void DBSpecializer::replaceGetRow(const DBGetRow &getRow) {
  auto ty = getRow.row.toStorageType();
  if (!addedDecls.count(getRow.row)) {
    std::string storageDecl;
    llvm::raw_string_ostream ostream(storageDecl);
    ostream << ty << " " << getRow.row.rowName << "_t;\n";
    insertText(getRow.rowDeclLocation, ostream.str());
    addedDecls.insert(getRow.row);
  }
  std::string repStr = ", &" + getRow.row.rowName + "_t)";
  insertText(getRow.assignmentLocation.getEnd(), repStr, 1);
}

void DBSpecializer::replaceAccesses() {
  std::unordered_set<DBAccess *> filesAccesses =
      infoStorage.fileAccessMap[file];
  for (auto &IT : filteredRows) {
    DBRow row = IT.first;
    if (!addedDecls.count(row))
      continue;

    std::vector<DBAccess *> accesses = IT.second;
    for (auto &access : accesses) {
      if (filesAccesses.count(access)) {
        std::string str;
        llvm::raw_string_ostream ostream(str);
        if (access->type == RD) {
          ostream << access->secondArg << " = " << access->row.rowName
                  << "_t.f_" << access->columnName;
        } else {
          ostream << access->row.rowName << "_t.f_" << access->columnName
                  << " = " << access->secondArg;
        }
        clang::tooling::Replacement rep(
            context.getSourceManager(),
            clang::CharSourceRange::getCharRange(
                access->sourceRange.getBegin(),
                access->sourceRange.getEnd().getLocWithOffset(1)),
            ostream.str());
        if (auto err = replacements.add(rep)) {
          llvm::errs() << "Error: " << err << "\n";
        }
      }
    }
  }
}

void DBSpecializer::print(clang::raw_ostream &stream) {}

void DBSpecializer::filterAccesses() {
  for (auto &IT : infoStorage.rowAccessMap) {
    DBRow row = IT.first;
    std::vector<DBAccess *> accesses = IT.second;
    bool shouldAdd = true;
    for (auto &access : accesses) {
      if (access->isPtr) {
        // TODO: bail out if it's a write in a loop

        // bail out, we don't want to copy pointers
        shouldAdd = false;
        break;
      }
    }
    if (!shouldAdd)
      continue;
    filteredRows[row] = accesses;
  }
}
