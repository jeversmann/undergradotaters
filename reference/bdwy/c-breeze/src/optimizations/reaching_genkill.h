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

#ifndef CBZ_REACHING_GENKILL_H
#define CBZ_REACHING_GENKILL_H

//
// reaching_genkill.h
//
// This file contains definitions for defFlowVal and 
// reachingGenKillWalker.


/** @brief a flow value for reaching definitions.
    / A defFlowVal is a flow value for the reaching definitions
    problem.  This flow value represents a set of definitions,
    a subset of the set of all definitions in a given procedure.  
    These definitions are found by the reachingGenKillWalker,
    described below.  The definitions are indexed starting
    from 1, and can be referred to by either their index
    or by a pointer to the definition's stmtNode pointer.
    
    defFlowVal is a subclass of FlowVal, an abstract
    flow value class in C-Breeze, so it can live in the 
    gen() and kill() fields of nodes.
    
    When we refer to gen and kill sets, we're refering to
    defFlowValue's occupying the gen() and kill() fields
    of stmtNode's and basicblockNode's.
*/
class defFlowVal : public FlowVal {

public:

  /// set this flow value to top (i.e., the empty set)
  void to_top (void);

  /// get a clone of this flow value
  FlowVal * clone (void) const;

  /// returns true iff the two flow values differ
  bool diff (FlowVal *other) const;

  /// return true if the definition is in the set, by a pointer to the definition
  bool in (stmtNode *);

  /// return true if the definition is in the set, by the index of the definition
  bool in (int);

  /// insert a definition into the set, by a pointer to the definition
  void insert (stmtNode *);

  /// insert a definition into the set, by the index of the definition
  void insert (int);

  /// remove a definition from the set, by a pointer to the definition
  void remove (stmtNode *);

  /// remove a definition from the set, by the index of the definition
  void remove (int);

  /// make this flow value a copy of the parameter
  void copy (FlowVal *);

  /// set this definition to the union of itself with the parameter
  void Union (const FlowVal *);

  /// set this definition to the intersection of itself with the parameter
  void Intersect (const FlowVal *v);

  /// set this definition to the set difference of itself with the parameter (i.e. this - the other)
  void Difference (FlowVal *);

  /// meet operator (union for reaching definitions)
  void meet (const FlowVal *);

  /** @brief the constructor.

      Note: this constructor for defFlowVal is
      called from within the reachingGenKillWalker;
      you will likely not need to call it yourself.
      Use the new_flowval() function of reachingGenKillWalker
      to get a new flow value, set to top, with the
      right values for the reaching definitions problem
      the walker was called on.
  */
	defFlowVal 

		// the number of definitions
		(int, 			

		// a map from a definition to its index
		map<stmtNode *, int> *, 

		// an array mapping a definition's index to its pointer
		stmtNode **);

	/// destructor
	~defFlowVal (void);

private:

	map <stmtNode*, int> *def2num;
	int	size;
	Bits	*bits;
	stmtNode **definitions;
};

/** @brief populates the gen and kill sets of each statement and basic block in a procedure.
    @ingroup Phases

    reachingGenKillWalker populates the gen() and kill() sets
    of each stmtNode and basicblockNode in a procedure.  It should
    be supplied as the parameter to the walk() function of the
    procNode, preferably from the at_proc() function of some
    other walker that's doing reaching definitions analysis.
    The constructor requires several parameters (described below)
    that will be filled with useful information that can be
    used by the caller. 
    
    Note: it's the caller's responsibility to free memory for
    gen() and kill() of each stmtNode and basicblockNode, and
    to make sure that the variables supplied as arguments to
    the constructor at least until the time the gen() and kill()
    sets are deleted.  In particular, the arguments should not
    be local variables to an at_proc(), unless that at_proc()
    is going to do all the dataflow analysis and then clean up
    after itself before exiting.
    
    About definitions: there are two kinds of definitions this
    walker gets information about.  Unambiguous definitions are
    statements of the form id = expression, where id is an idNode.
    Pointers to these statements will be available in the maps
    and array after walk() is called.  Ambiguous definitions, which
    take the form of an assignment through a pointer or a call
    to a function, are not represented by pointers to the
    statements themselves.  Rather, for each variable defined
    in the procedure, one empty expression statement is allocated
    as "the" ambiguous definition of that variable.  (Otherwise,
    each ambiguous definition would generate a number of definitions
    in the flow value equal to the number of variables, wasting
    a lot of storage.)  Thus, after reaching definitions analysis
    is done, the particular ambiguous definition reaching a given
    block isn't available, but the fact that some ambiguous definition
    reaches the block is.
*/
class reachingGenKillWalker : public Walker {

public:

	// pointers to several variables needed by the walker
	// and useful to the caller are supplied to this
	// constructor
	reachingGenKillWalker (

		// map of definition statements to indices in flow values.
		map <stmtNode *, int> *,

		// defines - will map definition statements
		// to the variables (declNode's) they define

		map <stmtNode *, declNode *> *,

		// map of variables (declarations) to a flow value
		// representing the set of all definitions for 
		// that variable
		map <declNode *, defFlowVal *> *,

		// an array mapping indices in flow values (integers) 
		// to definitions (statement nodes)

		stmtNode ***,

		// the number of definitions
		int *);

	~reachingGenKillWalker (void);

	/** @brief return a new flow value, set to top.

	    this is how you get new empty flow values
	    (rather than calling the defFlowValue constructor)
	*/
	defFlowVal * new_flowval (void);

	void at_proc (procNode *, Order);

	void at_basicblock (basicblockNode *, Order);

	// we just want to populate gen and kill sets at the
	// procedure level; it makes no sense at the unit level.
	void at_unit (unitNode *, Order) {
		cerr << 
		"Don't invoke the reachingGenKillWalker from a unitNode!\n";
		exit (1);
	}

private:
	/// map stmtNode's to bit positions
	map <stmtNode *, int> * node2num;

	/// map stmtNode's to the variable (declarations) they degine
	map <stmtNode *, declNode *> * defines;

	/// map variable declarations to the set of definitions for them
	map <declNode *, defFlowVal *> *defs;

	/// list of ambiguous definitions
	stmt_list ambiguous_defs;

	stmtNode **num2node, ***pn2n;

	/// number of definitions
	int	n, *pn; 

	/// a generic flow value to clone
	defFlowVal * _flowval;

	/// all ambiguous definitions
	defFlowVal * _everything;

};

#endif // CBZ_REACHING_GENKILL_H
