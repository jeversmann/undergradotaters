#ifndef CS380C_FMETAPASS_H
#define CS380C_FMETAPASS_H

#include <utility>
#include <map>
#include <set>

#include <llvm/Pass.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/AliasSetTracker.h>
#include <llvm/Support/raw_ostream.h>

namespace cs380c {

class AAPass : public llvm::FunctionPass, public llvm::AliasAnalysis {
private:
  // Private fields go here
  llvm::AliasSetTracker *AT;

public:
  static char ID;
  AAPass() : llvm::FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &);

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  void initializeAAPassPass(llvm::PassRegistry &Registry);

  void *getAdjustedAnalysisPointer(const void *ID) override {
    llvm::errs() << "HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    if (ID == &AliasAnalysis::ID)
      return (AliasAnalysis *)this;
    return this;
  }

  AliasAnalysis::AliasResult alias(const Location &LocA,
                                   const Location &LocB) override {
    if (auto callinst = llvm::dyn_cast<llvm::CallInst>(LocA.Ptr))
      if (callinst->getArgOperand(0) == LocB.Ptr)
        return MustAlias;
    if (auto callinst = llvm::dyn_cast<llvm::CallInst>(LocB.Ptr))
      if (callinst->getArgOperand(0) == LocA.Ptr)
        return MustAlias;
    return AliasAnalysis::alias(LocA, LocB);
  }
};
}

namespace llvm {
void initializeAAPassPass(PassRegistry &);
}

#endif
