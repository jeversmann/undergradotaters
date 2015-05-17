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
      auto &procedure = *pass.procedures[function->getName()];
      for (auto &analysis : procedure.analyses) {
        auto &property = analysis.name;
        if (property != state.name)
          continue;

        for (auto &rule : analysis.rules) {
          if (rule.condition.op == "") { // || evaluate condition to true
            for (auto &effect : rule.effects) {
              if (effect.op == "<-" || effect.op == "<-+") {
                auto &propname = effect.rhs->name;

                llvm::Value *arg;
                for (auto &entry : procedure.entryPointers) {
                  auto *pointerDef = entry.findDefinition(effect.lhs->name);
                  auto *topDef = entry.findDefinition(pointerDef->parent);
                  while (pointerDef->parent != pointerDef->name) {
                    pointerDef = entry.findDefinition(pointerDef->parent);
                  }
                  auto argNum = getArgumentForPointer(topDef->name, procedure);
                  if (argNum != -1) {
                    arg = inst.getArgOperand(argNum)->stripPointerCasts();
                    break;
                  }

                  if (arg) {
                    errs() << "------adding: " << propname << " " << *arg
                           << "\n";
                    state.addToProperty(propname, arg);
                    /* printLattice(state); */
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
  FILE *fp = fopen("memory.json", "r"); // non-Windows use "r"
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
        processCallInstPointers(*callinst);
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

void BroadwayPass::processCallInstPointers(CallInst &inst) {
  if (auto calledFunc = inst.getCalledFunction()) {
    outs() << calledFunc->getName() << "()\n";
    if (procedures.find(calledFunc->getName()) != procedures.end()) {
      auto &procedure = *procedures[calledFunc->getName()];
      // We have information, so prevent others from looking at it
      deleteValue(&inst);

      /*
       * Find any value that occurrs on the lhs more than once, and merge the
       * alias sets of all of the things that point to it.
       */
      for (auto &entry : procedure.entryPointers) {
        auto *pointerDef = entry.findDefinition(effect.lhs->name);
      }

      /* Old stuff for reference
      if (!inst.getType()->isVoidTy()) {
        AliasSet &operand = AT->getAliasSetForPointer(
          inst.getArgOperand(0),
          getTypeStoreSize(inst.getArgOperand(0)->getType()),
          AAMDNodes());
        AliasSet &otherOperand = AT->getAliasSetForPointer(
          &inst, getTypeStoreSize(inst.getType()), AAMDNodes());
        if (&operand != &otherOperand) {
          operand.mergeSetIn(otherOperand, *AT);
        }
      }*/
    } else {
      AT->add(&inst); // We don't have information
    }
  } else {
    AT->add(&inst); // Who knows about a no-name function?
  }
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
