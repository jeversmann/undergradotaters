#include "llvm/Pass.h"
#include "llvm/BasicBlock.h"
#include "llvm/Instructions.h"

using namespace llvm;

namespace llvm_bdwy {
  struct SplitPass : public BasicBlockPass {
    public:
    static char ID;
    SplitPass() : BasicBlockPass(ID) {}
    virtual bool runOnBasicBlock(BasicBlock &BB);
  };
}
