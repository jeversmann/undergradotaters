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

#ifndef CBZ_PROCEDUREDB_H
#define CBZ_PROCEDUREDB_H

class memoryBlock;

#include "pointeroptions.h"
#include "location.h"
#include "dfpreds.h"
#include "memoryaccess.h"
#include "worklist.h"
#include "linker.h"
#include "loops.h"
#include "callgraph.h"
#include "cbztimer.h"

class procedureInfo;

typedef set< procedureInfo * > procedureinfo_set;
typedef procedureinfo_set::iterator procedureinfo_set_p;
typedef procedureinfo_set::const_iterator procedureinfo_set_cp;

typedef pair< basicblockNode *, basicblockNode *> cfg_edge_pair;
typedef set< cfg_edge_pair > cfg_edge_set;
typedef cfg_edge_set::iterator cfg_edge_set_p;

/** @brief Procedure information
 *
 *  This class holds information about a single procedure definition, including:
 *
 *  _unit :  the unit in which it is defined
 *  _worklist_order : the best ordering of its basic blocks for the worklist
 *  _dominance_frontiers : the precomputed dominance frontiers
 *  _callsites : the set of places this procedure is called
 *
 * At the start of analysis, only the first two fields are filled. When a
 * procedure is actually encountered during analysis, then the worklist
 * order and dominance frontiers are computed. */

class procedureInfo
{
public:

  typedef map< basicblockNode *, int > worklist_order_map;
  typedef worklist_order_map::iterator worklist_order_map_p;

  // -- This is a map from callsites to the caller

  typedef map< stmtLocation *, procedureInfo *> callsite_map;
  typedef callsite_map::iterator callsite_map_p;
  typedef callsite_map::const_iterator callsite_map_cp;

  typedef map< basicblockLocation *, memoryblock_set > mergepoint_map;
  typedef mergepoint_map::iterator mergepoint_map_p;

  // -- Pending changes

  typedef map< stmtLocation *, memoryblock_set> callsite_changes_map;
  typedef callsite_changes_map::iterator callsite_changes_map_p;

  // -- For use in counting contexts:

  typedef map< procedureInfo *, int > proc_int_map;
  typedef proc_int_map::iterator proc_int_map_p;

private:

  /** @brief A pointer to the procedure definition */

  procNode * _proc;

  /** @brief The worklist of basic blocks */

  workList _worklist;

  /** @brief The forward ordering of basic blocks in the worklist
   *
   *  We precompute this ordering to favor quick convergence of dataflow
   *  problems. It's just a depth-first ordering. */

  worklist_order_map _forward_worklist_order;

  /** @brief The forward enumeration of basic blocks
   *
   *  This maps basic blocks to bit positions in the worklist. */

  vector< basicblockNode * > _forward_basicblock_order;

  /** @brief The backward ordering of basic blocks in the worklist
   *
   *  We precompute this ordering to favor quick convergence of dataflow
   *  problems. It's just a depth-first ordering. */

  worklist_order_map _backward_worklist_order;

  /** @brief The backward enumeration of basic blocks
   *
   *  This maps basic blocks to bit positions in the worklist. */

  vector< basicblockNode * > _backward_basicblock_order;

  /** @brief Merge points 
   *
   *  These are the phi functions for this procedure. Note that since a
   *  procedure may be called in many different contexts, we need to
   *  separate the merge points out. */

  mergepoint_map _merge_points;

  /** @brief Dominance frontiers
   *
   *  This is a special version of dominance frontiers. For each node X in
   *  the dominance frontier of some node Y, it also provide the particular
   *  immediate predecessor of Y that is dominated by X. */

  DFPreds * _dominance_frontiers;

  /** @brief Loop tree
   *
   * This object contains all the information we need about the loops in
   * this procedure. In addition, it provides a classification of all edges
   * in the control-flow graph. */

  loopTree * _loops;

  /** @brief Call sites
   *
   *  This set holds all of the statements that are determined to be calls
   *  to this procedure. The map is from callsites to the procedure making
   *  the call (the context). */

  callsite_map _callsites;

  /** @brief Return value variable
   *
   * Store the special return value variable so that we can easily get it
   * when we need to retrieve the return value. */

  declNode * _return_variable;

  /** @brief Return statement
   *
   * Store a pointer to the return statement (there should only be one!) */

  returnNode * _return_stmt;

  /** @brief Never returns
   *
   * When this flag is set true it indicates that the given procedure never
   * returns. This can happen if a procedure always calls exit(), for
   * example. We'll use this information to prune the control-flow
   * graph. */

  bool _never_returns;

  /** @brief Pending changes
   *
   * When this procedure calls a procedure that is context-insensitive, it
   * is possible that other callsites will produce changes that are
   * propagated here. For example:
   *
   * foo() { bar(); }
   * baz() { bar(); }
   *
   * Assume bar() is context-insensitive. While analyzing bar() in the
   * baz() context, we may need to propagate changes to the foo()
   * callsite. The reason that this isn't a problem in the baz() context is
   * that we actually pass the changes back to baz() as the
   * external_changes set.
   *
   * The solution is to store the changes produced by passing the external
   * output in this map, and then pick them up when we actually revisit the
   * context. */

  callsite_changes_map _pending_changes;

  /** @brief Current call site
   *
   * Set by the call() method.

  stmtLocation * _current_callsite; */

  /** @brief Context insensitive
   *
   * Set to true if this procedure is analyzed context insensitively. */

  bool _context_insensitive;

  /** @brief Prefer context sensitive
   *
   * This flag is set at the beginning of the analysis to indicate that the
   * preferred mode for this procedure is context sensitive. We only check
   * this flag during context insensitive analysis. */

  bool _prefer_context_sensitive;

  /** @brief Only context
   *
   * For context insensitive procedures, we build a separate location tree
   * that holds all of the context insensitive uses and defs.
   *
   * Note that since this location is removed from the location tree, this
   * object actually owns it. */

  TREE procLocation * _only_context;

  /** @brief External inputs
   *
   * When a procedure is analyzed without context sensitivity, we need to
   * record those objects that enter the procedure from outside. We can
   * think of these objects as inputs to the procedure that are not
   * represented in the parameter list (e.g, a global variable). Since each
   * context could have a different reaching def, we need to merge together
   * those reaching defs before entering the procedure. This works a bit
   * like a merge-point. */

  memoryblock_set _external_inputs;

  /** @brief External outputs
   *
   * Like the external inputs, when we're using context insensitive mode,
   * defs inside the procedure can reach outside the procedure. We can
   * think of these objects as outputs from the procedure. In order to make
   * sure that those defs reach the right uses, we create a def at the
   * callsite with a use whose reaching definition is inside the
   * procedure. */

  memoryblock_set _external_outputs;

  /** @brief Ancestors
   *
   * Maintain a set of ancestors in the call graph. This makes searching
   * the scope and updating the worklist faster. */

  procedureinfo_set _ancestors;

  /** @brief First caller
   *
   * Creates a minimum spanning tree of the call graph. We use this to mark
   * for reanalysis. */

  procedureInfo * _first_caller;

  /** @brief Called procedures
   *
   * The procedures directly called by this one. */

  procedureinfo_set _calls;

  /** @brief Called recursively
   *
   * Record if this procedure is recursive. */

  bool _is_recursive;

  /** @brief Blocks to skip
   *
   * In conditional mode, this tells us which blocks are currently being
   * skipped because they are on branches that are not taken. We need this
   * list in order to set up the merge points properly. */

  basicblock_set _blocks_to_skip;

  /** @brief Active edges
   *
   * */

  cfg_edge_set _active_edges;

  /** @brief True branches
   *
   * This sucks: keep a mapping from conditionals to the true
   * branches. Apparently there is no convention in the dismantler/cfg. */

  typedef map< basicblockNode *, basicblockNode * > true_branch_map;
  typedef true_branch_map::iterator true_branch_map_p;

  true_branch_map _true_branches;

  /** @brief Analysis count
   *
   * Count the number of times the procedure is analyzed. */

  int _analysis_count;

  /** @brief Verbose
   *
   * When true, turn on verbose output when we're analyzing this
   * procedure. */

  bool _verbose;

  /** @brief Self timer
   *
   * Keep track of the total time spent analyzing this routine (not
   * including called routines). */

  cbzTimer _self_timer;

  /** @brief Total timer
   *
   * Keep track of the total time spend analyzing this routine, including
   * procedures that it calls. */

  cbzTimer _total_timer;

public:

  // EDB: Added!
  /** @brief Return the dominance frontiers. */

  DFPreds * dominance_frontiers() const { return _dominance_frontiers; }
  
  /** @brief Create a procedureInfo object for a procedure
   *
   *  This constructor creates a new instance, and precomputes the worklist
   *  orderings, and the dominance frontiers. */

  procedureInfo(procNode * the_proc);

  /** @brief Destroy a procedureInfo object */

  ~procedureInfo();

  /** @brief Add ancestor set
   *
   * (Only called once by the procedureDB::build method). Add the list of
   * ancestors. This list is used to force reanalysis to reach the
   * procedure. */

  void add_ancestor_set(procedureinfo_set & ancestors);

  /** @brief Call the procedure
   *
   * Set up the procedure at the given callsite. This method performs many
   * tasks, including making the procedure context-insensitive, if that's
   * called for. */

  void setup_call_at(procedureInfo * caller,
		     stmtLocation * callsite,
		     bool is_recursive_call,
		     bool multiple_target_call);

  /** @brief Return from the procedure
   *
   * This just clears the current callsite, so that there's no
   * confusion.

  void return_from();
  */

  /** @brief Get the procedure */

  inline procNode * proc() const { return _proc; }

  /** @brief Get the procedure name */

  inline string & name() { return proc()->decl()->name(); }

  /** @brief Qualified name
   *
   * Includes either (I), (S), or (R) for context-insensitive,
   * context-sensitive, or recursive. */

  string qualified_name();

  /** @brief Set of ancestors */

  const procedureinfo_set & ancestors() const { return _ancestors; }

  /** @brief Return the calls
   *
   * Return the set of procedures that this one calls. */

  inline const procedureinfo_set & calls() const { return _calls; }

  /** @brief Return the set of callsites. */

  inline const callsite_map & callsites() const { return _callsites; }

  /** @brief Return the first caller */

  inline procedureInfo * first_caller() const { return _first_caller; }

  /** @brief Is procedure recursive?
   *
   * This boolean is set if the analyzer discovers that the procedure is
   * part of a recursive cycle. */

  inline bool is_recursive() const { return _is_recursive; }

  /** @brief Set recursive flag
   *
   * Set by the analyzer when it discovers that the procedure is part of a
   * recrusive cycle. */

  inline void set_recursive() { _is_recursive = true; }

  /** @brief Return variable
   *
   * For procedures with a return value this field holds the declaration of
   * the special dismantler-created variable to hold that value. Such
   * variables are flagged as return values in their declaration. */

  inline declNode * return_variable() const { return _return_variable; }

  /** @brief Return statment
   *
   * Get a pointer to the returnNode (there should only be one!) */

  inline returnNode * return_stmt() const { return _return_stmt; }

  /** @brief Test never returns flag
   *
   * When true, it indicates that this procedure never returns, probably
   * because it always calls exit() or abort(). */

  inline bool never_returns() const { return _never_returns; }

  /** @brief Set never returns flag
   *
   * Setting this flag indicates that the procedure never returns. This is
   * caused by a call to exit() or abort() that dominates the end of the
   * procedure. */

  inline void set_never_returns() { _never_returns = true; }

  /** @brief Set pending changes
   *
   * In context-sensitive analysis when we pass external outputs back to
   * other callsites, we need to collect those changes so that the calling
   * procedure will see them when it's analyzed. This routine stores that
   * set of changes for the caller according to the callsite. */

  void set_pending_changes(stmtLocation * callsite, memoryblock_set & changes);

  /** @brief Get pending changes
   *
   * Check the current callsite to see if changes were left behind by a
   * previous analysis in a different context. If so, get the changes now
   * and merge them in. */

  void get_pending_changes(stmtLocation * callsite, memoryblock_set & changes);

  /** @brief Return the current callsite

  inline stmtLocation * current_callsite() const { return _current_callsite; } */

  /** @brief Return the current caller
   *
   * Returns null is there is none.

  procedureInfo * current_caller();
  */

  /** @brief Return the caller
   *
   * Given a callsite, return the caller at that callsite. */

  procedureInfo * caller_at(stmtLocation * callsite);

  /** @brief Is ancestor?
   *
   * Return true if the given procedure is an ancestor in the call graph. */

  bool is_ancestor(procedureInfo * other);

  /** @brief Get the procedure location
   *
   * For a given callsite, return the corresponding procLocation of the
   * called procedure. For context-insensitive procedures, we just return
   * the one context. */

  procLocation * procedure_location(stmtLocation * callsite);

  /** @name Context insensitivity */
  //@{

  /** @brief Is context-insensitive */

  inline bool is_context_insensitive() const { return _context_insensitive; }

  /** @brief Do we prefer context sensitive? */

  inline bool prefer_context_sensitive() const { return _prefer_context_sensitive; }

  /** @brief Set context-sensitivity preference */

  inline void set_prefer_context_sensitive() { _prefer_context_sensitive = true; }

  /** @brief Get context-insensitive
   *
   * Return the context-insensitive callsite, or null if there is none. */

  procLocation * get_context() const { return _only_context; }

  //@}

  /** @name Merge-point management */
  //@{

  /** @brief Set up merge points
   *
   * When a memory block changes during pointer analysis, this method sets
   * up merg points for the block in the dominance frontier of the
   * procedure. */

  void setup_merge_point(memoryBlock * block_to_merge,
			 basicblockLocation * cur);

  /** @brief Lookup a merge point
   *
   *  Given a particular basic block, see if there are any objects to be
   *  merged at its entry. */

  memoryblock_set * lookup_merge_point(basicblockLocation * where);

  /** @brief Check merge point
   *
   * This method is used only for debugging to make sure we aren't finding
   * new things to merge. */

  void check_merge_point(memoryBlock * block_to_merge,
			 basicblockLocation * cur);

  //@}

  /** @name External input and output management
   *
   * Manage external inputs and outputs when working in context-insensitive
   * mode. */

  //@{

  /** @brief Get external inputs */

  const memoryblock_set & external_inputs() const { return _external_inputs; }

  /** @brief Get external outputs */

  const memoryblock_set & external_outputs() const { return _external_outputs; }

  /** @brief Add an external input
   *
   * This method is called by Pointers::record_external_inputs_and_outputs
   * when it finds a use whose reaching definition is outside the current
   * procedure. This method adds that variable as an external input.
   *
   * Return true if the input wasn't already there. */

  bool add_external_input(memoryBlock * block);

  /** @brief Add an external output
   *
   * This method is called by Pointers::record_external_inputs_and_outputs
   * when it finds a def that could be accessed outside the current
   * procedure. This method adds that variable as an extern output.
   *
   * If the output is the result of a weak update, then we also add to the
   * external inputs, and return whether or not this created a new
   * input. */

  bool add_external_output(memoryBlock * block);

  //@}

  /** @name Worklist management */
  //@{

  /** @brief Get the next block on the worklist.
   *
   *  Return the next block on the worklist, and update the position. */

  basicblockNode * get_next_block(Direction dir);

  /** @brief Add a block to the worklist
   *
   * Add a block, fixing the position as necessary to point to the earliest
   * block on the list. */

  void add_block(basicblockNode * block, Direction dir);

  /** @brief Add all reachable blocks to the worklist
   *
   * Add all the blocks reachable from the given block, NOT including the
   * block itself. */

  void add_reachable_blocks(basicblockNode * block, Direction dir);

  /** @brief Add all blocks
   *
   * Add all blocks in the procedure to the worklist */

  void add_all_blocks();

  /** @brief Add the start node
   *
   *  This is either the entry node (for forward analysis) or the exit node
   *  (for backward analysis). */

  void add_start_block(Direction dir);

  /** @brief Test for empty worklist
   *
   *  Return true if the worklist is empty, which typically indicates that
   *  we are done analyzing this particular procedure. */

  bool is_empty() const;

  /** @brief Remove branch
   *
   * This routine removes basic blocks that will not be visited when the
   * direction of a branch can be determined. */

  void remove_branch_blocks(basicblockNode * cond,
			    basicblockNode * branch_taken,
			    basicblockNode * branch_not_taken);

  /** @brief Conditional worklist algorithm
   *
   * Process a conditional branch. */

  void update_conditional_worklist(basicblockNode * block,
				   bool has_truth_value,
				   bool which_branch);

  /** @brief Active edges
   *
   * Return the set of active edges. */

  inline cfg_edge_set & active_edges() { return _active_edges; }

  //@}

  /** @brief Get the analysis count */

  inline int analysis_count() const { return _analysis_count; }

  /** @brief Increment analysis count */

  inline void incr_analysis_count() { _analysis_count++; }

  /** @brief Is verbose? */

  inline bool is_verbose() const { return _verbose; }

  /** @brief Is library routine?
   *
   * Test the decl_location of the procedure declaration. */

  bool is_library_routine() { return (proc()->decl()->decl_location() == declNode::UNKNOWN); }

  /** @brief Output */

  void print(ostream & out);

  /** @brief Stats
   *
   * Output a whole bunch of statistics about this procedure (particularly
   * useful after analysis is complete). */

  void stats(ostream & out);

  /** @brief Count contexts
   *
   * Count the number of calling contexts for the given procedure. */

  int count_calling_contexts();

  /** @brief Procedure size
   *
   * Count the number of statements in the dismantled form of the
   * procedure. */

  int procedure_size();

  /** @brief Start the self timer */

  void start_self_timer() { _self_timer.start(); }

  /** @brief Stop the self timer */

  void stop_self_timer() { _self_timer.stop(); }

  /** @brief Return the self time */

  double self_timer() { return _self_timer.time(); }

  /** @brief Start the total timer */

  void start_total_timer() { _total_timer.start(); }

  /** @brief Stop the total timer */

  void stop_total_timer() { _total_timer.stop(); }

  /** @brief Return the total time */

  double total_timer() { return _total_timer.time(); }

private:

    /** @brief Count contexts
   *
   * Count the number of calling contexts for the given procedure. */

  int count_calling_contexts_rec(proc_int_map & count,
				 stmtNode * callsite);

  /** @brief Compute  a reverse post-ordering of the basic blocks
   *
   * This routine builds a reverse-post-order traversal of the control-flow
   * graph. The direction argument is used to produce an ordering for the
   * regular CFG as well as the reverse CFG (for backward analysis). */

  void reverse_post_order(Direction dir,
			  basicblockNode * cur,
			  basicblock_set & visited,
			  basicblock_list & order);

  /** @brief Alternative ordering
   *
   * Depth-first search of the dominator tree. */

  void dfs_dominators(Direction dir,
		      basicblockNode * cur,
		      basicblock_set & visited,
		      basicblock_list & order,
		      basicblock_list & rpo_order);

  int block_position(basicblockNode * block, Direction dir);

  basicblockNode * block_at(int position, Direction dir);

  void add_reachable_blocks_rec(Direction dir,
				basicblockNode * cur,
				worklist_set & temp, bool first); 
};

typedef map< procNode *, procedureInfo *> proc_info_map;
typedef proc_info_map::iterator proc_info_map_p;

typedef list< procedureInfo *> proc_info_list;
typedef proc_info_list::iterator proc_info_list_p;

typedef pair< stmtLocation *, procedureInfo * > procedurecall_pair;

typedef list< procedurecall_pair> procedurecall_stack;
typedef procedurecall_stack::iterator procedurecall_stack_p;
typedef procedurecall_stack::const_iterator procedurecall_stack_cp;
typedef procedurecall_stack::reverse_iterator procedurecall_stack_rp;
typedef procedurecall_stack::const_reverse_iterator procedurecall_stack_crp;

/** @brief Database of procedures
 *
 *  This class holds all of the procedureInfo instances. It also contains
 *  an instance of the Linker class, which connects up declarations across
 *  translation units. We also use it to look up procNodes given their
 *  declarations. */

class procedureDB : public Walker
{
private:

  /** @brief All procedures
   *
   * A mapping from procedure definitions to procedure info objects */

  proc_info_map _procedures;

  /** @brief Call graph */

  callGraph * _callgraph;

  /** @brief Need reanalysis
   *
   * In context-insensitive mode, we may need to revisit procedures because
   * of changes in other calling contexts. Whenever a non-local change
   * occurs, we add the procedure and all of it's ancestors back on this
   * list. This ensures that we will revisit the procedure at some point. */

  procedureinfo_set _need_reanalysis;

  /** @brief Root procedure */

  procedureInfo * _root;

  /** @brief Call stack
   *
   * Maintain a call stack during interprocedural analysis. Each entry
   * consists of a callsite and a called procedure. */

  procedurecall_stack _callstack;

  /** @brief Procedure worklist
   *
   */

  proc_info_list _worklist;

public:

  /** @brief Create a new procedureDB
   *
   * The constructor does not create the database */

  procedureDB();

  /** @brief Destroy the procedure database. */

  ~procedureDB();

  /** @brief Build database
   *
   *  This method first visits all the procedures currently parsed and
   *  builds a procedureInfo object for each. It uses the Linker object,
   *  which has already traversed the entire system and fixed the linkages
   *  between symbol references and externally defined symbols. */

  void build(procNode * root, Linker & linker);

  /** @brief Clear
   *
   *  Remove all procedureInfo objects and delete them. */

  void clear();

  /** @brief Add a procedure to the database */

  void add_procedure(procNode * proc);

  /** @brief Lookup procedure-specific information
   */

  procedureInfo * lookup(procNode * proc);

  /** @brief Call stack
   *
   * Return a const reference to the call stack. */

  inline const procedurecall_stack & callstack() const { return _callstack; }

  /** @brief Is recursive call?
   *
   * Return true if calling the given procedure would create a recursive
   * cycle in the call stack. */

  bool is_recursive_call(procedureInfo * callee,
			 int & num_instances);

  /** @brief Set up analysis
   *
   * Add all procedures to the _needs_analysis list. */

  void setup_analysis();

  /** @brief Next procedure
   *
   * Grab the next procedure from the work-list. */

  procedureInfo * next_procedure_on_worklist();

  /** @brief Is procedure on the worklist?
   *
   * If so, remove it and return true. */

  bool is_procedure_on_worklist(procedureInfo * info);

  /** @brief Add a procedure to the worklist
   */

  void add_procedure_to_worklist(procedureInfo * info);

  /** @brief Call a procedure
   *
   * This method is called when analysis is entering a new procedure. It
   * pushes the procedure on the stack and sets up the procedure for
   * analysis. */

  void call_to(stmtLocation * callsite, procedureInfo * callee);

  /** @brief Return from a procedure
   *
   * This method is called when analysis of a procedure is complete. It
   * just pops the current procedure off the stack. */

  void return_from();

  /** @brief Is variable in the caller's scope
   *
   * Check to see if the variable is visible to procedures earlier in the
   * call stack. */

  bool is_visible_to_caller(procedureInfo * info,
			    memoryBlock * block);

  /** @brief Is variable visible?
   *
   * Check to see if the given variable could be accessed in the given
   * procedure, or in any of its ancestors in the callgraph. If it is, then
   * we need to pass it as an external output. */
  
  bool is_visible_to(procedureInfo * info,
		     memoryBlock * block);

  /** @brief Return the current callsite
   */

  inline stmtLocation * current_callsite() { return _callstack.back().first; }

  /** @brief Return the current caller
   *
   * Return the caller of the current procedure in the call stack. */

  procedureInfo * current_caller();

  /** @brief Clear the call stack
   *
   * WARNING: Only use this routine at the end of an analysis pass. */

  void clear_call_stack();

  /** @brief Print call stack */

  void print_call_stack(ostream & out);

  /** @brief Progress meter */

  void progress_meter(ostream & out);

  /** @brief Mark procedure for reanalysis
   *
   * Record that particular procedure needs to be reanalyzed. Add all of
   * it's ancestors to the list as well to ensure that we actually reach it
   * again. This method does not add the given procedure itself unless the
   * include_self flag is true. */

  void mark_for_reanalysis(procedureInfo * info,
			   stmtLocation * callsite,
			   bool include_self);

  /** @brief Mark one procedure for reanalysis
   *
   * Add the given procedure to the list of those needing reanalysis. */

  void mark_one_for_reanalysis(procedureInfo * info);

  /** @brief Has reanalysis been forced?
   *
   * At a procedure call, check to see if this particular procedure is on
   * the _needs_reanalysis list, indicating that we cannot skip analyzing
   * it, even if the inputs appear not to have changed. */

  bool is_reanalysis_required(procedureInfo * info);

  /** @brief Print left-overs */

  void print_leftovers();

  /** @brief Times called
   *
   * Use the callgraph to estimate the number of paths from main down to
   * the given procedure. We'll use this to decide when to make a procedure
   * context insensitive. The basic heuristic is that if a procedure is
   * small, and called in many places, then leave it context sensitive. */

  int times_called(procedureInfo * info);

  /** @brief Print some statistics */

  void stats(ostream & out);

  /** @brief Return the number of procedures */

  int size() const { return _procedures.size(); }

  /** @brief Get the number of procedures
   *
   * This version only counts procedures that have been analyzed. */

  void number_of_procedures(int & total, int & analyzed,
			    int & context_insensitive,
			    int & recursive,
			    int & unanalyzed,
			    int & program_size);

};

#endif // CBZ_PROCEDUREDB_H
