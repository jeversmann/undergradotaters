// $Id: dismantle-flatten.cc,v 1.16 2007/05/10 14:30:36 abrown Exp $
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
//  Adam Brown
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
// dismantle-flatten.cc
//

#include "c_breeze.h"
#include "dismantle.h"
#include <sstream>

FlattenDismantle::FlattenDismantle(unitNode * u):
  Changer(Both, Subtree, false),
  _new_body(NULL),
  _last_stmt(NULL),
  _return_label(NULL),
  _cur_proc(NULL),
  _vars(),
  _cur_unit(u)
{}

Node * FlattenDismantle::at_proc(procNode * the_proc, Order ord) {
  if ( ord == Preorder ) {
    _new_body = new blockNode(NULL, NULL);
    _last_stmt = NULL;
    _return_label = the_proc->return_label();
    _cur_proc = the_proc;
    assert(the_proc->decl()->type()->typ() == Func);
    decl_list & args = ((funcNode *) the_proc->decl()->type())->args();
    for ( decl_list_p arg_decl = args.begin(); arg_decl != args.end();
          arg_decl++ ) {
      _vars.insert((*arg_decl)->name());
    }
  } else { // ord == Postorder
    // remove sections of code that are a label followed by a goto
    stmt_list_p next = _new_body->stmts().begin();
    stmtNode *prev = NULL;
    _return_label = NULL;
    _cur_proc = NULL;
    while ( next != _new_body->stmts().end() ) {
      stmt_list_p curr = next;
      next++;
      if ( next != _new_body->stmts().end()
           && (*curr)->typ() == Label
	   && (*next)->typ() == Goto ) {
	labelNode * the_label = (labelNode *) *curr;
	labelNode * new_label = ((gotoNode *) *next)->label();
	goto_list_p next_goto = the_label->references().begin();
	while ( next_goto != the_label->references().end() ) {
	  gotoNode * curr_goto = *next_goto;
	  next_goto++;
	  curr_goto->label(new_label);
	}
	_new_body->stmts().erase(curr);  // the Label
	curr = next;
	next++;
        if(prev && prev->typ() != Goto) {
          // keep the Goto.
          prev = *curr;
        } else {
          _new_body->stmts().erase(curr);  // the Goto
          // prev remains unchanged.
        }
      } else
        prev = *curr;
    }
    the_proc->body(_new_body);
  }
  return the_proc;
}

class DefHasName {
    public:
        DefHasName(string & name):
            _name(name)
        {}
        
        bool operator () (defNode * the_def) {
            declNode * the_decl = dynamic_cast<declNode *>(the_def);
            if ( procNode * the_proc = dynamic_cast<procNode *>(the_def) )
                the_decl = the_proc->decl();
            assert(the_decl);
            return the_decl->name() == _name;
        }

    private:
        string _name;
};

Node * FlattenDismantle::at_decl(declNode * the_decl, Order ord) {
  if ( ord == Preorder ) {
    if ( the_decl->decl_location() == declNode::BLOCK ) {
      if (!the_decl->name().empty()) { // avoid renaming abstract declarations
        int next_num = 0;
        string base_name(the_decl->name());
        bool changed_name = false;
        // ensure that we don't have another variable by this name in this proc
        while ( _vars.find(the_decl->name()) != _vars.end()
                || find_if(_cur_unit->defs().begin(),
                           _cur_unit->defs().end(), 
                           DefHasName(the_decl->name())) 
                   != _cur_unit->defs().end() ) {
	  ostringstream ost;
	  ost << base_name << next_num;
	  the_decl->name(ost.str());
	  next_num++;
	  changed_name = true;
        }
        if ( changed_name ) {
	  // update all the idNodes that point to this decl
	  for ( id_list_p p = the_decl->ref_list().begin();
	        p != the_decl->ref_list().end();
	        p++ ) {
	    (*p)->name(the_decl->name());
	  }
        }
        _vars.insert(the_decl->name());
      }
      _new_body->decls().push_back(the_decl);
    }
  }
  return the_decl;
}

Node * FlattenDismantle::at_label(labelNode * the_label, Order ord) {
  // we only want to keep the label if some goto points to it
  if ( ord == Preorder 
       && (the_label->references().size() > 0
           || the_label == _return_label))
    if ( _last_stmt && _last_stmt->typ() == Label ) {
      // if the last statment seen was also a label, just update all of the
      // gotoNode's that point to the_label to point to the last label
      goto_list_p next = the_label->references().begin();
      while ( next != the_label->references().end() ) {
	gotoNode * curr = *next;
	next++;
	curr->label((labelNode *) _last_stmt);
      }
      if(the_label == _return_label)
        _cur_proc->return_label( _return_label = (labelNode*) _last_stmt );
    } else {
      int next_num = 0;
      string base_name(the_label->name());
      bool changed_name = false;
      // ensure that we don't have another label with this same name
      while ( _labels.find(the_label->name()) != _labels.end() ) {
	ostringstream ost;
	ost << base_name << next_num;
	the_label->name(ost.str());
	next_num++;
	changed_name = true;
      }
      if (changed_name ) {
	// update all gotoNodes that point to this labelNode
	for ( goto_list_p p = the_label->references().begin();
	      p != the_label->references().end();
	      p++ ) {
	  (*p)->name(the_label->name());
	}
      }

      _labels.insert(the_label->name());
      _new_body->stmts().push_back(the_label);
      _last_stmt = the_label;
    }
  return the_label;
}

Node * FlattenDismantle::at_threeAddr(threeAddrNode * the_3addr,
					       Order ord) {
  if ( ord == Preorder ) {
    _new_body->stmts().push_back(the_3addr);
    _last_stmt = the_3addr;
  }
  return the_3addr;
}

// This also captures conditiongotoNodes.
Node * FlattenDismantle::at_goto(gotoNode * the_goto, Order ord) {
  if ( ord == Preorder ) {
    // the second unconditional goto is unreachable, so remove it here
    if ( ! (_last_stmt && _last_stmt->typ() == Goto) ) {
      _new_body->stmts().push_back(the_goto);
      _last_stmt = the_goto;
    }
  }
  return the_goto;
}

Node * FlattenDismantle::at_return(returnNode * the_return, 
					    Order ord) {
  if ( ord == Preorder ) {
    _new_body->stmts().push_back(the_return);
    _last_stmt = the_return;
  }
  return the_return;
}

Node * FlattenDismantle::at_exprstmt(exprstmtNode * the_exprstmt,
					      Order ord) {
  if ( ord == Preorder ) {
    if ( the_exprstmt->expr() ) {
      _new_body->stmts().push_back(the_exprstmt);
      _last_stmt = the_exprstmt;
    }
  }
  return the_exprstmt;
}
