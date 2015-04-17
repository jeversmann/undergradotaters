
#include "c_breeze.h"
#include "print_walker.h"
#include "constants.h"

constantAnalyzer::constantAnalyzer()
  : _schar_constants(),
    _uchar_constants(),
    _sshort_constants(),
    _ushort_constants(),
    _sint_constants(),
    _uint_constants(),
    _slong_constants(),
    _ulong_constants(),
    _float_constants(),
    _double_constants(),
    _null(0),
    _top(new constant()),
    _bottom(new constant()),
    _non_null(new constant()),
    _expr_values()
{}

void constantAnalyzer::clear()
{
  _expr_values.clear();
}

constantAnalyzer::~constantAnalyzer()
{
  /*
  if (pointerOptions::Verbose_constants)
    cout << "Number of unique constants: " << _constants.size() << endl;
  */

  delete _top;
  delete _bottom;
  delete _non_null;
}

/** @brief Check for truth value
 *
 * Return true if the given constant is true (including non-null). */

bool constantAnalyzer::has_truth_value(const constant * val, bool & value)
{
  if (val == _non_null) {
    value = true;
    return true;
  }

  if ((val == _top) ||
      (val == _bottom))
    {
      return false;
    }

  
  value = val->Boolean();
  return true;
}

const constant * constantAnalyzer::lookup(const constant & value)
{
  pair< constant_set_p, bool > result;

  bool found_type = false;

  const basic_type & bt = value.basic();

  if (bt == basic_type::SChar) {
    result = _schar_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::UChar) {
    result = _uchar_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::SShort) {
    result = _sshort_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::UShort) {
    result = _ushort_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::SInt) {
    result = _sint_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::UInt) {
    result = _uint_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::SLong) {
    result = _slong_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::ULong) {
    result = _ulong_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::Float) {
    result = _float_constants.insert(value);
    found_type = true;
  }

  if (bt == basic_type::Double) {
    result = _double_constants.insert(value);
    found_type = true;
  }

  if (found_type) {
    constant_set_p p = result.first;
    return & (*p);
  }
  else
    return _bottom;
}

const constant * constantAnalyzer::lookup_flowvalue(memoryDef * def)
{
  // -- No def -> bottom

  if (def == 0)
    return _bottom;

  // -- No current value -> top

  const constant * result = def->value();

  if ( ! result)
    result = _bottom;

  // -- Special case for pointers

  const memoryblock_set & points_to = def->points_to();

  if (points_to.size() > 0)
    result = evaluate_points_to(points_to);

  return result;
}  

bool constantAnalyzer::update_flowvalue(const constant * val, memoryDef * def)
{
  bool change = false;

  // -- NOTE: Don't use lookup_flow() because it handles the special
  // pointer case, which can cause the constants analysis not to converge.

  // const constant * old = lookup_flowvalue(def);

  const constant * old = def->value();

  if ( ! old)
    old = _top;

  // -- No value: use top

  if (! val)
    val = _top;

  // -- Handle additive updates

  // All are additive: if (def->is_additive())
  const constant * res = meet(val, old);

  if (res != _top) {
    def->set_value(res);
    change = (old != res);

    if (pointerOptions::Verbose_constants) {
      cout << "  + Update value of " << def->owner()->name()
	   << ": old = " << to_string(old) 
	   << ", new = " << to_string(val) 
	   << ", res = " << to_string(res) << endl;
    }
  }

  return change;
} 

const constant * constantAnalyzer::meet(const constant * one, const constant * two)
{
  // Case 1 : BOTTOM ^ <anything> = BOTTOM

  if ((one == _bottom) ||
      (two == _bottom))
    return _bottom;

  // Case 2 : TOP ^ <value> = <value>

  if (one == _top)
    return two;

  if (two == _top)
    return one;

  // Case 4 : NON-NULL ^ NON-NULL = NON-NULL

  if ((one == _non_null) &&
      (two == _non_null))
    return _non_null;

  // Case 5: NON_NULL ^ (!0) = NON_NULL

  if (one == _non_null) {
    if (two->is_zero())
      return _bottom;
    else
      return _non_null;
  }

  if (two == _non_null) {
    if (one->is_zero())
      return _bottom;
    else
      return _non_null;
  }

  // Case 6 : <value1> ^ <value2> = <value1>, if value1 == value2

  if (one == two)
    return one;

  return _bottom;
}

const constant * constantAnalyzer::rebuild_flowvalue(pointerValue & pointer)
{
  const constant * result = _top;

  if (pointerOptions::Verbose_constants)
    cout << "  + Rebuild constant:" << endl;

  if (pointer.is_address)
    result = evaluate_points_to(pointer.blocks);
  else
    if (pointer.blocks.empty())
      result = _bottom;
    else {
      for (memoryblock_set_p p = pointer.blocks.begin();
	   p != pointer.blocks.end();
	   ++p)
	{
	  memoryBlock * block = *p;

	  if (block->decl()->decl_location() == declNode::ENUM) {
	    declNode * ed = block->decl();
	    exprNode * in = ed->init();
	    if (! in)
	      cout << "No init for " << block->name() << " decl = 0x" << ed << endl;
	    else {
	      const constant * enum_val = lookup(in->value());

	      if (pointerOptions::Verbose_constants)
		cout << "    - Enum " << block->name() << " = " << to_string(enum_val) << endl;

	      result = meet(result, enum_val);
	    }
	  }
	  else {

	    memoryUse * use = block->current_use();
	    if (use) {

	      // -- Get the value associated with the reaching def
	    
	      const constant * val = 0;

	      memoryDef * def = use->reaching_def();
	      if (def)
		val = lookup_flowvalue(def);
	      else
		val = bottom();

	      // -- Accumulate this value into the result

	      result = meet(result, val);

	      if (pointerOptions::Verbose_constants) {
		cout << "    - Value of " << block->name() << " = " << to_string(val) << endl;
		cout << "      + use at " << * (use->where());

		if (def)
		  cout << " def at " << * (def->where()) << endl;
		else
		  cout << " (no def)" << endl;
	      }
	    }
	  }
	}
    }
  
  if (pointerOptions::Verbose_constants)
    cout << "  => " << to_string(result) << endl;

  return result;
}

void constantAnalyzer::record_expression(exprNode * expr, const constant * val)
{
  if (val && (val != _top)) {
    expr_value_map_p p = _expr_values.find(expr);
    if (p == _expr_values.end())
      _expr_values[expr] = val;
    else
      _expr_values[expr] = meet((*p).second, val);

    if (pointerOptions::Verbose_constants) {

      const constant * newval = _expr_values[expr];

      cout << "  + Record constant expression = " << to_string(val) << endl;
      cout << "      - Accumulated value = " << to_string(newval) << endl;
    }
  }
}

/** @brief Evaluate points-to set
 *
 * Determine the constant value of a pointer by looking at what it points
 * to. There are basically three cases: (1) it only points to the NULL
 * object, in which case it is zero, (2) it points to heap objects, which
 * might fail to be allocated, so the value is bottom, or (3) it points to
 * regular variables, whose addresses cannot be zero, so the value is
 * non-null. */

const constant * constantAnalyzer::evaluate_points_to(const memoryblock_set & points_to)
{
  // -- Check for special cases

  bool points_to_null = false;
  bool points_to_heap = false;
    
  for (memoryblock_set_cp p = points_to.begin();
       p != points_to.end();
       ++p)
    {
      memoryBlock * target = *p;

      if (target == _null)
	points_to_null = true;

      if (target->is_heap_object())
	points_to_heap = true;
    }

  if (points_to_null && 
      (points_to.size() == 1)) {

    // -- Null pointer; return zero

    return lookup(constant(0));
  }

  if (points_to_heap) {

    // -- Heap allocation can fail, so we return bottom

    return _bottom;
  }

  // -- Regular pointer: always non-null

  return _non_null;
}


const constant * constantAnalyzer::lookup_expression(exprNode * expr)
{
  expr_value_map_p p = _expr_values.find(expr);

  const constant * result = 0;

  if (p == _expr_values.end())
    result = _bottom;
  else
    result = (*p).second;

  if (pointerOptions::Verbose_constants) {
    if (p != _expr_values.end())
      cout << "  + Lookup constant expression = " << to_string(result) << endl;
  }

  return result;
}

// -- Non-pointer expressions

void constantAnalyzer::at_id(stmtLocation * current, idNode * id,
			     pointerValue & block)
{
  const constant * val = rebuild_flowvalue(block);
  block.constant_value = val;

  if (block.is_a_use)
    record_expression(id, val);
}

void constantAnalyzer::at_unary(stmtLocation * current,
				unaryNode * unary,
				pointerValue & operand,
				pointerValue & result)
{
  const constant * result_c = _bottom;

  if (unary->op()->id() == Operator::SIZEOF) {

    typeNode * ty;

    if (unary->expr())
      ty = unary->expr()->type();
    else
      ty = unary->sizeof_type();

    if (ty && (ty->typ() == Prim)) {
      primNode * prim = (primNode *) ty;
      basic_type & basic = prim->basic();
      constant size(basic.size());
      result_c = lookup(size);
    }
  }
  else {
    const constant * operand_c = operand.constant_value;

    if (operand_c == _non_null) {

      // -- Non-null cases

      if (unary->op()->id() == '!') {

	// -- !p is false for non-null pointers

	result_c = lookup(constant(0));
      }
      else {

	// -- Any other operator goes to bottom

	result_c = _bottom;
      }
    }
    else {

      // -- Check for top and bottom

      if ((operand_c == _bottom) ||
	  (operand_c == _top))
	result_c = operand_c;
      else {

	// -- Compute

	constant unary_result = constant::eval(unary->op(), * operand_c);
	result_c = lookup(unary_result);
      }
    }
  }

  result.constant_value = result_c;

  if (result.is_a_use)
    record_expression(unary, result_c);
}

void constantAnalyzer::at_binary(stmtLocation * current,
				 binaryNode * binary,
				 pointerValue & left,
				 pointerValue & right,
				 pointerValue & result)
{
  const constant * result_c = _bottom;    
  const constant * left_c = left.constant_value;
  const constant * right_c = right.constant_value;

  if ((left_c == _non_null) ||
      (right_c == _non_null))
    {
      // -- Non-null cases

      switch (binary->op()->id()) {

      case '+':
      case '-':

	// -- Pointer arithmetic is a no-op

	result_c = _non_null;
	break;

      case Operator::EQ:

	// -- Compare: non_null == <value>. Two cases: (1) non_null == 0 is
	// false, (2) non_null == <other> is bottom.

	if (left_c->is_zero() ||
	    right_c->is_zero())
	  result_c = lookup(constant(0));
	else
	  result_c = _bottom;
	break;

      case Operator::NE:

	// -- Compare: non_null != <value>. Two cases: (1) non_null != 0 is
	// true, (2) non_null == <other> is bottom.

	if (left_c->is_zero() ||
	    right_c->is_zero())
	  result_c = lookup(constant(1));
	else
	  result_c = _bottom;
	break;

      default:
	result_c = _bottom;
      }
    }
  else
    {
      // -- Check for top and bottom

      if ((left_c != _bottom) &&
	  (left_c != _top) &&
	  (right_c != _bottom) &&
	  (right_c != _top))
	{
	  if ((binary->op()->id() == '/') &&
	      right_c->is_zero())
	    {
	      /*
	      cout << "WARNING: Division by zero at " << * current << endl;
	      output_context oc(cout);
	      current->stmt()->output(oc, 0);
	      cout << endl;
	      */
	    }
	  else {

	    // -- Go ahead and evaluate

	    constant binary_result = constant::eval(binary->op(), *left_c, *right_c);
	    result_c = lookup(binary_result);
	  }
	}
    }
  
  result.constant_value = result_c;

  if (result.is_a_use)
    record_expression(binary, result_c);
}

void constantAnalyzer::at_cast(stmtLocation * current, castNode * cast,
			       pointerValue & operand,
			       pointerValue & result)
{
  const constant * result_c = _bottom;
  const constant * operand_c = operand.constant_value;


  if (operand_c) {

    // -- Non-null value just propagates

    if (operand_c == _non_null)
      {
	result_c = _non_null;
      }
    else {

      // -- Check for top and bottom

      if ((operand_c == _bottom) ||
	  (operand_c == _top))
	result_c = operand_c;
      else {

	// -- Compute

	typeNode * type = cast->type();

	if (type->typ() == Prim) {
	  primNode * prim = (primNode *) type;
	  constant cast_result = constant::cast(prim->basic(), * operand_c);
	  result_c = lookup(cast_result);
	}
	else
	  result_c = operand_c;
      }
    }
  }

  result.constant_value = result_c;

  if (result.is_a_use)
    record_expression(cast, result_c);
}

void constantAnalyzer::at_const(stmtLocation * current, constNode * cons,
				pointerValue & result)
{
  // -- Skip strings

  if (cons->value().is_str()) {
    if (pointerOptions::Verbose_constants)
      cout << "String constant; skipping" << endl;
    result.constant_value = _bottom;
  }
  else {
    typeNode * type = cons->type();
    if (type->typ() == Prim) {

      primNode * prim = (primNode *) type;
      result.constant_value = lookup(constant::cast(prim->basic(), cons->value()));
    }
    else
      result.constant_value = lookup(cons->value());
  }
}

  // -- Pointer expressions

void constantAnalyzer::at_field_access(stmtLocation * current,
				       binaryNode * binary,
				       pointerValue & operand,
				       idNode * field,
				       pointerValue & result)
{
  const constant * val = rebuild_flowvalue(result);
  result.constant_value = val;

  if (result.is_a_use)
    record_expression(binary, val);
}

void constantAnalyzer::at_dereference(stmtLocation * current,
				      unaryNode * unary,
				      pointerValue & operand,
				      pointerValue & result)
{
  const constant * val = rebuild_flowvalue(result);
  result.constant_value = val;

  if (result.is_a_use)
    record_expression(unary, val);
}

void constantAnalyzer::at_address(stmtLocation * current,
				  unaryNode * unary,
				  pointerValue & operand,
				  pointerValue & result)
{
  result.constant_value = _bottom; // Not working: _non_null;
}

void constantAnalyzer::at_index(stmtLocation * current,
				binaryNode * binary,
				pointerValue & left,
				pointerValue & right,
				pointerValue & result)
{
  const constant * val = rebuild_flowvalue(result);
  result.constant_value = val;

  if (result.is_a_use)
    record_expression(binary, val);
}

// -- Assignments

void constantAnalyzer::at_assignment(stmtLocation * current,
				     binaryNode * binary,
				     pointerValue & left,
				     pointerValue & right,
				     pointerValue & result,
				     memoryblock_set & changes)
{
  const constant * right_c = right.constant_value;

  if (! right_c)
    right_c = _bottom;

  result.constant_value = right_c;

  for (memoryblock_set_p p = left.blocks.begin();
       p != left.blocks.end();
       ++p)
    {
      memoryBlock * block = *p;

      // -- Skip write-proctected blocks

      if ( ! block->write_protected()) {

	memoryDef * def = block->current_def();

	const constant * newval = _top;

	if (def->is_weak()) {
	  // memoryDef * previous_def = def->previous();
	  memoryUse * weak_use = block->current_use();
	  memoryDef * previous_def = weak_use->reaching_def();

	  if (previous_def)
	    newval = meet(right_c, lookup_flowvalue(previous_def));
	  else
	    newval = right_c;
	}
	else {
	  newval = right_c;
	}

	bool change = update_flowvalue(newval, def);
      
	if (change)
	  changes.insert(block);
      }
    }
}

void constantAnalyzer::at_self_assignment(Location * source,
					  Location * target,
					  memoryBlock * block,
					  memoryblock_set & changes)
{

  if ( ! block->write_protected()) {

    // -- Build the current value using the current use (which should be at
    // the source location).

    pointerValue temp(block);
    const constant * cur_val = rebuild_flowvalue(temp);

    // -- Assign it to the current def (which should be at the target
    // location).

    memoryDef * def = block->current_def();

    const constant * newval = _top;

    if (def->is_weak()) {
      // memoryDef * previous_def = def->previous();

      memoryUse * weak_use = block->current_use();
      memoryDef * previous_def =weak_use->reaching_def();

      if (previous_def)
	newval = meet(cur_val, lookup_flowvalue(previous_def));
      else
	newval = cur_val;
    }
    else {
      newval = cur_val;
    }

    bool change = update_flowvalue(newval, def);
      
    if (change)
      changes.insert(block);
  }
}


void constantAnalyzer::at_parameter_pass(Location * current,
					 pointerValue & left,
					 pointerValue & right,
					 memoryblock_set & changes)
{
  if (right.is_address)
    return;

  const constant * right_c = right.constant_value;

  if (! right_c)
    right_c = _top;

  for (memoryblock_set_p p = left.blocks.begin();
       p != left.blocks.end();
       ++p)
    {
      memoryBlock * block = *p;
      memoryDef * def = block->current_def();

      if (def) {

	bool change = update_flowvalue(right_c, def);
      
	if (change)
	  changes.insert(block);
      }
      else {
	// cout << "ERROR: no current def for " << block->name()
	//     << " at " << * current << endl;
      }
    }
}

  // -- Statement types

void constantAnalyzer::at_return(stmtLocation * stmt,
				 returnNode * ret,
				 pointerValue & result,
				 pointerValue & return_val)
{
  return_val.constant_value = result.constant_value;
}

  // -- Process a merge point

void constantAnalyzer::at_merge(Location * where,
				memoryBlock * block,
				memoryuse_list & phi_uses,
				pointerValue & result,
				memoryblock_set & changes)
{
  memoryDef * def = block->current_def();

  const constant * merged_val = _top;

  for (memoryuse_list_p p = phi_uses.begin();
       p != phi_uses.end();
       ++p)
    {
      memoryUse * phi_use = *p;
      memoryDef * reaching_def = phi_use->reaching_def();

      if (reaching_def)
	merged_val = meet(merged_val, lookup_flowvalue(reaching_def));
    }

  bool change = update_flowvalue(merged_val, def);

  if (change)
    changes.insert(block);
}

  // -- Control-flow options

void constantAnalyzer::at_basicblock_entry(basicblockLocation * block,
					   procedureInfo * info,
					   pointerValue & initial)
{
  initial.constant_value = _top;
}

void constantAnalyzer::at_stmt_entry(stmtLocation * stmt,
				     pointerValue & result)
{
  result.constant_value = _top;
}

  // -- Procedure boundaries

void constantAnalyzer::at_conservative_procedure_call(stmtLocation * current,
						      callNode * call,
						      pointerValue & call_target,
						      pointervalue_list & arguments,
						      memoryblock_set & reachable_blocks,
						      pointerValue & return_val,
						      memoryblock_set & changes)
{
  // -- At a procedure call with no source, all reachable blocks become
  // BOTTOM.

  for (memoryblock_set_p p = reachable_blocks.begin();
       p != reachable_blocks.end();
       ++p)
    {
      memoryBlock * block = *p;

      if ( ! block->write_protected()) {

	memoryDef * def = block->current_def();

	bool change = update_flowvalue(bottom(), def);
      
	if (change)
	  changes.insert(block);
      }
    }
}

// ----------------------------------------------------------------------
//  Changer
// ----------------------------------------------------------------------

constantsChanger::constantsChanger(constantAnalyzer * constants,
				   bool simplify)
  : Changer( Preorder, Subtree, true),
    _constants(constants),
    _simplify(simplify)
{}

Node * constantsChanger::at_expr(exprNode * the_expr, Order ord)
{
  // -- Look up the constant value of this expression

  const constant * val = _constants->lookup_expression(the_expr);

  // -- See if it has a recorded value

  if (_constants->has_value(val))
    {
      // -- It does, so return that value

      if (pointerOptions::Verbose_constants) {
	cout << "--- Replace constant -----------------------------------" << endl;
	output_context oc(cout);
	the_expr->output(oc,0);
	cout << endl;
	cout << val->to_string() << endl;
      }

      return new constNode( *val );
    }
  else {

    // -- There's no recorded value, but it still could be a combination of
    // arithmetic operators and constants, e.g. "5+(4/2)". Evaluate the
    // expression and check again for a constant value.

    the_expr->eval();
    if ( ! the_expr->value().no_val()) {
      constNode * out = new constNode(the_expr->value());
      return out;
    }
  }
  
  return the_expr;
}


Node * constantsChanger::at_if(ifNode * the_if, Order ord)
{
  // -- Only simplify if indicated

  if ( ! _simplify)
    return the_if;

  // -- Get the value of the condition

  exprNode * the_expr = the_if->expr();
  const constant * val = _constants->lookup_expression(the_expr);

  bool is_constant = false;
  bool which_branch;

  // -- If the variable is not TOP or BOTTOM, then we have a constant.

  if (_constants->has_truth_value(val, which_branch))
    {
      // The branch argument above gets the value

      is_constant = true;
    }
  else {

    // -- No record for this expression, but it could be a combination of
    // arithmetic and constants, e.g., "5+(4/2)". Evaluate it:

    the_expr->eval();
    if ( ! the_expr->value().no_val()) {
      is_constant = true;
      which_branch = the_expr->value().Boolean();
    }
  }

  // -- If we can determine the outcome, replace the condition with the
  // appropriate branch.

  if (is_constant) {
    blockNode * branch = which_branch ? the_if->get_true_br() : the_if->get_false_br();

    if (branch)
      return branch;
    else
      return new exprstmtNode(0);
  } else
    return the_if;
}
