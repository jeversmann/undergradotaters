// $Id: scope_walker.h,v 1.6 2003/09/09 20:51:53 toktb Exp $
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

#ifndef CBZ_SCOPE_WALKER_H
#define CBZ_SCOPE_WALKER_H


/** @brief visits each scope in a unit.
    @ingroup Phases
*/
class scope_walker : public Walker
{
private:

  unitNode * _unit;

  Identifiers_table * _ids;
  Tags_table        * _tags;
  Externals_table   * _externs;
  int _symbol_level;

  int _in_su;
  int _in_enum;
  int _in_formals;

protected:
  procNode * _cur_proc;

public:

  scope_walker(unitNode * u);
  virtual ~scope_walker();

  /// Enter and exit scopes
  virtual void at_proc(procNode * the_proc, Order ord);
  virtual void at_block(blockNode * the_block, Order ord);

  /** @defgroup SCOPE_WALKER_H_CONTEXT
      @brief Keep track of context.
   */
  /*@{*/
  virtual void at_suespec(suespecNode * the_suespec, Order ord);
  virtual void at_func(funcNode * the_func, Order ord);
  /*@}*/

  /** @defgroup SCOPE_WALKER_H_SYMBOL_TABLE
      @brief Access the symbol tables.
  */
  /*@{*/
  inline Identifiers_table * ids() const { return _ids; }
  inline Tags_table * tags() const { return _tags; }
  inline Externals_table * externs() const { return _externs; }
  inline int symbol_level() const { return _symbol_level; }
  /*@}*/

  /** @defgroup SCOPE_WALKER_H_SCOPE_PRED
      @brief Scope predicates.
  */
  /*@{*/
  inline bool in_su() const { return _in_su != 0; }
  inline bool in_enum() const { return _in_enum; }
  inline bool in_formals() const { return _in_formals != 0; }
  /*@}*/

  /// unitNode
  inline unitNode * unit() { return _unit; }

  /** @defgroup SCOPE_WALKER_H_SCOPE
      @brief Symbol table scoping.
  */
  /*@{*/
  void enter_scope();
  void exit_scope();
  /*@}*/
};


#endif // CBZ_SCOPE_WALKER_H
