#pragma once
#include "DataFlow.h"

#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/CFG.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/AliasSetTracker.h>
#include "rapidjson/document.h"
#include "BroadwayAST.h"

namespace dataflow {
using namespace llvm;
using Lattice = BroadwayLattice<llvm::Value *>;

template <class Pass, class T>
class BroadwayVisitor : public InstVisitor<BroadwayVisitor<Pass, T>> {

private:
  T &context;
  Pass &pass;

public:
  Lattice state;

  BroadwayVisitor(Pass &pass, T &context, const Lattice &state)
      : context(context), pass(pass), state(state) {}

  void visitCallInst(CallInst &);

  int getArgumentForPointer(const std::string &, BroadwayProcedure &);

  Lattice &&getState() { return std::move(state); }

  template <class C> static void postMeet(C &, BasicBlock &) {}
};

class BroadwayPass : public FunctionPass, public AliasAnalysis {
  using FlowSet = flow_set<llvm::Value *>;
  using DataFlow =
      DataFlowPass<Function, BroadwayVisitor<BroadwayPass, Function>,
                   BroadwayPass, llvm::Value *>;

private:
  llvm::AliasSetTracker *AT;
  void processPropertyAnnotations();
  void processProcedureAnnotations();
  void processCallInstPointers(CallInst &inst);

public:
  static char ID;
  rapidjson::Document annotations;
  std::unordered_map<std::string, BroadwayProcedure *> procedures;
  std::unordered_map<std::string, DataFlow *> analyzers;
  std::unordered_set<llvm::Value *> instructions;
  BroadwayPass() : FunctionPass(ID) {}
  bool doInitialization(Module &) override;
  bool runOnFunction(Function &) override;
  void getAnalysisUsage(AnalysisUsage &) const override;

  const Lattice getInState(const BasicBlock *bb) const { return Lattice(); }

  const Lattice getOutState(const BasicBlock *bb) const { return Lattice(); }

  // AliasAnalysis things
  void *getAdjustedAnalysisPointer(const void *ID) override {
    if (ID == &AliasAnalysis::ID)
      return (AliasAnalysis *)this;
    return this;
  }

  AliasAnalysis::AliasResult
  alias(const AliasAnalysis::Location &LocA,
        const AliasAnalysis::Location &LocB) override;

  void initializeBroadwayPassPass(llvm::PassRegistry &Registry) {}
};
}

namespace llvm {
void initializeBroadwayPassPass(PassRegistry &);
}
