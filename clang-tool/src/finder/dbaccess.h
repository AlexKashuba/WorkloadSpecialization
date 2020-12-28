#pragma once

#include "finder.h"
#include <clang/Analysis/CFG.h>

enum DBAccessType { RD, WR };

struct DBAccess {
  DBAccessType type;
  std::string columnName;
  clang::NamedDecl *row, *storage;
  const clang::Expr *value;

  void print(llvm::raw_ostream &ostream) {
    std::string type_str = type == RD ? "RD" : "WR";
    ostream << type_str << ":" << row->getName().str() << "[" << columnName
            << "]";
  }
};
