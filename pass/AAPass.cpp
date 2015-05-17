#define DEBUG_TYPE "AAPass"

#include "AAPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>

#include <llvm/IR/Function.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/CallSite.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/AliasSetTracker.h>

using namespace cs380c;
using namespace llvm;
using namespace rapidjson;

bool AAPass::runOnFunction(Function &f) {
  InitializeAliasAnalysis(this, DL);
  outs() << "====================== " << f.getName()
         << " ======================"
         << "\n";
  AT = new AliasSetTracker(*this);

  for (auto &block : f) {
    for (auto &inst : block) {
      if (auto callinst = dyn_cast<CallInst>(&inst)) {
        deleteValue(callinst);
        if (auto calledFunc = callinst->getCalledFunction()) {
          outs() << calledFunc->getName() << "\n";
        }
      } else {
        AT->add(&inst);
      }
    }
  }

  AT->print(errs());
  delete AT;
  return 0;
}

void AAPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AliasAnalysis::getAnalysisUsage(AU);
  AU.setPreservesCFG();
  AU.addRequiredTransitive<AliasAnalysis>();
}

void AAPass::initializeAAPassPass(PassRegistry &Registry) {}

char AAPass::ID = 0;
static RegisterPass<AAPass> X("myaa", "Example Alias Analysis Pass");
static RegisterAnalysisGroup<AliasAnalysis> A("Alias Analysis");

INITIALIZE_AG_PASS(AAPass, AliasAnalysis, "myaa",
                   "A more complex alias analysis implementation",
                   false, // Is CFG Only?
                   true,  // Is Analysis?
                   true)  // Is default Analysis Group implementation?
