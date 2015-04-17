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
#include <iomanip>
#include "proceduredb.h"
#include "memoryblock.h"

procedureDB::procedureDB()
  : Walker(Preorder, Subtree),
    _procedures(),
    _callgraph(0),
    _need_reanalysis(),
    _root(0),
    _callstack(),
    _worklist()
{}

procedureDB::~procedureDB()
{
  clear();
}

void procedureDB::build(procNode * root, Linker & linker)
{
  // Make sure the DB is empty
  // Don't do this: clear();

  if (pointerOptions::Verbose)
    cout << "procedureDB::build " << root->decl()->name() << endl;
    
  // -- Build the call graph

  _callgraph = new callGraph(root, linker);

  // -- Create entries for each procedure definition

  const proc_decl_map & procs = linker.procedures();
  for (proc_decl_map_cp p = procs.begin();
       p != procs.end();
       ++p)
    add_procedure((*p).second);

  // -- Populate the ancestors set for each procedure

  for (proc_info_map_p p = _procedures.begin();
       p != _procedures.end();
       ++p)
    {
      procNode * proc = (*p).first;
      procedureInfo * info = (*p).second;

      // -- Look up the callgraph node

      callGraphNode * node = _callgraph->lookup(proc);

      // -- Build the ancestor set

      callgraph_node_set cg_ancestors;
      node->ancestors(cg_ancestors);

      // -- Translate that set into a procedureInfo set

      procedureinfo_set ancestors;

      for (callgraph_node_set_p p = cg_ancestors.begin();
	   p != cg_ancestors.end();
	   ++p)
	{
	  callGraphNode * cg_ancestor = *p;

	  // -- Look up the ancestor and add it to the set

	  procedureInfo * ancestor = lookup(cg_ancestor->proc());
	  ancestors.insert(ancestor);
	}

      // -- Pass the ancestor set to the procedure

      info->add_ancestor_set(ancestors);

      // -- Mark recursive procedures

      if (ancestors.find(info) != ancestors.end()) {
	info->set_recursive();
	cout << " + " << info->qualified_name() << " is recursive" << endl;
      }
    }

  // -- Store the root

  _root = lookup(root);
}

void procedureDB::clear()
{
  for (proc_info_map_p p = _procedures.begin();
       p != _procedures.end();
       ++p)
    delete (*p).second;

  _procedures.clear();

  delete _callgraph;
  _callgraph = 0;
}

// Add a procedure to the database. This method is called during the
// construction of the procedure database, but can also be called for other
// procedures that are not immediately accessible, but will be needed later
// (e.g., in the Broadway compiler).

void procedureDB::add_procedure(procNode * proc)
{
  string & name = proc->decl()->name();

  if (pointerOptions::Verbose)
    cout << "procedureDB: Add procedure " << name << endl;

  _procedures[proc] = new procedureInfo(proc);
}

procedureInfo * procedureDB::lookup(procNode * proc)
{
  proc_info_map_p p = _procedures.find(proc);
  if (p != _procedures.end())
    return (*p).second;
  else
    return 0;
}

/** @brief Is recursive call?
 *
 * Return true if calling the given procedure would create a recursive
 * cycle in the call stack. */

bool procedureDB::is_recursive_call(procedureInfo * callee,
				    int & num_instances)
{
  // -- There are a couple of possible implementations of this routine. One
  // is to just see if the current top of the stack is an ancestor to the
  // callee. This will cause us to skip all recursive cycles. The other is
  // to just check to see if the given procedure is already in the call
  // stack. This could be much more expensive, but more accurate.

  bool result = false;
  num_instances = 0;

  // -- This is option 2:

  for (procedurecall_stack_p p = _callstack.begin();
       p != _callstack.end();
       ++p)
    {
      procedureInfo * current = (*p).second;
      if (current == callee) {
	result = true;
	num_instances++;
      }
    }

  if (result && ( ! callee->is_recursive())) {
    /*
    cout << "RECURSION ERROR: procedure " << callee->qualified_name()
	 << " -- callstack : ";
    print_call_stack(cout);
    cout << endl;
    */

    callee->set_recursive();
  }

  return result;
}

/** @brief Call a procedure
 *
 * This method is called when analysis is entering a new procedure. It
 * pushes the procedure on the stack and sets up the procedure for
 * analysis. */

void procedureDB::call_to(stmtLocation * callsite, procedureInfo * callee)
{
  // -- Push the newly called procedure on the callstack along with the
  // callsite.

  procedurecall_pair cp(callsite, callee);
  _callstack.push_back(cp);
}

/** @brief Return from a procedure
 *
 * This method is called when analysis of a procedure is complete. It
 * just pops the current procedure off the stack. */

void procedureDB::return_from()
{
  // -- Just pop the stack

  _callstack.pop_back();
}

/** @brief Handle recursive cycle
 *
 * Make all the procedures in the cycle context-insensitive. */

void procedureDB::make_cycle_context_insensitive(procedureInfo * callee)
{
  if ( ! callee->is_context_insensitive())
    callee->make_context_insensitive();

  return;

  /*

  for (procedurecall_stack_rp p = _callstack.rbegin();
       p != _callstack.rend();
       ++p)
    {
      procedureInfo * current = (*p).second;

      if (! current->is_context_insensitive())
	current->make_context_insensitive();

      current->set_recursive();

      if (current == callee)
	break;
    }
  */
}


/** @brief Set up analysis
 *
 * Add all procedures except the root to the _needs_analysis list. */

void procedureDB::setup_analysis()
{
  for (proc_info_map_p p = _procedures.begin();
       p != _procedures.end();
       ++p)
    {
      procedureInfo * info = (*p).second;
      if (info != _root)
	_need_reanalysis.insert(info);
    }
}

bool procedureDB::is_visible_to_caller(procedureInfo * info,
				       memoryBlock * block)
{
  // -- Short circuit: if the variable is not local to any procedure, then
  // it's always in accessible.

  procNode * proc = block->local_to();

  if ( ! proc)
    return true;

  if ( ! info)
    return false;

  // -- Another quick out: if the variable is local to the given procedure,
  // then it can't be visible to the caller.

  if (proc == info->proc())
    return false;

  // -- Look up the owning procedure

  procedureInfo * owner = lookup(proc);

  /* -- This doesn't work when we use the call chain (rather than the
  ancestor set) to control recursion

 // -- See if it's in the ancestor set

  const procedureinfo_set & ancestors = info->ancestors();

  procedureinfo_set_cp found = ancestors.find(owner);

  if (found != ancestors.end())
    return true;
  else
    return false;

  */

  // -- Inspect the call stack to see if the variable is owned by any other
  // procedures. Start at the bottom of the stack (earlier).

  procedurecall_stack_p p = _callstack.begin();
  while (p != _callstack.end()) {

    procedureInfo * cur = (*p).second;

    // -- If we find the given procedure, then the search failed

    if (cur == info)
      return false;

    // -- If we find the owner of the variable, return true

    if (cur == owner)
      return true;

    ++p;
  }

  /* Obsolete:
  // -- Look up the call chain to see if the variable is in scope

  procedureInfo * caller = info->current_caller();
  while (caller) {
    caller = caller->current_caller();
    if (caller == owner)
      return true;
  }
  */

  return false;
}

/** @brief Is variable visible?
 *
 * Check to see if the given variable could be accessed in the given
 * procedure, or in any of its ancestors in the callgraph. If it is, then
 * we need to pass it as an external output. */

bool procedureDB::is_visible_to(procedureInfo * info,
				memoryBlock * block)
{
  // -- Short circuit: if the variable is not local to any procedure, then
  // it's always in accessible.

  procNode * proc = block->local_to();

  if ( ! proc)
    return true;

  if ( ! info)
    return false;

  // -- Another quick out: if the variable is local to the given procedure:

  if (proc == info->proc())
    return true;

  // -- Look up the owning procedure

  procedureInfo * owner = lookup(proc);

 // -- See if it's in the ancestor set

  const procedureinfo_set & ancestors = info->ancestors();

  procedureinfo_set_cp found = ancestors.find(owner);

  if (found != ancestors.end())
    return true;
  else
    return false;
}

/** @brief Return the current caller
 *
 * Return the caller of the current procedure in the call stack. */

procedureInfo * procedureDB::current_caller()
{
  // -- Get the end of the call stack

  procedurecall_stack::reverse_iterator p = _callstack.rbegin();

  // -- Go to the second to last element

  p++;

  // -- If its not the rend, then return it

  if ( ! (p == _callstack.rend()) )
    return (*p).second;
  else
    return 0;
}

/** @brief Clear the call stack
 *
 * WARNING: Only use this routine at the end of an analysis pass. */

void procedureDB::clear_call_stack()
{
  _callstack.clear();

  // -- Initialize the call stack with the root procedure

  procedurecall_pair root_entry((stmtLocation *)0, _root);
  _callstack.push_back(root_entry);
}


void procedureDB::print_call_stack(ostream & out)
{
  bool first = true;

  if (_callstack.empty())
    out << "(empty)";
  else {

    for (procedurecall_stack_p p = _callstack.begin();
	 p != _callstack.end();
	 ++p)
      {
	stmtLocation * s = (*p).first;

	if (s) {
	  s->print_callsite(cout);
	  cout << '/';
	}
      }

    procedureInfo * top = _callstack.back().second;
    cout << top->name();
  }
}

void procedureDB::progress_meter(ostream & out)
{
  for (procedurecall_stack_p p = _callstack.begin();
       p != _callstack.end();
       ++p)
    {
      procedureInfo * current = (*p).second;

      out << current->qualified_name();
      cout << '/';
    }
}

/** @brief Mark procedure for reanalysis
 *
 * Record that particular procedure needs to be reanalyzed. Add all of it's
 * ancestors to the list as well to ensure that we actually reach it
 * again. */

void procedureDB::mark_for_reanalysis(procedureInfo * info,
				      stmtLocation * callsite,
				      bool include_self)
{

  pair< procedureinfo_set_p, bool> res;
  
  if (pointerOptions::Verbose) {
    cout << "  Mark for reanalysis: " << info->name();

    if (callsite)
      cout << " at " << *callsite << endl;
    else
      cout << " (no callsite)" << endl;
  }

  // -- Add the procedure itself, if need be.

  if (include_self)
    _need_reanalysis.insert(info);


  // -- New version: just provide a path to the right caller

  procedureInfo * current = info;
  while (current->first_caller()) {

    procedureInfo * first_caller = current->first_caller();

    res = _need_reanalysis.insert(first_caller);
    if (pointerOptions::Verbose) {
      if (res.second)
	cout << "      + " << first_caller->name() << endl;
      else
	cout << "      - " << first_caller->name() << endl;
    }

    current = first_caller;
  }

  /*
  // -- Add all of the procedures ancestors

  for (procedureinfo_set_cp q = info->ancestors().begin();
       q != info->ancestors().end();
       ++q)
    {
      res = _need_reanalysis.insert(*q);
      if (pointerOptions::Verbose) {
	if (res.second)
	  cout << "      + " << (*q)->name() << endl;
	else
	  cout << "      - " << (*q)->name() << endl;
      }
    }
  */
}

/** @brief Mark one procedure for reanalysis
 *
 * Add the given procedure to the list of those needing reanalysis. */

void procedureDB::mark_one_for_reanalysis(procedureInfo * info)
{
  // -- Add the procedure itself to the list

  pair< procedureinfo_set_p, bool> res = _need_reanalysis.insert(info);
} 

/** @brief Has reanalysis been forced?
 *
 * At a procedure call, check to see if this particular procedure is on the
 * _needs_reanalysis list, indicating that we cannot skip analyzing it,
 * even if the inputs appear not to have changed. */

bool procedureDB::is_reanalysis_required(procedureInfo * info)
{
  if (pointerOptions::Verbose)
    cout << "Is analysis for " << info->name() << " mandatory?" << endl;

  // -- Look for this callgraph edge

  procedureinfo_set_p p = _need_reanalysis.find(info);

  // -- Return true if it's found

  if (p != _need_reanalysis.end()) {
    _need_reanalysis.erase(p);

    // -- Extra test: if it's in the callstack more than once, then don't
    // force reanalysis. This will allow recursion to converge faster.
    /*
    int instances = 0;
    procedurecall_stack_p q = _callstack.begin();
    while (q != _callstack.end()) {

      procedureInfo * cur = (*q).second;

      if (cur == info) {
	instances++;

	if (instances > 1) {
	  if (pointerOptions::Verbose)
	    cout << " ....no (in recursion)" << endl;

	  return false;
	}
      }

      ++q;
    }
    */
    if (pointerOptions::Verbose)
      cout << " ....yes" << endl;

    return true;
  }
  else {

    if (pointerOptions::Verbose)
      cout << " ....no" << endl;

    return false;
  }
}

/** @brief Print left-overs */

void procedureDB::print_leftovers()
{
  cout << "Procedures that still need analysis:" << endl;
  for (procedureinfo_set_p p = _need_reanalysis.begin();
       p != _need_reanalysis.end();
       ++p)
    {      
      if ((*p)->analysis_count() > 0)
	cout << "  +" << (*p)->name() << endl;
    }
}

int procedureDB::times_called(procedureInfo * info)
{
  callGraphNode * node = _callgraph->lookup(info->proc());
  return node->times_called();
}

void  procedureDB::stats(ostream & out)
{

  if (pointerOptions::Show_procedures) {

    out.width(20);
    out << "Procedure";

    out.width(6);
    out << "CI";

    out.width(6);
    out << "Rec";

    out.width(8);
    out << "Sites";

    out.width(8);
    out << "X in";

    out.width(8);
    out << "X out";

    out.width(8);
    out << "Anc";

    out.width(8);
    out << "Calls";

    out.width(12);
    out << "Time (self)";

    out.width(12);
    out << "Time (total)";

    out.width(8);
    out << "Cont";

    out.width(8);
    out << "Size";

    out << endl;

    for (proc_info_map_p p = _procedures.begin();
	 p != _procedures.end();
	 ++p)
      (*p).second->stats(out);

    out.width(0);
  }
}

/** @brief Get the number of procedures */

void procedureDB::number_of_procedures(int & total, int & analyzed,
				       int & context_insensitive,
				       int & recursive,
				       int & program_size)
{
  total = 0;
  analyzed = 0;
  context_insensitive = 0;
  recursive = 0;
  program_size = 0;

  for (proc_info_map_p p = _procedures.begin();
       p != _procedures.end();
       ++p)
    {
      procedureInfo * info = (*p).second;

      total++;

      if (info->analysis_count() > 0) {
	analyzed++;

	if (info->is_context_insensitive()) {

	  context_insensitive++;

	  if (info->is_recursive())

	    recursive++;
	}

	program_size += info->procedure_size();
      }
    }
}


// ------------------------------------------------------------
// procedureInfo
// ------------------------------------------------------------

procedureInfo::procedureInfo(procNode * the_proc)
  : _proc(the_proc),
    _worklist(),
    _forward_worklist_order(),
    _forward_basicblock_order(),
    _backward_worklist_order(),
    _backward_basicblock_order(),
    _merge_points(),
    _dominance_frontiers(new DFPreds(the_proc)),
    _loops(new loopTree(the_proc)),
    _callsites(),
    _return_variable(0),
    _never_returns(false),
    _context_insensitive(false),
    _prefer_context_sensitive(false),
    _only_context(0),
    _external_inputs(),
    _external_outputs(),
    _ancestors(),
    _first_caller(0),
    _calls(),
    _is_recursive(false),
    _blocks_to_skip(),
    _active_edges(),
    _true_branches(),
    _analysis_count(0),
    _verbose(false)
{

  // -- Compute the forward ordering of the basic blocks (reverse
  // post-order)

  basicblock_set visited;
  basicblock_list order;

#ifdef DOMINATOR_TREE

  dfs_dominators(Forward, _proc->entry(), visited, order);

#else

  reverse_post_order(Forward, _proc->entry(), visited, order);

#endif

  int size = order.size();

  // -- Store that ordering with two mappings: a mapping from basic blocks
  // to their indices, and from indices back to basic blocks (this is just
  // a vector).

  _forward_basicblock_order.resize(size);
  int cur = 0;
  for (basicblock_list_p p = order.begin();
       p != order.end();
       ++p)
    {
      basicblockNode * block = *p;
      _forward_basicblock_order[cur] = block;
      _forward_worklist_order[block] = cur;
      cur++;
    }

  // -- Now do the same for the reverse control-flow graph

  visited.clear();
  order.clear();
  reverse_post_order(Backward, _proc->exit(), visited, order);

  _backward_basicblock_order.resize(size);
  cur = 0;
  for (basicblock_list_p p = order.begin();
       p != order.end();
       ++p)
    {
      basicblockNode * block = *p;
      _backward_basicblock_order[cur] = block;
      _backward_worklist_order[block] = cur;
      cur++;
    }  

  // Set the max size of the worklist

  _worklist.max_size(size);

  // -- Find the return value variable. It must be the argumen to the
  // return statement of the exit basic block.

  basicblockNode * bb = _proc->exit();
  stmtNode * statement = bb->stmts().back();

  if (statement->typ() == Return) {
    returnNode * ret = (returnNode *) statement;
    if (ret->expr() &&
	ret->expr()->typ() == Id)
      {
	idNode * id = (idNode *) ret->expr();
	_return_variable = id->decl();
      }
  }

  // -- Build the true branch mapping

  typedef map< stmtNode *, basicblockNode * > label_location_map;
  typedef label_location_map::iterator label_location_map_p;

  label_location_map labels;

  for (basicblock_list_p p = order.begin();
       p != order.end();
       ++p)
    {
      basicblockNode * block = *p;

      // -- Find any labels

      for (stmt_list_p q = block->stmts().begin();
	   q != block->stmts().end();
	   ++q)
	{
	  stmtNode * stmt = *q;

	  // -- Record where the labels are

	  if (stmt->typ() == Label) {
	    labels[stmt] = block;
	    labelNode * l = (labelNode *)stmt;
	  }
	}
    }

  for (basicblock_list_p p = order.begin();
       p != order.end();
       ++p)
    {
      basicblockNode * block = *p;

      // -- Find the if statement

      for (stmt_list_p q = block->stmts().begin();
	   q != block->stmts().end();
	   ++q)
	{
	  stmtNode * stmt = *q;

	  if (stmt->typ() == If) {
	    ifNode * cond = (ifNode *) stmt;
	    blockNode * branch = cond->true_br();
	    stmtNode * s = branch->stmts().front();
	    assert(s->typ() == Goto);
	    gotoNode * go = (gotoNode *) s;
	    labelNode * lab = go->label();
	    label_location_map_p ll = labels.find(lab);
	    if (ll == labels.end())
	      cout << "ERROR: No label " << lab->name() << " in " << name() << endl;
	    else
	      _true_branches[block] = (*ll).second;
	  }
	}
    }

  // -- Check to see if this procedure is preferred context sensitive

  str_set_p cs = pointerOptions::Context_sensitive_procedures.find(_proc->decl()->name());
  if (cs !=  pointerOptions::Context_sensitive_procedures.end())
    _prefer_context_sensitive = true;

  // -- Check to see if the procedure should be verbose

  str_set_p vb = pointerOptions::Verbose_procedures.find(_proc->decl()->name());
  if (vb !=  pointerOptions::Verbose_procedures.end())
    _verbose = true;
}

procedureInfo::~procedureInfo()
{
  delete _dominance_frontiers;
  delete _loops;
  delete _only_context;
}

void procedureInfo::add_ancestor_set(procedureinfo_set & ancestors)
{
  _ancestors = ancestors;
}

void procedureInfo::setup_call_at(procedureInfo * caller, 
				  stmtLocation * callsite)
{
  // -- Make this the current call site

  // _current_callsite = callsite;

  // -- Set up the call site

  callsite->setup_call(proc(), is_context_insensitive());

  // -- Store the callsite information

  _callsites[callsite] = caller;

  // -- Record this procedure as being called

  caller->_calls.insert(this);

  // -- See if this is the first caller

  if ( ! _first_caller)
    _first_caller = caller;
}

/** @brief Qualified name
 *
 * Includes either (I), (S), or (R) for context-insensitive,
 * context-sensitive, or recursive. */

string procedureInfo::qualified_name()
{
  string nm(name());

  if (is_context_insensitive()) {
    if (is_recursive())
      nm += "(R)";
    else
      nm += "(I)";
  }
  else
    nm += "(S)";

  if (never_returns())
    nm += "!";

  char temp[200];

  sprintf(temp, "[%d,%d,%d]", _callsites.size(),
	  _external_inputs.size(),
	  _external_outputs.size());

  nm += temp;

  return nm;
}

/** @brief Return from the procedure
 *
 * Indicated that we're returning from analyzing a procedure. Reset the
 * current_callsite to prevent confusion.

void procedureInfo::return_from()
{
  if (_current_callsite == 0)
    cout << "ERROR: Current callsite is null" << endl;

  // -- Print some info

  if (pointerOptions::Verbose) {
    if (is_context_insensitive()) {
      cout << "------------------------------------------------------------" << endl;
      cout << "   Return from : ";
      print_call_stack(cout);
      cout << endl;
      cout << "------------------------------------------------------------" << endl;
      cout << endl;
    }
  }
  
  // -- Reset the callsite

  _current_callsite = 0;
}
*/

/** @brief Return the current caller
 *
 * Returns null is there is none.

procedureInfo * procedureInfo::current_caller()
{
  if (_current_callsite)
    return _callsites[_current_callsite];

  return 0;
}
*/

/** @brief Set pending changes
 *
 * In context-sensitive analysis when we pass external outputs back to
 * other callsites, we need to collect those changes so that the calling
 * procedure will see them when it's analyzed. This routine stores that
 * set of changes for the caller according to the callsite. */

void procedureInfo::set_pending_changes(stmtLocation * callsite, memoryblock_set & changes)
{
  // -- Add the given changes into the pending changes set

  _pending_changes[callsite].insert(changes.begin(), changes.end());
}

/** @brief Get pending changes
 *
 * Check the current callsite to see if changes were left behind by a
 * previous analysis in a different context. If so, get the changes now
 * and merge them in. */

void procedureInfo::get_pending_changes(stmtLocation * callsite, memoryblock_set & changes)
{
  // -- Find the given callsite

  callsite_changes_map_p p = _pending_changes.find(callsite);

  if (p != _pending_changes.end()) {

    // -- Found some changes, merge them in

    memoryblock_set & new_changes = (*p).second;
    changes.insert(new_changes.begin(), new_changes.end());

    // -- Erase the entry

    _pending_changes.erase(p);
  }
}

/** @brief Return the caller
 *
 * Given a callsite, return the caller at that callsite. */

procedureInfo * procedureInfo::caller_at(stmtLocation * callsite)
{
  callsite_map_p p = _callsites.find(callsite);
  if (p != _callsites.end())
    return (*p).second;

  return 0;
}

/** @brief Is ancestor?
 *
 * Return true if the given procedure is an ancestor in the call graph. */

bool procedureInfo::is_ancestor(procedureInfo * other)
{
  procedureinfo_set_p p = _ancestors.find(other);

  return (p != other->_ancestors.end());
}

/** @brief Get the procedure location
 *
 * For a given callsite, return the corresponding procLocation of the
 * called procedure. For context-insensitive procedures, we just return the
 * one context. */

procLocation * procedureInfo::procedure_location(stmtLocation * callsite)
{
  if (is_context_insensitive())
    return _only_context;

  return callsite->calls();
}

void procedureInfo::make_context_insensitive()
{
  // -- Already context insensitive, nothing to do

  if (is_context_insensitive())
    return;

  // -- We can only make a procedure context insensitive when there is
  // exactly one call site. It becomes the only context.

  if (_callsites.size() == 1) {

    // -- Get the one context seen so far

    stmtLocation * first_callsite = (* _callsites.begin()).first;

    if (pointerOptions::Verbose)
      cout << ">>> Make procedure " << name() << " context insensitive..." << endl << endl;

    _context_insensitive = true;

    // -- Set up the only context by removing the procLocation subtree that
    // we built.

    _only_context = first_callsite->remove_call();

    // -- Create the dummy procLocation at the call site

    first_callsite->setup_call(proc(), true);
  }
}

// ----------------------------------------------------------------------
//  Manage external inputs and outputs
// ----------------------------------------------------------------------

bool procedureInfo::add_external_input(memoryBlock * block)
{
  // -- Each use with an external reaching def is considered an input

  if (_external_inputs.find(block) == _external_inputs.end()) {

    _external_inputs.insert(block);

    if (pointerOptions::Verbose)
      cout << "   Added external input " << block->name() << endl;

    return true;
  }
  else
    return false;
}

bool procedureInfo::add_external_output(memoryBlock * block)
{
  bool found_new_output = false;

  // -- Each externally visible def is considered an output

  if (_external_outputs.find(block) == _external_outputs.end()) {

    _external_outputs.insert(block);

    if (pointerOptions::Verbose) {
      cout << "   New external output " << block->name() << endl;
    }

    // -- Also, make sure weak updates are represented in the
    // external inputs list.

    if (block->current_def() &&
	(block->current_def()->is_weak()))
      {
	if (add_external_input(block)) {
	  found_new_output = true;
	  if (pointerOptions::Verbose)
	    cout << "    (Caused by weak update)" << endl;
	}
      }

    return found_new_output;
  }
  else
    return false;
}

// ----------------------------------------------------------------------
//  Merge point management
// ----------------------------------------------------------------------

void procedureInfo::setup_merge_point(memoryBlock * block_to_merge,
				      basicblockLocation * cur)
{
  // -- Flow insensitive objects have no merge points

  if ( ! block_to_merge->is_flow_sensitive())
    return;

  // -- Don't merge temporaries: crap, this doesn't work for the ternary
  // operator.

  /*
  const char * nm = block_to_merge->decl()->name().c_str();
  if ((strncmp(nm, "__TE", 4) == 0) ||
      (strncmp(nm, "__IE", 4) == 0))
    return;
  */

  procLocation * proc_loc = cur->proc_location();

  // --- First, find all the places where merge points should be
  // (dominance frontier).

  bool found_dominance_frontier = false;

  basicblock_set_map_map_p p = _dominance_frontiers->find(cur->block());
  if ((p != _dominance_frontiers->end()) &&
      ! (*p).second.empty()) {

    found_dominance_frontier = true;

    // --- Found a dominance frontier entry

    basicblock_set_map & df = (*p).second;

    // --- Visit each entry in the dominance frontier. This formulation
    // includes the set of predecessors.
    
    for (basicblock_set_map_p bbq = df.begin();
	 bbq != df.end();
	 ++bbq)
      {
	basicblockNode * merge_basicblock = (*bbq).first;
	basicblockLocation * merge_location = proc_loc->lookup_block(merge_basicblock);
	basicblock_set & predecessors = (*bbq).second;

	// -- Get the reaching def. If it occured inside a called
	// procedure, then the current_def field will be null and we'll
	// need to find the reaching def.

	memoryDef * cur_def = block_to_merge->current_def();
	memoryDef * def;

	if ( cur_def )
	  def = cur_def;
	else
	  def = block_to_merge->last_def_at(cur);

	// -- Set up the merge point: create the special phi uses.
	    
	block_to_merge->setup_merge_uses_at(merge_location, def, predecessors);
	_merge_points[merge_location].insert(block_to_merge);
	    
	if (pointerOptions::Verbose)
	  cout << "      Merge " << block_to_merge->name() << " at "
	       << * merge_location << endl;
      }
  }

  if (pointerOptions::Verbose && found_dominance_frontier)
    cout << endl;
}

// --- Find the set of objects to be merged at a particular basic
// block entry point.

memoryblock_set * procedureInfo::lookup_merge_point(basicblockLocation * merge_location)
{
  if (pointerOptions::Verbose) {
    cout << " Lookup merge: " << *merge_location << endl;
  }

  mergepoint_map_p p = _merge_points.find(merge_location);
  if (p == _merge_points.end()) {
    if (pointerOptions::Verbose) {
      cout << "    None found." << endl;
      cout << endl;
    }
    return 0;
  }
  else {
    if (pointerOptions::Verbose) {
      memoryblock_set & objects = (*p).second;
      for (memoryblock_set_cp q = objects.begin();
	   q != objects.end();
	   ++q)
	cout << "    Merge: " << (*q)->name() << endl;
    }

    if (pointerOptions::Verbose)
      cout << endl;

    return &((*p).second);
  }
}

void procedureInfo::check_merge_point(memoryBlock * block_to_merge,
				      basicblockLocation * cur)
{
  // -- Flow insensitive objects have no merge points

  if ( ! block_to_merge->is_flow_sensitive())
    return;

  procLocation * proc_loc = cur->proc_location();

  // --- First, find all the places where merge points should be
  // (dominance frontier).

  bool found_dominance_frontier = false;

  basicblock_set_map_map_p p = _dominance_frontiers->find(cur->block());
  if ((p != _dominance_frontiers->end()) &&
      ! (*p).second.empty()) {

    found_dominance_frontier = true;

    // --- Found a dominance frontier entry

    basicblock_set_map & df = (*p).second;

    // --- Visit each entry in the dominance frontier. This formulation
    // includes the set of predecessors.
    
    for (basicblock_set_map_p bbq = df.begin();
	 bbq != df.end();
	 ++bbq)
      {
	basicblockNode * merge_basicblock = (*bbq).first;
	basicblockLocation * merge_location = proc_loc->lookup_block(merge_basicblock);
	basicblock_set & predecessors = (*bbq).second;

	// -- Get the reaching def. If it occured inside a called
	// procedure, then the current_def field will be null and we'll
	// need to find the reaching def.

	memoryDef * cur_def = block_to_merge->current_def();
	memoryDef * def;

	if ( cur_def )
	  def = cur_def;
	else
	  def = block_to_merge->last_def_at(cur);

	// -- This is the check: there had better be a def at the
	// merge-point by now.

	/** Allow this for now because it can happen in Broadway...
	memoryDef * merge_def = block_to_merge->find_def_at(merge_location);
	if (! merge_def) {
	  cout << "ERROR at setup-merge: found a new merge for " << block_to_merge->name() << endl;
	  cout << "   Current location is : " << * cur << endl;
	  cout << "   Merge location is : " << * merge_location << endl;
	  if (cur_def)
	    cout << "  Cur def at : " << * (cur_def->where()) << endl;
	  else
	    cout << "  (No current def)" << endl;
	  if (def)
	    cout << "  Last def at " << * (def->where()) << endl;
	}
	*/
      }
  }

  if (pointerOptions::Verbose && found_dominance_frontier)
    cout << endl;
}

// ----------------------------------------------------------------------
//   Worklist management
// ----------------------------------------------------------------------

void procedureInfo::reverse_post_order(Direction dir,
				       basicblockNode * cur,
				       basicblock_set & visited,
				       basicblock_list & order)
{
  // If the node is not already there, then process it.

  if (visited.find(cur) == visited.end()) {
    visited.insert(cur);

    // For post-order, we process the children first. Note that we visit
    // the children in *reverse* order because we're building the list from
    // back to front.

    // -- Choose successors or predecessors depending on the direction

    basicblock_list & children = (dir == Forward) ? cur->succs() : cur->preds();

    // -- Improvement: use the loopTree object to make sure we visit back-edges first.

    for (basicblock_list::reverse_iterator p = children.rbegin();
	 p != children.rend();
	 ++p)
      {
	basicblockNode * child = *p;
	if (_loops->classifyEdge(cur, child) != loopTree::BackEdge)
	  reverse_post_order(dir, child, visited, order);
      }

    for (basicblock_list::reverse_iterator p = children.rbegin();
	 p != children.rend();
	 ++p)
      {
	basicblockNode * child = *p;
	if (_loops->classifyEdge(cur, child) == loopTree::BackEdge)
	  reverse_post_order(dir, child, visited, order);
      }

    // For reverse post-order, put the parent on the front of the
    // list.

    order.push_front(cur);
  }
}

void procedureInfo::dfs_dominators(Direction dir,
				   basicblockNode * cur,
				   basicblock_set & visited,
				   basicblock_list & order)
{
  // If the node is not already there, then process it.

  if (visited.find(cur) == visited.end()) {
    visited.insert(cur);
    order.push_back(cur);

    basicblockNode * last = 0;

    for (basicblock_list_p p = cur->children().begin();
	 p != cur->children().end();
	 ++p)
      {
	basicblockNode * child = *p;
	
	if (Dominators::dominates(child, proc()->exit()))
	  last = child;
	else
	  dfs_dominators(dir, child, visited, order);
      }

    if (last)
      dfs_dominators(dir, last, visited, order);
  }
}

basicblockNode * procedureInfo::get_next_block(Direction dir)
{
  // When empty, return 0

  if (_worklist.is_empty())
    return 0;

  basicblockNode * next_block = block_at(_worklist.get_next_block(), dir);

  // cout << "WORKLIST: Get next block " << next_block->dfn() << endl;

  return next_block;
}

void procedureInfo::add_block(basicblockNode * block, Direction dir)
{
  // Figure out where the bit should be

  int position = block_position(block, dir);

  // Add it to the worklist

  _worklist.add_block(position);

  // Remove it from the skip-list

  basicblock_set_p p = _blocks_to_skip.find(block);
  if (p != _blocks_to_skip.end())
    _blocks_to_skip.erase(p);

  // cout << "[ block=" <<  block->dfn() << ", position=" << position << " ] ";

  // cout << "WORKLIST: Add block " << block->dfn() << " at position " << position << endl;
}

void procedureInfo::add_reachable_blocks(basicblockNode * block, Direction dir)
{
  // Make a temporary list for the depth-first search

  worklist_set temp;

  // Use the temporary list to perform a depth-first search, marking nodes
  // in the real worklist.

  // cout << "WORKLIST: add: ";

  add_reachable_blocks_rec(dir, block, temp, true);

  // _worklist.skip_current_block();

  // cout << endl;

  // cout << "WORKLIST: add_reachable_blocks, cur_size = " << _cur_size << endl;
}

void procedureInfo::add_reachable_blocks_rec(Direction dir,
					     basicblockNode * cur,
					     worklist_set & temp, bool first)
{
  // Figure out where the bit should be

  int position = block_position(cur, dir);

  // Stop recursion when we find a bit set already

  if ( temp.test(position))
    return;

  // Skip the root basic block

  if (! first)
    add_block(cur, dir);

  // Update the temp to avoid visiting duplicates (also skip the root)

  if (! first)
    temp.set(position);

  // Add all successors or predecessors, depending on the direction argument

  basicblock_list & children = (dir == Forward) ? cur->succs() : cur->preds();

  for (basicblock_list_p p = children.begin();
       p != children.end();
       ++p)
    {
      basicblockNode * cur_child = *p;
      add_reachable_blocks_rec(dir, cur_child, temp, false);
    }
}

void procedureInfo::add_all_blocks()
{
  _worklist.add_all_blocks();

  _blocks_to_skip.clear();

  _active_edges.clear();
}      

void procedureInfo::add_start_block(Direction dir)
{
  // -- Depending on the direction, add either the entry node, or the exit
  // node.

  if (dir == Forward)
    add_block(proc()->entry(), dir);
  else
    add_block(proc()->exit(), dir);

  // cout << "WORKLIST: add_entry_block, cur_size = " << _cur_size << endl;
}

bool procedureInfo::is_empty() const
{
  return _worklist.is_empty();
}

/** @brief Remove branch
 *
 * This routine removes basic blocks that will not be visited when the
 * direction of a branch can be determined. */

void procedureInfo::remove_branch_blocks(basicblockNode * cond,
					 basicblockNode * branch_taken,
					 basicblockNode * branch_not_taken)
{
  // -- Remove all blocks that are dominated by the branch not taken,
  // unless they are also dominated by the branch taken.

  int size = _forward_basicblock_order.size();

  for (int i = 0; i < size; i++) {

    basicblockNode * block = _forward_basicblock_order[i];

    if (Dominators::dominates(branch_not_taken, block) &&
	( ! Dominators::dominates(cond, block)))
      {
	// -- Figure out where the bit should be

	int position = block_position(block, Forward);

	// -- Remove it from the list

	_worklist.remove_block(position);

	// -- Add it to the skip list

	_blocks_to_skip.insert(block);
      }
  }

  // -- Add the conditional block itself to the list: this fixes the "if"
  // with no else problem.

  _blocks_to_skip.insert(cond);
}

/** @brief Conditional worklist algorithm
 *
 * */

void procedureInfo::update_conditional_worklist(basicblockNode * block,
						bool has_truth_value,
						bool which_branch)
{
  if (pointerOptions::Verbose)
    cout << "Conditional analysis:" << endl;

  if (has_truth_value &&
      (block->succs().size() == 2)) {

    // -- Get the two branches

    true_branch_map_p p = _true_branches.find(block);
    if (p == _true_branches.end()) {
      cout << "ERROR: at basic block " << block->dfn() << " could not find true branch" << endl;
      return;
    }

    basicblockNode * true_branch = (*p).second;

    basicblockNode * false_branch;
    basicblockNode * one = block->succs().front();
    basicblockNode * two = block->succs().back();

    if (true_branch == one)
      false_branch = two;
    else
      false_branch = one;
  
    // -- One of the branch is definitely taken...

    basicblockNode * branch_taken = 0;
    basicblockNode * branch_not_taken = 0;

    if (which_branch) {

      // -- Only execute the true branch: remove the false branch blocks

      branch_taken = true_branch;
      branch_not_taken = false_branch;
    }
    else {

      // -- Only execute the false branch: remove the true branch blocks
    
      branch_taken = false_branch;
      branch_not_taken = true_branch;
    }

    // -- Only add the edge for the taken branch

    cfg_edge_pair edge(block, branch_taken);
    _active_edges.insert(edge);

    if (pointerOptions::Verbose) {
      cout << "  + Add edge: " << block->dfn() << " -> " << branch_taken->dfn() << endl;
      cout << "    (skipping: " <<  block->dfn() << " -> " << branch_not_taken->dfn() << ")" << endl;
    }

    // -- Remove the blocks that make up the false branch, if it's not
    // active

    cfg_edge_pair edge_not_taken(block, branch_not_taken);
    cfg_edge_set_p q = _active_edges.find(edge_not_taken);

    if (q == _active_edges.end())
      remove_branch_blocks(block, branch_taken, branch_not_taken);
  }
  else {

    if (pointerOptions::Verbose)
      cout << "  - No constant branch: add all edges:" << endl;

    // -- Can't tell which branch, or not branch at all: add all the
    // outgoing edges

    for (basicblock_list_p p = block->succs().begin();
	 p != block->succs().end();
	 ++p)
      {
	basicblockNode * succ = *p;

	cfg_edge_pair edge(block, succ);
	_active_edges.insert(edge);

	if (pointerOptions::Verbose)
	  cout << "  + Add edge: " << block->dfn() << " -> " << succ->dfn() << endl;
      }
  }
}


int procedureInfo::block_position(basicblockNode * block, Direction dir)
{
  // -- Choose which work list ordering, based on the direction

  worklist_order_map & ordering = (dir == Forward) ? _forward_worklist_order : _backward_worklist_order;

  // -- Look for the block and return it's position

  worklist_order_map_p p = ordering.find(block);
  if (p == ordering.end()) {

    cout << "---------------------------------------------------------" << endl;
    cout << "Looking for block: " << block << endl;
    output_context oc(cout);
    block->output(oc, (Node *)0);

    cout << "---------------------------------------------------------" << endl;
    cout << "Found:" << endl;

    for (p = ordering.begin();
	 p != ordering.end();
	 ++p)
      {
	basicblockNode * bb = (*p).first;

	cout << bb << endl;
	bb->output(oc, (Node *)0);
      }

    return -1;
  }
  else
    return (*p).second;
}

basicblockNode * procedureInfo::block_at(int position, Direction dir)
{
  if (dir == Forward)
    return _forward_basicblock_order[position];
  else
    return _backward_basicblock_order[position];
}

void procedureInfo::print(ostream & out)
{
  out << "Procedure: " << _proc->decl()->name() << endl;
}

/** @brief Stats
 *
 * Output a whole bunch of statistics about this procedure (particularly
 * useful after analysis is complete). */

void procedureInfo::stats(ostream & out)
{
  out << std::setw(20) << _proc->decl()->name().c_str();

  if (is_context_insensitive())
    out << std::setw(6) << "true";
  else
    out << std::setw(6) << "false";

  if (_is_recursive)
    out << std::setw(6) << "true";
  else
    out << std::setw(6) << "false";

  out << std::setw(8) << _callsites.size();
  out << std::setw(8) << _external_inputs.size();
  out << std::setw(8) << _external_outputs.size();
  out << std::setw(8) << _ancestors.size();
  out << std::setw(8) << _calls.size();

  char temp[100];
  sprintf(temp, "%10.4f %10.4f", self_timer(), total_timer());

  out << std::setw(20) << temp;

  out << std::setw(8) << count_calling_contexts();
  out << std::setw(8) << procedure_size();

  out << endl;

  /*
  for (procedureinfo_set_p p = _calls.begin();
       p != _calls.end();
       ++p)
    {
      procedureInfo * called =*p;
      out << "      " << called->proc()->decl()->name();
    }
  */
}

/** @brief Count contexts
 *
 * Count the number of calling contexts for the given procedure. */

int procedureInfo::count_calling_contexts()
{
  proc_int_map counts;

  /*
  cout << "COUNT " << qualified_name() << endl;
  string indent(" ");
  */
  return count_calling_contexts_rec(counts, (stmtNode *)0);
}

/** @brief Count contexts
 *
 * Count the number of calling contexts for the given procedure. */

int procedureInfo::count_calling_contexts_rec(proc_int_map & counts,
					      stmtNode * callsite)
{
  // -- Special case: always return 1 for recursive procedures.

  if (is_recursive()) {
    /*
    cout << indent << " + " << qualified_name() << " = 1 (recursive)";
    if (callsite)
      cout << " at " << callsite->coord() << endl;
    else
      cout << " (no callsite)" << endl;
    */
    return 1;
  }

  // -- If we've already computed the count for this procedure, then return
  // it immediately

  proc_int_map_p p = counts.find(this);
  if (p !=  counts.end()) {
    /*
    cout << indent << " + " << qualified_name() << " = " << (*p).second << " (already seen)";
    if (callsite)
      cout << " at " << callsite->coord() << endl;
    else
      cout << " (no callsite)" << endl;
    */
    return (*p).second;
  }

  // -- Get the list of call sites

  const callsite_map & sites = callsites();

  // -- Base-case: main has only one context

  if (sites.empty())
    return 1;

  // -- For all other procedures, the number of contexts is the sum of the
  // number of contexts for each call site. We have to be careful here: a
  // callsite in this situation is not a stmtLocation; it's a particular
  // stmtNode. So, first build a list of actual call sites.

  typedef pair< procedureInfo *, stmtNode * > call_site_pair;
  typedef set< call_site_pair> stmt_set;
  typedef stmt_set::iterator stmt_set_p;

  stmt_set stmts;

  for (callsite_map_cp q = sites.begin();
       q != sites.end();
       ++q)
    {
      procedureInfo * caller = (*q).second;
      stmtNode * stmt = ((*q).first)->stmt();

      call_site_pair call_site(caller, stmt);
      stmts.insert(call_site);
    }

  // string indent2 = indent + "  ";
  int total = 0;

  counts[this] = total;

  for (stmt_set_p w = stmts.begin();
       w != stmts.end();
       ++w)
    {
      procedureInfo * caller = (*w).first;
      stmtNode * call_site = (*w).second;

      total += caller->count_calling_contexts_rec(counts, call_site);
    }

  // -- Store the count and return it

  counts[this] = total;

  /*
  cout << indent << " + " << qualified_name() << " = " << total;
  if (callsite)
    cout << " at " << callsite->coord() << endl;
  else
    cout << " (no callsite)" << endl;
  */

  return total;
}

  /** @brief Procedure size
   *
   * Count the number of statements in the dismantled form of the
   * procedure. */

int procedureInfo::procedure_size()
{
  blockNode * body = proc()->body();

  int total = 0;

  for (stmt_list_p p = body->stmts().begin();
       p != body->stmts().end();
       ++p)
    {
      basicblockNode * bb = (basicblockNode *) (*p);

      total += bb->stmts().size();
    }

  return total;
}

	   

