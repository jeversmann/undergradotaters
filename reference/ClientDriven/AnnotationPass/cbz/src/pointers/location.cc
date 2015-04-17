// $Id: location.cc,v 1.23 2003/08/08 15:16:29 toktb Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Daniel A. Jimenez
//  Calvin Lin
// 
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation
//  files (the "Software"), to deal in the Software without
//  restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies
//  of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//  NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF TEXAS AT
//  AUSTIN BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  We acknowledge the C-to-C Translator from MIT Laboratory for
//  Computer Science for inspiring parts of the C-Breeze design.
//
// ----------------------------------------------------------------------

#include "c_breeze.h"
#include "location.h"
#include <limits>

// ------------------------------------------------------------
// Location
// ------------------------------------------------------------

unsigned int Location::stmt_count = 0;
unsigned int Location::block_count = 0;
unsigned int Location::proc_count = 0;
unsigned int Location::dom_calls = 0;

unsigned int Location::current_subtree_id = 0;
unsigned int Location::current_tree_number = 0;

Location::Location(Location * parent, LocationKind kind)
  : _parent(parent),
    _kind(kind),
    _num_children(0),
    _dominator(0),
    _tree_min(0),
    _tree_max(numeric_limits<unsigned int>::max())
{

#ifdef OLD_DOMINATOR_TEST

  _depth = parent ? parent->depth() + 1 : 0;
  _dominates_exit = 0;
  _not_basicblock = (kind != BasicBlock);
  _not_procedure = (kind != Procedure);

#endif

  if (parent)
    _subtree_id = parent->subtree_id();
}

void Location::set_dominator(Location * d)
{
  // -- Set the dominator

  _dominator = d;

  // -- Note the presence of the child on the dominator

  d->increment_children();
}

void Location::clear_dominator()
{
  // -- Note the removal of the child on the dominator

  _dominator->decrement_children();
  
  // -- Clear the dominator

  _dominator = 0;
}


// Dominance tests

bool Location::strictly_dominates(const Location * dom, const Location * cur)
{
  // Quick short-circuits

  if (dom == cur)
    return false;

  // Don't bother comparing locations in disconnected subtrees (this only
  // occurs with context insensitive analysis).

  if (dom->subtree_id() != cur->subtree_id())
    return false;

#ifdef OLD_DOMINATOR_TEST

  bool old_dominates = old_strictly_dominates(dom, cur);

#endif

  dom_calls++;

  bool new_dominates = false;

  unsigned int dom_min = dom->tree_min();
  unsigned int cur_min = cur->tree_min();
  unsigned int dom_max = dom->tree_max();
  unsigned int cur_max = cur->tree_max();

  // if (dom_max == 0) dom_max = numeric_limits<unsigned int>::max();
  // if (cur_max == 0) cur_max = numeric_limits<unsigned int>::max();
 
  if ((dom_min < cur_min) && (cur_max <= dom_max))
    new_dominates = true;

  /*
  if (dom_min < cur_min) {
    dom_max = dom->tree_max();
    cur_max = cur->tree_max();
    if (dom_max == 0) dom_max = numeric_limits<unsigned int>::max();
    if (cur_max == 0) cur_max = numeric_limits<unsigned int>::max();

    if (cur_max <= dom_max)
      new_dominates = true;
  }
  */

#ifdef OLD_DOMINATOR_TEST

  if (new_dominates != old_dominates) {
    cout << "TEST FAILED for " 
	 << * dom << "(" << dom->tree_min() << " - " << dom->tree_max() << ")" << endl;
    
    if (old_dominates)
      cout << "          DOM " << endl;
    else
      cout << "          NOT DOM " << endl;
    
    cout << "                " << * cur << "(" << cur->tree_min() << " - " << cur->tree_max() << ")" << endl;
  }

#endif

  return new_dominates;
}
  
#ifdef OLD_DOMINATOR_TEST

bool Location::old_strictly_dominates(const Location * dom, const Location * cur)
{
  // Quick short-circuits

  if (dom == cur)
    return false;

  // Don't bother comparing locations in disconnected subtrees (this only
  // occurs with context insensitive analysis).

  if (dom->subtree_id() != cur->subtree_id())
    return false;

  // Special case: main function entry point dominates everything

  if (dom == procLocation::main())
    return true;

  if (cur == procLocation::main())
    return false;

  dom_calls++;

  // Get to the same level in the context chain (side-effects dom_p and
  // cur_p by moving up the chain until they are at the same depth).

  const Location * dom_p = dom;
  const Location * cur_p = cur;

  unsigned int depth_dom = dom_p->depth();
  unsigned int depth_cur = cur_p->depth();
  unsigned int max_depth = (depth_dom > depth_cur) ? depth_dom : depth_cur;

  // -- The following flags are used whenever we need to move up in the
  // dominating location. For a location inside a procedure to dominate a
  // location outside that procedure, it must dominate the exit of the
  // procedure. Therefore, at each procedure boundary we cross, we need to
  // know that the basic block from which we came dominates the exit.

  // The algorithm works as follows:
  //   1. dom_exit is initially set to 1
  //   2. local_dom_exit is true when the last basic block
  //      visited dominates the exit.
  //   3. not_procedure_boundary is true as long as we have
  //      never crossed a procedure boundary
  // => dom_exit is true if it was previously true, and if
  //      either we have never crossed a boundary, or the
  //      last basic block visited dominates its exit.

  unsigned int dom_exit = 1;
  unsigned int local_dom_exit = 0;
  unsigned int not_procedure_boundary = 1;

  while ((depth_dom > depth_cur) & dom_exit) {

    // -- At each basic block, reset local_dom_exit

    local_dom_exit &= dom_p->_not_basicblock;

    // -- Whenever we hit a basic block, record whether it dominates the
    // exit. Since dominates_exit() is false for all other nodes, they
    // don't affect the value.

    local_dom_exit |= dom_p->dominates_exit();

    // -- Record whether we have NOT hit a procedure boundary. This value
    // starts out true, but once we hit a procedure boundary it turns
    // false, and stays that way. That tells the algorithm that it's
    // important to check for dominates_exit.

    not_procedure_boundary &= dom_p->_not_procedure;

    // -- Compute dom_exit: either the most recently visited basic block
    // must dominate the exit, or we have not crossed a procedure boundary
    // yet. Once this value is false, it stays false.

    dom_exit &= (local_dom_exit | not_procedure_boundary );

    // -- Move up

    dom_p = dom_p->parent();
    depth_dom--;
  }

  // -- Short circuit: if dom_exit is false, then we crossed a procedure
  // boundary in which the basic block we came from didn't dominate the
  // exit node: therefore, it cannot dominate the input location.

  if ( ! dom_exit )
    return false;

  while (depth_cur > depth_dom) {
    cur_p = cur_p->parent();
    depth_cur--;
  }

  // --- One is a prefix of the other; use the special rules described
  // in the header file.

  if (dom_p == cur_p) {
    if (dom->depth() > cur->depth()) {
      // Dom is the descendant -- it only dominates ancestor
      // statements.

      return (cur->kind() == Statement);
    }
    else {
      // Dom is the ancestor -- it always dominates, unless it is a
      // statement.

      return (dom->kind() != Statement);
    }
  }

  // --- Find a common parent

  const Location * dom_next = dom_p->parent();
  const Location * cur_next = cur_p->parent();

  while ((dom_next != cur_next) & dom_exit) {

    // -- Use the same algorithm to determine whether dom_p dominates the
    // exit of any procedure that we cross.

    local_dom_exit &= dom_p->_not_basicblock;
    local_dom_exit |= dom_p->dominates_exit();
    not_procedure_boundary &= dom_p->_not_procedure;
    dom_exit &= (local_dom_exit | not_procedure_boundary );

    // -- Move up

    dom_p = dom_next;
    dom_next = dom_p->parent();

    cur_p = cur_next;
    cur_next = cur_p->parent();
  }

  // --- Same short-circuit:

  if ( ! dom_exit )
    return false;

  // --- Found a common parent, so test dominance locally

  if (dom_p->kind() == Statement) {
    stmtLocation * dom_s = (stmtLocation *) dom_p;
    stmtLocation * cur_s = (stmtLocation *) cur_p;

    return (dom_s->stmt_num() < cur_s->stmt_num());
  }

  if (dom_p->kind() == BasicBlock) {
    basicblockLocation * dom_b = (basicblockLocation *) dom_p;
    basicblockLocation * cur_b = (basicblockLocation *) cur_p;

    return Dominators::dominates(dom_b->block(), cur_b->block());
  }

  // --- This shouldn't happen

  // --- HOWEVER, with context insensitivity, it can

  return false;

  cerr << "ERROR: Cannot determine path dominance." << endl;
  cerr << "   Dom  : " << *dom << " depth = " << dom->depth() << endl;
  if (dom_p)
    cerr << "   DomP : " << *dom_p << " depth = " << dom_p->depth() << endl;
  cerr << "   Cur  : " << *cur << " depth = " << cur->depth() << endl;
  if (cur_p)
    cerr << "   CurP : " << *cur_p << " depth = " << cur_p->depth() << endl;
  
  return false;
}

Location * Location::common_parent(Location * one,
				   Location * two)
{
  if (one == two)
    return one;

  // Get to the same level in the context chain (side-effects one_p and
  // two_p by moving up the chain until they are at the same depth).

  Location * one_p = one;
  Location * two_p = two;

  unsigned int depth_one = one_p->depth();
  unsigned int depth_two = two_p->depth();
  unsigned int max_depth = (depth_one > depth_two) ? depth_one : depth_two;

  while (depth_one > depth_two) {
    one_p = one_p->parent();
    depth_one--;
  }

  while (depth_two > depth_one) {
    two_p = two_p->parent();
    depth_two--;
  }

  // --- One is a prefix of the other; return it

  if (one_p == two_p)
    return one_p;

  // --- Find a common parent

  Location * one_next = one_p->parent();
  Location * two_next = two_p->parent();

  while (one_next != two_next) {

    one_p = one_next;
    one_next = one_p->parent();

    two_p = two_next;
    two_next = two_p->parent();
  }

  return one_p;
}

#endif


procLocation * Location::procedure(Location * where)
{
  if (where->kind() == Procedure)
    return (procLocation *) where;

  if (where->kind() == Statement) {
    stmtLocation * stmt = (stmtLocation *) where;
    return stmt->block_location()->proc_location();
  }

  if (where->kind() == BasicBlock) {
    basicblockLocation * block = (basicblockLocation *) where;
    return block->proc_location();
  }

  return 0;
}

bool Location::is_prefix(const Location * prefix, const Location * longer)
{
  while (longer) {
    if (longer == prefix)
      return true;
    longer = longer->parent();
  }

  return false;
}

void Location::stats()
{
  cout << "  stmtLocation : " << stmt_count << " objects x " 
       << sizeof(stmtLocation) << " bytes = " << stmt_count * sizeof(stmtLocation) 
       << " bytes. " << endl;

  cout << "  basicblockLocation : " << block_count << " objects x " 
       << sizeof(basicblockLocation) << " bytes = " << block_count * sizeof(basicblockLocation) 
       << " bytes. " << endl;

  cout << "  procLocation : " << proc_count << " objects x " 
       << sizeof(procLocation) << " bytes = " << proc_count * sizeof(procLocation) 
       << " bytes. " << endl;

}
  

bool Location::dominates(const Location * dom, const Location * cur)
{
  if (dom == cur)
    return true;

  return strictly_dominates(dom, cur);
}

Location::~Location()
{
}

// ------------------------------------------------------------
//  Tree numbering algorithm
// ------------------------------------------------------------

void Location::visit()
{
  if (tree_min() == 0) {

    set_tree_min(next_tree_number());

    /*
    cout << "set tree min = " << tree_min() << " for " << *this;
    if (dominator())
      cout << " IDOM = " << * dominator() << endl;
    else
      cout << " (No dominator)" << endl;
    */
  }
}


void Location::finish()
{
  if ((tree_max() == numeric_limits<unsigned int>::max()) &&
      (num_children() == 0))
    {
      // -- Dominator leaf node: start setting tree max values

      unsigned int cur_max = tree_min();
      Location * cur_node = this;
      Location * cur_dom;

      do {
	cur_node->set_tree_max(next_tree_number());
	cur_dom = cur_node->dominator();

	/*
	cout << "set tree max = " << cur_node->tree_max() << " for " << * cur_node;
	if (cur_dom)
	  cout << " IDOM = " << * cur_dom << endl;
	else
	  cout << " (No dominator)" << endl;
	*/

	cur_node = cur_dom;

	if (cur_node)
	  cur_node->decrement_children();

      } while (cur_node &&
	       (cur_node->num_children() == 0));
    }
}

// ------------------------------------------------------------
// stmtLocation
// ------------------------------------------------------------

stmtLocation::stmtLocation(basicblockLocation * parent)
  : Location(parent, Statement),
    _stmt_num(0),
    _calls(0)
{
  Location::stmt_count++;
}

// At a call-site : look up the procLocation, creating it if
// necessary.

void stmtLocation::setup_cs_call(procNode * proc)
{
  if(_calls && _calls->proc() != proc) { // TB need a new _calls
    _calls = _all_calls[proc];
  }

  if (! _calls) {
    _calls = _all_calls[proc] = new procLocation(this, proc, false);

    // -- Set tree numbering on the new procLocation

    _calls->visit();

    // -- The new procedure is dominated by the old dominator of the
    // statement.

    _calls->set_dominator(dominator());
    clear_dominator();

    // -- Reset the tree min (it will be fixed later)

    set_tree_min(0);

    // cout << "un  tree_min = 0 for " << *this << endl;

    // -- The statement itself is now dominated by the exit of the
    // procedure.

    set_dominator(_calls->last());
  }
}

/** @brief Remove procedure
 *
 * Remove a procedure from it's callsite, adjusting the depths to reflect
 * this. */

procLocation * stmtLocation::remove_call()
{
  procLocation * out = 0;

  if (_calls) {

    // -- Disconnect the pointers

    out = _calls;
    out->remove_from_tree();
    _calls = 0;

    // -- Undo the dominator links from setup_call()

    set_dominator(out->dominator());
    out->clear_dominator();

    // -- Number the stmtLocation now, if it needs it. This is critical for
    // handling recursive procedures.

    visit();
  }

  return out;
}

/** @brief Get the callNode
 *
 * If this statement is a procedure call, return the callNode. */

/* TB: remove.
callNode * stmtLocation::callnode()
{
  stmtNode * the_stmt = stmt();
  callNode * call = findCallNodeWalker::find(the_stmt);

  / * -- This doesn't work any more...
  if (the_stmt->typ() == Expr) {
    exprstmtNode * es = (exprstmtNode *) the_stmt;
    exprNode * e = es->expr();
    if (e) {
      if (e->typ() == Call)
	call = (callNode *)e;
      if (e->typ() == Binary) {
	binaryNode * b = (binaryNode *) e;
	if (b->right()->typ() == Call)
	  call = (callNode *) b->right();
      }
    }
  }
  * /

  return call;
} */

// Check for the presence of a function call

bool stmtLocation::is_present(const procNode * proc) const
{
  const procLocation * procl = block_location()->proc_location();
  if (procl->proc() == proc)
    return true;

  if (procl->stmt_location())
    return procl->stmt_location()->is_present(proc);

  return false;
}

void stmtLocation::adjust_depth()
{
#ifdef OLD_DOMINATOR_TEST

  // -- Adjust the depth here
  
  _depth = _parent->depth() + 1;

#endif

  // -- Fix the subtree ID

  _subtree_id = _parent->subtree_id();

  // -- Visit the called procedure
 
  if (_calls)
    _calls->adjust_depth();
}

// Output

void stmtLocation::print(ostream & o) const
{

}

void stmtLocation::print_path(ostream & o) const
{
  block_location()->print_path(o);
  o << ':' << _stmt_num << '(' << _stmt->coord() << ')';
}

void stmtLocation::print_callsite(ostream & o) const
{
  basicblockLocation * bb = block_location();
  procLocation * p = bb->proc_location();

  o << p->proc()->decl()->name() 
    << ':' << bb->block()->dfn()
    << ':' << _stmt_num;
}

// Destructor

stmtLocation::~stmtLocation()
{
  // If this is a call-site, delete the called procedure location

  if (_calls)
    delete _calls;

  Location::stmt_count--;
}

// ------------------------------------------------------------
// basicblockLocation
// ------------------------------------------------------------

basicblockLocation::basicblockLocation(procLocation * parent,
				       basicblockNode * block)
  : Location(parent, BasicBlock),
    _block(block),
    _statements(block->stmts().size(), stmtLocation(this))
{
  basicblockNode * exit = parent->proc()->exit();

#ifdef OLD_DOMINATOR_TEST

  // -- Figure out if this block dominates the exit

  _dominates_exit = Dominators::dominates(block , exit);

#endif

  // -- First statement is dominated by the basic block itself.

  Location * cur_dom = this;

  // -- Set all the statement numbers and stmtNode pointers

  stmt_list & stmts = block->stmts();
  unsigned int num = 0;

  for (stmt_list_p p = stmts.begin();
       p != stmts.end();
       ++p)
    {
      stmtLocation * cur_stmt = & _statements[num];
      cur_stmt->stmt_num(num);
      cur_stmt->stmt(*p);
      num++;

      // -- Each subsequent statement is dominated by the previous
      // statement.

      cur_stmt->set_dominator(cur_dom);
      cur_dom = cur_stmt;
    }

  Location::block_count++;
  Location::stmt_count += block->stmts().size();
}

void basicblockLocation::adjust_depth()
{

#ifdef OLD_DOMINATOR_TEST

  // -- Adjust the depth here

  _depth = _parent->depth() + 1;

#endif

  // -- Fix the subtree ID

  _subtree_id = _parent->subtree_id();

  // -- Visit the statements

  unsigned int size = _statements.size();

  for (unsigned int i = 0; i < size ; i++)
    _statements[i].adjust_depth();
}

// Output

void basicblockLocation::print(ostream & o) const
{

}

void basicblockLocation::print_path(ostream & o) const
{
  proc_location()->print_path(o);
  o << ':' << block()->dfn();
}

// Destructor

basicblockLocation::~basicblockLocation()
{
  Location::block_count--;
  // stmtLocation takes care of this: Location::stmt_count -= block()->stmts().size() - 1;
}

// ------------------------------------------------------------
// procLocation
// ------------------------------------------------------------

procLocation * procLocation::Main = 0;

static unsigned int progress_counter = 0;

procLocation::procLocation(stmtLocation * parent,
			   procNode * proc,
			   bool context_insensitive)
  : Location(parent, Procedure),
    _proc(proc),
    _basicblocks()
{
  Location::proc_count++;

  if (! parent) {

    // -- The only function with no parent should be the main function

    Main = this;

    current_subtree_id = 0;
    _subtree_id = current_subtree_id;

    // -- Main function starts the tree numbering at 1

    current_tree_number = 0;
    set_tree_min(next_tree_number());
  }

  /*
  // -- At context sensitive callsites, just create a single node that
  // represents the call. We use this to attach uses and defs that pass
  // external inputs and outputs from the context.

  if (is_context_insensitive)
    return;
  */

  // Set up each basic block

  stmt_list & stmts = proc->body()->stmts();
  for (stmt_list_p p = stmts.begin();
       p != stmts.end();
       ++p)
    {
      basicblockNode * block = (basicblockNode *) *p;

      // -- Make a new basic block (including statements)

      basicblockLocation * block_loc = new basicblockLocation(this, block);

      // -- Store it

      _basicblocks[block] = block_loc;
    }

  progress_counter++;

  // -- Progress counter is now in the pointer analyzer
  // if (progress_counter % 100 == 0)
  //  cout << "Progress: " << progress_counter << " at " << *this << endl;

  // -- Set up the dominators

  for (basicblock_location_map_p p = _basicblocks.begin();
       p != _basicblocks.end();
       ++p)
    {
      basicblockNode * bb = (*p).first;
      basicblockLocation * block_loc = (*p).second;

      // -- The entry block is dominated by the procLocation

      if (bb == proc->entry())
	block_loc->set_dominator(this);
      else {

	// -- All other blocks are dominated by the last statement in the
	// dominating block.

	basicblockNode * dom = bb->parent();
	basicblockLocation * dom_loc = lookup_block(dom);
	block_loc->set_dominator(dom_loc->last());
      }
    }

  // -- For context-insensitive procedures, remove the structure from the
  // tree immediately and assign the proper subtree numbers.

  if (context_insensitive)
    remove_from_tree();
}

/*
Location * procLocation::parent() const
{
  return _parent;
}
*/

basicblockLocation * procLocation::lookup_block(basicblockNode * block) const
{
  basicblock_location_map_cp p = _basicblocks.find(block);
  return (*p).second; // Should never == _basicblocks.end()
}

procLocation * procLocation::parent_proc() const
{
  if (_parent) {
    stmtLocation * parent2 = (stmtLocation *) _parent;
    return parent2->block_location()->proc_location();
  }
  else
    return 0;
}

// -- Find the last statement

stmtLocation * procLocation::last()
{
  basicblockNode * the_exit = _proc->exit();
  basicblockLocation * the_exit_location = lookup_block(the_exit);
  return the_exit_location->last();
}

/** @brief Remove from tree
 *
 * Used to remove an existing branch and adjust the depths
 * appropriately. */

void procLocation::remove_from_tree()
{
  // -- Disconnect the parent

  _parent = 0;

  // -- Get a new subtree ID

  current_subtree_id++;
  _subtree_id = current_subtree_id;

  // -- Adjust the depth numbers and assign the new subtree ID

  adjust_depth();
}

void procLocation::adjust_depth()
{

#ifdef OLD_DOMINATOR_TEST

  // -- Adjust the depth here

  if (_parent)
    _depth = _parent->depth() + 1;
  else
    _depth = 0;

#endif


  if (_parent) {

    // -- Fix the subtree ID

    _subtree_id = _parent->subtree_id();
  }

  // -- Visit all the basic blocks

  for (basicblock_location_map_p p = _basicblocks.begin();
       p != _basicblocks.end();
       ++p)
    (*p).second->adjust_depth();
}

// Output

void procLocation::print(ostream & o) const
{

}

void procLocation::print_path(ostream & o) const
{
  if (stmt_location()) {
    stmt_location()->print_path(o);
    o << '/';
  }
  o << _proc->decl()->name();
}

// Destructor

procLocation::~procLocation()
{
  // Delete all the contained basic blocks

  for (basicblock_location_map_p p = _basicblocks.begin();
       p != _basicblocks.end();
       ++p)
    delete (*p).second;

  progress_counter--;

  // Handle the main

  if (this == Main)
    Main = 0;

  Location::proc_count--;
}
