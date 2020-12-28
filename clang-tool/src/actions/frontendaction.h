#ifndef FRONTEND_ACTION_HPP
#define FRONTEND_ACTION_HPP

#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/StringRef.h>

#include "../utils/dbanalysisstorage.h"
#include <clang/Tooling/Core/Diagnostic.h>
#include <memory>
#include <string>
#include <vector>

namespace clang {
class CompilerInstance;
}

class DBAnalysisFrontendAction : public clang::ASTFrontendAction {
protected:
  DBAnalysisInfoStorage &infoStorage;

public:
  explicit DBAnalysisFrontendAction(DBAnalysisInfoStorage &storage)
      : infoStorage(storage){};

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &compiler,
                    llvm::StringRef inFile) override;
};

class DBTransformationFrontendAction : public DBAnalysisFrontendAction {
  std::map<std::string, clang::tooling::Replacements> &replacements;

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &compiler,
                    llvm::StringRef inFile) override;

public:
  explicit DBTransformationFrontendAction(
      DBAnalysisInfoStorage &storage,
      std::map<std::string, clang::tooling::Replacements> &replacements)
      : DBAnalysisFrontendAction(storage), replacements(replacements){};
};

class DBAnalysisFrontendActionFactory
    : public clang::tooling::FrontendActionFactory {

protected:
  DBAnalysisInfoStorage &infoStorage;

public:
  explicit DBAnalysisFrontendActionFactory(DBAnalysisInfoStorage &infoStorage)
      : infoStorage(infoStorage){};

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<DBAnalysisFrontendAction>(infoStorage);
  }
};

class DBTransformationFrontendActionFactory
    : public DBAnalysisFrontendActionFactory {

  std::map<std::string, clang::tooling::Replacements> &replacements;

public:
  explicit DBTransformationFrontendActionFactory(
      DBAnalysisInfoStorage &infoStorage,
      std::map<std::string, clang::tooling::Replacements> &replacements)
      : DBAnalysisFrontendActionFactory(infoStorage),
        replacements(replacements){};

  std::unique_ptr<clang::FrontendAction> create() override {
    return std::make_unique<DBTransformationFrontendAction>(infoStorage,
                                                            replacements);
  }
};

#endif
