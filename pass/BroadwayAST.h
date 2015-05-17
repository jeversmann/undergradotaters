#pragma once
#include "rapidjson/document.h"
#include <vector>

namespace dataflow {
using namespace rapidjson;

class BroadwayProcedure {
  using jsValue = rapidjson::Value;

public:
  std::string name;
  std::vector<std::string> arguments;
  std::vector<const jsValue *> exitPointers;
  std::vector<const jsValue *> entryPointers;
  std::vector<const jsValue *> analyses;
  std::vector<const jsValue *> accesses;
  std::vector<const jsValue *> modifies;
  std::vector<const jsValue *> reports;
  std::vector<const jsValue *> actions;

  BroadwayProcedure();
  BroadwayProcedure(std::string name) : name(name){};
  BroadwayProcedure(const jsValue &);
};
}
