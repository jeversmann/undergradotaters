#pragma once
#include "DataFlow.h"

#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/CFG.h>

namespace dataflow {
using namespace llvm;

class DCEPass : public FunctionPass {
  using FlowSet = flow_set<Value *>;

private:
public:
  static char ID;
  DCEPass() : FunctionPass(ID) {}
  bool runOnFunction(Function &) override;
  void getAnalysisUsage(AnalysisUsage &) const override;
};
}
