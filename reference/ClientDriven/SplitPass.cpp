#include "SplitPass.h"

using namespace llvm;

namespace llvm_bdwy {
  bool SplitPass::runOnBasicBlock(BasicBlock &BB) {
    for(BasicBlock::iterator i = BB.begin(), e = BB.end(); i != e; i++) {
      if(isa<CallInst>(*i) && (++i) != e) {
        BB.splitBasicBlock(i);
        return true;
      }
    }
    return false;
  }
  char SplitPass::ID = 0;
  static RegisterPass<SplitPass> X("split-at-call", "Split basic blocks after function calls");
}

