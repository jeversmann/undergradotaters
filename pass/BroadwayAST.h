#pragma once
#include "rapidjson/document.h"
#include "Utils.h"
#include <llvm/Support/raw_ostream.h>
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
  BroadwayOperation *lhs;
  BroadwayOperation *rhs;
  std::string property;
  std::string time;
  BroadwayOperation(){};
  BroadwayOperation(const jsValue &);
  ~BroadwayOperation() {
    // if (lhs)
    //   delete lhs;
    // if (rhs)
    //   delete rhs;
  }
};

class BroadwayAnalysisRule : public BroadwayBase {
public:
  BroadwayOperation condition;
  std::vector<BroadwayOperation> effects;
  BroadwayAnalysisRule(){};
  BroadwayAnalysisRule(const jsValue &);
};

class BroadwayPointer : public BroadwayBase {
public:
  std::string name;
  std::string parent;
  bool io_flag, delete_flag, new_flag;
  BroadwayPointer *target;
  std::vector<BroadwayPointer> members;

  BroadwayPointer(){};
  BroadwayPointer(const jsValue &);
  ~BroadwayPointer(){
      // if (target)
      //   delete target;
  };

  BroadwayPointer *findDefinition(const std::string &);
  /* BroadwayPointer *getTopParent(); */
};

class BroadwayExitPointer : public BroadwayBase {
public:
  BroadwayOperation condition;
  std::vector<BroadwayPointer> pointers;
  BroadwayExitPointer(){};
  BroadwayExitPointer(const jsValue &);
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

class BroadwayReportElement : public BroadwayBase {
public:
  std::string text;
  std::string property;
  std::string name;
  std::string time;
  BroadwayOperation expression;
  bool callsite = false;
  bool context = false;

  BroadwayReportElement(){};
  BroadwayReportElement(const jsValue &);
};

class BroadwayReport : public BroadwayBase {
public:
  BroadwayOperation condition;
  std::vector<BroadwayReportElement> elements;

  BroadwayReport(){};
  BroadwayReport(const jsValue &);
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
  std::vector<BroadwayPointer> entryPointers;
  std::vector<BroadwayAnalysis> analyses;
  std::vector<BroadwayAccess> accesses;
  std::vector<BroadwayModify> modifies;
  std::vector<BroadwayReport> reports;
  std::vector<BroadwayAction> actions;

  BroadwayProcedure(){};
  BroadwayProcedure(std::string name) : name(name){};
  BroadwayProcedure(const jsValue &);

  // Janky variable lookup funcions
  int getVarNameLocation(std::string var);
  int getVarNameIndirection(std::string var);
};
}
