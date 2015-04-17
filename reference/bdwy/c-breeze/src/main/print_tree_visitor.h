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

#ifndef CBZ_PRINT_TREE_VISITOR_H
#define CBZ_PRINT_TREE_VISITOR_H


/** @brief puts the AST on an output stream.
    @ingroup Phases
*/
class print_tree_visitor : public tree_visitor
{

public:

  // -- To display the AST, call this static function:

  static void print(Node * n, ostream & out);

private:

  ostream & _out;
  int _indent;
  numbering_map * _num;

  void indent(Node * n) const;

  void in()  { ++_indent; }
  void out() { if (_indent > 0) --_indent; }

public:

  print_tree_visitor(ostream & out, numbering_map * num);

  virtual void at_unit(unitNode * the_unit);
  virtual void at_decl(declNode * the_decl);
  virtual void at_proc(procNode * the_proc);
  virtual void at_prim(primNode * the_prim);
  virtual void at_tdef(tdefNode * the_tdef);
  virtual void at_ptr(ptrNode * the_ptr);
  virtual void at_array(arrayNode * the_array);
  virtual void at_func(funcNode * the_func);
  virtual void at_sue(sueNode * the_sue);
  virtual void at_struct(structNode * the_struct);
  virtual void at_union(unionNode * the_union);
  virtual void at_enum(enumNode * the_enum);
  virtual void at_suespec(suespecNode * the_suespec);
  virtual void at_const(constNode * the_const);
  virtual void at_id(idNode * the_id);
  virtual void at_binary(binaryNode * the_binary);
  virtual void at_unary(unaryNode * the_unary);
  virtual void at_cast(castNode * the_cast);
  virtual void at_comma(commaNode * the_comma);
  virtual void at_ternary(ternaryNode * the_ternary);
  virtual void at_call(callNode * the_call);
  virtual void at_initializer(initializerNode * the_initializer);
  virtual void at_block(blockNode * the_block);
  virtual void at_exprstmt(exprstmtNode * the_exprstmt);
  virtual void at_label(labelNode * the_label);
  virtual void at_case(caseNode * the_case);
  virtual void at_if(ifNode * the_if);
  virtual void at_switch(switchNode * the_switch);
  virtual void at_while(whileNode * the_while);
  virtual void at_do(doNode * the_do);
  virtual void at_for(forNode * the_for);
  virtual void at_goto(gotoNode * the_goto);
  virtual void at_continue(continueNode * the_continue);
  virtual void at_break(breakNode * the_break);
  virtual void at_return(returnNode * the_return);
  virtual void at_attrib(attribNode * the_attrib);
  virtual void at_text(textNode * the_text);
};


#endif // CBZ_PRINT_TREE_VISITOR_H
