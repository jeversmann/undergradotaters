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
// dismantle-init.cc
//
// Dismantle "normal" initializers, like int a = 1;, into uninitialized
// declarations and assignment statements.
//
#include <stdio.h>
#include "c_breeze.h"

#include "dismantle.h"

Node * InitializerDismantleChanger::at_block (blockNode *p, Order) {

	// we'll keep the assignments in this block so we can keep
	// them in order; we may have some genius depend on the
	// order of something like int a = 1, b = a;
	// shouldn't work, but we'll be safe anyway

	blockNode *newinnerblock = NULL;

	for (decl_list_p i=p->decls().begin(); i!=p->decls().end(); i++) {
		// could be Text!
		if ((*i)->typ() != Decl) continue;
		exprNode *init = (*i)->init();
		// the initializer may be one of those aggregate ones.
		// we'll ignore them since they can't be easily initialized
		// in C; they're required to be constant expressions
		// by the standard anyway, so moving the declarations
		// anyway later won't hurt.
		if (!init || init->typ() == Initializer) continue;
		// a static initializer must be constant and can't
		// be transformed to an executable statement, since
		// statics are only supposed to be initialized once.
		// so we ignore it.
		if ((*i)->storage_class() == declNode::STATIC) continue;
		if (init) {
			(*i)->init(NULL);
			idNode *id = new idNode ((*i)->name().c_str());
			id->decl (*i);
			binaryNode *assg = 
				new binaryNode ('=', id, init);
			if (!newinnerblock) {

				// ok, we're gonna make this block
				// to hold initializing stmts.

				newinnerblock = new blockNode (NULL, NULL);
#ifdef I_FORGOT_WHY_I_IFDEFED_THIS_OUT_BUT_IT_MUST_BE_A_GOOD_REASON
				// if the current block has labels at the
				// front, we want to take them out and
				// put them at the front of the new
				// block, so control will always reach	
				// the initializing stmts

				while (!(p->stmts().empty())
				&& (p->stmts().front()->typ() == Label)) {
					newinnerblock->stmts().push_back
						(p->stmts().front());
					p->stmts().pop_front();
				}
#endif
			}
			newinnerblock->stmts().push_back
				(new exprstmtNode (assg));
		}
	}
	if (newinnerblock) p->stmts().push_front(newinnerblock);
	return p;
}

// we want all labels to label empty statements
               
Node * InitializerDismantleChanger::at_label (labelNode *l, Order) {
	// if label already labels an empty statement, leave it alone.
	bool empty = true;
	blockNode *b = l->stmt();
	for (stmt_list_p p=b->stmts().begin(); 
		empty && p!=b->stmts().end(); p++) {
		if ((*p)->typ() != Expr) empty = false;
		else if (((exprstmtNode *)(*p))->expr()) empty = false;
	}
	if (empty) return l;
        blockNode *newblock = new blockNode (NULL, NULL);
        stmtNode *s = l->get_stmt();
        l->stmt(clone_empty_stmt());
        newblock->stmts().push_back (l);
        newblock->stmts().push_back (s);
        return newblock;                             
}                                            
