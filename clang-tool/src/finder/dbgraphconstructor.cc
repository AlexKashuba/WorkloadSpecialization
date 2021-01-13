#include "dbgraphconstructor.h"

#include "../utils/ast_utils.h"
#include "dbaccess.h"
#include "path_fragment.h"
#include "clang/Analysis/CFG.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>

DBGraphConstructor::DBGraphConstructor(clang::ASTContext &context,
                                       DBAnalysisInfoStorage &storage,
                                       std::string &file)
    : Finder(context), infoStorage(storage), file(file) {}

void DBGraphConstructor::start() {
  using namespace clang::ast_matchers;

  MatchFinder txnMatchFinder;
  auto txnCodeMatcher = functionDecl(matchesName(".*run_.*")).bind("txnMethod");
  txnMatchFinder.addMatcher(txnCodeMatcher, this);
  txnMatchFinder.matchAST(context);
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
        if (name.contains("get_value")) {
          if (Optional<NamedDecl *> obj =
                  extractDecl(CE->getImplicitObjectArgument())) {
            auto args = CE->getArgs();
            std::string columnNameStr = getColumnString(args[0]);
            Optional<NamedDecl *> storage;

            // XXX: char* returns the value for some reason
            std::string cType, extraInfo;
            bool isPtr = false;
            if (CE->getNumArgs() > 1) {
              storage = extractDecl(args[1]);
              if (auto *valueDecl = dyn_cast<ValueDecl>(*storage)) {
                QualType ty = valueDecl->getType();
                cType = ty.getAsString();
                isPtr = ty->isPointerType();
                extraInfo = (*storage)->getName().str();
              }
            } else {
              cType = "char *";
              isPtr = true;
            }

            std::string rowName = (*obj)->getName().str();
            auto *access = new DBAccess{
                RD,        {currTxn, rowName},   columnNameStr, cType,
                extraInfo, CE->getSourceRange(), isPtr};
            curr->accesses.emplace_back(access);

            infoStorage.fileAccessMap[file].insert(access);
            infoStorage.rowAccessMap[access->row].emplace_back(access);
          }
        }
        if (name.contains("set_value")) {
          if (Optional<NamedDecl *> obj =
                  extractDecl(CE->getImplicitObjectArgument())) {
            auto args = CE->getArgs();
            std::string columnNameStr = getColumnString(args[0]);
            std::string cType, extraInfo;
            bool isPtr = false;
            if (const Expr *value = tryUnwrapCast(args[1])) {
              QualType ty = value->getType();
              cType = ty.getAsString();
              isPtr = ty->isPointerType();

              llvm::raw_string_ostream commentStream(extraInfo);
              value->printPretty(commentStream, nullptr,
                                 PrintingPolicy(context.getLangOpts()));
              commentStream.flush();
            }

            std::string rowName = (*obj)->getName().str();
            auto *access = new DBAccess{
                WR,        {currTxn, rowName},   columnNameStr, cType,
                extraInfo, CE->getSourceRange(), isPtr};
            curr->accesses.emplace_back(access);
            infoStorage.fileAccessMap[file].insert(access);
            infoStorage.rowAccessMap[access->row].emplace_back(access);
          }
        }
      }
    }
  }
}

std::string DBGraphConstructor::getColumnString(const clang::Expr *arg) const {
  std::string columnNameStr;
  if (llvm::Optional<clang::NamedDecl *> column = extractDecl(arg)) {
    columnNameStr = (*column)->getDeclName().getAsString();
  } else {
    llvm::raw_string_ostream ostream(columnNameStr);
    arg->printPretty(ostream, nullptr,
                     clang::PrintingPolicy(context.getLangOpts()));
    ostream.flush();
  }
  return columnNameStr;
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

    // Check that we are not stuck in a loop
    if (currDomTree->dominates(succ, B) /*is a backedge*/) {
      curr->back->emplace_back(blockToFragmentMap[succId]);
      loopHeaders.insert(succId);
      continue;
    }

    // Only visit each block once
    auto pfIterator = blockToFragmentMap.find(succId);
    if (pfIterator != blockToFragmentMap.end()) {
      curr->next->emplace_back(pfIterator->second);
      continue;
    }

    //TODO: PathFragments are probably leaking during graph pruning, use shared_ptr
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
      currTxn = txnDecl->getNameAsString();
      llvm::errs() << "DB Graph txn: " << currTxn << "\n";
      CFGBlock *entry = &sourceCFG->getEntry();
      PathFragment pathFragment{.blockId = entry->getBlockID()};
      trace(entry, &pathFragment, 0);

      // Prune and print the access graph
      StringRef funcName = txnDecl->getName();
      std::string graph = pathFragment.printGraph(funcName, loopHeaders);

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
