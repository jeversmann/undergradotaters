#define DEBUG_TYPE "FMetaPass"

#include "FMetaPass.h"

#include <llvm/IR/Function.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>

using namespace cs380c;
using namespace llvm;

bool FMetaPass::runOnFunction(Function &f) {
  errs() << f.hasMetadata() << f;
  return true;
}

void FMetaPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
}

char FMetaPass::ID = 0;
static RegisterPass<FMetaPass>
    X("fmeta-pass",
      "Messes with Function metadata");
