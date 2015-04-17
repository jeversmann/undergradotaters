// $Id: symbol.cc,v 1.7 2003/09/19 17:04:56 toktb Exp $
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

// --- Initialize the symbol tables
/*
Identifiers_table * Symbols::Identifiers = new Identifiers_table();
Labels_table      * Symbols::Labels =      new Labels_table();
Tags_table        * Symbols::Tags =        new Tags_table();
Externals_table   * Symbols::Externals =   new Externals_table();
*/
bool Symbols::TrackScopeExits = false;
bool Symbols::TrackInsertSymbol = false;
bool Symbols::TrackIds = false;
/*
int Symbols::Level = 0;
*/
// ------------------------------------------------------------
//   SymbolTable Template
// ------------------------------------------------------------

// --- Symbol table destructor

template <class T>
SymbolTable<T>::~SymbolTable()
{
  // -- Delete all the scopes

  for (table_p p = _table.begin() ; p != _table.end(); ++p)
    delete (*p);
}

// mark nodes for garbage collection
template <class T>
void SymbolTable<T>::mark_nodes (void) {
  for (table_p p = _table.begin() ; p != _table.end(); ++p) {

    scope * sc = (*p);

    // -- For each entry in the scope, mark the node 

    for (scope_p s = sc->begin(); s != sc->end(); ++s) {
      Node *n = (Node *) ((*s).second);
      n->mark = true;
    }
  }
}

// --- Symbol table reset.

template <class T>
void SymbolTable<T>::reset()
{
  // -- For each scope...

  for (table_p p = _table.begin() ; p != _table.end(); ++p) {

    scope * sc = (*p);

    // -- For each entry in the scope, call the exit proc...

    for (scope_p s = sc->begin(); s != sc->end(); ++s)
      notify_exit_scope((*s).second);

    // -- Delete the scope

    delete sc;
  }

  _table.clear();
  _table.push_front(new scope());
}

// --- Symbol table insert
// If the new symbol conflicts with a previously declared symbol, then the original
// symbol is returned. It's up to the called to decide what to do.

template <class T>
T SymbolTable<T>::insert(const string & name, T sym)
{
  if (Symbols::TrackInsertSymbol)
    cerr << "InsertSymbol(" << name << ", " << ")" << endl;

  // -- Get the current scope
  scope * sc = _table.front();

  // -- Check if the name is already there...
  scope_p s = sc->find(name);

  // -- If it is, return the original declaration...

  if (s != sc->end())
    return (*s).second;

  // -- otherwise, add it to the table...

  (*sc)[name] = sym;

  return (T) 0;
}

// --- Symbol table lookup

template <class T>
T SymbolTable<T>::lookup(const string & name, bool cur_scope_only)
{
  table_p p = _table.begin();
  bool found = false;
  scope * sc;
  scope_p s;
  T sym_out;

  do {
    sc = (*p);
    s = sc->find(name);
    if (s != sc->end()) {
      found = true;
      p = _table.end();
    }
    else if(cur_scope_only) break;
    else
      ++p;
  } while (! found && (p != _table.end()));

  if (found)
    sym_out = (*s).second;
  else
    sym_out = (T) 0;

  return sym_out;
}

// --- Symbol table enter scope

template <class T>
void SymbolTable<T>::enter_scope()
{
  if (_is_nested)
   _table. push_front(new scope());
}

// --- Symbol table exit scope

template <class T>
void SymbolTable<T>::exit_scope()
{
  if (_is_nested) {

    scope * top = _table.front();

    for (scope_p s = top->begin(); s != top->end(); ++s)
      notify_exit_scope((*s).second);

    _table.pop_front();
    delete top;
  }
}

template <class T>
void SymbolTable<T>::print(FILE * out)
{
}

template <class T>
string SymbolTable<T>::insert_unique(const string & root, T sym)
{
  char buf[33];
  static unsigned counter = 0;
  T existing;
  string new_name;

  // --- First try the unmodified name...

  existing = lookup( root );
  if (existing) {

    // --- Find a name that's unique in the table...

    do {
      sprintf(buf, "%.16s%d", root.c_str(), ++counter);
      string tmp(buf);
      existing = lookup( tmp );
    } while (existing != 0);

    new_name = string(buf);
  }
  else
    new_name = root;

  // --- Insert the symbol under that name...

  existing = insert(new_name, sym);

  return new_name;
}

// -- IsAType from type.c

bool Identifiers_table::is_a_type(const string & name)
{
  declNode * var = lookup(name);
  if (var)
    return var->storage_class() == declNode::TYPEDEF;

  return false;
}

// ------------------------------------------------------------
// Symbol table constructors
// ------------------------------------------------------------

Identifiers_table::Identifiers_table()
  : SymbolTable< declNode * >(true)
{}

Labels_table::Labels_table()
  : SymbolTable< labelNode * >(false)
{}

Tags_table::Tags_table()
  : SymbolTable< suespecNode * >(true)
{}

Externals_table::Externals_table()
  : SymbolTable< declNode * >(false)
{}

// ------------------------------------------------------------
//  Shadow and notify_exit_scope methods
// ------------------------------------------------------------

void Identifiers_table::shadow(declNode * create, declNode * shadowed)
{
  // the two are equal only for redundant function/extern declarations
  if (create != shadowed  && CBZ::WarningLevel == 5) {
    CBZ::Warning(5, create->coord(),
		 string("`") + create->name() + string("' shadows previous declaration"));
    cerr << "\tPrevious declaration: " << shadowed->coord() << endl;
  }
}

void Identifiers_table::notify_exit_scope(declNode * var)
{
  typeNode * type = var->type();
  assert(type);

  if (var->references() == 0 && type->typ() != Func) {
    if ((var->decl_location() == declNode::BLOCK) &&
	(var->storage_class() != declNode::EXTERN))
      CBZ::Warning(5, var->coord(), string("unused variable `") + var->name() + string("'"));
  }
}

void Labels_table::shadow(labelNode * create, labelNode * shadowed)
{
}

void Labels_table::notify_exit_scope(labelNode * label)
{
  if (label->is_undeclared())
    CBZ::SyntaxError(label->coord(),
		     string("undefined label `") + label->name() + string("'"));

  if (label->references().empty()) {
    if (label->coord().is_unknown() == false)
    CBZ::Warning(5, label->coord(), string("unreferenced label `") + 
		 label->name() + string("'"));
  }
}

void Tags_table::shadow(suespecNode * create, suespecNode * shadowed)
{
  // the two are equal only for redundant function/extern declarations
  if (create != shadowed && CBZ::WarningLevel == 5) {
    CBZ::Warning(5, create->coord(),
		 string("struct/union/enum tag `") + create->name() + 
		 string("' shadows previous declaration"));
    cerr << "\tPrevious declaration: " << shadowed->coord() << endl;
  }
}

void Tags_table::notify_exit_scope(suespecNode * dead)
{
}

void Externals_table::shadow(declNode * create, declNode * shadowed)
{
}

void Externals_table::notify_exit_scope(declNode * var)
{
  typeNode * type = var->type();
  assert(type);

  if (var->references() == 0 && type->typ() != Func) {
    if ((var->decl_location() == declNode::BLOCK) &&
	(var->storage_class() != declNode::EXTERN))
      CBZ::Warning(5, var->coord(), string("unused variable `") + var->name() + string("'"));
  }
}

// ------------------------------------------------------------
//   Static symbol methods
// ------------------------------------------------------------
/*
void Symbols::EnterScope()
{
  Symbols::Identifiers->enter_scope();
  Symbols::Labels->enter_scope();
  Symbols::Tags->enter_scope();
  Symbols::Externals->enter_scope();

  Level++;
}

void Symbols::ExitScope()
{
  if (Level == 0)
    CBZ::SyntaxError(string("missing '{' detected"));
  else {

    Symbols::Identifiers->exit_scope();
    Symbols::Labels->exit_scope();
    Symbols::Tags->exit_scope();
    Symbols::Externals->exit_scope();

    Level--;
  }
}

*/

// Force instantiation of the templates

// stupid gcc needs to have these explicitly instantiated.
// no help from the ANSI C++ standard; what good are templates then?
// yeah, yeah. -- djimenez

template class SymbolTable<declNode *>;
template class SymbolTable<suespecNode *>;
template class SymbolTable<labelNode *>;
