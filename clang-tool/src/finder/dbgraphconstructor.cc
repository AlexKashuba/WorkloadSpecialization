#include "dbgraphconstructor.h"

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

const clang::Expr *tryUnwrapCast(const clang::Expr *node) {
  using namespace clang;
  if (auto cast = dyn_cast<CastExpr>(node)) {
    return cast->getSubExpr();
  }
  return node;
}

llvm::Optional<clang::NamedDecl *> extractDecl(const clang::Expr *node) {
  using namespace clang;
  auto unwrapped = tryUnwrapCast(node);
  if (auto declRef = dyn_cast<DeclRefExpr>(unwrapped)) {
    if (auto *decl =
            const_cast<NamedDecl *>(dyn_cast<NamedDecl>(declRef->getDecl()))) {
      return {decl};
    }
  }
  return {};
}

void extractAccesses(clang::CFGBlock *B, PathFragment *curr) {
  using namespace clang;

  for (CFGElement &E : *B) {
    if (Optional<CFGStmt> S = E.getAs<CFGStmt>()) {
      if (const auto *CE = dyn_cast<CXXMemberCallExpr>(S->getStmt())) {
        const FunctionDecl *func = CE->getDirectCallee();
        auto name = func->getName();
        if (name.contains("get_row")) {
          //TODO: determine the accessed table
          auto args = CE->getArgs();
          if (Optional<NamedDecl *> row_decl = extractDecl(args[0])) {
            (*row_decl)->getDeclName().dump();
          }

          if (Optional<NamedDecl *> accessType = extractDecl(args[1])) {
            (*accessType)->getDeclName().dump();
          }
        }
        if (name.contains("get_value")) {
          if (Optional<NamedDecl *> obj =
                  extractDecl(CE->getImplicitObjectArgument())) {
            auto args = CE->getArgs();
            std::string columnNameStr;
            if (Optional<NamedDecl *> column = extractDecl(args[0])) {
              columnNameStr = (*column)->getDeclName().getAsString();
            }
            Optional<NamedDecl *> storage;
            // TODO: char* returns the value for some reason
            if (CE->getNumArgs() > 2) {
              storage = extractDecl(args[1]);
            }
            DBAccess access{RD, columnNameStr, *obj,
                            storage.getValueOr(nullptr)};
            curr->accesses.emplace_back(access);
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
            DBAccess access{WR, columnNameStr, *obj, nullptr};
            curr->accesses.emplace_back(access);
          }
        }
      }
    }
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
    if (currDomTree->dominates(succ, B) /*is a backedge*/)
      continue;

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

  if (const auto *txnDecl = result.Nodes.getNodeAs<FunctionDecl>("txnMethod")) {
    if (Stmt *txnBody = txnDecl->getBody()) {
      ASTContext *context = result.Context;
      int64_t id = txnBody->getID(*context);
      if (visitedFuncs.count(id)) {
        return;
      }
      visitedFuncs.insert(id);

      std::unique_ptr<CFG> sourceCFG =
          CFG::buildCFG(txnDecl, txnBody, context, clang::CFG::BuildOptions());

      currDomTree = new CFGDomTree();
      currDomTree->buildDominatorTree(sourceCFG.get());

      blockToFragmentMap.clear();

      llvm::errs() << "DB Graph txn: " << txnDecl->getNameAsString() << "\n";
      CFGBlock *entry = &sourceCFG->getEntry();
      PathFragment pathFragment{.blockId = entry->getBlockID()};
      trace(entry, &pathFragment, 0);

      StringRef funcName = txnDecl->getName();
      std::string graph = pathFragment.printGraph(funcName);
      llvm::errs() << graph;

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
