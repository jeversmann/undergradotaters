#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "broadway.h"
#include "c_breeze.h"
#include <string>
#include <iostream>

#include "AnnotationPass.h"

#define DEBUG_ANNOT 1

using namespace llvm;
using namespace std;
using namespace llvm_bdwy;

// Grab Command Line Arguments
// -pal Filename
static cl::opt<string> AnnotationFilename("pal", cl::desc("Specify annotation filename"),
  cl::value_desc("filename"), cl::Optional);

// Primary Pass Driver
bool AnnotationPass::runOnModule(Module& M) {
  if (AnnotationFilename.size() == 0) {
    // don't do anything, then
    return false;
  } else {
    ready = true;
  }
  
  if(DEBUG_ANNOT) {
    ferrs() << "Annotations File: " << AnnotationFilename.c_str() << "\n\n";
  }
  
  string fileName(AnnotationFilename.c_str());
  
  // Command Line Arguments to pass to Annotations
  str_list* args = new str_list();
  CBZ::cpp_flags = args;
  CBZ::cc_cmd = "gcc";
  
  annotations = new Annotations(fileName, args, &M);
  procAnnInfo = new ProcedureAnnotationInfo(annotations);

  // Simple Annotations Print Test
  
  
  annotations->print(std::cerr);
  /*
  // Testing ProcedureAnnotationInfo
  procAnnInfo->printEntryRules("strcpy", cout);
  procAnnInfo->printExitRules("strcpy", cout);
  procAnnInfo->printUses("strcpy", cout);
  procAnnInfo->printDefs("strcpy", cout);*/
  
  const enum_property_map& properties = annotations->enum_properties();
  
  for (enum_property_map::const_iterator b = properties.begin(); b != properties.end(); b++) {
    
    if (b->second->name() != "constants")
      propAnalyses.push_back(AnnotationAnalysis(this, b->second));
    
    /*enumPropertyAnn* property = b->second;
    
    if (property->name() == "constants") continue;
    
    const rule_list& rules = procAnnInfo->getProcAnalysisRules("read", property);
    for(rule_list::const_iterator it = rules.begin(); it != rules.end(); it++) {
      
      const exprann_list& effects = (*it)->effects();
      exprann_list::const_iterator expr_it;
      for(expr_it = effects.begin(); expr_it != effects.end(); expr_it++) {
	
	exprAnn* expr = *expr_it;
	
	enumPropertyExprAnn* enumExpr = reinterpret_cast<enumPropertyExprAnn*>(expr);
    
	if (enumExpr) {
	  string lhs = enumExpr->lhs_name();
	  string rhs = enumExpr->rhs_name();
	  cout << "LHS: " << lhs << "\tRHS: " << rhs << endl;
	}
      }
    }*/
  
  }

  return 0;
}

// Pass Registration
char AnnotationPass::ID = 0;
static RegisterPass<AnnotationPass> X("annotation", "Annotation Pass");
