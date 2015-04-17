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

#ifndef CBZ_DEFUSE_H
#define CBZ_DEFUSE_H

//
// defuse.h
//
// For each node, gen() will contain the set of variables used in that
// node (unless they are defined first in that node).
// kill() will contain the set of variables defined in that node
// (unless they are used first in that node).
//

#define def kill
#define use gen

typedef string var_id;

/** @brief flow value for storing sets of declarations
    for use in dataflow analyses.
    top is empty set.  meet is union, but
    can be overridden.  Implemented to get
    def/use information, but can be used for
    other situations where you need sets
    of variable declarations.
*/
class declSetFlowVal : public FlowVal {

private:
	int	size;
	Bits	*bits;
	map <var_id, int> *name2num;
	declNode **decls;

public:

	virtual void meet (const FlowVal *v) {
		Union (v);
	}

	void to_top (void) {
		bits->reset_all ();
	}

	FlowVal * clone (void) const {
		declSetFlowVal *other = new declSetFlowVal (size, name2num, decls);
		other->bits = bits->clone();
		return other;
	}

	bool diff (FlowVal *other) const {
		declSetFlowVal *that = (declSetFlowVal *) other;
		for (int i=1; i<=size; i++)
			if (that->bits->value(i) != bits->value(i)) 
				return true;
		return false;
	}

	declSetFlowVal (int n, map<var_id, int> *m, declNode **d) {
		size = n;
		name2num = m;
		decls = d;
		bits = new Bits (n + 1);
		to_top ();
	}

	~declSetFlowVal (void) {
		delete bits;
	}

	bool in (var_id id) {
		return bits->value((*name2num)[id]);
	}

	bool in (int i) {
		return bits->value (i);
	}

	void insert (var_id id) {
		bits->set ((*name2num)[id]);
	}

	void insert (int i) {
		bits->set (i);
	}

	void copy (FlowVal *other) {
		bits->copy (((declSetFlowVal *)other)->bits);
	}

	void Union (const FlowVal *v) {
		declSetFlowVal *w = (declSetFlowVal *) v;
		bits->Or (w->bits);
	}

	// this -= that

	void Difference (FlowVal *other) {
		declSetFlowVal *that = (declSetFlowVal *) other;
		Bits comp(size+1);
		comp.copy (that->bits);
		comp.Not ();
		bits->And (&comp);
	}
};

/** @brief create DEF and USE sets for a unit
    @ingroup Phases
 */
class DefUseWalker: public Walker {
	private:
	declSetFlowVal	*defs;
	declSetFlowVal	*uses;
	map<var_id, int> *name2num;
	declNode 	**decls;
	int flowsize;

	void get_uses (Node *);
	void get_def (Node *);

	public:

	DefUseWalker (int n, map<var_id, int> *m, declNode **d) :
		Walker (Both, Subtree),
		name2num(m),
		decls(d),
		flowsize(n) { }

	void at_basicblock (basicblockNode *, Order);
	void at_proc (procNode *, Order);
};

#endif // CBZ_DEFUSE_H
