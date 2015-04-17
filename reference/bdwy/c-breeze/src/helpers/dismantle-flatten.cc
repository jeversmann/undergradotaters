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
#include "semcheck.h"

// for the unusedVariableCleanupChanger, we include dead.h;
// doing actual dead code elimination at this point would be
// too costly.

#include "dead.h"
#include "dismantle.h"

class discard_const_walker : public Walker {
public:
	discard_const_walker (void) : Walker (Preorder, Subtree) { }

	void at_type (typeNode *t, Order) {
		typeNode::Type_qualifiers tq = t->type_qualifiers();
		if (tq & typeNode::CONST)
			t->type_qualifiers(typeNode::NONE);
	}
};

Node * FlattenDismantleChanger::at_proc (procNode *p, Order ord) {
       	if (ord != Postorder) return p;
	unusedVariableCleanupChanger uvc;
       
        int count = 0;
	unreachable = false;
	blockNode *body = p->get_body();
	blockNode *newblock = new blockNode (NULL, NULL);
	newblock->stmts().push_back (body);
	body = new blockNode (NULL, NULL);
	p->body (body);
        count = 0;
         flatten_block (p->body(), newblock);
         for (stmt_list_p q=body->stmts().begin();
             q!=body->stmts().end(); ) {
          // remove empty blocks and empty stmts
          if ((!*q || (((*q)->typ() == Expr)
                       && ((exprstmtNode*)(*q))->expr() == NULL))
              || (*q)->typ() == Block){
                  q = body->stmts().erase (q);
                  count ++;
          }
          else q++;
        }
         if (count > 1){
                
                 iterateflag = true;
         }

        // run the unused variable changer and that removes unused variables
        p->change (uvc);
        return p;
}

// find the target of a goto stmt in r, returning end if there's a problem.

stmt_list_p FlattenDismantleChanger::search_target (stmt_list_p r, gotoNode *g, stmt_list_p end) {
        for (; r != end; r++) {
        		if ((*r)->typ() == Label) {
			labelNode *l = (labelNode *) *r;
			if (l->name() == g->name()) return r;
		}
	}
	return end;
}

// replace all occurences of the label in 'search' with that in 'replace'

void FlattenDismantleChanger::search_and_replace_labels
	(stmt_list stmts, stmt_list_p search, stmt_list_p replace) {
	labelNode *s = (labelNode *) * search;
	labelNode *r = (labelNode *) * replace;
	for (stmt_list_p p=stmts.begin(); p!=stmts.end(); p++) {
		gotoNode *g = NULL;
		if ((*p)->typ() == Goto)
			g = (gotoNode *) *p;
		else if ((*p)->typ() == If) {
			blockNode *b = ((ifNode *) *p)->stmt();
			assert (b->stmts().front()->typ() == Goto);
			g = (gotoNode *) b->stmts().front();
		} else if ((*p)->typ() == Block) {
			blockNode *b = (blockNode *) *p;
			search_and_replace_labels (b->stmts(), search, replace);
			continue;
		} else continue;
		if (g->name() == s->name()) {
			g->name (r->name());
			g->label (r);
		}
	}
	(*labels)[s->name()] = false;
	(*labels)[r->name()] = true;
}

void FlattenDismantleChanger::flatten_block (blockNode *top, blockNode *b) {
                for (decl_list_p p=b->decls().begin();
		p!=b->decls().end(); p++) {
		top->decls().push_back (*p);
	}
	b->decls().clear();
	for (stmt_list_p r=b->stmts().begin();
             r!=b->stmts().end(); r++) {
                stmt_list_p s = r;

		// if we find a label, code at this point is reachable
		// (or at least harder to prove it's not reachable)

		if ((*r)->typ() == Label) {
                         map <string,bool>::iterator pp;
                         labelNode * lab = ((labelNode *) *r);
                         pp = (*labels).find(lab->name());
                         if (pp == (*labels).end()){
                                 unreachable = true;
                         }
                         else
                                 unreachable = false;

			// if we have two labels next to each other, get rid of one.

			s++;
			if (s != b->stmts().end()) {
			  /*
				if ((*s)->typ() == Label) {
					unreachable = true;
					search_and_replace_labels (b->stmts(), r, s);
					search_and_replace_labels (top->stmts(), r, s);
				}
			  */
			}
		}

		// if this stmt is a goto and it's target is the next
		// stmt, that's pretty stupid, so we'll get rid of the
		// goto by claiming that's it's unreachable.
		// This is a lie, but the label will toggle the unreachable
		// flag on the next iteration, so it works.

		else if ((*r)->typ() == Goto) {
			gotoNode *g = (gotoNode *) *r;
			s++;

			// if nobody is home at *s, we goto out
			// of this mess.

			if (s == b->stmts().end()) goto out_of_if;
			if ((*s)->typ() == Label
			&& (g->name() == ((labelNode *)*s)->name()))
				unreachable = true;
			else {

				// make a jump to a jump into just a jump

				s = search_target (b->stmts().begin(),
					(gotoNode *) *r, b->stmts().end());
				if (s == b->stmts().end()) goto out_of_if;
				s++;
				if (s != b->stmts().end()
				&& (*s)->typ() == Goto) {
					g->name(((gotoNode *)*s)->name());
					g->label(((gotoNode *)*s)->label());
				}
			}

		// make:
		// if (foo) goto L1;
		// ...
		// L1: goto L2;
		// into
		// if (foo) goto L2;

		} else if ((*r)->typ() == If) {
			ifNode *g = (ifNode *) *r;
			blockNode *b1 = (blockNode *) g->stmt();
			stmtNode *a = b1->stmts().front();
			// dismantled code should only have if/goto
			assert (a->typ() == Goto);
			gotoNode *c = (gotoNode *) a;
			s = search_target (b->stmts().begin(),
				c, b->stmts().end());
			if (s == b->stmts().end()) goto out_of_if;
			s++;
			if (s != b->stmts().end() && (*s)->typ() == Goto) {
                          
                                iterateflag = true;
				c->name(((gotoNode *)*s)->name());
				c->label(((gotoNode *)*s)->label());
			}
		// check to see if we can do a simple copy prop and
		// dead code elim on a copy/if pair
		// this is a horrible hack, looking for cases like
		// __Itmp = foo;
		// if (__Itmp) blah...
		// and making it into
		// if (foo) blah...
		// why not just wait to do a copyprop and dead code elim
		// pass?  by the time we get there, we've had to do an
		// analysis involving __Itmp that may have had to be slow 
		// because of the proliferation of such decls.
		// yes, I know this is spaghetti code.  don't maintain
		// it; just rewrite it :-)

		} 
#ifdef HACKY
		else if ((*r)->typ() == Expr) {
			s++;
			if (s == b->stmts().end()) goto out_of_if;
			if ((*s)->typ() != If) goto out_of_if;
			ifNode *ifn = (ifNode *) *s;
			bool is_if_copy = false;
			binaryNode *bi;
			idNode *n;
			exprNode *e = ((exprstmtNode *) (*r))->expr();
			if (!e) goto out_of_if;
			if (e->typ() == Binary) {
				bi = (binaryNode *) e;
				if (bi->left()->typ() == Id 
					&& bi->right()->typ() == Id) {
					n = (idNode *) bi->left();
					const char *p = n->name().c_str();
					if (strncmp (p, "__I", 3) == 0)
						is_if_copy = true;
				}
			}
			if (is_if_copy) {
				// replace the if condition with the 	
				// rhs of the copy

				ifn->expr (bi->right());
				// get rid of the copy

				b->stmts().erase (r);

				// point r at the if stmt so it
				// can be incremented later

				r = s;
			}
		}
#endif
        out_of_if:
                if (unreachable){ // DEAD CODE ELIMINATION REQUIRED
                        // set the flag to true so that the iteration continues atleast two more times
                        
                        iterateflag = true;
                  
                } else {
			top->stmts().push_back (*r);
		}
		if ((*r)->typ() == Goto){
			unreachable = true;
                }
		if ((*r)->typ() == Block)
			flatten_block (top, (blockNode *) *r);
	}
	b->stmts().clear();
}

// we do this quick transformation, since this is a convenient place

Node * FlattenDismantleChanger::at_unary (unaryNode *p, Order ord) {
	// & (* p) => p
	if (ord != Postorder) return p;
	if (p->op()->id() == Operator::ADDRESS) {
		if (p->expr()) if (p->expr()->typ() == Unary) {
			unaryNode *expr = (unaryNode *)(p->expr());
			if (expr->op()->id() == Operator::INDIR) {
				return expr->expr();
			}
		}
	} else if (p->op()->id() == Operator::INDIR) {
	// * (& p) => p
		if (p->expr()) if (p->expr()->typ() == Unary) {
			unaryNode *expr = (unaryNode *)(p->expr());
			if (expr->op()->id() == Operator::ADDRESS) {
				return expr->expr();
			}
		}
	}
	return p;
}

Node * FlattenDismantleChanger::at_label (labelNode *p, Order ord) {
	if (!(*labels)[p->name()]) return p->stmt();
	return p;
}









