#pragma once
#include "DataFlow.h"

#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/CFG.h>

namespace dataflow {
    using namespace llvm;

    template <class T>
    class DCEVisitor: public InstVisitor<DCEVisitor<T>> {
        using FlowSet = flow_set<Value*>;
    private:
        T &context;
    public:
        FlowSet state;

        DCEVisitor(T &context, const FlowSet& state): context(context), state(state) {}

        void visitInstruction(Instruction&);

        FlowSet&& getState() { return std::move(state); }

        template <class C>
        static void postMeet(C&, BasicBlock&) {};
    };

    class DCEPass : public FunctionPass {
        using FlowSet = flow_set<Value*>;
    private:
        DataFlowPass<Function, DCEVisitor<Function>, Value*> analysis;
    public:
        static char ID;
        DCEPass(): FunctionPass(ID), analysis(MeetUnion<Value*>, backward) {}
        bool runOnFunction(Function&) override;
        void getAnalysisUsage(AnalysisUsage&) const override;

        const FlowSet getInState(const BasicBlock* bb) const {
            return analysis.getInState(bb);
        }

        const FlowSet getOutState(const BasicBlock* bb) const {
            return analysis.getOutState(bb);
        }



    };

}
