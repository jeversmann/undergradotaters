#include "BroadwayPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>

using namespace dataflow;
using namespace rapidjson;
using jsValue = rapidjson::Value;

template <class Pass, class T>
void BroadwayVisitor<Pass, T>::visitCallInst(CallInst &inst) {
  if (const auto &function = inst.getCalledFunction()) {
    if (pass.procedures.find(function->getName()) != pass.procedures.end()) {
      auto &procedure = pass.procedures[function->getName()];
      for (auto &analysis : procedure->analyses) {
        auto &property = analysis.name;
        if (property != state.name)
          continue;

        for (auto &rule : analysis.rules) {
          if (rule.condition.op == "") { // || evaluate condition to true
            for (auto &effect : rule.effects) {
              if (effect.op == "<-" || effect.op == "<-+") {
                auto &variable = effect.lhs->name;
                auto &propname = effect.rhs->name;
                std::vector<std::string>::iterator ptr =
                    std::find(procedure->arguments.begin(),
                              procedure->arguments.end(), variable);
                if (ptr != procedure->arguments.end()) {
                  int argNum = (int)(ptr - procedure->arguments.begin());
                  errs() << "------adding: " << propname << " "
                         << *inst.getArgOperand(argNum) << "\n";
                  state.addToProperty(propname, inst.getArgOperand(argNum)
                                                    ->stripPointerCasts());
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
  // Start by doing Alias Analysis things
  InitializeAliasAnalysis(this, &(f.getParent()->getDataLayout()));
  outs() << "====================== " << f.getName()
         << " ======================"
         << "\n";
  AT = new AliasSetTracker(*this);

  // Everything needs to be added to the AT
  for (auto &block : f) {
    for (auto &inst : block) {
      // Get all the names of all the things for later
      instructions.insert(&inst);
      if (auto callinst = dyn_cast<CallInst>(&inst)) {
        deleteValue(callinst);
        if (auto calledFunc = callinst->getCalledFunction()) {
          outs() << calledFunc->getName();
          AliasSet &operand = AT->getAliasSetForPointer(
              callinst->getArgOperand(0),
              getTypeStoreSize(callinst->getArgOperand(0)->getType()),
              AAMDNodes());
          AliasSet &otherOperand = AT->getAliasSetForPointer(
              &inst, getTypeStoreSize(inst.getType()), AAMDNodes());
          if (&operand != &otherOperand) {
            operand.mergeSetIn(otherOperand, *AT);
          }
        }
      } else {
        AT->add(&inst); // Everything else is handled
      }
    }
  }

  // Print Alias Results for now
  AT->print(errs());

  // Now perform Data Flow
  auto changed = false;

  for (auto &arg : f.getArgumentList())
    instructions.insert(&arg);

  for (auto &analyzerPair : analyzers)
    analyzerPair.second->initializeState(f);

  for (auto &analyzerPair : analyzers) {
    changed = analyzerPair.second->run(f) || changed;
  }

  // example::DataFlowAnnotator<BroadwayPass> annotator(*this, errs());
  // annotator.print(f);

  delete AT;

  return changed;
}

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

AliasAnalysis::AliasResult
BroadwayPass::alias(const AliasAnalysis::Location &LocA,
                    const AliasAnalysis::Location &LocB) {
  if (auto callinst = llvm::dyn_cast<llvm::CallInst>(LocA.Ptr))
    if (callinst->getArgOperand(0) == LocB.Ptr)
      return AliasAnalysis::AliasResult::MustAlias;
  if (auto callinst = llvm::dyn_cast<llvm::CallInst>(LocB.Ptr))
    if (callinst->getArgOperand(0) == LocA.Ptr)
      return AliasAnalysis::AliasResult::MustAlias;
  return AliasAnalysis::alias(LocA, LocB);
}

void BroadwayPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AliasAnalysis::getAnalysisUsage(AU);
  AU.setPreservesAll(); // this is wrong
  AU.addRequiredTransitive<AliasAnalysis>();
}

static RegisterPass<BroadwayPass>
    X("broadway-pass", "Run analysis using Broadway annotations.", true, true);

static RegisterAnalysisGroup<AliasAnalysis> A("Alias Analysis");

INITIALIZE_AG_PASS(BroadwayPass, AliasAnalysis, "broadway-pass",
                   "Run analysis using Broadway annotations.",
                   false, // Is CFG Only?
                   true,  // Is Analysis?
                   true)  // Is default Analysis Group implementation?
