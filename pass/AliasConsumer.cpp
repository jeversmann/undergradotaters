#include "DCEPass.h"
#include "AAPass.h"
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/AliasSetTracker.h>
#include <llvm/Support/raw_ostream.h>

namespace dataflow {

char DCEPass::ID = 2;

bool DCEPass::runOnFunction(Function &f) {
  AliasSetTracker *ASTracker = getAnalysis<AAPass>().AT;
  ASTracker->print(errs());
  delete ASTracker;
  return 0;
}

void DCEPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll(); // this is wrong
  AU.addRequired<AAPass>();
}

static RegisterPass<DCEPass> X("dce-pass", "Remove dead code from a function "
                                           "using faint variable analysis. "
                                           "(NOW SUPER BORKEN)",
                               true, true);
}
