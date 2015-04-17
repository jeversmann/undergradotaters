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

#ifndef CBZ_SEMCHECK_H
#define CBZ_SEMCHECK_H



/** @brief Statement checker.
    @ingroup Phases
*/
class semcheck_walker : public Walker
{
public:

  static void check(unitNode * u, bool verbose);

  static bool showErrors;

private:

  semcheck_walker()
    : Walker(Postorder, Subtree)
  {}

public:

  virtual void at_exprstmt(exprstmtNode * the_exprstmt, Order ord);
  virtual void at_suespec(suespecNode * the_suespec, Order ord);
};

/** @brief Expression checker.
    @ingroup Phases
 */
class semcheck_expr_visitor : public Visitor
{
public:

  // -- Public entry points

  static void check(exprNode * e);
  static void check(unitNode * u);

private:

  typeNode * default_type();
  typeNode * check_binary(unsigned int opid, exprNode * lhs, exprNode * rhs,
			  exprNode ** new_lhs, exprNode ** new_rhs);
  typeNode * check_unary(unsigned int opid, exprNode * ex,
			 exprNode ** new_ex);

  semcheck_expr_visitor()
    : Visitor()
  {}

public:

  virtual void at_const(constNode * the_const);
  virtual void at_id(idNode * the_id);
  virtual void at_binary(binaryNode * the_binary);
  virtual void at_unary(unaryNode * the_unary);
  virtual void at_cast(castNode * the_cast);
  virtual void at_comma(commaNode * the_comma);
  virtual void at_ternary(ternaryNode * the_ternary);
  virtual void at_call(callNode * the_call);
  virtual void at_initializer(initializerNode * the_initializer);

};


#endif // CBZ_SEMCHECK_H
