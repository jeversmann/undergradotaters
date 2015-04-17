// $Id: print_tree_visitor.cc,v 1.4 2003/08/07 23:14:04 pnav Exp $
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

#include "c_breeze.h"
#include "tree_visitor.h"
#include "print_tree_visitor.h"
#include "df_number_walker.h"

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

void print_tree_visitor::print(Node * n, ostream & out)
{
  numbering_map * num = df_number_walker::number(n, Walker::Preorder);
  print_tree_visitor p(out, num);

  n->visit(&p);

  delete num;
}

// ------------------------------------------------------------
// Internal methods
// ------------------------------------------------------------

print_tree_visitor::print_tree_visitor(ostream & out, numbering_map * num)
  : tree_visitor(),
    _out(out),
    _indent(0),
    _num(num)
{}

void print_tree_visitor::indent(Node * n) const
{
  _out.width(4);
  _out << (*_num)[n];

  _out.width(_indent+1);
  _out.fill('.');
  // _out.setf(ios_base::left, ios_base::adjustfield);

  _out << ".";

  _out.width(0);
  _out.fill(' ');
  // _out.setf(ios_base::right, ios_base::adjustfield);
}

void print_tree_visitor::at_unit(unitNode * the_unit)
{
  indent(the_unit);
  _out << "unit" << endl;

  in();

  tree_visitor::at_unit(the_unit);

  out();
}

void print_tree_visitor::at_decl(declNode * the_decl)
{
  indent(the_decl);

  _out << "decl: ";

  if (the_decl->name().empty())
    _out << "(abstract)";
  else
    _out << "\"" << the_decl->name() << "\"";

  _out << " ";
  _out << declNode::decl_location_name(the_decl->decl_location());
  _out << " ";
  _out << declNode::storage_class_name(the_decl->storage_class());
  _out << endl;

  in();

  tree_visitor::at_decl(the_decl);

  out();
}

void print_tree_visitor::at_proc(procNode * the_proc)
{
  indent(the_proc);
  _out << "proc: ";

  if (! the_proc->decl())
    _out << "(no func decl) ";

  if (! the_proc->body())
    _out << "(no body) ";

  _out << endl;

  in();

  tree_visitor::at_proc(the_proc);

  out();
}

void print_tree_visitor::at_prim(primNode * the_prim)
{
  indent(the_prim);
  _out << "prim: ";

  _out << the_prim->basic().to_string() << " ";
  _out << the_prim->type_qualifiers_name() << " ";
  _out << endl;

  in();

  tree_visitor::at_prim(the_prim);

  out();
}

void print_tree_visitor::at_tdef(tdefNode * the_tdef)
{
  indent(the_tdef);

  _out << "tdef: ";

  if (the_tdef->name().empty())
    _out << "(no name)";
  else
    _out << "\"" << the_tdef->name() << "\"";

  _out << " ";
  _out << "def=#" << (*_num)[the_tdef->def()] << " ";
  _out << the_tdef->type_qualifiers_name() << endl;

  in();

  tree_visitor::at_tdef(the_tdef);

  out();
}

void print_tree_visitor::at_ptr(ptrNode * the_ptr)
{
  indent(the_ptr);
  _out << "ptr: " << the_ptr->type_qualifiers_name() << endl;

  in();

  tree_visitor::at_ptr(the_ptr);

  out();
}

void print_tree_visitor::at_array(arrayNode * the_array)
{
  indent(the_array);
  _out << "array: size=" << the_array->size() << " ";

  if (! the_array->dim())
    _out << "(no dimension) ";

  _out << the_array->type_qualifiers_name() << endl;

  in();

  tree_visitor::at_array(the_array);

  out();
}

void print_tree_visitor::at_func(funcNode * the_func)
{
  indent(the_func);
  _out << "func: " << the_func->type_qualifiers_name() << endl;

  in();

  tree_visitor::at_func(the_func);

  out();
}

void print_tree_visitor::at_sue(sueNode * the_sue)
{
  indent(the_sue);

  switch (the_sue->typ()) {
  case Struct:
    _out << "struct: ";
    break;
  case Union:
    _out << "union: ";
    break;
  case Enum:
    _out << "enum: ";
    break;
  default:
    _out << "ERROR ";
  }

  if (the_sue->spec()->name().empty())
    _out << "(no name) ";
  else
    _out << "\"" << the_sue->spec()->name() << "\"";

  _out << endl;

  in();

  if (the_sue->elaborated() && the_sue->spec())
    the_sue->spec()->visit(this);

  out();
}

void print_tree_visitor::at_struct(structNode * the_struct)
{ at_sue(the_struct); }

void print_tree_visitor::at_union(unionNode * the_union)
{ at_sue(the_union); }

void print_tree_visitor::at_enum(enumNode * the_enum)
{ at_sue(the_enum); }


void print_tree_visitor::at_suespec(suespecNode * the_suespec)
{
  indent(the_suespec);
  _out << "suespec: ";

  if (the_suespec->complete())
    _out << "complete ";

  if (the_suespec->visited())
    _out << "visited ";

  _out << "size=" << the_suespec->size() << " align=" << the_suespec->align();
  _out << " " << the_suespec->type_qualifiers_name() << endl;

  in();

  tree_visitor::at_suespec(the_suespec);

  out();
}

void print_tree_visitor::at_const(constNode * the_const)
{
  indent(the_const);
  _out << "const: val=\"" << the_const->text() << "\"" << endl;

  in();

  tree_visitor::at_const(the_const);

  out();
}

void print_tree_visitor::at_id(idNode * the_id)
{
  indent(the_id);
  _out << "id: ";
  _out << "\"" << the_id->name() << "\" ";
  _out << "decl=#" << (*_num)[the_id->decl()];
  _out << endl;

  in();

  tree_visitor::at_id(the_id);

  out();
}

void print_tree_visitor::at_binary(binaryNode * the_binary)
{
  indent(the_binary);
  _out << "binary: op='" << the_binary->op()->print() << "' ";

  if (! the_binary->left())
    _out << "(no LHS) ";

  if (! the_binary->right())
    _out << "(no RHS) ";
    
  _out << endl;

  in();

  tree_visitor::at_binary(the_binary);

  out();
}

void print_tree_visitor::at_unary(unaryNode * the_unary)
{
  indent(the_unary);
  _out << "unary: op='" << the_unary->op()->print() << "'";

  if (! the_unary->expr())
    _out << "(no expr) ";

  _out << endl;

  in();

  tree_visitor::at_unary(the_unary);

  out();
}

void print_tree_visitor::at_cast(castNode * the_cast)
{
  indent(the_cast);
  if (the_cast->is_implicit())
    _out << "implicit cast: ";
  else
    _out << "cast: ";

  if (! the_cast->expr())
    _out << "(no type) ";

  _out << endl;

  in();

  tree_visitor::at_cast(the_cast);

  out();
}

void print_tree_visitor::at_comma(commaNode * the_comma)
{
  indent(the_comma);
  _out << "comma: " << endl;

  in();

  tree_visitor::at_comma(the_comma);

  out();
}

void print_tree_visitor::at_ternary(ternaryNode * the_ternary)
{
  indent(the_ternary);
  _out << "ternary: ";

  if (! the_ternary->cond())
    _out << "(no cond) ";

  if (! the_ternary->true_br())
    _out << "(no true branch) ";

  if (! the_ternary->false_br())
    _out << "(no false branch) ";

  _out << endl;

  in();

  tree_visitor::at_ternary(the_ternary);

  out();
}

void print_tree_visitor::at_call(callNode * the_call)
{
  indent(the_call);
  _out << "call: ";

  if (! the_call->name())
    _out << "(no object) ";

  if (the_call->args().empty())
    _out << "(no args) ";

  _out << "proc=#" << (*_num)[the_call->proc()];

  _out << endl;

  in();

  tree_visitor::at_call(the_call);

  out();
}

void print_tree_visitor::at_initializer(initializerNode * the_initializer)
{
  indent(the_initializer);
  _out << "initializer: ";

  if (the_initializer->exprs().empty())
    _out << "(no exprs) ";

  _out << endl;

  in();

  tree_visitor::at_initializer(the_initializer);

  out();
}

void print_tree_visitor::at_block(blockNode * the_block)
{
  indent(the_block);
  _out << "block: ";

  if (the_block->decls().empty())
    _out << "(no decls) ";

  if (the_block->stmts().empty())
    _out << "(no stmts) ";

  _out << endl;

  in();

  tree_visitor::at_block(the_block);

  out();
}

void print_tree_visitor::at_exprstmt(exprstmtNode * the_exprstmt)
{
  indent(the_exprstmt);
  _out << "exprstmt: ";

  if (! the_exprstmt->expr())
    _out << "(no expr) ";

  _out << endl;

  in();

  tree_visitor::at_exprstmt(the_exprstmt);

  out();
}

void print_tree_visitor::at_label(labelNode * the_label)
{
  indent(the_label);
  _out << "label: ";

  if (the_label->name().empty())
    _out << "(no name)";
  else
    _out << "\"" << the_label->name() << "\" ";

  if (! the_label->stmt())
    _out << "(no stmt) ";

  _out << " gotos=(";

  goto_list & g = the_label->references();
  for (goto_list_p p = g.begin();
       p != g.end();
       ++p) {
    if (p != g.begin())
      _out << ", ";
    _out << "#" << (*_num)[*p];
  }

  _out << ")";
  _out << endl;

  in();

  tree_visitor::at_label(the_label);

  out();
}

void print_tree_visitor::at_case(caseNode * the_case)
{
  indent(the_case);

  if (! the_case->expr())
    _out << "default: ";
  else
    _out << "case: ";

  if (! the_case->stmt())
    _out << "(no stmt) ";

  _out << "switch=#" << (*_num)[the_case->container()];

  _out << endl;

  in();

  tree_visitor::at_case(the_case);

  out();
}

void print_tree_visitor::at_if(ifNode * the_if)
{
  indent(the_if);
  _out << "ifelse: ";

  if (! the_if->expr())
    _out << "(no expr) ";

  if (! the_if->true_br())
    _out << "(no true branch) ";

  if (! the_if->false_br())
    _out << "(no false branch) ";

  _out << endl;

  in();

  tree_visitor::at_if(the_if);

  out();
}

void print_tree_visitor::at_switch(switchNode * the_switch)
{
  indent(the_switch);
  _out << "switch: ";

  if (! the_switch->expr())
    _out << "(no expr) ";

  if (! the_switch->stmt())
    _out << "(no body) ";

  _out << "cases=(";

  target_list & g = the_switch->cases();
  for (target_list_p p = g.begin();
       p != g.end();
       ++p) {
    if (p != g.begin())
      _out << ", ";
    _out << "#" << (*_num)[*p];
  }

  _out << ")";
  _out << endl;

  in();

  tree_visitor::at_switch(the_switch);

  out();
}

void print_tree_visitor::at_while(whileNode * the_while)
{
  indent(the_while);
  _out << "while: ";

  if (! the_while->cond())
    _out << "(no cond) ";

  if (! the_while->body())
    _out << "(no body) ";

  _out << endl;

  in();

  tree_visitor::at_while(the_while);

  out();
}

void print_tree_visitor::at_do(doNode * the_do)
{
  indent(the_do);
  _out << "do: ";

  if (! the_do->cond())
    _out << "(no cond) ";

  if (! the_do->body())
    _out << "(no body) ";

  _out << endl;

  in();

  tree_visitor::at_do(the_do);

  out();
}

void print_tree_visitor::at_for(forNode * the_for)
{
  indent(the_for);
  _out << "for: ";

  if (! the_for->init())
    _out << "(no init) ";

  if (! the_for->cond())
    _out << "(no cond) ";

  if (! the_for->next())
    _out << "(no next) ";

  if (! the_for->body())
    _out << "(no body) ";

  _out << endl;

  in();

  tree_visitor::at_for(the_for);

  out();
}

void print_tree_visitor::at_goto(gotoNode * the_goto)
{
  indent(the_goto);

  _out << "goto: ";
  _out << "\"" << the_goto->name() << "\" ";
  _out << "label=#" << (*_num)[the_goto->label()];
  _out << endl;

  in();

  tree_visitor::at_goto(the_goto);

  out();
}

void print_tree_visitor::at_continue(continueNode * the_continue)
{
  indent(the_continue);

  _out << "continue: ";
  _out << "loop=#" << (*_num)[the_continue->container()];
  _out << endl;

  in();

  tree_visitor::at_continue(the_continue);

  out();
}

void print_tree_visitor::at_break(breakNode * the_break)
{
  indent(the_break);

  _out << "break: ";
  _out << "container=#" << (*_num)[the_break->container()];
  _out << endl;

  in();

  tree_visitor::at_break(the_break);

  out();
}

void print_tree_visitor::at_return(returnNode * the_return)
{
  indent(the_return);

  _out << "return: ";

  if (! the_return->expr())
    _out << "(no expr) ";

  _out << "proc=#" << (*_num)[the_return->proc()];
  _out << endl;

  in();

  tree_visitor::at_return(the_return);

  out();
}

void print_tree_visitor::at_attrib(attribNode * the_attrib)
{
  indent(the_attrib);
  _out << "attrib: " << the_attrib->name() << endl;

  in();

  tree_visitor::at_attrib(the_attrib);

  out();
}

void print_tree_visitor::at_text(textNode * the_text)
{
  indent(the_text);
  _out << "text: " << the_text->text() << endl;

  in();

  tree_visitor::at_text(the_text);

  out();
}


