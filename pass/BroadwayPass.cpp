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

template <class Pass, class T>
void BroadwayVisitor<Pass, T>::visitCallInst(CallInst &inst) {
  if (const auto &function = inst.getCalledFunction()) {
    if (pass.procedures.find(function->getName()) != pass.procedures.end()) {
      auto &procedure = *pass.procedures[function->getName()];
      for (auto &analysis : procedure.analyses) {
        auto &property = analysis.name;
        if (property != state.name)
          continue;

        for (auto &rule : analysis.rules) {
          if (rule.condition.op == "") { // || evaluate condition to true
            for (auto &effect : rule.effects) {
              if (effect.op == "<-" || effect.op == "<-+") {
                auto &variable = effect.lhs->name;
                auto &propname = effect.rhs->name;

                llvm::Value *arg;
                for (auto &entry : procedure.entryPointers) {
                  auto *pointerDef = entry.findDefinition(propname);
                  if (pointerDef) {
                    auto *topDef = pointerDef->getTopParent();
                    auto argNum =
                        getArgumentForPointer(topDef->name, procedure);
                    if (argNum != -1) {
                      arg = inst.getArgOperand(argNum)->stripPointerCasts();
                      break;
                    }
                  }

                  if (arg) {
                    errs() << "------adding: " << propname << " " << *arg
                           << "\n";
                    state.addToProperty(propname, arg);
                    printLattice(state);
                  }
                }
              }
            }
          }
        }
        // do stuff
      }
    }
  }
}

template <class Pass, class T>
int BroadwayVisitor<Pass, T>::getArgumentForPointer(
    const std::string &variable, BroadwayProcedure &procedure) {
  std::vector<std::string>::iterator ptr = std::find(
      procedure.arguments.begin(), procedure.arguments.end(), variable);
  if (ptr != procedure.arguments.end()) {
    return (int)(ptr - procedure.arguments.begin());
  }
  return -1;
}

char BroadwayPass::ID = 4;

bool BroadwayPass::doInitialization(Module &m) {
  FILE *fp = fopen("test.json", "r"); // non-Windows use "r"
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  annotations.ParseStream(is);
  fclose(fp);

  assert(annotations.IsObject());

  processPropertyAnnotations();
  processProcedureAnnotations();

  return false;
}

bool BroadwayPass::runOnFunction(Function &f) {
  auto changed = false;

  // Get all the names of all the things
  for (auto &bbl : f.getBasicBlockList())
    for (auto &inst : bbl)
      instructions.insert(&inst);
  for (auto &arg : f.getArgumentList())
    instructions.insert(&arg);

  for (auto &analyzerPair : analyzers)
    analyzerPair.second->initializeState(f);

  for (auto &analyzerPair : analyzers) {
    changed = analyzerPair.second->run(f) || changed;
  }

  // example::DataFlowAnnotator<BroadwayPass> annotator(*this, errs());
  // annotator.print(f);

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
    lattice.name = propertyName;
    const auto &values = property["lattice"];
    assert(values.IsArray());
    for (jsValue::ConstValueIterator latticeIter = values.Begin();
         latticeIter != values.End(); ++latticeIter) {
      auto &latticeValue = *latticeIter;
      if (latticeValue.IsObject()) {
        assert(latticeValue.IsObject());
        assert(latticeValue["name"].IsString());
        const auto &valueParent = latticeValue.HasMember("parent") &&
                                          latticeValue["parent"].IsString()
                                      ? latticeValue["parent"].GetString()
                                      : "bottom";
        const auto &valueName = latticeValue["name"].GetString();
        lattice.addProperty(valueName, valueParent);
        errs() << latticeValue["name"].GetString();
      }
    }

    if (property.HasMember("initial") && property["initial"].IsString())
      lattice.initial = property["initial"].GetString();

    analyzers[propertyName] =
        new DataFlow(*this, MeetIntersect<llvm::Value *>, direction, lattice);
  }
}

void BroadwayPass::processProcedureAnnotations() {
  const jsValue &procs = annotations["procedures"];
  assert(procs.IsArray());
  for (jsValue::ConstValueIterator procIter = procs.Begin();
       procIter != procs.End(); ++procIter) {
    auto &procedure = *procIter;
    if (procedure.IsObject()) {
      procedures[procedure["name"].GetString()] =
          new BroadwayProcedure(procedure);
    }
  }
}
}
