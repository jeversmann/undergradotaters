#include "BroadwayPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>

namespace dataflow {
using namespace rapidjson;

template <class T> void BroadwayVisitor<T>::visitCallInst(CallInst &inst) {}

char BroadwayPass::ID = 4;

bool BroadwayPass::doInitialization(Module &m) {
  FILE *fp = fopen("memory.json", "r"); // non-Windows use "r"
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  Document d;
  d.ParseStream(is);
  fclose(fp);

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  StringBuffer buffer2;
  Writer<StringBuffer> writer2(buffer2);

  d["properties"].Accept(writer);
  std::cout << buffer.GetString() << std::endl;
  d["procedures"].Accept(writer2);
  std::cout << buffer2.GetString() << std::endl;
  return false;
}

bool BroadwayPass::runOnFunction(Function &f) {
  auto changed = false;

  // Get all the names of all the things
  for (auto const &bbl : f.getBasicBlockList()) {
    for (auto const &inst : bbl.getInstList()) {
      if (!isa<TerminatorInst>(inst)) {
        /* names.insert(inst.getName()); */
      }
      for (unsigned int i = 0; i < inst.getNumOperands(); i++) {
        llvm::Value *v = inst.getOperandUse(i);
        if (isa<Instruction>(v) || isa<Argument>(v)) {
          /* names.insert(v->getName()); */
        }
      }
    }
  }

  analysis = DataFlow(MeetUnion<llvm::Value *>, backward); // Lattice

  analysis.run(f);

  example::DataFlowAnnotator<BroadwayPass> annotator(*this, errs());
  annotator.print(f);

  return changed;
}

void BroadwayPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll(); // this is wrong
}

static RegisterPass<BroadwayPass>
    X("broadway-pass", "Run analysis using Broadway annotations.", true, true);
}
