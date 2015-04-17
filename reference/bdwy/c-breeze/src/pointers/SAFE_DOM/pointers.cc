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
#include "print_walker.h"
#include "pointers.h"

long int Pointers::call_counter;

// ------------------------------------------------------------
//  Pointer options
// ------------------------------------------------------------

/** @brief Context-sensitivity control */

bool pointerOptions::Context_insensitive = false;

/** @brief Use multiplicity analysis */

bool pointerOptions::Use_multiplicity = true;

/** @brief Verbose output */

bool pointerOptions::Verbose = false;
  
/** @brief Show memory blocks
 *
 * At the end of pointer analysis print out a list of all the
 * memoryblocks along with information about their properties, including
 * numbers of defs and uses. */

bool pointerOptions::Show_memoryblocks = false;

/** @brief Show procedures
 *
 * At the end of pointer analysis print out a list of all the procedures
 * and information about them. */

bool pointerOptions::Show_procedures = false;

/** @brief Pointer statistics */

bool pointerOptions::Pointer_statistics = false;

/** @brief Ignore struct/union fields
 *
 * The "dot" operator becomes a no-op. This also forces all updates to the
 * structure to be weak (as if the multiplicity were high). */

bool pointerOptions::Ignore_fields = false;

/** @brief One struct per type
 *
 * With this flag on, there is only one set of memory blocks for each
 * structure type. This emulates the situation in the type-system
 * world. */

bool pointerOptions::One_struct_per_type = false;

/** @brief Show the stack state */

bool pointerOptions::Show_stack = false;

/** @brief Flow-insensitive analysis
 *
 * This is implemented in the following way: (1) every object has only one
 * def that occurs at the main function (and therefore dominates
 * everything), (2) all assignments are additive, (3) we don't perform any
 * control-flow merges (phi functions), and (4) all heap allocations are
 * immediately Unbounded in size. */

bool pointerOptions::Flow_insensitive = false;

/** @brief Conditional analysis
 *
 * When set to true this flag causes the analyzer to skip branches when it
 * can ascertain the value of the condition. */

bool pointerOptions::Conditional_analysis = false;

/** @brief Bi-directional assignment
 *
 * This options is intended to emulate equality-based type analysis. It
 * causes all assignments (including parameter passing) to be
 * bi-directional. That is, for an assignment "x = y" we also perform
 * "y = x". */

bool pointerOptions::Bidirectional_assignment = false;

/** @brief Aggressive multiplicity
 *
 * With this option turned on, we use a trick that could be unsafe in
 * some cases, but it's very unlikely. The observation is that if a
 * memoryBlock is allocated and then a subsequent if-then-else frees it
 * on one branch, but not the other, then it is probably the case that
 * either way it is unallocated. For example:
 *
 *  f = fopen(...);
 *  if (f) {
 *    ...
 *    fclose(f);
 *  }
 *
 *  At the end of this code, the file is always closed because in the
 *  case that f is null, the file was never opened in the first place. */

bool pointerOptions::Aggressive_multiplicity = false;

/** @brief One string constant
 *
 * When set true, we only create a single object to represent all string
 * constants. Since we never look at the value, and we don't allow them to
 * be changed, this should not affect our results. */

bool pointerOptions::One_string_constant = true;

/** @brief Context-insensitive memory model
 *
 * When true, all objects, including variables and heap objects, are only
 * instantiated once for each calling context regardless of the analysis
 * mode. */

bool pointerOptions::Context_insensitive_memory = false;

/** @brief Show constants
 *
 * During constant propagation, show constant values being computed. */

bool pointerOptions::Verbose_constants = false;

/** @brief Verbose liveness
 *
 * During liveness analysis, show liveness computations. */

bool pointerOptions::Verbose_liveness = false;

/** @brief Show unknown procedures
 */

bool pointerOptions::Show_unknown_procedures = false;

  /** @brief Monitor precision
   *
   * When true, the analysis engine monitors precision and builds a graph
   * of the causes and effects of precision loss. */

bool pointerOptions::Monitor_precision = false;

/** @brief Context sensitive procedures
 *
 * Populate this list with procedures that are to be treated context
 * sensitive. */

str_set pointerOptions::Context_sensitive_procedures;

/** @brief Flow-sensitive objects
 *
 * Populate this list with variables that are to be treated flow
 * sensitive. */

flow_sensitive_set pointerOptions::Flow_sensitive_objects;

/** @brief Path-sensitive branches
 *
 * Record the basic block that controls path sensitive information --
 * that is, the branch at the end of the basic block needs to be path
 * sensitive.*/

basicblock_set pointerOptions::Path_sensitive_branches;

/** @brief Verbose procedures
 *
 * Turn on verbose output when we're analyzing these procedures. */

str_set pointerOptions::Verbose_procedures;



// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

Pointers::Pointers(procNode * root, unitNode * unit)
  : Memory(),
    HeapMap(),
    Root(root),
    root_location(0),
    linker(),
    Procedures(),
    _constants(),
    Problem(0),
    computePointers(true),
    _direction(Forward),
    _procedureCount(0),
    _skipCount(0),
    _show_progress(true),
    _use_multiplicity(true),
    _unknown_procedures(),
    _def_accuracy(),
    _star_accuracy()
{
  _constants.set_null_object(Memory.null());
}

Pointers::~Pointers()
{
  delete root_location;

  for (heap_map_p p = HeapMap.begin();
       p != HeapMap.end();
       ++p)
    {
      delete (*p).second;
    }
}

// ------------------------------------------------------------
// Analyze
// ------------------------------------------------------------

void Pointers::analyze()
{
  // -- Check to see if this is a re-analysis

  if (root_location) {

    // -- It is, so make sure all the data structures are clear.

    // -- Delete all memory blocks

    Memory.clear();

    // -- Clear the heap mapping

    HeapMap.clear();

    // -- Reset the computePointers flag

    computePointers = true;

    // -- Make sure the direction of analysis is Forward

    _direction = Forward;

    // -- Delete all the location objects

    delete root_location;

    // -- Clear the user-defined problem

    Problem = 0;

    // -- All other components will reinitialize themselves
  }

  // -- Run the linker to build a global symbol table

  linker.link();

  // -- Build the procedure database

  Procedures.build(Root, linker);

  // -- Set up the main() location (must be done *after* build above).

  root_location = new procLocation(0, Root);

  // -- Run the analyzer

  memoryblock_set external_defs;
  memoryuse_set external_uses;
  memoryblock_set external_changes;
  memoryblock_set nonlocal_changes;
  _procedureCount = 0;
  _skipCount = 0;
  int iterations = 0;
  call_counter = 0;

  if (_show_progress)
    cout << "  ==== Analyze Pointers ====" << endl;

  procedureInfo * root_info = Procedures.lookup(root_location->proc());

  Procedures.setup_analysis();

  bool done = false;

  pointerValue main_return;

  // -- This is a chance for a subclass to run some initialization

  initialize();

  // -- Run the analysis

  do {

    Procedures.call_to((stmtLocation *)0, root_info);

    root_info->start_self_timer();
    root_info->start_total_timer();

    analyze_procedure(root_location, 
		      external_defs,
		      external_uses,
		      external_changes,
		      nonlocal_changes);

    Procedures.return_from();

    root_info->stop_self_timer();
    root_info->stop_total_timer();

    iterations++;

    bool needs_reanalysis = Procedures.is_reanalysis_required(root_info);

    done = ( ! needs_reanalysis) || (iterations > 10);

    // Procedures.print_leftovers();

    if (_show_progress && ! done)
      cout << "  ---- Reanalyze ----" << endl;

  } while ( ! done );

  // -- Set up the def-use chains

  Memory.update_def_use_chains();

  computePointers = false;

  // -- Show unkown procedures

  if (pointerOptions::Show_unknown_procedures) {
    cout << "-----------------------------------------------------------" << endl;
    cout << "Unknown procedures:" << endl;
    for (string_set_p p = _unknown_procedures.begin();
	 p != _unknown_procedures.end();
	 ++p)
      cout << "  " << (*p) << endl;
    pointerOptions::Show_unknown_procedures = false;
  }
}

void Pointers::analyze(analysisProblem * problem)
{
  // -- See if pointers have been computed at all..

  if (computePointers == true) {

    // -- Run the linker to build a global symbol table

    linker.link();

    // -- Build the procedure database

    Procedures.build(Root, linker);

    // -- Set up the main() location (must be done *after* build above).

    root_location = new procLocation(0, Root);
  }

  Problem = problem;

  _direction = problem->direction();

  memoryblock_set external_defs;
  memoryuse_set external_uses;
  memoryblock_set external_changes;
  memoryblock_set nonlocal_changes;
  _procedureCount = 0;
  _skipCount = 0;
  int iterations = 0;
  call_counter = 0;

  if (_show_progress)
    cout << "  ==== Analyze " << Problem->name() << " ====" << endl;

  procedureInfo * root_info = Procedures.lookup(root_location->proc());

  Procedures.setup_analysis();

  bool done = false;

  pointerValue main_return;

  // -- This is a chance for a subclass to run some initialization

  initialize();

  // -- Run the analysis

  do {

    root_info->start_self_timer();
    root_info->start_total_timer();

    Procedures.call_to((stmtLocation *)0, root_info);

    if (Problem) {
      if (_direction == Forward)
	Problem->at_procedure_entry(root_location, root_info, main_return);
      else
	Problem->at_procedure_exit(root_location, root_info, main_return);
    }
      
    analyze_procedure(root_location, 
		      external_defs,
		      external_uses,
		      external_changes,
		      nonlocal_changes);

    if (Problem) {
      if (_direction == Forward)
	Problem->at_procedure_exit(root_location, root_info, main_return);
      else
	Problem->at_procedure_entry(root_location, root_info, main_return);
    }
      
    Procedures.return_from();

    root_info->stop_self_timer();
    root_info->stop_total_timer();

    iterations++;

    bool needs_reanalysis = Procedures.is_reanalysis_required(root_info);

    done = ( ! needs_reanalysis) || (iterations > 10);

    if (_show_progress)
      Procedures.print_leftovers();

    if (_show_progress && ! done)
      cout << "    ---- Reanalyze ----" << endl;

  } while ( ! done );

  Problem = 0;

  // -- Record the fact that we've computed the pointers

  computePointers = false;

  // -- Show unkown procedures

  if (pointerOptions::Show_unknown_procedures) {
    cout << "-----------------------------------------------------------" << endl;
    cout << "Unknown procedures:" << endl;
    for (string_set_p p = _unknown_procedures.begin();
	 p != _unknown_procedures.end();
	 ++p)
      cout << "  " << (*p) << endl;
    pointerOptions::Show_unknown_procedures = false;
  }

  if (pointerOptions::Show_procedures)
    Procedures.stats(cout);
}

/** @brief Reanalyze a callsite */
/*
void Pointers::reanalyze_at(procedureInfo * info, stmtLocation * callsite)
{
  // -- Get the caller at this callsite

  procedureInfo * caller = info->caller_at(callsite);

  // -- Get the actual expression for the callsite

  exprstmtNode * stmt = (exprstmtNode *) callsite->stmt();
  exprNode * E = stmt->expr();

  // -- Store the defs, uses and changes, and the result

  memoryblock_set defs;
  memoryuse_set uses;
  memoryblock_set changes;
  pointerValue result;

  // -- Run the analyzer

  eval(caller, callsite, E,
       defs, uses, changes,
       result, true);

  // -- If anything changed here, put the caller back on the worklist

  if ( ! changes.empty())
    Procedures.mark_for_reanalysis(caller, callsite);
}
*/
// ------------------------------------------------------------
// Analyze a single procedure
// ------------------------------------------------------------

void Pointers::analyze_procedure(procLocation * current,
				 memoryblock_set & external_defs,
				 memoryuse_set & external_uses,
				 memoryblock_set & external_changes,
				 memoryblock_set & all_nonlocal_changes)
{
  procNode * procedure = current->proc();

  // -- Look up the current procedure information

  procedureInfo * info = Procedures.lookup(procedure);

  info->add_all_blocks();
  info->incr_analysis_count();

#ifdef DOMINATOR_TREE

  // -- Set up the tree numbering

  // Moved to stmtLocation::setup_call(): current->visit();

#endif

  // -- Collect all the defs (whether or not they change) and uses

  memoryblock_set defs;
  memoryuse_set uses;
  memoryblock_set changes;

  // === MAIN LOOP: until the worklist is empty...

  while ( ! info->is_empty() ) {

    // -- Get the next basic block off the list

    basicblockNode * bb = info->get_next_block(_direction);
    basicblockLocation * current_block = current->lookup_block(bb);

#ifdef DOMINATOR_TREE

    // -- Set tree numbering minimum

    current_block->visit();

#endif

    // -- Print some information about the current block

    if (pointerOptions::Verbose)
      cout << "==== " << *current_block << " ====" << endl << endl;

    // -- Use this flag to record if this basic block calls a procedure
    // that never returns.

    bool never_returns = false;

    // -- IP Analyzer: Initialize the flow value for this block

    pointerValue result;

    if (Problem) {
      if (_direction == Forward)
	Problem->at_basicblock_entry(current_block, info, result);
      else
	Problem->at_basicblock_exit(current_block, info, result);
    }

    // -- Keep a list of the changes that occur in this block. Distinguish
    // between those caused by processing any phi functions, and those
    // cause by regular statements.

    memoryblock_set merge_changes;
    memoryblock_set stmt_changes;

    // -- Record the value of the branch

    const constant * branch_value = _constants.bottom();

    // -- Process the merge point now, if the direction is Forward
    // (otherwise, we'll process it at the end of the basic block).

    if (_direction == Forward)
      process_merge_point(info, current_block, result, defs, uses, merge_changes);

    // -- Visit each statement and corresponding statement location

    stmt_list & stmts = bb->stmts();
    stmt_location_vec & stmt_locs = current_block->statements();

    // Current statement location (we'll also save this value to hold the
    // last statement for use during the insertion of merge points).

    stmtLocation * current_stmt = 0;
    stmtLocation * previous_stmt = 0;

    stmt_list_p stmt_p;
    int stmt_num;
    int number_of_statements = stmts.size();

    // -- Start at the beginning or end, depending on the direction

    if (_direction == Forward) {
      stmt_p = stmts.begin();
      stmt_num = 0;
    }
    else {
      stmt_p = stmts.end();
      stmt_p--;
      stmt_num = number_of_statements - 1;
    }

    for (int counter = 0; counter < number_of_statements; counter++)
      {
	// -- Get the current statement and statement location

	stmtNode * statement = *stmt_p;
	current_stmt = & (stmt_locs[stmt_num]);

	// -- Print out the current statement

	if (pointerOptions::Verbose) {
	  cout << "---- " << *current_stmt << " -- (" << statement->coord() << ") ----" << endl;

	  output_context oc(cout);
	  statement->output(oc, 0);
	  cout << endl << endl;
	}

#ifdef DOMINATOR_TREE

	// -- Set tree numbering minimum (do this *after* processing the
	// statement -- see special call after finishing a procedure).

	current_stmt->visit();

#endif

	// -- Check the never_returns flag: if it's true, then break out
	// immediately.

	if ( ! never_returns) {

	  // -- IP Analyzer: Initialize the flow value for this statement

	  result.initialize_pointers();
	  _constants.at_stmt_entry(current_stmt, result);

	  if (Problem) {
	    if (_direction == Forward)
	      Problem->at_stmt_entry(current_stmt, result);
	    else
	      Problem->at_stmt_exit(current_stmt, result);
	  }

	  // -- Handle the different statement types

	  // == CASE 1: (Used to be return statement: now it's handled more
	  // directly by pass_return_value().

	  // == CASE 2: Plain expression statement

	  if (statement->typ() == Expr) {

	    // -- Special case: If we're about to make a procedure call, then
	    // make sure process all the changes that have happened so far
	    // right now.
	  /*
	  callNode * call = current_stmt->callnode();
	  if (call) {
	    if ( ! merge_changes.empty() ||
		 ! stmt_changes.empty()) {

	      if (pointerOptions::Verbose) {
		print_memoryblock_def_set("  Phi  Changes   : ", merge_changes, cout);
		print_memoryblock_def_set("  Stmt Changes   : ", stmt_changes, cout);
	      }

	      // -- Post-process the changes from this basic block. We set up merge
	      // points for each block, and transfer the local changes to the
	      // global change set.

	      process_local_changes(info, current_block, merge_changes, changes);
	      process_local_changes(info, current_block, stmt_changes, changes);

	      // -- Record the external inputs and outputs

	      record_external_inputs_and_outputs(info, current,
						 changes, external_changes,
						 all_nonlocal_changes,
						 uses, external_uses);
	    }
	  }
	  */
	    // -- Don't generate a use of the result

	    result.is_a_use = false;

	    exprstmtNode * es = (exprstmtNode *) statement;
	    if (es->expr())
	      eval(info, current_stmt, es->expr(),
		   defs, uses, stmt_changes, result, never_returns);
	  
	    // -- IP Analyzer: Expression statement transfer function

	    if (Problem)
	      Problem->at_exprstmt(current_stmt, es, result);
	  }

	  // == CASE 3: If statement

	  if (statement->typ() == If) {

	    // -- Here the result is used

	    result.is_a_use = true;

	    ifNode * ifnode = (ifNode *) statement;
	    if (ifnode->expr())
	      eval(info, current_stmt, ifnode->expr(),
		   defs, uses, stmt_changes, result, never_returns);

	    // -- IP Analyzer: If statement transfer function
	    
	    if (Problem)
	      Problem->at_if(current_stmt, ifnode, result);

	    // -- Record the value of the branch

	    if (result.constant_value)
	      branch_value = result.constant_value;
	  }

	  // -- End of the statement
	  
	  if (Problem) {
	    if (_direction == Forward)
	      Problem->at_stmt_exit(current_stmt, result);
	    else
	      Problem->at_stmt_entry(current_stmt, result);
	  }
	}

	// -- Go to the next statement, depending on direction

	if (_direction == Forward) {
	  stmt_p++;
	  stmt_num++;
	}
	else {
	  stmt_p--;
	  stmt_num--;
	}
      } // END For all statements

    // When done with the basic block, take all the changed memoryBlocks
    // and set up merge points for them.

    if (pointerOptions::Verbose)
      cout << "          --- (end basic block) ---" << endl << endl;

#ifdef DOMINATOR_TREE

    // -- Set tree numbering maximum: if the basic block doesn't dominate
    // anything, then zip up the dominator tree.

    if (bb->children().empty() &&
	(bb != procedure->exit()))
      {
	stmtLocation * last = current_block->last();
	last->finish();
      }

#endif

    // -- Handle changes: add merge-points in the appropriate places, and
    // update the worklists

    if ( ! merge_changes.empty() ||
	 ! stmt_changes.empty())
      {

	if (pointerOptions::Verbose) {
	  print_memoryblock_def_set("  Phi  Changes   : ", merge_changes, cout);
	  print_memoryblock_def_set("  Stmt Changes   : ", stmt_changes, cout);
	}

	if ( ! never_returns ) {

	  // -- Post-process the changes from this basic block. We set up
	  // merge points for each block, and transfer the local changes to
	  // the global change set.
	
	  process_local_changes(info, current_block, merge_changes, changes);
	  process_local_changes(info, current_block, stmt_changes, changes);

	  // -- Add all reachable basic blocks to the worklist

	  //if ( ! stmt_changes.empty())

	  info->add_reachable_blocks(bb, Forward);
	}
      }

    // -- This is not a real merge-point; we just need this here for
    // backward analyses, which need to see the merge points *after* the
    // statements in the block.

    if (_direction == Backward)
      process_merge_point(info, current_block, result, defs, uses, merge_changes);

    // -- IP Analyzer: End of the basic block

    if (Problem) {
      if (_direction == Forward)
	Problem->at_basicblock_exit(current_block, info, result);
      else
	Problem->at_basicblock_entry(current_block, info, result);
    }

    // -- Conditional analysis

    if (pointerOptions::Conditional_analysis) {

      bool has_truth_value = false;
      bool which_branch = false;
      
      has_truth_value = _constants.has_truth_value(branch_value, which_branch);
      if (has_truth_value) {

	// -- The branch has a definite value

	if (pointerOptions::Verbose)
	  cout << "Found constant branch at " << * current_block
	       << " -- value = " << _constants.to_string(branch_value) << endl;
      }

      // -- Run the special worklist algorithm, with the following
      // exception: if the basic block we just processed calls exit() or
      // abort() (or another procedure that never returns), then don't
      // activate any of the outgoing edges.

      if ( ! never_returns)
	info->update_conditional_worklist(bb, has_truth_value, which_branch);
    }

    // -- Check the never_returns flag: if this basic block calls exit() or
    // abort(), and it dominates the exit of the procedure, then the
    // procedure itself never returns.

    if (never_returns &&
	current_block->dominates_exit()) {

      info->set_never_returns();

      if (pointerOptions::Verbose)
	cout << info->qualified_name() << " never returns." << endl;
    }

  } // END until worklist empty

  // -- For context insensitivity, set up the external inputs and external
  // outputs. EXCEPT: if the procedure never returns, then ignore all
  // changes that occured.

  if ( ! info->never_returns()) {

    record_external_inputs_and_outputs(info, current,
				       changes, external_changes,
				       all_nonlocal_changes,
				       uses, external_uses);
  }
  else {

    // -- Don't let any changes propagate back

    external_changes.clear();
    all_nonlocal_changes.clear();
  }
}

// ------------------------------------------------------------
// Procedure call 
// ------------------------------------------------------------

void Pointers::procedure_call(procedureInfo * caller,
			      stmtLocation * callsite,
			      callNode * call,
			      pointerValue & call_target,
			      pointervalue_list & arguments,
			      memoryblock_set & external_defs,
			      memoryuse_set & external_uses,
			      memoryblock_set & external_changes,
			      pointerValue & return_val,
			      bool & never_returns)
{
  procedureInfo * info = 0;
  bool resolved = false;
  bool recursive_call = false;
  procNode * callee = 0;

  if (call_target.blocks.size() > 1) {
    if (pointerOptions::Verbose) {
      cout << "WARNING: Cannot handle indirect calls with multiple targets." << endl;

      for (memoryblock_set_p p = call_target.blocks.begin();
	   p != call_target.blocks.end();
	   ++p)
	cout << "  CALLS: " << (*p)->name() << endl;
    }
  }
  else {

    // -- Find the target procedure

    memoryBlock * target_mb;

    if (call_target.blocks.size() == 0) {
      target_mb = 0;
      if (pointerOptions::Verbose)
	cout << "ERROR: Call target corrupted: " << *callsite << endl;
    }
    else {
      
      // ------- Special-case procedure calls ---------------------------
    
      // -- Is this a call to *alloc or *free?

      if (is_allocation(call_target)) {

	// -- Call the allocation method

	memoryBlock * alloc_block;

	string name = caller->proc()->decl()->name() + "/malloc";

	alloc_block = at_allocation(name, caller,
				    callsite, callsite->stmt(), (declNode *)0,
				    external_defs, external_uses, external_changes,
				    return_val);

	if (pointerOptions::Verbose)
	  cout << "Allocate heap object: " << alloc_block->name() << endl;

	// -- IP Analyzer: Memory allocation transfer function  

	if (Problem)
	  Problem->at_allocation(callsite, arguments, alloc_block, external_changes);

	return;
      }

      if (is_deallocation(call_target)) {

	// -- First, dereference the pointer argument
	
	pointerValue & dealloc_ptrs = arguments.front();
	pointerValue to_deallocate;
	
	star_operator(caller, callsite, dealloc_ptrs,
		      external_defs, external_uses, external_changes, to_deallocate);
	
	if (pointerOptions::Verbose)
	  print_memoryblock_set("Deallocate heap objects: ", to_deallocate.blocks, cout);
	
	// -- Call the deallocation routine for each target
	
	at_deallocation(caller, callsite, to_deallocate,
			external_defs, external_uses, external_changes);
	
	// -- IP Analyzer: Memory deallocation transfer function  
	
	if (Problem)
	  Problem->at_deallocation(callsite, to_deallocate, external_changes);
	
	return;
      }
      
      if (is_reallocation(call_target)) {
	
	// -- Realloc is basically a no-op
	
	pointerValue & realloc_ptrs = arguments.front();
	
	// -- Just dump the input pointers into the return value
	
	star_operator(caller, callsite, realloc_ptrs,
		      external_defs, external_uses, external_changes, return_val);
	return_val.is_address = true;
	
	if (pointerOptions::Verbose)
	  print_memoryblock_set("Reallocate heap objects: ", return_val.blocks, cout);
	
	return;
      }
      
      if (is_va_arg(call_target)) {
	
	// -- va_arg works by just dereferencing the va_list object, which
	// should by the first argument.
	
	pointerValue & varargs_list = arguments.front();
	
	// -- Just dump the resulting objects into the return value
	
	star_operator(caller, callsite, varargs_list,
		      external_defs, external_uses, external_changes, return_val);
	return_val.is_address = true;
	
	if (pointerOptions::Verbose)
	  print_memoryblock_set("Get varargs objects: ", return_val.blocks, cout);
	
	return;
      }
      
      if (is_va_start(call_target) ||
	  is_va_end(call_target))
	return;
      
      if (is_exit(call_target)) {
	
	// -- This is a call to exit() or abort() or some other procedure that
	// never returns. Set the never_returns flag.
	
	never_returns = true;
	return;
      }

      // -- Look up the target procedure

      target_mb = * (call_target.blocks.begin());

      callee = linker.lookup_procedure(target_mb->decl());
      if (callee == 0) {
	
	if (pointerOptions::Verbose)
	  cout << "WARNING: No source for " << target_mb->name() << 
	    " at " << * callsite << endl;
	
	_unknown_procedures.insert(target_mb->name());
      }
      else {
	
	// -- Make callNode point to the called procedure
	
	call->proc(callee);
	
	resolved = true;
      }
    }
  }

  // ------ Can we procede? -----------------------

  if ( ! resolved ) {

    // -- Conservative: assume all reachable memory blocks could be read or
    // modified.

    memoryblock_set reachable_blocks;
    conservative_procedure_call(caller, callsite, arguments, reachable_blocks,
				external_defs, external_uses, external_changes,
				return_val);

    // -- Constant propagation

    _constants.at_conservative_procedure_call(callsite, call, call_target,
					      arguments, reachable_blocks,
					      return_val, external_changes);

    // -- User-defined analysis

    if (Problem)
      Problem->at_conservative_procedure_call(callsite, call, call_target,
					      arguments, reachable_blocks,
					      return_val, external_changes);

    return;
  }

  // ------- All other procedures -----------------------------------

  // -- User-defined analysis

  if (Problem)
    Problem->at_call(callsite, call, call_target, callee, arguments, return_val);

  // -- Look up the procedure information

  info = Procedures.lookup(callee);

  // -- Check for recursion.

  int num_instances = 0;

  recursive_call = Procedures.is_recursive_call(info, num_instances);

  if (recursive_call) {

    // -- All recursive procedures are context-insensitive

    Procedures.make_cycle_context_insensitive(info);

    // -- Set up the call site. We need to do this *after* setting the
    // whole cycle context-insensitive.
    
    info->setup_call_at(caller, callsite);

    // -- Set up the extern inputs
    /* WHY?
    memoryblock_set rec_changes;
    pass_external_inputs(info, callsite, external_uses, rec_changes);
    */
  }
  else {

    // -- Not recursive: just set up the call site normally

    info->setup_call_at(caller, callsite);
  }

  // -- Timers

  caller->stop_self_timer();

  if (! recursive_call) {
    info->start_self_timer();
    info->start_total_timer();
  }

  // -- Run the analyzer at this particular call site

  analyze_procedure_at(info, callsite, arguments,
		       external_defs,
		       external_uses,
		       external_changes,
		       return_val);

  // -- Timers

  caller->start_self_timer();

  if (! recursive_call) {
    info->stop_self_timer();
    info->stop_total_timer();
  }

  // -- Pick up any pending external changes for this callsite. We need
  // to do this *after* calling pass_external_outputs.

  memoryblock_set pending_changes;
  caller->get_pending_changes(callsite, pending_changes);

  if ( ! pending_changes.empty()) {

    if (pointerOptions::Verbose)
      print_memoryblock_set("  => Pickup pending changes: ", pending_changes, cout);
    
    external_changes.insert(pending_changes.begin(), pending_changes.end());
  }

  // -- Check to see if the procedure never returns, and pass that value
  // up.

  never_returns = info->never_returns();
}

void Pointers::analyze_procedure_at(procedureInfo * info,
				    stmtLocation * callsite,
				    pointervalue_list & arguments,
				    memoryblock_set & external_defs,
				    memoryuse_set & external_uses,
				    memoryblock_set & external_changes,
				    pointerValue & return_val)
{
  long int call_id;

  // -- Check for recursion (again). Note, we must do this before we call
  // procedureDB::call_to, otherwise everything will seem recursive!

  int num_instances = 0;
  bool recursive_call = Procedures.is_recursive_call(info, num_instances);

  // -- Decide whether to make it context insensitive

  if (pointerOptions::Context_insensitive &&
      ! info->prefer_context_sensitive()) {
 
    // -- Set the flag
 
    info->make_context_insensitive();
 
    // -- Set up the extern inputs, but only for recursive calls.
    /*
    if (recursive_call)
      pass_external_inputs(info, callsite, external_uses, changed_inputs);
    */
  }

  // -- Push this call on the call stack
      
  Procedures.call_to(callsite, info);

  // -- Get the callee path

  procLocation * callee_path = info->procedure_location(callsite);

  // -- Print some context info

  if (pointerOptions::Verbose || pointerOptions::Show_stack) {

    call_id = call_counter;
    call_counter++;

    cout << "------------------------------------------------------------" << endl;

    if (info->is_context_insensitive())
      cout << "    CI Call ";
    else
      cout << "    CS Call ";

    cout << info->name() << " (id=" << call_id << ")";

    if (callsite) {
      callNode * call = callsite->callnode();
      if (call)
	cout << " (" << call->coord() << ")";
      else
	cout << " (unknown source)";
    }

    cout << endl;
    
    cout << "    Call-Path: ";
    Procedures.print_call_stack(cout);
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
  }

  // -- Find out if reanalysis of this procedure is mandated

  bool reanalysis_required = Procedures.is_reanalysis_required(info);

  // -- Forward or backward....

  if (_direction == Forward) {

    // -- Forward analysis ------------------------------

    // -- Collect changes to the inputs

    memoryblock_set changed_inputs;

    // -- Pass in the parameters
    
    pass_parameters(info, callsite, arguments, changed_inputs);

    // -- Pass in any external, implied parameters (for context insensitivity)

    pass_external_inputs(info, callsite, external_uses, changed_inputs);

    // -- Short circuit: if the procedure is context insensitive, and both
    // the arguments and the external inputs are the same, then there is no
    // need to reanalyze it.

    bool skip_analysis = false;

    if ((num_instances >= 1) || (info->is_context_insensitive() &&
				 (! reanalysis_required) &&
				 (changed_inputs.empty())))
      {
	skip_analysis = true;

	if (pointerOptions::Verbose) {
	  cout << "SKIP " << info->name() << " at ";
	  Procedures.print_call_stack(cout);
	  cout << endl;
	}
      }

    // -- Analyze the body of the procedure

    int start_progress = _procedureCount;
    int start_def_count = memoryAccess::def_count;

    memoryblock_set nonlocal_changes;

    progress_meter(info, skip_analysis);

    if ( ! skip_analysis) {

      if (Problem)
	Problem->at_procedure_entry(callee_path, info, return_val);
      
      analyze_procedure(callee_path,
			external_defs,
			external_uses,
			external_changes,
			nonlocal_changes);

      if (Problem)
	Problem->at_procedure_exit(callee_path, info, return_val);
    }

#ifdef DOMINATOR_TREE

    // -- Set tree numbering minimum (do this *after* processing the
    // procedure).
    
    callsite->visit();

#endif

    int end_progress = _procedureCount;
    int end_def_count = memoryAccess::def_count;

    /*
    if (computePointers &&
 	( ! info->is_context_insensitive()))
      {
 	int progress_diff = (end_progress - start_progress);
 	int times_called = Procedures.times_called(info);
 	int def_diff = (end_def_count - start_def_count);
 	int num_ancestors = info->ancestors().size();
 
 	cout << "STATS: " << info->name() <<
 	  " -- progress = " << progress_diff <<
 	  " -- called = " << times_called <<
 	  " -- defs = " << def_diff << endl;
 
 	if (pointerOptions::Context_insensitive &&
	    ! info->prefer_context_sensitive()) {
 
 	  // -- Set the flag
 
 	  info->make_context_insensitive();
 
 	  // -- Set up the extern inputs, but only for recursive calls.

 	  if (recursive_call)
	    pass_external_inputs(info, callsite, external_uses, changed_inputs);
 	}
      }
    */

    // -- Pass the return value

    pass_return_value(info, callsite, return_val, external_changes);

    // -- Pass out the external, implied outputs. We should only need to do
    // this if the procedure was actually analyzed, which might cause the
    // external outputs to change.

    if ( ! skip_analysis)
      pass_external_outputs(info, nonlocal_changes, callsite);
  }
  else {

    // -- Backward analysis ------------------------------
    // (Do the same thing in reverse)

    // -- External changes and the return value are now the changed inputs

    memoryblock_set changed_inputs;
    memoryblock_set nonlocal_changes;

    pass_external_outputs(info, nonlocal_changes, callsite);

    pass_return_value(info, callsite, return_val, changed_inputs);

    // TBD: We need to pass in the return value here and check if it changed

    // -- Short circuit: if the procedure is context insensitive, and both
    // the arguments and the external inputs are the same, then there is no
    // need to reanalyze it.

    bool skip_analysis = false;

    if (info->is_context_insensitive() &&
	( ! reanalysis_required) &&
	(changed_inputs.empty()))
      {
	skip_analysis = true;

	if (pointerOptions::Verbose) {
	  cout << "SKIP " << info->name() << " at ";
	  Procedures.print_call_stack(cout);
	  cout << endl;
	}
      }

    if ( ! skip_analysis) {

      if (Problem)
	Problem->at_procedure_exit(callee_path, info, return_val);
      
      analyze_procedure(callee_path,
			external_defs,
			external_uses,
			external_changes,
			nonlocal_changes);

      if (Problem)
	Problem->at_procedure_entry(callee_path, info, return_val);
    }

    progress_meter(info, skip_analysis);

    // -- Pass back the inputs.

    pass_external_inputs(info, callsite, external_uses, external_changes);
    
    pass_parameters(info, callsite, arguments, external_changes);
  }

  // -- Indicate that we're returning.

  Procedures.return_from();

  // -- Debug info

  if (pointerOptions::Verbose || pointerOptions::Show_stack) {
    cout << "------------------------------------------------------------" << endl;
    cout << "    Return from " << info->name() << " (id=" << call_id << ")" << endl;
    cout << "    Return-Path: ";
    Procedures.print_call_stack(cout);
    cout << endl;

    print_memoryblock_set("    Returning: ", return_val.blocks, cout);

    cout << "------------------------------------------------------------" << endl;
    cout << endl;
  }
}

void Pointers::pass_parameters(procedureInfo * info,
			       stmtLocation * callsite,
			       pointervalue_list & arguments,
			       memoryblock_set & changed_inputs)
{
  procNode * callee = info->proc();
  procLocation * callee_path = info->procedure_location(callsite);
  procedureInfo * caller = Procedures.current_caller();

  // -- Perform the parameter passing

  funcNode * f = (funcNode *) callee->decl()->no_tdef_type();
  if (f->typ() != Func) {
    if (pointerOptions::Verbose)
      cout << "INTERNAL ERROR: Attempting to call a non-procedure." << endl;
    return;
  }

  if ( ! f->is_void_args()) {

    decl_list & params = f->args();

    // -- Treat each passed parameter as an assignment from the actual
    // parameter to the formal parameter. We don't care about defs or uses,
    // but we will record if any of the inputs are different.

    memoryblock_set defs;
    memoryuse_set uses;
    memoryblock_set changes;

    pointervalue_list_p args_p = arguments.begin();

    for (decl_list_p params_p = params.begin();
	 params_p != params.end();
	 ++params_p)
      {
	declNode * formal = * params_p;
	typeNode * formal_type = formal->no_tdef_type();

	// -- Test for varargs...

	if (formal_type->is_ellipsis() ||
	    is_va_list(formal)) {

	  // -- Here's how we'll handle varargs: we will use the "..."
	  // declaration as a special object to collect the rest of the
	  // arguments. We'll use the additive assignment so that the
	  // values are accumulated. Then, any variables declared with type
	  // "va_list" are pointers to that thing. Then let the rest of the
	  // system handle itself.

	  // -- Create the ellipsis object

	  memoryBlock * ellipsis = Memory.lookup_variable(callee_path, formal, callee);

	  pointerValue lhs(ellipsis);

	  if (pointerOptions::Verbose)
	    cout << "Pass varargs object: " << ellipsis->name() << endl;

	  // -- Assign each remaining argument to it
	  
	  while (args_p != arguments.end()) {
	    pointerValue & actual = * args_p;

	    // -- Additive assignment...

	    assignment_operator(info, callee_path, callsite, lhs, actual,
				defs, uses, changed_inputs);

	    // -- Constant propagation

	    _constants.at_parameter_pass(callee_path, lhs, actual, changed_inputs);

	    // -- User-defined analysis
	    
	    if (Problem)
	      Problem->at_parameter_pass(callee_path, callsite, lhs, actual, changed_inputs);

	    args_p++;
	  }

	  // -- Make the va_list variables point to the ellipsis object

	  setup_va_list_variables(info, callee_path, callsite, callee, ellipsis);

	  // -- Get out of the loop...

	  break;
	}
	else {

	  // -- Pass a regular parameter

	  // -- Get the actual parameter (and value)

	  if (args_p == arguments.end()) {
	    if (pointerOptions::Verbose)
	      cout << "ERROR: Too few arguments to " << callee->decl()->name() << 
		" at " << callee->coord() << endl;
	    break;
	  }

	  pointerValue & actual = * args_p;

	  // -- Look up the formal parameter.

	  memoryBlock * left = Memory.lookup_variable(callee_path, formal, callee);
	  pointerValue lhs(left);

	  if (pointerOptions::Verbose)
	    cout << endl << "Pass parameter: " << left->name() << endl;

	  // -- Perform the assignment. THIS IS CRITICAL: the current path
	  // must be in the context of the procedure being called (the
	  // callee path) ALSO NOTE: We assume that no argument to
	  // function is an address-of operator.

	  assignment_operator(info, callee_path, callsite, lhs, actual,
			      defs, uses, changed_inputs);

	  // -- If the direction is backward, make sure we have the uses

	  if (_direction == Backward)
	    generate_uses(info, callsite, uses, actual);

	  // -- Constant propagation

	  _constants.at_parameter_pass(callee_path, lhs, actual, changed_inputs);

	  // -- User-defined analysis
	  
	  if (Problem)
	    Problem->at_parameter_pass(callee_path, callsite, lhs, actual, changed_inputs);

	  // -- Check

	  if ((formal_type->typ() == Struct) ||
	      (formal_type->typ() == Union))
	    {
	      /* This happens a lot in bind
	      cout << "WARNING at " << * callee_path << "struct/union parameter:" << endl;
	      output_context oc(cout);
	      formal->output(oc,0);
	      cout << endl;
	      */
	    }
	}

	++args_p;
      }
  }
}

void Pointers::pass_return_value(procedureInfo * info,
				 stmtLocation * current_callsite,
				 pointerValue & return_val,
				 memoryblock_set & changes)
{
  procLocation * callee_path = info->procedure_location(current_callsite);

  // -- Get the return value variable, if there is one.

  declNode * return_var = info->return_variable();

  if (return_var) {

    // -- Look up the return variable.

    memoryBlock * return_block = Memory.lookup_variable(callee_path, return_var, info->proc());

    if (pointerOptions::Verbose)
      cout << endl << "Pass return value " << return_block->name() << endl;

    // -- Pass this variable back as the return value

    return_val.blocks.insert(return_block);

    // -- Is this procedure context sensitive?

    if (info->is_context_insensitive()) {

      // -- Context insensitive: pass the return value back to all the call
      // sites.

      bool is_return_value = true;

      pass_one_external_output(info, current_callsite,
			       return_block, is_return_value);
    }
    else {

      // -- Context sensitive: there is nothing to do here because the
      // assignment to the return variable at the end of a
      // context-sensitive call will dominate the call site.

      memoryuse_set ignore_uses;
      generate_uses(info, current_callsite, ignore_uses, return_val);

      /*
      pass_one_external_output(info, current_callsite,
			       return_block, def,
			       changes);
      */
    }
  } 
}


void Pointers::process_merge_point(procedureInfo * info,
				   basicblockLocation * current_block,
				   pointerValue & result,
				   memoryblock_set & defs,
				   memoryuse_set & uses,
				   memoryblock_set & changes)
{
  // -- Find all the objects to be merge at this point, if any

  const memoryblock_set * to_be_merged = info->lookup_merge_point(current_block);
  if (to_be_merged) {

    // -- For each one, merge the incoming uses and create the
    // corresponding def.

    for (memoryblock_set_cp p = to_be_merged->begin();
	 p != to_be_merged->end();
	 ++p)
      {
	memoryuse_list phi_uses;
	memoryBlock * block_to_merge = *p;

	// -- Only merge flow-sensitive blocks

	if (block_to_merge->is_flow_sensitive()) {

	  // -- When computing pointers, find the def that dominates the
	  // merge point

	  memoryDef * dominating_def = 0;
	  if (computePointers)
	    dominating_def = nearest_def_at(info, block_to_merge, current_block);

	  // -- First find the corresponding uses, one for each control-flow
	  // predecessor.

	  block_to_merge->merge_uses_at(current_block, phi_uses, info->active_edges(),
					dominating_def, computePointers);

	  // -- Merge the values together

	  merge_operator(info, current_block, block_to_merge, phi_uses, defs, uses, changes);

	  // -- Constant propagation

	  _constants.at_merge(current_block, block_to_merge, phi_uses, result, changes);

	  // -- IP Analyzer: merge the incoming flow values

	  if (Problem)
	    Problem->at_merge(current_block, block_to_merge, phi_uses, result, changes);
	}
      }
  }
}

/** @brief Process local changes
 *
 * After analyzing a particular basic block, we process the blocks that
 * have changed by inserting merge-points for those blocks. */

void Pointers::process_local_changes(procedureInfo * info,
				     basicblockLocation * current_block,
				     memoryblock_set & local_changes,
				     memoryblock_set & changes)
{
  if (pointerOptions::Verbose)
    cout << " Process local changes at " << * current_block << endl;


  // -- For each changed block...

  for (memoryblock_set_p q = local_changes.begin();
       q != local_changes.end();
       ++q)
    {
      memoryBlock * block = (*q);

      if (pointerOptions::Verbose)
	cout << "    Change: " << block->name();

      /* NOTE: We used to test to see if the block is visible, but that is
	 no longer appropriate. That functionality is handled by
	 record_external_inputs_and_outputs.

      // -- If the block is visible to the caller, pass this change up

      if (Procedures.is_visible_to_caller(info, block)) {
	changes.insert(block);
      
	if (pointerOptions::Verbose)
	  cout << " -- visible to caller";
      }
      else
	if (pointerOptions::Verbose)
	  cout << " -- NOT visible to caller";
      */

      // -- Add the changed block to the global changes

      changes.insert(block);

      if (pointerOptions::Verbose) {
	memoryDef * cur_def = block->current_def();
	memoryDef * def;

	if ( cur_def )
	  def = cur_def;
	else
	  def = block->last_def_at(current_block);

	if (def)
	  cout << " -- def at " << * (def->where()) << endl;
	else
	  cout << " -- (non-local def)" << endl;
      }

      // -- For flow insensitive objects, make sure all the procedures
      // that use this object are reanalyzed.

      if ( ! block->is_flow_sensitive()) {
	procedureinfo_set input_to = block->input_to();

	for (procedureinfo_set_p w = input_to.begin();
	     w != input_to.end();
	     ++w)
	  {
	    procedureInfo * info_needs_input = *w;

	    Procedures.mark_one_for_reanalysis(info_needs_input);
	  }
      }

      // -- If we're computing pointers, then set up a merge point. In
      // flow-insensitive mode, there are no merge points.

      if (computePointers)
	info->setup_merge_point(block, current_block);

      /*** I don't like sanity...

      // -- Sanity check

      if (!computePointers)
	info->check_merge_point(block, current_block);
      */
    }

  if (pointerOptions::Verbose && ( ! local_changes.empty()))
    cout << endl;
}

/** @brief Process external inputs
 *
 * In context insensitive mode, this method visits all the external
 * inputs from the given procedure. For each one, it merges together the
 * reaching defs from all the calling contexts, and produces a single def
 * that reaches the uses inside the procedure.
 *
 * Each use is attached to the special procLocation at that calling
 * context. The merged def itself is attach to basic block zero in the
 * actual procedure. */

void Pointers::pass_external_inputs(procedureInfo * info,
				    stmtLocation * current_callsite,
				    memoryuse_set & external_uses,
				    memoryblock_set & changed_inputs)
{
  // -- Just to make sure...

  if ( ! info->is_context_insensitive())
    return;

  pointerValue result;

  // -- Get the external inputs

  const memoryblock_set & external_inputs = info->external_inputs();

  // -- Get the list of callsites

  const procedureInfo::callsite_map & callsites = info->callsites();

  // -- Get the current caller

  procedureInfo * caller = Procedures.current_caller();

  // -- Attach the merge to basic block zero.

  Location * input_location = info->procedure_location(current_callsite);

  // -- For each external input

  for (memoryblock_set_cp p = external_inputs.begin();
       p != external_inputs.end();
       ++p)
    {
      memoryBlock * block_to_pass = *p;

      if (block_to_pass->write_protected()) {

	// -- Write protected inputs: nothing to do because they always
	// return the only definition.

	if (pointerOptions::Verbose)
	  cout << endl << "Pass write-protected input " << block_to_pass->name() << endl;
      }
      else
	if ( ! block_to_pass->is_flow_sensitive()) {

	  // -- No need to pass flow-insensitive objects

	  if (pointerOptions::Verbose)
	    cout << endl << "Skip flow-insensitive input " << block_to_pass->name() << endl;
	}
	else {

	  // -- Only pass inputs that could be visible in the caller

	  if (Procedures.is_visible_to_caller(info, block_to_pass)) {

	    /*
	    caller &&
	      Procedures.is_visible_to(caller, block_to_pass)) {
	    */
	    // Procedures.is_visible_to_caller(info, block_to_pass)) {

	    // -- Regular inputs: use additive assignment to meet together
	    // the reaching defs in the different contexts.

	    if (pointerOptions::Verbose)
	      cout << endl << "Pass external input " << block_to_pass->name() << endl;

	    // -- Set up the use in the calling context, and set it's
	    // reaching def properly. Attach the use to the procLocation
	    // that represents the callsite

	    // procLocation * attach_to = current_callsite->calls();
	    Location * attach_to = current_callsite;

	    // -- Look up the use. By calling use_at() here, we set the
	    // current_use so that the assignment operator will find it.

	    memoryUse * use = block_to_pass->use_at(attach_to);

	    // -- When computing pointers, actually find the reaching def

	    memoryDef * def = 0;

	    if (computePointers) {
	      def = nearest_def_at(info, block_to_pass, current_callsite);
	      use->reaching_def(def);
	    }

	    if (pointerOptions::Verbose) {
	      def = use->reaching_def();
	      cout << "  At callsite " << * current_callsite
		   << " use at " << * attach_to << " reaching def is ";
	      if (def)
		cout << * (def->where()) << endl;
	      else
		cout << "(no reaching def)" << endl;
	    }

	    // if (use->reaching_def()) {

	      // -- Use the assignment operator with the additive flag to merge
	      // this actual into the formal.

	      self_assignment(attach_to, input_location, block_to_pass, changed_inputs);

	      // -- Constant propagation

	      _constants.at_self_assignment(attach_to, input_location, block_to_pass, changed_inputs);

	      // -- User-defined analysis
	      
	      if (Problem)
		Problem->at_self_assignment(attach_to, input_location, block_to_pass, changed_inputs);
	      // }
	  }
	}
    }
}

/** @brief Process external outputs
 *
 * In context insensitive mode, this method visits all the external outputs
 * from the given procedure. For each one, it propagates the last value to
 * all the call sites. It does this by creating a use at the callsite that
 * is reached by the last def inside the procedure. This use is then
 * assigned to a def at that location. That way, the def will contain the
 * right value, and dominate any uses of the object that follow. */

void Pointers::pass_external_outputs(procedureInfo * info,
				     memoryblock_set & all_nonlocal_changes,
				     stmtLocation * current_callsite)
{
  // -- Just to make sure...

  if ( ! info->is_context_insensitive())
    return;

  // -- Don't pass back values when the procedure never returns

  if (info->never_returns())
    return;

  // -- Get the external outputs

  const memoryblock_set & external_outputs = info->external_outputs();

  /*************************************************************
   *
   *  This doesn't work, here's why: when we discover a new callsite, we
   *  need to propagate all of the external outputs to that callsite,
   *  regardless of whether or not they have changed.
   ************************************************************ */
  /*
  // -- Compute the set of external outputs that has actually changed

  memoryblock_list changed_outputs;

  set_intersection(external_outputs.begin(), external_outputs.end(),
		   all_nonlocal_changes.begin(), all_nonlocal_changes.end(),
		   back_inserter(changed_outputs));

  memoryblock_list diffs;

  set_difference(external_outputs.begin(), external_outputs.end(),
		 all_nonlocal_changes.begin(), all_nonlocal_changes.end(),
		 back_inserter(diffs));

  for (memoryblock_list_p p = diffs.begin();
       p != diffs.end();
       ++p)
    {
      memoryBlock * cur = (*p);
      if (cur->is_flow_sensitive()) {
	cout << "At " << info->qualified_name() << " nochange-skip " << cur->name() << endl;
      }
    }

  copy(external_outputs.begin(), external_outputs.end(),
       back_inserter(changed_outputs));
  */

  // -- For each external output

  for (memoryblock_set_cp p = external_outputs.begin();
       p != external_outputs.end();
       ++p)
    {
      memoryBlock * block_to_pass = *p;

      if (pointerOptions::Verbose)
	cout << endl << "Pass external output " << block_to_pass->name() << endl;

      bool is_return_value = false;

      pass_one_external_output(info, current_callsite,
			       block_to_pass, is_return_value);
    }
}

void Pointers::pass_one_external_output(procedureInfo * info,
					stmtLocation * current_callsite,
					memoryBlock * block_to_pass,
					bool is_return_value)
{
  // -- Generate a special use inside the procedure (actually at the last
  // statement of the exit block).

  stmtLocation * last_stmt_of_proc = info->get_context()->last();

  memoryUse * use = block_to_pass->use_at(last_stmt_of_proc);

  // -- When computing pointers, we also search for the def that reaches
  // the exit of the procedure

  memoryDef * internal_reaching_def = 0;

  if (computePointers) {
    internal_reaching_def = block_to_pass->nearest_def_at(last_stmt_of_proc);
    use->reaching_def(internal_reaching_def);
  }

  internal_reaching_def = use->reaching_def();

  if (pointerOptions::Verbose) {

    cout << "  Interal use at " << * (use->where())
	 << " with reaching def at ";

    memoryDef * def = use->reaching_def();
    if (def)
      cout << * (def->where()) << endl;
    else
      cout << "(no reaching def)" << endl;
  }

  // -- Skip flow insensitive objects

  if (! block_to_pass->is_flow_sensitive()) {
    if (pointerOptions::Verbose)
      cout << endl << "Skip flow-insensitive external output " << block_to_pass->name() << endl;
    return;
  }

  // -- Get the list of callsites

  const procedureInfo::callsite_map & callsites = info->callsites();

  // -- For each call site...

  for (procedureInfo::callsite_map_cp csp = callsites.begin();
       csp != callsites.end();
       ++csp)
    {
      // -- At each target call site

      stmtLocation * target_callsite = (*csp).first;
      procedureInfo * caller  = (*csp).second;

      // -- Test to see if the given memoryBlock is actually visible in the
      // caller's scope. The exception is the return value variable, which
      // must be passed back.

      if (is_return_value ||
	  Procedures.is_visible_to(caller, block_to_pass)) {

	if (pointerOptions::Verbose)
	  cout << "+ Back to callsite " << * target_callsite << endl;

	// -- Use the assignment operator to pass the value back.

	memoryblock_set pending_changes;

	self_assignment(last_stmt_of_proc, target_callsite,
			block_to_pass, pending_changes);

	// -- Constant propagation
	
	_constants.at_self_assignment(last_stmt_of_proc, target_callsite,
				      block_to_pass, pending_changes);
	
	// -- User-defined analysis
	
	if (Problem)
	  Problem->at_self_assignment(last_stmt_of_proc, target_callsite,
				      block_to_pass, pending_changes);
	
	// -- If changes occured, force reanalysis of the callsite and
	// store the changes until we get there. Don't record changes to
	// the return value variable.
	
	if ( ! is_return_value && ! pending_changes.empty()) {
	  
	  // -- Save the pending changes for the caller to pick up
	  
	  caller->set_pending_changes(target_callsite, pending_changes);
	  if (pointerOptions::Verbose)
	    print_memoryblock_set("    => Pending changes: ", pending_changes, cout);
	  
	  // -- Force reanalysis of the caller, unless it is the
	  // current caller.
	  
	  if (target_callsite != current_callsite)
	    Procedures.mark_for_reanalysis(caller, target_callsite, true);
	}
      }
      else {
	
	// -- Variable not visible
	
	if (pointerOptions::Verbose)
	  cout << "+ Not visible at callsite " << * target_callsite << endl;
      }
    }

  // -- Reset the current def/use information: THIS IS CRITICAL: don't
  // reset the current use and def for the return value, otherwise the
  // assignment won't be able to find the reaching definition.

  if ( ! is_return_value)
    block_to_pass->reset_current_def_use((Location *)0);
}

/** @brief Post-process uses and defs
 *
 * When we're done analyzing a particular procedure, we go through the uses
 * and defs to determine which ones are external. A use is external if it's
 * reaching definition is outside the procedure. A def is external if it's
 * value could be read outside the procedure. */

void Pointers::record_external_inputs_and_outputs(procedureInfo * info,
						  procLocation * current,
						  memoryblock_set & changes,
						  memoryblock_set & external_changes,
						  memoryblock_set & all_nonlocal_changes,
						  memoryuse_set & uses,
						  memoryuse_set & external_uses)
{
  bool found_new_inputs = false;
  procNode * procedure = info->proc();

  procedureInfo * caller = Procedures.current_caller();
  stmtLocation * current_callsite = Procedures.current_callsite();

  stmtLocation * last_stmt_of_proc = current->last();

  // -- Visit all the uses and identify those whose reaching definitions
  // are outside the procedure.

  if (pointerOptions::Verbose) {
    cout << "Record external inputs to " << procedure->decl()->name() << " at ";
    Procedures.print_call_stack(cout);
    cout << endl;
  }

  if ( ! uses.empty() )
    for (memoryuse_set_p p = uses.begin();
	 p != uses.end();
	 ++p)
      {
	memoryUse * use = *p;
	memoryBlock * block = use->owner();
	memoryDef * def = use->reaching_def();
	
	// -- The only variables we care about are those that are not local
	// to this procedure.

	if (block->local_to() != procedure) {

	  // -- For flow insensitive objects, just record that the object
	  // is an input to this procedure

	  if ( ! block->is_flow_sensitive())
	    block->input_to().insert(info);

	  // -- Check for external input. There are two cases: (1) the
	  // input has a reaching def outside the current procedure, (2)
	  // the input has no reaching def, but it might later in the
	  // analysis.

	  bool is_external_input = false;

	  if (def) {

	    memoryBlock * block = def->owner();

	    // -- We're looking for reaching defs that are not inside this
	    // procedure. If the current location is NOT a prefix location
	    // of the reaching def, then this is a non-local use.

	    if ( ! Location::is_prefix(current, def->where()))
	      is_external_input = true;
	  }
	  else {
	    is_external_input = true;
	  }

	  if (is_external_input) {

	    // -- If computing pointers, add the new input

	    if (computePointers)
	      if (info->add_external_input(block))
		found_new_inputs = true;

	    // -- Pass it up to the caller, if it's visible there

	    if (caller &&
		Procedures.is_visible_to(caller, block))
	      external_uses.insert(use);
	  }
	}
      }

  if (pointerOptions::Verbose)
    cout << endl;

  // -- Visit all the defs and identify those that are visible outside the
  // procedure. We reset their current_def and current_use to avoid
  // reaching definition errors.

  if (pointerOptions::Verbose) {
    cout << "Record external outputs to " << procedure->decl()->name() << " at ";
    Procedures.print_call_stack(cout);
    cout << endl;
  }

  // -- Get the location of the input merges

  Location * input_location = 0;
  if (current_callsite)
    input_location = info->procedure_location(current_callsite);

  if ( ! changes.empty() ) {
    for (memoryblock_set_p p = changes.begin();
	 p != changes.end();
	 ++p)
      {
	memoryBlock * block = *p;

	// -- Outputs are non-local if they are accessible outside this
	// procedure. We'll test this in the following way: if the block is
	// not local to the current procedure, but is in scope above it,
	// then it's external.

	if ((block->local_to() != procedure)) {
	  // && Procedures.is_visible_to_caller(info, block)) {

	  // -- Special case: check for a trivial def. If the only def of
	  // this object is the merge of the external inputs, then we
	  // won't really consider this to be a def.

	  bool trivial_def = false;

	  if (input_location &&
	      block->current_def()) {

	    if (block->current_def()->where() == input_location)
	      trivial_def = true;
	  }

	  // -- Special case: don't pass out unallocated or deallocated
	  // heap objects.

	  bool not_allocated = false;

	  if (block->is_heap_object() &&
	      ! block->is_allocation_object()) {
	    
	    memoryuse_set ignore;  
	    Multiplicity multiplicity = current_multiplicity(info, last_stmt_of_proc, block, ignore);
	    if ((multiplicity == Deallocated)) { // || (multiplicity == Unallocated)
	      not_allocated = true;

	      if (pointerOptions::Verbose) {
		cout << "Ignore " << block->name() << " at " << * current << " multiplicity = ";
		memoryBlock::print_multiplicity(multiplicity, cout);
		cout << endl;
	      }
	    }
	  }

	  // -- If we are computing pointers, and this is a non-trivial
	  // def, then add it to the external outputs list.

	  if (computePointers) {
	    if (! trivial_def && ! not_allocated) {
	      if (info->add_external_output(block))
		found_new_inputs = true;
	    }
	    else 
	      if (pointerOptions::Verbose) {
		if (trivial_def)
		  cout << "   Skip external output " << block->name() << " -- trivial def" << endl;
		if (not_allocated)
		  cout << "   Skip external output " << block->name() << " -- no longer allocated" << endl;
	      }
	  }

	  if (! trivial_def && ! not_allocated) {

	    // -- Pass it up to the caller, if it's visible there

	    if (caller &&
		Procedures.is_visible_to(caller, block))
	      external_changes.insert(block);

	    // -- Record this as a non-local change. This helps
	    // pass_external_outputs be more efficient.

	    all_nonlocal_changes.insert(block);
	  }

	  // -- Reset the current def and use
	  
	  block->reset_current_def_use((Location *)0);
	}
      }
  }

  if (pointerOptions::Verbose)
    cout << endl;

  // const memoryblock_set & external_outputs = info->external_outputs();

  // -- If we found any new inputs, mark this procedure for reanalysis

  /*** We shouldn't need to do this
  if (found_new_inputs &&
      info->is_context_insensitive())
    Procedures.mark_for_reanalysis(info, current_callsite);
  */
}


// ------------------------------------------------------------
// eval : Pointer expression evaluator
// ------------------------------------------------------------

void Pointers::eval(procedureInfo * info,
		    stmtLocation * current,
		    exprNode * E,
		    memoryblock_set & defs,
		    memoryuse_set & uses,
		    memoryblock_set & changes,
		    pointerValue & result,
		    bool & never_returns)
{
  switch (E->typ()) {
    
  case Id:
    {
      // === CASE 1: simple identifier
      // Look up the identifier in the memory model

      idNode * id = (idNode *) E;
      procLocation * procloc = Location::procedure(current);
      procNode * proc = procloc->proc();
      memoryBlock * mb = Memory.lookup_variable(procloc, id->decl(), proc);
      result.blocks.insert(mb);

      typeNode * the_type = id->decl()->no_tdef_type();
      if (the_type) {

	// The name of a procedure is also a pointer to that procedure.

	if (the_type->typ() == Func)
	  result.is_address = true;
      }

      // -- Generate uses if required

      if (result.is_a_use)
	generate_uses(info, current, uses, result);

      // -- Constant propagation

      _constants.at_id(current, id, result);

      // -- User-defined analysis

      if (Problem)
	Problem->at_id(current, id, result);
    }
    break;

  case Unary:
    {
      unaryNode * u = (unaryNode *) E;
      unsigned int oper = u->op()->id();
      pointerValue subexpr;

      // Evaluate the sub-expression. This operand subexpression is a
      // "use", unless the operator is address-of

      subexpr.is_a_use = (oper != Operator::ADDRESS);

      if (u->expr())
	eval(info, current, u->expr(),
	     defs, uses, changes, subexpr, never_returns);

      // Apply the pointer-analysis transfer function

      switch (oper) {

      case Operator::ADDRESS:
	{
	  // === CASE 2: Unary "&" : Indicate that the results are to
	  // be used as addresses.

	  result.copy_pointers_from(subexpr);
	  result.is_address = true;

	  // -- Constant propagation

	  _constants.at_address(current, u, subexpr, result);

	  // -- User-defined analysis

	  if (Problem)
	    Problem->at_address(current, u, subexpr, result);
	}
	break;

      case Operator::INDIR:
	{
	  // === CASE 3: Unary "*" : Apply the star operator

	  star_operator(info, current, subexpr, defs, uses, changes, result);

	  // -- Generate uses if required

	  if (result.is_a_use)
	    generate_uses(info, current, uses, result);

	  // -- Constant propagation

	  _constants.at_dereference(current, u, subexpr, result);

	  // -- User-defined analysis

	  if (Problem)
	    Problem->at_dereference(current, u, subexpr, result);
	}
	break;

      case Operator::SIZEOF:
	{
	  // === CASE 3.5: Size of works just like the general unary case,
	  // but doesn't propagate any pointers.

	  // -- Constant propagation

	  _constants.at_unary(current, u, subexpr, result);

	  // -- User-defined analysis

	  if (Problem)
	    Problem->at_unary(current, u, subexpr, result);
	}
	break;

      default:
	{
	  // === CASE 4: Other unary operators have no affect on
	  // pointers.

	  result.copy_pointers_from(subexpr);

	  // -- Generate uses if required

	  if (result.is_a_use)
	    generate_uses(info, current, uses, result);

	  // -- Constant propagation

	  _constants.at_unary(current, u, subexpr, result);

	  // -- User-defined analysis

	  if (Problem)
	    Problem->at_unary(current, u, subexpr, result);
	}
	break;
      }
    }
    break;

  case Binary:
    {
      binaryNode * b = (binaryNode *) E;
      unsigned int id = b->op()->id();
      pointerValue left;
      pointerValue right;

      if ((id == '.') || (id == Operator::ARROW)) {

	// === CASES 5 and 6: binary "." and "->"

	// Get the field
	  
	assert(b->right()->typ() == Id);
	idNode * field = (idNode *) b->right();
	declNode * field_decl = field->decl();

	// Evaluate the struct part of the expression

	left.is_a_use = true;
	eval(info, current, b->left(),
	     defs, uses, changes, left, never_returns);

	if (id == '.') {

	  // Apply the "dot" operator

	  dot_operator(current, field->name(), field_decl, left, uses, result);
	}
	else {

	  // Compute p->field as (*p).field

	  pointerValue after_star;

	  star_operator(info, current, left, defs, uses, changes, after_star);
	  dot_operator(current, field->name(), field_decl, after_star, uses, result);
	}

	// -- Generate uses if required

	if (result.is_a_use)
	  generate_uses(info, current, uses, result);

	// -- Constant propagation

	_constants.at_field_access(current, b, left, field, result);

	// -- User-defined analysis

	if (Problem)
	  Problem->at_field_access(current, b, left, field, result);
      }
      else {

	// Evaluate the right-hand side

	right.is_a_use = true;
	
	eval(info, current, b->right(),
	     defs, uses, changes, right, never_returns);

	// Evaluate the left-hand side. For assignments, the left-hand
	// sides are not uses.

	left.is_a_use = (id != '=');
	eval(info, current, b->left(),
	     defs, uses, changes, left, never_returns);

	// === CASE 7: Index operator

	if (id == Operator::Index) {

	  // -- Index behaves like "*"

	  star_operator(info, current, left, defs, uses, changes, result);

	  // -- Mark the elements as indexed objects

	  mark_as_indexed(result.blocks);

	  // -- Generate uses if required

	  if (result.is_a_use)
	    generate_uses(info, current, uses, result);

	  // -- Constant propagation

	  _constants.at_index(current, b, left, right, result);

	  // -- User-defined analysis
	  
	  if (Problem)
	    Problem->at_index(current, b, left, right, result);
	}
	else {

	  if (id == '=') {
	      
	    // === CASE 8: Assignment

	    // Result is the left-hand side

	    result.copy_pointers_from(left);

	    // Call assignment operator

	    assignment_operator(info, current, (stmtLocation *)0, left, right, defs, uses, changes);

	    // -- Generate uses if required

	    if (result.is_a_use)
	      generate_uses(info, current, uses, result);

	    // -- Constant propagation

	    _constants.at_assignment(current, b,
				     left, right, result,
				     changes);

	    // -- User-defined analysis
	
	    if (Problem)
	      Problem->at_assignment(current, b,
				     left, right, result,
				     changes);

	    // -- Check for struct/union assignment

	    typeNode * btype = b->type()->follow_tdefs();

	    if (btype &&
		((btype->typ() == Struct) ||
		 (btype->typ() == Union)))
	      {
		sueNode * suetype = (sueNode *) btype;
		struct_union_assignment(info, current, b, suetype,
					left, right, defs, uses, changes);
	      }
	  }
	  else {

	    // === CASE 9: All other binary operators. If they involve
	    // pointers and they are appropriate operators, then just pass
	    // the pointers through.

	    if ((id == '+') ||
		(id == '-'))
	      {

		// -- See if the left points to anything

		pointerValue left_after_star;
		star_operator(info, current, left, defs, uses, changes, left_after_star);

		if (! left_after_star.blocks.empty())
		  result.add_blocks_from(left);

		// -- See if the right points to anything

		pointerValue right_after_star;
		star_operator(info, current, right, defs, uses, changes, right_after_star);

		if (! right_after_star.blocks.empty())
		  result.add_blocks_from(right);

		if (pointerOptions::Verbose &&
		    (! result.blocks.empty()))
		  print_memoryblock_set("binaryOp: combination is: ", result.blocks, cout);

		// -- Generate uses if required

		if (result.is_a_use)
		  generate_uses(info, current, uses, result);
	      }

	    // -- Constant propagation

	    _constants.at_binary(current, b, left, right, result);

	    // -- User-defined analysis
	
	    if (Problem)
	      Problem->at_binary(current, b, left, right, result);
	  }
	}
      }
    }
    break;

  case Cast:
    {
      // === CASE 10: casts are essentially no-ops; we evaluate the
      // subexpression into a temporary, then just copy them into the
      // result.

      castNode * c = (castNode *) E;

      pointerValue operand;
      operand.is_a_use = true;

      eval(info, current, c->expr(),
	   defs, uses, changes, operand, never_returns);

      // -- Now just copy the pointer information

      result.copy_pointers_from(operand);

      // -- Generate uses if required

      if (result.is_a_use)
	generate_uses(info, current, uses, result);

      // -- Constant propagation

      _constants.at_cast(current, c, operand, result);

      // -- User-defined analysis

      if (Problem)
	Problem->at_cast(current, c, operand, result);
    }
    break;

  case Call:
    {
      // === CASE 11: function call

      callNode * call = (callNode *) E;
      call_operator(info, current, call,
		    defs, uses, changes, result, never_returns);
    }
    break;

  case Const:
    {
      // === CASE 11: Constants. Zero is interpreted as a null pointer
      // (which we implement as the addess of the null memoryBlock).

      constNode * con = (constNode *) E;

      if (con->value().is_zero()) {
	result.blocks.insert(Memory.null());
	result.is_address = true;
      }

      // -- Handle string constants

      if (con->value().is_str()) {
	memoryBlock * mb = Memory.lookup_string_constant(con);
	result.blocks.insert(mb);
	result.is_address = true;
      }

      // -- Generate uses if required

      if (result.is_a_use)
	generate_uses(info, current, uses, result);

      // -- Constant propagation

      _constants.at_const(current, con, result);

      // -- User-defined analysis

      if (Problem)
	Problem->at_const(current, con, result);
    }
    break;

  default:
    {
      // === CASE 12: All other node types
    }
  }
}

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------

// Star "*" : For each operand, find the nearest dominating definition
// and retrieve the pointer values from it.

void Pointers::star_operator(procedureInfo * info,
			     Location * current,
			     pointerValue & operand,
			     memoryblock_set & defs,
			     memoryuse_set & uses,
			     memoryblock_set & changes,
			     pointerValue & result)
{
  // -- If precision monitor is on, collect any dereferenced pointers that
  // led to this one. This is especially important in Broadway, where we
  // have on_entry expressions like "a --> b --> c". We need imprecision in
  // c to percolate up to a, not just b.

  if (pointerOptions::Monitor_precision) {
    result.dereferenced.insert(operand.dereferenced.begin(),
			       operand.dereferenced.end());
  }

  // -- If operands are addresses, then star is a no-op

  if (operand.is_address) {
    result.copy_pointers_from(operand);
    return;
  }

  // -- Otherwise, visit each operand and follow the pointers

  for (memoryblock_set_p p = operand.blocks.begin();
       p != operand.blocks.end();
       ++p)
    {
      memoryBlock * mb = *p;

      if (mb == Memory.null()) {
	if (pointerOptions::Verbose)
	  cout << "ERROR: Attempt to access location zero." << endl;
      }
      else {

	// -- Look up the use entry (also looks up the reaching
	// definition).

	memoryUse * use = mb->current_use();
	if ( ! use)
	  use = mb->use_at(current);

	uses.insert(use);

	// -- Find the last assignment to it, get points-to edges
	// and add them to the results.

	memoryDef * def = use->reaching_def();

	if (def) {

	  const memoryblock_set & points_to = def->points_to();

	  if (points_to.empty())
	    if (pointerOptions::Verbose)
	      cout << "ERROR: Attempt to dereference a pointer with no value:" <<
		mb->name() << endl;

	  bool may_point_to_null = false;

	  for (memoryblock_set_cp q = points_to.begin();
	       q != points_to.end();
	       ++q)
	    {
	      memoryBlock * target = *q;

	      if (target == Memory.null())
		may_point_to_null = true;
	      else {

		// -- NOTE: we never return the null object from a star
		// operation.

		result.blocks.insert(*q);
	      }
	    }

	  if (pointerOptions::Monitor_precision) {

	    // -- Record that this pointer was dereferenced, but only if it
	    // has multiple targets.

	    if (points_to.size() > 1)
	      result.dereferenced.insert(mb);
	  }

	  // -- Dereference of a possibly null pointer: now it cannot be
	  // null

	  if (0) { // may_point_to_null) {

	    // -- Get all the non-null pointers

	    pointerValue non_null_pointers;

	    for (memoryblock_set_cp q = points_to.begin();
		 q != points_to.end();
		 ++q)
	      {
		memoryBlock * target = *q;

		if (target != Memory.null())
		  non_null_pointers.blocks.insert(target);
	      }

	    // -- Use assignment

	    pointerValue temp(mb);

	    assignment_operator(info, current, (stmtLocation *)0, temp, non_null_pointers, defs, uses, changes);
	  }

	  // OLD CODE: result.blocks = def->points_to();
	}
      }
    }

  // -- Store the accuracy information

  if (current->kind() == Location::Statement) {

    stmtLocation * stmt_loc = (stmtLocation *) current;
    // _star_accuracy[stmt_loc] = count_accuracy_pair(1, (double) result.blocks.size());
  }
}

// Dot "." : For each operand, retrieve the block corresponding to the
// field with the given name. Fields are created as necessary (on
// demand).

void Pointers::dot_operator(Location * current,
			    const string & field_name,
			    declNode * field_decl,
			    pointerValue & operand,
			    memoryuse_set & uses,
			    pointerValue & result)
{
  if (operand.is_address) {
    if (pointerOptions::Verbose)
      cout << "ERROR: Cannot apply \".\" to address." << endl;
    return;
  }

  for (memoryblock_set_p p = operand.blocks.begin();
       p != operand.blocks.end();
       ++p)
    {
      memoryBlock * mb = *p;
      
      if (mb == Memory.null()) {
	if (pointerOptions::Verbose)
	  cout << "ERROR: Attempt to access memory at address zero." << endl;
      }
      else {

	// -- Update the uses information: "p.field" is a use of p.

	memoryUse * use = mb->current_use();
	if ( ! use)
	  use = mb->use_at(current);

	uses.insert(use);

	if (pointerOptions::Ignore_fields) {

	  // -- Option: struct/union field access is a no-op

	  result.blocks.insert(mb);

	  // -- Force weak updates..

	  mb->set_indexed();
	}
	else {

	  // -- Retrieve the field

	  memoryBlock * block = *p;
	  block->dot(field_name, field_decl, Memory, result.blocks);
	}
      }
    }

  // -- Copy all the dereferenced pointers

  result.dereferenced.insert(operand.dereferenced.begin(),
			     operand.dereferenced.end());
}

/** @brief Struct/union assignment
 *
 * Direct assignments between structs and unions are allowed, so here we
 * recursively descend through the fields assigning them in a pairwise
 * fashion. NOTE: Here we are relying on having the correct types. */

void Pointers::struct_union_assignment(procedureInfo * info,
				       stmtLocation * current,
				       binaryNode * binary,
				       sueNode * suetype,
				       pointerValue & left_hand_side,
				       pointerValue & right_hand_side,
				       memoryblock_set & defs,
				       memoryuse_set & uses,
				       memoryblock_set & changes)
{
  // -- Visit each field in the struct/union definition

  suespecNode * spec = suetype->spec();
  
  if (spec) {

    decl_list & fields = spec->fields();

    for (decl_list_p p = fields.begin();
	 p != fields.end();
	 ++p)
      {
	declNode * field = *p;
	    
	// -- Get the field from the left-hand side

	pointerValue left_field;

	dot_operator(current, field->name(), field,
		     left_hand_side, uses, left_field);

	// -- Get the field from the right-hand side

	pointerValue right_field;

	dot_operator(current, field->name(), field,
		     right_hand_side, uses, right_field);

	generate_uses(info, current, uses, right_field);	

	// -- Assign the current objects

	assignment_operator(info, current, (stmtLocation *)0, left_field, right_field, 
			    defs, uses, changes);

	pointerValue ignore_result;

	// -- Constant propagation

	_constants.at_assignment(current, binary,
				 left_field, right_field, ignore_result,
				 changes);

	// -- User-defined analysis

	if (Problem)
	  Problem->at_assignment(current, binary,
				 left_field, right_field, ignore_result,
				 changes);

	// -- If this field is itself a struct/union, then call this method
	// recursively

	typeNode * field_type = field->no_tdef_type();

	if ((field_type->typ() == Struct) ||
	    (field_type->typ() == Union))
	  {
	    sueNode * field_suetype = (sueNode *) field_type;

	    struct_union_assignment(info, current, binary, field_suetype,
				    left_field, right_field, defs, uses, changes);
	  }
      }
  }
}

/** @brief Assignment "=" operator
 *
 * When the additive flag is set, the assignment will include the previous
 * value of the pointer at this location. This allows us to say things like
 * "p = &x and p = &y" without having to build a single pointerValue that
 * contains both x and y.
 * */

void Pointers::assignment_operator(procedureInfo * info,
				   Location * current,
				   stmtLocation * parameter_callsite,
				   pointerValue & left_hand_side,
				   pointerValue & right_hand_side,
				   memoryblock_set & defs,
				   memoryuse_set & uses,
				   memoryblock_set & changes)
{
  bool unique_lhs = false;
  bool changed = false;
  
  // -- Quick short-circuit

  if (left_hand_side.blocks.empty())
    return;

  // -- Build the RHS set of pointers

  memoryblock_set rhs_points_to;

  // -- Precision analysis: keep track of may and must pointers

  int min_size = 999999;
  bool any_must_pointers = false;
  memoryblock_set complicit_pointers;

  // -- Handle the two different kinds of RHS

  if (right_hand_side.is_address) {

    // -- If the RHS is a list of addresses, then the new points-to set
    // just consists of the RHS objects themselves...

    memoryblock_set & rhs_blocks = right_hand_side.blocks;

    rhs_points_to.insert(rhs_blocks.begin(),
			 rhs_blocks.end());

    // -- Special case: for address objects, set any_must_pointers to true

    // any_must_pointers = true;
    min_size = 1; // rhs_blocks.size();
  }
  else {

    // Regular assignment: visit each RHS objects and collect the objects
    // that they point to.

    for (memoryblock_set_p q = right_hand_side.blocks.begin();
	 q != right_hand_side.blocks.end();
	 ++q)
      {
	memoryBlock * right = *q;

	// Look up a use entry -- NOTE: the reaching definition
	// should already be there from generate_uses.

	memoryUse * right_use = right->current_use();
	if ( ! right_use)
	  right_use = right->use_at(current);

	// Find the last assignment to it, get points-to edges and
	// add them to the LHS.

	memoryDef * reaching_def = right_use->reaching_def();
	if (reaching_def) {
	  const memoryblock_set & reaching_points_to = reaching_def->points_to();

	  rhs_points_to.insert(reaching_points_to.begin(),
			       reaching_points_to.end());

	  // -- Monitor: keep track of may and must pointers.

	  if (pointerOptions::Monitor_precision) {

	    int size = reaching_points_to.size();
	    if (size < min_size)
	      // any_must_pointers = true;
	      min_size = size;

	    if (size > 1)
	      complicit_pointers.insert(right);
	  }
	  
	}
	else
	  if (pointerOptions::Verbose) {
	    cout << "WARNING: Variable " << right->name() <<
	      " may not have a value at " << *current << endl;
	    right->print_def_use(cout);
	  }
      }
  }

  // -- If there is only one object on the left, then it is a strong
  // update.

  if (left_hand_side.blocks.size() == 1)
    unique_lhs = true;

  // -- Store the accuracy information

  if (current->kind() == Location::Statement) {

    stmtLocation * stmt_loc = (stmtLocation *) current;
    // _def_accuracy[stmt_loc] = count_accuracy_pair(1, (double) left_hand_side.blocks.size());
  }

  // -- Save the size of the RHS points-to set

  int rhs_points_to_size = rhs_points_to.size();

  // -- Monitor: if the right-hand-side pointer value is larger than one,
  // even though there were must-pointers on the right-hand-side, then
  // we'll blame the dereferenced objects.

  // Example:  p = *q; where     /-> a --> b
  //                         q -<
  //                             \-> c --> d

  if (pointerOptions::Monitor_precision) {

    if ((rhs_points_to_size > 1) && (rhs_points_to_size > min_size)) // any_must_pointers)
      complicit_pointers.insert(right_hand_side.dereferenced.begin(),
				right_hand_side.dereferenced.end());
  }

  // -- Bidirectional assignment

  if (pointerOptions::Bidirectional_assignment) {

    // -- The way we implement "equality-based" analysis is sort of a hack:
    // we take all the objects on the right-hand side and force them to
    // also be on the left. For example, "y = x" becomes "{y,x} = x". This
    // essentially forces all of the objects to take on the same value.

    if ( ! right_hand_side.is_address) {
      left_hand_side.blocks.insert(right_hand_side.blocks.begin(),
				   right_hand_side.blocks.end());
    }

    // -- Also, collect all the existing points-to sets from the left-hand
    // side objects.

    for (memoryblock_set_p p = left_hand_side.blocks.begin();
	 p != left_hand_side.blocks.end();
	 ++p)
      {
	memoryBlock * left = *p;

	memoryDef * def = left->find_def_at(current);
	if (def) {
	  const memoryblock_set & points_to = def->points_to();

	  rhs_points_to.insert(points_to.begin(),
			       points_to.end());
	}
      }
  }

  // -- For each object on the left

  for (memoryblock_set_p p = left_hand_side.blocks.begin();
       p != left_hand_side.blocks.end();
       ++p)
    {
      memoryBlock * left = *p;

      // -- Are we allowed to modify this object?

      if ( ! left->write_protected()) {

	// -- Look up or create a def entry

	bool is_new;
	memoryDef * left_def = left->def_at(current, is_new);
	defs.insert(left);

	// -- If we are actually computing the pointer info...

	if (computePointers) {

	  // -- If the assignment resulted in a new memoryDef instance, then
	  // it automatically is a "change".

	  if (is_new) {
	    changes.insert(left);
	    changed = true;
	  }

	  // -- Save the current points-to edges. Here we don't mean the
	  // incoming flow value, we mean the flow value at this point the
	  // last time we visited it. We'll use this to test for convergence.

	  memoryblock_set previous = left_def->points_to();

	  // -- All this junk is only needed for flow-sensitive objects

	  bool single_multiplicity = false;
	  bool strong_update = true;
	  memoryDef * reaching_def = 0;
	  bool reaching_def_is_must_pointer = false;

	  Multiplicity multiplicity = current_multiplicity(info, current, left, uses);

	  if (left->is_flow_sensitive()) {

	    // -- Figure out whether this is a strong or weak update. We can
	    // perform a strong update when we have a single LHS object, and
	    // that object is not a multi-object

	    single_multiplicity = ((multiplicity == Unallocated) ||
				   (multiplicity == Deallocated) ||
				   (multiplicity == Single));

	    strong_update = unique_lhs && single_multiplicity;

	    // Standard version: strong_update = unique_lhs && (multiplicity == Single);

	    // -- For weak updates, we include the points-to values before the
	    // assignment; that is, we just accumulate points-to information at
	    // each assignment.
	    
	    if ( ! strong_update) {

	      // -- Find and set the previous reaching definition in
	      // preparation for a weak def.

	      reaching_def = nearest_def_at(info, left, current);
	      // left_def->set_previous(reaching_def);

	      // -- If the def wasn't weak and now it is, then that's a
	      // change

	      if ( ! left_def->is_weak()) {
		changes.insert(left);
		changed = true;
	      }

	      left->apply_weak_update(current, reaching_def, uses);

	      // -- Record if the reaching was a must pointer

	      if (reaching_def &&
		  (reaching_def->points_to().size() <= 1))
		reaching_def_is_must_pointer = true;
	    }
	  }

	  // -- Perform the points-to assignment

	  left_def->add_pointers(rhs_points_to);

	  const memoryblock_set & lhs_points_to = left_def->points_to();

	  // -- Monitor: if the left-hand-side object ends up as a
	  // may-pointer, figure out why and then store the destructive and
	  // complicit assignment information.

	  if (pointerOptions::Monitor_precision) {

	    // -- If this is a parameter being passed, then record the
	    // reaching defs.

	    if ((parameter_callsite) &&
		(current->kind() == Location::Procedure))
	      {
		procLocation * procloc = (procLocation *) current;

		left->add_parameter_assignment(procloc->proc(),
					       parameter_callsite,
					       right_hand_side.blocks);
	      }

	    if (lhs_points_to.size() > 1) {

	      // -- Any right-hand-side may-pointers are automatically complicit

	      left->add_complicit_assignment(current, complicit_pointers);

	      // -- Check for strong or weak update...

	      if ( ! strong_update) {

		// -- If a weak update occured, check to see if it was caused by
		// multiple left-hand-sides or by multiplicity.

		if ( ! unique_lhs) {

		  // -- Multiple left-hand-sides: the problem is the
		  // left-hand dereferenced pointers.

		  left->add_complicit_assignment(current, left_hand_side.dereferenced);
		}

		if ( ! single_multiplicity) {

		  // -- High-multiplicity: this is an additive assignment
		  // because of allocation or array access. If the previous
		  // reaching def was a must pointer, then this is a
		  // destructive assignment.

		  if (reaching_def_is_must_pointer) {

		    // -- If this is a heap object, then blame the
		    // allocation information attached to the alloc
		    // object. Otherwise, just record it as a destructive
		    // assignment.

		    memoryBlock * alloc_object = left->allocation_object();
		    if (alloc_object)
		      left->add_complicit_assignment(current, alloc_object);
		    else
		      left->add_destructive_assignment(current, memoryBlock::Weak_update);
		  }
		}
	      }
	      else {

		// -- Strong update: these kinds of assignments are only
		// destructive if the right-hand-side evaluates to a single
		// must-pointer value, but the left ends up as a may
		// pointer.

		if (rhs_points_to_size < lhs_points_to.size()) {

		  if (current->kind() == Location::Statement) {

		    // -- Regular statement: this can only happend with an
		    // additive assignment caused by flow-insensitivity.
		    
		    left->add_destructive_assignment(current, memoryBlock::Additive);
		  }

		  if (current->kind() == Location::Procedure) {

		    // -- Parameter pass: this happens when the procedure
		    // is context-insensitive and it gets different values
		    // in the different contexts.

		    left->add_destructive_assignment(current, memoryBlock::Parameter_pass);
		  }
		}
	      }
	    }
	  } // -- END monitor

	  // -- Check for change

	  if (lhs_points_to != previous) {
	    changes.insert(left);
	    changed = true;

	    /*
	    if ( ! previous.empty()) {
	      if (left->is_flow_sensitive())
		cout << "FS-";
	      else
		cout << "FI-";

	      if (current->kind() == Location::Statement)
		cout << "ASSIGN:";
	      else
		cout << "PASS:";

	      cout << " lost information about " 
		   << left->name() << " at " << * current << endl;
	      print_memoryblock_set("    WAS: ", previous, cout);
	      print_memoryblock_set("    NOW: ", lhs_points_to, cout);
	    }
	    */
	  }

	  if (pointerOptions::Verbose) {

	    if (left->is_flow_sensitive()) {
	      if (strong_update)
		cout << "  Strong assign -- ";
	      else
		cout << "  Weak assign -- ";
	    }
	    else
	      cout << "  Flow-insensitive assign -- ";

	    memoryBlock::print_multiplicity(multiplicity, cout);
	    cout << " -- ";

	    if (changed)
	      cout << "changed: ";
	    else
	      cout << "unchanged: ";

	    cout << left->name() << endl;
	    left_def->print(cout);

	    if ( ! lhs_points_to.empty())
	      print_memoryblock_set("      points-to NOW: ", lhs_points_to, cout);

	    if (reaching_def) {
	      cout << "    Reaching def at " << * (reaching_def->where());
	      print_memoryblock_set(" before assignment: ", reaching_def->points_to(), cout);
	    }

	    if (changed && ( ! previous.empty()))
	      print_memoryblock_set("      points-to WAS: ", previous, cout);
	    
	    cout << endl;
	  }
	}
	else {

	  // Not computing pointers...

	  // -- Make sure that the "current_use" field is set for blocks that
	  // have weak updates.

	  if (left_def->is_weak()) {
	    memoryUse * use = left->use_at(current);
	    uses.insert(use);
	  }

	  if (is_new) {

	    if (pointerOptions::Verbose) {
	      cout << "WARNING: in assignment: No new defs of " << left->name()
		   << " should be created at " << *current << " in ";
	      Procedures.print_call_stack(cout);
	      cout << endl;
	    }

	    changes.insert(left);
	  }
	}
      }
    } /* END for all LHS */
}

void Pointers::merge_operator(procedureInfo * info,
			      Location * merge_point,
			      memoryBlock * block_to_merge,
			      memoryuse_list & phi_uses,
			      memoryblock_set & defs,
			      memoryuse_set & uses,
			      memoryblock_set & changes)
{
  bool changed = false;

  // -- Are we allowed to modify this object?

  if (block_to_merge->write_protected())
    return;

  // -- Set up a def at the merge point

  bool is_new;
  memoryDef * def = block_to_merge->def_at(merge_point, is_new);
  defs.insert(block_to_merge);

  // -- If we are actually computing the pointer info...

  if (computePointers) {

    // -- If the merge resulted in a new memoryDef instance, then it
    // automatically is a "change".

    if (is_new) {
      changes.insert(block_to_merge);
      changed = true;
    }

    if (block_to_merge->is_allocation_object()) {

      // --------------------------------------------------
      //   Special behavior for allocation objects

      // -- Save the previous value

      Multiplicity old_multiplicity = def->multiplicity();

      // -- Compute the multiplicity of the new definition

      Multiplicity multiplicity = Unallocated;

      // -- Merge in the multiplicity values of each use

      Multiplicity min = Error;

      for (memoryuse_list_p use_p = phi_uses.begin();
	   use_p != phi_uses.end();
	   ++use_p)
	{
	  memoryUse * use = *use_p;

	  uses.insert(use);

	  // -- Get the reaching value

	  memoryDef * reaching_def = use->reaching_def();
	  if (reaching_def) {

	    // -- Include the multiplicity: we'll use max() as the meet
	    // function because the Multiplicity enum is ordered so that
	    // the conservative value is the largest.

	    Multiplicity reaching_multiplicity = reaching_def->multiplicity();

	    if (reaching_multiplicity > multiplicity)
	      multiplicity = reaching_multiplicity;

	    // -- Compute the min value as well. We need this for the
	    // aggressive multiplicity mode.

	    if (reaching_multiplicity < min)
	      min = reaching_multiplicity;
	  }
	}

      // -- Aggressive mode: if the new value is Single and the min value
      // is Deallocated, then just go to Unallocated. This is not sound,
      // but probably is correct in practice. Question: is there a way we
      // can make it sound?

      if (pointerOptions::Aggressive_multiplicity) {

	if ((multiplicity == Single) &&
	    (min == Deallocated))
	  multiplicity = Unallocated;
      }

      // -- Always meet with the old value

      if (old_multiplicity > multiplicity)
	multiplicity = old_multiplicity;

      // -- Set the new multiplicity

      def->set_multiplicity(multiplicity);

      // -- Check for change

      if (multiplicity != old_multiplicity) {
	changes.insert(block_to_merge);
	changed = true;
      }

      if (pointerOptions::Verbose) {
	cout << "Merge -- ";

	if (changed)
	  cout << "changed: ";
	else
	  cout << "unchanged: ";

	cout << block_to_merge->name() << " : ";

	memoryBlock::print_multiplicity(old_multiplicity, cout);
	cout << " -> ";
	memoryBlock::print_multiplicity(multiplicity, cout);
	cout << endl;

	def->print(cout);
	cout << endl;
      }
    }
    else {

      // --------------------------------------------------
      //   Regular objects

      // -- Save the previous points-to edges

      memoryblock_set previous = def->points_to();

      // -- Record if any of the reaching defs are must-pointers

      bool any_must_pointers = false;

      // -- Collect the new points-to set from the merge uses

      memoryblock_set merged_points_to;

      for (memoryuse_list_p use_p = phi_uses.begin();
	   use_p != phi_uses.end();
	   ++use_p)
	{
	  memoryUse * use = *use_p;

	  if (! use) {

	    cout << "ERROR: block " << block_to_merge->name() << " at " << *merge_point << endl;
	    cout << "    Num uses = " << phi_uses.size() << endl;
	    block_to_merge->print_def_use(cout);
	  }

	  uses.insert(use);

	  // -- Add the pointers from the reaching definition. It is
	  // possible that there is no reaching definition on one or more
	  // of the uses. This typically means that the variable being
	  // merged is dead, but we arent' checking that right now.

	  memoryDef * reaching_def = use->reaching_def();
	  if (reaching_def) {
	    const memoryblock_set & reaching_points_to = reaching_def->points_to();

	    if (reaching_points_to.size() <= 1)
	      any_must_pointers = true;

	    merged_points_to.insert(reaching_points_to.begin(),
				   reaching_points_to.end());
	  }
	}

      // -- Add the merge points-to set

      def->add_pointers(merged_points_to);

      // -- Monitor: check for destructive merge

      if (pointerOptions::Monitor_precision) {

	if ((def->points_to().size() > 1) &&
	    any_must_pointers) {

	  // -- This merge caused must-pointers to be lost

	  block_to_merge->add_destructive_assignment(merge_point, memoryBlock::Control_flow);
	}
      }

      // -- Check for change

      if (def->points_to() != previous) {
	changes.insert(block_to_merge);
	changed = true;
      }

      if (pointerOptions::Verbose) {
	cout << "Merge -- ";

	if (changed)
	  cout << "changed: ";
	else
	  cout << "unchanged: ";

	cout << block_to_merge->name() << endl;
	def->print(cout);

	if ( ! def->points_to().empty())
	  print_memoryblock_set("      points-to: ", def->points_to(), cout);
	cout << endl;
      }
    }
  }
  else {

    // Not computing pointers...

    if (is_new) {

      if (pointerOptions::Verbose) {
	cout << "WARNING: in merge: No new defs of " << block_to_merge->name() <<
	  " should be created at " << * merge_point << endl;
	Procedures.print_call_stack(cout);

	cout << endl;

	block_to_merge->print_def_use(cout);

	cout << endl;
      }

      changes.insert(block_to_merge);
    }
  }
}

/** @brief Self-assignment
 *
 * This method is used to pass an external input or output. It works like
 * a hybrid of the assignment operator and the merge operator, except
 * that it allows the use to be at a different location from the def. */

void Pointers::self_assignment(Location * source,
			       Location * target,
			       memoryBlock * block,
			       memoryblock_set & changes)
{
  bool changed = false;

  // -- Are we allowed to modify this object?

  if (block->write_protected())
    return;

  // -- Set up a def at the target location

  bool is_new;
  memoryDef * def = block->def_at(target, is_new);

  // -- Get the use from the source location. Note: any method calling
  // this one should probably set the reaching def on this use.

  memoryUse * use = block->use_at(source);

  // -- Record the special relationship:

  def->set_self_assign_use(use);

  // -- Get the reaching def

  memoryDef * reaching_def = use->reaching_def();

  // -- If we are actually computing the pointer info...

  if (computePointers) {

    // -- If the merge resulted in a new memoryDef instance, then it
    // automatically is a "change".

    if (is_new) {
      changes.insert(block);
      changed = true;
    }

    // -- Flow insensitive objects don't need self-assignment

    if ( ! block->is_flow_sensitive())
      return;

    // -- Record the self-assignment, if this is an input

    if ((target->kind() == Location::Procedure) &&
	(source->kind() == Location::Statement))
    {
      procLocation * procloc = (procLocation *) target;
      stmtLocation * callsite = (stmtLocation *) source;

      // -- Let Broadway handle property blocks

      if ( ! block->property)
	block->add_parameter_assignment(procloc->proc(),
					callsite,
					block);
    }

    if (block->is_allocation_object()) {

      // --------------------------------------------------
      //   Special behavior for allocation objects

      // -- Save the previous value

      Multiplicity old_multiplicity = def->multiplicity();

      // -- Get the reaching multiplicity value. If there isn't one, then
      // the value will be "Unallocated", which behaves like TOP.

      Multiplicity reaching_multiplicity = Unallocated;
      if (reaching_def)
	reaching_multiplicity = reaching_def->multiplicity();

      // -- Always meet with the old value (take the max)

      Multiplicity multiplicity;

      if (old_multiplicity > reaching_multiplicity)
	multiplicity = old_multiplicity;
      else
	multiplicity = reaching_multiplicity;

      // -- Set the new multiplicity
      
      def->set_multiplicity(multiplicity);

      // -- Monitor: check for parameter pass loss of allocation
      // information

      if (pointerOptions::Monitor_precision) {

	// -- A destructive assignment for the allocation object occurs
	// when the multiplicity value coming from different call sites is
	// different. The only case we care about is when one of the values
	// is Single and the other is Unbounded.

	if (((old_multiplicity == Single) && (reaching_multiplicity == Unbounded)) ||
	    ((old_multiplicity == Unbounded) && (reaching_multiplicity == Single))) {

	  /*
	  cout << "DESTRUCTIVE " << block->name() << " at " << * target << " : ";
	  memoryBlock::print_multiplicity(old_multiplicity, cout);
	  cout << " -> ";
	  memoryBlock::print_multiplicity(reaching_multiplicity, cout);
	  cout << endl;
	  */

	  block->add_destructive_assignment(target, memoryBlock::Parameter_pass);
	}
      }

      // -- Check for change

      if (multiplicity != old_multiplicity) {
	changes.insert(block);
	changed = true;
      }

      if (pointerOptions::Verbose) {
	cout << "  Self-assign -- ";

	if (changed)
	  cout << "changed: ";
	else
	  cout << "unchanged: ";

	cout << block->name() << " : ";

	memoryBlock::print_multiplicity(old_multiplicity, cout);
	cout << " -> ";
	memoryBlock::print_multiplicity(multiplicity, cout);
	cout << endl;

	def->print(cout);
      }
    }
    else {

      // --------------------------------------------------
      //   Regular variables

      // -- Save the previous points-to edges

      memoryblock_set previous = def->points_to();

      // -- Add in points-to edges from the source

      if (reaching_def) {

	// -- Add the pointers

	def->add_pointers(reaching_def->points_to());
      }

      // -- Monitor: check for parameter pass loss of information

      if (pointerOptions::Monitor_precision) {

	memoryBlock * current = block;

	// -- Find the top-most object container

	while (current->container())
	  current = current->container();

	if (current->decl()->decl_location() == declNode::TOP) {
	  if (reaching_def &&
	      (previous.size() > 0) &&
	      (reaching_def->points_to().size() > 0) &&
	      (def->points_to().size() > reaching_def->points_to().size())) {

	    // -- The reaching def was a must-pointer, but the assignment
	    // resulted in a may-pointer.

	    block->add_destructive_assignment(target, memoryBlock::Parameter_pass);
	  }
	}
      }

      // -- Check for change

      if (def->points_to() != previous) {
	changes.insert(block);
	changed = true;
      }

      if (pointerOptions::Verbose) {
	cout << "  Self-assign -- ";

	if (changed)
	  cout << "changed: ";
	else
	  cout << "unchanged: ";

	cout << block->name() << endl;
	cout << "    From " << * source << " to " << * target << endl;
	def->print(cout);

	if ( ! def->points_to().empty())
	  print_memoryblock_set("      points-to: ", def->points_to(), cout);
	cout << endl;
      }
    }
  }
  else {

    // Not computing pointers...

    if (is_new) {

      if (pointerOptions::Verbose) {
	cout << "ERROR in Self-assign: No new defs of " << block->name() <<
	  " should be created at " << * target << endl;
	Procedures.print_call_stack(cout);
	cout << endl;
      }
      // Return the def so that the caller can access it...

      changes.insert(block);
    }
  }
}


void Pointers::call_operator(procedureInfo * caller,
			     stmtLocation * current,
			     callNode * call,
			     memoryblock_set & defs,
			     memoryuse_set & uses,
			     memoryblock_set & changes,
			     pointerValue & result,
			     bool & never_returns)
{
  // Evaluate the actual arguments, store the results in a list of
  // pointerValues.

  expr_list & args = call->args();
  pointervalue_list actuals;

  for (expr_list_p p = args.begin();
       p != args.end();
       ++p)
    {
      exprNode * actual = * p;
      actuals.push_back(pointerValue());
      pointerValue & actual_results = actuals.back();

      // -- Special case: a va_list argument must be dereferenced before
      // passing it. Since they always point to the special ellipsis
      // variable, this will pass the value of the ellipsis through.

      if (0) { // is_va_list(actual)) {

	pointerValue temp;
	temp.is_a_use = true;
	eval(caller, current, actual,
	     defs, uses, changes, temp, never_returns);

	// -- Dereference the va_list

	star_operator(caller, current, temp, defs, uses, changes, actual_results);

	// -- Generate uses

	generate_uses(caller, current, uses, actual_results);
      }
      else {

	// Evaluate the argument and save the results

	actual_results.is_a_use = true;
	eval(caller, current, actual,
	     defs, uses, changes, actual_results, never_returns);
      }
    }

  if (pointerOptions::Verbose) {
    cout << "  Arguments:" << endl;
    int count = 0;
    for (pointervalue_list_p p = actuals.begin();
	 p != actuals.end();
	 ++p)
      {
	cout << "    Arg " << count << " : ";
	if ((*p).is_address)
	  cout << "pointer-to: ";
	print_memoryblock_set("", (*p).blocks, cout);
	count++;
      }
    cout << endl;
  }

  // -- Determine the target of the call

  pointerValue call_targets;
  determine_call_targets(caller, current, call, defs, uses, changes, call_targets);

  // -- Call the analyzer recursively. NOTE: all the special-case calls
  // (e.g., malloc and free) are handled in the procedure_call() method.

  procedure_call(caller, current, call, call_targets, actuals,
		 defs, uses, changes, result, never_returns);
}

void Pointers::determine_call_targets(procedureInfo * caller,
				      stmtLocation * current,
				      callNode * call,
				      memoryblock_set & defs,
				      memoryuse_set & uses,
				      memoryblock_set & changes,
				      pointerValue & call_targets)
{
  // Evaluate the name of the procedure being called (could be
  // pointer expression).

  pointerValue temp;
  temp.is_a_use = true;
  bool exit_not_allowed_here;
  eval(caller, current, call->name(),
       defs, uses, changes, temp, exit_not_allowed_here);

  // If necessary, dereference the pointers. This handles the
  // fact that if we have a pointer to a function we can call
  // it in two different ways:

  // (*f)(...)
  // f(...)

  // We distinguish this special case: if the call target is an
  // identifier that does not refer to a function, then
  // automatically dereference it.

  bool deref = false;
  if (call->name()->typ() == Id) {
    idNode * call_name = (idNode *) call->name();
    if (call_name->decl()->no_tdef_type()->typ() != Func)
      deref = true;
  }

  if (deref)
    star_operator(caller, current, temp, defs, uses, changes, call_targets);
  else
    call_targets = temp;
}

void Pointers::generate_uses(procedureInfo * info,
			     Location * where,
			     memoryuse_set & uses,
			     pointerValue & pointer)
{
  if ( ! pointer.is_address ) {

    for (memoryblock_set_p p = pointer.blocks.begin();
	 p != pointer.blocks.end();
	 ++p)
      {
	memoryBlock * block = (*p);

	// -- Find the memoryUse object for this location, creating one if
	// necessary.

	memoryUse * use = block->use_at(where);
	uses.insert(use);

	// -- Context insensitive analysis: the reaching definition could
	// be in another context.

	if (computePointers) {

	  // -- Look for a non-local reaching definition

	  memoryDef * def = nearest_def_at(info, block, where);
	  use->reaching_def(def);
	}
      }
  }
}

/** @brief Reaching def
 *
 * This is a special version of "nearest dominating def" that works
 * across context insensitive boundaries. It handles the case where we
 * discover an "input" to the current context that wasn't in previous
 * analyses of the procedure.
 */

memoryDef * Pointers::nearest_def_at(procedureInfo * info,
				     memoryBlock * block,
				     Location * where)
{
  memoryDef * def = block->nearest_def_at(where);
  procedureInfo * current_info = 0;
  stmtLocation * current_callsite = 0;

  // -- Move up the call stack until we find a reaching def

  const procedurecall_stack & callstack = Procedures.callstack();

  procedurecall_stack_crp stack_p = callstack.rbegin();

  while ( ! def &&
	  (stack_p != callstack.rend()))
    {
      current_callsite = (*stack_p).first;
      current_info = (*stack_p).second;

      // -- Short-circuit: don't both looking for a non-local def of a local
      // variable

      if (block->local_to() == current_info->proc())
	return 0;

      // -- See if there is a def that reaches the current callsite

      // -- YIKES: this was a major bug. Because of the dominance rules for
      // ancestors and descendants, it's critical that we look for a
      // dominator of the called procLocation, not the callsite
      // stmtLocation itself.

      if (current_callsite &&
	  current_callsite->calls())
	def = block->nearest_def_at(current_callsite->calls());

      // -- Move up the call stack

      stack_p++;
    }

  /*
  if (computePointers) {
  
    // -- If we do find a reaching def, add it to the external inputs for all
    // the procedures in between.

    if (def) {

      procedureInfo * current_info_2 = info;

      while (current_info_2 != current_info) {

	// -- Add the new input

	current_info_2->add_external_input(block);

	// -- Move up the call stack

	current_info_2 = current_info_2->current_caller();
      }
    }
  }
  */

  /*
  if (pointerOptions::Verbose && def && (current_info != info)) {
    cout << "   Pointers::nearest_def_at: Found non-local def of " << block->name() << endl; 
    cout << "       Def at " << * (def->where()) << endl;
    cout << "       Reaches " << * where << endl;
  }
  */

  return def;
}


// ------------------------------------------------------------
//  Conservative procedure call mechanics
// ------------------------------------------------------------

void Pointers::conservative_procedure_call(procedureInfo * info,
					   stmtLocation * current,
					   pointervalue_list & arguments,
					   memoryblock_set & reachable,
					   memoryblock_set & external_defs,
					   memoryuse_set & external_uses,
					   memoryblock_set & external_changes,
					   pointerValue & return_val)
{
  // -- Find all the memory blocks that could be reached throught the
  // formal parameters...

  memoryblock_list worklist;

  // -- Visit all the input arguments and build an initial worklist

  for (pointervalue_list_p p = arguments.begin();
       p != arguments.end();
       ++p)
    {
      pointerValue & pointer = (*p);

      for (memoryblock_set_p q = pointer.blocks.begin();
	   q != pointer.blocks.end();
	   ++q)
	{
	  memoryBlock * arg = (*q);

	  // -- Ignore the "null" node

	  if (arg != Memory.null()) {

	    // -- Put each block on the starting worklist

	    worklist.push_back(arg);

	    // -- For address arguments (e.g., "&x"), we can already assume
	    // that the object is reachable.

	    if (pointer.is_address)
	      reachable.insert(arg);
	  }
	}
    }

  // -- Generate the reachable blocks

  reachable_blocks(current, worklist, reachable);

  // -- Create uses and defs for these objects. We'll do this by setting up
  // an assignment that looks like this: for all variables V, V = &(any
  // other object).

  // BAIL OUT: This approach is too inefficient

  pointerValue left;
  pointerValue right;

  // -- Make pointerValue objects

  // left.blocks = reachable;
  right.blocks = reachable;

  // -- Generate uses

  generate_uses(info, current, external_uses, right);

  /* Don't do it this way
  // -- Now perform the assignment
  right.is_address = true;
  assignment_operator(current, left, right, external_changes);
  */

  // -- Just generate defs:

  for (memoryblock_set_p p = reachable.begin();
       p != reachable.end();
       ++p)
    {
      memoryBlock * block = *p;

      // -- Are we allowed to modify this object?

      if ( ! block->write_protected()) {

	bool is_new;
	memoryDef * def = block->def_at(current, is_new);
	external_defs.insert(block);

	if (is_new)
	  external_changes.insert(block);

	// --- Ug: if it was previously a pointer, then it could point to
	// anything:

	memoryDef * reaching_def = block->current_use()->reaching_def();
	if (reaching_def &&
	    ! reaching_def->points_to().empty()) {
	  def->add_pointers(reaching_def->points_to());
	}
      }
    }

  if (pointerOptions::Verbose)
    print_memoryblock_def_set("  Potentially reachable:", reachable, cout);
}

/** @brief Reachable blocks
 *
 * Given a set of memoryBlocks, compute the set of all blocks reachable
 * by following pointer or structure-field relationships. */

void Pointers::reachable_blocks(Location * where,
				memoryblock_list & worklist,
				memoryblock_set & already_seen)
{
  while ( ! worklist.empty()) {

    // -- Remove the first element from the worklist..

    memoryBlock * mb = worklist.front();
    worklist.pop_front();

    // -- Add all the immediately reachable blocks (that we haven't seen yet)

    mb->reachable_blocks(where, false, worklist, already_seen, Memory.null());
  }
}

// ------------------------------------------------------------
// Manage the Heap
// ------------------------------------------------------------

memoryBlock * Pointers::at_allocation(const string & name,
				      procedureInfo * info,
				      stmtLocation * current,
				      stmtNode * allocation_stmt,
				      declNode * decl,
				      memoryblock_set & defs,
				      memoryuse_set & uses,
				      memoryblock_set & changes,
				      pointerValue & result)
{
  bool changed;
  declNode * the_decl = decl;
  bool from_heap_map = false;

  if (decl == 0) {

    // -- Lookup or create a declNode for this site

    from_heap_map = true;

    stmtNode * stmt = current->stmt();
    heap_map_p p = HeapMap.find(stmt);
    if (p == HeapMap.end()) {

      // -- Not there, create it.

      // -- Add a unique number onto the name

      char buf[50];

      sprintf(buf, "_#%d", Memory.counter());

      string fullname = name + buf;

      // -- Create a synthetic declaration

      the_decl = new declNode(fullname.c_str(),
			      declNode::NONE,
			      (typeNode *)0,
			      (exprNode *)0,
			      (exprNode *)0);

      // -- Store it for future use

      HeapMap[stmt] = the_decl;
    }
    else {

      // -- Found it

      the_decl = (*p).second;
    }
  }

  // -- Look up the memoryBlock for this allocation site based on the
  // location and the given declaration. Ignore the context for regular
  // mallocs, if the flag indicates to do so. VERY IMPORTANT: we need to
  // pass from_heap_map = true which tells the memory model that this
  // declNode is synthetic, otherwise the adaptive algorithm can't set heap
  // objects flow sensitive.

  memoryBlock * block = Memory.lookup_heap_object(the_decl->name(), current, allocation_stmt,
						  the_decl, from_heap_map);

  // -- If the block is flow-sensitive and we are actually computing the
  // pointer info...

  if (computePointers &&
      block->is_flow_sensitive()) {

    // -- Perform multiplicity analysis: this information is associated
    // with a separate allocation object.

    memoryBlock * alloc_object = block->get_allocation_object(Memory);

    // -- Looking up its nearest reaching definition of the alloc object

    memoryDef * previous_alloc_def = nearest_def_at(info, alloc_object, current);

    // -- Compute the new multiplicity

    Multiplicity new_multiplicity;

    new_multiplicity = alloc_object->at_allocation(current,
						   previous_alloc_def,
						   defs, uses, changes);
  }

  // -- Return the allocated block

  result.blocks.insert(block);  
  result.is_address = true;

  return block;
}

void Pointers::at_deallocation(procedureInfo * info,
			       Location * current,
			       pointerValue & to_deallocate,
			       memoryblock_set & defs,
			       memoryuse_set & uses,
			       memoryblock_set & changes)
{
  // -- For each block that need to be deallocated...

  for (memoryblock_set_p p = to_deallocate.blocks.begin();
       p != to_deallocate.blocks.end();
       ++p)
    {
      memoryBlock * block = *p;
      bool changed = false;

      if (block->write_protected()) {

	if (pointerOptions::Verbose)
	  cout << "  Skip deallocation of write-protected object: " << block->name() << endl;
      }
      else {
	
	// -- Perform multiplicity analysis: check for a reaching definition of this
	// object, and decrease the multiplicity, if we can.

	// -- If we are actually computing the pointer info...

	if (computePointers &&
	    block->is_flow_sensitive()) {

	  // -- Perform multiplicity analysis: this information is associated
	  // with a separate allocation object.

	  memoryBlock * alloc_object = block->get_allocation_object(Memory);

	  // -- Looking up its nearest reaching definition of the alloc object

	  memoryDef * previous_alloc_def = nearest_def_at(info, alloc_object, current);

	  // -- Compute the new multiplicity

	  Multiplicity new_multiplicity;

	  new_multiplicity = alloc_object->at_deallocation(current,
							   previous_alloc_def,
							   defs, uses, changes);
	}
      }
    }
}

Multiplicity Pointers::current_multiplicity(procedureInfo * info,
					    Location * current,
					    memoryBlock * block_in,
					    memoryuse_set & uses)
{
  Multiplicity multiplicity = Unallocated;

  // -- Get the top-level block

  memoryBlock * block = block_in;
  while (block->container())
    block = block->container();

  // -- Perform multiplicity analysis

  if (block->is_indexed())
    multiplicity = Bounded;
  else
    if (block->is_heap_object()) {

      // -- Multiplicity analysis only applies to flow sensitive heap
      // objects, and only when this option is turned on.

      if (block->is_flow_sensitive() &&
	  _use_multiplicity)
	{
	  // -- Get the special allocation object

	  memoryBlock * alloc_object = block->get_allocation_object(Memory);

	  // -- Find it's reaching def

	  memoryDef * alloc_def = nearest_def_at(info, alloc_object, current);

	  // -- Reading the multiplicity is a use of the alloc object

	  memoryUse * use = alloc_object->use_at(current);
	  use->reaching_def(alloc_def);
	  uses.insert(use);
	
	  // -- Get the current multiplicity value

	  if (alloc_def)
	    multiplicity = alloc_def->multiplicity();
	}
      else {

	// -- Flow insensitive block, or multiplicity is turned off:
	// default to Unbounded

	multiplicity = Unbounded;
      }
    }
    else {

      // -- Regular variables are always "single"

      multiplicity = Single;
    }

  return multiplicity;
}


bool Pointers::is_allocation(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if ((decl->name() == string("malloc")) ||
	(decl->name() == string("calloc")))
      return true;
  }

  return false;
}

bool Pointers::is_deallocation(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if ((decl->name() == string("free")) ||
	(decl->name() == string("cfree")))
      return true;
  }

  return false;
}

bool Pointers::is_reallocation(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if (decl->name() == string("realloc"))
      return true;
  }

  return false;
}

/** @brief Mark as indexed
 *
 * Mark all of the given objects as indexed, meaning that they conceptually
 * represent many objects (as in an array). This is necessary when we
 * discover that an object is being treated like an array (for example, the
 * index operator is applied, or pointer arithmetic is used to access
 * it). */

void Pointers::mark_as_indexed(memoryblock_set & blocks)
{
  for (memoryblock_set_p p = blocks.begin();
       p != blocks.end();
       ++p)
    {
      memoryBlock * block = *p;
      block->set_indexed();
    }
}

// ------------------------------------------------------------
// Misc
// ------------------------------------------------------------

// -- Print some statistics

void Pointers::stats(ostream & out)
{
  out << "Pointer analysis:" << endl;
  out << "  Number of procedure-level passes: " << _procedureCount << endl;

  Memory.stats(out);

  out << "Unknown procedures:" << endl;
  for (string_set_p p = _unknown_procedures.begin();
       p != _unknown_procedures.end();
       ++p)
    out << "  " << (*p) << endl;

  Procedures.stats(out);
}

void Pointers::uses_and_defs()
{
  Memory.print(cout);
}

void Pointers::print_memoryblock_set(const string & label,
				     const memoryblock_set & the_set,
				     ostream & o)
{
  cout << label;
  for (memoryblock_set_cp mbp = the_set.begin();
       mbp != the_set.end();
       ++mbp)
    {
      if (mbp != the_set.begin())
	cout << " , ";
      cout << (*mbp)->name();
    }
  cout << endl;
}

void Pointers::print_memorydef_set(const string & label,
				   const memorydef_set & the_set,
				   ostream & o)
{
  cout << label << endl;
  for (memorydef_set_cp p = the_set.begin();
       p != the_set.end();
       ++p)
    {
      memoryBlock * owner = (*p)->owner();
      cout << "        " << owner->name() << " (def at " << * (*p)->where() << ")" << endl;
    }
  cout << endl;
}

void Pointers::print_memoryblock_def_set(const string & label,
					 const memoryblock_set & the_set,
					 ostream & o)
{
  cout << label << endl;
  for (memoryblock_set_cp p = the_set.begin();
       p != the_set.end();
       ++p)
    {
      memoryBlock * owner = (*p);
      memoryDef * def = owner->current_def();

      if (def)
	cout << "        " << owner->name() << " (def at " << * def->where() << ")" << endl;
      else
	cout << "        " << owner->name() << " (Non-local def)" << endl;	
    }
  cout << endl;
}

// ----------------------------------------------------------------------
//   Public methods
// ----------------------------------------------------------------------

// -- Lookup a procedure

procedureInfo * Pointers::lookup_procedure(procNode * proc)
{
  return Procedures.lookup(proc);
}


bool Pointers::is_pointer_expression(exprNode * expr)
{
  unsigned int oper;

  switch (expr->typ()) {

  case Unary:
    {
      unaryNode * u = (unaryNode *) expr;
      oper = u->op()->id();

      if ((oper == Operator::ADDRESS) ||
	  (oper == Operator::INDIR))
	return true;
    }
    break;

  case Binary:
    {
    binaryNode * b = (binaryNode *) expr;
    oper = b->op()->id();

    if ((oper == '.') ||
	(oper == Operator::ARROW))
      return true;
    }
    break;
  }

  return false;
}

// ----------------------------------------------------------------------
//  Exit/abort calls
// ----------------------------------------------------------------------

bool Pointers::is_exit(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if ((decl->name() == string("exit")) ||
	(decl->name() == string("_exit")) ||
	(decl->name() == string("abort")))
      return true;
  }

  return false;
}


// ----------------------------------------------------------------------
//  Var args
// ----------------------------------------------------------------------

void Pointers::setup_va_list_variables(procedureInfo * info,
				       procLocation * callee_path, 
				       stmtLocation * parameter_callsite,
				       procNode * callee,
				       memoryBlock * ellipsis)
{
  memoryblock_set defs;
  memoryuse_set uses;
  memoryblock_set changes;
  pointerValue rhs(ellipsis);
  rhs.is_address = true;

  decl_list & local_vars = callee->body()->decls();
  for (decl_list_p lvp = local_vars.begin();
       lvp != local_vars.end();
       ++lvp)
    {
      declNode * cur_lv = *lvp;

      // -- Look for variables whose type is typedef va_list

      if (is_va_list(cur_lv)) {

	// -- Found one, perform the assignment like below

	memoryBlock * left = Memory.lookup_variable(callee_path, cur_lv, callee);
	pointerValue lhs(left);

	assignment_operator(info, callee_path, parameter_callsite, lhs, rhs, defs, uses, changes);

	// -- Here's a trick: we'll prevent these pointers from
	// being overwritten by marking the va_list variable with
	// the "numerous" flag.
	  
	left->set_indexed();

	// -- Constant propagation

	_constants.at_parameter_pass(callee_path, lhs, rhs, changes);

	// -- User-defined analysis
	
	if (Problem)
	  Problem->at_parameter_pass(callee_path, parameter_callsite, lhs, rhs, changes);
      }
    }
}

// Right now, this stuff is Linux-specific

bool Pointers::is_va_arg(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if (decl->name() == "__builtin_va_arg")
      return true;
  }

  return false;
}

bool Pointers::is_va_start(pointerValue & call_targets)
{
  if (call_targets.blocks.size() == 1) {
    memoryBlock * call = * call_targets.blocks.begin();
    declNode * decl = call->decl();
    if (decl->name() == "__builtin_next_arg")
      return true;
  }

  return false;
}

bool Pointers::is_va_end(pointerValue & call_targets)
{
  return false;
}

bool Pointers::is_va_list(declNode * decl)
{
  // -- Look for variables whose type is typedef va_list

  if (decl->type() && (decl->type()->typ() == Tdef)) {
	tdefNode * tdef = (tdefNode *) decl->type();
	if (tdef->name() == "va_list")
	  return true;
	if (tdef->name() == "__builtin_va_alist_t")
	  return true;
	if (tdef->name() == "__gnuc_va_list")
	  return true;
  }

  return false;
}

bool Pointers::is_va_list(exprNode * expr)
{
  if (expr->typ() == Id) {
    idNode * id = (idNode *) expr;
    if (id->decl())
      return is_va_list(id->decl());
  }

  return false;
}

void Pointers::progress_meter(procedureInfo * info, bool skipped)
{
  if (skipped)
    _skipCount++;

  _procedureCount++;

  bool show_now = false;

  if (_procedureCount < 50000)
    show_now = ((_procedureCount % 1000) == 0);
  else
    show_now = ((_procedureCount % 10000) == 0);

  if (_show_progress && show_now) {
    if (info->is_context_insensitive())
      cout << "Progress (I): ";
    else
      cout << "Progress (S): ";

    cout << _procedureCount << "/" << _skipCount << " at ";
    Procedures.progress_meter(cout);
    cout << endl;
  }
}

void Pointers::show_accuracy()
{
  double defs = compute_accuracy(_def_accuracy);

  cout << "STAT-accuracy-pointers-defs " << defs << endl;

  double star = compute_accuracy(_star_accuracy);

  cout << "STAT-accuracy-pointers-star " << star << endl;
  
  // -- Clear information

  _def_accuracy.clear();
  _star_accuracy.clear();
}

double Pointers::compute_accuracy(accuracy_map & accuracy_data)
{
  // -- Compute accuracy

  typedef map< stmtNode *, count_accuracy_pair > stmt_accuracy_map;
  typedef stmt_accuracy_map::iterator stmt_accuracy_map_p;

  stmt_accuracy_map by_statement;

  for (accuracy_map_p p = accuracy_data.begin();
       p != accuracy_data.end();
       ++p)
    {
      stmtLocation * where = (*p).first;
      count_accuracy_pair & cap = (*p).second;

      // -- Count the number of ways to reach this location.

      procLocation * cur = Location::procedure(where);

      // -- Look up the procedure

      procedureInfo * info = lookup_procedure(cur->proc());

      // -- Now get the context count

      int count = info->count_calling_contexts();

      // -- Construct accuracy information that takes into account the
      // number of contexts.

      count_accuracy_pair new_cap;

      new_cap.first  = cap.first * count;
      new_cap.second = cap.second * ((double)count);

      // -- Get the statement itself

      stmtNode * stmt = where->stmt();

      // -- Add the values in to the total

      stmt_accuracy_map_p w = by_statement.find(stmt);
      if (w == by_statement.end())
	by_statement[stmt] = new_cap;
      else {
	count_accuracy_pair & existing_cap = (*w).second;

	existing_cap.first  += new_cap.first;
	existing_cap.second += new_cap.second;
      }
    }

  // -- Print out the results, according to the call site and compute
  // aggregate accuracy

  double total = 0.0;
  int count = 0;

  for (stmt_accuracy_map_p p = by_statement.begin();
	 p != by_statement.end();
       ++p)
    {
      stmtNode * stmt = (*p).first;
      count_accuracy_pair & cap = (*p).second;

      double local_accuracy = (cap.second) / ((double) cap.first);

      // cout << "ACCURACY: Pointers at " << stmt->coord() << " = " << local_accuracy << endl;

      count++;
      total += local_accuracy;
    }

  return (total / ((double)count));
}
