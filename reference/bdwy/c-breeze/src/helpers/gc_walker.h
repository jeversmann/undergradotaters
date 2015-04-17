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

#ifndef CBZ_GC_WALKER_H
#define CBZ_GC_WALKER_H

class gcWalker : public Walker {
public:
	gcWalker (void) : Walker (Preorder, Subtree) {
		// mark all nodes as unvisited

		return;
		for (node_list_p p=Node::nodes.begin();
			p!=Node::nodes.end(); p++) {
			(*p)->mark = false;
		}
	}

	static void collect (void) {
		return; // until we can fix the bug
		gcWalker g;
		text_list_p p;
		for (p=CBZ::pragmas.begin(); p!=CBZ::pragmas.end(); p++)
			(*p)->walk (g);
		unit_list_p n;
		for (n=CBZ::Program.begin(); n!=CBZ::Program.end(); n++)
			(*n)->walk (g);
		g.delete_nodes();
	}

	void at_unit (unitNode *u, Order) {

		// mark all the stuff that won't get visited
		// by a normal tree traversal

	
		u->types()->mark_nodes();
		u->tags()->mark_nodes();
		decl_list_p p;
		for (p=u->undef_funcs().begin(); p!=u->undef_funcs().end(); p++)
			(*p)->mark = true;
		suespec_list_p q;
		for (q=u->suespecs().begin(); q!=u->suespecs().end(); q++)
			(*q)->mark = true;
	}
		
	void at_binary (binaryNode *b, Order o) {
		b->mark = true;
		// because of -> and .
		Node *r = b->right();
		if (r) {
			r->mark = true;
			r->walk (*this);
		}
	}

	void at_node (Node *n, Order) {
		n->mark = true;
	}

	void delete_nodes (void) {
		int count = 0;
		list <Node *> remaining;
		map <Node *,bool>::iterator p;
		list<Node *>::iterator q;
		for (q=Node::nodes.begin(); q!=Node::nodes.end(); ) {
			Node *n = *q;
			if (n->mark == false) {
				if (n->typ() == Unit) {
					q++;
					continue;
				}
				if (!Node::deleted_nodes[n]) delete n;
				q = Node::nodes.erase (q);
				count++;
			} else {
				remaining.push_back (n);
				q++;
			}
		}
		Node::deleted_nodes.clear();
		cout << "deleted " << count << " nodes.\n";
	}
};

#endif // CBZ_GC_WALKER_H
