#ifndef CONSTANTANALYSIS_H
#define CONSTANTANALYSIS_H
#include "ClientProblem.h"
#include "llvm/ADT/APInt.h"

using namespace std;
using namespace llvm;

namespace llvm_bdwy {

struct CmpAPInt {
  bool operator()(const APInt i1, const APInt i2) {
    return i1.ult(i2);
  }
};
typedef set<APInt, CmpAPInt> APIntSet;

// we want to track what each value MAY be at each point

class ConstantAnalysis : public ClientProblem {
  private:
    map<pair<Value*, Value*>, APIntSet*> values;
    Mem_Table &memory;
    APIntSet* lookup (Value *loc, Value *val);

  public:
    ConstantAnalysis(Mem_Table &memory) : memory(memory) {}
    virtual void at_assignment(Stmt_Loc *context, mem_block_ls_t *lhs, Ptr_Val *rhs, Value* src);
    virtual void analysis_done();
};

}

namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const llvm_bdwy::APIntSet &set);
}

#endif
