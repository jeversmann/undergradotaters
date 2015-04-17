// $Id: tree_visitor.h,v 1.6 2003/08/07 23:13:57 pnav Exp $
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

#ifndef CBZ_TREE_VISITOR_H
#define CBZ_TREE_VISITOR_H



template< class T >
void visit_list(list< T > & l, Visitor * the_visitor)
{
  for (typename list< T >::iterator p = l.begin();
       p != l.end();
       ++p)
    (*p)->visit(the_visitor);
}


class tree_visitor : public Visitor
{

public:

  virtual void at_unit(unitNode * the_unit)
  {
    visit_list(the_unit->defs(), this);
  }

  virtual void at_decl(declNode * the_decl)
  {
    if (the_decl->type())
      the_decl->type()->visit(this);

    if (the_decl->init())
      the_decl->init()->visit(this);

    if (the_decl->bitsize())
      the_decl->bitsize()->visit(this);

    visit_list(the_decl->attribs(), this);
  }

  virtual void at_proc(procNode * the_proc)
  {
    if (the_proc->decl())
      the_proc->decl()->visit(this);

    if (the_proc->body())
      the_proc->body()->visit(this);
  }

  virtual void at_prim(primNode * the_prim)
  {}

  virtual void at_tdef(tdefNode * the_tdef)
  {}

  virtual void at_ptr(ptrNode * the_ptr)
  {
      the_ptr->type()->visit(this);
  }

  virtual void at_array(arrayNode * the_array)
  {
    if (the_array->type())
      the_array->type()->visit(this);

    if (the_array->dim())
      the_array->dim()->visit(this);
  }

  virtual void at_func(funcNode * the_func)
  {
    visit_list(the_func->args(), this);

    if (the_func->returns())
      the_func->returns()->visit(this);
  }

  virtual void at_struct(structNode * the_struct)
  {
    if (the_struct->elaborated() && the_struct->spec())
      the_struct->spec()->visit(this);
  }

  virtual void at_union(unionNode * the_union)
  {
    if (the_union->elaborated() && the_union->spec())
      the_union->spec()->visit(this);
  }

  virtual void at_enum(enumNode * the_enum)
  {
    if (the_enum->elaborated() && the_enum->spec())
      the_enum->spec()->visit(this);
  }

  virtual void at_suespec(suespecNode * the_suespec)
  {
    visit_list(the_suespec->fields(), this);
  }

  virtual void at_const(constNode * the_const)
  {
    if (the_const->type())
      the_const->type()->visit(this);
  }

  virtual void at_id(idNode * the_id)
  {
    if (the_id->type())
      the_id->type()->visit(this);
  }

  virtual void at_binary(binaryNode * the_binary)
  {
    if (the_binary->right())
      the_binary->right()->visit(this);

    if (the_binary->left())
      the_binary->left()->visit(this);

    if (the_binary->type())
      the_binary->type()->visit(this);
  }

  virtual void at_unary(unaryNode * the_unary)
  {
    if (the_unary->expr())
      the_unary->expr()->visit(this);

    if (the_unary->type())
      the_unary->type()->visit(this);

    if (the_unary->sizeof_type())
      the_unary->sizeof_type()->visit(this);
  }

  virtual void at_cast(castNode * the_cast)
  {
    if (the_cast->type())
      the_cast->type()->visit(this);

    if (the_cast->expr())
      the_cast->expr()->visit(this);
  }

  virtual void at_comma(commaNode * the_comma)
  {
    visit_list(the_comma->exprs(), this);

    if (the_comma->type())
      the_comma->type()->visit(this);
  }

  virtual void at_ternary(ternaryNode * the_ternary)
  {
    if (the_ternary->cond())
      the_ternary->cond()->visit(this);

    if (the_ternary->true_br())
      the_ternary->true_br()->visit(this);

    if (the_ternary->false_br())
      the_ternary->false_br()->visit(this);

    if (the_ternary->type())
      the_ternary->type()->visit(this);
  }

  virtual void at_call(callNode * the_call)
  {
    if (the_call->name())
      the_call->name()->visit(this);

    visit_list(the_call->args(), this);

    if (the_call->type())
      the_call->type()->visit(this);
  }

  virtual void at_initializer(initializerNode * the_initializer)
  {
    visit_list(the_initializer->exprs(), this);

    if (the_initializer->type())
      the_initializer->type()->visit(this);
  }

  virtual void at_block(blockNode * the_block)
  {
    visit_list(the_block->decls(), this);
    visit_list(the_block->stmts(), this);

    if (the_block->type())
      the_block->type()->visit(this);
  }

  virtual void at_exprstmt(exprstmtNode * the_exprstmt)
  {
    if (the_exprstmt->expr())
      the_exprstmt->expr()->visit(this);
  }

  virtual void at_label(labelNode * the_label)
  {
    if (the_label->stmt())
      the_label->stmt()->visit(this);
  }

  virtual void at_case(caseNode * the_case)
  {
    if (the_case->expr())
      the_case->expr()->visit(this);

    if (the_case->stmt())
      the_case->stmt()->visit(this);
  }

  virtual void at_if(ifNode * the_if)
  {
    if (the_if->expr())
      the_if->expr()->visit(this);

    if (the_if->true_br())
      the_if->true_br()->visit(this);

    if (the_if->false_br())
      the_if->false_br()->visit(this);
  }

  virtual void at_switch(switchNode * the_switch)
  {
    if (the_switch->expr())
      the_switch->expr()->visit(this);

    if (the_switch->stmt())
      the_switch->stmt()->visit(this);
  }

  virtual void at_while(whileNode * the_while)
  {
    if (the_while->cond())
      the_while->cond()->visit(this);

    if (the_while->body())
      the_while->body()->visit(this);
 }

  virtual void at_do(doNode * the_do)
  {
    if (the_do->body())
      the_do->body()->visit(this);

    if (the_do->cond())
      the_do->cond()->visit(this);
  }

  virtual void at_for(forNode * the_for)
  {
    if (the_for->init())
      the_for->init()->visit(this);

    if (the_for->cond())
      the_for->cond()->visit(this);

    if (the_for->next())
      the_for->next()->visit(this);

    if (the_for->body())
      the_for->body()->visit(this);
  }

  virtual void at_goto(gotoNode * the_goto)
  {}

  virtual void at_continue(continueNode * the_continue)
  {}

  virtual void at_break(breakNode * the_break)
  {}

  virtual void at_return(returnNode * the_return)
  {
    if (the_return->expr())
      the_return->expr()->visit(this);
  }

  virtual void at_attrib(attribNode * the_attrib)
  {
    if (the_attrib->arg())
      the_attrib->arg()->visit(this);
  }

  virtual void at_text(textNode * the_text)
  {}
};


#endif // CBZ_TREE_VISITOR_H
