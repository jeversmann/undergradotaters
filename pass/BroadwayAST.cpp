#include "BroadwayAST.h"

namespace dataflow {

BroadwayProcedure::BroadwayProcedure(const jsValue &procedure) {
  assert(procedure.IsObject());
  const jsValue &nameV = procedure["name"];
  name = nameV.GetString();
  const jsValue &args = procedure["arguments"];
  assert(args.IsArray());
  for (jsValue::ConstValueIterator argIter = args.Begin();
       argIter != args.End(); ++argIter) {
    const jsValue &arg = *argIter;
    arguments.push_back(arg.GetString());
  }

  const jsValue &exits = procedure["exit_pointers"];
  assert(exits.IsArray());
  for (jsValue::ConstValueIterator exitIter = exits.Begin();
       exitIter != exits.End(); ++exitIter) {
    const jsValue &exit = *exitIter;
    exitPointers.push_back(&exit);
  }

  const jsValue &entries = procedure["entry_pointers"];
  assert(entries.IsArray());
  for (jsValue::ConstValueIterator entryIter = entries.Begin();
       entryIter != entries.End(); ++entryIter) {
    const jsValue &entry = *entryIter;
    entryPointers.push_back(&entry);
  }

  const jsValue &analysesV = procedure["analyses"];
  assert(analysesV.IsArray());
  for (jsValue::ConstValueIterator analysisIter = analysesV.Begin();
       analysisIter != analysesV.End(); ++analysisIter) {
    const jsValue &analysis = *analysisIter;
    analyses.push_back(&analysis);
  }

  const jsValue &accessesV = procedure["accesses"];
  assert(accessesV.IsArray());
  for (jsValue::ConstValueIterator accessIter = accessesV.Begin();
       accessIter != accessesV.End(); ++accessIter) {
    const jsValue &access = *accessIter;
    accesses.push_back(&access);
  }

  const jsValue &modifiesV = procedure["modifies"];
  assert(modifiesV.IsArray());
  for (jsValue::ConstValueIterator modifyIter = modifiesV.Begin();
       modifyIter != modifiesV.End(); ++modifyIter) {
    const jsValue &modify = *modifyIter;
    modifies.push_back(&modify);
  }

  const jsValue &reportsV = procedure["reports"];
  assert(reportsV.IsArray());
  for (jsValue::ConstValueIterator reportIter = reportsV.Begin();
       reportIter != reportsV.End(); ++reportIter) {
    const jsValue &report = *reportIter;
    reports.push_back(&report);
  }

  const jsValue &actionsV = procedure["actions"];
  assert(actionsV.IsArray());
  for (jsValue::ConstValueIterator actionIter = actionsV.Begin();
       actionIter != actionsV.End(); ++actionIter) {
    const jsValue &action = *actionIter;
    actions.push_back(&action);
  }
}
}
