#include "transformer.h"
#include <clang/Rewrite/Core/Rewriter.h>

Transformer::Transformer(clang::ASTContext &context, clang::Rewriter &rewriter,
                         clang::tooling::Replacements &replacements)
    : context(context), replacements(replacements), rewriter(rewriter) {}
