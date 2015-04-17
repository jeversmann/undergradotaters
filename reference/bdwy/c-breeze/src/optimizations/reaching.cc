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
#include "bits.h"
#include "reaching_genkill.h"
#include "reaching.h"

void reachingDefinitionsWalker::at_proc (procNode *p, Order ord) {
	if (ord == Postorder) {
		// clean up
		delete [] num2node;
		num2node = NULL;
		return;
	}
	node2num.clear();
	defines.clear();
	in.clear();
	out.clear();
	defs.clear();

	// get gen/kill sets for each basic block

	reachingGenKillWalker gcw (& node2num, & defines, & defs, & num2node, & n);
	p->walk (gcw);
	defFlowVal *v = gcw.new_flowval();
	blockNode *b = p->body();

	// for each basic block B,
	// out[B] = gen[B],
	// in[B] = empty

	stmt_list_p r;
	for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
		basicblockNode *B = (basicblockNode *) *r;
		out[B] = (defFlowVal *) B->gen()->clone();
		in[B] = (defFlowVal *) v->clone();
	}
	delete v;
	defFlowVal *oldout = gcw.new_flowval();
	v = gcw.new_flowval();
	bool change = true;
	
	// there is a lot of copying going on in this loop,
	// but we do this because to move pointers around would
	// require lots of changing the map and malloc'ing,
	// which tends to run out of memory on some pathological
	// functions (like perl's eval.c)

	while (change) {
		change = false;

		// for each block B do

		for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
			basicblockNode *B = (basicblockNode *) *r;
			defFlowVal *inb = in[B];
			defFlowVal *outb = out[B];

			// from Dragon book:
			// in[B] = U_{q in pred(B)} out[Q]

			basicblock_list_p q;
			for (q=B->preds().begin(); q!=B->preds().end(); q++)
				inb->Union (out[*q]);

			oldout->copy (outb);

			// out[B] = gen[B] U (in[B] - kill[B])

			// v is a copy of in[B]
			v->copy (inb);
			v->Difference (B->kill());
			v->Union (B->gen());
			outb->copy (v);
			if (oldout->diff(v)) change = true;
		}
	}
	delete v;
	delete oldout;
}

void reachingDefinitionsWalker::make_ud_chains (exprNode *e) {
	if (!e) return;
	switch (e->typ()) {
	case Id: {
		idNode *i = (idNode *) e;

		// i_defs is the set of definitions of i

		defFlowVal *i_defs = defs[i->decl()];

		// maybe unknown id, like a function identifier

		if (!i_defs) return;

		udChainAnnote *a = new udChainAnnote;
		bool has_ambiguous = false;
		for (int j=1; j<=n; j++) 
			if (i_defs->in (j) 
			&& current_in->in (j)) {
			stmtNode *t = num2node[j];
			if (t->typ() == Expr 
				&& ((exprstmtNode *)t)->expr() == NULL)
				has_ambiguous = true;
			else
				a->ud_chain().push_back (num2node[j]);
		}
		if (has_ambiguous) a->ud_chain().push_back (NULL);
		i->annotations().push_front (a);
		break;
	}
	case Binary: {
		binaryNode *b = (binaryNode *) e;
		if (b->op()->id() != '=')
			make_ud_chains (b->left());
		if (b->op()->id() != Operator::ARROW)
			make_ud_chains (b->right());
		break;
	}
	case Unary: {
		unaryNode *u = (unaryNode *) e;
		// an address computation isn't a
		// use in a real sense; it can't
		// e.g. be replaced by a copy,
		// it has to be the real thing,
		// and doesn't fetch the rvalue
		// of its argument.
		if (u->op()->id() == Operator::ADDRESS) break;
		make_ud_chains (u->expr());
		break;
	}
	case Cast: {
		castNode *c = (castNode *) e;
		make_ud_chains (c->expr());
		break;
	}
	case Call: {
		callNode *c = (callNode *) e;
		// function pointer?
		make_ud_chains (c->name());
		// all the args
		for (expr_list_p i=c->args().begin();
			i!=c->args().end(); i++)
			make_ud_chains (*i);
		break;
        }
	default: ; // we don't care
	}
}

void reachingDefinitionsWalker::make_ud_chains (stmtNode *s) {
	if (s->typ() == If) {
		ifNode *i = (ifNode *) s;
		make_ud_chains (i->expr());
	} else if (s->typ() == Expr) {
		exprstmtNode *ex = (exprstmtNode *) s;
		make_ud_chains (ex->expr());
	} else if (s->typ() == Return) {
		returnNode *r = (returnNode *) s;
		make_ud_chains (r->expr());
	}
}

void reachingDefinitionsWalker::at_basicblock (basicblockNode *b, Order ord) {
	stmt_list_p p;
	if (ord == Postorder) {
		for (p=b->stmts().begin(); p!=b->stmts().end(); p++) {
			stmtNode *s = *p;
			if (s->gen()) {
				delete s->gen();
				s->gen(NULL);
			}
			if (s->kill()) {
				delete s->kill();
				s->kill(NULL);
			}
		}
		if (b->gen()) {
			delete b->gen();
			b->gen(NULL);
		}
		if (b->kill()) {
			delete b->kill();
			b->kill(NULL);
		}
		delete in[b];
		delete out[b];
		return;
	}
	current_in = (defFlowVal *) in[b]->clone();
	for (p=b->stmts().begin(); p!=b->stmts().end(); p++) {
		make_ud_chains (*p);

		// the current reaching definitions
		// are the current ones union
		// the ones generated here minus
		// the ones killed here.

		current_in->Difference ((*p)->kill());
		current_in->Union ((*p)->gen());
	}
	delete current_in;
}

class reachingPhase : public Phase {
public:

	void run () {
		unit_list_p n;
		reachingDefinitionsWalker rdw;

		for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
			(*n)->walk (rdw);
	}
};

Phases reachingPhases("reach", new reachingPhase());
