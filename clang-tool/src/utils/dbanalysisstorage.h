#ifndef CLANG_TOOL_DBANALYSISSTORAGE_H
#define CLANG_TOOL_DBANALYSISSTORAGE_H

#include "../finder/dbaccess.h"
#include <clang/AST/Decl.h>
#include <unordered_map>
#include <unordered_set>

struct DBAnalysisInfoStorage {
  bool addedStructDefs = false;
  std::unordered_map<std::string, std::unordered_set<DBAccess *>> fileAccessMap;
  std::unordered_map<DBRow, std::vector<DBAccess *>> rowAccessMap;
//  std::unordered_map<std::string, std::vector<DBGetRow>> fileGetRowMap;
};

#endif // CLANG_TOOL_DBANALYSISSTORAGE_H
