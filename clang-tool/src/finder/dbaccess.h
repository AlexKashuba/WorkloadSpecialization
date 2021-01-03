#pragma once

#include "finder.h"
#include <clang/Analysis/CFG.h>

enum DBAccessType { RD, WR };

struct DBRow {
  std::string txnName, rowName;

  std::string toStorageType() const {
    return txnName + "_" + rowName + "_storage";
  }

  bool operator==(const DBRow &rhs) const {
    return txnName == rhs.txnName && rowName == rhs.rowName;
  }
};

struct DBAccess {
  DBAccessType type;
  DBRow row;
  std::string columnName, cType, secondArg;
  clang::SourceRange sourceRange;
  bool isPtr;

  void print(llvm::raw_ostream &ostream) {
    std::string type_str = type == RD ? "RD" : "WR";
    ostream << type_str << ":" << row.rowName << "[" << columnName << "]";
  }
};

struct DBGetRow {
  clang::SourceLocation rowDeclLocation;
  clang::SourceRange assignmentLocation;
//  std::string txnName, rowName;
  DBRow row;

//  bool operator==(const DBGetRow &rhs) const {
//    return row == rhs.row;
//  }
};

namespace std {
template <> struct hash<DBRow> {
  size_t operator()(const DBRow &k) const {
    return hash<string>()(k.txnName) ^ hash<string>()(k.rowName);
  }
};

//template <> struct hash<DBGetRow> {
//  size_t operator()(const DBGetRow &k) const {
//    return hash<DBRow>()(k.row);
//  }
//};
} // namespace std
