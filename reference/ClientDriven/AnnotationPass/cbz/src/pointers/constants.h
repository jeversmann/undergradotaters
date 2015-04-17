// $Id: constants.h,v 1.17 2003/08/08 15:16:29 toktb Exp $
// ----------------------------------------------------------------------
//
//  C-Breeze
//  C Compiler Framework
// 
//  Copyright (c) 2003 University of Texas at Austin
// 
//  Samuel Z. Guyer
//  Adam Brown
//  Teck Bok Tok
//  Paul Arthur Navratil
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


#ifndef CBZ_CONSTANTS_H
#define CBZ_CONSTANTS_H

#include "pointeroptions.h"
#include "location.h"
#include "memoryblock.h"
#include "pointervalue.h"

class constantAnalyzer
{
private:

  typedef set< constant > constant_set;
  typedef constant_set::iterator constant_set_p;

  typedef map< exprNode *, const constant * > expr_value_map;
  typedef expr_value_map::iterator expr_value_map_p;

  typedef map< stmtNode *, const constant * > stmt_value_map;
  typedef stmt_value_map::iterator stmt_value_map_p;

  /** @brief The set of all constants
   *
   * This set ensures that each unique constant value is represented by
   * exactly one constant object. It "owns" all of the constant values. All
   * other data structures refer to these. */

  TREE constant_set _schar_constants;
  TREE constant_set _uchar_constants;
  TREE constant_set _sshort_constants;
  TREE constant_set _ushort_constants;
  TREE constant_set _sint_constants;
  TREE constant_set _uint_constants;
  TREE constant_set _slong_constants;
  TREE constant_set _ulong_constants;
  TREE constant_set _float_constants;
  TREE constant_set _double_constants;

  /** @brief Null object
   *
   * Keep this around so that we handle null pointers properly. */

  REF memoryBlock * _null;

  /** @brief Unique lattice top element */

  const constant * _top;

  /** @brief Unique lattice bottom element */

  const constant * _bottom;

  /** @brief Special non-null element */

  const constant * _non_null;

  /** @brief Record the value of each expression
   *
   * This ends up as the context insensitive information */

  expr_value_map _expr_values;
  stmt_value_map _stmt_values;

public:

  /** @brief Create a new constant analysis */

  constantAnalyzer();

  /** @brief Destructor */

  ~constantAnalyzer();

  /** @brief Clear
   *
   * Prepare for a new analysis. */

  void clear();

  /** @brief Set the null object
   *
   * The pointer analyzer needs to provide this so that we can properly
   * handle null pointers as constants. */

  void set_null_object(memoryBlock * null_object) { _null = null_object; }

  /** @brief TOP value */

  inline const constant * top() const { return _top; }

  /** @brief BOTTOM value */

  inline const constant * bottom() const { return _bottom; }

  /** @brief Has value
   *
   * Return true if the constant is not top, bottom, or non-null */

  inline bool has_value(const constant * val)
  {
    return ((val != _top) && (val != _bottom) && (val != _non_null));
  }

  /** @brief Check for truth value
   *
   * Return true if the given constant is true (including non-null). */

  bool has_truth_value(const constant * val, bool & value);

  /** @brief Lookup a constant value
   *
   * All constants are stored in a set so that there is only one constant
   * object for each unique value. */

  const constant * lookup(const constant & value);

  /** @brief Lookup a variable value
   *
   * Look for the constant value associated with a specific def. Default is
   * TOP if none is found. */

  const constant * lookup_flowvalue(memoryDef * def);

  /** @brief Update a variable value
   *
   * Don't record any TOP values (because lookup_flowvalue() will return
   * TOP by default). Also, return a boolean indicating if the update
   * resulted in a change. */

  bool update_flowvalue(const constant * val, memoryDef * def);

  /** @brief Meet function */

  const constant * meet(const constant * one, const constant * two);

  /** @brief Rebuild the flow value
   *
   * Collect the current values of the given pointer using the meet
   * function */

  const constant * rebuild_flowvalue(pointerValue & pointer);

  /** @brief Record the value of an expression */

  void record_expression(exprNode * expr, const constant * val);

  /** @brief Record the rhs value of an threeAddr */

  void record_stmt(stmtNode * stmt, const constant * val);

  /** @brief Evaluate points-to set
   *
   * Determine the constant value of a pointer by looking at what it points
   * to. There are basically three cases: (1) it only points to the NULL
   * object, in which case it is zero, (2) it points to heap objects, which
   * might fail to be allocated, so the value is bottom, or (3) it points
   * to regular variables, whose addresses cannot be zero, so the value is
   * non-null. */

  const constant * evaluate_points_to(const memoryblock_set & points_to);

public:

  /** @brief Look up an expression */

  const constant * lookup_expression(exprNode * expr);
  const constant * lookup_stmt(stmtNode * expr);

  // --- Transfer functions -----------------------

  // -- Non-pointer expressions

  void at_id(stmtLocation * current, idNode * id,
		     pointerValue & value);

  void at_unary(stmtLocation * current,
		stmtNode *t,
		pointerValue & operand,
		pointerValue & result);

  void at_sizeof(stmtLocation * current,
		 threeAddrNode *t,
		 pointerValue & operand,
		 pointerValue & result);

  void at_binary(stmtLocation * current,
		 stmtNode *s,
		 pointerValue & left,
		 pointerValue & right,
		 pointerValue & result);

  void at_const(stmtLocation * current, constNode * cons,
			pointerValue & result);

  // -- Pointer expressions

  void at_address(stmtLocation * current,
		  operandNode * operand,
                  pointerValue & result);

  void at_cast(stmtLocation * current,
               operandNode *operand,
               pointerValue & operand_val,
               pointerValue & result);

  void at_dereference(stmtLocation * current,
                      operandNode *operand,
                      pointerValue & result);

  void at_field_access(stmtLocation * current,
                       operandNode *operand,
                       pointerValue & result);

  void at_index(stmtLocation * current,
                operandNode *operand,
                pointerValue & result);

  // -- Assignments

  void at_assignment(stmtLocation * current,
			     pointerValue & left,
			     pointerValue & right,
			     pointerValue & result,
			     memoryblock_set & changes);

  void at_parameter_pass(Location * current,
				 pointerValue & left,
				 pointerValue & right,
				 memoryblock_set & changes);

  void at_self_assignment(Location * source,
				  Location * target,
				  memoryBlock * block,
				  memoryblock_set & changes);

  // -- Statement types

  void at_return(stmtLocation * stmt,
			 returnNode * ret,
			 pointerValue & result,
			 pointerValue & return_val);

  // -- Process a merge point

  void at_merge(Location * where,
			memoryBlock * block,
			memoryuse_list & phi_uses,
			pointerValue & result,
			memoryblock_set & changes);

  // -- Control-flow options

  void at_basicblock_entry(basicblockLocation * block,
				   procedureInfo * info,
				   pointerValue & initial);

  void at_stmt_entry(stmtLocation * stmt,
			     pointerValue & result);

  // -- Procedure boundaries

  void at_conservative_procedure_call(stmtLocation * current,
					      operandNode * call,
                                              operand_list & args,
					      pointerValue & call_target,
					      pointervalue_list & arguments,
					      memoryblock_set & reachable_blocks,
					      pointerValue & return_val,
					      memoryblock_set & changes);

  // --- Output ---------------------

  void print(ostream & o, const constant * val) {
    if (val == _bottom)
      o << "(bottom)";
    else
      if (val == _top)
	o << "(top)";
      else {
	constant temp = *val;
	o << temp.to_string();
      }
  }

  string to_string(const constant * val) {
    if (val == _bottom)
      return string("(bottom)");
    else
      if (val == _top)
	return string("(top)");
      else
	if (val == _non_null)
	  return string("(non-null)");
	else {
	  constant temp = *val;
	  return temp.to_string();
	}
  }


private:

  /** @brief Obsolete: Record a value change */
  
  void record_change(memoryDef * def);


};


class constantsChanger : public Changer
{
public:

  static void optimize(unitNode * u,
		       constantAnalyzer * constants,
		       bool simplify)
  {
    constantsChanger ipcc(constants, simplify);
    u->change(ipcc);
  }

private:

  /** @brief Constants information */

  constantAnalyzer * _constants;

  /** @brief Simplify
   *
   * When true, the changer also performs control simplification by
   * resolving conditional branches whose outcome can be determined. */

  bool _simplify;

  /** @brief Private: create a new changer object */

  constantsChanger(constantAnalyzer * constants, bool simplify);

public:

  Node * at_id(idNode * id, Order ord);
  Node * at_threeAddr(threeAddrNode * operand, Order ord);
  Node * at_conditiongoto(conditiongotoNode * C, Order ord);
};

#endif // CBZ_CONSTANTS_H
