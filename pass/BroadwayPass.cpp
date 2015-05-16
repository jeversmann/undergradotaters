#include "BroadwayPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>

namespace dataflow {
using namespace rapidjson;
using jsValue = rapidjson::Value;
template <class T> void BroadwayVisitor<T>::visitCallInst(CallInst &inst) {}

char BroadwayPass::ID = 4;

bool BroadwayPass::doInitialization(Module &m) {
  FILE *fp = fopen("memory.json", "r"); // non-Windows use "r"
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  annotations.ParseStream(is);
  fclose(fp);

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  StringBuffer buffer2;
  Writer<StringBuffer> writer2(buffer2);

  assert(annotations.IsObject());
  const jsValue &properties = annotations["properties"];
  assert(properties.IsArray());
  for (jsValue::ConstValueIterator property = properties.Begin();
       property != properties.End(); ++property) {
    assert(property->IsObject());
    // do stuff with properties
  }

  const jsValue &procedures = annotations["procedures"];
  assert(procedures.IsArray());
  for (jsValue::ConstValueIterator procedure = procedures.Begin();
       procedure != properties.End(); ++procedure) {
    if (procedure->IsObject()) {
      errs() << procedure->GetType() << "\n";
      assert(procedure->IsObject());
      const jsValue &name = (*procedure)["name"];
      assert(name.IsString());
      procedureAnnotations[name.GetString()] = procedure;
    }
  }

  return false;
}

bool BroadwayPass::runOnFunction(Function &f) {
  auto changed = false;

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
