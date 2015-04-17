// $Id: dismantle-init.cc,v 1.8 2003/08/28 15:26:43 toktb Exp $
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
//
// dismantle-init.cc
//

#include "c_breeze.h"
#include "dismantle.h"
#include "ref_clone_changer.h"

InitializerDismantle::InitializerDismantle(void):
  Changer(Both, Subtree, false)
{}

Node * InitializerDismantle::at_block(blockNode * the_block, Order ord) {
  if ( ord == Preorder ) {
    _blockStack.push(the_block);
  } else { // ord == Postorder
    if ( _assgblock.find(the_block) != _assgblock.end() )
      the_block->stmts().push_front(_assgblock[the_block]);
    assert(_blockStack.top() == the_block);
    _blockStack.pop();
  }
  return the_block;
}

exprNode * InitializerDismantle::eval_or_cast(exprNode * expr, typeNode * type)
{
  assert(type);
  if ( !expr->no_tdef_type()
       || *(expr->no_tdef_type()) != *type )
    expr = new castNode((typeNode *) ref_clone_changer::clone(type, false),
			expr);
  expr->eval();
  if ( !expr->value().no_val() ) {
    return new constNode(expr->value());
  }
  return expr;
}

void InitializerDismantle::init_scalar(declNode * the_decl,
				       exprNode * init) {
  init = eval_or_cast(init, the_decl->no_tdef_type());

  if ( the_decl->decl_location() == declNode::BLOCK ) {
    the_decl->init(NULL);
    idNode * id = new idNode(the_decl);
    binaryNode * assg = new binaryNode('=', id, init);
    assg->type((typeNode *) ref_clone_changer::clone(the_decl->type(),
						     false));
    blockNode * decl_block = _blockStack.top();
    if ( _assgblock.find(decl_block) == _assgblock.end() )
      _assgblock[decl_block] = new blockNode(NULL, NULL,
					     decl_block->coord(),
					     decl_block->coord());
    _assgblock[decl_block]->stmts().push_back(new exprstmtNode(assg));
  } else if ( the_decl->decl_location() == declNode::TOP ) {
    the_decl->init(init);
  }
}

initializerNode * InitializerDismantle::init_array(arrayNode * the_array, 
						   expr_list_p & val,
						   expr_list_p end,
						   bool inSublist) {
  constant array_size;
  if ( the_array->dim() ){
    the_array->dim()->eval();
    array_size = the_array->dim()->value();
  }

  int count = 1;
  initializerNode * result_init = new initializerNode(NULL);

  while ( val != end
	  && ( array_size.no_val()
	       || count <= array_size.Integer() ) ) {
    if ( arrayNode * sub_array = 
	 dynamic_cast<arrayNode *>(the_array->no_tdef_type()) ) {
      initializerNode * sub_init = NULL;
      if ( initializerNode * cur_val =
	   dynamic_cast<initializerNode *>(*val) ) {
	expr_list_p new_begin = cur_val->exprs().begin();
	sub_init = init_array(sub_array, new_begin, cur_val->exprs().end(),
			      true);
	val++;
      } else
	sub_init = init_array(sub_array, val, end, false);

      assert(sub_init);
      result_init->exprs().push_back(sub_init);
    } else if ( structNode * sub_struct =
		dynamic_cast<structNode *>(the_array->no_tdef_type()) ) {
      initializerNode * sub_init = NULL;
      if ( initializerNode * cur_val =
	   dynamic_cast<initializerNode *>(*val) ) {
	expr_list_p new_begin = cur_val->exprs().begin();
	sub_init = init_struct(sub_struct, new_begin, cur_val->exprs().end(),
			       true);
	val++;
      } else
	sub_init = init_struct(sub_struct, val, end, false);

      assert(sub_init);
      result_init->exprs().push_back(sub_init);
    } else if ( unionNode * sub_union =
		dynamic_cast<unionNode *>(the_array->no_tdef_type()) ) {
      exprNode * sub_init = NULL;
      if ( initializerNode * cur_val = dynamic_cast<initializerNode *>(*val) ) {
        assert(cur_val->exprs().size() == 1);
	sub_init = cur_val->exprs().front();
      } else
	sub_init = *val;
      val++;

      assert(sub_init);
      declNode *field1 = sub_union->spec()->fields().front();
      exprNode * initVal = eval_or_cast(sub_init, field1->no_tdef_type());
      result_init->exprs().push_back(initVal);
    } else {
      if ( initializerNode * init_expr = 
	   dynamic_cast<initializerNode *>(*val) ) {
	CBZ::Error(the_array->coord(), "Scalar initialized by list of"
		   " initial values");
      } else {
	// if this is a string, update the count accordingly
	typeNode *elt_type = the_array->no_tdef_type();
        if ( elt_type->typ() != Ptr && isStringLiteral(*val) ) {
	  const char * str = (dynamic_cast<constNode *>(*val))->value().Str();
	  count += strlen(str);
	  result_init->exprs().splice(result_init->exprs().end(),
				      strLit2Init(str)->exprs());
	} else {
	  exprNode * initVal = eval_or_cast(*val, the_array->no_tdef_type());
	  result_init->exprs().push_back(initVal);
	}
      }
      val++;
    }
    count++;
  } // end while

  if ( array_size.no_val() ) {
    the_array->dim(new constNode(constant(count-1)));
  } else { 
    while ( count <= array_size.Integer() ) {
      // initialize to zero
      if ( arrayNode * sub_array =
	   dynamic_cast<arrayNode *>(the_array->no_tdef_type()) )
	result_init->exprs().push_back(init_array(sub_array, end, end, false));
      else if ( structNode * sub_struct = 
		dynamic_cast<structNode *>(the_array->no_tdef_type()) )
	result_init->exprs().push_back(init_struct(sub_struct, end, end,
						   false));
      else if ( ptrNode * sub_ptr = 
                dynamic_cast<ptrNode *>(the_array->no_tdef_type()) ) {
        constant value(0);
        exprNode * initVal = eval_or_cast(new constNode(value), sub_ptr);
        result_init->exprs().push_back(initVal);
      } else if ( enumNode * sub_enum = 
                dynamic_cast<enumNode *>(the_array->no_tdef_type()) ) {
        constant value(0);
        exprNode * initVal = eval_or_cast(new constNode(value), sub_enum);
        result_init->exprs().push_back(initVal);
      } else if ( unionNode * sub_union = 
                dynamic_cast<unionNode *>(the_array->no_tdef_type()) ) {
        declNode *field1 = sub_union->spec()->fields().front();
        constant value(0);
        exprNode * initVal = eval_or_cast(new constNode(value),
                                          field1->no_tdef_type());
        result_init->exprs().push_back(initVal);
      } else {
	primNode * elmtType = dynamic_cast<primNode *>(the_array->type());
	assert(elmtType);
	constant value = constant::cast(elmtType->basic(), constant(0));
	result_init->exprs().push_back(new constNode(value));
      }
      count++;
    }
  }

  if ( val != end && inSublist) {
    CBZ::Error(the_array->coord(), "Too many values in initializer list");
  }

  return result_init;
}

initializerNode * InitializerDismantle::init_struct(structNode * the_struct,
						    expr_list_p & val,
						    expr_list_p end,
						    bool inSublist) {
  initializerNode * result_init = new initializerNode(NULL);

  suespecNode * the_spec = the_struct->spec();
  decl_list_p field = the_spec->fields().begin();
  decl_list_p field_end = the_spec->fields().end();

  while ( val != end
	  && field != field_end ) {
    if ( arrayNode * sub_array =
	 dynamic_cast<arrayNode *>((*field)->no_tdef_type()) ) {
      initializerNode * sub_init = NULL;
      if ( initializerNode * cur_val =
	   dynamic_cast<initializerNode *>(*val) ) {
	expr_list_p new_begin = cur_val->exprs().begin();
	sub_init = init_array(sub_array, new_begin, cur_val->exprs().end(),
			      true);
	val++;
      } else
	sub_init = init_array(sub_array, val, end, false);

      assert(sub_init);
      result_init->exprs().push_back(sub_init);
    } else if ( structNode * sub_struct =
		dynamic_cast<structNode *>((*field)->no_tdef_type()) ) {
      initializerNode * sub_init = NULL;
      if ( initializerNode * cur_val =
	   dynamic_cast<initializerNode *>(*val) ) {
	expr_list_p new_begin = cur_val->exprs().begin();
	sub_init = init_struct(sub_struct, new_begin, cur_val->exprs().end(),
			       true);
	val++;
      } else
	sub_init = init_struct(sub_struct, val, end, false);

      assert(sub_init);
      result_init->exprs().push_back(sub_init);
    } else if ( unionNode * sub_union =
		dynamic_cast<unionNode *>((*field)->no_tdef_type()) ) {
      exprNode * sub_init = NULL;
      if ( initializerNode * cur_val =
	   dynamic_cast<initializerNode *>(*val) ) {
        assert(cur_val->exprs().size() == 1);
	sub_init = cur_val->exprs().front();
      } else
	sub_init = *val;
      val++;

      assert(sub_init);
      declNode *field1 = sub_union->spec()->fields().front();
      exprNode * initVal = eval_or_cast(sub_init, field1->no_tdef_type());
      result_init->exprs().push_back(initVal);
    } else {
      if ( initializerNode * init_expr =
	   dynamic_cast<initializerNode *>(*val) ) {
	CBZ::Error(the_struct->coord(), "Scalar initialized by list of"
		   " initial values");
      } else {
	exprNode * initVal = eval_or_cast(*val, (*field)->no_tdef_type());
	result_init->exprs().push_back(initVal);
      }
      val++;
    }
    field++;
  } // end while

  // pad the end with zeroes
  while ( field != field_end ) {
    if ( arrayNode * sub_array =
	 dynamic_cast<arrayNode *>((*field)->no_tdef_type()) )
      result_init->exprs().push_back(init_array(sub_array, end, end, false));
    else if ( structNode * sub_struct = 
	      dynamic_cast<structNode *>((*field)->no_tdef_type()) )
      result_init->exprs().push_back(init_struct(sub_struct, end, end, false));
    else if ( ptrNode * sub_ptr = 
	      dynamic_cast<ptrNode *>((*field)->no_tdef_type()) ) {
      constant value(0);
      exprNode * initVal = eval_or_cast(new constNode(value), sub_ptr);
      result_init->exprs().push_back(initVal);
    } else if ( enumNode * sub_enum = 
	      dynamic_cast<enumNode *>((*field)->no_tdef_type()) ) {
      constant value(0);
      exprNode * initVal = eval_or_cast(new constNode(value), sub_enum);
      result_init->exprs().push_back(initVal);
    } else if ( unionNode * sub_union = 
	      dynamic_cast<unionNode *>((*field)->no_tdef_type()) ) {
      declNode *field1 = sub_union->spec()->fields().front();
      constant value(0);
      exprNode * initVal = eval_or_cast(new constNode(value),
                                        field1->no_tdef_type());
      result_init->exprs().push_back(initVal);
    } else {
      primNode * fieldType = dynamic_cast<primNode*>((*field)->no_tdef_type());
      assert(fieldType);
      constant value = constant::cast(fieldType->basic(), constant(0));
      result_init->exprs().push_back(new constNode(value));
    }
    field++;
  }

  if ( val != end && inSublist ) {
    CBZ::Error(the_struct->coord(), "Too many values in initializer list");
  }

  return result_init;
}

bool InitializerDismantle::isStringLiteral(exprNode * the_expr) {
  if ( constNode * the_const = dynamic_cast<constNode *>(the_expr) ) {
    return the_const->value().is_str();
  }
  return false;
}

initializerNode * InitializerDismantle::strLit2Init(const char * str) {
  initializerNode * newInit = new initializerNode(NULL);
  for ( int i = 0; i < strlen(str); i++ ) {
    char text[4];
    sprintf(text, "'%c'", str[i]);
    constNode * newConst = new constNode(constant(str[i]), text);
    newConst->type(new primNode(basic_type::Char));
    newInit->add_expr(newConst);
  }
  newInit->add_expr(new constNode(constant('\0')));
  return newInit;
}

// Dismantle "normal" initializers, like int a = 1;, into uninitialized
// declarations and assignment statements
Node * InitializerDismantle::at_decl(declNode * the_decl, Order ord) {
  if ( ord == Preorder ) {
    exprNode * initExpr = the_decl->init();
    if ( initExpr ) {
      if ( isStringLiteral(initExpr) 
	   && the_decl->no_tdef_type()->typ() == Array ) {
	// turn the string literal into an array initializer
	char * str = (dynamic_cast<constNode *>(initExpr))->value().Str();
	initializerNode * newInit = strLit2Init(str);
	initExpr = newInit;
	the_decl->init(newInit);
      }

      // TODO: add handling for union & enum?
      if ( initializerNode * the_init 
             = dynamic_cast<initializerNode *>(initExpr) ) {
	typeNode * top_type = the_decl->no_tdef_type();
	if ( top_type->is_aggregate() ) { // struct or array
	  expr_list_p val = the_init->exprs().begin();
	  initializerNode * new_init = NULL;
	  if ( arrayNode * the_array = dynamic_cast<arrayNode *>(top_type) )
	    new_init = init_array(the_array, val, the_init->exprs().end(),
				  true);
	  else if ( structNode * the_struct = 
		    dynamic_cast<structNode *>(top_type) )
	    new_init = init_struct(the_struct, val, the_init->exprs().end(),
				   true);
	  else
	    assert(false); // TODO: better error reporting

	  assert(new_init);
	  the_decl->init(new_init);
	} else if ( top_type->is_scalar() ) {
	  // it is valid to initailize a scalar with a single value, surrounded
	  // by braces.
	  if ( the_init->exprs().size() != 1 )
	    CBZ::Error(the_decl->coord(), "Too many values in initializer"
		       " list");
	  if ( the_init->exprs().front()->typ() == Initializer )
	    CBZ::Error(the_decl->coord(), "Scalar initialized with a list"
		       " of initial values");

	  init_scalar(the_decl, the_init->exprs().front());
	}
      } else { // the_init is not initializerNode
	init_scalar(the_decl, initExpr);
      } // end if ( the_init )
    } // end if ( initExpr )
  } // end if ( ord == Preorder )
  return the_decl;
}
