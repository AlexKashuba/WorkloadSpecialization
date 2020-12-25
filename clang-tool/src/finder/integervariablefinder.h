#ifndef INTEGER_VARIABLE_FINDER_HPP
#define INTEGER_VARIABLE_FINDER_HPP

#include "finder.h"

namespace clang
{
    class ASTContext;
}

class IntegerVariableFinder : public Finder
{
    public:

        explicit IntegerVariableFinder(clang::ASTContext &context);

        void start() override;
        void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};


#endif
