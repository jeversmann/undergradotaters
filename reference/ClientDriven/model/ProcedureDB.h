#ifndef LLVM_BDWY_PROC_DB
#define LLVM_BDWY_PROC_DB

// authors: Dustin, Parth 

#include "llvm/Module.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/Analysis/Dominators.h"
#include "../Utils.h"
#include "llvm/Analysis/CallGraph.h"
#include <set>

using namespace llvm;

namespace llvm_bdwy {

// TODO do we need both?
typedef std::vector<BasicBlock*> bb_ls_t;
typedef std::set<BasicBlock*> bb_set_t;

class Stmt_Loc;
class Proc_Loc;
class Mem_Block;
class ProcedureInfo;
class Loc;

// TODO header nightmare, dupe it
typedef std::set<Mem_Block*> mem_block_ls_t;
typedef std::map<Stmt_Loc*, ProcedureInfo*> callsite_map_t;
typedef std::set<Function*> fxn_ls_t;
typedef std::map<Function*, ProcedureInfo*> proc_map_t;

class ProcedureInfo {
  /*
   * Store static info like optimal worklist order and dominance frontiers, and
   * dynamic info like callsites and current worklist.
   */
  private:
    // the CBZ implementation of a worklist uses a bit set, which is actually a
    // really clever idea we will mimic to efficiently keep sorted order.
    BitVector* worklist;

    // pre-computed to sort the worklist better.
    // TODO backwards too, for some clients?
    std::map<int, BasicBlock*> fwd_optimal_order;

    void add_block (BitVector *ls, BasicBlock *bb);
    int block_idx (BasicBlock *bb);

  public:
    bool is_context_sensitive;
    Function *proc;
    DominatorTree *dom_tree;
    std::map<BasicBlock*, bb_set_t> frontiers;
    Mem_Block *return_val;
    callsite_map_t callsites;  // who calls us (and from where)
    std::set<ProcedureInfo*> calls; // who we call, that is
    fxn_ls_t ancestors;
    // [CI]
    mem_block_ls_t *extern_inputs;    // think of as implicit parameters, and acts like merge
    mem_block_ls_t *extern_outputs;   // implicit outs when we def things inside a CI proc
    // TODO diverging from cbz here. :|
    Proc_Loc *ci_loc;   // the one location if this is CI

    ProcedureInfo* current_caller();

    Stmt_Loc* current_callsite;

    ProcedureInfo (Function *func);

    // extern stuff
    bool add_extern_input (Mem_Block *mb);
    bool add_extern_output (Mem_Block *mb);
    bool is_prefix (Loc *prefix, Loc *longer);  // TODO er, could be static somewhere

    // worklist management
    void add_all_blocks ();
    bool worklist_empty ();
    BasicBlock* shift_worklist ();
    // return number of added blocks
    int add_reachable_blocks (BasicBlock *bb, BitVector *visited);
};

class ProcedureDB {
  /*
   * A mapping from procedures to their info.
   */

  private:
    proc_map_t procs;
    fxn_ls_t needs_reanalysis;  // for [CI], changes in other contexts cause this

  public:
    void initialize (Module *module, Function *root, CallGraph &cg);

    ProcedureInfo* lookup (Function *f);
    DominatorTree* get_domtree (Function *f);
    bb_set_t* get_dom_frontier (Function *f, BasicBlock *bb);
    Mem_Block* get_return (Function *f);

    Proc_Loc* setup_call (Stmt_Loc *at, Function *called);
    void return_from_call (Function *called);

    void mark_for_reanalysis (Function *f);
    bool is_reanalysis_required (Function *f);
    bool is_in_scope (Function *f, Mem_Block *mb);
    bool is_current_call_recursive (Function *called, Function *caller);
};

}

#endif
