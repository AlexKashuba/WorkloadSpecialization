#ifndef CLANG_TOOL_AST_UTILS_H
#define CLANG_TOOL_AST_UTILS_H

#include "clang/Analysis/CFG.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/ASTMatchers/ASTMatchers.h>

namespace clang {
class SourceLocation;
class ASTContext;
}

const clang::Expr *tryUnwrapCast(const clang::Expr *node);
llvm::Optional<clang::NamedDecl *> extractDecl(const clang::Expr *node);
const clang::FunctionDecl *findEnclosingFunction(const clang::Stmt *child, clang::ASTContext &context);

#endif // CLANG_TOOL_AST_UTILS_H
