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

  addArrayOfValues(procedure["exit_pointers"], exitPointers);
  addArrayOfValues(procedure["entry_pointers"], entryPointers);
  addArrayOfValues(procedure["analyses"], analyses);
  addArrayOfValues(procedure["accesses"], accesses);
  addArrayOfValues(procedure["modifies"], modifies);
  addArrayOfValues(procedure["reports"], reports);
  addArrayOfValues(procedure["actions"], actions);
}
}
