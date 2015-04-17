// $Id: print_walker.cc,v 1.9 2003/08/07 23:14:05 pnav Exp $
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
#include "print_walker.h"
#include "df_number_walker.h"

// ------------------------------------------------------------
// Entry point
// ------------------------------------------------------------

void print_walker::print(Node * n, ostream & out)
{
  numbering_map * num = df_number_walker::number(n, Preorder);
  print_walker p(out, num);

  n->walk(p);

  delete num;
}

// ------------------------------------------------------------
// Internal methods
// ------------------------------------------------------------

print_walker::print_walker(ostream & out, numbering_map * num)
  : Walker(Both, Subtree), 
    _out(out),
    _indent(0),
    _num(num)
{}

void print_walker::indent(Node * n) const
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

void print_walker::at_unit(unitNode * the_unit, Order ord)
{
  if (ord == Preorder) {
    indent(the_unit);
    _out << "unit" << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_decl(declNode * the_decl, Order ord)
{
  if (ord == Preorder) {
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

    _out << " ids=(";
    id_list & i = the_decl->ref_list();
    for ( id_list_p p = i.begin(); p != i.end(); p++ ) {
      if ( p != i.begin() )
	_out << ", ";
      _out << "#" << (*_num)[*p];
    }
    _out << ")" << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_proc(procNode * the_proc, Order ord)
{
  if (ord == Preorder) {
    indent(the_proc);
    _out << "proc: ";

    if (! the_proc->decl())
      _out << "(no func decl) ";

    if (! the_proc->body())
      _out << "(no body) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_prim(primNode * the_prim, Order ord)
{
  if (ord == Preorder) {
    indent(the_prim);
    _out << "prim: ";

    _out << the_prim->basic().to_string() << " ";
    _out << the_prim->type_qualifiers_name() << " ";
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_tdef(tdefNode * the_tdef, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_ptr(ptrNode * the_ptr, Order ord)
{
  if (ord == Preorder) {
    indent(the_ptr);
    _out << "ptr: " << the_ptr->type_qualifiers_name() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_array(arrayNode * the_array, Order ord)
{
  if (ord == Preorder) {
    indent(the_array);
    _out << "array: size=" << the_array->size() << " ";

    if (! the_array->dim())
      _out << "(no dimension) ";

    _out << the_array->type_qualifiers_name() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_func(funcNode * the_func, Order ord)
{
  if (ord == Preorder) {
    indent(the_func);
    _out << "func: " << the_func->type_qualifiers_name() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_sue(sueNode * the_sue, Order ord)
{
  if (ord == Preorder) {
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
      _out << "\"" << the_sue->spec()->name() << "\" ";

    _out << "spec=#" << (*_num)[the_sue->spec()];

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_suespec(suespecNode * the_suespec, Order ord)
{
  if (ord == Preorder) {
    indent(the_suespec);
    _out << "suespec: ";

    if (the_suespec->name().empty())
      _out << "(no name) ";
    else
      _out << "\"" << the_suespec->name() << "\"";

    if (the_suespec->complete())
      _out << "complete ";

    if (the_suespec->visited())
      _out << "visited ";

    _out << "size=" << the_suespec->size() << " align=" << the_suespec->align();
    _out << " " << the_suespec->type_qualifiers_name() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_const(constNode * the_const, Order ord)
{
  if (ord == Preorder) {
    indent(the_const);
    _out << "const: val=\"" << the_const->text() << "\"" << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_id(idNode * the_id, Order ord)
{
  if (ord == Preorder) {
    indent(the_id);
    _out << "id: ";
    _out << "\"" << the_id->name() << "\" ";
    _out << "decl=#" << (*_num)[the_id->decl()];
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_binary(binaryNode * the_binary, Order ord)
{
  if (ord == Preorder) {
    indent(the_binary);
    _out << "binary: op='" << the_binary->op()->print() << "' ";

    if (! the_binary->left())
      _out << "(no LHS) ";

    if (! the_binary->right())
      _out << "(no RHS) ";
    
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_unary(unaryNode * the_unary, Order ord)
{
  if (ord == Preorder) {
    indent(the_unary);
    _out << "unary: op='" << the_unary->op()->print() << "'";

    if (! the_unary->expr())
      _out << "(no expr) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_cast(castNode * the_cast, Order ord)
{
  if (ord == Preorder) {
    indent(the_cast);
    if (the_cast->is_implicit())
      _out << "implicit cast: ";
    else
      _out << "cast: ";

    if (! the_cast->expr())
      _out << "(no type) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_comma(commaNode * the_comma, Order ord)
{
  if (ord == Preorder) {
    indent(the_comma);
    _out << "comma: " << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_ternary(ternaryNode * the_ternary, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_call(callNode * the_call, Order ord)
{
  if (ord == Preorder) {
    indent(the_call);
    _out << "call: ";

    if (! the_call->name())
      _out << "(no object) ";

    if (the_call->args().empty())
      _out << "(no args) ";

    _out << "proc=#" << (*_num)[the_call->proc()];

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_initializer(initializerNode * the_initializer, Order ord)
{
  if (ord == Preorder) {
    indent(the_initializer);
    _out << "initializer: ";

    if (the_initializer->exprs().empty())
      _out << "(no exprs) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_block(blockNode * the_block, Order ord)
{
  if (ord == Preorder) {
    indent(the_block);
    _out << "block: ";

    if (the_block->decls().empty())
      _out << "(no decls) ";

    if (the_block->stmts().empty())
      _out << "(no stmts) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_basicblock(basicblockNode * the_bb, Order ord)
{
  if (ord == Preorder) {
    indent(the_bb);
    _out << "basicblock: ";

    if (the_bb->decls().empty())
      _out << "(no decls) ";

    if (the_bb->stmts().empty())
      _out << "(no stmts) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_exprstmt(exprstmtNode * the_exprstmt, Order ord)
{
  if (ord == Preorder) {
    indent(the_exprstmt);
    _out << "exprstmt: ";

    if (! the_exprstmt->expr())
      _out << "(no expr) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_label(labelNode * the_label, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_case(caseNode * the_case, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_if(ifNode * the_if, Order ord)
{
  if (ord == Preorder) {
    indent(the_if);
    _out << "if: ";

    if (! the_if->expr())
      _out << "(no expr) ";

    if (! the_if->true_br())
      _out << "(no true branch) ";

    if (! the_if->false_br())
      _out << "(no false branch) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_switch(switchNode * the_switch, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_while(whileNode * the_while, Order ord)
{
  if (ord == Preorder) {
    indent(the_while);
    _out << "while: ";

    if (! the_while->cond())
      _out << "(no cond) ";

    if (! the_while->body())
      _out << "(no body) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_do(doNode * the_do, Order ord)
{
  if (ord == Preorder) {
    indent(the_do);
    _out << "do: ";

    if (! the_do->cond())
      _out << "(no cond) ";

    if (! the_do->body())
      _out << "(no body) ";

    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_for(forNode * the_for, Order ord)
{
  if (ord == Preorder) {
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
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_goto(gotoNode * the_goto, Order ord)
{
  if (ord == Preorder) {
    indent(the_goto);

    _out << "goto: ";
    _out << "\"" << the_goto->name() << "\" ";
    _out << "label=#" << (*_num)[the_goto->label()];
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_continue(continueNode * the_continue, Order ord)
{
  if (ord == Preorder) {
    indent(the_continue);

    _out << "continue: ";
    _out << "loop=#" << (*_num)[the_continue->container()];
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_break(breakNode * the_break, Order ord)
{
  if (ord == Preorder) {
    indent(the_break);

    _out << "break: ";
    _out << "container=#" << (*_num)[the_break->container()];
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_return(returnNode * the_return, Order ord)
{
  if (ord == Preorder) {
    indent(the_return);

    _out << "return: ";

    if (! the_return->expr())
      _out << "(no expr) ";

    _out << "proc=#" << (*_num)[the_return->proc()];
    _out << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_attrib(attribNode * the_attrib, Order ord)
{
  if (ord == Preorder) {
    indent(the_attrib);
    _out << "attrib: " << the_attrib->name() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}

void print_walker::at_operand(operandNode * the_oper, Order ord)
{
  if ( ord == Preorder ) {
    indent(the_oper);
    _out << "operand: addr='" << (the_oper->addr() ? "true" : "false") << "' "
	 << "star='" << (the_oper->star() ? "true" : "false") << "' ";
    if ( ! the_oper->index() )
      _out << "(no Index)";

    _out << endl;

    in();
  }

  if ( ord == Postorder )
    out();
}

void print_walker::at_conditiongoto(conditiongotoNode * the_condgoto, 
				    Order ord)
{
  if ( ord == Preorder ) {
    indent(the_condgoto);
    _out << "conditiongoto: relop='" << the_condgoto->op()->print() << "' ";
    _out << "\"" << the_condgoto->name() << "\" ";
    _out << "label=#" << (*_num)[the_condgoto->label()];
    _out << endl;

    in();
  }
  
  if ( ord == Postorder )
    out();
}

void print_walker::at_threeAddr(threeAddrNode * the_3addr, Order ord)
{
  if ( ord == Preorder ) {
    indent(the_3addr);
    _out << "threeAddr:";
    if ( the_3addr->op() ) 
      _out << " op='" << the_3addr->op()->print() << "' ";
    _out << endl;

    in();
  }

  if ( ord == Postorder )
    out();
}

void print_walker::at_text(textNode * the_text, Order ord)
{
  if (ord == Preorder) {
    indent(the_text);
    _out << "text: " << the_text->text() << endl;

    in();
  }

  if (ord == Postorder)
    out();
}


