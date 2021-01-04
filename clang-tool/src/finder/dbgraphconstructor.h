#pragma once

#include "../utils/dbanalysisstorage.h"
#include "finder.h"
#include "path_fragment.h"
#include "clang/Analysis/Analyses/Dominators.h"
#include <clang/Analysis/CFG.h>
#include <unordered_map>

namespace clang {
class ASTContext;
}

class DBGraphConstructor : public Finder {
  llvm::SmallSet<int64_t, 32> visitedFuncs;
  clang::CFGDomTree *currDomTree = nullptr;
  std::unordered_map<unsigned, PathFragment *> blockToFragmentMap;

  void extractAccesses(clang::CFGBlock *B, PathFragment *curr);

  DBAnalysisInfoStorage &infoStorage;
  std::string currTxn;
  std::string file;

  std::string getColumnString(const clang::Expr *arg) const;

public:
  explicit DBGraphConstructor(clang::ASTContext &context,
                              DBAnalysisInfoStorage &infoStorage,
                              std::string &file);

  void trace(clang::CFGBlock *B, PathFragment *curr, int depth);
  void start() override;
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
