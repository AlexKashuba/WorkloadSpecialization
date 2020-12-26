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
  std::unordered_map<unsigned, PathFragment *> blockToFragmentMap;

  std::unordered_map<clang::NamedDecl *, std::vector<DBAccess *>> rowAccessMap;
  void extractAccesses(clang::CFGBlock *B, PathFragment *curr);

  std::string genDecl(clang::NamedDecl *row, std::vector<DBAccess *> &accesses);
  void genStructDecls();

public:
  explicit DBGraphConstructor(clang::ASTContext &context);

  void trace(clang::CFGBlock *B, PathFragment *curr, int depth);
  void start() override;
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
