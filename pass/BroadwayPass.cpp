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

  assert(annotations.IsObject());

  processPropertyAnnotations();
  processProcedureAnnotations();
  processAnalysisAnnotations();
  processActionAnnotations();
  processReportAnnotations();

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

  // analysis = DataFlow(MeetUnion<llvm::Value *>, backward); // Lattice

  // analysis.run(f);

  example::DataFlowAnnotator<BroadwayPass> annotator(*this, errs());
  annotator.print(f);

  return changed;
}

void BroadwayPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll(); // this is wrong
}

static RegisterPass<BroadwayPass>
    X("broadway-pass", "Run analysis using Broadway annotations.", true, true);

void BroadwayPass::processPropertyAnnotations() {
  const auto &properties = annotations["properties"];
  assert(properties.IsArray());
  for (jsValue::ConstValueIterator iter = properties.Begin();
       iter != properties.End(); ++iter) {
    auto &property = *iter;
    assert(property.IsObject());
    assert(property["name"].IsString());
    const auto &propertyName = property["name"].GetString();
    auto direction = property["direction"] == "backward" ? backward : forward;

    Lattice lattice;
    const auto &values = property["lattice"];
    assert(values.IsArray());
    for (jsValue::ConstValueIterator latticeIter = properties.Begin();
         latticeIter != properties.End(); ++latticeIter) {
      auto &latticeValue = *latticeIter;
      assert(latticeValue.IsObject());
      assert(latticeValue["name"].IsString());
      const auto &valueParent = latticeValue.HasMember("parent")
                                    ? latticeValue["parent"].GetString()
                                    : "bottom";
      const auto &valueName = latticeValue["name"].GetString();
      lattice.addProperty(valueName, valueParent);
    }

    if (property.HasMember("initial") && property["initial"].IsString())
      lattice.initial = property["initial"].GetString();

    auto *analyzer =
        new DataFlow(MeetIntersect<llvm::Value *>, direction, lattice);
    analyzers[propertyName] = std::unique_ptr<DataFlow>(analyzer);
  }
}

void BroadwayPass::processProcedureAnnotations() {
  const jsValue &procs = annotations["procedures"];
  assert(procs.IsArray());
  for (jsValue::ConstValueIterator procIter = procs.Begin();
       procIter != procs.End(); ++procIter) {
    auto &procedure = *procIter;
    if (procedure.IsObject()) {
      auto *proc = new BroadwayProcedure(procedure);
      procedures[proc->name] = std::unique_ptr<BroadwayProcedure>(proc);
    }
  }
}

void BroadwayPass::processAnalysisAnnotations() {}

void BroadwayPass::processActionAnnotations() {}

void BroadwayPass::processReportAnnotations() {}
}
