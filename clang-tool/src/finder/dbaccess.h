#pragma once

#include "finder.h"
#include <clang/Analysis/CFG.h>

enum DBAccessType { RD, WR };

struct DBAccess {
  DBAccessType type;
  std::string txnName, rowName, columnName, cType, extraInfo;
  clang::SourceLocation beginLocation;
  bool isPtr;

  void print(llvm::raw_ostream &ostream) {
    std::string type_str = type == RD ? "RD" : "WR";
    ostream << type_str << ":" << rowName << "[" << columnName
            << "]";
  }
};
