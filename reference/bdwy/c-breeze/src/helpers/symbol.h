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

#ifndef CBZ_SYMBOL_H
#define CBZ_SYMBOL_H


// ------------------------------------------------------------
//   Symbol Table
// ------------------------------------------------------------

template <class T>
class SymbolTable {

private:

  // -- Nested or flat symbol table
  bool _is_nested;

  // -- A symbol table is a list of scopes

  typedef map<string,
	  T,
	  less< string > > scope;

  typedef scope::iterator scope_p;

  typedef list< scope * > table;
  typedef table::iterator table_p;

  table _table;

  virtual void shadow(T create, T shadowed) =0;
  virtual void notify_exit_scope(T dead) =0;

public:

  SymbolTable(bool is_nested)
    : _is_nested(is_nested), _table()
  { _table.push_front(new scope()); }

  ~SymbolTable();

  void mark_nodes (void);
  void reset();
  T insert(const string & name, T sym); // Returns any conflicting object
  T lookup(const string & name);
  void print(FILE * out);
  string insert_unique(const string & root, T sym); // Returns new name
  void enter_scope();
  void exit_scope();

};

// ------------------------------------------------------------
//   Subclass the symbol-table for each type of table
// ------------------------------------------------------------

class Identifiers_table : public SymbolTable< declNode * > {

public:

  Identifiers_table();
  bool is_a_type(const string & name);

private:

  void shadow(declNode * create, declNode * shadowed);
  void notify_exit_scope(declNode * dead);
};

class Labels_table : public SymbolTable< labelNode * > {

public:

  Labels_table();

private:

  void shadow(labelNode * create, labelNode * shadowed);
  void notify_exit_scope(labelNode * dead);
};

class Tags_table : public SymbolTable< suespecNode * > {

public:

  Tags_table();

private:

  void shadow(suespecNode * create, suespecNode * shadowed);
  void notify_exit_scope(suespecNode * dead);
};

class Externals_table : public SymbolTable< declNode * > {

public:

  Externals_table();

private:

  void shadow(declNode * create, declNode * shadowed);
  void notify_exit_scope(declNode * dead);
};

// ------------------------------------------------------------
//   Symbols class holds the global information and static
//   symbol table functions.
// ------------------------------------------------------------

class Symbols {

public:

  /* Moved to unitNode...
  // -- Static functions -------------------

  static void EnterScope();
  static void ExitScope();

  // -- Static members ---------------------

  // -- ANSI defines the following name spaces (K&R A11.1, pg 227): 

  static Identifiers_table  * Identifiers;
  static Labels_table       * Labels; 
  static Tags_table         * Tags;

  // -- This table is used to ensure consistency across the translation unit 

  static Externals_table    * Externals;
  */

  static bool TrackScopeExits;
  static bool TrackInsertSymbol;
  static bool TrackIds;

  static int Level;
};


#endif // CBZ_SYMBOL_H
