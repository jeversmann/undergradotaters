#ifndef CS380C_FMETAPASS_H
#define CS380C_FMETAPASS_H

#include <utility>
#include <map>
#include <set>

#include "llvm/Pass.h"

namespace cs380c {

class FMetaPass : public llvm::FunctionPass {
private:
  // Private fields go here
public:
  static char ID;
  FMetaPass() : llvm::FunctionPass(ID) {}

  bool runOnFunction(llvm::Function &);

  void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};
}

#endif
