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
// dead.cc
//
// Dead code elimination
//
#include "c_breeze.h"
#include "bits.h"
#include "defuse.h"
#include "live.h"
#include "dead.h"

bool deadCodeEliminationChanger::has_side_effects (exprNode *e) {
	switch (e->typ()) {
		// maybe one day we'll see if
		// the const qualifier on the funcNode for
		// this call means that its a pure function
		// whose results can be ignored.  i doubt it.
		// certainly, with interprocedural analysis,
		// we might be able to detect pure functions,
		// and definitely C library functions.
		// for now, we assume all functions have
		// side effects.
		case Call: return true;
		case Unary: {
			unaryNode *u = (unaryNode *) e;
			// might bus or segfault; this is
			// a side effect we should try
			// to preserve?
			if (u->op()->id() == Operator::INDIR) return true;
			return false;
		}
		case Binary: {
			binaryNode *b = (binaryNode *) e;
			if (b->op()->id() == Operator::ARROW) return true;
			// this should *never* happen, but I'm
			// not 100% sure so better safe than sorry.
			if (b->op()->id() == '=') return true;
			return false;
		}
		default: return false;
	}
}

void deadCodeEliminationChanger::find_address_taken (exprNode *e) {
	if (!e) return;
	switch (e->typ()) {
		case Unary: {
			unaryNode *u = (unaryNode *) e;
			if (u->op()->id() == Operator::ADDRESS) {
				if (u->expr()->typ() == Id) {
					idNode *i = (idNode *) u->expr();
					is_live[i->decl()] = true;
				}
			}
			break;
		}
		case Binary: {
			binaryNode *b = (binaryNode *) e;
			find_address_taken (b->right());
			break;
		}
		default: ;
	}
}

void deadCodeEliminationChanger::mark_as_live (exprNode *e) {
	if (!e) return;
	switch (e->typ()) {
		case Call: {
			callNode *c = (callNode *) e;

			// function pointer?

			mark_as_live (c->name());
			for (expr_list_p i=c->args().begin(); i!=c->args().end(); i++)
				mark_as_live (*i);
			break;
		}
		case Binary: {
			binaryNode *b = (binaryNode *) e;
			if (b->op()->id() != '=')
				mark_as_live (b->left());
			// the rhs is a tag, not a var
			if (b->op()->id() != Operator::ARROW)
				mark_as_live (b->right());
			break;
		}
		case Unary: {
			unaryNode *u = (unaryNode *) e;
			mark_as_live (u->expr());
			break;
		}
		case Cast: {
			castNode *c = (castNode *) e;
			mark_as_live (c->expr());
			break;
		}
		case Const: 
			break;
		case Id: {
			idNode *i = (idNode *) e;
			if (is_local[i->decl()])
				is_live[i->decl()] = true;
			break;
		}
		default: ;
	}
}

exprNode * deadCodeEliminationChanger::handle_expr (exprNode *e) {
	if (!e) return NULL;
	if (e->typ() == Binary) {
		binaryNode *b = (binaryNode *) e;
		if (b->op()->id() == '=') {
			exprNode *lhs = b->left();
			exprNode *rhs = b->right();
			if (lhs->typ() == Id) {
				idNode *i = (idNode *) lhs;
				declNode *d = i->decl();
				if (is_local[d] && !is_live[d]) {
					// dead assignment
					if (has_side_effects (rhs)) {
						mark_as_live (rhs);
						return rhs;
					}
					else
						return NULL;
				}
			} else
				// this assignment was probably
				// through a pointer, and counts
				// as a use of that pointer

				mark_as_live (lhs);

			// everything on the right hand size is live;
			// the lhs is def'ed, so might not be live before this;
			// the analysis would have told us so we don't
			// have to worry.

			mark_as_live (rhs);
			return e;
		}
	}

	// everything mentioned from this expression is live as far
	// as the preceeding statements are concerned

	mark_as_live (e);
	return e;
}

void deadCodeEliminationChanger::local_dce (basicblockNode *b) {
	is_live.clear();

	// get list of live variables on exit
	// of this block

	liveAnnote *l = liveAnnote::getLiveAnnote (b);
	if (!l) {
		cerr << "no list of live vars!\n";
		return;
	}

	// mark as live all the variables we know are live
	// on exit of this block

	for (decl_list_p i=l->live_vars().begin(); 
		i!=l->live_vars().end(); i++) {
		is_live[*i] = true;
	}

	// go forward looking for variables whose address
	// is taken; these must be marked as used throughout the
	// whole block, since they may look dead but be accessed
	// through an alias

	for (stmt_list_p a=b->stmts().begin(); a!=b->stmts().end(); a++) {
		stmtNode *s = *a;
		if (s->typ() == Expr) {
			exprstmtNode *ex = (exprstmtNode *) s;
			find_address_taken (ex->expr());
		}
	}

	// go backward trying to find dead code,
	// marking as live variables that become live
	// as we go back in time

	for (stmt_list::reverse_iterator r=b->stmts().rbegin(); 
		r!=b->stmts().rend(); r++) {
		stmtNode *s = *r;
		switch (s->typ()) {
			case Expr: {
				exprstmtNode *e = (exprstmtNode *) s;
				e->expr (handle_expr (e->expr()));
				break;
			}
			case If: {
				ifNode *i = (ifNode *) s;
				mark_as_live (i->expr());
				break;
			}
			case Return: {
				returnNode *r = (returnNode *) s;
				mark_as_live (r->expr());
				break;
			}
			default: ;
		}
	}

	// get rid of null exprstmts

	for (stmt_list_p p=b->stmts().begin(); p!=b->stmts().end(); ) {
		if ((*p)->typ() == Expr 
		&& ((exprstmtNode *) (*p))->expr() == NULL)
			p = b->stmts().erase (p);
		else
			p++;
	}
}

// d will be counted as a local variable only
// if it isn't static, extern, or volatile.

void deadCodeEliminationChanger::tick_local (declNode *d) {
	declNode::Storage_class sc = d->storage_class();
	if (sc != declNode::STATIC && sc != declNode::EXTERN) {
		typeNode::Type_qualifiers tq 
			= d->type()->type_qualifiers();
		if (!(tq & typeNode::VOLATILE)) 
			is_local[d] = true;
	}
}
	
Node * deadCodeEliminationChanger::at_proc (procNode *p, Order) {
	stmt_list_p sp;
	decl_list_p dp;

	// get a list of local variables

	is_local.clear();
	funcNode *f = (funcNode *) p->decl()->type();
	blockNode *b = p->body();
	for (dp=f->args().begin(); dp!=f->args().end(); dp++) 
		if ((*dp)->typ() == Decl) tick_local (*dp);
	for (dp=b->decls().begin(); dp!=b->decls().end(); dp++)
		if ((*dp)->typ() == Decl) tick_local (*dp);
	for (stmt_list_p sp=b->stmts().begin(); 
		sp!=b->stmts().end(); sp++) {
		local_dce ((basicblockNode *) *sp);
	}
	return p;
}
