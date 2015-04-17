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
#include "sue_complete_walker.h"

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------
typeNode::typeNode(NodeType typ, Type_qualifiers tq, typeNode * subtype,
		   const Coord coord)
  : Node(typ, coord),
    _type(subtype),
    _type_qualifiers(tq)
{}

// ------------------------------------------------------------
//   deep_base_type
// ------------------------------------------------------------

typeNode * typeNode::deep_base_type()
{
  typeNode * chain = this;

  while (chain->is_derived())
    chain = chain->type();

  return chain;
}

// ------------------------------------------------------------
// SetBaseType from complex-types.c
// ------------------------------------------------------------

// SetBaseType:
//   Follow chain of pointers, arrays, and functions to bottom,
//   then set the base type, which should be NULL coming in.
//
//   Example:
//         In: base=int, complex=(Ptr (Array NULL))
//                               [pointer to an array of ???]
//	 Out:  (Ptr (Array int))
//               [pointer to an array of int]
//	 In: base=void, complex=(Ptr (Func (int) NULL))
//	                        [pointer to a function from int to ???]
//	 Out:  (Ptr (Func (int) void))
//	       [pointer to a function from int to void]
//

void typeNode::set_base_type(typeNode * base)
{
  typeNode * cur_type = this;

  // -- Traverse down the derived types Ptr, Array, Func...

  while ( cur_type->is_derived() && cur_type->type() )
    cur_type = cur_type->type();

  if (cur_type->is_derived())
    cur_type->type(base);
}

typeNode * typeNode::set_base_type_and(typeNode * base)
{
  set_base_type(base);
  return this;
}

// ------------------------------------------------------------
// FinishType from complex-types.c
// ------------------------------------------------------------
//   FinishType performs consistency checks that can't be conveniently 
//   expressed in the grammar, some time after the type has been
//   constructed.  It is called for both declarations and type names
//   (such as in a cast or sizeof expression). 

//   WARNING:  FinishType may be run more than once on a type, so it
//   should not blindly make unnecessary changes.

void typeNode::finish()
{
  typeNode * deepbasetype = deep_base_type();
  Type_qualifiers basetq = deepbasetype->type_qualifiers();

  if (basetq & INLINE) {
    if (CBZ::ANSIOnly)
      CBZ::SyntaxError("inline keyword not allowed with -ansi switch");
    else
      if (typ() != Func)
	CBZ::Warning(1, coord(),
		     "inline qualifier applies only to functions");
      else
	// -- If the basetype has inline, add it to the current type...
	add_type_qualifiers(INLINE);

    // -- Remove it from the base type
    deepbasetype->remove_type_qualifiers(INLINE);
  }
}

typeNode * typeNode::finish_and()
{
  finish();
  return this;
}

// ------------------------------------------------------------
//   verify_sue_complete
// ------------------------------------------------------------

void typeNode::verify_sue_complete()
{
  sue_complete_walker::check(this);
}

// ------------------------------------------------------------
// Handle typedefs
// ------------------------------------------------------------

typeNode * typeNode::no_tdef_type()
{
  typeNode * out = this;

  out = out->type();
  if (out)
    out = out->follow_tdefs();

  return out;
}

typeNode * typeNode::follow_tdefs()
{
  typeNode * out = this;

  while (out && out->typ() == Tdef) {
    tdefNode * td = (tdefNode *) out;
    out = td->def();
  }

  return out;
}
// ----------------------------------------------------------------------
//
//  TypeEqual -- implements type equivalence according to K&R2 section
//               A8.10 (fix: what section in ANSI standard?)
//
//     strict_toplevel and strict_recursive control whether 
//        const and volatile (and other type qualifiers specified
//	in TQ_COMPATIBLE) are ignored:
//
//	!strict_toplevel ==> type quals are ignored when comparing
//	                     roots of type1 and type2
//        !strict_recursive => type quals are ignored when comparing
//	                     children of type1 and type2
//	
// ----------------------------------------------------------------------

typeNode * typeNode::unwind_tdefs(Type_qualifiers & the_tq)
{
  typeNode * cur = this;
  tdefNode * tdef;

  // -- Traverse typedef chain collecting type qualifiers
  the_tq = type_qualifiers();

  while (cur->typ() == Tdef) {
    tdef = (tdefNode *) cur;
    cur = tdef->def();
    the_tq = Type_qualifiers(the_tq | cur->type_qualifiers());
  }

  return cur;
}

// -- equal_to provides more control: the two booleans control whether or
//    not certain qualifiers are taken into account when comparing the
//    current node (strict_toplevel) and sub-nodes (strict_recursive).

bool typeNode::equal_to(typeNode * node1, typeNode * node2,
			bool strict_toplevel, bool strict_recursive)
{
  Type_qualifiers tq1 = NONE;
  Type_qualifiers tq2 = NONE;

  assert(node1 && node2);

  node1 = node1->unwind_tdefs(tq1);
  node2 = node2->unwind_tdefs(tq2);

  if (!strict_toplevel) {
    // -- Remove any qualifiers that do not affect type compatibility...
    tq1 = Type_qualifiers(tq1 & COMPATIBLE);
    tq2 = Type_qualifiers(tq2 & COMPATIBLE);
  }
  if (tq1 != tq2)
    return false;
  if (node1->typ() != node2->typ())
    // pnav 
    // check if comparing an array to a pointer
    // if so, fall through to the virtual type comparison call
    if ((node1->typ() != Ptr && node1->typ() != Array)
	|| (node2->typ() != Array && node2->typ() != Ptr))
      return false;

  // -- Virtual: call the appropriate type comparison...
  return node1->qualified_equal_to(node2, strict_toplevel, strict_recursive);
}

// --- Default case: fail

bool typeNode::qualified_equal_to(typeNode * node2, bool strict_toplevel, bool strict_recursive)
{
  CBZ::Fail(__FILE__, __LINE__, "type_equal(): Unrecognized type.");
  return false;
}

// ------------------------------------------------------------
//  Type qualifier
// ------------------------------------------------------------

string typeNode::type_qualifiers_name(Type_qualifiers the_tq)
{
  string out;
  bool not_empty = false;

  if (the_tq & CONST) {
    out += "const";
    not_empty = true;
  }

  if (the_tq & VOLATILE) {
    if (not_empty)
      out += " ";
    out += "volatile";
    not_empty = true;
  }

  if (the_tq & INLINE) {
    if (not_empty)
      out += " ";
    out += "inline";
    not_empty = true;
  }

  return out;
}

// ------------------------------------------------------------
// Type conversions
// ------------------------------------------------------------

typeNode * typeNode::integral_promotions(typeNode * old_type)
{
  if (old_type->typ() == Prim) {
    primNode * p = (primNode *) old_type;

    basic_type & b = p->basic();

    if (b.is_char()) {
      if (b.is_unsigned())
	return (typeNode *) new primNode(basic_type::UInt);
      else
	return (typeNode *) new primNode(basic_type::SInt);
    }

    if (b.is_int() && b.is_short()) {
      if (b.is_unsigned())
	return (typeNode *) new primNode(basic_type::UInt);
      else
	return (typeNode *) new primNode(basic_type::SInt);
    }

    // This is added in only for convenience

    if (b.is_float())
      return (typeNode *) new primNode(basic_type::Double);
  }

  return 0;
}

pair<typeNode *, typeNode *>
typeNode::usual_arithmetic_conversions(typeNode * left,
				       typeNode * right)
{
  pair<typeNode *, typeNode *> out;
  basic_type lbasic;
  basic_type rbasic;

  out.first = 0;
  out.second = 0;

  // -- Get the basic types, if possible

  if (left->typ() == Prim)
    lbasic = ((primNode *)left)->basic();

  if (right->typ() == Prim)
    rbasic = ((primNode *)right)->basic();

  // -- Apply the rules...

  // -- If either type is long double, the other is converted to long double

  if (lbasic == basic_type::LongDouble) {
    out.second = new primNode(basic_type::LongDouble);
    return out;
  }

  if (rbasic == basic_type::LongDouble) {
    out.first = new primNode(basic_type::LongDouble);
    return out;
  }

  // -- If either type is double, the other is converted to double

  if (lbasic == basic_type::Double) {
    out.second = new primNode(basic_type::Double);
    return out;
  }

  if (rbasic == basic_type::Double) {
    out.first = new primNode(basic_type::Double);
    return out;
  }

  // -- If either type is float, the other is converted to float

  if (lbasic == basic_type::Float) {
    out.second = new primNode(basic_type::Float);
    return out;
  }

  if (rbasic == basic_type::Float) {
    out.first = new primNode(basic_type::Float);
    return out;
  }

  // -- Perform the integral promotions...

  out.first  = integral_promotions(left);
  out.second = integral_promotions(right);

  if (out.first)
    lbasic = ((primNode *)left)->basic();

  if (out.second)
    rbasic = ((primNode *)right)->basic();

  // -- The integral rules...

  // -- If either type is unsigned long, the other is converted to unsigned long

  if (lbasic == basic_type::ULong) {
    out.second = new primNode(basic_type::ULong);
    return out;
  }

  if (rbasic == basic_type::ULong) {
    out.first = new primNode(basic_type::ULong);
    return out;
  }

  // -- If one type is unsigned int and the other is long int,
  //    convert both to unsigned long (to be safe)

  if (((lbasic == basic_type::SLong) &&
       (rbasic == basic_type::UInt)) ||
      ((lbasic == basic_type::UInt) &&
       (rbasic == basic_type::SLong)))
    {
      out.first  = new primNode(basic_type::ULong);
      out.second = new primNode(basic_type::ULong);
      return out;
    }

  // -- If either type is long, the other is converted to long

  if (rbasic == basic_type::SLong) {
    out.first = new primNode(basic_type::SLong);
    return out;
  }

  if (lbasic == basic_type::SLong) {
    out.second = new primNode(basic_type::SLong);
    return out;
  }

  // -- If either type is unsigned int, the other is converted to unsigned int

  if (rbasic == basic_type::UInt) {
    out.first = new primNode(basic_type::UInt);
    return out;
  }

  if (lbasic == basic_type::UInt) {
    out.second = new primNode(basic_type::UInt);
    return out;
  }

  return out;
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void typeNode::output(output_context & ct, Node * parent)
{
  output_type(ct, parent, Left, type_qualifiers());
  output_type(ct, parent, Right, type_qualifiers());
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

typeNode::~typeNode()
{
}
