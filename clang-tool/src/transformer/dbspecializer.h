#pragma once

#include "../utils/dbanalysisstorage.h"
#include "transformer.h"

#include <set>
#include <string>

#include <clang/Tooling/Core/Replacement.h>

namespace clang {
class ASTContext;
} // namespace clang

class DBSpecializer : public Transformer {
private:
  DBAnalysisInfoStorage &infoStorage;
  std::string &file;

  static void genDecl(const DBRow& row, std::vector<DBAccess *> &accesses,
                      clang::raw_ostream &stream);
  std::string genStructDecls();

  std::unordered_map<DBRow, std::vector<DBAccess *>> filteredRows;
  void filterAccesses();
  void replaceAccesses();

  void insertText(const clang::SourceLocation &loc, const std::string &str, unsigned len=0);

  void replaceGetRow(const DBGetRow &getRow);
  std::unordered_set<DBRow> addedDecls;

public:
  explicit DBSpecializer(clang::ASTContext &context, clang::Rewriter &rewriter,
                         clang::tooling::Replacements &replacements,
                         DBAnalysisInfoStorage &infoStorage, std::string &file);

  void start() override;
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
  void print(clang::raw_ostream &stream) override;
};
