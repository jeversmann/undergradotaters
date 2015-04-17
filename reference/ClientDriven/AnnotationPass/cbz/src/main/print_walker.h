// $Id: print_walker.h,v 1.8 2003/08/07 23:14:06 pnav Exp $
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

#ifndef CBZ_PRINT_WALKER_H
#define CBZ_PRINT_WALKER_H


/** @brief prints each node to an out stream.
    @ingroup Phases
*/
class print_walker : public Walker
{

public:

  /// To display the AST, call this static function:
  static void print(Node * n, ostream & out);

private:

  ostream & _out;
  int _indent;
  numbering_map * _num;

  void indent(Node * n) const;

  void in()  { ++_indent; }
  void out() { if (_indent > 0) --_indent; }

public:

  print_walker(ostream & out, numbering_map * num);

  virtual void at_unit(unitNode * the_unit, Order ord);
  virtual void at_decl(declNode * the_decl, Order ord);
  virtual void at_proc(procNode * the_proc, Order ord);
  virtual void at_prim(primNode * the_prim, Order ord);
  virtual void at_tdef(tdefNode * the_tdef, Order ord);
  virtual void at_ptr(ptrNode * the_ptr, Order ord);
  virtual void at_array(arrayNode * the_array, Order ord);
  virtual void at_func(funcNode * the_func, Order ord);
  virtual void at_sue(sueNode * the_sue, Order ord);
  virtual void at_suespec(suespecNode * the_suespec, Order ord);
  virtual void at_const(constNode * the_const, Order ord);
  virtual void at_id(idNode * the_id, Order ord);
  virtual void at_binary(binaryNode * the_binary, Order ord);
  virtual void at_unary(unaryNode * the_unary, Order ord);
  virtual void at_cast(castNode * the_cast, Order ord);
  virtual void at_comma(commaNode * the_comma, Order ord);
  virtual void at_ternary(ternaryNode * the_ternary, Order ord);
  virtual void at_call(callNode * the_call, Order ord);
  virtual void at_initializer(initializerNode * the_initializer, Order ord);
  virtual void at_block(blockNode * the_block, Order ord);
  virtual void at_basicblock(basicblockNode * the_bb, Order ord);
  virtual void at_exprstmt(exprstmtNode * the_exprstmt, Order ord);
  virtual void at_label(labelNode * the_label, Order ord);
  virtual void at_case(caseNode * the_case, Order ord);
  virtual void at_if(ifNode * the_if, Order ord);
  virtual void at_switch(switchNode * the_switch, Order ord);
  virtual void at_while(whileNode * the_while, Order ord);
  virtual void at_do(doNode * the_do, Order ord);
  virtual void at_for(forNode * the_for, Order ord);
  virtual void at_goto(gotoNode * the_goto, Order ord);
  virtual void at_continue(continueNode * the_continue, Order ord);
  virtual void at_break(breakNode * the_break, Order ord);
  virtual void at_return(returnNode * the_return, Order ord);
  virtual void at_attrib(attribNode * the_attrib, Order ord);
  virtual void at_operand(operandNode * the_oper, Order ord);
  virtual void at_conditiongoto(conditiongotoNode * the_condgoto, Order ord);
  virtual void at_threeAddr(threeAddrNode * the_3addr, Order ord);
  virtual void at_text(textNode * the_text, Order ord);
};


#endif // CBZ_PRINT_WALKER_H
