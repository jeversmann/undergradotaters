#ifndef LLVM_BDWY_ANN_PASS
#define LLVM_BDWY_ANN_PASS

#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "broadway.h"
#include "c_breeze.h"

namespace llvm_bdwy {
  
  class AnnotationPass;
  
}
  
#include "ProcedureAnnotationInfo.h"
#include "AnnotationAnalysis.h"

namespace llvm_bdwy {
  
  using namespace llvm;
  using namespace std;
  using namespace llvm_bdwy;
  
  class AnnotationPass : public ModulePass {
  public:
    Annotations* annotations;
    ProcedureAnnotationInfo* procAnnInfo;
    vector<AnnotationAnalysis> propAnalyses;
    bool ready;
    
    static char ID;
    AnnotationPass() : ModulePass(ID), ready(false) {}

    bool runOnModule(Module &M);
  };

}

#endif
