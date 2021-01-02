#include "ast_utils.h"
#include <clang/Lex/Lexer.h>

const clang::Expr *tryUnwrapCast(const clang::Expr *node) {
  using namespace clang;
  if (!node) {
    return nullptr;
  }
  if (auto cast = dyn_cast<CastExpr>(node)) {
    return cast->getSubExpr();
  }
  return node;
}

llvm::Optional<clang::NamedDecl *> extractDecl(const clang::Expr *node) {
  using namespace clang;
  auto unwrapped = tryUnwrapCast(node);
  if (auto declRef = dyn_cast<DeclRefExpr>(unwrapped)) {
    if (auto *decl =
            const_cast<NamedDecl *>(dyn_cast<NamedDecl>(declRef->getDecl()))) {
      return {decl};
    }
  }
  return {};
}
