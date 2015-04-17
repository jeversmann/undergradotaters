// $Id: dead.cc,v 1.5 2003/08/07 23:14:12 pnav Exp $
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
//
// dead.cc
//
// Dead code elimination
//

#include "c_breeze.h"
#include "live.h"
#include "dead.h"

typedef stmt_list::reverse_iterator stmt_list_rp;

deadCodeEliminationChanger::deadCodeEliminationChanger () :
  Changer(Both, Subtree, false), _lw(NULL)
{}

Node * deadCodeEliminationChanger::at_proc (procNode * proc, Order ord) {
  if (ord == Preorder) {
    // first perform the liveness analysis on the procedure
    // to get def, use, live_in and live_out information
    _lw = LivenessWalker::walk(proc);
  }
  else {  // ord == Postorder
    UnusedDeclarationCleanupChanger udc;
    proc->change(udc);  // remove any unused declarations

    if (_lw) delete _lw;
  }
  return proc;
}

Node * deadCodeEliminationChanger::at_threeAddr (threeAddrNode * node, Order ord) {
  if (ord == Postorder) {
    if (node->lhs()) {  // this is an assignment statement
      id_list& ids = node->lhs()->fields();      
      // Assumption: non-empty fields = struct, non-empty index = array
      if ( (! ids.empty()) || node->lhs()->index() ) {
        return node;  // skip any dead-code elimination
      }

      // get the live_out for this stmt
      decl_set * live_out = _lw->live_out(node);
      // get the defs for this stmt
      decl_set * defs = _lw->defs(node);

      bool in_live_out = false;
      for (decl_set_p iter = defs->begin() ; iter != defs->end() ; iter++) {
        declNode * decl = (* iter);
        // if this definition is in the live_out set,
        if (live_out->find(decl) != live_out->end()) {
          in_live_out = true;
          break;
        }
      }

      // if this definition is NOT in the live_out set,
      // then it is a dead code
      if (! in_live_out) {
//        cout << "Deleting var " << decl->name() << endl;
        return NULL;
      }
//      else cout << "Var " << decl->name() << " is not deleted." << endl;
    }
  }
  return node;
}
