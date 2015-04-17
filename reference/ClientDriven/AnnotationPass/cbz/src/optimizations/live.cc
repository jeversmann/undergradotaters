// $Id: live.cc,v 1.7 2003/08/07 23:14:16 pnav Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2000 University of Texas at Austin
// 
//  Adam Brown
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

#include "live.h"
#include "cfg.h"

LivenessWalker::LivenessWalker(): 
  Walker (Both, Subtree),
  _num_decls(0),
  _num2decl()
{}

LivenessWalker * LivenessWalker::walk(procNode * the_proc) {
  LivenessWalker * lw = new LivenessWalker();
  the_proc->walk(*lw);
  return lw;
}

void LivenessWalker::at_proc(procNode * the_proc, Order ord) {
  if ( ord == Preorder ) {
    assert(the_proc->decl()->type()->typ() == Func);
    funcNode * the_func = (funcNode *) the_proc->decl()->type();
    blockNode * the_body = the_proc->body();

    // Set the _decl2num & _num2decl for the arguments & local vars
    // We start from 1 b/c all globals map to 0 (the default)
    int num_decls =  the_func->args().size() + the_body->decls().size();
    _num2decl.push_back(NULL);
    int i = 1;
    for ( decl_list_p p = the_func->args().begin();
	  p != the_func->args().end();
	  p++, i++ ) {
      _decl2num[*p] = i;
      _num2decl.push_back(*p);
    }
    for ( decl_list_p p = the_body->decls().begin();
	  p != the_body->decls().end();
	  p++, i++ ) {
      _decl2num[*p] = i;
      _num2decl.push_back(*p);
    }

    assert(i == num_decls + 1);
    _num_decls = i;
  } else { // ord == Postorder 
    // Perform global Liveness analysis based on the local analysis from
    // each basic block.  Use IDFA.

    // TODO: change this to use a worklist
    bool changed = false;
    do { 
      changed = false;
      for ( stmt_list_p p = the_proc->body()->stmts().begin();
	    p != the_proc->body()->stmts().end();
	    p++ ) {
	basicblockNode * the_bb = dynamic_cast<basicblockNode *> (*p);
	assert(the_bb);
	
	// live_out = U live_in (over all successors)
	for ( basicblock_list_p q = the_bb->succs().begin();
	      q != the_bb->succs().end();
	      q++ ) {
	  _live_out[the_bb]->Or(_live_in[*q]);
	}
	
	// live_in = use U (live_out - def)
	Bits old_in(_num_decls);
	old_in.copy(_live_in[the_bb]);	
	_live_in[the_bb]->copy(_live_out[the_bb]);
	_live_in[the_bb]->Difference(_def[the_bb]);
	_live_in[the_bb]->Or(_use[the_bb]);
	
	if ( *_live_in[the_bb] != old_in )
	  changed = true;
      }
//       cout << "changed? " << changed << endl;
    } while ( changed );

    // Propagate the global liveness information back into the statements 
    // inside each basic block.  Note that this simply adds to the information
    // that each stmtNode already had;  it does not completely replace/generate
    // the liveness information for each statment.
    for ( stmt_list_p p = the_proc->body()->stmts().begin();
	  p != the_proc->body()->stmts().end();
	  p++ ) {
      stmt_list::reverse_iterator q;
      basicblockNode * the_bb = (basicblockNode *) *p;
      Bits current(_num_decls);
      current.copy(_live_out[the_bb]);
      for ( q = the_bb->stmts().rbegin(); q != the_bb->stmts().rend(); q++ ) {
	if ( _live_out.find(*q) != _live_out.end() ) {
	  _live_out[*q]->Or(&current);
	  current.Difference(_def[*q]);
	  _live_in[*q]->Or(&current);
	}
      }
    }
  }
}

void LivenessWalker::at_basicblock(basicblockNode * the_bb, Order ord) {
  if ( ord == Postorder ) {
    // Do local Liveness analysis inside the basic block.
    Bits current(_num_decls);
    Bits block_def(_num_decls);
    Bits block_use(_num_decls);
    stmt_list::reverse_iterator p;
    for ( p = the_bb->stmts().rbegin(); p != the_bb->stmts().rend(); p++ ) {
      if ( _def.find(*p) != _def.end() ) {
	_live_out[*p]->copy(&current);

	// in[n] = use[n] U (out[n] - def[n])
	current.Difference(_def[*p]);
	current.Or(_use[*p]);
	_live_in[*p]->copy(&current);

	block_def.Or(_def[*p]);
	block_use.Difference(_def[*p]);
	block_use.Or(_use[*p]);
      }
    } // end for ( ... the_bb->stmts() ... )
    _def[the_bb] = block_def.clone();
    _use[the_bb] = block_use.clone();
    _live_in[the_bb] = new Bits(_num_decls);
    _live_out[the_bb] = new Bits(_num_decls);
  }
}

void LivenessWalker::at_return(returnNode * the_return, Order ord) {
  if ( ord == Preorder ) {
    _use[the_return] = new Bits(_num_decls);
    _def[the_return] = new Bits(_num_decls);
    _live_in[the_return] = new Bits(_num_decls);
    _live_out[the_return] = new Bits(_num_decls);
    if ( the_return->expr() ) {
      idNode * return_expr = dynamic_cast<idNode *> (the_return->expr());
      assert(return_expr); // otherwise, this isn't really dismantled code
      _use[the_return]->set(_decl2num[return_expr->decl()]);
    }
  }
}

void LivenessWalker::at_conditiongoto(conditiongotoNode * the_condgoto,
				      Order ord) {
  if ( ord == Preorder ) {
    _use[the_condgoto] = new Bits(_num_decls);
    _def[the_condgoto] = new Bits(_num_decls);
    _live_in[the_condgoto] = new Bits(_num_decls);
    _live_out[the_condgoto] = new Bits(_num_decls);
    idNode * left = dynamic_cast<idNode *> (the_condgoto->left());
    if ( left )
      _use[the_condgoto]->set(_decl2num[left->decl()]);
    idNode * right = dynamic_cast<idNode *> (the_condgoto->right());
    if ( right )
      _use[the_condgoto]->set(_decl2num[right->decl()]);
  }
}

void LivenessWalker::at_threeAddr(threeAddrNode * the_3addr, Order ord) {
  if ( ord == Preorder ) {
    // Generate the _def's and _use's sets for each stmt
    _use[the_3addr] = new Bits(_num_decls);
    _def[the_3addr] = new Bits(_num_decls);
    _live_in[the_3addr] = new Bits(_num_decls);
    _live_out[the_3addr] = new Bits(_num_decls);

    if ( the_3addr->lhs() )
      defUseOperand(the_3addr, the_3addr->lhs(), true, false);
    if ( the_3addr->rhs1() )
      defUseOperand(the_3addr, the_3addr->rhs1(), false, false);
    if ( the_3addr->rhs2() )
      defUseOperand(the_3addr, the_3addr->rhs2(), false, false);
    if ( !the_3addr->arg_list().empty() )
      for ( operand_list_p p = the_3addr->arg_list().begin();
	    p != the_3addr->arg_list().end();
	    p++ )
	defUseOperand(the_3addr, *p, false, true);
  }
}

// This analysis does not differentiate between different fields in a
// struct nor the different elements in an array.
// TODO: update defUseOperand to perform some simple pointer analyses.
//       Instead of the set_all() calls, we can track which variables have
//       had their address taken (&) and only set the bits corresponding to
//       those variables instead.  We will also have to track arrays and
//       pointer assignments (?).
void LivenessWalker::defUseOperand(threeAddrNode * the_3addr, 
				   operandNode * the_oper,
				   bool on_left,
				   bool func_arg) {
  if ( on_left ) {
    if ( the_oper->star() ) {
      _def[the_3addr]->set_all();
      _use[the_3addr]->set(_decl2num[((idNode *) the_oper->var())->decl()]);
    } else
      _def[the_3addr]->set(_decl2num[((idNode *) the_oper->var())->decl()]);
  } else if ( func_arg ) {
    idNode * var = dynamic_cast<idNode *> (the_oper->var());
    if ( var ) {
      if ( the_oper->star() )
	_use[the_3addr]->set_all();
      else if ( the_oper->addr() )
	_def[the_3addr]->set(_decl2num[var->decl()]);
      else // simply a use of the variable
	_use[the_3addr]->set(_decl2num[var->decl()]);
    }
  } else {
    idNode * var = dynamic_cast<idNode *> (the_oper->var());
    if ( var ) {
      if ( the_oper->star() )
	_use[the_3addr]->set_all();
      else if ( the_oper->addr() )
	;  // possibly add to set of vars whose address is taken
      else // simply a use of the variable
	_use[the_3addr]->set(_decl2num[var->decl()]);
    }
  }
  
  if ( the_oper->index()
       && the_oper->index()->typ() == Id )
    _use[the_3addr]->set(_decl2num[((idNode *) the_oper->index())->decl()]);
}

void LivenessWalker::at_stmt(stmtNode * the_stmt, Order ord) {
  // catch all for statements that don't have defs or uses, and thus
  // don't affect the liveness within a block.  for example,
  // labelNodes, and gotoNodes
  if ( ord == Preorder ) {
    _use[the_stmt] = new Bits(_num_decls);
    _def[the_stmt] = new Bits(_num_decls);
    _live_in[the_stmt] = new Bits(_num_decls);
    _live_out[the_stmt] = new Bits(_num_decls);
  }
}

// returns NULL if bits is NULL
decl_set * LivenessWalker::bits2decls(Bits * bits) {
  if ( !bits )
    return NULL;
  decl_set * the_set = new decl_set;
  for ( int i = 1; i < bits->size(); i++ )
    if ( bits->value(i) )
      the_set->insert(_num2decl[i]);
  return the_set;
}

// returns NULL if the_stmt is not in the procedure that this LivenessWalker
// have liveness information about
decl_set * LivenessWalker::defs(stmtNode * the_stmt) {
  return bits2decls(_def[the_stmt]);
}

// returns NULL if the_stmt is not in the procedure that this LivenessWalker
// have liveness information about
decl_set * LivenessWalker::uses(stmtNode * the_stmt) {
  return bits2decls(_use[the_stmt]);
}

// returns NULL if the_stmt is not in the procedure that this LivenessWalker
// have liveness information about
decl_set * LivenessWalker::live_in(stmtNode * the_stmt) {
  return bits2decls(_live_in[the_stmt]);
}

// returns NULL if the_stmt is not in the procedure that this LivenessWalker
// have liveness information about
decl_set * LivenessWalker::live_out(stmtNode * the_stmt) {
  return bits2decls(_live_out[the_stmt]);
}

LivenessComments::LivenessComments(void):
  Walker(Both, Subtree),
  _lw(NULL)
{}

void LivenessComments::at_proc(procNode * the_proc, Order ord) {
  if ( ord == Preorder )
    _lw = LivenessWalker::walk(the_proc);
  else
    if ( _lw )
      delete _lw;
}

void LivenessComments::comment_stmt(stmtNode * the_stmt) {
  ostringstream ost;
  ost << endl << "defs: ";
  decl_set * defs = _lw->defs(the_stmt);
  for ( decl_set_p p = defs->begin(); p != defs->end(); p++ )
    ost << (*p)->name() << " ";
  delete defs;
  decl_set * uses = _lw->uses(the_stmt);
  ost << "; uses: ";
  for ( decl_set_p p = uses->begin(); p != uses->end(); p++ )
    ost << (*p)->name() << " ";
  delete uses;
  decl_set * live_in = _lw->live_in(the_stmt);
  ost << "; live_in: ";
  for ( decl_set_p p = live_in->begin(); p != live_in->end(); p++ )
    ost << (*p)->name() << " ";
  delete live_in;
  decl_set * live_out = _lw->live_out(the_stmt);
  ost << "; live_out: ";
  for ( decl_set_p p = live_out->begin(); p != live_out->end(); p++ )
    ost << (*p)->name() << " ";
  delete live_out;
  the_stmt->comment() += ost.str();
}
void LivenessComments::at_threeAddr(threeAddrNode * the_3addr, Order ord) {
  if ( ord == Preorder )
    comment_stmt(the_3addr);
}

void LivenessComments::at_basicblock(basicblockNode * the_bb, Order ord) {
  if ( ord == Preorder )
    comment_stmt(the_bb);
}

void LivenessComments::at_return(returnNode * the_return, Order ord) {
  if ( ord == Preorder )
   comment_stmt(the_return);
}

void LivenessComments::at_conditiongoto(conditiongotoNode * the_condgoto,
					Order ord) {
  if ( ord == Preorder )
    comment_stmt(the_condgoto);
}

/** @brief Generates comments based on liveness analysis
 *  @ingroup Phases
 */
class LiveTest : public Phase {
public:
  void run(void) {
    for ( unit_list_p u = CBZ::Program.begin(); u != CBZ::Program.end(); 
	  u++ ) {
      cfg_changer::generate_cfg(*u);

      LivenessComments lc;
      (*u)->walk(lc);
    }
  }
};  

Phases LiveTestPhase("livetest", new LiveTest());
