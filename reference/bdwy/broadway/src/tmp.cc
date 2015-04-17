stmtNode * generate_inline_body(callNode * call,
				binaryNode * bin)
{
  // -- Retrieve the library function, make sure there is one

  procNode * proc = call->proc();

  if ( proc ) {
    
    funcNode * func = (funcNode *) proc->decl()->type();

    // -- Clone the procedure. Here we use the ref_clone_changer because
    // it preserves the internal references (that is, the local variable
    // declaration pointers). That way we can use the name mangler to fix
    // any conflicting variable names.

    procNode * dup_proc = (procNode *) ref_clone_changer::clone(proc, false);

    // -- Remove the body...this is the return object. BUT, get the exit
    // basic block first.

    basicblockNode * exit = dup_proc->exit();
    blockNode * body = dup_proc->get_body();

    // -- Fix the parameter passing: remove each formal parameter from
    // the funcNode, use the actual argument from the callNode as the
    // initializer, and add it to the block declarations of the body.

    //    Formal arguments...

    funcNode * fd = (funcNode *) dup_proc->decl()->type();
    decl_list & formal_args = fd->args();

    //    Local declarations...

    decl_list & topdecls = body->decls();

    //    Clone the call so we can canibalize the actual arguments (memory leak?)

    callNode * dup_call = (callNode *)  ref_clone_changer::clone(call, false);
    expr_list & actual_args = dup_call->args();

    while (! formal_args.empty()) {
      declNode * one_arg = formal_args.back();
      exprNode * one_act = actual_args.back();

      // -- Use the initialize to pass the actual value

      one_arg->init(one_act);

      // -- Important: set the decl location to BLOCK

      one_arg->decl_location(declNode::BLOCK);

      topdecls.push_front(one_arg);

      formal_args.pop_back();
      actual_args.pop_back();
    }

    // -- Assumption: last statement of the exit basic block is the
    // return statement. This is true after the regular dismantle/CFG
    // phase. Remove it.

    returnNode * ret = (returnNode *) exit->stmts().back();
    exit->stmts().pop_back();

    assert(ret->typ() == Return);

    // -- If there is a returned value and a left-hand side at the
    // callsite, then set up the appropriate assignment.

    if (ret->expr() && (callsite->stmt()->expr()->typ() == Binary)) {

      // -- Get the returned value

      exprNode * returned_expr = ret->get_expr();

      // -- Copy the left-hand side of the binary node

      exprNode * lhs = (exprNode *) ref_clone_changer::clone(bin->left(), false);

      // -- Create a new assignment statement and place it at the end of
      // the basic block.

      binaryNode * assignment = new binaryNode('=', lhs, returned_expr, call->coord());
      exprstmtNode * s = new exprstmtNode(assignment);
      exit->stmts().push_back(s);
    }

    return body;
  }

  return 0;
}
