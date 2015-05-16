#include "BroadwayPass.h"

namespace dataflow {

template <class T> void BroadwayVisitor<T>::visitCallInst(CallInst &inst) {

}

char BroadwayPass::ID = 4;

bool BroadwayPass::doInitialization(Module &m) {
  return false;
}

bool BroadwayPass::runOnFunction(Function &f) {
  auto changed = false;

  analysis.run(f);

  example::DataFlowAnnotator<BroadwayPass> annotator(*this, errs());
  annotator.print(f);

  return changed;
}

void BroadwayPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll(); // this is wrong
}

static RegisterPass<BroadwayPass>
    X("broadway-pass",
      "Run analysis using Broadway annotations.", true,
      true);
}
