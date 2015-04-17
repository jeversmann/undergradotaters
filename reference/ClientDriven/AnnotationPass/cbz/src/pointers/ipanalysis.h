// $Id: ipanalysis.h,v 1.22 2005/06/03 02:00:48 akkartik Exp $
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

#ifndef CBZ_IPANALYSIS_H
#define CBZ_IPANALYSIS_H

#include "pointeroptions.h"
#include "proceduredb.h"

class analysisVal
{
public:
  virtual ~analysisVal() {}
  virtual analysisVal * clone() const =0;
//  virtual analysisVal * clone() const = 0;
};


class pointerValue;

typedef list< pointerValue > pointervalue_list;
// typedef pointervalue_list::iterator pointervalue_list_p;



class analysisProblem
{
protected:

  Direction _direction;

public:

  /** @brief Create a new analysis problem object
   *
   * The only purpose of this constructor is to set the direction field. */

  analysisProblem(Direction direction)
    : _direction(direction)
  {}

  /** @brief Get the direction */

  Direction direction() const { return _direction; }

  /** @brief Name of the analysis
   *
   * Just for debug purposes. */

  virtual string name() =0;

  /** @name Transfer functions
   *
   * The rest of these functions are hooks for the pointer analyzer to call
   * at various points in the analysis. They serve as the transfer
   * functions for the dataflow analysis problem. */

  //@{

  // -- Non-pointer expressions

  virtual void at_id(stmtLocation * current, idNode * id,
		     pointerValue & result)
  {}

  virtual void at_unary(stmtLocation * current,
			threeAddrNode *r,
			pointerValue & operand,
			pointerValue & result)
  {}

  virtual void at_sizeof(stmtLocation * current,
			 threeAddrNode *r,
			 pointerValue & operand,
			 pointerValue & result)
  {}

  virtual void at_binary(stmtLocation * current,
			 stmtNode *r,
			 pointerValue & left,
			 pointerValue & right,
			 pointerValue & result)
  {}

  virtual void at_cast(stmtLocation * current, operandNode *operand,
		       pointerValue & operand_value,
		       pointerValue & result)
  {}

  virtual void at_const(stmtLocation * current, constNode * cons,
			pointerValue & result)
  {}

  // -- Procedure calls

  virtual void at_call(stmtLocation * current, operandNode * call,
		       pointerValue & call_target,
		       procNode * callee,
		       pointervalue_list & arguments,
		       pointerValue & return_val)
  {}

  // -- Pointer expressions

  virtual void at_field_access(stmtLocation * current,
                               operandNode *operand,
			       pointerValue & operand_value,
			       pointerValue & result)
  {}

  virtual void at_dereference(stmtLocation * current,
                              operandNode *operand,
			      pointerValue & operand_value,
			      pointerValue & result)
  {}

  virtual void at_address(stmtLocation * current,
                          operandNode *operand,
			  pointerValue & operand_value,
			  pointerValue & result)
  {}

  virtual void at_index(stmtLocation * current,
			operandNode *operand,
			pointerValue & left,
			pointerValue & right,
			pointerValue & result)
  {}

  // -- Assignments

  virtual void at_assignment(stmtLocation * current,
			     pointerValue & left,
			     pointerValue & right,
			     pointerValue & result,
			     memoryblock_set & changes)
  {}

  virtual void at_parameter_pass(Location * current,
				 stmtLocation * parameter_callsite,
				 pointerValue & left,
				 pointerValue & right,
				 memoryblock_set & changes)
  {}

  virtual void at_self_assignment(Location * source,
				  Location * target,
				  memoryBlock * block,
				  memoryblock_set & changes,
                                  bool is_input)
  {}

  // -- Statement types

  virtual void at_return(Location * current,
			 pointerValue & left,
			 pointerValue & right,
			 memoryblock_set & changes)
  {}

  // TB: new
  virtual void at_threeAddr(stmtLocation *stmt,
                            threeAddrNode *threeaddr,
                            pointerValue &result) {}

  virtual void at_conditiongoto(stmtLocation *stmt,
                                conditiongotoNode *c,
                                pointerValue &left,
                                pointerValue &right,
                                pointerValue &result) {}

  // -- Memory allocation and deallocation

  virtual void at_allocation(stmtLocation * stmt,
			     pointervalue_list & arguments,
			     memoryBlock * block,
			     memoryblock_set & changes)
  {}

  virtual void at_deallocation(stmtLocation * stmt,
			       pointerValue & to_deallocate,
			       memoryblock_set & changes)
  {}

  // -- Process a merge point

  virtual void at_merge(Location * where,
			memoryBlock * block,
			memoryuse_list & phi_uses,
			pointerValue & result,
			memoryblock_set & changes)
  {}

  // -- Control-flow options

  virtual void at_basicblock_entry(basicblockLocation * block,
				   procedureInfo * info,
				   pointerValue & initial)
  {}

  virtual void at_stmt_entry(stmtLocation * stmt,
			     pointerValue & result)
  {}

  virtual void at_stmt_exit(stmtLocation * stmt,
			    pointerValue & result)
  {}

  virtual void at_basicblock_exit(basicblockLocation * block,
				  procedureInfo * info,
				  pointerValue & final)
  {}

  // -- Procedure boundaries

  virtual void at_conservative_procedure_call(stmtLocation * current,
					      operandNode * call,
                                              operand_list & args,
					      pointerValue & call_target,
					      pointervalue_list & arguments,
					      memoryblock_set & reachable_blocks,
					      pointerValue & return_val,
					      memoryblock_set & changes)
  {}

  virtual void at_procedure_entry(procLocation * proc,
				  procedureInfo * info,
				  pointerValue & return_val)
  {}

  virtual void at_procedure_exit(procLocation * proc,
				 procedureInfo * info,
				 pointerValue & return_val)
  {}
  //@}

  /** @brief Compare current property value against the last input to callee.
  *
  *   Returns true if equal. */
  // TB_unify
  virtual bool compare_property_value(Location *where,
                                      memoryBlock *property_block,
                                      procedureInfo *callee) { return true; }

  /** @brief Record the value of a property when it is used as an
   * input_to/external_input value to a call to callee at a call site. */
  // TB_unify
  virtual void record_input_to_value(procedureInfo *callee,
                                     memoryBlock *property_block,
                                     stmtLocation *callsite) {}

};

#endif // CBZ_IPANALYSIS_H

