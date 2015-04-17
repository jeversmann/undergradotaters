// $Id: precision.h,v 1.5 2003/08/08 15:16:29 toktb Exp $
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

#ifndef CBZ_PRECISION_H
#define CBZ_PRECISION_H

#include "pointers.h"

class precisionAnalyzer : public analysisProblem
{
private:

  typedef pair< stmtLocation *, memoryBlock * > stmt_pointer_pair;

  typedef map< stmt_pointer_pair, memoryblock_set > pointer_targets_map;
  typedef pointer_targets_map::iterator pointer_targets_map_p;

  /** @brief Pointer targets
   *
   * For each dereference, store the targets of the pointer according to
   * the pointer variable itself and location. */

  pointer_targets_map _pointer_targets;

  /** @brief Dereference count
   */

  long int _derefs;

public:

  /** @brief Create a new precision analyzer object */

  precisionAnalyzer();

  /** @brief Name of the analysis
   *
   * Just for debug purposes. */

  virtual string name() { return string("Precision"); }

  /** @brief At each field access
   *
   * Count the number of targets if this is an -> operator. */

  virtual void at_field_access(stmtLocation * current,
                               operandNode *operand,
			       pointerValue & operand_value,
			       pointerValue & result);

  /** @brief At each dereference
   *
   * Count the number of targets and store it in the deref map. */
  
  virtual void at_dereference(stmtLocation * current,
                              operandNode *operand,
			      pointerValue & operand_value,
			      pointerValue & result);

  /** @brief At an array index
   *
   * Array index is a dereference. */

  virtual void at_index(stmtLocation * current,
			operandNode *operand,
			pointerValue & left,
			pointerValue & right,
			pointerValue & result);

  /** @brief Report
   *
   * Print the results of the precision analysis. */

  void report(ostream & out);

private:

  void record(stmtLocation * current,
	      pointerValue & operand,
	      pointerValue & result);
};

#endif

