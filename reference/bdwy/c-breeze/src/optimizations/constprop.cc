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
// global constant propagation using reaching definitions info
//

#include "c_breeze.h"
#include "semcheck.h"
#include "bits.h"
#include "reaching_genkill.h"
#include "reaching.h"
#include "constprop.h"

Node *constantPropChanger::at_id (idNode *i, Order) {
	constant *value = NULL;

	// get the ud chain for this node

	udChainAnnote *ud = udChainAnnote::getUdChain (i);

	// no ud chain?  never mind.

	if (!ud) return i;

	// no definitions?  it could happen.

	if (ud->ud_chain().size() == 0) return i;

	// do all definitions reaching this use
	// define it as the same constant?

	stmt_list_p q;
	for (q=ud->ud_chain().begin(); q!=ud->ud_chain().end(); q++) {

		stmtNode *s = *q;

		// if it's not NULL (i.e., ambiguous)

		if (!s) return i;

		// make sure its an expression statement

		assert (s->typ() == Expr);
		exprstmtNode *ex = (exprstmtNode *) s;

		// with a non-null expression

		assert (ex->expr());

		// that is a binaryNode

		assert (ex->expr()->typ() == Binary);
		binaryNode *b = (binaryNode *) ex->expr();

		// that is an assignment

		assert (b->op()->id() == '=');

		// to an identifier

		if (b->left()->typ() != Id) return i;
		idNode *lhs = (idNode *) b->left();

		// whose declaration is the same as this use

		if (lhs->decl() != i->decl()) return i;

		// we're pretty sure we've got the only
		// definition of this use reaching here;
		// let's see if it would be appropriate
		// to replace this use with the rhs

		exprNode *rhs = b->right();
		if (rhs->typ() == Const) {
			if (value == NULL) {
				value = & rhs->value();
			} else if (!(value->is_equal_to (rhs->value()))) {
				return i;
			}
		} else 
			// not a constant?  can't have that.
			return i;
	}
	// at this point, every definition of i is the
	// same constant value; we'll return a constNode
	// with that value.
	changed = true;
	return new constNode (*value);
}

Node * constantFoldingChanger::at_binary (binaryNode *b, Order) {
	switch (b->op()->id()) {
	case '*': case '/': case '%': case '+': case '-':
	case Operator::LS: case Operator::RS: case Operator::LE: case Operator::GE:
	case '<': case '>': case Operator::EQ: case Operator::NE: 
	// exclude bit operations - they're not portable
	//case '&': case '|': case '^': 
		break;
	default: 
		return b;
	}
	if (b->left()->typ() == Const && b->right()->typ() == Const) {
		semcheck_expr_visitor::check(b);
		constNode *c1 = (constNode *) b->left();
		constNode *c2 = (constNode *) b->right();
		constant oper1 = c1->value();
		constant oper2 = c2->value();
		if (oper1.basic() == oper2.basic()) {
			constant c = constant::eval (b->op(), oper1, oper2);
			if (c.no_val() == false) {
				changed = true;
				return new constNode (c);
			}
		}
	}
	return b;
}

Node *constantFoldingChanger::at_unary (unaryNode *u, Order) {
	return u;
}

class constantPropPhase : public Phase {
public:

void run () {
	unit_list_p n;
	constantPropChanger cpc;
	constantFoldingChanger cfc;

	for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
		(*n)->change (cfc);
	for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
		(*n)->change (cpc);
	for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
		(*n)->change (cfc);

}
};

Phases constant_prop_phase("constprop", new constantPropPhase());
