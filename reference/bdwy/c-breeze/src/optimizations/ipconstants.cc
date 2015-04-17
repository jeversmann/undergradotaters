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
#include "ipconstants.h"

// ------------------------------------------------------------
// ipConstant
// ------------------------------------------------------------

// This class represents a single constant value associated with one
// def of a variable. It can hold either a constant value, or have the
// special lattice TOP value. The lattice BOTTOM is represented by a
// constant no_val (which is built into the constant class).

// Constructors

ipConstant::ipConstant(constant & value)
  : _value(value),
    _top(false),
    _internal(false)
{}

ipConstant::ipConstant()
  : _value(0),
    _top(true),
    _internal(false)
{}

ipConstant::ipConstant(const ipConstant & other)
  : _value(other._value),
    _top(other._top),
    _internal(false)
{}

ipConstant::~ipConstant()
{
  if (_internal)
    cout << "ERROR: Deleting an internal ipConstant" << endl;
}

// -- Top and bottom

bool ipConstant::is_top() const
{
  return _top && (! _value.no_val());
}

bool ipConstant::is_bottom() const
{
  return _value.no_val();
}

void ipConstant::to_bottom()
{
  _value.set_no_val();
  _top = false;
}

// -- Assignment

void ipConstant::assign(const ipConstant * other)
{
  _value = other->_value;
  _top = other->_top;
}

// -- Comparison

bool ipConstant::diff(analysisVal * other) const
{
  ipConstant * ic_other = (ipConstant *) other;

  if (is_bottom() || ic_other->is_bottom())
    return is_bottom() != ic_other->is_bottom();

  if (is_top() || ic_other->is_top())
    return is_top() != ic_other->is_top();

  return ! (value().is_equal_to(ic_other->value()));
}

// -- Meet "^" operator
// Must satisfy the following:
//    Associative: x ^ (y ^ z) = (x ^ y) ^ z
//    Commutative: x ^ y = y ^ x
//    Idempotent:  x ^ x = x

void ipConstant::meet_with(const analysisVal * other)
{
  ipConstant * ic_other = (ipConstant *) other;

  /*
  cout << "Meet : ";
  print(cout);
  cout << "  ";
  ic_other->print(cout);
  cout << endl;
  */

  // Case 1 : BOTTOM ^ <anything> = BOTTOM

  if (is_bottom())
    return;

  // Case 2 : <anything> ^ BOTTOM = BOTTOM

  if (ic_other->is_bottom()) {
    to_bottom();
    return;
  }

  // Case 3 : TOP ^ <value> = <value>

  if (is_top()) {
    value() = ic_other->value();
    _top = false;
    return;
  }

  // Case 4 : <value> ^ TOP = <value>

  if (ic_other->is_top()) {
    return;
  }

  // Case 5 : <value1> ^ <value2> = <value1>, if value1 == value2

  if (value().is_equal_to(ic_other->value())) {
    return;
  }
  else
    // Case 6 : <value1> ^ <value2> = BOTTOM, if value1 != value2
    to_bottom();
}

// -- Computational functions

void ipConstant::binary_operator(const Operator * op,
				 const analysisVal * right_operand)
{
  ipConstant * ic_right = (ipConstant *) right_operand;

  /*
  cout << "Binop : ";
  print(cout);
  cout << "  ";
  ic_right->print(cout);
  cout << endl;
  */

  value() = constant::eval(op, value(), ic_right->value());
}

void ipConstant::unary_operator(const Operator * op)
{
  /*
  cout << "Unary : ";
  print(cout);
  cout << endl;
  */

  value() = constant::eval(op, value());
}

void ipConstant::cast_operator(const typeNode * type)
{
  /*
  cout << "Cast : ";
  print(cout);
  cout << endl;
  */

  if (type->typ() == Prim) {
    primNode * prim = (primNode *) type;
    value() = constant::cast(prim->basic(), value());
  }
}

void ipConstant::print(ostream & o)
{
  if (is_top())
    o << "TOP";
  else
    if (is_bottom())
      o << "BOTTOM";
    else
      o << value().to_string();
}

// ------------------------------------------------------------
// ipConstantPropagation
// ------------------------------------------------------------

// This class controls the constant propagation algorithm by holding
// the current states of all objects, looking them up when needed, and
// setting their values at assignments.

// -- Process an assignment; return true if the state of the defined
// object changes.

bool ipConstantPropagation::assignment(const Path * where,
				       memoryDef * left_hand_side,
				       analysisVal * ipa_rhs,
				       bool is_strong_update)
{
  ipConstant * ic_rhs = (ipConstant *) ipa_rhs;

  // Look up the particular def, creating one if necessary

  const_variables_map_p found = _values.find(left_hand_side);
  ipConstant * ic_lhs = 0;
  bool f;
  if (found == _values.end()) {
    ic_lhs = new ipConstant();
    allocate(ic_lhs);
    _values[left_hand_side] = ic_lhs;
    ic_lhs->intern();
    f = true;
  }
  else {
    ic_lhs = (*found).second;
    f = false;
  }

  bool diff;
  if (is_strong_update) {

    // Check to see if a change will occur

    diff = ic_lhs->diff(ic_rhs);
    ic_lhs->assign(ic_rhs);
  }
  else {

    // If not a strong update, go to bottom

    diff = ! ic_lhs->is_bottom();
    ic_lhs->assign(_bottom);
  }

  return diff;
}

// -- Lookup the flow value for an object (not a copy)

analysisVal * ipConstantPropagation::lookup(memoryBlock * block, memoryUse * use)
{
  ipConstant * out = 0;

  // For enum types, use the constant value stored on the declaration.

  if (block->decl()->decl_location() == declNode::ENUM) {
    ipConstant * res = new ipConstant(block->decl()->init()->value());
    allocate(res);
    return res;
  }

  memoryDef * def = use->reaching_def();

  if (def) {
    const_variables_map_p found = _values.find(def);
    if (found == _values.end()) {
      cout << "lookup: Could not find " << def->owner()->decl()->name() << endl;
    }
    else {
      ipConstant * con = _values[def];
      if (con) {

	/*
	cout << "lookup: ";
	con->print(cout);
	cout << endl;
	*/

	return clone(con);
      }
    }
  }

  return 0;
}

analysisVal * ipConstantPropagation::lookup(constNode * con)
{
  ipConstant * out = 0;

  /*
  constants_map_p found = _constants.find(con);
  if (found == _constants.end()) {
    out = new ipConstant(con->value());
    _count++;
    _constants[con] = out;
  }
  else
    out = (*found).second;
  */

  out = new ipConstant(con->value());
  allocate(out);

  /*
  cout << "lookup const:";
  out->print(cout);
  cout << endl;
  */

  return out;
}

analysisVal * ipConstantPropagation::lookup(const string & field_name)
{
  return 0;
}

// -- Clone

ipConstant * ipConstantPropagation::clone(analysisVal * to_clone)
{
  ipConstant * copy = new ipConstant( * ((ipConstant *) to_clone));
  allocate(copy);
  return copy;
}

// -- Free

void ipConstantPropagation::free(analysisVal * to_free)
{
  if (to_free) {
    ipConstant * ic_to_free = (ipConstant *) to_free;
  
    ipconstant_set_p p = _deleted.find(ic_to_free);
    if (p == _deleted.end()) {
      _deleted.insert(ic_to_free);

      /*
      cout << "Deallocate : (" << _count << ") ";
      ic_to_free->print(cout);
      cout << endl;
      */

      _count--;
    }
    else
      cout << "ERROR: Double delete" << endl;
  }
}

// -- Return the TOP flow value (not a copy)

analysisVal * ipConstantPropagation::top()
{
  return clone(_top);
}

// -- Return the BOTTOM flow value (not a copy)

analysisVal * ipConstantPropagation::bottom()
{
  return clone(_bottom);
}

// ------------------------------------------------------------
// Computational Operators
// ------------------------------------------------------------

analysisVal * ipConstantPropagation::binary_operator(const Operator * op,
						     const analysisVal * left_operand,
						     const analysisVal * right_operand)
{
  if ((op->id() == '.') || (op->id() == Operator::ARROW))
    return 0;

  ipConstant * left  = (ipConstant *) left_operand;
  ipConstant * right = (ipConstant *) right_operand;

  left->binary_operator(op, right);

  return left;
}

analysisVal * ipConstantPropagation::unary_operator(const Operator * op,
						    const analysisVal * operand)
{
  if ((op->id() == Operator::ADDRESS) || (op->id() == Operator::INDIR))
    return 0;

  ipConstant * oper = (ipConstant *) operand;

  oper->unary_operator(op);

  return oper;
}

analysisVal * ipConstantPropagation::cast_operator(const typeNode * type,
				      const analysisVal * operand)
{
  ipConstant * oper = (ipConstant *) operand;

  oper->cast_operator(type);

  return oper;
}

// ------------------------------------------------------------
// Stats
// ------------------------------------------------------------

void ipConstantPropagation::stats()
{
  for (const_variables_map_p p = _values.begin();
       p != _values.end();
       ++p)
    {
      memoryDef * def = (*p).first;
      ipConstant * con = (*p).second;

      cout << "Value of " << def->owner()->decl()->name();
      def->print(cout);
      cout << "  = ";
      con->print(cout);
      cout << endl;

      if (def->where()->block() != 0) {
	stmtNode * stmt = def->where()->stmt();

	/*
	  ostringstream com;
	  com << "  " << def->owner()->decl()->name() << " = ";
	  con->print(com);
	  com << endl;
	  stmt->comment() += com.str();
	*/
      }
    }

  cout << "Total count = " << _count << endl;
  cout << "Accounted for = " << _values.size() << endl;
}
