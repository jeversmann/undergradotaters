#include "BroadwayAST.h"
#include <llvm/Support/raw_ostream.h>

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

BroadwayAnalysis::BroadwayAnalysis(const jsValue &ast) {
  assert(ast.IsObject());
  name = ast["name"].GetString();
  addArrayOfValues(ast["rules"], rules);
}

BroadwayAnalysisRule::BroadwayAnalysisRule(const jsValue &ast) {
  assert(ast.IsObject());
  condition = BroadwayOperation(ast["condition"]);
  addArrayOfValues(ast["effects"], effects);
}

BroadwayOperation::BroadwayOperation(const jsValue &ast) {
  switch (ast.GetType()) {
  case kStringType:
    name = ast.GetString();
    op = "name";
    break;

  case kNumberType:
    number = ast.GetInt();
    op = "number";
    break;

  case kObjectType:
    op = ast["operator"].GetString();
    lhs = new BroadwayOperation(ast["lhs"]);
    if (ast.HasMember("rhs"))
      rhs = new BroadwayOperation(ast["rhs"]);
    if (ast.HasMember("time") && ast["time"].IsString())
      time = ast["time"].GetString();
    if (ast.HasMember("property") && ast["property"].IsString())
      time = ast["property"].GetString();

    break;

  default:
    return;
  }
}

BroadwayPointer::BroadwayPointer(const jsValue &ast) {
  assert(ast.IsObject());
  name = ast["name"].GetString();
  parent = name;
  io_flag = ast.HasMember("io") && ast["io"].IsBool() && ast["io"].GetBool();
  new_flag =
      ast.HasMember("new") && ast["new"].IsBool() && ast["new"].GetBool();
  delete_flag = ast.HasMember("delete") && ast["delete"].IsBool() &&
                ast["delete"].GetBool();
  if (ast.HasMember("target") && ast["target"].IsObject()) {
    target = new BroadwayPointer(ast["target"]);
    target->parent = name;
  }
  if (ast.HasMember("members") && ast["members"].IsArray())
    addArrayOfValues(ast["members"], members);
  for (auto &member : members)
    member.parent = name;
}

BroadwayPointer *BroadwayPointer::findDefinition(const std::string &thing) {
  if (name == thing)
    return &*this;
  if (target) {
    auto *result = target->findDefinition(thing);
    if (result)
      return result;
  }
  for (auto &member : members) {
    auto *result = member.findDefinition(thing);
    if (result)
      return result;
  }
  return nullptr;
}

/* BroadwayPointer *BroadwayPointer::getTopParent() { */
/*   if (parent) */
/*     return parent->getTopParent(); */
/*   return this; */
/* } */

BroadwayExitPointer::BroadwayExitPointer(const jsValue &ast) {
  assert(ast.IsObject());
  if (ast.HasMember("condition") && !ast["condition"].IsNull())
    condition = BroadwayOperation(ast["condition"]);
  if (ast["pointers"].IsArray())
    addArrayOfValues(ast["pointers"], pointers);
}
}
