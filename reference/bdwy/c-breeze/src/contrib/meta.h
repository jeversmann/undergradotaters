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

#ifndef CBZ_META_H
#define CBZ_META_H

#include "ast.h"
#include "c_breeze.h"

class metaVariable
{
private:

  string _name;

public:

  metaVariable(string & name)
    : _name(name)
    {}

  inline string & name() { return _name; }
};

/** @brief Meta-variable expression node, for use in lexer.
    @ingroup AST
*/
class metaexprNode : public exprNode,
		     public metaVariable
{
public:

  /** @brief Call this to identify meta variables of type expr (in the
   * lexer).
   */
  static bool is_meta_expr(char * name);

  /** @brief Call this method when you recognize a macro formal argument
   * of type $expr (in the parser).
   */
  static void add_meta_expr(char * name);

  /** @brief --- Call this method to clear the list of recognized macro
   * variables (e.g., at the end of a macro definition.
   */
  static void clear();

private:

  /** @brief Meta-expression list.
   */
  static list< string > meta_expr_variables;

public:

  metaexprNode(idNode * id)
    : exprNode(Meta, 0, id->coord()),
      metaVariable(id->name())
    {
      delete id;
    }

  // -- Virtual Functions from exprNode

  virtual void eval() {}

  // -- Walk and change

  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch);

  // -- Dataflow

  virtual void dataflow(FlowVal * v, FlowProblem & fp);

  // -- Clone

  virtual Node * clone() const { return new metaexprNode ( *this ); }

  // -- Output

  virtual void output_expr(output_context & ct, Node * par, int prec, Assoc assoc);

};


/** @brief Meta-variable statement node, for use in lexer.
    @ingroup AST
*/
class metastmtNode : public stmtNode,
		     public metaVariable
{
public:

  /**  @brief Call this to identify meta variables of type stmt (in the
   *   lexer).
   */
  static bool is_meta_stmt(char * name);

  /** @brief Call this method when you recognize a macro formal argument
   * of type $stmt (in the parser).
   */
  static void add_meta_stmt(char * name);

  /** @brief Call this method to clear the list of recognized macro
   * variables (e.g., at the end of a macro definition.
   */
  static void clear();

private:

  /** @brief Meta-statement list.
   */
  static list< string > meta_stmt_variables;

public:

  metastmtNode(idNode * id)
    : stmtNode(Meta, id->coord()),
    metaVariable(id->name())
    {
      delete id;
    }

  // -- Walk and change
  
  virtual void visit(Visitor * the_visitor);
  virtual void walk(Walker & the_walker);
  virtual Node * change(Changer & the_changer, bool redispatch = false);
  
  // -- Dataflow
  
  virtual void dataflow(FlowVal * v, FlowProblem & fp);
  
  // -- Clone
  
  virtual Node * clone() const { return new metastmtNode ( *this ); }
  
  // -- Output 
  virtual void output_stmt(output_context & ct, Node * par);
  
};


#endif // 
