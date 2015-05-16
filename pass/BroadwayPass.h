#pragma once
#include "DataFlow.h"

#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/CFG.h>

namespace dataflow {
using namespace llvm;
using Lattice = BroadwayLattice<Value *>;

template <class T> class BroadwayVisitor : public InstVisitor<BroadwayVisitor<T>> {

private:
  T &context;

public:
  Lattice state;

  BroadwayVisitor(T &context, const Lattice &state)
      : context(context), state(state) {}

  void visitCallInst(CallInst &);

  Lattice &&getState() { return std::move(state); }

  template <class C> static void postMeet(C &, BasicBlock &) {}
};

class BroadwayPass : public FunctionPass {
  using FlowSet = flow_set<Value *>;

private:
  DataFlowPass<Function, BroadwayVisitor<Function>, Value *> analysis;

public:
  static char ID;
  BroadwayPass()
    : FunctionPass(ID),
      analysis(MeetUnion<Value *>,
               BroadwayLattice<Value *>().addProperty("dce"), backward) {}
  bool doInitialization(Module &) override;
  bool runOnFunction(Function &) override;
  void getAnalysisUsage(AnalysisUsage &) const override;

  const FlowSet getInState(const BasicBlock *bb) const {
    return analysis.getInState(bb).get("dce");
  }

  const FlowSet getOutState(const BasicBlock *bb) const {
    return analysis.getOutState(bb).get("dce");
  }
};
}
