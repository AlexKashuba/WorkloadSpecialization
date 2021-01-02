#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include "../utils/dbanalysisstorage.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>

namespace clang {
class ASTContext;
}

class DBAnalysisConsumer : public clang::ASTConsumer {
private:
  DBAnalysisInfoStorage &infoStorage;
  std::string file;

public:
  explicit DBAnalysisConsumer(clang::ASTContext &context,
                              DBAnalysisInfoStorage &storage,
                              std::string file);
  void HandleTranslationUnit(clang::ASTContext &context) override;
};

class DBSpecializationConsumer : public clang::ASTConsumer {
private:
  DBAnalysisInfoStorage &infoStorage;
  clang::Rewriter rewriter;
  clang::tooling::Replacements &replacements;
  std::string file;

public:
  explicit DBSpecializationConsumer(clang::ASTContext &context,
                                    DBAnalysisInfoStorage &storage,
                                    clang::tooling::Replacements &replacements,
                                    std::string file);
  void HandleTranslationUnit(clang::ASTContext &context) override;
};

#endif
