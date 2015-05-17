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
  outs() << "Starting Function Pass"
         << "\n";
  AA = &getAnalysis<AliasAnalysis>();
  AT = new AliasSetTracker(*AA);
  /*for(auto const &block : f) {
      for(auto const &inst : block) {
          //std::cout<< AA->getModRefInfo(&inst) << std::endl;
      }
  }*/
  for (auto const &mySet : AT->getAliasSets()) {
    for (auto const &val : mySet) {
      outs() << val.getValue()->getName() << "\n";
    }
  }
  outs() << "After first printed set"
         << "\n";
  for (auto &block : f) {
    for (auto &inst : block) {
      AA->deleteValue(&inst);
      if (auto callinst = dyn_cast<CallInst>(&inst)) {
        if (auto calledFunc = callinst->getCalledFunction()) {
          outs() << calledFunc->getName() << "\n";
          if (calledFunc->getName() == "printf") {
            AA->deleteValue(calledFunc);
          }
        }
      }
    }
  }
  outs() << "After deletions"
         << "\n";
  for (auto const &mySet : AT->getAliasSets()) {
    for (auto const &val : mySet) {
      AA->deleteValue(val.getValue());
      outs() << val.getValue()->getName() << "\n";
    }
  }
  std::cout << "Hello World" << std::endl;
  delete AT;
  return 0;
}

void AAPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  AU.addRequired<AliasAnalysis>();
}

void AAPass::initializeAAPassPass(PassRegistry &Registry) {}

char AAPass::ID = 0;
static RegisterPass<AAPass> X("myaa", "Example Alias Analysis Pass");
static RegisterAnalysisGroup<AliasAnalysis> A("Alias Analysis");

namespace llvm {
INITIALIZE_AG_PASS(AAPass, AliasAnalysis, "myaa",
                   "A more complex alias analysis implementation",
                   false, // Is CFG Only?
                   true,  // Is Analysis?
                   false) // Is default Analysis Group implementation?
}
