#pragma once
#include "rapidjson/document.h"
#include <vector>
#include <memory>

namespace dataflow {
using namespace rapidjson;

class BroadwayBase {
public:
  using jsValue = rapidjson::Value;
  template <class V>
  void addArrayOfValues(const jsValue &array, std::vector<V> &things) {
    assert(array.IsArray());
    for (jsValue::ConstValueIterator iter = array.Begin(); iter != array.End();
         ++iter) {
      const jsValue &value = *iter;
      things.push_back(V(value));
    }
  }
};

class BroadwayOperation : public BroadwayBase {
public:
  std::string op;
  int number;
  std::string name;
  std::unique_ptr<BroadwayOperation> lhs;
  std::unique_ptr<BroadwayOperation> rhs;
  std::string property;
  std::string time;
  BroadwayOperation(){};
  BroadwayOperation(const jsValue &);
};

class BroadwayAnalysisRule : public BroadwayBase {
public:
  BroadwayOperation condition;
  std::vector<BroadwayOperation> effects;
  BroadwayAnalysisRule(){};
  BroadwayAnalysisRule(const jsValue &);
};

class BroadwayExitPointer : public BroadwayBase {
public:
  BroadwayExitPointer(){};
  BroadwayExitPointer(const jsValue &){};
};

class BroadwayEntryPointer : public BroadwayBase {
public:
  BroadwayEntryPointer(){};
  BroadwayEntryPointer(const jsValue &){};
};

class BroadwayAnalysis : public BroadwayBase {
public:
  std::string name;
  std::vector<BroadwayAnalysisRule> rules;
  BroadwayAnalysis(){};
  BroadwayAnalysis(const jsValue &);
};

class BroadwayAccess : public BroadwayBase {
public:
  BroadwayAccess(){};
  BroadwayAccess(const jsValue &){};
};

class BroadwayModify : public BroadwayBase {
public:
  BroadwayModify(){};
  BroadwayModify(const jsValue &){};
};

class BroadwayReport : public BroadwayBase {
public:
  BroadwayReport(){};
  BroadwayReport(const jsValue &){};
};

class BroadwayAction : public BroadwayBase {
public:
  BroadwayAction(){};
  BroadwayAction(const jsValue &){};
};

class BroadwayProcedure : public BroadwayBase {
public:
  std::string name;
  std::vector<std::string> arguments;
  std::vector<BroadwayExitPointer> exitPointers;
  std::vector<BroadwayEntryPointer> entryPointers;
  std::vector<BroadwayAnalysis> analyses;
  std::vector<BroadwayAccess> accesses;
  std::vector<BroadwayModify> modifies;
  std::vector<BroadwayReport> reports;
  std::vector<BroadwayAction> actions;

  BroadwayProcedure(){};
  BroadwayProcedure(std::string name) : name(name){};
  BroadwayProcedure(const jsValue &);
};
}
