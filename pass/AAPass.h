#ifndef CS380C_FMETAPASS_H
#define CS380C_FMETAPASS_H

#include <utility>
#include <map>
#include <set>

#include <llvm/Pass.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/AliasSetTracker.h>

namespace cs380c {

class AAPass : public llvm::FunctionPass, public llvm::AliasAnalysis {
private:
  // Private fields go here
  llvm::AliasAnalysis *AA;
  llvm::AliasSetTracker *AT;

public:
  static char ID;
  AAPass() : llvm::FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &);

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  void initializeAAPassPass(llvm::PassRegistry &Registry);
};
}

namespace llvm {
  void initializeAAPassPass(PassRegistry&);
}

#endif
