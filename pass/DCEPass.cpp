#include "DCEPass.h"

namespace dataflow {

template <class T> void DCEVisitor<T>::visitInstruction(Instruction &inst) {
  bool found = false;
  for (auto &op : inst.operands()) {
    if (auto i = dyn_cast<Instruction>(op)) {
      if (i == &inst)
        found = true;
    }
  }

  for (auto &op : inst.operands()) {
    if (auto i = dyn_cast<Instruction>(op)) {
      // gen the operands if it is used
      if (inst.mayHaveSideEffects() || inst.isTerminator())
        state.insert(i);

      // gen the operands if the result is not faint
      if (state.find(&inst) != state.end())
        state.insert(i);
    }
  }

  // kill the defined value if it is not in the operands
  if (!found)
    state.erase(&inst);
}

char DCEPass::ID = 2;

bool DCEPass::runOnFunction(Function &f) {
  auto changed = false;

  analysis.run(f);

  std::set<Instruction *> toRemove;

  for (auto &block : f) {
    auto state = FlowSet(analysis.getOutState(&block));

    // iterate over the instructions, applying the transfer function and
    // removing it if possible
    auto visitor = DCEVisitor<Function>(f, state);
    for (auto inst = block.rbegin(), end = block.rend(); inst != end; ++inst) {
      // remove it if it isn't live
      if (state.find(&*inst) == state.end() && !inst->isTerminator() &&
          !inst->mayHaveSideEffects()) {
        toRemove.insert(&*inst);
        changed = true;
      }
      // update the state to the next program point
      visitor.visitInstruction(*inst);
      state = visitor.state;
    }
  }

  for (auto &inst : toRemove) {
    inst->dropAllReferences();
    inst->removeFromParent();
  }

  example::DataFlowAnnotator<DCEPass> annotator(*this, errs());
  annotator.print(f);

  return changed;
}

void DCEPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll(); // this is wrong
}

static RegisterPass<DCEPass>
    X("dce-pass",
      "Remove dead code from a function using faint variable analysis.", true,
      true);
}
