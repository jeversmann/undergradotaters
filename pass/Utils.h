#pragma once
#include <llvm/Pass.h>
#include <llvm/IR/InstVisitor.h>
#include <unordered_set>
#include <unordered_map>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SetVector.h>

namespace dataflow {
    using namespace llvm;

    // a wrapper for the flow sets, it includes sets for each incoming edge in a
    // phi function so that they can be handled differently
    template <class T>
    class flow_set : public std::unordered_set<T>{
    public:
        std::unordered_map<const BasicBlock*, std::unordered_set<T>> phiSets;
        bool universal;
        flow_set(bool universal=false) : std::unordered_set<T>(), universal(universal) {}
    };


    template <class FlowValue>
    flow_set<FlowValue> MeetUnion(flow_set<FlowValue>& first, flow_set<FlowValue>& second, const BasicBlock* from) {
        // take the union of the sets
        flow_set<FlowValue> result(first);
        for (auto &e : second)
            result.insert(e);

        // push the phi set for this node into the result set
        for (auto e : second.phiSets[from])
            result.insert(e);
        
        result.universal = first.universal || second.universal;
        
        return result;
    }

    template <class FlowValue>
    flow_set<FlowValue> MeetIntersect(flow_set<FlowValue>& first, flow_set<FlowValue>& second, const BasicBlock* from) {
        flow_set<FlowValue> result(first);
        result.clear();
        result.phiSets[from].clear();

        // if either is universal, just take the other one
        if (first.universal) {
            result.insert(second.begin(), second.end());
            result.phiSets[from].insert(second.phiSets[from].begin(), second.phiSets[from].end());
        } else if (second.universal) {
            result.insert(first.begin(), first.end());
            result.phiSets[from].insert(first.phiSets[from].begin(), first.phiSets[from].end());
        } else {
            // if neither is universal, intersect of the sets
            for (auto &e : first)
                if (second.find(e) != second.end())
                    result.insert(e);

            // intersect the phi set for this node
            for (auto e : first.phiSets[from])
                if (second.phiSets[from].find(e) != second.phiSets[from].end())
                    result.insert(e);
        }
        
        result.universal = first.universal && second.universal;
        
        return result;
    }

    template <class FlowValue>
    void printSet(flow_set<FlowValue> set, bool printPhi=false) {
        errs() << "{\n";
        for (auto &e : set) {
            errs() << " " << e->getName();
        }
        errs() << "\n}\n";

        if (printPhi) {
            errs() << "phiSets {\n";
            for (auto &p : set.phiSets) {
                errs() << "  " << p.first->getName() << ":";
                for (auto &e : p.second)
                    errs() << " " << e->getName();
                errs() << "\n";
            }
            errs() << "}\n";
        }
    }
}
