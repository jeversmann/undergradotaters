#ifndef LLVM_BDWY_CLIENT_PROBLEM
#define LLVM_BDWY_CLIENT_PROBLEM

#include "model/Location.h"
#include "model/Blocks.h"

namespace llvm_bdwy {

// implement this to use the results from pointer analysis
// every method is just a hook acting as a transfer function. it'll be called by
// the main DFA engine when appropriate.
class ClientProblem {
  // TODO direction

  public:
    ClientProblem () {}

    // note the places we'll have hooks is a bit different from CBZ due to IR
    // differences. Andy, let me know what other hooks/parameters you need.
    // TODO notably, we're missing lots of things like arithmetic that clients
    // might care about.

    // control flow
     virtual void at_proc_entry (Proc_Loc *context, ProcedureInfo *info) {}
     virtual void at_proc_exit (Proc_Loc *context, ProcedureInfo *info, mem_block_ls_t &return_val) {}
     virtual void at_bb_entry (BB_Loc *context) {}
     virtual void at_bb_exit (BB_Loc *context) {}
     virtual void at_stmt_entry (Stmt_Loc *context) {}
     virtual void at_stmt_exit (Stmt_Loc *context) {}

     virtual void at_merge (BB_Loc *context, Mem_Block *block) {}
     virtual void analysis_done () {}

    // exciting things
     virtual void at_return (Stmt_Loc *context, mem_block_ls_t *return_val) {}
     virtual void at_call (Proc_Loc *context, std::vector<mem_block_ls_t*>& args,
                           mem_block_ls_t &changes, Mem_Def *return_def) {}
     virtual void at_lib_call (Proc_Loc *context, std::vector<mem_block_ls_t*>& args,
                               Mem_Def *return_def) {}

    // expressions
     virtual void at_addr_taken (Stmt_Loc *context, Mem_Block *mb, bool is_use) {}
     virtual void at_dereference (Stmt_Loc *context, mem_block_ls_t *result, bool is_use) {}
     virtual void at_struct_access (Stmt_Loc *context, Mem_Block *base, Mem_Block *field) {}
     virtual void at_array_access (Stmt_Loc *context, Mem_Block *base) {}
     virtual void at_assignment (Stmt_Loc *context, mem_block_ls_t *lhs, Ptr_Val *rhs, Value *src) {}
};

}

#endif
