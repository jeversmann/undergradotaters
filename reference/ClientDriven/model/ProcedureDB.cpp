// authors: Dustin, Parth 

#include "ProcedureDB.h"
#include "Location.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "Mem_Table.h"

using namespace llvm_bdwy;

typedef std::map<Function*, fxn_ls_t*> fxn_map_t;

// a util fxn
void compute_ancestors (ProcedureInfo *info, Function *f, fxn_map_t &callers_of) {
  // add all immediate callers
  for (fxn_ls_t::iterator caller = callers_of[f]->begin(); caller != callers_of[f]->end();
       ++caller)
  {
    // it'll already be in here if... some kind of SCC. (brain is tired)
    if (info->ancestors.count(*caller) == 0) {
      // so add it and recurse
      info->ancestors.insert(*caller);
      compute_ancestors(info, *caller, callers_of);
    }
  }
}

void ProcedureDB::initialize (Module *module, Function *root, CallGraph &call_graph) {
  // temporarily build a mapping from a function to its callers
  fxn_map_t callers_of;

  // Make an entry for every procedure we know about
  foreach_func(func, module) {
    procs[func] = new ProcedureInfo(func);

    callers_of[func] = new fxn_ls_t();
  }

  // now the second pass, to fill out callers_of
  foreach_func(func, module) {
    CallGraphNode *cg_node = call_graph[func];
    for (CallGraphNode::iterator calls_edge = cg_node->begin(); calls_edge != cg_node->end();
         ++calls_edge)
    {
      Function *call_to = calls_edge->second->getFunction();
      // it says printf calls NULL. lol.
      if (call_to) {
        //log() << func->getName() << " calls " << call_to->getName() << "\n";
        callers_of[call_to]->insert(func);
      }
    }
  }

  // now figure out ancestor sets for everybody
  for (proc_map_t::iterator iter = procs.begin(); iter != procs.end(); ++iter) {
    ProcedureInfo *info = iter->second;
    compute_ancestors(info, info->proc, callers_of);
  }

  log() << "Initialized " << procs.size() << " procedures in the DB\n";
}

ProcedureInfo* ProcedureDB::lookup (Function *f) {
  return procs[f];
}

DominatorTree* ProcedureDB::get_domtree (Function *f) {
  ProcedureInfo *info = lookup(f);
  return info->dom_tree;
}

bb_set_t* ProcedureDB::get_dom_frontier (Function *f, BasicBlock *bb)
{
  ProcedureInfo *info = lookup(f);
  return &(info->frontiers[bb]);
}

Mem_Block* ProcedureDB::get_return (Function *f) {
  ProcedureInfo *info = lookup(f);
  return info->return_val;
}

Proc_Loc* ProcedureDB::setup_call (Stmt_Loc *at, Function *called) {
  ProcedureInfo *called_info = lookup(called);
  bool is_cs = called_info->is_context_sensitive;

  called_info->current_callsite = at;
  // store callsite and remember who our caller calls
  ProcedureInfo *caller_info = lookup(at->get_function());
  called_info->callsites[at] = caller_info;
  caller_info->calls.insert(called_info);

  // if we're [CI], the point of making a proc_loc is to have a place to attach
  // external stuff to?
  Proc_Loc *callee_path = at->make_call_to(called, is_cs);

  if (is_cs) {
    return callee_path;
  } else {
    return called_info->ci_loc;
  }
}

void ProcedureDB::return_from_call (Function *called) {
  ProcedureInfo *info = lookup(called);
  info->current_callsite = NULL;
}

void ProcedureDB::mark_for_reanalysis (Function *f) {
  needs_reanalysis.insert(f);

  ProcedureInfo *info = lookup(f);
  // have to add all ancestors too, "to ensure we reach it again"
  for (fxn_ls_t::iterator ancestor = info->ancestors.begin();
       ancestor != info->ancestors.end(); ++ancestor)
  {
    needs_reanalysis.insert(*ancestor);
  }
}

bool ProcedureDB::is_reanalysis_required (Function *f) {
  return needs_reanalysis.count(f) > 0;
}

bool ProcedureDB::is_in_scope (Function *f, Mem_Block *mb) {
  // globals are in scope everywhere.
  if (mb->owner_proc == NULL) {
    return true;
  }

  // is it local to the proc?
  if (mb->owner_proc == f) {
    return true;
  }

  // if the block's owner is in the ancestor set of f, then it's not in scope.
  if (lookup(f)->ancestors.count(mb->owner_proc) == 0) {
    return true;
  } else {
    return false;
  }
}

bool ProcedureDB::is_current_call_recursive (Function *called, Function *caller)
{
  if (called == caller) {
    // well, yes.
    return true;
  }

  ProcedureInfo *caller_info = lookup(caller);

  return (caller_info->ancestors.count(called) > 0);
}

////////////////////////////////////////////////////////////////////////////////

ProcedureInfo::ProcedureInfo (Function *func)
  : is_context_sensitive(!ENABLE_CI), proc(func),
    current_callsite(NULL)
{
  extern_inputs = new mem_block_ls_t();
  extern_outputs = new mem_block_ls_t();

  // main is always C.S., otherwise weird things happen?
  if (proc->getName().compare("main") == 0) {
    is_context_sensitive = true;
  }

  if (is_context_sensitive) {
    ci_loc = NULL;
  } else {
    // TODO [CI] cbz makes a CS proc CI, so they set this up by playing with the
    // first normal callsite to it or something... why can't we just make one
    // and always use it? have to cheat and say it's CS, otherwise, it doesnt
    // recurse and make BB_Locs... we'll see.
    ci_loc = new Proc_Loc(NULL, proc, true);
  }

  // Only do stuff if we have a body
  if (proc->isDeclaration()) {
    worklist = new BitVector(); // forever empty
  } else {
    // RPO order for the worklist
    worklist = new BitVector(proc->getBasicBlockList().size(), false);

    ReversePostOrderTraversal<Function*> rpot(proc);
    int idx = 0;    // index encodes ordering!
    for (ReversePostOrderTraversal<Function*>::rpo_iterator i = rpot.begin(), e = rpot.end();
         i != e; ++i)
    {
      BasicBlock *bb = *i;
      fwd_optimal_order[idx] = bb;
      idx++;
    }

    // Pre-compute the static single-procedure dominator tree
    dom_tree = new DominatorTree();
    dom_tree->runOnFunction(*func);

    // and the dominance frontier for each BB
    foreach_bb(bb, proc) {
      DomTreeNode *node = dom_tree->getNode(bb);
      if (node) {
        DominanceFrontier df;
        // calculate() returns a DomSetType, which is typedef'd as...
        //std::set<BasicBlock*> set = df.calculate(*dom_tree, node);
        frontiers[bb] = df.calculate(*dom_tree, node);
      } else {
        // TODO
        log() << "why wasn't the BB in the dom tree?\n";
      }
    }

    // proc DB will make a second pass and fill out our ancestor sets
  }

  return_val = Loc::memory_model->create_synthetic_obj(proc, RETURN_TYPE);
}

void ProcedureInfo::add_all_blocks () {
  // clear() erases size, so we have to use this silly implementation.
  // TODO off by one?
  for (uint i = 0; i < worklist->size(); i++) {
    worklist->set(i);
  }
}

bool ProcedureInfo::worklist_empty () {
  return worklist->none();
}

BasicBlock* ProcedureInfo::shift_worklist () {
  int idx = worklist->find_first();
  worklist->reset(idx);
  return fwd_optimal_order[idx];
}

void ProcedureInfo::add_block (BitVector *ls, BasicBlock *bb) {
  ls->set(block_idx(bb));
}

int ProcedureInfo::block_idx (BasicBlock *bb) {
  // TODO it's... maybe dumb to do a linear lookup, but easy to change later
  for (std::map<int, BasicBlock*>::iterator iter = fwd_optimal_order.begin();
       iter != fwd_optimal_order.end(); ++iter)
  {
    if (iter->second == bb) {
      return iter->first;
    }
  }
  // shouldn't happen
  log() << "couldn't find idx of bb " << *bb << "\n";
  return -1;
}

// this is not a sparse analysis (although maybe it should be later), and it
// only does BBs of the current proc (i think interprocedural happens due to phi
// placement)
int ProcedureInfo::add_reachable_blocks (BasicBlock *bb, BitVector *visited) {
  // we need to track BBs we've visited while figuring out reachability that
  // might have been already marked
  if (visited == NULL) {
    // base case; first call to this
    visited = new BitVector(proc->getBasicBlockList().size(), false);
  }

  int added = 0;
  foreach_succ(succ, bb) {
    // if it's visited, don't flood the CFG any more
    if (!(*visited)[block_idx(*succ)]) {
      add_block(visited, *succ);  // don't pass over it again
      add_block(worklist, *succ); // ... but actually visit it.
      added += 1 + add_reachable_blocks(*succ, visited);
    }
  }

  return added;
}

ProcedureInfo* ProcedureInfo::current_caller() {
  return callsites[current_callsite];
}

// return true if it's new input

bool ProcedureInfo::add_extern_input (Mem_Block *mb) {
  if (extern_inputs->count(mb)) {
    return false;
  } else {
    extern_inputs->insert(mb);
    return true;
  }
}

bool ProcedureInfo::add_extern_output (Mem_Block *mb) {
  bool found_new_in = false;

  extern_outputs->insert(mb);

  if (mb->current_def != NULL && mb->current_def->is_weak) {
    if (add_extern_input(mb)) {
      found_new_in = true;
    }
  }

  return found_new_in;
}

// is 'longer' a child of 'prefix' in the location tree?
// "prefix" is an awkward name.
bool ProcedureInfo::is_prefix (Loc *prefix, Loc *longer) {
  while (longer) {
    if (longer == prefix) {
      return true;
    } else {
      longer = longer->get_parent();
    }
  }
  
  // nope, just chuck testa
  return false;
}
