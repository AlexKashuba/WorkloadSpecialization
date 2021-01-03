#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>

#include "actions/frontendaction.h"

#include <clang/Tooling/Refactoring.h>

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;

int main(int argc, const char **argv) {
  llvm::cl::OptionCategory ctCategory("clang-tool options");
  CommonOptionsParser optionsParser(argc, argv, ctCategory);

  ClangTool analysisTool(optionsParser.getCompilations(),
                         optionsParser.getSourcePathList());

  DBAnalysisInfoStorage storage;
  DBAnalysisFrontendActionFactory factory(storage);
  analysisTool.run(&factory);

  RefactoringTool Tool(optionsParser.getCompilations(),
                       optionsParser.getSourcePathList());

  DBTransformationFrontendActionFactory transformationFactory(
      storage, Tool.getReplacements());

  if (int Error = Tool.runAndSave(&transformationFactory)) {
    return Error;
  }

  llvm::outs() << "Replacements collected by the tool:\n";
  for (auto &r : Tool.getReplacements()) {
    llvm::outs() << r.first << "\n";
  }


  //  for (auto &sourceFile : optionsParser.getSourcePathList()) {
  //    if (!utils::fileExists(sourceFile)) {
  //      llvm::errs() << "File: " << sourceFile << " does not exist!\n";
  //      return -1;
  //    }
  //
  //    auto sourcetxt = utils::getSourceCode(sourceFile);
  //    auto compileCommands =
  //    optionsParser.getCompilations().getCompileCommands(
  //        getAbsolutePath(sourceFile));
  //
  //    std::vector<std::string> compileArgs =
  //        utils::getCompileArgs(compileCommands);
  //    compileArgs.push_back("-I" +
  //    utils::getClangBuiltInIncludePath(argv[0])); for (auto &s : compileArgs)
  //      llvm::outs() << s << "\n";
  //
  //    auto xfrontendAction = std::make_unique<DBAnalysisFrontendAction>();
  //    utils::customRunToolOnCodeWithArgs(move(xfrontendAction), sourcetxt,
  //                                       compileArgs, sourceFile);
  //  }

  return 0;
}
