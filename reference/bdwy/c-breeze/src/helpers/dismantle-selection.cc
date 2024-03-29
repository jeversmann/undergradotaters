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
#include "ref_clone_changer.h"
#include "semcheck.h"

#include "dismantle.h"

Node * SelectionDismantleChanger::at_selection (selectionNode *p, Order) {
	return dismantle_selection (p);
}

Node * SelectionDismantleChanger::dismantle_selection (selectionNode *p) {
	switch (p->typ()) {
		case If: 
			return dismantle_ifelse ((ifNode *) p);
		case Switch: 
			return dismantle_switch ((switchNode *) p);
		default: 
			return p;
	}
}

exprNode * SelectionDismantleChanger::negate_if_condition(exprNode * node)
{
	// is it unary negation?
	if ( node->typ() == Unary && ((unaryNode*)node)->op()->id() == '!' )
	{
		// just take off the !
		return (exprNode*)ref_clone_changer::clone( ((unaryNode*)node)->expr(), false );
	}
	// maybe some kind of binary?
	else if ( node->typ() == Binary )
	{
		// which operator?
		unsigned int newOp = (unsigned int)-1;
		switch ( ((binaryNode*)node)->op()->id() )
		{
		case Operator::EQ:	newOp = Operator::NE;	break;
		case Operator::NE:	newOp = Operator::EQ;	break;
		case '<':			newOp = Operator::GE;	break;
		case Operator::LE:	newOp = '>';			break;
		case '>':			newOp = Operator::LE;	break;
		case Operator::GE:	newOp = '<';			break;
		}

		// was it binary comparison op?
		if ( newOp != (unsigned int)-1 )
		{
			// just make a new comparison with the switched operator
			return new binaryNode( newOp, 
				(exprNode*)ref_clone_changer::clone( ((binaryNode*)node)->left(), false ),
				(exprNode*)ref_clone_changer::clone( ((binaryNode*)node)->right(), false ),
				node->coord() );
		}
	}

	// it is not explicitly a comparison - so just compare it with zero
	// NOTE: since this if() is implicitly testing (expr != 0), what we 
	//	want to test is (expr == 0), because we are doing the negation
	constant zero;
	zero.make_zero( LirVt::getVarType( node ) );
	constNode * con = new constNode( zero );
	con->type(new primNode(basic_type::UInt));
	return new binaryNode( Operator::EQ, node, con, node->coord() ); 
}

// change
//	if (cond) stmt;
// into
//	if (! cond) goto L;
//	stmt;
//	L: ;
// OR (ifdef TWO_JUMP_IF)
//	if (cond) goto L1;
//	goto L2;
//	L1: stmt;
//	L2: ;


Node * SelectionDismantleChanger::dismantle_if (ifNode *p) {
	blockNode *newblock = new blockNode (NULL, NULL, p->coord());
	
	// annotation to tell where this if came from

	p->annotations().push_back (new fromAnnote (FROM_IF));

	// if this 'if' isn't already dismantled, dismantle it

	if (((p->expr()->typ() != Id) && (p->expr()->typ() != Const))
	|| (!(p->true_br()->stmts().empty())
		&& (p->true_br()->stmts().front()->typ() != Goto))
	|| (p->true_br()->stmts().empty())) {
		if (dismantle_flags & TWO_JUMP_IF) {
			unaryNode *u;
			ifNode *ie;
			// if we have 'if (!cond)'...
			if (p->expr()->typ() == Unary) {
				u = (unaryNode *) p->expr();
				if (u->op()->id() == '!') {
					// make it an if (cond); else stmt
					ie = new ifNode 
					(u->expr(), clone_empty_stmt(), 
						p->true_br(), p->coord());
	
					// annotation to tell where this is from
	
					ie->annotations().push_back 
						(new fromAnnote (FROM_IF));
					return dismantle_ifelse (ie);
				}
			}
			ie = new ifNode 
				(p->expr(), p->true_br(), clone_empty_stmt(), p->coord());
			return dismantle_ifelse (ie);
		}
		// negate the test expr
		exprNode *newcond = negate_if_condition(p->expr());
		newcond->type(new primNode(basic_type::UInt));
		// make a variable that holds the results of the test
		idNode *condvar = new_id("I");
		condvar->coord(p->coord());
		declNode *c_decl = 
		  new declNode((idNode *) ref_clone_changer::clone(condvar, false),
			       declNode::NONE,
			       (typeNode *) ref_clone_changer::clone(newcond->type(), false),
			       NULL, NULL);
		c_decl->decl_location(declNode::BLOCK);
		condvar->decl (c_decl);
		// if (condvar)
		p->expr((exprNode *) ref_clone_changer::clone(condvar, false));
		// condvar = test expr
		binaryNode *condassg = new binaryNode ('=', condvar, newcond, p->coord());
		condassg->type (new primNode(basic_type::UInt));
		newblock->decls().push_back (c_decl);
		newblock->stmts().push_back (new exprstmtNode (condassg));
		stmtNode *stmt = p->true_br ();
		idNode *l = new_id ("I");
		l->coord(p->coord());
		labelNode *L = new labelNode (l, clone_empty_stmt());
		gotoNode * newgoto = new gotoNode(L, p->coord());
		blockNode *newgotoblock = new blockNode (NULL, NULL, p->coord());
		// goto L;
		newgotoblock->stmts().push_back (newgoto);
		p->true_br (newgotoblock);

		newblock->stmts().push_back (p);
		newblock->stmts().push_back (stmt);
		newblock->stmts().push_back (L);
		return newblock;
	} else 
		return p;
}

// change
//	if (cond) stmt1; else stmt2;
// into
//	if (cond) goto L1;
//	stmt2;
//	goto L2;
//	L1: stmt1;
//	L2:;

Node * SelectionDismantleChanger::dismantle_ifelse (ifNode *p) {
	// any if/else can't be have been dismantled, since it has an else,
	// so we don't need to check whether it's dismantled.

        // Call the old code for if nodes when there is no else

        if ( ! p->false_br())
          return dismantle_if(p);

	// make sure the type of the expression is set

	semcheck_expr_visitor::check(p->expr());
	blockNode *newblock = new blockNode (NULL, NULL, p->coord());
	idNode *l1 = new_id ("IE");
	idNode *l2 = new_id ("IE");

	l1->coord(p->coord());
	labelNode *L1 = new labelNode (l1, clone_empty_stmt());

	l2->coord(p->coord());
	labelNode *L2 = new labelNode (l2, clone_empty_stmt());

	gotoNode * gotol1 = new gotoNode(L1, p->coord());
	gotoNode * gotol2 = new gotoNode(L2, p->coord());
	bool condition_is_simple = true;
	declNode *c_decl;
	binaryNode *condassg;

	// if the condition isn't an Id or Const,
	// make it one

	if ((p->expr()->typ() != Id) && (p->expr()->typ() != Const)) {
		condition_is_simple = false;
		idNode *condvar = new_id ("IE");
		condvar->coord(p->coord());
		c_decl = new declNode((idNode *) ref_clone_changer::clone(condvar, false),
				     declNode::NONE,
				     (typeNode *) ref_clone_changer::clone(p->expr()->type(), false),
				     NULL, NULL);
		c_decl->decl_location(declNode::BLOCK);
		condvar->decl (c_decl);
		condassg = new binaryNode ('=', 
					   (exprNode *) ref_clone_changer::clone(condvar, false),
					   p->expr(), p->coord());
		condassg->type((typeNode *) ref_clone_changer::clone(p->expr()->type(), false));
		p->expr (condvar);
	}
	ifNode *newif = new ifNode (p->expr(), gotol1, 0, p->coord());

	// annotation to tell whether this came from an if or an ifelse

	if (fromAnnote::find_from (p, "from_if"))
		newif->annotations().push_back (new fromAnnote (FROM_IF));
	else
		newif->annotations().push_back (new fromAnnote (FROM_IFELSE));
	if (!condition_is_simple) {
		newblock->decls().push_back (c_decl);
		newblock->stmts().push_back (new exprstmtNode (condassg));
	}
	newblock->stmts().push_back (newif);
	newblock->stmts().push_back (p->false_br());
	newblock->stmts().push_back (gotol2);
	newblock->stmts().push_back (L1);
	newblock->stmts().push_back (p->true_br());
	newblock->stmts().push_back (L2);
	return newblock;
}

Node * SelectionDismantleChanger::dismantle_switch (switchNode *p) {

	target_list_p	j;
	gotoNode	*on_default = NULL;
	idNode 		*exprvar;

	blockNode *newblock = new blockNode (NULL, NULL, p->coord());
	idNode *breakid = new_id ("B");
	breakid->coord(p->coord());
	semcheck_expr_visitor::check(p->expr());

	// if the switch expression is already just an id,
	// we don't need to make a temporary variable.

	if (p->expr()->typ() != Id) {
		exprvar = new_id ("S");
		exprvar->coord(p->coord());
		declNode *edecl = 
		  new declNode((idNode *) ref_clone_changer::clone(exprvar, false),
			       declNode::NONE,
			       (typeNode *) ref_clone_changer::clone(p->expr()->type(), false),
				NULL, NULL);
		edecl->decl_location(declNode::BLOCK);
		exprvar->decl (edecl);
		newblock->decls().push_back (edecl);
		binaryNode *exprassg = new binaryNode ('=', 
						       (exprNode *) ref_clone_changer::clone(exprvar, false),
						       p->expr(), p->coord());
		newblock->stmts().push_back (new exprstmtNode (exprassg));
	} else {
		exprvar = (idNode *) (p->expr());
	}
	idNode *comparvar = new_id();
	comparvar->coord(p->coord());
	declNode *c_decl = 
	  new declNode((idNode *) ref_clone_changer::clone(comparvar, false),
		       declNode::NONE,
		       new primNode(basic_type::SInt),
		       NULL,
		       NULL);
	c_decl->decl_location(declNode::BLOCK);
	comparvar->decl (c_decl);
	newblock->decls().push_back (c_decl);
	for (j=p->cases().begin(); j!=p->cases().end(); j++) {
		if ((*j)->typ() == Case) {
			caseNode *cn = (caseNode *) *j;
			if ( ! cn->expr()) {
			  // Default case: do something :-)
			  idNode *l = new_id ("D");
			  l->coord(p->coord());
			  labelNode *L = new labelNode (l, clone_empty_stmt());
			  on_default = new gotoNode(L, p->coord());
			  (*j)->stmt()->stmts().push_front (L);
			}
			else {
			  binaryNode *compar = 
			    new binaryNode (Operator::EQ, 
					    (exprNode *) ref_clone_changer::clone(exprvar, false),
					    (exprNode *) ref_clone_changer::clone(cn->expr(), false),
					    p->coord());
			  compar->type((typeNode *) ref_clone_changer::clone(p->expr()->type(), false));
			  binaryNode *comparassg = new binaryNode ('=',
								   (exprNode *) ref_clone_changer::clone(comparvar, false),
								   compar, p->coord());
			  newblock->stmts().push_back 
			    (new exprstmtNode (comparassg));
			  idNode *l = new_id ("C");
			  l->coord(p->coord());
			  labelNode *L = new labelNode (l, clone_empty_stmt());
			  gotoNode * jump = new gotoNode(L, p->coord());
			  // hack: change the case stmt to be the label.
			  // we'll fix it up later.
			  (*j)->stmt()->stmts().push_front (L);
			  ifNode * caseif = new ifNode((idNode *) ref_clone_changer::clone(comparvar, false),
						       jump,
						       0, p->coord());

			  // annotation to tell where this 'if' came from

			  if (p->cases().size() >= 8) {
			    caseif->annotations().push_back 
			      (new fromAnnote (FROM_MANY_CASES));
			  }
			  caseif->annotations().push_back 
			    (new fromAnnote (FROM_SWITCH));
			  newblock->stmts().push_back (caseif);
			}
		} 
	}
	// no default; make one up; pretend to 'break'
	labelNode *breaklabel = 
	  new labelNode (breakid, clone_empty_stmt());
	if (!on_default) {
	  on_default = new gotoNode(breaklabel, p->coord());
	}
	newblock->stmts().push_back (on_default);
	newblock->stmts().push_back (p->stmt());
		//((blockNode *) clone_changer::clone(p->stmt()));
	newblock->stmts().push_back (breaklabel);
	newblock = (blockNode *) LoopDismantleChanger::fix_break_continue 
	  (newblock, breaklabel, NULL, false);
	return newblock;
}

Node * SelectionDismantleChanger::at_case (caseNode *n, Order) {
	return n->stmt();
}

/*

Apply the following transformations:
if (!a) b;
=>
if (a) ; else b;

if (a && b) c;
=>
if (a) if (b) c;

if (a || b) c;
=>
if (a) goto l;
if (b) {
l: 	c;
}

if (a) b; else c;
=>
if (a) 
{ 
	b; 
	goto l; 
}
c;
l: ;

*/

Node * IfConverterChanger::at_if_noelse (ifNode *n, Order) {
	exprNode *e = n->expr();
	if (e->typ() == Unary) {
		unaryNode *u = (unaryNode *) e;
		if (u->op()->id() == '!') {
			// change 
			//    if (!a) c; 
			// into
			//    if (a) ; else c;

			// if this if is of the form 'if(...)goto'
			// then don't change it; it is probably from
			// an earlier phase that may have been trying to
			// bias the branch, and we don't want to change
			// the bias here.  we may change this in a later
			// update if we decide it's stupid, so phases
			// shouldn't count on this behavior.

			//blockNode *b = (blockNode *) n->stmt();
			//if (b && b->stmts().size()) {
			//	stmtNode *bn = b->stmts().front();
			//	if (bn->typ() == Goto) return n;
			//}
			// ok, we've decided that this is stupid.
			// later optimizations make it futile.
			// if someone really wants to bias branches,
			// they should do it as a post-processing phase
			// after optimization (and good luck to them once
			// all the contextual information is gone!).

			ifNode *ie = new ifNode 
				(u->expr(), clone_empty_stmt(), n->stmt(), n->coord());
			change = true;
			return ie;
		}
	} else if (e->typ() == Binary) {
		binaryNode *b = (binaryNode *) e;
		if (b->op()->id() == Operator::ANDAND) {
			// change
			//    if (a && b) c;
			// into
			//    if (a) if (b) c;
			ifNode *i2 = new ifNode (b->right(), n->stmt(), 0, n->coord());
			ifNode *i = new ifNode (b->left(), i2, 0, n->coord());
			i->annotations().push_back 
				(new fromAnnote (FROM_AND1));
			i2->annotations().push_back 
				(new fromAnnote (FROM_AND2));
			change = true;
			return i;
		} else if (b->op()->id() == Operator::OROR) {
			// change
			//    if (a || b) c;
			// into
			//    if (a) goto l;
			//    if (b) {
			//      l: 	c;
			//    }

			// make the label for l
			idNode *l = new_id ("OR");
			l->coord(n->coord());
			labelNode *L = new labelNode (l, clone_empty_stmt());

			// make the first if stmt

			gotoNode *g = new gotoNode(L, n->coord());
			ifNode *first_if = new ifNode (b->left(), g, 0, n->coord());

			// make the second if stmt

			blockNode *bl = new blockNode (NULL, NULL, n->coord());
			bl->stmts().push_back (L);
			bl->stmts().push_back (n->stmt());
			ifNode *second_if = new ifNode (b->right(), bl, 0, n->coord());
			first_if->annotations().push_back 
				(new fromAnnote (FROM_OR1));
			second_if->annotations().push_back 
				(new fromAnnote (FROM_OR2));

			// put them together into a block node

			blockNode *r = new blockNode (NULL, NULL, n->coord());
			r->stmts().push_back (first_if);
			r->stmts().push_back (second_if);
			change = true;
			return r;
		}
	}
	return n;
}

Node * IfConverterChanger::at_if (ifNode *n, Order o) {
	// convert
	//    if (a) b; else c;
	// to
	//    if (a) { b; goto l; }
	//    c;
	//    l: ;

  // Old if statement (with no else)
  if (! n->false_br())
    return at_if_noelse(n, o);

	idNode *l = new_id ("IC");
	l->coord(n->coord());
	labelNode *L = new labelNode (l, clone_empty_stmt());
	gotoNode *g = new gotoNode (L, n->coord());
	blockNode *bl = new blockNode (NULL, NULL, n->coord());
	bl->stmts().push_back (n->true_br());
	bl->stmts().push_back (g);
	ifNode *i = new ifNode (n->expr(), bl, 0, n->coord());
	i->annotations().push_back (new fromAnnote (FROM_IFELSE));
	blockNode *r = new blockNode (NULL, NULL, n->coord());
	r->stmts().push_back (i);
	r->stmts().push_back (n->false_br());
	r->stmts().push_back (L);
	return r;
}
