class bdwySimplifier :public Changer
{
public:

  static void fix(unitNode * node)
  {
    bdwySimplifier simp;
    node->change(simp);

    if (simp._count > 0)
      cout << "Simplifier: removed " << simp._count << " branches from " << node->input_file() << endl;
  }

  bdwySimplifier()
    : Changer( Preorder, Subtree, true),
      _count(0)
  {}

  Node * at_if(ifNode * the_if, Order ord)
  {
    bool is_constant = false;
    bool which_branch;

    // -- Get the value of the condition

    exprNode * the_expr = the_if->expr();
    the_expr->eval();
    if ( ! the_expr->value().no_val()) {
      is_constant = true;
      which_branch = the_expr->value().Boolean();
    }

    // -- If we can determine the outcome, replace the condition with the
    // appropriate branch.

    if (is_constant) {
      blockNode * branch = which_branch ? the_if->get_true_br() : the_if->get_false_br();

      /*
      cout << "FOUND: constant branch at " << the_if->coord() << endl;
      output_context oc(cout);
      the_if->output(oc,0);
      cout << endl;
      */

      _count++;

      if (branch)
	return branch;
      else
	return new exprstmtNode(0);
    } else
      return the_if;
  }

private:

  int _count;

};
