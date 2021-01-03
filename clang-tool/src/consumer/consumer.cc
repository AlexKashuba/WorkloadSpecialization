#include "consumer.h"

#include <utility>
#include "../finder/dbgraphconstructor.h"
#include "../transformer/dbspecializer.h"

DBAnalysisConsumer::DBAnalysisConsumer(clang::ASTContext &context,
                                       DBAnalysisInfoStorage &storage,
                                       std::string file)
    : infoStorage(storage), file(std::move(file)) {}

void DBAnalysisConsumer::HandleTranslationUnit(clang::ASTContext &context) {
  DBGraphConstructor graphConstructor(context, infoStorage, file);
  graphConstructor.start();
}

DBSpecializationConsumer::DBSpecializationConsumer(
    clang::ASTContext &context, DBAnalysisInfoStorage &storage,
    clang::tooling::Replacements &replacements, std::string file)
    : infoStorage(storage), replacements(replacements), file(std::move(file)) {}

void DBSpecializationConsumer::HandleTranslationUnit(
    clang::ASTContext &context) {

  rewriter.setSourceMgr(context.getSourceManager(), context.getLangOpts());
  DBSpecializer specializer(context, rewriter, replacements, infoStorage, file);
  specializer.start();
  rewriter.overwriteChangedFiles();
  if(!clang::tooling::applyAllReplacements(replacements, rewriter)){
    llvm::errs() << "couldn't apply all replacements\n";
  }
//  replacements.clear();
}
