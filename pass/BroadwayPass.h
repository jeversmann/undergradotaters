#pragma once
#include "DataFlow.h"

#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/CFG.h>
#include "rapidjson/document.h"
#include "BroadwayAST.h"

namespace dataflow {
using namespace llvm;
using Lattice = BroadwayLattice<llvm::Value *>;

template <class T>
class BroadwayVisitor : public InstVisitor<BroadwayVisitor<T>> {

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
  using FlowSet = flow_set<llvm::Value *>;
  using DataFlow =
      DataFlowPass<Function, BroadwayVisitor<Function>, llvm::Value *>;

private:
  rapidjson::Document annotations;
  std::unordered_map<std::string, std::unique_ptr<BroadwayProcedure>>
      procedures;
  std::unordered_map<std::string, std::unique_ptr<DataFlow>> analyzers;

  void processPropertyAnnotations();
  void processProcedureAnnotations();
  void processAnalysisAnnotations();
  void processActionAnnotations();
  void processReportAnnotations();

public:
  static char ID;
  BroadwayPass() : FunctionPass(ID) {}
  bool doInitialization(Module &) override;
  bool runOnFunction(Function &) override;
  void getAnalysisUsage(AnalysisUsage &) const override;

  const Lattice getInState(const BasicBlock *bb) const { return Lattice(); }

  const Lattice getOutState(const BasicBlock *bb) const { return Lattice(); }
};
}
