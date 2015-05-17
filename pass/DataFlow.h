#pragma once
#include "DataFlowAnnotator.h"
#include "Utils.h"

#include <llvm/Pass.h>
#include <llvm/Analysis/LoopPass.h>
#include <llvm/IR/InstVisitor.h>
#include <unordered_set>
#include <unordered_map>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SetVector.h>

namespace dataflow {
using namespace llvm;

enum Direction { backward = 0, forward = 1 };

using Worklist = SetVector<BasicBlock *>;

template <class T, class Transfer, class Pass, class FlowValue = Instruction *>
class DataFlowPass {
private:
  using Lattice = BroadwayLattice<FlowValue>;
  using FlowSet = flow_set<FlowValue>;

  using AnalysisStates = std::map<const BasicBlock *, Lattice>;
  using Meet = Lattice (*)(Lattice &, Lattice &, const BasicBlock *);

  AnalysisStates inStates, outStates;
  Meet meet;
  Direction direction;
  Worklist worklist;

public:
  Lattice initial, boundary;
  Pass &pass;

  DataFlowPass(Pass &pass, Meet meet, Direction dir = forward,
               Lattice initial = Lattice())
      : DataFlowPass(pass, meet, dir, initial, initial) {}

  DataFlowPass(Pass &pass, Meet meet, Direction dir, Lattice initial,
               Lattice boundary)
      : meet(meet), direction(dir), initial(initial), boundary(boundary),
        pass(pass) {}

  void applyMeet(Lattice &start, Lattice &end, const BasicBlock *node) {
    start = meet(start, end, node);
  }

  void initializeState(Loop &f) {
    for (auto bb = f.block_begin(), end = f.block_end(); bb != end; ++bb) {
      worklist.insert(*bb);
      inStates[*bb] = Lattice(initial);
      outStates[*bb] = Lattice(initial);
    }
  }

  void initializeState(Function &f) {
    /* printLattice(initial); */
    errs() << initial.initial << "\n";
    std::unordered_set<llvm::Value *> instructions;
    for (auto &bb : f)
      for (auto &inst : bb)
        instructions.insert(&inst);
    for (auto &inst : instructions)
      initial.addToProperty(initial.initial, inst->stripPointerCasts());
    for (auto &bb : f) {
      worklist.insert(&bb);
      inStates[&bb] = Lattice(initial);
      outStates[&bb] = Lattice(initial);
    }
  }

  bool run(T &f) {
    auto superChanged = false;
    initializeState(f);

    // keep track of the first time we visit each node
    std::unordered_set<BasicBlock *> seen;

    while (!worklist.empty()) {
      auto node = worklist.back();
      worklist.pop_back();

      // set the positions of in and out in the dataflow equations
      auto &startStates = direction == forward ? inStates : outStates;
      auto &endStates = direction == forward ? outStates : inStates;

      auto changed = false;

      if (seen.find(node) == seen.end()) {
        seen.insert(node);
        changed = true;
      }

      // save the old value to see if it changes
      Lattice start(startStates[node]);

      // apply the meet operator over the pred/succ nodes
      if (direction == forward) {
        for (auto itr = pred_begin(node), ite = pred_end(node); itr != ite;
             ++itr) {
          applyMeet(startStates[node], endStates[*itr], node);
          Transfer::postMeet(f, *node);
        }

      } else {
        for (auto itr = succ_begin(node), ite = succ_end(node); itr != ite;
             ++itr) {
          applyMeet(startStates[node], endStates[*itr], node);
          Transfer::postMeet(f, *node);
        }
      }

      if (start != startStates[node]) {
        changed = true;
        superChanged = true;
      }

      if (!changed)
        continue;

      // apply the transfer functions to the node in the correct direction
      auto visitor = Transfer(pass, f, startStates[node]);
      if (direction == forward)
        visitor.visit(node);
      else
        visitor.visit(node->rbegin(), node->rend());

      endStates[node] = visitor.getState();

      for (auto &prop : initial.properties) {
        FlowSet updatedPhi(startStates[node].get(prop));
        updatedPhi.phiSets = endStates[node].get(prop).phiSets;
        startStates[node].put(prop, updatedPhi);
      }

      // add the pred/succ to the worklist
      if (direction == forward)
        for (auto itr = succ_begin(node), ite = succ_end(node); itr != ite;
             ++itr)
          worklist.insert(*itr);
      else
        for (auto itr = pred_begin(node), ite = pred_end(node); itr != ite;
             ++itr)
          worklist.insert(*itr);
    }

    return superChanged;
  }

  // to get printable states, get the values of the values
  const Lattice getInState(const BasicBlock *bb) const {
    Lattice values = initial.clone();
    if (inStates.find(bb) == inStates.end())
      return values;

    for (auto &prop : initial.properties)
      for (auto &inst : inStates.at(bb).get(prop))
        values.addToProperty(prop, inst);
    // the in state includes the phi sets coming into the node
    // This is probably ugly
    for (auto &prop : initial.properties)
      for (const auto &p : inStates.at(bb).get(prop).phiSets)
        for (auto &inst : p.second)
          values.addToProperty(prop, inst);
    return values;
  }
  const Lattice getOutState(const BasicBlock *bb) const {
    Lattice values = initial.clone();
    if (outStates.find(bb) == outStates.end())
      return values;

    for (auto &prop : initial.properties)
      for (auto &inst : outStates.at(bb).get(prop))
        values.addToProperty(prop, inst);

    return values;
  }
};
}
