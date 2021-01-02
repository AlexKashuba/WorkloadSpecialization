#ifndef CLANG_TOOL_DBANALYSISSTORAGE_H
#define CLANG_TOOL_DBANALYSISSTORAGE_H

#include "../finder/dbaccess.h"
#include <clang/AST/Decl.h>
#include <unordered_map>
#include <unordered_set>

struct DBAnalysisInfoStorage {
  bool addedStructDefs = false;
  std::unordered_map<std::string, std::unordered_set<DBAccess *>> fileAccessMap;
  std::unordered_map<std::string, std::vector<DBAccess *>> rowAccessMap;
};

#endif // CLANG_TOOL_DBANALYSISSTORAGE_H
