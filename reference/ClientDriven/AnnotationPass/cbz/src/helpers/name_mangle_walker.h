// $Id: name_mangle_walker.h,v 1.7 2003/08/07 23:13:50 pnav Exp $
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
#ifndef CBZ_NAME_MANGLE_H
#define CBZ_NAME_MANGLE_H


#include "c_breeze.h"
#include "scope_walker.h"

using namespace std;


/** @brief mangles names to avoid id conflicts.
    @ingroup Phases
*/
class name_mangle_walker : public scope_walker
{
public:

  /** @defgroup NAME_MANGLE_WALKER_ENTRY
      @brief Entry points
  */
  /*@{*/
  static void mangle();
  static void mangle(unitNode * u);
  /*@}*/

private:

  Labels_table * _labels;

  name_mangle_walker(unitNode * u)
    : scope_walker(u),
      _labels(new Labels_table())
  {}

public:

  /** @defgroup NAME_MANGLE_WALKER_SYMBOL_TABLE
      @brief Put declarations into the symbol table
  */
  /*@{*/
  virtual void at_proc(procNode * the_proc, Order ord);
  virtual void at_decl(declNode * the_decl, Order ord);
  /*@}*/

  /// Fix identifier names
  virtual void at_id(idNode * the_id, Order ord);

  /// Fix label names
  virtual void at_label(labelNode * the_label, Order ord);

  inline Labels_table * labels() const { return _labels; }
};

class fix_goto_walker : public Walker
{


public:
  fix_goto_walker(void):  Walker(Preorder, Subtree)
    {};

private:
  void at_goto(gotoNode * g, Order ord);
};

#endif
