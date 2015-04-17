#include "Location.h"
#include "llvm/Analysis/Dominators.h"
#include "llvm/Support/raw_os_ostream.h"

using namespace llvm_bdwy;

ProcedureDB* Loc::proc_db = NULL;
Mem_Table* Loc::memory_model = NULL;

// same as strictly_dominates, but also allow l1 == l2
bool Loc::dominates (Loc *l1, Loc *l2) {
  if (l1 == l2) {
    return true;
  } else {
    return strictly_dominates(l1, l2);
  }
}

// does l1 dominate l2?
bool Loc::strictly_dominates (Loc *l1, Loc *l2) {
  // A few special cases
  if (l1 == l2) {
    return false;   // we're strict
  } else if (l1 == Proc_Loc::root_loc) {
    return true;    // main() dominates all
  } else if (l2 == Proc_Loc::root_loc) {
    return false;   // nothing dominates main()
  } else if (isa<Global_Loc>(l1)) {
    return !isa<Global_Loc>(l2); // Nothing sdoms a Global_Loc
  }

  // There are lots of special cases I don't understand yet, but I think this is
  // the big picture:
  // - if we're comparing two locs in the same BB, it's a matter of seeing which
  //   statement comes first
  // - two locs in the same procedure? just see if one BB dominates the other in
  //   the CFG.
  // - say a location is buried deep in the call graph. it has to have a common
  //   ancestor with the other location somewhere up the call graph. within that
  //   parent, one location is a child of some statement or basic block
  //   (containing a CALL). then we can apply the above test

  // TODO context insensitivity complicates this, and I don't know why cbz cares
  // about crossing procedure boundaries so much.

  Loc *parent1 = l1;
  Loc *parent2 = l2;
  find_common_ancestor(&parent1, &parent2);

  DominatorTree *dom_tree = proc_db->get_domtree(parent1->get_function());

  // they can't be procedures, and they'll have the same types.
  // it also turns out the same LLVM pass can tell us dominance info for BBs and
  // instructions. do we want to trust it?
  
  if (Stmt_Loc *p1_stmt = dyn_cast<Stmt_Loc>(parent1)) {
    Stmt_Loc *p2_stmt = dyn_cast<Stmt_Loc>(parent2);
    return dom_tree->dominates(p1_stmt->expr, p2_stmt->expr);
  } else if (BB_Loc *p1_bb = dyn_cast<BB_Loc>(parent1)) {
    BB_Loc *p2_bb = dyn_cast<BB_Loc>(parent2);
    return dom_tree->dominates(p1_bb->bb, p2_bb->bb);
  } else if (Proc_Loc *p1_proc = dyn_cast<Proc_Loc>(parent1)) {
    if (p1_proc->parent != NULL) {
      // TODO shouldnt happen!
      log() << "common ancestor is procs, but theyre not CI?!\n";
    }
    // two proc_locs... both parents are NULL. two CIs procs, or a CI proc and
    // main. either way, return false. this occurs while we're looking for a
    // rdef in ci_nearest_def_at, which handles popping up a call-graph level.
    // so try that.
    return false;
  } else {
    log() << "p1 is " << *parent1 << "\n";
    log() << "p2 is " << *parent2 << "\n";
    // TODO shouldnt happen!
    log() << "WARNING common ancestor of two locations cant be a call-site!\n";
    return false;
  }
}

// modifies l1 and l2 so that l1->parent == l2->parent
// which means typeof(l1) == typeof(l2) since parents can only have one type of
// kid
void Loc::find_common_ancestor(Loc **l1, Loc **l2) {
  while ((*l1)->get_parent() != (*l2)->get_parent()) {
    // depth has to match too, so figure out which to wind upwards based on that
    if ((*l1)->depth == (*l2)->depth) {
      *l1 = (*l1)->get_parent();
      *l2 = (*l2)->get_parent();
    } else if ((*l1)->depth < (*l2)->depth) {
      *l2 = (*l2)->get_parent();
    } else if ((*l1)->depth > (*l2)->depth) {
      *l1 = (*l1)->get_parent();
    }
  }
}

namespace llvm {
  raw_ostream &operator<<(raw_ostream &os, const Loc &loc) {
    if (const Stmt_Loc *stmt = dyn_cast<Stmt_Loc>(&*&loc)) {
      stmt->prettyprint(os);
    } else if (const BB_Loc *bb = dyn_cast<BB_Loc>(&*&loc)) {
      bb->prettyprint(os);
    } else if (const Proc_Loc *proc = dyn_cast<Proc_Loc>(&*&loc)) {
      proc->prettyprint(os);
    } else if (const Global_Loc *glob = dyn_cast<Global_Loc>(&*&loc)) {
      glob->prettyprint(os);
    }

    return os;
  }
}

////////////////////////////////////////////////////////////////////////////////

Proc_Loc* Proc_Loc::root_loc = NULL;

Proc_Loc::Proc_Loc (Stmt_Loc *callsite, Function *f, bool is_cs)
  : is_context_sensitive(is_cs), parent(callsite), func(f)
{
  // we used to detect root by a NULL parent, but now C.I. procedures have those
  // too.
  if (parent) {
    depth = parent->depth + 1;
  } else {
    depth = 0;
  }
  if (f->getName().compare("main") == 0) {
    // we're root. remember this for a fast dominator case
    assert(Proc_Loc::root_loc == NULL);
    Proc_Loc::root_loc = this;
  }

  // [CI] Attach all uses and defs and external IO here.
  if (is_context_sensitive) {
    // Set up our children (basic blocks)
    foreach_bb(bb, func) {
      BB_Loc *bb_loc = new BB_Loc(this, bb);
      blocks[bb] = bb_loc;
    }
  }
}

Function* Proc_Loc::get_function () {
  return func;
}

Loc* Proc_Loc::get_parent () const {
  return parent;
}

BB_Loc* Proc_Loc::lookup_block (BasicBlock *bb) {
  return blocks[bb];
}

Stmt_Loc* Proc_Loc::get_last_stmt () {
  BB_Loc *last_bb = lookup_block(&(func->back()));
  return last_bb->lookup_stmt(&(last_bb->bb->back()));
}

void Proc_Loc::prettyprint (raw_ostream &os) const {
  // TODO later have symbolic id. it's just tedious to print our parent, since
  // we could be arbitrarily deep
  os << "ProcLoc(" << func->getName() << ")";
}

void Proc_Loc::graph (raw_os_ostream &os) const {
  os << "  \"" << this << "\" [label = \"" << func->getName() << "\"];\n";
  std::map<BasicBlock *, BB_Loc *>::const_iterator i;
  for(i = blocks.begin(); i != blocks.end(); i++) {
    os << "  \"" << this << "\"->\"" << i->second << "\";\n";
    i->second->graph(os);
  }
}

void Proc_Loc::dump () const {
  log() << *this << "\n";
  log() << "Called by:\n";
  log_push();
  if (parent) {
    parent->dump();
  } else {
    log() << "Nobody\n";
  }
  log_pop();
}

////////////////////////////////////////////////////////////////////////////////

BB_Loc::BB_Loc (Proc_Loc *proc, BasicBlock *block)
  : parent(proc), bb(block)
{
  depth = parent->depth + 1;
  // TODO do we care if this dominates the exit? (doesn't everything?)

  // Set up our children (statements)
  foreach_stmt(stmt, bb) {
    Stmt_Loc *stmt_loc = new Stmt_Loc(this, stmt);
    statements[stmt] = stmt_loc;
  }
}

Function* BB_Loc::get_function () {
  return parent->func;
}

Loc* BB_Loc::get_parent () const {
  return parent;
}

Stmt_Loc* BB_Loc::lookup_stmt (Value *stmt) {
  return statements[stmt];
}

Stmt_Loc* BB_Loc::last() {
  return lookup_stmt(&(bb->back()));
}

void BB_Loc::prettyprint (raw_ostream &os) const {
  // TODO later have symbolic id. it's just tedious to print our parent, since
  // we could be arbitrarily deep
  os << "BBLoc(" << bb->getName() << ")";
}

void BB_Loc::graph (raw_os_ostream &os) const {
  int count = 1;
  int max = statements.size();
  std::stack<Stmt_Loc *> queue;
  os << "  \"" << this << "\" [label = \" ";
  std::map<Value *, Stmt_Loc *>::const_iterator i;
  for(i = statements.begin(); i != statements.end(); i++) {
    os << "<" << i->second << "> " << *(i->second->expr);
    queue.push(i->second);
    if(count < max) {
      os << " | ";
      count ++;
    }
  }
  os << "\"];\n";
  while(!queue.empty()) {
    Stmt_Loc * cur = queue.top();
    cur->graph(os);
    queue.pop();
  }
}

void BB_Loc::dump () const {
  log() << *this << "\n";
  log() << "Inside proc:\n";
  log_push();
  if (parent) {
    parent->dump();
  } else {
    log() << "WARNING Nothing\n";
  }
  log_pop();
}

////////////////////////////////////////////////////////////////////////////////

Stmt_Loc::Stmt_Loc (BB_Loc *bb, Instruction *value)
  : call_to(NULL), parent(bb), expr(value)
{
  depth = parent->depth + 1;
}

Function* Stmt_Loc::get_function () {
  return parent->parent->func;
}

Loc* Stmt_Loc::get_parent () const {
  return parent;
}

Proc_Loc* Stmt_Loc::make_call_to (Function *f, bool is_cs) {
  // Only ever create once
  if (!call_to) {
    call_to = new Proc_Loc(this, f, is_cs);
  }
  return call_to;
}

void Stmt_Loc::prettyprint (raw_ostream &os) const {
  // TODO later have symbolic id. it's just tedious to print our parent, since
  // we could be arbitrarily deep
  // TODO line nums might be more readable...
  os << "StmtLoc(" << *expr << ")";
}

void Stmt_Loc::graph (raw_os_ostream &os) const {
  if(call_to) {
    os << "  \"" << this->parent << "\":\"" << this << "\"->\"" << call_to << "\" ";
    os << "[ label = \"Call\" ];\n";
    call_to->graph(os);
  }
}

void Stmt_Loc::dump () const {
  log() << *this << "\n";
  log() << "Inside BB:\n";
  log_push();
  if (parent) {
    parent->dump();
  } else {
    log() << "WARNING Nothing\n";
  }
  log_pop();
}

void Global_Loc::graph(raw_os_ostream &os) const {
  // Do what now?
}

void Global_Loc::dump () const {
  log() << *this << "\n";
}
