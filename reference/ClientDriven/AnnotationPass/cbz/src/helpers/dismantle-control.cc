// $Id: dismantle-control.cc,v 1.9 2004/11/18 17:02:11 abrown Exp $
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
// dismantle-control.cc
//

#include "c_breeze.h"
#include "dismantle.h"
#include "ref_clone_changer.h"
#include "set_container_walker.h"

ControlDismantle::ControlDismantle(void):
  Changer(Preorder, Subtree, false)
{}

Node * ControlDismantle::at_proc(procNode * the_proc, Order ord) {
  set_container_walker::fixup(the_proc);

  LoopDismantle ld;
  the_proc->change(ld);

  TernaryDismantle td;
  the_proc->change(td);

  SelectionDismantle sd;
  the_proc->change(sd);

  ReturnDismantle rd;
  the_proc->change(rd);

  return the_proc;
}

// This class changes the body of a loop so that every 'break' is replaced
// with a 'goto break_label' and every 'continue' is replaced with a
// 'goto continue_label'.  It assumes that the set_container_walker has
// already been run on the AST.
BreakContinueChanger::BreakContinueChanger(Node * top, labelNode * break_label,
					   labelNode * continue_label, loopNode *new_container) :
  Changer(Preorder, Subtree, false),
  _top(top),
  _break_label(break_label),
  _continue_label(continue_label),
  _new_container(new_container)
{}

Node * BreakContinueChanger::at_break(breakNode * the_break, Order ord) {
  if ( the_break->container() == _top 
       && _break_label )
    return new gotoNode(_break_label, the_break->coord());
  if ( the_break->container() == _top && _new_container )
    the_break->container( _new_container );
  return the_break;
}

Node * BreakContinueChanger::at_continue(continueNode * the_continue, 
					 Order ord) {
  if ( the_continue->container() == _top
       && _continue_label )
    return new gotoNode(_continue_label, the_continue->coord());
  if ( the_continue->container() == _top && _new_container )
    the_continue->container( _new_container );
  return the_continue;
}

LoopDismantle::LoopDismantle(void):
  Changer(Preorder, Subtree, false)
{}

Node * LoopDismantle::at_while(whileNode * the_while, Order ord) {
  // while ( cond ) { body; ...
  //                  break; ...
  //                  continue; }
  // =>
  // {
  //   __test: ;
  //   if ( ! cond ) goto __exit;
  //   body; ...
  //   goto __exit; ...
  //   goto __test;
  //   __exit: ;
  // }

  blockNode * new_block = new blockNode(NULL, NULL, the_while->coord());
  labelNode * test_label = 
    DismantleUtil::new_label(the_while->coord());
  labelNode * exit_label = 
    DismantleUtil::new_label(the_while->coord());
  gotoNode * goto_exit = new gotoNode(exit_label, the_while->coord());
  exprNode * not_cond = DismantleUtil::Not(the_while->cond());
  ifNode * test_cond = new ifNode(not_cond, goto_exit, NULL, 
				  the_while->coord());
  gotoNode * goto_test = new gotoNode(test_label, the_while->coord());

  new_block->stmts().push_back(test_label);
  new_block->stmts().push_back(test_cond);
  // replace 'break' & 'continue' with goto_exit & goto_test (resp).
  BreakContinueChanger bcc(the_while, exit_label, test_label);
  new_block->stmts().push_back((stmtNode *) the_while->body()->change(bcc));

  new_block->stmts().push_back(goto_test);
  new_block->stmts().push_back(exit_label);

  return new_block;
}

Node * LoopDismantle::at_for(forNode * the_for, Order ord) {
  // for ( init; cond; next ) {
  //   body; ...
  //   break; ...
  //   continue; ...
  // }
  // =>
  // {
  //   init;
  //   while ( cond ) {
  //     body; ...
  //     break; ...
  //     goto __next;
  //     __next: ;
  //     next;
  //   }
  // }

  blockNode * new_block = new blockNode(NULL, NULL, the_for->coord());
  exprstmtNode * init = new exprstmtNode(the_for->init());
  exprNode * while_cond = (the_for->cond()) ? the_for->cond() :
    new constNode(constant(1), "1", the_for->coord());

  blockNode * while_body = new blockNode(NULL, NULL, the_for->coord());
  whileNode * while_loop = new whileNode(while_cond, while_body,
					 the_for->coord());

  // we only need to create the __next: label if the next portion of the
  // for loop is present.  otherwise, we let the while loop update any
  // continue statements.
  if ( the_for->next() ) {
    labelNode * next_label = DismantleUtil::new_label(the_for->coord());
    exprstmtNode * next_stmt = new exprstmtNode(the_for->next());

    // replace 'continue' with "goto next_label"
    BreakContinueChanger bcc(the_for, NULL, next_label, while_loop);
    while_body->stmts().push_back((stmtNode *) the_for->body()->change(bcc));
    while_body->stmts().push_back(next_label);
    while_body->stmts().push_back(next_stmt);
  } else {
    // update the container() for any remaining break & continue statements
    BreakContinueChanger bcc(the_for, NULL, NULL, while_loop);
    while_body->stmts().push_back((stmtNode *) the_for->body()->change(bcc));
  }

  new_block->stmts().push_back(init);
  new_block->stmts().push_back(while_loop);

  return new_block;
}

Node * LoopDismantle::at_do(doNode * the_do, Order ord) {
  // do {
  //   body; ...
  //   break; ...
  //   continue; ...
  // } while ( cond );
  // =>
  // {
  //   __body: ;
  //   body; ...
  //   goto __exit; ...
  //   goto __test; ...
  //   __test: ;
  //   if ( !cond ) goto __exit;
  //   goto __body;
  //   __exit: ;
  // }

  blockNode * new_block = new blockNode(NULL, NULL, the_do->coord());
  labelNode * body_label = DismantleUtil::new_label(the_do->coord());
  labelNode * test_label = DismantleUtil::new_label(the_do->coord());
  labelNode * exit_label = DismantleUtil::new_label(the_do->coord());
  exprNode * not_cond = DismantleUtil::Not(the_do->cond());
  gotoNode * goto_exit = new gotoNode(exit_label);
  ifNode * test_cond = new ifNode(not_cond, goto_exit, NULL, the_do->coord());
  gotoNode * goto_body = new gotoNode(body_label);
  gotoNode * goto_test = new gotoNode(test_label);

  new_block->stmts().push_back(body_label);
  // replace 'break' & 'continue' with goto_exit & goto_test (resp).
  BreakContinueChanger bcc(the_do, exit_label, test_label);
  new_block->stmts().push_back((stmtNode *) the_do->body()->change(bcc));

  new_block->stmts().push_back(test_label);
  new_block->stmts().push_back(test_cond);
  new_block->stmts().push_back(goto_body);
  new_block->stmts().push_back(exit_label);

  return new_block;
}

TernaryDismantle::TernaryDismantle(void):
  Changer(Both, Subtree, false),
  _cur_stmt(NULL), _in_type(0)
{}

Node * TernaryDismantle::at_stmt(stmtNode * the_stmt, Order ord) {
  if ( ord == Preorder ) {
    _cur_stmt = the_stmt;
  } else {
    _cur_stmt = NULL;
    if ( _new_block[the_stmt] ) {
      _new_block[the_stmt]->stmts().push_back(the_stmt);
      blockNode * ret = _new_block[the_stmt];
      // we want to re-change the block, b/c there may be additional 
      // ternaryNodes that we didn't/couldn't dismantle on this pass
      TernaryDismantle td;
      return ret->change(td);
    }
  }
  return the_stmt;
}

Node * TernaryDismantle::at_ternary(ternaryNode * the_ternary,
					     Order ord) {
  if (! _cur_stmt || _in_type>0) {
    return the_ternary; // we're not in a statement.
  }
  if ( ord == Preorder ) {
    // a ? b : c
    // =>
    // {
    //   if ( a )
    //     __tmp = b;
    //   else
    //     __tmp = c;
    // } (returns __tmp)
    if ( !_new_block[_cur_stmt] )
      _new_block[_cur_stmt] = new blockNode(NULL, NULL, the_ternary->coord());

    declNode * result_decl;
    typeNode * result_type = the_ternary->type();
    bool is_void = result_type->is_void();
    exprstmtNode * assign_true,
                 * assign_false;
    if(is_void) {
      /* this is to handle case like "(cond) ? (void) 0 : __assert(...)"
       * ie. both true and false branches are void expressions, so this
       * ternaryNode should not be replaced by assignment statements.
       */ 
      assign_true = new exprstmtNode(the_ternary->true_br());
      assign_false = new exprstmtNode(the_ternary->false_br());
    } else {
      result_decl = DismantleUtil::new_temp_decl(the_ternary->type(),
					         the_ternary->coord());
      _new_block[_cur_stmt]->decls().push_back(result_decl);
      assign_true = new exprstmtNode(new binaryNode('=',
                        new idNode(result_decl),
                        the_ternary->true_br(),
                        the_ternary->coord()));
      assign_true->expr()->type((typeNode*)ref_clone_changer::clone(result_type,
                                                                    false));
      assign_false =
        new exprstmtNode(new binaryNode('=',
                new idNode(result_decl),
                the_ternary->false_br(),
                the_ternary->coord()));
      assign_false->expr()->type(
        (typeNode*)ref_clone_changer::clone(result_type, false));
    }
    ifNode * new_if = new ifNode(the_ternary->cond(),
				 assign_true,
				 assign_false,
				 the_ternary->coord());
    _new_block[_cur_stmt]->stmts().push_back(new_if);
    if(is_void)
      return NULL;
    else
      return new idNode(result_decl, the_ternary->coord());
  }
  return the_ternary;
}

Node * TernaryDismantle::at_type(typeNode * the_type, Order ord) {
  if (ord==Preorder) _in_type++;
  else _in_type--;
  return the_type;
}

SelectionDismantle::SelectionDismantle(void):
  Changer(Both, Subtree, false),
  _cur_stmt(NULL),
  _in_expr(false)
{}


// Do the following 4 methods belong in a different Changer?  They don't
// really dismantle selectionNodes, they only transform && and || into 
// equivalent ifNodes.
Node * SelectionDismantle::andand_oror_in_expr(stmtNode * the_stmt,
							Order ord) {
  if ( ord == Preorder ) {
    _in_expr = true;
  } else { // ord == Postorder
    _in_expr = false;
    if ( _expr_block[the_stmt] ) {
      blockNode * ret = _expr_block[the_stmt];
      ret->stmts().push_back(the_stmt);
      
      // must do this so that the newly generated "if ( a && b )"
      // are dismantled properly
      SelectionDismantle sd;
      return ret->change(sd);
    }
  }
  return the_stmt;
}

Node * SelectionDismantle::at_exprstmt(exprstmtNode * the_exprstmt,
						Order ord) {
  _cur_stmt = the_exprstmt;
  return andand_oror_in_expr(the_exprstmt, ord);
}

Node * SelectionDismantle::at_return(returnNode * the_return,
					      Order ord) {
  _cur_stmt = the_return;
  return andand_oror_in_expr(the_return, ord);
}

Node * SelectionDismantle::at_binary(binaryNode * the_binary,
					      Order ord) {
  if ( ord == Preorder ) {
    Operator * op = the_binary->op();
    if ( _in_expr
	 && ( op->id() == Operator::ANDAND
	      || op->id() == Operator::OROR ) ) {
      // (in an expression) ... b && c ...; // (or ||)
      // =>
      // { 
      //   tmp = 0;
      //   if ( b && c) // (or ||)
      //     tmp = 1;
      // }
      // ... tmp ...; (returns tmp)
      if(! _expr_block[_cur_stmt])
        _expr_block[_cur_stmt] = new blockNode(NULL, NULL, the_binary->coord());
      declNode * temp_decl = 
	DismantleUtil::new_temp_decl(the_binary->type(),
					      the_binary->coord());
      typeNode * bin_type = the_binary->type();
      exprstmtNode * assign_zero =
	new exprstmtNode(new binaryNode('=', new idNode(temp_decl),
					new constNode(constant(0)),
					the_binary->coord()));
      assign_zero->expr()->type((typeNode *) ref_clone_changer::clone(bin_type,
								      false));
      exprstmtNode * assign_one =
	new exprstmtNode(new binaryNode('=', new idNode(temp_decl),
					new constNode(constant(1)),
					the_binary->coord()));
      assign_one->expr()->type((typeNode *) ref_clone_changer::clone(bin_type,
								     false));
      ifNode * new_if = new ifNode(the_binary, assign_one, NULL,
				   the_binary->coord());
      _expr_block[_cur_stmt]->decls().push_back(temp_decl);
      _expr_block[_cur_stmt]->stmts().push_back(assign_zero);
      _expr_block[_cur_stmt]->stmts().push_back(new_if);
      return new idNode(temp_decl);
    }
  } // end if ( ord == Preorder )
  return the_binary;
}

// TODO:  add comments from 1/9/2003 - #1 notes
Node * SelectionDismantle::at_switch(switchNode * the_switch, 
					      Order ord) {
  _cur_stmt = the_switch;
  if ( ord == Preorder ) {
    // switch ( expr ) {
    //   body;
    //   case a:
    //     bodya;
    //   case b:
    //     bodyb;
    //   [default:
    //     bodyn;]
    // }
    // =>
    // { tmp = expr;
    //   if ( tmp == a ) goto bodya_label;
    //   if ( tmp == b ) goto bodyb_label;
    //   goto default_label;
    //   bodya_label: ;
    //   bodya; // with 'break' replaced by 'goto break_label;'
    //   bodyb_label: ;
    //   bodyb; // ditto
    //   default_label: ;
    //   bodyn; // ditto
    //   break_label: ;
    // }
    // Note:  we currently leave the caseNode's in the body and they are
    //        later removed by DismantleFlatten
    blockNode * new_block = new blockNode(NULL, NULL, the_switch->coord());
    exprNode * switch_expr = the_switch->expr();
    // TODO: if the switch_expr is an idNode or a constNode, we don't really
    //       need the temp_decl.
    declNode * temp_decl = 
      DismantleUtil::new_temp_decl(switch_expr->type(),
					    the_switch->coord());
    new_block->decls().push_back(temp_decl);
    
    exprstmtNode * assign_expr = 
      new exprstmtNode(new binaryNode('=', new idNode(temp_decl),
				      switch_expr, the_switch->coord()) );
    assign_expr->expr()->type((typeNode *) 
			      ref_clone_changer::clone(temp_decl->type(),
						       false));
    new_block->stmts().push_back(assign_expr);
    
    labelNode * break_label = 
      DismantleUtil::new_label(the_switch->coord());
    labelNode * default_label = break_label;
    
    for( target_list_p p = the_switch->cases().begin();
	 p != the_switch->cases().end();
	 p++ ) {
      if ( (*p)->typ() == Case ) { // make sure that it isn't Label
	caseNode * the_case = (caseNode *) *p;
	if ( the_case->expr() ) {
	  binaryNode * case_cmp = new binaryNode(Operator::EQ,
						 new idNode(temp_decl),
						 the_case->expr(),
						 the_case->coord());
	  case_cmp->type((typeNode *)
			 ref_clone_changer::clone(temp_decl->type(),
						  false));
	  labelNode * case_label = 
	    DismantleUtil::new_label(the_case->coord());
	  ifNode * case_if = new ifNode(case_cmp, 
					new gotoNode(case_label),
					NULL,
					the_case->coord());
	  new_block->stmts().push_back(case_if);
	  the_case->stmt()->stmts().push_front(case_label);
	} else { // the default case
	  default_label = DismantleUtil::new_label(the_case->coord());
	  the_case->stmt()->stmts().push_front(default_label);
	}
      }
    }
    new_block->stmts().push_back(new gotoNode(default_label, 
					      the_switch->coord()));
    // replace 'break's with 'goto <break_label>'
    BreakContinueChanger bcc(the_switch, break_label, NULL);
    new_block->stmts().push_back((stmtNode *) the_switch->stmt()->change(bcc));

    new_block->stmts().push_back(break_label);
    
    return new_block;
  } // end if ( ord == Preorder )
  return the_switch;
}

indexNode * SelectionDismantle::comparison_operand
(
 exprNode * orig_operand, 
 blockNode * temp_block
 )
{
  indexNode * ret = NULL;
  if ( orig_operand->typ() == Const ||
       orig_operand->typ() == Id )
    ret = (indexNode *) orig_operand;
  else {
    declNode * temp_decl =
      DismantleUtil::new_temp_decl(orig_operand->type(),
					    orig_operand->coord());
    exprstmtNode * assign_temp =
      new exprstmtNode(new binaryNode('=', new idNode(temp_decl),
				      orig_operand));
    assign_temp->expr()->type((typeNode *)
			      ref_clone_changer::clone(temp_decl->type(),
						       false));
    temp_block->decls().push_back(temp_decl);
    temp_block->stmts().push_back(assign_temp);
    ret = new idNode(temp_decl);
  }
  return ret;
}

// TODO: factor into smaller chunks?
Node * SelectionDismantle::at_if(ifNode * the_if, Order ord) {
  _cur_stmt = the_if;
  if ( ord == Preorder ) {
    blockNode * new_block = new blockNode(NULL, NULL, the_if->coord());
    // transform
    // if ( a )
    //   <null>
    // else
    //   <null>
    // =>
    // a;
    if ( ! the_if->true_br()
	 && ! the_if->false_br() )
      return new exprstmtNode(the_if->expr());

    // transform
    // if ( a )
    //   <null>
    // else
    //   b;
    // =>
    // if ( ! a )
    //   b;
    if ( !the_if->true_br() ) {
      the_if->expr(DismantleUtil::Not(the_if->expr()));
      the_if->true_br(the_if->false_br());
      the_if->false_br(NULL);
    }

    // transform
    // if ( a )
    //   b;
    // else
    //   c;
    // into
    // if ( a ) 
    //  { b; goto <exit_label>; }
    // <else_label>: ;
    // c;
    // <exit_label>: ;
    labelNode* exit_label = DismantleUtil::new_label(the_if->coord());
    labelNode * else_label = exit_label;
    if ( the_if->false_br() ) {
      else_label =
	DismantleUtil::new_label(the_if->false_br()->coord());
      new_block->stmts().push_back(else_label);
      new_block->stmts().push_back(the_if->false_br());
      new_block->stmts().push_back(exit_label);
      the_if->true_br()->stmts().push_back(new gotoNode(exit_label));
      the_if->false_br(DismantleUtil::empty_block());
    } else {
      new_block->stmts().push_back(exit_label);
    }
    
    if ( the_if->expr()->typ() == Binary ) {
      binaryNode * test = (binaryNode *) the_if->expr();
      if ( test->op()->id() == Operator::ANDAND ) {
	// if ( a && b ) foo;
	// =>
	// if ( a )
	//    if ( b )
	//       foo;
	ifNode * inner_if = new ifNode(test->right(),
				       the_if->true_br(),
				       NULL,
				       the_if->coord());
	ifNode * outer_if = new ifNode(test->left(),
				       inner_if,
				       NULL,
				       the_if->coord());
	new_block->stmts().push_front(outer_if);
	return new_block;
      } else if ( test->op()->id() == Operator::OROR ) {
	// if ( a || b ) foo;
	// =>
	// if ( a ) goto then;
	// if ( b ) { 
	//   then: ;
	//   foo;
	// }
	labelNode * then_label =
	  DismantleUtil::new_label(the_if->coord());
	ifNode * first_if = new ifNode(test->left(),
				       new gotoNode(then_label),
				       NULL,
				       the_if->coord());
	ifNode * second_if = new ifNode(test->right(),
					the_if->true_br(),
					NULL,
					the_if->coord());
	second_if->true_br()->stmts().push_front(then_label);
	new_block->stmts().push_front(second_if);
	new_block->stmts().push_front(first_if);
	return new_block;
      } else if ( test->op()->is_comparison() ) {
	// if ( a <relop> b ) c;
	// =>
	// a' = a; /* if a ! indexNode */
	// b' = b; /* if b ! indexNode */
	// if ( a' <!relop> b' ) goto skip;
	// c;
	// skip: ;
	blockNode * sub_block = new blockNode(NULL, NULL);
	indexNode * left = comparison_operand(test->left(), sub_block);
	indexNode * right = comparison_operand(test->right(), sub_block);
	assert(left != NULL && right != NULL);
	Operator * op = test->op();
	conditiongotoNode * new_if =
	  new conditiongotoNode(else_label, left, 
				DismantleUtil::not_relop[op->id()],
				right, the_if->coord());
	sub_block->stmts().push_back(new_if);
	sub_block->stmts().push_back(the_if->stmt());
	new_block->stmts().push_front(sub_block);
	return new_block;
      }
    } else if ( the_if->expr()->typ() == Const ||
		the_if->expr()->typ() == Id ) {
      // TODO: should this be the type of the expr()?
      // yes, but is this still okay?
      constNode * zero = new constNode(constant(0));
      indexNode * left = (indexNode *) the_if->expr();
      conditiongotoNode * new_if =
	new conditiongotoNode(else_label, left, Operator::EQ, zero,
			      the_if->coord());
      new_block->stmts().push_front(the_if->stmt());
      new_block->stmts().push_front(new_if);
      return new_block;
    } else if ( the_if->expr()->typ() == Unary ) {
      unaryNode * unary_expr = (unaryNode *) the_if->expr();
      if ( unary_expr->op()->id() == '!' ) {
	// if ( !a ) b;
	// =>
	// if ( a ) ; else b;
	ifNode * new_if = new ifNode(unary_expr->expr(),
				     DismantleUtil::empty_block(),
				     the_if->true_br(),
				     the_if->coord());
	new_block->stmts().push_front(new_if);
	return new_block;
      }
    }

    // the_if->expr() is either Call, Unary, or arithmetic binary
    exprNode * test = the_if->expr();
    declNode * temp_decl =
      DismantleUtil::new_temp_decl(test->type(),
					    test->coord());
    exprstmtNode * assign_temp =
      new exprstmtNode(new binaryNode('=', new idNode(temp_decl),
				      test, the_if->coord()));
    assign_temp->expr()->type((typeNode *)
			      ref_clone_changer::clone(temp_decl->type(),
						       false));
    the_if->expr(new idNode(temp_decl));
    new_block->decls().push_back(temp_decl);
    new_block->stmts().push_front(the_if);
    new_block->stmts().push_front(assign_temp);
    return new_block;
  }
  return the_if;
}


Node * SelectionDismantle::at_stmt(stmtNode * the_stmt, Order ord) {
  _cur_stmt = the_stmt;
  return the_stmt;
}

ReturnDismantle::ReturnDismantle(void):
  Changer(Preorder, Subtree, false)
{}

Node * ReturnDismantle::at_proc(procNode * the_proc, Order ord) {
  if ( !the_proc->return_label() ) {
    labelNode * return_label = 
      DismantleUtil::new_label(the_proc->coord()); // ???
    the_proc->return_label(return_label);
    blockNode * return_block = new blockNode(NULL, NULL);
    return_block->stmts().push_front(return_label);
    the_proc->body()->stmts().push_back(return_block);
    returnNode * proc_return = NULL;
    funcNode * return_type = (funcNode *) the_proc->decl()->no_tdef_type();
    if ( ! return_type->returns()->follow_tdefs()->is_void() ) {
      // the proc returns non-void
      declNode * return_decl =
	DismantleUtil::new_temp_decl(return_type->type(),
					      the_proc->coord()); // ???
      the_proc->body()->decls().push_back(return_decl);
      the_proc->return_decl(return_decl);
      proc_return = new returnNode(new idNode(return_decl), the_proc, true);
    } else { // the_proc returns void
      proc_return = new returnNode(NULL, the_proc, true);
    }
    return_block->stmts().push_back(proc_return);
  }
  return the_proc;
}

Node * ReturnDismantle::at_return(returnNode * the_return, 
					   Order ord) {
  stmtNode * ret = the_return;
  if ( ! the_return->proc_exit() ) {
    procNode * curr_proc = the_return->proc();
    blockNode * new_block = new blockNode(NULL, NULL, the_return->coord());

    if ( the_return->expr() ) { // function returns non-void
      exprstmtNode * assign_return = 
	new exprstmtNode(new binaryNode('=',
					new idNode(curr_proc->return_decl(),
						   the_return->coord()),
					the_return->expr(),
					the_return->coord()));
      assign_return->expr()->type(the_return->expr()->type());
      new_block->stmts().push_back(assign_return);
    }

    gotoNode * goto_return = new gotoNode(curr_proc->return_label(),
					  the_return->coord());
    new_block->stmts().push_back(goto_return);
    ret = new_block;
  } // else, this is the already dismantled return for the proc
  return ret;
}
