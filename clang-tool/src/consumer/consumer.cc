#include "consumer.h"
#include "../finder/dbgraphconstructor.h"
#include "../transformer/dbspecializer.h"

DBAnalysisConsumer::DBAnalysisConsumer(clang::ASTContext &context,
                                       DBAnalysisInfoStorage &storage)
    : infoStorage(storage) {}

void DBAnalysisConsumer::HandleTranslationUnit(clang::ASTContext &context) {
  DBGraphConstructor graphConstructor(context, infoStorage);
  graphConstructor.start();
}

DBSpecializationConsumer::DBSpecializationConsumer(
    clang::ASTContext &context, DBAnalysisInfoStorage &storage,
    clang::tooling::Replacements &replacements)
    : infoStorage(storage), replacements(replacements) {}

void DBSpecializationConsumer::HandleTranslationUnit(
    clang::ASTContext &context) {

  rewriter.setSourceMgr(context.getSourceManager(), context.getLangOpts());
  DBSpecializer specializer(context, rewriter, replacements, infoStorage);
  specializer.start();
  rewriter.overwriteChangedFiles();
}
