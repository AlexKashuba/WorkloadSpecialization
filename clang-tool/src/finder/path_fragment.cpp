#include "path_fragment.h"

std::string
PathFragment::printGraph(llvm::StringRef functionName,
                         std::unordered_set<unsigned> &loopHeaders) {
  std::unordered_set<PathFragment *> prunned;
  pruneGraph(prunned, loopHeaders);

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

// Remove blocks from the graph that do not contain accesses
void PathFragment::pruneGraph(std::unordered_set<PathFragment *> &visited,
                              std::unordered_set<unsigned> &loopHeaders) {
  auto *new_next = new std::vector<PathFragment *>;
  visited.insert(this);

  for (auto child : *next) {
    // Only visit each node once
    if (!visited.count(child))
      child->pruneGraph(visited, loopHeaders);

    if (child->accesses.empty() && !loopHeaders.count(child->blockId) &&
        child->back->empty()) {
      for (auto grandchild : *child->next) {
        if (!grandchild->accesses.empty()) {
          addIfNotPresent(new_next, grandchild);
        }
      }
    } else {
      addIfNotPresent(new_next, child);
    }
  }

  delete next;
  next = new_next;
}

void PathFragment::printGraphFragment(llvm::raw_ostream &ostream) {
  ostream << blockId << " [shape=box, label=\"";
  for (DBAccess *access : accesses) {
    access->print(ostream);
    ostream << "\n";
  }
  ostream << "\"];\n";
  for (PathFragment *child : *next) {
    ostream << "  " << blockId << " -> " << child->blockId << ";\n";
  }
  for (PathFragment *loopHeader : *back) {
    ostream << "  " << blockId << " -> " << loopHeader->blockId << ";\n";
  }
}
