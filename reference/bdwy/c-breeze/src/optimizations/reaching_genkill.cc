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
#include "reaching_getdefs.h"

// get all stmts that are definitions.
// defines, provided as an empty map to the constructor, 
// will on exit map statements to the declarations of the variables 
// they define.

// return true if an expression contains a procedure call

bool GetDefsWalker::contains_call (exprNode *e) {
	if (!e) return false;
	switch (e->typ()) {
		case Call: return true;

		// search the left and right operands

		case Binary: {
			binaryNode *b = (binaryNode *) e;
			return contains_call (b->left()) 
			|| contains_call (b->right());
		}

		// search the operand

		case Unary: {
			unaryNode *u = (unaryNode *) e;
			return contains_call (u->expr());
		}

		// search the casted expression

		case Cast: {
			castNode *c = (castNode *) e;
			return contains_call (c->expr());
		}

		// none of these is a procedure call

		case Id:
		case Const:
		default: 
			return false;
	}
}

// find out whether and what an expression statement defines

void GetDefsWalker::handle_exprstmt (exprstmtNode *s) {
	exprNode *e = s->expr();
	if (!e) return;
	if (contains_call (e)) {
		ambiguous_defs->push_back (s);
		// don't return from here just yet;
		// this statement might define
		// even more stuff after the call
	}
	if (e->typ() == Binary) {
		binaryNode *b = (binaryNode *) e;
		if (b->op()->id() == '=') {
			exprNode *l = b->left();
			if (l->typ() == Unary) {
				unaryNode *u = (unaryNode *) l;
				if (u->op()->id() == Operator::INDIR) {

					// a pointer dereference is
					// an ambiguous definition

					ambiguous_defs->push_back (s);
					return;
				}
			} else if (l->typ() == Binary) {
				binaryNode *p = (binaryNode *) l;
				if (p->op()->id() == Operator::ARROW) {

					// this is another way to do
					// a pointer dereference

					ambiguous_defs->push_back (s);
					return;
				}
				if (p->op()->id() == Operator::Index) {
				  // an ambiguous ref for all intents
				  // and purposes
				  
				  ambiguous_defs->push_back (s);
				  return;
				}
			} else if (l->typ() == Id) {

				// id = something defines this id

				idNode *i = (idNode *) l;
				(*defines)[s] = i->decl();
			} else {
				// yikes, don't know what this is,
				// but I'm going to say it's an
				// ambiguous def conservatively.
				// this actually happens in 147.vortex
				//cerr << "unknown lvalue type? " << l->typ() << '\n';
				ambiguous_defs->push_back (s);
				return;
			}
		}
	}
}

void GetDefsWalker::at_stmt (stmtNode *s, Order) {

	// if the statement is an expression statement, check
	// it for definitions

	if (s->typ() == Expr) {
		exprstmtNode *e = (exprstmtNode *) s;
		handle_exprstmt (e);
	}
}

reachingGenKillWalker::reachingGenKillWalker (
	// node2num - will map definition statements 
	// to bit positions in flow values

	map <stmtNode *, int> *n2n, 

	// defines - will map definition statements
	// to the variables (declNode's) they define

	map <stmtNode *, declNode *> *dfns, 

	// defs - will map a variable to a flow value
	// representing the set of all definitions
	// for that variable

	map <declNode *, defFlowVal *> *dfs,

	// num2node - will be an array mapping bit
	// positions (integers) to definition
	// statements

	stmtNode ***num2n,
	int *numdefs) : 
	Walker (Preorder, Subtree), 
	node2num(n2n),
	defines(dfns),
	defs(dfs),
	pn2n (num2n),
	pn (numdefs),
	_flowval(NULL),
	num2node(NULL) { }

reachingGenKillWalker::~reachingGenKillWalker (void) { }

void reachingGenKillWalker::at_proc (procNode *p, Order) {
	int	i;

	// get the definitions from this procedure

	defines->clear();
	GetDefsWalker gdw (defines, & ambiguous_defs);
	p->walk (gdw);

	// get a list of all the variables defined

	decl_list vars;
	map <stmtNode *, declNode *>::iterator r;
	for (r=defines->begin(); r!=defines->end(); r++)
		vars.push_back ((*r).second);
	vars.sort();
	vars.unique();

	// n will be the length of a bit vector (minus 1)

	n = defines->size() + vars.size();
	*pn = n;

	// num2node will map bit positions to statements.
	// bit positions go from 1..n, since 0
	// might be returned by the map on an unrecognized
	// statement

	num2node = new stmtNode *[n + 1];
	*pn2n = num2node;

	// get an empty flow value we can clone later

	_flowval = new defFlowVal (n, node2num, num2node);
	_flowval->to_top();

	// map stmtNode's to bit positions
	// and vice versa; also, populate defs[]

	// first, unambiguous definitions

	for (i=1,r=defines->begin(); r!=defines->end(); i++,r++) {

		// the bit position of this definition is i

		(*node2num)[(*r).first] = i;

		// the i'th bit position represents this definition

		num2node[i] = (*r).first;

		// for the variable declaration (*p).second,
		// defs[(*r).second] will contain all
		// its definitions

		defFlowVal *v = (*defs)[(*r).second];

		// no such flow val?  make one.

		if (v == NULL) {
			v = (defFlowVal *) _flowval->clone();
			(*defs)[(*r).second] = v;
		}
		v->insert (i);
	}

	// now, ambiguous definitions; we'll make up one for
	// each variable

	_everything = (defFlowVal *) _flowval->clone();
	decl_list_p q;
	for (q=vars.begin(); q!=vars.end(); q++, i++) {

		// make a new statement that will represent
		// "the" ambiguous definition of this variable

		exprstmtNode *dummy = new exprstmtNode (NULL);

		// set up the mapping between bit positions
		// and the stmtNode *

		(*node2num)[dummy] = i;
		num2node[i] = dummy;

		// this ``definition'' defines this variable

		(*defines)[dummy] = *q;

		// get the set of definitions for this variable

		defFlowVal *v = (*defs)[*q];

		// this should have been created already

		assert (v);

		// insert this ambiguous definition

		v->insert (i);

		// also insert it into _everything; at the end,
		// _everything will be a set, for each variable,
		// of "the" ambiguous definition for that variable.

		_everything->insert (i);
	}

	// if the first block has a label, we need a preheader
	// to receive gen for the parameters and globals

	basicblockNode *b = (basicblockNode *) p->body()->stmts().front();
	if (b->stmts().size() && b->stmts().front()->typ() == Label) {

		// make an empty preheader for the procedure

		b = new basicblockNode (NULL, NULL);
		b->comment() = "preheader to generate parameters and globals";
	
		// link it to the first block in the procedure
	
		((basicblockNode *)(p->body()->stmts().front()))->
			preds().push_back (b);
		b->succs().push_back ((basicblockNode *) 
			p->body()->stmts().front());
		p->body()->stmts().push_front (b);
	}

	// make gen and kill sets for each statement,
	// giving them initially empty gen/kill sets

	stmt_list_p t;
	for (t=p->body()->stmts().begin(); t!=p->body()->stmts().end(); t++) {
		b = (basicblockNode *) *t;
		stmt_list_p r;
		for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
			defFlowVal *gen = (defFlowVal *) _flowval->clone();
			defFlowVal *kill = (defFlowVal *) _flowval->clone();
			(*r)->gen (gen);
			(*r)->kill (kill);
		}
	}

	// for every statement that is an ambiguous definition,
	// it generates everything

	for (t=ambiguous_defs.begin(); t!=ambiguous_defs.end(); t++) {
		defFlowVal *v = (defFlowVal *) (*t)->gen();
		assert (v);
		v->Union (_everything);
	}

	// let at_basicblock take care of the rest
} 

void reachingGenKillWalker::at_basicblock (basicblockNode *b, Order) {
	stmt_list_p r;

	defFlowVal *gen;
	defFlowVal *kill;
	// make gen and kill for each statement

	for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {

		// d is the definition (or just a statement)

		stmtNode *d = *r;

		// gen and kill are new flowvals, initially empty

		gen = (defFlowVal *) d->gen();
		kill = (defFlowVal *) d->kill();

		// v is the variable defined by definition d.

		declNode *v = (*defines)[d];
		if (v == NULL) {

			// nothing to do; d is not a definition,
			// or may be ambiguous and thus not
			// in defines.

		} else {

			// definition d defines variable v
			// all definitions of v are killed

			kill->Union ((*defs)[v]);

			// except for this one!

			kill->remove (d);

			// this definition gens itself

			gen->insert (d);
		}
	}

	// gen and kill are defined for each statement.
	// put them all together for gen and kill
	// for this block.

	gen = (defFlowVal *) _flowval->clone();
	kill = (defFlowVal *) _flowval->clone();

	// the (pre)header should be seen to generate all ambiguous
	// definitions, so that parameters and global variables are 
	// seen with definitions reaching into the procedure

	if (b->preds().size() == 0) gen->Union (_everything);

	for (r=b->stmts().begin(); r!=b->stmts().end(); r++) {
		stmtNode *d = *r;

		// from Dragon book:
		// gen[S] = gen[S_2] U (gen[S_1] - kill[S_2])
		// kill[S] = kill[S_2] U (kill[S_1] - gen[S_2])
		// we think of the basic block as seen so far
		// as S_1, and the current statement (d) as S_2,
		// with the resulting current basic block so far
		// as S.

		gen->Difference (d->kill());
		gen->Union (d->gen());
		kill->Difference (d->gen());
		kill->Union (d->kill());
	}
	b->gen (gen);
	b->kill (kill);
}

defFlowVal *reachingGenKillWalker::new_flowval (void) {
	defFlowVal *f = (defFlowVal *) _flowval->clone();
	f->to_top();
	return f;
}

void defFlowVal::to_top (void) {
	bits->reset_all ();
}

FlowVal * defFlowVal::clone (void) const {
	defFlowVal *other = new defFlowVal (size, def2num, definitions);
	other->bits = bits->clone();
	return other;
}

bool defFlowVal::diff (FlowVal *other) const {
	defFlowVal *that = (defFlowVal *) other;
	for (int i=1; i<=size; i++)
		if (that->bits->value(i) != bits->value(i)) 
			return true;
	return false;
}

defFlowVal::defFlowVal (int n, map<stmtNode *, int> *m, stmtNode **d) :
	size (n),
	def2num (m),
	definitions (d),
	bits (new Bits (n+1)) { to_top (); }

defFlowVal::~defFlowVal (void) {
	delete bits;
}

bool defFlowVal::in (stmtNode *s) {
	return bits->value((*def2num)[s]);
}

bool defFlowVal::in (int i) {
	return bits->value (i);
}

void defFlowVal::insert (stmtNode *s) {
	bits->set ((*def2num)[s], true);
}

void defFlowVal::insert (int i) {
	bits->set (i, true);
}

void defFlowVal::remove (int i) {
	bits->set (i, false);
}

void defFlowVal::remove (stmtNode *s) {
	bits->set ((*def2num)[s], false);
}

void defFlowVal::copy (FlowVal *other) {
	bits->copy (((defFlowVal *)other)->bits);
}

void defFlowVal::Union (const FlowVal *v) {
	defFlowVal *w = (defFlowVal *) v;
	bits->Or (w->bits);
}

void defFlowVal::Intersect (const FlowVal *v) {
	defFlowVal *w = (defFlowVal *) v;
	bits->And (w->bits);
}

// this -= that

void defFlowVal::Difference (FlowVal *other) {
	defFlowVal *that = (defFlowVal *) other;
	Bits comp(size+1);
	comp.copy (that->bits);
	comp.Not ();
	bits->And (&comp);
}

void defFlowVal::meet (const FlowVal *v) {
	Union (v);
}
