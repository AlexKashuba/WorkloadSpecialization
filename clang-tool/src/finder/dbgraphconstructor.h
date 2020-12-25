#pragma once

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
  std::unordered_map<unsigned, PathFragment*> blockToFragmentMap;

public:
  explicit DBGraphConstructor(clang::ASTContext &context);

  void trace(clang::CFGBlock *B, PathFragment *curr, int depth);
  void start() override;
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
