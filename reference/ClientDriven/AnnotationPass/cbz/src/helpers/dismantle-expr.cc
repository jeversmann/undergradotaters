// $Id: dismantle-expr.cc,v 1.26 2003/08/28 22:11:28 abrown Exp $
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
// dismantle-expr.cc
//

#include <assert.h>
#include "c_breeze.h"
#include "dismantle.h"
#include "ref_clone_changer.h"

ArrowDismantle::ArrowDismantle(void):
  Changer(Postorder, Subtree, false)
{}

Node * ArrowDismantle::at_binary(binaryNode * the_binary, Order ord) {
  Operator * op = the_binary->op();
  if ( op->id() == Operator::ARROW ) {
    // change foo->bar to *(foo).bar
    unaryNode * deref = new unaryNode(Operator::INDIR, 
				      the_binary->left(), 
				      the_binary->left()->coord());
    binaryNode * dot = new binaryNode('.', deref, the_binary->right(), 
				      the_binary->right()->coord());
    dot->type((typeNode*) ref_clone_changer::clone(the_binary->type(),
                                                   false));
    return dot;
  }
  return the_binary;
}

map<unsigned int, unsigned int> ExpressionDismantle::_op_assign_map;
set<unsigned int> ExpressionDismantle::_op_post;

void ExpressionDismantle::init() {
  if ( _op_assign_map.empty() ) {
    _op_assign_map[Operator::PLUSassign] = '+';
    _op_assign_map[Operator::MINUSassign] = '-';
    _op_assign_map[Operator::MULTassign] = '*';
    _op_assign_map[Operator::DIVassign] = '/';
    _op_assign_map[Operator::MODassign] = '%';
    _op_assign_map[Operator::ANDassign] = '&';
    _op_assign_map[Operator::ORassign] = '|';
    _op_assign_map[Operator::LSassign] = Operator::LS;
    _op_assign_map[Operator::RSassign] = Operator::RS;
    _op_assign_map[Operator::ERassign] = '^';
    _op_assign_map[Operator::PREINC] = Operator::PLUSassign;
    _op_assign_map[Operator::POSTINC] = Operator::PLUSassign;
    _op_assign_map[Operator::PREDEC] = Operator::MINUSassign;
    _op_assign_map[Operator::POSTDEC] = Operator::MINUSassign;
  }

  if( _op_post.empty() ) {
    _op_post.insert(Operator::POSTINC);
    _op_post.insert(Operator::POSTDEC);
  }
}

ExpressionDismantle::ExpressionDismantle(void):
  Changer(Both, Subtree, false),
  _new_block(NULL),
  _in_type(0)
{}

Node * ExpressionDismantle::at_exprstmt(exprstmtNode * the_exprstmt,
						 Order ord) {
  if ( ord == Preorder ) {
    // don't worry if the expr() is NULL, like in the stmt() of a labelNode
    if ( the_exprstmt->expr() ) {
      ArrowDismantle ad;
      the_exprstmt = (exprstmtNode *) the_exprstmt->change(ad);
      _new_block = new blockNode(NULL, NULL, the_exprstmt->coord());
    }
  } else { // ord == Postorder
    // _new_block is NULL only when the expr() was NULL, like in the stmt()
    // of a labelNode
    if ( _new_block ) {
      blockNode * ret = _new_block;
      _new_block = NULL;
      // This 'recursion' is safe, because we bottom out once we
      // have only threeAddrNodes in the AST, and no more exprstmtNodes.
      ExpressionDismantle ed;
      return ret->change(ed);
    }
  }
  return the_exprstmt;
}

Node * ExpressionDismantle::at_type(typeNode * the_type, Order ord) {
  // We don't want to inadvertently process exprNodes under a typeNode, thus
  // generating incorrect intermediate result nodes.
  if ( ord == Preorder ) 
    _in_type++;
  else
    _in_type--;
  return the_type;
}

// TODO: factor?
Node * ExpressionDismantle::at_binary(binaryNode * the_binary,
					       Order ord) {
  if ( !_new_block || _in_type ) // we're not in an expression statement
    return the_binary;

  if ( ord == Postorder ) {
    Operator * op = the_binary->op();
    exprNode * left = the_binary->left();
    exprNode * right = the_binary->right();
    exprNode * ret = the_binary;  // the return value
    
    switch ( op->id() ) {
    case '=': 
      {
	if ( left->typ() == Id ) {
	  // make an operandNode from left
	  left = new operandNode((idNode *) ref_clone_changer::clone(left, 
								     false),
				 left->coord());
	}
	// make an operandNode from right
	if ( right->typ() != Operand ) {
	  assert(right->typ() == Id || right->typ() == Const);
	  right = new operandNode((indexNode *) right, right->coord());
	}
	assert(left->typ() == Operand && right->typ() == Operand);
	threeAddrNode * new_stmt = new threeAddrNode((operandNode *) left, 
						     (operandNode *) right,
						     the_binary->coord());
	_new_block->stmts().push_back(new_stmt);
	// must return a clone, b/c left appears in the AST in the
	// threeAddrNode new_stmt
	ret = (operandNode *) ref_clone_changer::clone(left, false);
      }
      break;
    case Operator::PLUSassign: case Operator::MINUSassign: 
    case Operator::MULTassign: case Operator::DIVassign: 
    case Operator::MODassign: case Operator::ANDassign:
    case Operator::ORassign: case Operator::LSassign:
    case Operator::RSassign: case Operator::ERassign:
      {
	// TODO: finish/fix to handle a[b] += 1; properly
	// actually, b/c we dismantle from bottom to top, we are guaranteed
	// that an array subscript will be either a constant or a variable;
	// we won't have a more complex expression.  so, it is safe not
	// to generate a temporary pointer.
	exprNode * new_rhs = 
	  new binaryNode(_op_assign_map[op->id()],
			 (exprNode*) ref_clone_changer::clone(left,
							      false),
			 right);
	new_rhs->type((typeNode *) ref_clone_changer::clone(the_binary->type(),
							    false));
	the_binary->right(new_rhs);
	the_binary->op(Operators::table['=']);
	_new_block->stmts().push_back(new exprstmtNode(the_binary));
	ret = (exprNode *) ref_clone_changer::clone(left, false);
      }
      break;
    case '.':
      {
	if ( left->typ() == Id ) {
	  // make an operandNode from left
	  left = new operandNode((idNode *) left, left->coord());
	} else {
	  assert(left->typ() == Operand);
	  operandNode * left_oper = (operandNode *) left;

	  // Sec. 6.3.4
	  // The type of a cast must either be void or scalar, therefore
	  // we don't have to worry about the 'potential' case of:
	  // ((cast) left).field
	  if ( left_oper->cast() ) {
	    CBZ::SyntaxError(left_oper->coord(),
			     "conversion to non-scalar type requested");
	  }

          if(left_oper->addr()) {
            cout << "Error: does not make sense to access field in \"(";
            output_context oc(cout);
            left_oper->output(oc,NULL);
            cout << ").";
            right->output(oc,NULL);
            cout << "\" at " << the_binary->coord() << "\n";
            exit(1);
          }
	  if ( left_oper->index() ) {
	    // foo[idx].fld
	    // =>
	    // elem * T1;
	    // T1 = &foo[idx];
	    // (*T1).fld (is output)

	    // the temporary is really a pointer to the array element type
	    typeNode * left_type = 
	      new ptrNode(typeNode::NONE, left_oper->type());
	    declNode * tmp_arry_decl =
	      DismantleUtil::new_temp_decl(left_type,
						    the_binary->coord());
	    _new_block->decls().push_back(tmp_arry_decl);
	    left_oper->addr(true);
	    threeAddrNode * assign_tmp_array =
	      new threeAddrNode(new operandNode(new idNode(tmp_arry_decl),
						left_oper->coord()), 
				left_oper,
				left_oper->coord());
	    _new_block->stmts().push_back(assign_tmp_array);
	    left = new operandNode(new idNode(tmp_arry_decl), true, false,
				   left_oper->coord());
	  }
	}
	assert(left->typ() == Operand && right->typ() == Id);
	((operandNode *) left)->fields().push_back((idNode *) right);
	// don't clone, b/c left doesn't appear anywhere in the AST, yet
	// it won't until it appears in a threeAddrNode
	ret = left;
      }
      break;
    case Operator::Index:
      {
	if ( left->typ() == Id ) {
	  // make an operandNode from left
	  left = new operandNode((idNode *) left, left->coord());
	} else if ( left->typ() == Const ) {
	  // make an operandNode from left
	  left = new operandNode((constNode *) left, left->coord());
	} else if ( left->typ() == Operand ) {
	  // must ensure that the left is not already being subscripted
	  operandNode * left_oper = (operandNode *) left;

          if(left_oper->cast()) {
            // ((cast) left)[i]
            // =>
            // T = (cast) left;
            // T[i]
	    declNode * tmp_decl =
	      DismantleUtil::new_temp_decl(left_oper->cast(),
					   the_binary->coord());
	    _new_block->decls().push_back(tmp_decl);
	    threeAddrNode * assign_tmp =
	      new threeAddrNode(new operandNode(new idNode(tmp_decl),
						left_oper->coord()), 
				left_oper,
				left_oper->coord());
	    _new_block->stmts().push_back(assign_tmp);
	    left = left_oper
                 = new operandNode(new idNode(tmp_decl), left_oper->coord());
          }

          if(left_oper->addr()) {
            // "(&S)[x]" -> "tmp = &S; tmp[x]"
	    typeNode * left_type = left_oper->type();
	    declNode * tmp_addr_decl =
              DismantleUtil::new_temp_decl(left_type, the_binary->coord());
	    _new_block->decls().push_back(tmp_addr_decl);
	    threeAddrNode * assign_tmp_addr =
	      new threeAddrNode(new operandNode(new idNode(tmp_addr_decl),
						left->coord()), 
				(operandNode *) left,
				left->coord());
	    _new_block->stmts().push_back(assign_tmp_addr);
            left_oper = new operandNode(new idNode(tmp_addr_decl), 
					left->coord());
            left = left_oper;
          }
	  if ( left_oper->index() ) {
	    typeNode * left_type = left_oper->type();
	    declNode * tmp_arry_decl =
	      DismantleUtil::new_temp_decl(left_type,
						    the_binary->coord());
	    _new_block->decls().push_back(tmp_arry_decl);
	    threeAddrNode * assign_tmp_array =
	      new threeAddrNode(new operandNode(new idNode(tmp_arry_decl),
						left->coord()), 
				(operandNode *) left,
				left->coord());
	    _new_block->stmts().push_back(assign_tmp_array);
	    left = new operandNode(new idNode(tmp_arry_decl), 
				   left->coord());
	  }
	}
	if ( right->typ() == Operand ) {
	  // create temporary for the index value
	  declNode * index_decl =
	    DismantleUtil::new_temp_decl(right->type(),
						  the_binary->coord());
	  _new_block->decls().push_back(index_decl);
	  threeAddrNode * assign_index = 
	    new threeAddrNode(new operandNode(new idNode(index_decl),
					      right->coord()), 
			      (operandNode *) right);
	  _new_block->stmts().push_back(assign_index);
	  right = new idNode(index_decl);
	}

	assert(left->typ() == Operand && ( right->typ() == Id 
					   || right->typ() == Const ) );
	((operandNode *) left)->index((indexNode *)right);
	// don't clone, b/c left doesn't appear anywhere in the AST, yet
	// it won't until it appears in a threeAddrNode
	ret = left;
      }
    break;
    case '*': case '/': case '%': case '+': case '-': case Operator::LS:
    case Operator::RS: case '&': case '|': case '^': case Operator::LE:
    case Operator::GE: case Operator::EQ: case Operator::NE: case '<': 
    case '>':
      {
	if ( left->typ() != Operand ) {
	  assert(left->typ() == Id || left->typ() == Const);
	  left = new operandNode((indexNode *) left, left->coord());
	}
	if ( right->typ() != Operand ) {
	  assert(right->typ() == Id || right->typ() == Const);
	  right = new operandNode((indexNode *) right, right->coord()) ;
	}
	assert(left->typ() == Operand && right->typ() == Operand);
	declNode * temp_decl =
	  DismantleUtil::new_temp_decl(the_binary->type(),
						the_binary->coord());
	_new_block->decls().push_back(temp_decl);
	threeAddrNode * assign_temp =
	  new threeAddrNode(new operandNode(new idNode(temp_decl),
					    the_binary->coord()), 
			    (operandNode *) left, 
			    op->id(), 
			    (operandNode *) right);
	_new_block->stmts().push_back(assign_temp);
	ret = new idNode(temp_decl);
      }
      break;
    default:
      cout << "Unknown binary op to dismantle: " << op->id() << endl;
      break;
    }
    return ret;
  } // end if ( ord == Postorder )
  return the_binary;
}

// TODO: factor?
Node * ExpressionDismantle::at_unary(unaryNode * the_unary, 
					      Order ord) {
  if ( !_new_block || _in_type ) // we're not in an expression statement
    return the_unary;

  Operator * op = the_unary->op();
  exprNode * expr = the_unary->expr();
  exprNode * ret = the_unary;

  if ( ord == Preorder ) {
    switch ( op->id() ) {
    case Operator::SIZEOF:
      {
	if ( expr ) {
	  the_unary->sizeof_type(the_unary->expr()->type());
	  the_unary->expr(NULL);
	  return the_unary;
	}
      }
    break;
    }
  }

  if ( ord == Postorder ) {
    switch ( op->id() ) {
    case Operator::SIZEOF:
      {
	assert(the_unary->expr() == NULL && the_unary->sizeof_type() != NULL);
	declNode * unary_temp_decl =
	  DismantleUtil::new_temp_decl(the_unary->type(),
						the_unary->coord());
	_new_block->decls().push_back(unary_temp_decl);
	threeAddrNode * assign_unary_temp =
	  new threeAddrNode(new operandNode(new idNode(unary_temp_decl),
					    the_unary->coord()),
			    the_unary->sizeof_type(),
			    the_unary->coord());
	_new_block->stmts().push_back(assign_unary_temp);
	return new idNode(unary_temp_decl);
      }
    break;
    case Operator::UMINUS: case Operator::UPLUS: case '!': case '~':
      {
	if ( expr->typ() == Operand ) {
	  operandNode * expr_oper = (operandNode *) expr;
	  declNode * temp_decl = 
	    DismantleUtil::new_temp_decl(expr->type(),
						  expr->coord());
	  _new_block->decls().push_back(temp_decl);
	  threeAddrNode * assign_temp =
	    new threeAddrNode(new operandNode(new idNode(temp_decl),
					      expr_oper->coord()),
			      expr_oper);
	  _new_block->stmts().push_back(assign_temp);
	  expr = new idNode(temp_decl, expr->coord());
	}
	assert(expr->typ() == Id || expr->typ() == Const);
	typeNode * unary_type = the_unary->type();
	declNode * unary_temp_decl =
	  DismantleUtil::new_temp_decl(unary_type,
						expr->coord());
	_new_block->decls().push_back(unary_temp_decl);
	threeAddrNode * assign_unary_temp =
	  new threeAddrNode(new operandNode(new idNode(unary_temp_decl),
					    expr->coord()),
			    op->id(),
			    new operandNode((indexNode *) expr, 
					    expr->coord()));
	_new_block->stmts().push_back(assign_unary_temp);
	ret = new idNode(unary_temp_decl);
      }
    break;
    case Operator::ADDRESS:
      {
	if ( expr->typ() == Operand ) {
	  operandNode * expr_oper = (operandNode *) expr;
	  // ANSI Sec 6.3.4 & 6.3.3.2
	  // A cast does not yield an lvalue and the unary '&' takes
	  // either a function or lvalue (with restrictions).
	  // Therefore, we don't need to worry about:
	  // & ((cast) expr)
	  if ( expr_oper->cast() ) {
	    CBZ::SyntaxError(expr_oper->coord(),
			     "invalid lvalue in unary `&'");
	  }

	  expr_oper->addr(true);
	  ret = expr_oper;
	} else {
	  assert(expr->typ() == Id);
	  expr = new operandNode((indexNode*) expr, false, true);
	}
	ret = expr;
      }
    break;
    case Operator::INDIR:
      {
	if ( expr->typ() == Operand ) {
	  operandNode * expr_oper = (operandNode *) expr;

          if(expr_oper->cast()) {
            // * ((cast) expr)
            // =>
            // T = (cast) expr;
            // * T
	    declNode * tmp_decl =
	      DismantleUtil::new_temp_decl(expr_oper->cast(),
					   the_unary->coord());
	    _new_block->decls().push_back(tmp_decl);
	    threeAddrNode * assign_tmp =
	      new threeAddrNode(new operandNode(new idNode(tmp_decl),
						expr_oper->coord()), 
				expr_oper,
				expr_oper->coord());
	    _new_block->stmts().push_back(assign_tmp);
	    expr = expr_oper
                 = new operandNode(new idNode(tmp_decl), expr_oper->coord());
          }

	  if ( expr_oper->star() ||
               ! expr_oper->fields().empty() ||
               expr_oper->index()) {
            // case 1:
	    // **foo
	    // =>
	    // {
	    //   tmp = *foo;
	    // } (returns tmp w/ star)
	    // must create a temporary if we are already deref'ing
            //
            // case 2:
	    // *(foo.f)
	    // =>
	    // {
	    //   tmp = foo.f;
	    // } (returns tmp w/ star)
            //
            // case 3:
	    // *(foo[a])
	    // =>
	    // {
	    //   tmp = foo[a];
	    // } (returns tmp w/ star)
	    declNode * temp_decl =
	      DismantleUtil::new_temp_decl(expr->type(),
						    expr->coord());
	    _new_block->decls().push_back(temp_decl);
	    threeAddrNode * assign_temp = 
	      new threeAddrNode(new operandNode(new idNode(temp_decl), 
						expr_oper->coord()),
				expr_oper);
	    _new_block->stmts().push_back(assign_temp);
	    expr = new operandNode(new idNode(temp_decl), true, false);
	  } else if(expr_oper->addr())
            expr_oper->addr(false);
	  else
	    expr_oper->star(true);
	} else {
	  assert(expr->typ() == Id || expr->typ() == Const);
	  expr = new operandNode((indexNode *) expr, true, false);
	}
	ret = expr;
      }
    break;
    case Operator::PREINC: case Operator::POSTINC:
    case Operator::PREDEC: case Operator::POSTDEC:
      {
	// ++a => a += 1; (return a)
	// a++ => tmp = a; a += 1; (return tmp)
	// --a => a -= 1; (return a);
	// a-- => tmp = a; a -= 1; (return tmp)
	bool post_op = false;
	if ( _op_post.find(the_unary->op()->id()) !=
	     _op_post.end() ) {
	  declNode * temp_decl = 
	    DismantleUtil::new_temp_decl(the_unary->type(),
						  the_unary->coord());
	  // temp = expr;
	  exprNode * clone_expr = (exprNode *) ref_clone_changer::clone(expr,
									false);
	  exprstmtNode * assign_temp = 
	    new exprstmtNode(new binaryNode('=',
					    new idNode(temp_decl,
						       the_unary->coord()),
					    clone_expr,
					    the_unary->coord()));
	  assign_temp->expr()->type((typeNode *) 
				    ref_clone_changer::clone(the_unary->type(),
							     false));
	  _new_block->decls().push_back(temp_decl);
	  _new_block->stmts().push_back(assign_temp);
	  ret = new idNode(temp_decl, the_unary->coord());
	  post_op = true;
	}
	// TODO: is this the right type?
	constNode * one = new constNode(constant(1));
	exprstmtNode * update = 
	  new exprstmtNode(new binaryNode(_op_assign_map[op->id()], // += or -=
					  expr,
					  one,
					  the_unary->coord()));
	update->expr()->type(the_unary->type());
	_new_block->stmts().push_back(update);
	if ( !post_op )
	  ret = (exprNode *) ref_clone_changer::clone(expr, false);
      }
    break;
    default:
      cout << "Unknown unary op to dismantle: " << op->id() << endl;
      break;
    }
    return ret;
  } // end if ( ord == Postorder )
  return the_unary;
}

Node * ExpressionDismantle::at_call(callNode * the_call, Order ord) {
  if ( !_new_block || _in_type ) // we're not in an expression statement
    return the_call;

  if ( ord == Postorder ) {
    operand_list * args = new operand_list;
    for ( expr_list_p p = the_call->args().begin();
	  p != the_call->args().end();
	  p++ ) {
      operandNode * arg;
      if ( (*p)->typ() != Operand ) {
	assert((*p)->typ() == Id || (*p)->typ() == Const);
	arg = new operandNode((indexNode *) *p, (*p)->coord());
      } else
	arg = (operandNode *) *p;
      args->push_back(arg);
    }

    exprNode * call_expr = the_call->name();
    if ( call_expr->typ() == Id ) {
      call_expr = new operandNode((idNode *) call_expr, call_expr->coord());
    }
    assert(call_expr->typ() == Operand);
    operandNode * call_oper = (operandNode *) call_expr;

    // TODO: is this right for void return types?
    if ( the_call->type() && 
	 the_call->type()->is_void() ) { // the proc returns void
      threeAddrNode * call =
	new threeAddrNode(call_oper, args, the_call->coord());
      _new_block->stmts().push_back(call);
      return call; // value won't be in final code, but at_cast relies
		   // on it being present.
    } else { // the proc returns non-void
      declNode * return_decl =
	DismantleUtil::new_temp_decl(the_call->type(),
					      the_call->coord());
      _new_block->decls().push_back(return_decl);
      threeAddrNode * assign_return =
	new threeAddrNode(new operandNode(new idNode(return_decl)), call_oper,
			  args, the_call->coord());
      _new_block->stmts().push_back(assign_return);
      return new idNode(return_decl, the_call->coord());
    }
  } // end if ( ord == Postorder )
  return the_call;
}

Node * ExpressionDismantle::at_comma(commaNode * the_comma, 
					      Order ord) {
  if ( ord == Postorder ) {
    Node * last_expr = the_comma->exprs().back();
    if ( last_expr->typ() == Id || last_expr->typ() == Const ) {
      last_expr = new operandNode((indexNode *) last_expr);
    } else if ( threeAddrNode * the_3addr = 
		dynamic_cast<threeAddrNode *>(last_expr) ) {
      if ( the_3addr->lhs() )
	last_expr = (exprNode *) ref_clone_changer::clone(the_3addr->lhs(),
							  false);
      else
	return NULL;
    }
    assert(last_expr->typ() == Operand);
    return last_expr;
  } // end if ( ord == Postorder )
  return the_comma;
}

Node * ExpressionDismantle::at_cast(castNode * the_cast, Order ord) {
  if ( !_new_block || _in_type ) // we're not in an expression statement
    return the_cast;

  // TODO: should we distinguish between implicit and explicit casts?
  if ( ord == Postorder ) {
    exprNode * expr = the_cast->expr();
    if ( expr->typ() == ThreeAddr ) {
      assert( the_cast->type()->is_void() );
      return expr;
    } else if ( expr->typ() != Operand ) {
      assert(expr->typ() == Id || expr->typ() == Const);
      expr = new operandNode((indexNode *) expr);
    }
    assert(expr->typ() == Operand);
    ((operandNode *) expr)->cast(the_cast->type());
    return expr;
  }
  return the_cast;
}

// We don't want to generate operandNodes everywhere by using at_id() and
// at_const().  For field names and array indices, we would generate a
// useless operandNode on the way up from the bottom of the AST.  This
// operandNode would be discarded once we extract the indexNode from it
// to obtain the actual Node we wanted.
