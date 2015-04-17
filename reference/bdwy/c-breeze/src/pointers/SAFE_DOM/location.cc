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

#ifdef DOMINATOR_TREE

unsigned int Location::current_tree_number = 0;

#endif

Location::Location(Location * parent, LocationKind kind)
  : _parent(parent)
{
  _depth = parent ? parent->depth() + 1 : 0;
  _kind = kind;
  _dominates_exit = 0;
  _not_basicblock = (kind != BasicBlock);
  _not_procedure = (kind != Procedure);

  if (parent)
    _subtree_id = parent->subtree_id();

#ifdef DOMINATOR_TREE

  _dominator = 0;
  _num_children = 0;
  _tree_min = 0;
  _tree_max = 0;

#endif

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

  /*
  // Special case: main function entry point dominates everything

  if (dom == procLocation::main())
    return true;

  if (cur == procLocation::main())
    return false;
  */

  dom_calls++;

  bool new_dominates = false;
  bool old_dominates; // = old_strictly_dominates(dom, cur);

#ifdef DOMINATOR_TREE

  unsigned int dom_min = dom->tree_min();
  unsigned int dom_max = dom->tree_max();
  unsigned int cur_min = cur->tree_min();
  unsigned int cur_max = cur->tree_max();

  if (dom_max == 0) dom_max = numeric_limits<unsigned int>::max();
  if (cur_max == 0) cur_max = numeric_limits<unsigned int>::max();

  if ((dom_min < cur_min) && (cur_max <= dom_max))
    new_dominates = true;

  /*
  if (new_dominates != old_dominates) {
    cout << "TEST FAILED for " 
	 << * dom << "(" << dom->tree_min() << " - " << dom->tree_max() << ")" << endl;
    
    if (old_dominates)
      cout << "          DOM " << endl;
    else
      cout << "          NOT DOM " << endl;
    
    cout << "                " << * cur << "(" << cur->tree_min() << " - " << cur->tree_max() << ")" << endl;
  }
  */

#else

  new_dominates = old_strictly_dominates(dom, cur); // old_dominates;

#endif


  return new_dominates;
}
  
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

  int depth_dom = dom_p->depth();
  int depth_cur = cur_p->depth();
  int max_depth = (depth_dom > depth_cur) ? depth_dom : depth_cur;

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

  int dom_exit = 1;
  int local_dom_exit = 0;
  int not_procedure_boundary = 1;

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

  int depth_one = one_p->depth();
  int depth_two = two_p->depth();
  int max_depth = (depth_one > depth_two) ? depth_one : depth_two;

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

/*
procLocation * Location::find_proc(Location * current, const procNode * proc)
{
  Location * cur = current;

  while (cur && ((cur->proc() != proc) ||
		 (cur->kind() != Procedure)))
    cur = cur->parent();

  return (procLocation *) cur;
}
*/

static unsigned long int common_parent_total[20] = {0, 0, 0, 0, 0,
						    0, 0, 0, 0, 0,
						    0, 0, 0, 0, 0,
						    0, 0, 0, 0, 0};
static unsigned long int common_parent_count = 0;


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

#ifdef DOMINATOR_TREE

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
    /*
    // -- Retrieve the tree min from the parent or sibling, depending on
    // the situation.

    unsigned int max_val = dominator()->tree_max();
    if (max_val != 0) {

      // -- This means we stored the sibling value on the parent
      // temporarily.

      set_tree_min(max_val);

      cout << "sib tree_min = " << max_val << " for " << *this << " IDOM = "
	   << * dominator() << endl;

      dominator()->set_tree_max(0);
    }
    else {

      unsigned int min_val = dominator()->tree_min();
      if (min_val == 0) {

	// -- Search for a non-zero value

	Location * cur = dominator();
	while (cur) {
	  min_val = cur->tree_min();
	  if (min_val != 0)
	    break;

	  cur = cur->dominator();
	}
      }

      // -- This is the first (or only) child

      set_tree_min(min_val + 1);

      cout << "par tree_min = " << min_val + 1 << " for " << *this << " IDOM = "
	   << * dominator() << endl;
    }
    */

  }
}


void Location::finish()
{
  if ((tree_max() == 0) &&
      (num_children() == 0))
    {
      // -- Dominator leaf node: start setting tree max values

      int cur_max = tree_min();
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

	if (cur_node) {
	  cur_node->decrement_children();
	  if (cur_node->num_children() < 0)
	    cout << "THIS IS BAD: "<< cur_node->num_children() << " for " << * cur_node << endl;
	}

      } while (cur_node &&
	       (cur_node->num_children() == 0));

      /*
      while (cur_dom &&
	     (cur_dom->num_children() <= 1))
	{
	  cur_node->set_tree_max(next_tree_number());

	  if (cur_max == 0)
	    cur_max = cur_node->tree_min();

	  cur_node->set_tree_max(cur_max);

	  cout << "set tree_max = " << cur_max << " for " << * cur_node 
	       << " IDOM = " << * cur_dom << endl;

	  cur_max++;

	  cur_node = cur_dom;
	  cur_dom = cur_dom->dominator();
	}

      // -- Number the last node

      cout << "set tree_max = " << cur_max << " for " << * cur_node 
	   << " IDOM = " << * cur_dom << endl;

      cur_node->set_tree_max(cur_max);
      cur_max++;

      // -- Store the tree max of the last child on the parent temporarily.

      if (cur_dom) {
	cur_dom->set_tree_max(cur_max);
	cur_dom->decrement_children();
	cout << "Save tree max = " << cur_max << " at " << * cur_dom 
	     << " num-children = " << cur_dom->num_children() << endl;
      }
      */
    }
}

#endif

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

void stmtLocation::setup_call(procNode * proc,
			      bool is_context_insensitive)
{
  if (! _calls) {
    _calls = new procLocation(this, proc, is_context_insensitive);

#ifdef DOMINATOR_TREE

    // -- Set tree numbering on the new procLocation

    _calls->visit();

    // -- The new procedure is dominated by the old dominator of the
    // statement.

    _calls->set_dominator(dominator());

    // -- Reset the tree min (it will be fixed later)

    set_tree_min(0);

    /*
    cout << "un  tree_min = 0 for " << *this << " IDOM = "
	 << * dominator() << endl;
    */

    if ( ! is_context_insensitive) {

      // -- The statement itself is now dominated by the exit of the
      // procedure.

      set_dominator(_calls->last());
    }
    else {

      // -- Context-insensitive stub:

      set_dominator(_calls);

      // -- Visit the callsite now

      visit();
    }

#endif

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
    out = _calls;
    out->remove_from_tree();
    _calls = 0;

#ifdef DOMINATOR_TREE

    // -- Undo the dominator links from setup_call()

    set_dominator(out->dominator());
    out->set_dominator(0);

#endif

  }

  return out;
}

/** @brief Get the callNode
 *
 * If this statement is a procedure call, return the callNode. */

callNode * stmtLocation::callnode()
{
  stmtNode * the_stmt = stmt();
  callNode * call = 0;

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

  return call;
}

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
  // -- Adjust the depth here
  
  _depth = _parent->depth() + 1;

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
  o << ':' << _stmt_num;
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
  // Figure out if this block dominates the exit

  basicblockNode * exit = parent->proc()->exit();
  _dominates_exit = Dominators::dominates(block , exit);

#ifdef DOMINATOR_TREE

  // -- First statement is dominated by the basic block itself.

  Location * cur_dom = this;

#endif

  // Set all the statement numbers and stmtNode pointers

  stmt_list & stmts = block->stmts();
  int num = 0;

  for (stmt_list_p p = stmts.begin();
       p != stmts.end();
       ++p)
    {
      stmtLocation * cur_stmt = & _statements[num];
      cur_stmt->stmt_num(num);
      cur_stmt->stmt(*p);
      num++;

#ifdef DOMINATOR_TREE

      // -- Each subsequent statement is dominated by the previous
      // statement.

      cur_stmt->set_dominator(cur_dom);
      cur_dom = cur_stmt;

      // -- Regular statements dominate one other statement:

      cur_stmt->init_children(1);

#endif
    }

#ifdef DOMINATOR_TREE

  // -- The basic block itself has one child in the dominator tree.

  init_children(1);

  // -- The last statement *could* have multiple children, if this basic
  // block dominates others, or is the exit block.

  if (block != exit)
    last()->init_children(block->children().size());

#endif

  Location::block_count++;
  Location::stmt_count += block->stmts().size();
}

void basicblockLocation::adjust_depth()
{
  // -- Adjust the depth here

  _depth = _parent->depth() + 1;

  // -- Fix the subtree ID

  _subtree_id = _parent->subtree_id();

  // -- Visit the statements

  int size = _statements.size();

  for (int i = 0; i < size ; i++)
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

static int progress_counter = 0;

procLocation::procLocation(stmtLocation * parent,
			   procNode * proc,
			   bool is_context_insensitive)
  : Location(parent, Procedure),
    _proc(proc),
    _basicblocks()
{
  Location::proc_count++;

  if (! parent) {
    // The only function with no parent should be the main function
    Main = this;

    current_subtree_id = 0;
    _subtree_id = current_subtree_id;

#ifdef DOMINATOR_TREE

    // -- Main function starts the tree numbering at 1

    current_tree_number = 0;
    set_tree_min(next_tree_number());

#endif

  }

#ifdef DOMINATOR_TREE

  // -- The procLocation always dominates exactly one thing

  init_children(1);

#endif

  // -- At context sensitive callsites, just create a single node that
  // represents the call. We use this to attach uses and defs that pass
  // external inputs and outputs from the context.

  if (is_context_insensitive)
    return;

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

#ifdef DOMINATOR_TREE

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

#endif
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
  // -- Adjust the depth here

  if (_parent) {
    _depth = _parent->depth() + 1;

    // -- Fix the subtree ID

    _subtree_id = _parent->subtree_id();
  }
  else
    _depth = 0;

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
