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

    errs() << "property: " << propertyName << " ";
    printLattice(lattice);
    errs() << "\n";

    auto *analyzer =
        new DataFlow(MeetIntersect<llvm::Value *>, direction, lattice);
    analyzers[propertyName] = std::unique_ptr<DataFlow>(analyzer);
  }

  const jsValue &procedures = annotations["procedures"];
  assert(procedures.IsArray());
  for (jsValue::ConstValueIterator procedure = procedures.Begin();
       procedure != properties.End(); ++procedure) {
    if (procedure->IsObject()) {
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
}
