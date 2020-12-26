#include "dbgraphconstructor.h"

#include "../utils/ast_utils.h"
#include "dbaccess.h"
#include "path_fragment.h"
#include "clang/Analysis/CFG.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>

DBGraphConstructor::DBGraphConstructor(clang::ASTContext &context)
    : Finder(context) {}

void DBGraphConstructor::start() {
  using namespace clang::ast_matchers;

  MatchFinder matchFinder;

  auto txnCodeMatcher = functionDecl(matchesName(".*run_.*")).bind("txnMethod");
  matchFinder.addMatcher(txnCodeMatcher, this);

  matchFinder.matchAST(context);
}

void DBGraphConstructor::extractAccesses(clang::CFGBlock *B,
                                         PathFragment *curr) {
  using namespace clang;

  // Iterate over all the statements in Basic Block
  for (CFGElement &E : *B) {
    if (Optional<CFGStmt> S = E.getAs<CFGStmt>()) {
      if (const auto *CE = dyn_cast<CXXMemberCallExpr>(S->getStmt())) {
        const FunctionDecl *func = CE->getDirectCallee();
        auto name = func->getName();
        //        if (name.contains("get_row")) {
        //          //TODO: determine the accessed table
        //          auto args = CE->getArgs();
        //          if (Optional<NamedDecl *> row_decl = extractDecl(args[0])) {
        //            (*row_decl)->getDeclName().dump();
        //          }
        //
        //          if (Optional<NamedDecl *> accessType = extractDecl(args[1]))
        //          {
        //            (*accessType)->getDeclName().dump();
        //          }
        //        }
        if (name.contains("get_value")) {
          if (Optional<NamedDecl *> obj =
                  extractDecl(CE->getImplicitObjectArgument())) {
            auto args = CE->getArgs();
            std::string columnNameStr;
            if (Optional<NamedDecl *> column = extractDecl(args[0])) {
              columnNameStr = (*column)->getDeclName().getAsString();
            }
            Optional<NamedDecl *> storage;

            // XXX: char* returns the value for some reason
            if (CE->getNumArgs() > 1) {
              storage = extractDecl(args[1]);
            }

            auto *access = new DBAccess{RD, columnNameStr, *obj,
                                        storage.getValueOr(nullptr), nullptr};
            curr->accesses.emplace_back(access);

            rowAccessMap[*obj].emplace_back(access);
          }
        }
        if (name.contains("set_value")) {
          if (Optional<NamedDecl *> obj =
                  extractDecl(CE->getImplicitObjectArgument())) {
            auto args = CE->getArgs();
            std::string columnNameStr;
            if (Optional<NamedDecl *> column = extractDecl(args[0])) {
              columnNameStr = (*column)->getDeclName().getAsString();
            }

            auto *access =
                new DBAccess{WR, columnNameStr, *obj, nullptr, args[1]};
            curr->accesses.emplace_back(access);
            rowAccessMap[*obj].emplace_back(access);
          }
        }
      }
    }
  }
}

std::string DBGraphConstructor::genDecl(clang::NamedDecl *row,
                                        std::vector<DBAccess *> &accesses) {
  using namespace clang;
  llvm::SmallSet<std::string, 32> inStruct;
  std::string out;
  llvm::raw_string_ostream ostream(out);
  ostream << "struct " << row->getDeclName().getAsString()
          << "_storage : row_storage_t { \n";
  for (DBAccess *access : accesses) {
    if (inStruct.count(access->columnName)) {
      continue;
    }
    inStruct.insert(access->columnName);

    std::string type;
    std::string comment;
    if (NamedDecl *storage = access->storage) {
      if (auto *valueDecl = dyn_cast<ValueDecl>(storage)) {
        type = valueDecl->getType().getAsString();
        comment = storage->getName().str();
      }
    }

    if (const Expr *value = tryUnwrapCast(access->value)) {
      type = value->getType().getAsString();

      llvm::raw_string_ostream commentStream(comment);
      value->printPretty(commentStream, nullptr,
                         PrintingPolicy(context.getLangOpts()));
      commentStream.flush();
    }

    if (type.empty()) {
      type = "char *";
    }

    ostream << "  " << type << " ";
    ostream << access->columnName << ";";
    ostream << " /* " << comment << " */\n";
  }
  ostream << "};\n";
  return ostream.str();
}

void DBGraphConstructor::genStructDecls() {
  for (auto &IT : rowAccessMap) {
    llvm::errs() << genDecl(IT.first, IT.second);
  }
}

void DBGraphConstructor::trace(clang::CFGBlock *B, PathFragment *curr,
                               int depth) {
  using namespace clang;

  if (depth > 400) {
    llvm::errs() << "IN TOO DEEP!!!\n";
    exit(1);
  }

  extractAccesses(B, curr);

  for (CFGBlock *succ : B->succs()) {
    if (!succ)
      continue;
    unsigned succId = succ->getBlockID();

    // TODO: mark accesses in the loop with quantifier
    // Check that we are not stuck in a loop
    if (currDomTree->dominates(succ, B) /*is a backedge*/)
      continue;

    // Only visit each block once
    auto pfIterator = blockToFragmentMap.find(succId);
    if (pfIterator != blockToFragmentMap.end()) {
      curr->next->emplace_back(pfIterator->second);
      continue;
    }

    auto *pf = new PathFragment{.blockId = succId};
    curr->next->emplace_back(pf);
    blockToFragmentMap[succId] = pf;

    trace(succ, pf, depth + 1);
  }
}

void DBGraphConstructor::run(
    const clang::ast_matchers::MatchFinder::MatchResult &result) {
  using namespace clang;

  // Find a txn function
  if (const auto *txnDecl = result.Nodes.getNodeAs<FunctionDecl>("txnMethod")) {
    if (Stmt *txnBody = txnDecl->getBody()) {
      ASTContext *context = result.Context;

      // Only visit each function body once (there can be multiple declarations)
      int64_t id = txnBody->getID(*context);
      if (visitedFuncs.count(id)) {
        return;
      }
      visitedFuncs.insert(id);

      // Build the CFG and the dominator tree
      std::unique_ptr<CFG> sourceCFG =
          CFG::buildCFG(txnDecl, txnBody, context, clang::CFG::BuildOptions());

      currDomTree = new CFGDomTree();
      currDomTree->buildDominatorTree(sourceCFG.get());

      blockToFragmentMap.clear();

      // Trace all paths in the CFG and construct a new fragment graph
      llvm::errs() << "DB Graph txn: " << txnDecl->getNameAsString() << "\n";
      CFGBlock *entry = &sourceCFG->getEntry();
      PathFragment pathFragment{.blockId = entry->getBlockID()};
      trace(entry, &pathFragment, 0);

      // Prune and print the access graph
      StringRef funcName = txnDecl->getName();
      std::string graph = pathFragment.printGraph(funcName);
      //      llvm::errs() << graph;

      genStructDecls();

      // Write the graph into a file
      std::error_code err;
      llvm::raw_fd_ostream fStream(funcName.str() + ".dot", err);
      if (!err)
        fStream << graph;
      else
        llvm::errs() << err.message();

      //      auto langOpt = context->getLangOpts();
      //      sourceCFG->dump(langOpt, true);
    }
  }
}
