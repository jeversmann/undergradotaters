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

#ifndef CBZ_DEAD_H
#define CBZ_DEAD_H

/** @brief Performs global (intraprocedural) dead-code elimination
    @ingroup Phases
*/
class deadCodeEliminationChanger : public Changer {
private:

	map <declNode *, bool> is_local;
	map <declNode *, bool> is_live;

	bool has_side_effects (exprNode *);
	void mark_as_live (exprNode *);
	exprNode * handle_expr (exprNode *);
	void local_dce (basicblockNode *);
	void tick_local (declNode *);
	void find_address_taken (exprNode *);

public:
	deadCodeEliminationChanger (void) : Changer (Preorder, Subtree, false) { }
	Node * at_proc (procNode *, Order);
};

/** @brief determines is subtree contains a node that uses a struct def
    @ingroup Phases
 */
class has_struct_walker : public Walker {
public:
	bool has_struct;

	has_struct_walker (void) : 
		Walker (Preorder, Subtree), has_struct(false) { }

	void at_decl (declNode *d, Order) {
		has_struct = d->storage_class() == declNode::TYPEDEF;
	}

	void at_sue (sueNode *, Order) {
		has_struct = true;
	}

	void at_func (funcNode *, Order) {
		has_struct = true;
	}
};

/** @brief removes unused variables from a procedure
    @ingroup Phases
*/
class unusedVariableCleanupChanger : public Changer {
	public:

	map <declNode *, bool> used;

	unusedVariableCleanupChanger (void) : Changer (Both, Subtree, false) { }

	Node * at_proc (procNode *p, Order ord) {
		if (ord == Preorder)
			used.clear();
		else {
			blockNode *b = p->body();
			for (decl_list_p i=b->decls().begin(); 
				i!=b->decls().end(); ) {
				has_struct_walker w;
				(*i)->walk (w);
				if (w.has_struct || used[*i] || (*i)->storage_class() == declNode::TYPEDEF)
					i++;
				else
					i = b->decls().erase (i);
			}
		}
		return p;
	}

	Node * at_id (idNode *i, Order ord) {
		used[i->decl()] = true;
		return i;
	}
};

#endif // CBZ_DEAD_H
