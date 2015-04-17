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
//
// defuse.cc
//
// For each node, gen() will contain the set of variables used in that
// node (unless they are defined first in that node).
// kill() will contain the set of variables defined in that node
// (unless they are used first in that node).
//

#include "c_breeze.h"
#include "bits.h"
#include "defuse.h"

// traverse a right hand side, collecting the names of
// all the variables used into the declSetFlowVal v

void DefUseWalker::get_uses (Node *r) {
	if (!r) return;
	switch (r->typ()) {
		case Id: {

			// if this id hasn't been def'ed 
			// before, it is a use.

			idNode *i = (idNode *) r;
			if (!(defs->in (i->name())))
				uses->insert (i->name());
			break;
		}

		// the rest of the cases basically
		// traverse the expression tree
		// trying to get to the first case

		case If: {
			ifNode *i = (ifNode *) r;
			get_uses (i->expr());
			break;
		}
		case Expr: {
			exprstmtNode *ex = (exprstmtNode *) r;
			get_uses (ex->expr());
			break;
		}
		case Return: {
			returnNode *re = (returnNode *) r;
			get_uses (re->expr());
			break; 
		}
		case Binary: {
			binaryNode *b = (binaryNode *) r;
			get_uses (b->left());

			// the rhs of a -> may look like
			// a use of a local variable with
			// the same name as the struct field,
			// so we won't go there.

			if (b->op()->id() != Operator::ARROW)
				get_uses (b->right());
			break;
		}
		case Unary: {
			unaryNode *b = (unaryNode *) r;
			get_uses (b->expr());
			break;
		}
		case Cast: {
			castNode *c = (castNode *) r;
			get_uses (c->expr());
			break;
		}
		case Call: {
			callNode *c = (callNode *) r;

			// function pointer?

			get_uses (c->name());

			// all the args

			for (expr_list_p i=c->args().begin(); 
				i!=c->args().end(); i++)
				get_uses (*i);
			break;
		}
		default: 
			; // we don't care
	}
}

// get a def from n, putting it in defs.

void DefUseWalker::get_def (Node *n) {
	// is it an expression stmt?
        if (n->typ() == Expr) {
		exprstmtNode *ex = (exprstmtNode *) n;
		exprNode *e = ex->expr();

		// e could be NULL ( e.g. dummy node inserted during cfg phase)
		
		if (e==NULL)
		  return;
		
		// is it a binary expression?

		if (e->typ() == Binary) {
			binaryNode *bi = (binaryNode *) e;

			// is it an assignment?

			if (bi->op()->id() == '=') {
				exprNode *lhs = bi->left();

				// is the lhs an id?

				if (lhs->typ() == Id) {
					idNode *i = (idNode *) lhs;

					// before we suspect this is really
					// a def, let's check to see whether
					// something (e.g. lhs!) is used
					// in the rhs

					get_uses (bi->right());

					// has it not been used before?

					if (!(uses->in(i->name()))) {

						// it's a def.

						defs->insert (i->name());
					}
				}
			}
		}
	}
}

// at a basic block, get all the defs and uses and stick them
// in kill and gen, resp. (which have been renamed with macros)

void DefUseWalker::at_basicblock (basicblockNode *b, Order ord) {
	if (ord != Preorder) return;
	declSetFlowVal *v = new declSetFlowVal (flowsize, name2num, decls);
	defs = new declSetFlowVal (flowsize, name2num, decls);
	uses = new declSetFlowVal (flowsize, name2num, decls);
	for (stmt_list_p i=b->stmts().begin(); 
		i!=b->stmts().end(); i++) {
		stmtNode *n = *i;
		
		// get a def

		get_def (n);

		// get uses

		get_uses (n);
	}
	b->def (defs);
	b->use (uses);

	// put in a comment giving the defs and uses for this block

	string s = "defs: ";
	for (int i=1; i<=flowsize; i++) {
		if (defs->in (i))
			s += decls[i]->name() + " ";
	}
	s += "; uses: ";
	for (int i=1; i<=flowsize; i++) {
		if (uses->in (i))
			s += decls[i]->name() + " ";
	}
	b->comment() = s;
}

class fixPointerWalker : public Walker {

private:
	map <basicblockNode *, bool> marks;
	basicblockNode *current_block;
	map <var_id, int> *name2num;

public:
	fixPointerWalker (map <var_id, int> *m, basicblockNode *c) :
		name2num(m), 
		current_block(c),
		Walker (Preorder, Subtree) { }

	void at_unary (unaryNode *, Order);
	void dfs_used (basicblockNode *, int);
	void at_basicblock (basicblockNode *b, Order) {
		current_block = b;
	}
};

void DefUseWalker::at_proc (procNode *p, Order ord) {
	if (ord == Postorder) {
		basicblockNode *b;
		assert (p->body()->stmts().size());
		b = (basicblockNode *) p->body()->stmts().front();
		assert (b->typ() == Block);
		fixPointerWalker w (name2num, b);
		p->walk (w);
	}
}

void fixPointerWalker::at_unary (unaryNode *u, Order) {
	// if the address of a local is taken, it could be used
	// (or def'ed) when it looks dead, so we must mark it used
	// anywhere reachable from the current basic block

	if (u->op()->id() == Operator::ADDRESS && u->expr()->typ() == Id) {
		idNode *id = (idNode *) u->expr();
		int i = (*name2num)[id->name()];
		if (i > 0) {
			marks.clear();
			dfs_used (current_block, i);
		}
	}
}

void fixPointerWalker::dfs_used (basicblockNode *b, int i) {
	marks[b] = true;
	if (!b) return;
	declSetFlowVal *v = (declSetFlowVal *) b->def();
	if (!v) return;
	if (!(v->in (i))) {
		declSetFlowVal *w = (declSetFlowVal *) b->use();
		if (!w) return;
		w->insert (i);
	}
	if (b->succs().size())
	for (basicblock_list_p p=b->succs().begin(); p!=b->succs().end(); p++)
		if (!(marks[*p])) dfs_used (*p, i);
}
