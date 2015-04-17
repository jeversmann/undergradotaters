#include "ConstantAnalysis.h"
#include "model/Blocks.h"
#include "model/Mem_Table.h"
#include "llvm/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Instructions.h"
#include <iostream>

using namespace std;
using namespace llvm;
using namespace llvm_bdwy;

void ConstantAnalysis::at_assignment(Stmt_Loc *context, mem_block_ls_t *lhs, Ptr_Val *rhs, Value* src) 
{
  // Only care when the RHS is an int
  if (!isa<IntegerType>(src->getType())) {
    //log() << "Not an integer type (" << *type << " instead), skipping\n";
    return;
  }

  // Either the rhs will be a constant int, or a value we can lookup and copy
  ConstantInt *constant = dyn_cast<ConstantInt>(src);
  
  APIntSet numbers;

  if (constant) {
    numbers.insert(constant->getValue());
  } else {
    // TODO lookup...
  }

  if (numbers.size() == 0) {
    // TODO happens when passing normal ints as args
    log() << "TODO can't figure out constant behind src " << *src << "\n";
  }

  log() << "Propagating constants during assignment of constants " << numbers
        << " to " << *lhs << "\n";

  Value* loc = context->expr;

  // If it's a weak update (many LHS or the one LHS has multiplicity), union the
  // possibilities
  // Else it's a redef
  foreach_memblock(left, *lhs) {
    bool strong_update = lhs->size() == 1 && (*left)->can_be_strong_update();
    APIntSet *so_far = lookup(loc, (*left)->decl);
    if (strong_update) {
      so_far->clear();
    }
    // add the new possibilities
    for (APIntSet::iterator num = numbers.begin(); num != numbers.end(); ++num) {
      so_far->insert(*num);
    }
  }
}

APIntSet* ConstantAnalysis::lookup (Value *loc, Value *var) {
  if (values.find(make_pair(loc,var)) == values.end()) {
    values[make_pair(loc,var)] = new APIntSet();
  }
  return values[make_pair(loc,var)];
}

void ConstantAnalysis::analysis_done() {
  for (map<pair<Value*, Value*>, APIntSet*>::iterator itr = values.begin(); itr != values.end(); ++itr) {
    Value* var_decl = itr->first.second;
    StringRef name = var_decl->getName();
    log() << "'";
    if(name.size() != 0) {ferrs() << name;} else {ferrs() << *var_decl;}
    ferrs() << "' could be: " << *(itr->second) << " at " << *(itr->first.first) << "\n";
  }
}

namespace llvm {
  // in functional I think this is something like concat("[", join(", ", set),
  // "]") >_<
  raw_ostream &operator<<(raw_ostream &os, const APIntSet &set) {
    log() << "[";
    for (APIntSet::iterator num = set.begin(); num != set.end(); ++num) {
      os << *num;
      APIntSet::iterator next = num;
      next++;
      if (next != set.end()) {
        os << ", ";
      }
    }
    os << "]";
    return os;
  }
}
