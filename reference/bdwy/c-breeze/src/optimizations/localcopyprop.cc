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
#include <map>
#include <string>
#include "c_breeze.h"
#include "ref_clone_changer.h"
#include "localcopyprop.h"

Node * LocalCopyPropChanger::at_proc (procNode *p, Order) {
	int count = 0;
	for (stmt_list_p i = p->body()->stmts().begin(); i!=p->body()->stmts().end(); i++) {
		do {
			change = false;
			local_copy_prop ((basicblockNode *) *i);
			count++;
		} while (change && (count < 10));
	}
	return p;
}

void LocalCopyPropChanger::local_copy_prop (basicblockNode *p) {
	// copies maps targets to sources of copies.
	// initially, we clear it since we don't locally know 
	// of any copies

	copies.clear();

	// go through all the stmts in this basic block

	for (stmt_list_p i=p->stmts().begin(); i!=p->stmts().end(); i++) {
		stmtNode *s = (stmtNode *) *i;

		// if the stmt is an expression stmt, see if it
		// kills the copies map by assigning through
		// a pointer

		if (s->typ() == Expr) {
			exprNode *e = (exprNode *) ((exprstmtNode *)s)->expr();
			if (e->typ() == Binary) {
				binaryNode *b = (binaryNode *) e;
				if (b->op()->id() == '=') {
					exprNode *lhs = b->left();
					if (lhs->typ() == Unary) {
						unaryNode *u = (unaryNode *) lhs;
						if (u->op()->id() == Operator::INDIR) {

							// here we have *p = something;
							// this might kill some copy, so
							// we conservatively kill all copies

							copies.clear();
							continue;
						}
					} else if (lhs->typ() == Binary) {
						binaryNode *u = (binaryNode *) lhs;
						if (b->op()->id() == Operator::ARROW) {

							// here we have p->id = something;
							// same idea as above

							copies.clear();
							continue;
						}
						if (b->op()->id() == Operator::Index)
						  copies.clear();
					}
				}
			}
		}

		// propagate copies through this stmt

		had_proc_call = false;
		prop (s);

		// if the statement included a procedure call,
		// we kill all copies, since they could have
		// been wiped out.  note that copies are propagated
		// in the arg list of the call, and that's OK
		// since the call hasn't been done yet.  since
		// we have dismantled code, nothing in this
		// stmt can be affected by copies after the call
		// since the only thing that can happen is
		// an assignment to the return value

		if (had_proc_call) copies.clear();

		// if this is a copy, record it in the map;
		// also, kill some copies because of new defs

		if (s->typ() == Expr) {
			exprstmtNode *ex = (exprstmtNode *) s;
			exprNode *e = ex->expr();
			if (e->typ() == Binary) {
				binaryNode *bn = (binaryNode *) e;
				if (bn->op()->id() == '=') {
					if (bn->left()->typ() == Id) {

						// this is a def of this lhs, killing
						// any previous copies of it

						string a = ((idNode *) bn->left())->name();
						copies[a] = NULL;

						// since lhs may have changed, anything that
						// maps to it is killed.  this is a kludge.

						list<string> l;
						for (map<string,idNode*>::iterator i=copies.begin(); 
							i!=copies.end(); i++) {
							if ((*i).second) if ((*i).second->name() == a)
								l.push_back ((*i).first);
						}
						for (list<string>::iterator j=l.begin(); j!=l.end(); j++) {
							copies[*j] = NULL;
						}

						// this stmt is a copy

						if (bn->right()->typ() == Id) {
							idNode *b = (idNode *) bn->right();
							idNode *c = b;
							// get the "earliest" copy of b
							do {
								b = c;
								c = copies[b->name()];
								if (b == c) break;
							} while (c);
							copies[a] = b;
						}
					}
				}
			}
		}
	}
}

// propagate copy information through a stmt

void LocalCopyPropChanger::prop (stmtNode *s) {
	if (s->typ() == If) {
		ifNode *i = (ifNode *) s;
		i->expr (prop_expr (i->expr()));
	} else if (s->typ() == Return) {
		returnNode *r = (returnNode *) s;
		r->expr (prop_expr (r->expr()));
	} else if (s->typ() == Expr) {
		exprstmtNode *e = (exprstmtNode *) s;
		e->expr (prop_expr (e->expr()));
	}
}

// propagate copy information through an expression, 
// returning the resulting expression

exprNode *LocalCopyPropChanger::prop_expr (exprNode *e) {
	if (!e) return NULL;
	switch (e->typ()) {
		case Call: {
			callNode *c = (callNode *) e;
			for (expr_list_p i=c->args().begin(); i!=c->args().end(); i++)
				*i = prop_expr (*i);
			had_proc_call = true;
			break;
		}
		case Binary: {
			binaryNode *b = (binaryNode *) e;
			// if the lhs is being assigned into,
			// we'd better not try to copyprop it
			if (b->op()->id() != '=')
				b->left (prop_expr (b->left()));
			// the rhs is a tag, not a var
			if (b->op()->id() != Operator::ARROW)
				b->right (prop_expr (b->right()));
			break;
		}
		case Unary: {
			unaryNode *u = (unaryNode *) e;
			switch (u->op()->id()) {
				case Operator::SIZEOF:
				case Operator::ADDRESS: return e;
				default: ;
			}
			u->expr (prop_expr (u->expr()));
			break;
		}
		case Cast: {
			castNode *c = (castNode *) e;
			c->expr (prop_expr (c->expr()));
			break;
		}
		case Const: 
			break;
		case Id: {
			idNode *i = (idNode *) e;
			idNode *j = copies[i->name()];
			if (j) {
				change = true;
				return (idNode *) ref_clone_changer::clone(j, false);
			}
			break;
		}
		default: ;
	}
	return e;
}
