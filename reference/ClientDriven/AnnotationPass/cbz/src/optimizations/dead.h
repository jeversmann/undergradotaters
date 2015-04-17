// $Id: dead.h,v 1.7 2003/08/07 23:14:13 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
//
//  Copyright (c) 2000 University of Texas at Austin
//
//  Lipphei Adam
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
  LivenessWalker * _lw;

public:
  deadCodeEliminationChanger ();
  virtual Node * at_proc (procNode * proc, Order ord);
  virtual Node * at_threeAddr (threeAddrNode * node, Order ord);
};


/** @brief determines is subtree contains a node that uses a struct def
 *
 */
class has_struct_walker : public Walker {
public:
  bool has_struct;

  has_struct_walker (void) :
    Walker(Preorder, Subtree), has_struct(false) { }

  void at_decl (declNode * d, Order) {
    has_struct = d->storage_class() == declNode::TYPEDEF;
  }

  void at_sue (sueNode * s, Order) {
    has_struct = true;
  }

  void at_func (funcNode * f, Order) {
    has_struct = true;
  }
};


/** @brief removes unused declarations from a procedure
 *
 */
class UnusedDeclarationCleanupChanger : public Changer {
private:
  map <declNode *, bool> used;

public:
  UnusedDeclarationCleanupChanger (void) : 
    Changer(Both, Subtree, false) { }

  Node * at_proc (procNode * p, Order ord) {
    if (ord == Preorder)
      used.clear();
    else {
      blockNode * b = p->body();
      decl_list_p i;
      for (i = b->decls().begin() ; i != b->decls().end(); ) {
        has_struct_walker w;
        (*i)->walk (w);
        if (w.has_struct || used[*i])
          i++;
        else {
          int count = b->decls().size();
          i = b->decls().erase (i);
          assert(count == b->decls().size() + 1);
        }
      }
    }
    return p;
  }

  Node * at_id (idNode * id, Order ord) {
    used[id->decl()] = true;
    return id;
  }
};

#endif

