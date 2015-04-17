// $Id: pointers.h,v 1.37 2003/09/29 13:01:33 toktb Exp $
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

#ifndef CBZ_POINTERS_H
#define CBZ_POINTERS_H

#include "pointeroptions.h"
#include "location.h"
#include "memorymodel.h"
#include "memoryblock.h"
#include "proceduredb.h"
#include "linker.h"
#include "ipanalysis.h"
#include "pointervalue.h"
#include "constants.h"
#include "unification.h" // TB_unify

typedef map< stmtNode *, declNode *> heap_map;
typedef heap_map::iterator heap_map_p;

typedef list< memoryblock_set > memoryblock_set_list;
typedef memoryblock_set_list::iterator memoryblock_set_list_p;

/** @brief Pointer analyzer
 *
 *  This class is the main pointer analyzer engine. */

class Pointers
{
public:

  typedef set< string > string_set;
  typedef string_set::iterator string_set_p;

  typedef pair< int, double > count_accuracy_pair;
  typedef map< stmtLocation *, count_accuracy_pair > accuracy_map;
  typedef accuracy_map::iterator accuracy_map_p;

protected:

  /** @brief Memory model
   *
   *  This object holds all of the memoryBlocks, and is responsible for
   *  their creation, indexing and deletion. */

  memoryModel Memory;

  /** @brief Heap decl map
   *
   *  Our pointer analysis keeps one heap object for each allocation
   *  site. This field maintains the mapping from allocation sites to
   *  synthetic declarations for those heap objects. */

  heap_map HeapMap;

  /** @brief Analysis starting point */

  procNode * Root;

  /** @brief Starting point in the location representation */
  
  procLocation * root_location;

  /** @brief Linker
   *
   *  The linker object is used to connect external declaration across
   *  translation units. We need this, for example, so that accesses to a
   *  global variable in different source files are properly connected.
   *
   *  NOTE: the adaptive algorithm needs the linker to be the same for all
   *  iterations, that way the synthetic declNodes stay the same. */

  Linker & linker;

  /** @brief Procedure database
   *
   *  This object holds per-procedure information used during analysis. In
   *  particular, each entry holds the worklist for that procedure. */

  procedureDB Procedures;

  /** @brief Constants
   *
   * Constant propagation is now built in. The most significant aspect of
   * this change is that memoryDef and pointerValue contain an explicit
   * pointer for holding the constant value. */

  constantAnalyzer _constants;

  /** @brief Use-defined analysis problem */

  analysisProblem * Problem;

  /** @brief Analysis mode switch
   *
   * This boolean is true during pointer analysis, which allows new defs
   * and uses to be created. Once complete, the boolean is set false so
   * that other analyses can call pointer methods without changing the
   * dependence information. */

  bool computePointers;

  /** @brief Analysis direction
   *
   * During pointer analysis, this is always set to "Forward". Otherwise,
   * it is determined by the corresponding value on the analysisProblem
   * object. */

  Direction _direction;

  /** @brief Progress counter */

  int _procedureCount;

  /** @brief Number of procedures skipped */

  int _skipCount;

  /** @brief Show progress counter */

  bool _show_progress;

  /** @brief Use multiplicity analysis */

  bool _use_multiplicity;

  /** @brief Unknown procedures */

  string_set _unknown_procedures;

  /** @brief Accuracy at defs
   *
   * Store the size of the points-to set at each def. */

  accuracy_map _def_accuracy;

  /** @brief Accuracy at star
   *
   * Store the size of the points-to set at every star operator. */

  accuracy_map _star_accuracy;

  /** @brief Unification-based analyzer */
  UnificationBasedPtr *_unification_based;

public:

  /** @brief Constructor */

  Pointers(procNode * root, Linker & l, unitNode * unit);

  /** @brief Destructor */

  virtual ~Pointers();

  inline Linker & getLinker() const { return linker; }

  /** @brief Create and run unification, if in unification mode. */
  virtual void run_unification();

  /** @brief Get the unification analyzer */
  inline UnificationBasedPtr *unification_analyzer() const
  { return _unification_based; }

  inline void use_unification_analyzer(UnificationBasedPtr *u)
  { Memory.unification(_unification_based = u); }

  /** @brief Run the analyzer */

  void analyze();
  void analyze(analysisProblem * problem);

  /** @brief Initialize
   *
   * Override this method in subclasses to do some initialization. The
   * Broadway analyzer needs this to initialize some of the analyses. */

  virtual void initialize() {}

  /** @brief Constants
   *
   * Return a reference to the constant propagation information. Be careful
   * with this! */

  inline constantAnalyzer & get_constants() { return _constants; }

  /** @brief Clear pointer information after a program change */

  void clear();

  /** @brief Turn on progress meter */

  void show_progress() { _show_progress = true; }

  /** @brief Turn off progress meter */

  void hide_progress() { _show_progress = false; }

  /** @brief Turn on multiplicity analysis */

  void turn_on_multiplicity() { _use_multiplicity = true; }

  /** @brief Turn off multiplicity analysis */

  void turn_off_multiplicity() { _use_multiplicity = false; }

  /** @brief Analyze one procedure */

  void analyze_procedure(procLocation * current,
			 memoryblock_set & defs,
			 memoryuse_set & uses,
			 memoryblock_set & changes);

  /** @brief Process a procedure call */

  virtual void procedure_call(procedureInfo * caller,
			      stmtLocation * current,
			      operandNode * call,
            operand_list & args,
			      pointerValue & call_target,
			      memoryBlock * target_mb,
			      pointervalue_list & arguments,
			      memoryblock_set & external_defs,
			      memoryuse_set & external_uses,
			      memoryblock_set & external_changes,
			      pointerValue & return_val,
			      bool & never_returns);

  /** @brief Analyze a procedure at a call
   *
   * This method provides the interface between procedure_call() and
   * analyze(). It handles all of the parameter passing, as well as context
   * sensitivity. */

  void analyze_procedure_at(procedureInfo * callee,
			    stmtLocation * callsite,
			    pointervalue_list & arguments,
			    memoryblock_set & external_defs,
			    memoryuse_set & external_uses,
			    memoryblock_set & external_changes,
			    pointerValue & return_val);

  /** @brief Pass parameters */

  void pass_parameters(procedureInfo * info,
		       stmtLocation * callsite,
		       pointervalue_list & arguments,
		       memoryblock_set & parameters,
		       memoryblock_set & changed_inputs);

  /** @brief Pass return value
   *
   * This method works almost exactly like the pass_external_outputs()
   * method. */

  void pass_return_value(procedureInfo * info,
			 stmtLocation * current_callsite,
			 pointerValue & return_val,
			 memoryblock_set & changes);

  /** @brief Process a merge point */

  void process_merge_point(procedureInfo * info,
			   basicblockLocation * current_block,
			   pointerValue & result,
			   memoryblock_set & defs,
			   memoryuse_set & uses,
			   memoryblock_set & changes);

  /** @brief Process local changes
   *
   * After analyzing a particular basic block, we process the blocks that
   * have changed by inserting merge-points for those blocks. */

  void process_local_changes(procedureInfo * info,
			     basicblockLocation * current_block,
			     memoryblock_set & local_changes,
			     memoryblock_set & changes);

  /** @brief Process external inputs
   *
   * In context insensitive mode, this method visits all the external
   * inputs from the given procedure. For each one, it merges together the
   * reaching defs from all the calling contexts, and produces a single def
   * that reaches the uses inside the procedure.
   *
   * Each use is attached to the special procLocation at that calling
   * context. The merged def itself is attach to basic block zero in the
   * actual procedure.
   *
   * Record any inputs that change so that we can decide whether to skip
   * analyzing the procedure. */

  void pass_external_inputs(procedureInfo * info,
			    stmtLocation * current_callsite,
			    memoryuse_set & external_uses,
			    memoryblock_set & changed_inputs);

  /** @brief Pass one external input
   *
   * This method is used right before entering a procedure to make sure
   * that external inputs are fresh, and also during analysis when we
   * discover a new external input during the Pointers::nearest_def_at()
   * method. */

  void pass_one_external_input(procedureInfo * callee,
			       procedureInfo * caller,
			       stmtLocation * current_callsite,
			       memoryBlock * block_to_pass,
			       memoryblock_set & changed_inputs);

  /** @brief Process external outputs
   *
   * In context insensitive mode, this method visits all the external
   * outputs from the given procedure. For each one, it propagates the last
   * value to all the call sites. */

  void pass_external_outputs(procedureInfo * info,
			     stmtLocation * current_callsite);

  /** @brief Pass one external output
   *
   * Called by the above routine and by the pass_return_value() routine. It
   * propagates the last value of the given memoryBlock from the callee
   * procedure back to all of the places it's called. It does this by
   * creating a use at the last statement of the procedure that holds the
   * last reaching def in the procedure. It then assigns this use to a def
   * at each target callsite. That way, the def will contain the right
   * value, and dominate any uses of the object that follow. If any changes
   * occur, they are stored in the pending_changes set of the caller. */

  void pass_one_external_output(procedureInfo * info,
				stmtLocation * current_callsite,
				memoryBlock * block_to_pass,
				bool is_return_val);

  /** @brief Record external inputs and outputs
   *
   * After a procedure has been analyzed, go through the uses and defs and
   * determine which are external. That is, which uses are reached by defs
   * outside the procedure, and which defs could reach uses outside the
   * procedure. */

  void record_external_inputs_and_outputs(procedureInfo * callee,
					  procLocation * current,
                                          pointervalue_list & arguments, //TB
					  memoryblock_set & parameters,
					  memoryblock_set & defs,
					  memoryblock_set & external_defs,
					  memoryuse_set & uses,
					  memoryuse_set & external_uses,
					  memoryblock_set & changes,
					  memoryblock_set & external_changes);

  /** @brief Conservative procedure call
   *
   * This function is called when we encounter a procedure with no source
   * code. It conservatively creates uses and defs for all blocks reachable
   * from the input arguments. */

  virtual void conservative_procedure_call(procedureInfo * info,
					   stmtLocation * current,
					   pointervalue_list & arguments,
					   memoryblock_set & reachable,
					   memoryblock_set & external_defs,
					   memoryuse_set & external_uses,
					   memoryblock_set & external_changes,
					   pointerValue & return_val);

  /** @brief Expression evaluator */

  void eval(procedureInfo * info,
	    stmtLocation * current,
	    exprNode * E,
            stmtNode *cur_stmt,
	    memoryblock_set & defs,
	    memoryuse_set & uses,
	    memoryblock_set & changes,
	    pointerValue & result,
	    bool & never_returns);

  // TB: new.
  void eval(procedureInfo * info,
	    stmtLocation * current,
	    threeAddrNode *threeAddr,
	    memoryblock_set & defs,
	    memoryuse_set & uses,
	    memoryblock_set & changes,
	    pointerValue & result,
	    bool & never_returns);

  /** @brief Star "*" operator */

  void star_operator(procedureInfo * info,
		     Location * current,
		     pointerValue & operand,
		     memoryblock_set & defs,
		     memoryuse_set & uses,
		     memoryblock_set & changes,
		     pointerValue & result);

  /** @brief Dot "." operator */

  void dot_operator(Location * current,
		    const string & field_name,
		    declNode * field_decl,
		    pointerValue & operand,
		    memoryuse_set & uses,
		    pointerValue & result);

  /** @brief Struct/union assignment
   *
   * Direct assignments between structs and unions are allowed, so here we
   * recursively descend through the fields assigning them in a pairwise
   * fashion. NOTE: Here we are relying on having the correct types. */

  void struct_union_assignment(procedureInfo * info,
			       stmtLocation * current,
			       threeAddrNode *threeaddr,
			       sueNode * suetype,
			       pointerValue & left_hand_side,
			       pointerValue & right_hand_side,
			       memoryblock_set & defs,
			       memoryuse_set & uses,
			       memoryblock_set & changes);

  /** @brief Assignment "=" operator
   *
   * When the additive flag is set, the assignment will include the
   * previous value of the pointer at this location. This allows us to say
   * things like "p = &x and p = &y" without having to build a single
   * pointerValue that contains both x and y.
   * */

  virtual void assignment_operator(procedureInfo * info,
				   Location * current,
				   stmtLocation * parameter_callsite,
				   pointerValue & left_hand_side,
				   pointerValue & right_hand_side,
				   memoryblock_set & defs,
				   memoryuse_set & uses,
				   memoryblock_set & changes);

  /** @brief Process a control-flow merge
   *
   * */

  void merge_operator(procedureInfo * info,
		      Location * current,
		      memoryBlock * block_to_merge,
		      memoryuse_list & phi_uses,
		      memoryblock_set & defs,
		      memoryuse_set & uses,
		      memoryblock_set & changes);

  /** @brief Self-assignment
   *
   * This method is used to pass an external input or output. It works like
   * a hybrid of the assignment operator and the merge operator, except
   * that it allows the use to be at a different location from the def. */

  void self_assignment(Location * from,
		       Location * to,
		       memoryBlock * block,
		       memoryblock_set & changes);

  /** @brief Function call operator
   *
   * Evaluate the actual parameters, determine the call target and then call procedure_call(). */

  void call_operator(procedureInfo * caller,
		     stmtLocation * current,
		     operandNode * call,
                     operand_list & args,
		     memoryblock_set & defs,
		     memoryuse_set & uses,
		     memoryblock_set & changes,
		     pointerValue & result,
                     pointerValue & call_targets,
		     bool & never_returns);

  /** @brief Determine call targets
   *
   * Evaluate the callNode subexpression that indicates which procedure
   * we're calling. In most cases, this is just a single
   * identifier. However, for calls through a function pointer, we need to
   * evaluate the pointer expression to determine the target or targets. */
  
  void determine_call_targets(procedureInfo * caller,
			      stmtLocation * current,
			      operandNode * call,
			      memoryblock_set & defs,
			      memoryuse_set & uses,
			      memoryblock_set & changes,
			      pointerValue & targets);

  /** @brief Generate memoryUses
   *
   *  For each block in the set, generate a use at the given location. This
   *  also sets the "current_use" field on each memoryBlock, so that it can
   *  be accessed during other computations. */

  void generate_uses(procedureInfo * info,
		     Location * where,
		     memoryuse_set & uses,
		     pointerValue & pointer);

  /** @brief Reaching def
   *
   * This is a special version of "nearest dominating def" that works
   * across context insensitive boundaries. It handles the case where we
   * discover an "input" to the current context that wasn't in previous
   * analyses of the procedure. */

  memoryDef * nearest_def_at(procedureInfo * info,
			     memoryBlock * block,
			     Location * where);

  /** @brief Reachable blocks
   *
   * Given a set of memoryBlocks, compute the set of all blocks reachable
   * by following pointer or structure-field relationships. */

  void reachable_blocks(Location * where,
			memoryblock_list & worklist,
			memoryblock_set & already_seen);

  /** @brief Lookup a procedure */

  procedureInfo * lookup_procedure(procNode * proc);

  /** @brief Return true if the expression involves pointers */

  bool is_pointer_expression(operandNode * operand);

  /** @brief Print some statistics */

  void stats(ostream & out);

  /** @brief Print all uses and defs */

  void uses_and_defs();

  /** @brief Compute accuracy */

  void show_accuracy();

protected:

  // -- Manage the heap

  /** @brief At allocation site
   *
   * Process a memory allocation. This method performs two important tasks:
   * (1) it creates a representation of the heap object, and (2) it manages
   * the multiplicity analysis. Allocation is considered a def of the newly
   * allocated space.  */

  memoryBlock * at_allocation(const string & name,
			      procedureInfo * info,
			      stmtLocation * current,
			      stmtNode * allocation_stmt,
			      declNode * decl,
			      memoryblock_set & defs,
			      memoryuse_set & uses,
			      memoryblock_set & changes,
			      pointerValue & result);

  /** @brief At deallocation site
   *
   * Process a deallocation. The main job of this method is to handle the
   * multiplicity analysis. Deallocation bumps the multiplicity down for
   * objects that are in the Single state. */

  void at_deallocation(procedureInfo * info,
		       Location * current,
		       pointerValue & to_deallocate,
		       memoryblock_set & defs,
		       memoryuse_set & uses,
		       memoryblock_set & changes);

  /** @brief Current multiplicity
   *
   * Get the allocation object for the given memoryBlock and use it to
   * determine the current multiplicity value. */

  Multiplicity current_multiplicity(procedureInfo * info,
				    Location * current,
				    memoryBlock * block,
				    memoryuse_set & uses);

  // memoryBlock * lookup_heap_object(stmtLocation * current);

  static bool is_allocation(pointerValue & call_targets);

  static bool is_deallocation(pointerValue & call_targets);

  static bool is_reallocation(pointerValue & call_targets);

  /** @brief Mark as indexed
   *
   * Mark all of the given objects as indexed, meaning that they
   * conceptually represent many objects (as in an array). This is
   * necessary when we discover that an object is being treated like an
   * array (for example, the index operator is applied, or pointer
   * arithmetic is used to access it). */

  void mark_as_indexed(memoryblock_set & blocks);

  /** @brief Var args */

  void setup_va_list_variables(procedureInfo * info,
			       procLocation * callee_path, 
			       stmtLocation * parameter_callsite,
			       procNode * callee,
			       memoryBlock * ellipsis);

  static bool is_va_list(declNode * decl);
  static bool is_va_list(exprNode * expr);
  static bool is_va_arg(pointerValue & call_targets);
  static bool is_va_start(pointerValue & call_targets);
  static bool is_va_end(pointerValue & call_targets);

  /** @brief Exit procedures
   *
   * Return true if the given call if to one of the system calls that never
   * returns, such as exit() or abort(). */

  static bool is_exit(pointerValue & call_targets);

  /** @brief Useful utilities */

public:

  static void merge(const memoryblock_set & source,
		    memoryblock_set & dest);

  static void print_memoryblock_set(const string & label,
				    const memoryblock_set & the_set,
				    ostream & o);
  
  static void print_memoryblock_def_set(const string & label,
					const memoryblock_set & the_set,
					ostream & o);
  
  static void print_memorydef_set(const string & label,
					const memorydef_set & the_set,
					ostream & o);

  /** @brief Progress meter */

  void progress_meter(procedureInfo * info, bool skipped);

  /** @brief Counter for call invocations */

  static long int call_counter;

  /** @brief Compute accuracy */

  double compute_accuracy(accuracy_map & accuracy_data);

  /** @brief Return all the procedures */
  inline proc_info_map procedures() const { return Procedures.procedures(); }
};

#endif // CBZ_POINTERS_H
