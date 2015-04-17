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

#ifndef CBZ_COPY_GENKILL_H
#define CBZ_COPY_GENKILL_H

#include "reaching_genkill.h"

/** @brief Creates GEN and KILL sets for copy definitions for copy propagation 
    optimization.
    @ingroup Phases

    PLEASE REFER TO reching_genkill.h/cc also

   A defFlowVal is a flow value for the reaching definitions
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
class copyGenKillWalker : public Walker {

public:

	copyGenKillWalker (

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

        ~copyGenKillWalker ();
        
	/** @brief return a new flow value, set to top.

	    this is how you get new empty flow values
	    (rather than calling the defFlowValue constructor)
	*/
        defFlowVal * new_flowval (void);
        
	void at_proc (procNode *, Order);
        inline void printflow(defFlowVal *flow,defFlowVal *flow,
                              stmtNode * r);
         
	void at_basicblock (basicblockNode *, Order);

	// we just want to populate gen and kill sets at the
	// procedure level; it makes no sense at the unit level.
	void at_unit (unitNode *, Order) {
		cerr << 
		"Don't invoke the reachingGenKillWalker from a unitNode!\n";
		exit (1);
	}

        /** @brief get function to get the Union of all the copies that are available
	 */
        FlowVal * getUnion(void) const {

                return copy_union;
        }

        /** @brief get the reference to the map rhs_defs */
        map <declNode *, defFlowVal *> *get_rhsdefs(void){

                
                return &rhs_defs;
        }

        /** @brief get the reference to the map c_defs */
         map <declNode *, defFlowVal *>* get_cdefs( void){
                 return &c_defs;
        }

private:
	 /** @brief map stmtNode's to bit positions */
	map <stmtNode *, int> * node2num;

        /** @brief map stmtNode's to bit positions for copy statements */
	map <stmtNode *, int> * c_node2num;

	/** @brief map stmtNode's to the variable (declarations) they degine  */
	map <stmtNode *, declNode *> * defines;
        
        /** @brief for copy statements that has the stmnt and
	    and the corresponding decl for the rhs.
	    for eg: < x = y , x>
	*/
	map <stmtNode *, declNode *>  c_defines;
        
        /** @brief the map to the rhs part of the copy.
	    similar to c_defines */
        map <stmtNode *, declNode *>  rhs_defines;

	/** @brief  map variable declarations to the set of
	    definitions for them */
	map <declNode *, defFlowVal *> *defs;
        
        /** @brief mappings of all the
	    variables that occur on the lhs of a copy statment.
	    for eg: x = y, n = w;
	    x and n will occur in the c_defs
	*/
        map <declNode *, defFlowVal *>  c_defs;
        
        /** @brief mappings of all the variables that occur on the rhs of a copy statement.
	    for eg: x = y, n = w;
	    declaration of y and w will occur in the rhs_defs map */
        map <declNode *, defFlowVal *>  rhs_defs;
        
	/** @brief list of ambiguous definitions */
	stmt_list ambiguous_defs;

	stmtNode **num2node, ***pn2n;
        stmtNode ** c_num2node;

        int n, c_n,*pn; 

	/** @brief  a generic flow value to clone */
	defFlowVal * _flowval;
       

        /** @brief  the union of all the copy definitions
	    flow values */
        defFlowVal * copy_union;
};

#endif // CBZ_COPY_GENKILL_H

