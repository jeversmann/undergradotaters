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

#ifndef CBZ_LIVE_H
#define CBZ_LIVE_H

///
// live.h
//

// string is not the best choice to identify a variable.  I'd like to
// have something like a uniqe address in a symbol table somewhere, to
// avoid confusing two variables with the same name
// (e.g. int foo () { { int a; } { int a; } } )
// whatever var_id turns out to be, it'll have to have a == operator.

typedef declSetFlowVal liveFlowVal;

/** @brief Liveness flowproblem.
 */
class liveFlowProblem: public FlowProblem {
	private:

	procNode *proc;
	declNode **decls;
	map <var_id, int> *name2num;
	int flowsize;

	public:
	liveFlowProblem (int n, map<var_id,int> *m, declNode **vars) :
		decls(vars),
		name2num(m),
		flowsize(n),
		FlowProblem(false, new liveFlowVal (n, m, vars)) { }

	~liveFlowProblem (void) { }
	void flow_node (FlowVal *, Node *, Point);
	void flow_block (FlowVal *, blockNode *, Point);
	void dan_iterate (procNode *p);
};

/** @brief annotations for liveness flowproblem.
 */
class liveAnnote: public Annote {
	decl_list _live_vars;

	public:

	static liveAnnote * getLiveAnnote(basicblockNode *b) {
		string live("liveness");
		for (annote_list_p i=b->annotations().begin();
			i!=b->annotations().end(); i++) {
			if ((*i)->is_annote_kind (live))
				return (liveAnnote *) *i;
		}
		return NULL;
	}

	static void removeliveness (basicblockNode *b) {
		string live("liveness");
		for (annote_list_p i=b->annotations().begin();
			i!=b->annotations().end(); ) {
			if ((*i)->is_annote_kind (live)) {
				// this deletes the liveness annotation
				delete *i;
				i = b->annotations().erase (i);
			}
			else
				i++;
		}
	}

	liveAnnote (decl_list l) : _live_vars (l) { }

	liveAnnote (void) { }

	decl_list & live_vars (void) { return _live_vars; }
	void live_vars (decl_list l) { _live_vars = l; }
	
	bool is_annote_kind (string & name) {
		return name == "liveness";
	}
};

/** @brief remove liveness annotation on basic blocks.
    @ingroup Phases
*/
class livenessRemover : public Walker {
	public:

	livenessRemover (void) : Walker (Preorder, Subtree) { }

	void at_basicblock (basicblockNode *b, Order) {
		if (b->annotations().size())
			liveAnnote::removeliveness (b);
	}
};

/** @brief visit live blocks in procedures
    @ingroup Phases
*/
class livenessWalker: public Walker {
        public:
        livenessWalker(): Walker (Preorder, Subtree) {}

	void at_proc (procNode *, Order);
};


#endif // CBZ_LIVE_H
