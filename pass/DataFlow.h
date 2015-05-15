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

template <class T, class Transfer, class FlowValue = Instruction *>
class DataFlowPass {
private:
  using FlowSet = flow_set<FlowValue>;

  using AnalysisStates = std::map<const BasicBlock *, FlowSet>;
  using Meet = FlowSet (*)(FlowSet &, FlowSet &, const BasicBlock *);

  AnalysisStates inStates, outStates;
  Meet meet;
  Direction direction;
  FlowSet initial, boundary;

public:
  DataFlowPass(Meet meet, Direction dir = forward, FlowSet initial = FlowSet())
      : DataFlowPass(meet, initial, initial, dir) {}

  DataFlowPass(Meet meet, FlowSet initial, FlowSet boundary = FlowSet(),
               Direction dir = forward)
      : meet(meet), direction(dir), initial(initial), boundary(boundary) {}

  void applyMeet(FlowSet &start, FlowSet &end, const BasicBlock *node) {
    start = meet(start, end, node);
  }

  void initializeState(Loop &f, Worklist &worklist) {
    for (auto bb = f.block_begin(), end = f.block_end(); bb != end; ++bb) {
      worklist.insert(*bb);
      inStates[*bb] = FlowSet(initial);
      outStates[*bb] = FlowSet(initial);
    }
  }

  void initializeState(Function &f, Worklist &worklist) {
    for (auto &bb : f) {
      worklist.insert(&bb);
      inStates[&bb] = FlowSet(initial);
      outStates[&bb] = FlowSet(initial);
    }
  }

  bool run(T &f) {
    auto worklist = Worklist();

    initializeState(f, worklist);

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
      FlowSet start(startStates[node]);

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

      if (start != startStates[node])
        changed = true;

      if (!changed)
        continue;

      // apply the transfer functions to the node in the correct direction
      auto visitor = Transfer(f, startStates[node]);
      if (direction == forward)
        visitor.visit(node);
      else
        visitor.visit(node->rbegin(), node->rend());

      endStates[node] = visitor.getState();
      startStates[node].phiSets = endStates[node].phiSets;

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

    // Print analysis result
    example::DataFlowAnnotator<DataFlowPass> annotator(*this, errs());
    annotator.print(f);

    return false;
  }

  // to get printable states, get the values of the values
  const FlowSet getInState(const BasicBlock *bb) const {
    FlowSet values;
    if (inStates.find(bb) == inStates.end())
      return values;

    for (auto &inst : inStates.at(bb))
      values.insert(inst);
    // the in state includes the phi sets coming into the node
    for (const auto &p : inStates.at(bb).phiSets)
      for (auto &inst : p.second)
        values.insert(inst);
    return values;
  }
  const FlowSet getOutState(const BasicBlock *bb) const {
    FlowSet values;
    if (outStates.find(bb) == outStates.end())
      return values;

    for (auto &inst : outStates.at(bb))
      values.insert(inst);

    return values;
  }
};
}
