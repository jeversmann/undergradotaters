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
// cfg.cc
//

#include <stdio.h>
#include "c_breeze.h"
#include "dismantle.h"
#include "goto_label_walker.h"
#include "cfg.h"

// At a procedure, do this:
// 1. Separate the dismantled code into basic blocks so that the
// body() of the procNode is a blockNode whose stmts() is a list of
// basicblockNode's.
//
// 2. Find the preds and succs of each basic block to create a control
// flow graph.
//
// So at the end of this function, the procedure will have a control
// flow graph with a source at the first stmt in body()->stmts(),
// and a sink (return) at the last stmt in body()->stmts().  This relies
// on the dismantler having changed all return stmts into goto's to
// the single return stmt at the end of the procedure.
//

class unreachableCodeRemover : public Walker {
private:
	map<stmtNode *,bool> marked;

public:
	unreachableCodeRemover (void) : Walker (Preorder, Subtree) { }

	void dfs (basicblockNode *n) {
		basicblock_list_p p;

		marked[n] = true;
		for (p=n->succs().begin(); p!=n->succs().end(); p++) {
			if (!marked[*p]) dfs (*p);
		}
	}

	void fixup_preds (basicblockNode *b) {
		basicblock_list_p p;

		for (p=b->preds().begin(); p!=b->preds().end(); ) {
			if (marked[*p])
				p++;
			else
				p = b->preds().erase (p);
		}
	}

	void at_proc (procNode *p, Order) {
		marked.clear();
		blockNode *b = p->body();
		stmtNode *s = b->stmts().front();
		assert (s->typ() == Block);
		basicblockNode *bb = (basicblockNode *) s;
		dfs (bb);
		for (stmt_list_p q = b->stmts().begin(); 
			q!=b->stmts().end(); ) {
			if (!marked[*q]) {
				q = b->stmts().erase (q);
			} else {
				assert ((*q)->typ() == Block);
				fixup_preds ((basicblockNode *) *q);
				q++;
			}
		}
	}
};

void cfg_changer::generate_cfg (unitNode *u, unsigned int flags) {
	cfg_changer cfgc;
	unreachableCodeRemover urcrm;
	Dismantle::dismantle (u, flags);
	u->change (cfgc);
	u->walk (urcrm);
	//goto_label_walker::fixup (u);
}

Node * cfg_changer::at_proc (procNode *p, Order) {
	blockNode *b = p->body();
	bool unreachable = false;

	// place stmts in here to grow a basic block

	basicblockNode *bb = new basicblockNode (NULL, NULL);

	// for comments, we'll number the basic blocks

	int block_id = 1;

	// remember which block has which id

	map <basicblockNode *, int> id_map;

	// map from (goto) label strings to the block nodes
	// where they are defined.

	map <string, basicblockNode *> goto_label_map;
	basicblock_list_p q;

	// separate stmts into basic blocks

#define EMIT_BB() { \
	if (bb->stmts().size()) basic_blocks.push_back (bb); \
		bb = new basicblockNode (NULL, NULL); }

	basicblock_list basic_blocks;

	// -- Special case: If the first basic block starts with a label,
	// then we need a dummy "entry" basic block. Otherwise, this first
	// basic block will appear to have only one predecessor: the back
	// edge. That will confuse the SSA algorithm.

	if (b->stmts().front()->typ() == Label) {
	  bb->stmts().push_back(new exprstmtNode((exprNode *)0));
	  EMIT_BB();
	}

	for (stmt_list_p t=b->stmts().begin(); t!=b->stmts().end(); t++) {
		stmtNode *s = *t;

		// if we come upon a label, it needs to go into a new
		// basic block

		if (s->typ() == Label) {
			EMIT_BB();
			unreachable = false;
		}

		// stick the current statement into the current basic block

		//if (!unreachable)
			bb->stmts().push_back (s);

		// if we come upon if, goto, or return, this is the end
		// of this basic block

		if (s->typ() == If) {
			stmt_list_p q = t;
			q++;
			if (q != b->stmts().end() && (*q)->typ() == Goto) {
				t++;
				s = *t;
				bb->stmts().push_back (s);
				unreachable = true;
			}
			EMIT_BB();
		} else  if (s->typ() == Goto 
			|| s->typ() == Return) {
			EMIT_BB();
			if (s->typ() == Goto) unreachable = true;
		}
	}

	// emit any stmts not yet emitted (this shouldn't be necessary,
	// since a return should end the function, but better safe than
	// sorry).

	EMIT_BB();

	// place blocks back into function body, numbering them with
	// unique (to this procedure) ids

	b->stmts().clear();
	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {
		id_map[*q] = block_id++;
		b->stmts().push_back (*q);
	}

	// find preds and succs from if's that fall through

	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {

		// if the last stmt in bb *q (get it?  BBQ?)
		// isn't an unconditional jump, then the next bb is 
		// a successor

		stmtNode *r = (*q)->stmts().back();
		if (r->typ() != Goto && r->typ() != Return) {
			basicblock_list_p s = q;
			s++;
			if (s != basic_blocks.end()) {
				(*s)->preds().push_front (*q);
				(*q)->succs().push_front (*s);
			}
		}
	}

	// find preds and succs from goto's and labels
	// first, make a map from labels to basic blocks

	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {
		stmtNode *r = (*q)->stmts().front();
		if (r->typ() == Label) 
			goto_label_map[((labelNode *)r)->name()] = *q;
	}

	// now, for any [if] goto stmt, the containing basic block
	// is a pred of the labeled stmt, and the labeled stmt
	// is a succ of the basic block

	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {

		// go through all the stmts in the block looking
		// for ifs and gotos.  Note: they should only appear
		// around the end, but this makes it more general
		// in case we want to start messing with "superblocks."

		for (stmt_list_p s=(*q)->stmts().begin();
			s != (*q)->stmts().end(); s++) {
			stmtNode *r = *s;
			bool is_if = false;

			// if we find an 'if', get a pointer to the
			// corresponding 'goto'.

			if (r->typ() == If) {
				is_if = true;
				ifNode *g = (ifNode *) r;
				blockNode *b = (blockNode *) g->stmt();
				r = b->stmts().front();
	
				// dismantled code should only have if/goto

				assert (r->typ() == Goto);
			}
			if (r->typ() == Goto) {
			        procNode * proc = p;
				basicblockNode *p = 
					goto_label_map[((gotoNode *)r)->name()];
	
				// the label should be defined in the function
				if (!p) {
				  cerr << "No label for goto " << ((gotoNode *)r)->name() << endl;
				  cerr << "  at " << proc->decl()->name() << endl;
				}
	
				assert (p);

				// if this came from an if, make
				// it the second succ,
				// else make it the first

				if (is_if)
					(*q)->succs().push_back (p);
				else
					(*q)->succs().push_front (p);
				p->preds().push_back (*q);
			}
		}
	}

	// uniquify each list of preds and succs

	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {
		(*q)->succs().sort();
		(*q)->succs().unique();
		(*q)->preds().sort();
		(*q)->preds().unique();
	}

	// insert comments into each basic block about its preds and succs

	for (q=basic_blocks.begin(); 
		q!=basic_blocks.end(); q++) {
		string s("");
		char c[100];
		sprintf (c, "%d", id_map[*q]);
		s += "# ";
		s += c;
		s += ": preds( ";
		(*q)->comment() = s;
		for (basicblock_list_p l=(*q)->preds().begin();
			l!=(*q)->preds().end(); l++) {
			sprintf (c, "%d", id_map[*l]);
			s += c;
			s += " ";
		}
		s += ") succs( ";
		for (basicblock_list_p l=(*q)->succs().begin();
			l!=(*q)->succs().end(); l++) {
			sprintf (c, "%d", id_map[*l]);
			s += c;
			s += " ";
		}
		s += ")";
		(*q)->comment() = s;
	}
	return p;
}
