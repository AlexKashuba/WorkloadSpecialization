#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>

namespace clang {
class ASTContext;
//    class raw_ostream;
class Rewriter;
} // namespace clang

class Transformer : public clang::ast_matchers::MatchFinder::MatchCallback {
protected:
  clang::ASTContext &context;
  clang::Rewriter &rewriter;
  clang::tooling::Replacements &replacements;

public:
  explicit Transformer(clang::ASTContext &context, clang::Rewriter &rewriter,
                       clang::tooling::Replacements &replacements);

  virtual void start() = 0;
  virtual void print(clang::raw_ostream &stream) = 0;
};

#endif
