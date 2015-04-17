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

#ifndef CBZ_REACHING_H
#define CBZ_REACHING_H

//
// reaching.h
//
// This file contains class definitions for solving the reaching
// definitions problem for basic blocks.  The end result of
// reaching definitions analysis is that use of a variable
// defined in a procedure will be annotated with a ud-chain
// giving all of the definitions that reach that use.

/** @brief annotation denoting a use-def chain

    udChainAnnote is an annotation (i.e., can be inserted into the
    annotations() field of a node) containing a ud-chain.
    Annotations are subclasses of the abstract class Annote.
    an annotation must define the function 
    
    bool is_annote_kind (string &);
    
    returning true when the identifying string of that kind
    of annotation is passed to it, false otherwise.
    This way the programmer can search the annotation list
    for a node for the particular annotation he or she
    is interested in.  See annote.h for more information 
    about annotations.
    
    Ud-chains are store as a list of statement node pointers.
    If a definition is unambiguous, the stmtNode * of the
    definition itself is stored in the list.  If an ambiguous 
    definition reaches a statement, NULL is used (rather than 
    the empty exprstmtNode* allocated during the reachingGenKillWalker, 
    which has a limited lifetime).  Note that, no matter how many 
    ambiguous definitions reach a statement, only one NULL pointer 
    needs to be stored in the ud-chain.
*/
class udChainAnnote : public Annote {

private:
	stmt_list _ud_chain;

public:
	udChainAnnote (void) { }

	/// return a reference to the ud-chain in this annotation
	stmt_list & ud_chain (void) { return _ud_chain; }

	/// return true if someone is searching for a ud-chain
	bool is_annote_kind (string & s) {
		return s == "ud_chain";
	}

	/// search a node for a udChainAnnote, returning it if it's found, NULL otherwise.
	static udChainAnnote * getUdChain (Node *n) {
		annote_list_p a;
		for (a=n->annotations().begin(); 
			a!=n->annotations().end(); a++) {
			string s("ud_chain");
			if ((*a)->is_annote_kind (s))
				return (udChainAnnote *) *a;
		}
		return NULL;
	}

	static void * removeUdChains (Node *n) {
		annote_list_p a;
		for (a=n->annotations().begin(); 
			a!=n->annotations().end(); ) {
			string s("ud_chain");
			if ((*a)->is_annote_kind (s))
				a = n->annotations().erase (a);
			else
				a++;
		}
			// EDB
		return NULL;

	}
		
};

/** @brief remove use-def chain annotations.
    @ingroup Phases
*/
class udChainRemover : public Walker {
public:
	udChainRemover (void) : Walker (Preorder, Subtree) { }

	void at_id (idNode *i, Order) {
		if (i->annotations().size())
			udChainAnnote::removeUdChains (i);
	}
};

/** @brief performs dataflow analysis for reaching definitions.
    @ingroup Phases

    reachingDefinitionsWalker does the dataflow analysis for the
    reaching definitions problem, leaving ud-chains (described above)
    at each use of a defined variable in a procedure.  It is (should be)
    designed to be walked from either a procNode, to compute ud-chains
    for one procedure, or from a unitNode, to compute ud-chains for
    every procedure.  Thus, the at_proc() function should re-initialize
    variables global to the class each time it is called.
*/
class reachingDefinitionsWalker : public Walker {

public:

	// plain old walker
	reachingDefinitionsWalker (void) : Walker (Both, Subtree) { }

	void at_proc (procNode *, Order);
	void at_basicblock (basicblockNode *, Order);

private:
	// make ud-chains for each use in this expression
	void make_ud_chains (exprNode *);

	// make ud-chains for each use in this statement
	void make_ud_chains (stmtNode *);

	// in and out sets for each basic block node
	map <basicblockNode *, defFlowVal *> in, out;

	// array of pointers to stmtNode's maps bit positions
	// to definitions in the current procedure
	stmtNode **num2node;

	// n is the number of definitions in the current procedure
	int	n;

	// the current value 
	defFlowVal *current_in;

	// maps definitions (stmts) to bit positions
	map <stmtNode *, int> node2num;	

	// maps a statement to the variable it defines
	map <stmtNode *, declNode *> defines;

	// maps a variable to the definitions
	// that (may) affect it
	map <declNode *, defFlowVal *> defs;
};

#endif // CBZ_REACHING_H
