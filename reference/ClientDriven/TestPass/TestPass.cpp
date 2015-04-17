#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "broadway.h"
#include "c_breeze.h"
#include <string>
#include <iostream>
#include "../AnnotationPass/AnnotationPass.h"

#define DEBUG 1

using namespace llvm;
using namespace std;

namespace bdwy {
  
  class TestPass : public llvm::ModulePass {
  public:
    static char ID;
    TestPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module &M);
    
    void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.addRequired<AnnotationPass>();
    }

  };

  // Primary Pass Driver
  bool TestPass::runOnModule(Module& M) {
    
    AnnotationPass& AP = getAnalysis<AnnotationPass>();
    
    AP.annotations->print(std::cout);
    
    return 0;
  }

  // Pass Registration
  char TestPass::ID = 0;
  static RegisterPass<TestPass> X("test", "Test Pass");

}

