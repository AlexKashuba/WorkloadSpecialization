#pragma once

#include "dbaccess.h"
#include "finder.h"
#include <clang/Analysis/CFG.h>
#include <unordered_set>

struct PathFragment {
  unsigned blockId;
  std::vector<DBAccess *> accesses;
  std::vector<PathFragment *> *next = new std::vector<PathFragment *>;
  std::vector<PathFragment *> *back = new std::vector<PathFragment *>;
  std::string printGraph(llvm::StringRef functionName, std::unordered_set<unsigned> &loopHeaders);

private:
  void pruneGraph(std::unordered_set<PathFragment *> &visited, std::unordered_set<unsigned> &loopHeaders);
  void printGraphFragment(llvm::raw_ostream &ostream);
};
