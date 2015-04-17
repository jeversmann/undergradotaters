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

#include "c_breeze.h"
#include "inliner.h"
#include "ref_clone_changer.h"

// ------------------------------------------------------------
// return_changer
// ------------------------------------------------------------
// -- Return changer converts return statements into an assignment
// (which captures the return value) and a goto (which jumps to the
// end of the inlined procedure).

class return_changer : public Changer
{
private:

  labelNode * _end_label;
  declNode * _res_var;
  bool _is_void;

public:

  return_changer(labelNode * l, declNode * r, bool is_void)
    : Changer(Postorder, Subtree, true),
      _end_label(l),
      _res_var(r),
      _is_void(is_void)
  {}

  virtual Node * at_return(returnNode * the_return, Order ord);
};

Node * return_changer::at_return(returnNode * the_return, Order ord)
{
  gotoNode * g = new gotoNode(_end_label);

  if (the_return->expr() && ! _is_void) {
    blockNode * b = new blockNode(0,0);
    binaryNode * bin = new binaryNode('=',
				      new idNode(_res_var),
				      the_return->get_expr());
    b->stmts().push_back(new exprstmtNode(bin));
    b->stmts().push_back(g);
    return b;
  }
  else
    return g;
}

// ------------------------------------------------------------
// inline_changer
// ------------------------------------------------------------

// Takes three arguments:
//     call : the callNode site to inline.
//     proc : the procNode (from call->proc()).
//     bin  : the binary assignment for functions that return a value,
//            or null. The two cases look like this:
//                  expr = func(...);
//                     or
//                  func(...);

blockNode * inliner::build_body(callNode * call,
				procNode * proc,
				binaryNode * bin)
{
  funcNode * func = (funcNode *) proc->decl()->type();

  // -- Clone the procedure

  procNode * dup_proc = (procNode *) ref_clone_changer::clone(proc, true);

  // -- Remove the body...this is the return object.

  blockNode * body = dup_proc->get_body();

  // -- Fix the parameter passing: remove each formal parameter from
  // the funcNode, use the actual argument from the callNode as the
  // initializer, and add it to the block declarations of the body.

  //    Formal arguments...

  funcNode * fd = (funcNode *) dup_proc->decl()->type();
  decl_list & formal_args = fd->args();

  //    Local declarations...

  decl_list & topdecls = body->decls();

  //    Clone the call so we can canibalize the actual arguments...

  callNode * dup_call = (callNode *)  ref_clone_changer::clone(call, false);
  expr_list & actual_args = dup_call->args();

  while (! formal_args.empty()) {
    declNode * one_arg = formal_args.back();
    exprNode * one_act = actual_args.back();

    one_arg->init(one_act);
    topdecls.push_front(one_arg);

    formal_args.pop_back();
    actual_args.pop_back();
  }

  //delete dup_proc;
  //delete dup_call;

  // -- Generate a result-value variable

  bool is_void = func->returns()->is_void();
  idNode * call_id = (idNode *) call->name();

  char buf[200];
  declNode * res_var = 0;

  if (! is_void) {
    sprintf(buf,"RES_%s", call_id->name().c_str(), call->coord().line());
    typeNode * t = (typeNode *) ref_clone_changer::clone(func->returns(), false);
    res_var = new declNode(buf, declNode::NONE, t,
			   (exprNode *)0, (exprNode *)0);

    // -- Add it's declaration to the body

    body->decls().push_front(res_var);
  }

  // -- Generate an END label

  sprintf(buf,"END_%s_%d", call_id->name().c_str(), call->coord().line());
  stmtNode * end_s = 0;
  labelNode * end_label = new labelNode(buf, end_s);

  // -- Fix the return statements

  return_changer rc(end_label, res_var, is_void);
  body = (blockNode *) body->change(rc);

  // -- Add the end label

  body->stmts().push_back(end_label);

  // -- Rebuild the assignment (if there was one) using the original
  // LHS and the result value variable as the RHS.

  if (bin) {
    exprNode * lhs = (exprNode *) ref_clone_changer::clone(bin->left(), false);
    binaryNode * new_bin = new binaryNode(bin->op()->id(),
					  lhs,
					  new idNode(res_var));
    exprstmtNode * bins = new exprstmtNode(new_bin);
    body->stmts().push_back(bins);
  }

  // textNode * t = new textNode(string("/* -- Inline: ") +  call_id->name() +
  //     string(" */"));

  return body;
}

blockNode * inliner::one_call(exprstmtNode * the_exprstmt)
{
  exprNode * e = the_exprstmt->expr();
  callNode * call = 0;
  blockNode * ib = 0;
  binaryNode * bin = 0;

  if (! e)
    return 0;

  // -- Figure out which case we have:
  //       expr = func();
  //   or
  //       func();

  if (e->typ() == Call)
    call = (callNode *) e;
  else
    if (e->typ() == Binary) {
      bin = (binaryNode *) e;
      if (bin->op()->is_assignment() &&
	  (bin->right()->typ() == Call))
	call = (callNode *) bin->right();
    }

  if (call) {
    procNode * proc = call->proc();

    if (proc) {
      // -- If the function is "inline", and it has not already been
      // inlined (prevent recursive inline)...

      funcNode * func = (funcNode *) proc->decl()->type();

      if (func->type_qualifiers() & typeNode::INLINE)
	ib = build_body(call, proc, bin);
    }
  }

  return ib;
}
