#include "frontendaction.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>

#include "../consumer/consumer.h"

std::unique_ptr<clang::ASTConsumer>
DBAnalysisFrontendAction::CreateASTConsumer(clang::CompilerInstance &compiler,
                                            llvm::StringRef inFile) {
  return std::unique_ptr<clang::ASTConsumer>(
      new DBAnalysisConsumer(compiler.getASTContext(), infoStorage, inFile.str()));
}

std::unique_ptr<clang::ASTConsumer>
DBTransformationFrontendAction::CreateASTConsumer(
    clang::CompilerInstance &compiler, llvm::StringRef inFile) {
  std::string file = inFile.str();
  return std::unique_ptr<clang::ASTConsumer>(new DBSpecializationConsumer(
      compiler.getASTContext(), infoStorage, replacements[file], file));
}
