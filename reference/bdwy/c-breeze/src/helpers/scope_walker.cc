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
#include "scope_walker.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------

scope_walker::scope_walker(unitNode * u)
  : Walker(Both, Subtree),
    _unit(u),
    _ids(new Identifiers_table()),
    _tags(new Tags_table()),
    _externs(new Externals_table()),
    _symbol_level(0),
    _in_su(0),
    _in_enum(false),
    _in_formals(0)
{}

scope_walker::~scope_walker()
{
  //delete _ids;
  //delete _tags;
  //delete _externs;
}

// ------------------------------------------------------------
// Keep track of context
// ------------------------------------------------------------

void scope_walker::at_suespec(suespecNode * the_suespec, Order ord)
{
  NodeType owner = the_suespec->owner();

  if (ord == Preorder)
    if (owner == Enum)
      _in_enum++;
    else
      _in_su++;
  else
    if (owner == Enum)
      _in_enum--;
    else
      _in_su--;
}

void scope_walker::at_func(funcNode * the_func, Order ord)
{
  if (ord == Preorder)
    _in_formals++;
  else
    _in_formals--;
}

// ------------------------------------------------------------
// Handle scope changes
// ------------------------------------------------------------

void scope_walker::at_block(blockNode * the_block, Order ord)
{
  if (ord == Preorder)
    enter_scope();

  if (ord == Postorder)
    exit_scope();
}

// ------------------------------------------------------------
// Symbol table scope
// ------------------------------------------------------------

void scope_walker::enter_scope()
{
  ids()->enter_scope();
  tags()->enter_scope();
  externs()->enter_scope();

  ++_symbol_level;
}

void scope_walker::exit_scope()
{
  if (_symbol_level == 0)
    CBZ::SyntaxError(string("missing '{' detected"));
  else {
    ids()->exit_scope();
    tags()->exit_scope();
    externs()->exit_scope();

    --_symbol_level;
  }
}

