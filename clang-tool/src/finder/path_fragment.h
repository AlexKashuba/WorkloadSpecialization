#pragma once

#include "dbaccess.h"
#include "finder.h"
#include <clang/Analysis/CFG.h>
#include <unordered_set>

struct PathFragment {
  unsigned blockId;
  std::vector<DBAccess*> accesses;
  std::vector<PathFragment *> *next = new std::vector<PathFragment *>;
  std::string printGraph(llvm::StringRef functionName);

private:
  void pruneGraph(std::unordered_set<PathFragment*> &visited);
  void printGraphFragment(llvm::raw_ostream &ostream);
};
