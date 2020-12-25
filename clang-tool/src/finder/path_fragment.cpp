#include "path_fragment.h"

std::string PathFragment::printGraph(llvm::StringRef functionName) {
  std::unordered_set<PathFragment *> prunned;
  pruneGraph(prunned);

  std::string out;
  llvm::raw_string_ostream ostream{out};
  ostream << "digraph " << functionName << "_access  { \n";

  std::unordered_set<PathFragment *> visited;
  std::vector<PathFragment *> children;
  children.emplace_back(this);
  while (!children.empty()) {
    auto childIT = children.begin();
    PathFragment *child = *childIT;
    if (!visited.count(child)) {
      child->printGraphFragment(ostream);
      visited.insert(child);
      children.insert(children.end(), child->next->begin(), child->next->end());
    }
    children.erase(children.begin());
  }
  ostream << "}\n";
  ostream.flush();
  return out;
}

void addIfNotPresent(std::vector<PathFragment *> *v, PathFragment *el) {
  if (std::find(v->begin(), v->end(), el) == v->end()) {
    v->emplace_back(el);
  }
}

void PathFragment::pruneGraph(std::unordered_set<PathFragment *> &visited) {
  auto *new_next = new std::vector<PathFragment *>;
  for (auto child : *next) {
    if (!visited.count(child))
      child->pruneGraph(visited);
    if (child->accesses.empty()) {
      for (auto grandchild : *child->next) {
        if (!grandchild->accesses.empty()) {
          addIfNotPresent(new_next, grandchild);
        }
      }
    } else {
      addIfNotPresent(new_next, child);
    }
    visited.insert(this);
  }
  delete next;
  next = new_next;
}

void PathFragment::printGraphFragment(llvm::raw_ostream &ostream) {
  ostream << blockId << " [shape=box, label=\"";
  for (DBAccess &access : accesses) {
    access.print(ostream);
    ostream << "\n";
  }
  ostream << "\"];\n";
  for (PathFragment *child : *next) {
    ostream << "  " << blockId << " -> " << child->blockId << ";\n";
  }
}
