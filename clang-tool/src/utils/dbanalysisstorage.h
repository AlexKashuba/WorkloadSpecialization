#ifndef CLANG_TOOL_DBANALYSISSTORAGE_H
#define CLANG_TOOL_DBANALYSISSTORAGE_H

#include "../finder/dbaccess.h"
#include <clang/AST/Decl.h>
#include <unordered_map>

struct DBAnalysisInfoStorage {
  bool addedStructDefs = false;
  std::unordered_map<clang::NamedDecl *, std::vector<DBAccess *>> rowAccessMap;


};

#endif // CLANG_TOOL_DBANALYSISSTORAGE_H
