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
#include <stdio.h>
#include "c_breeze.h"
#include "bits.h"
#include "defuse.h"
#include "live.h"
#include "dead.h"

class CleanGenKillWalker: public Walker {
	public:
	CleanGenKillWalker(): Walker (Preorder, Subtree) {}

	void at_node (Node *p, Order) {
		if (p->gen()) {
			delete p->gen();
			p->gen (NULL);
		}
		if (p->kill()) {
			delete p->kill();
			p->kill (NULL);
		}
	}
};

void liveFlowProblem::flow_node (FlowVal *v, Node *n, Point p) {
	liveFlowVal *lv = (liveFlowVal *) v;

	// from Dragon Book,
	// in[B] = use[B] U (out[B] - def[B])

	if (n->def()) {

		// v = v - def

		lv->Difference (n->def());

		// v = v U use
	
		lv->Union (n->use());
	}
}

void liveFlowProblem::flow_block (FlowVal *v, blockNode *n, Point p) {

	if (p == Exit && last()) {
		liveFlowVal *lv = (liveFlowVal *) v;

		// last time through, add an annotation and a
		// comment to this blockNode listing the
		// live variables

		liveAnnote *la = new liveAnnote;
		for (int i=1; i<=flowsize; i++) if (lv->in (i))
			la->live_vars().push_back (decls[i]);
		n->annotations().push_front (la);
	}
	flow_stmt (v, n, p);
}

typedef map<basicblockNode *, bool> blockmap;

// do reverse depth first search

static void rdfs (basicblockNode *b, basicblock_list *l, blockmap *mark) {
	l->push_back (b);
	(*mark)[b] = true;
	basicblock_list_p p;
	for (p=b->preds().begin(); p!=b->preds().end(); p++)
		if (!(*mark)[*p]) rdfs (*p, l, mark);
}

// get reverse depth first search ordering

static void get_rdfs_order (procNode *p, basicblock_list *l) {
	blockmap mark;
	// find the sink, i.e., the unique node with no successors.
	// this may not exist!
	stmt_list_p r;
	basicblockNode *sink;
	for (r=p->body()->stmts().begin(); r!=p->body()->stmts().end(); r++) {
		sink = (basicblockNode *) *r;
		if (sink->succs().size() == 0) break;
	}
	if (sink->succs().size()) {
		// some hoser has a "goto the_beginning;" at the end
		// of the function.  we really just want to get
		// a reasonable ordering of the CFG to visit, so we'll
		// just choose the last thing in the list.
		sink = (basicblockNode *) p->body()->stmts().back();
		//cerr << "can't find sink!  something wrong with cfg!\n";
		//exit (1);
	}
	rdfs (sink, l, &mark);
}
	
void liveFlowProblem::dan_iterate (procNode *p) {
	map <basicblockNode *, liveFlowVal *> in, out;
	blockNode *g = p->body();
	basicblock_list_p i;
	basicblock_list_p s;
	bool change;
	basicblock_list rdfs_order;
	get_rdfs_order (p, &rdfs_order);
	// from Dragon Book
	// for each block B do in[B] = empty set
	// (initialize out[], too)
	for (i=rdfs_order.begin(); i!=rdfs_order.end(); i++) {
		basicblockNode *B = *i;
		in[B] = (liveFlowVal *) top()->clone();
		out[B] = (liveFlowVal *) top()->clone();
	}
	// while changes to any of the in's occur do
	int count = 0;
	liveFlowVal *old_in = (liveFlowVal *) top()->clone();
	liveFlowVal *v = (liveFlowVal *) top()->clone();
	//cout << "beginning iteration\n";
	do {

		//cout << "iteration #" << ++count << "\n";
		change = false;

		// for each block B in reverse depth first order do

		for (i=rdfs_order.begin(); i!=rdfs_order.end(); i++) {
			basicblockNode *B = *i;
			liveFlowVal *inb = in[B];
			liveFlowVal *outb = out[B];

			// out[B] = union over successors S of B in[S]

			for (s=B->succs().begin(); s!=B->succs().end(); s++)
				outb->meet (in[*s]);

			// in[B] = use[B] U (out[B] - def[B])

			old_in->copy (inb);
			inb->copy (outb);
			inb->Difference (B->def());
			inb->meet (B->use());
			if (old_in->diff(inb)) change = true;
		}
	} while (change);
	delete old_in;
	delete v;
	
	//cout << "inserting annotations\n";
	for (i=rdfs_order.begin(); i!=rdfs_order.end(); i++) {
		basicblockNode *B = *i;
		liveAnnote *la = new liveAnnote;

		for (int j=1; j<=flowsize; j++) if (out[B]->in (j))
			la->live_vars().push_back (decls[j]);
		B->annotations().push_front (la);
		//cout << "there are " << la->live_vars().size() << " live vars in block " << B << "\n";
		delete in[B];
		delete out[B];
	}
	//cout << "done with annotations\n";
}

void livenessWalker::at_proc (procNode *p, Order) {
	decl_list_p i;
	int	j, flowsize;
	declNode **locals;

	// map names of variables to bit numbers in set

	map<var_id,int> name2num;

	// get the funcNode for this procNode, which has
	// the list of parameter decls

	funcNode *f = (funcNode *) p->decl()->type();

	// get the body of this proc, which has the
	// list of local variable decls

	blockNode *b = p->body();

	// find the number of locals and formals

	flowsize = f->args().size() + b->decls().size();

	// get an array of declNode pointers, one for each
	// local or formal

	locals = new declNode * [flowsize + 1];

	// establish mapping from locals and formals to bit
	// positions.  NOTE: we start bit positions at 1,
	// instead of 0, because the mapping might return 0
	// when there is a def or use of a nonlocal variable.
	// we don't want to mistake this def or use for
	// something we care about in "global" (i.e., intraprocedural)
	// liveness, so we'll just ignore that bit position.

	for (j=1,i=f->args().begin(); i!=f->args().end(); i++,j++) {
		if ((*i)->typ() != Decl) continue;
		locals[j] = *i;
		name2num[(*i)->name()] = j;
	}
	for (i=b->decls().begin(); i!=b->decls().end(); i++,j++) {
		if ((*i)->typ() != Decl) continue;
		declNode *de = (declNode *) *i;

		// statically initialized variables can give
		// other parts of the code a hard time, so they're
		// best thought of as global variables.

		//if (de->storage_class() == declNode::STATIC)
		//	if (de->init()) continue;
		locals[j] = *i;
		name2num[(*i)->name()] = j;
	}

	// populate gen and kill with defs and uses

	DefUseWalker du (flowsize, &name2num, locals);
	p->walk (du);

	// do the flow problem

	liveFlowProblem lfp (flowsize, &name2num, locals);
	//lfp.iterate (p);
	lfp.dan_iterate (p);

	// get rid of gen and kill

	CleanGenKillWalker cl;
	p->walk (cl);
	delete [] locals;
	for (stmt_list_p bl=p->body()->stmts().begin(); bl!=p->body()->stmts().end(); bl++) {
		basicblockNode *b = (basicblockNode *) *bl;
	}
}

class LivePhase: public Phase {
	public:
	livenessWalker lw;

	void run () { 
		unit_list_p n;

		// for each unit...

		for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++) {

			// liveness analysis

			(*n)->walk (lw);
		}
	}
};

Phases liveness_phase ("live", new LivePhase ());
