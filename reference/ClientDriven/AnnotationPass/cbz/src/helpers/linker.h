// $Id: linker.h,v 1.7 2003/08/07 23:13:49 pnav Exp $
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

#ifndef CBZ_LINKER_H
#define CBZ_LINKER_H

typedef map< string, declNode *> decl_name_map;
typedef decl_name_map::iterator decl_name_map_p;

typedef map< declNode *, procNode *> proc_decl_map;
typedef proc_decl_map::iterator proc_decl_map_p;
typedef proc_decl_map::const_iterator proc_decl_map_cp;

typedef map< unitNode *, decl_name_map > unit_decl_name_map;
typedef unit_decl_name_map::iterator unit_decl_name_map_p;



class Linker : public Walker
{
public:

  static bool debug;

private:

  /** @brief External variable definitions */

  decl_name_map _external_symbols;

  /** @brief Internally linked (static) variables */

  unit_decl_name_map _internal_symbols;

  /** @brief Mapping from declarations to procedure definitions */

  proc_decl_map _procedure_declarations;

  /** @brief Synthetic global variables
   *
   *  These declarations are created for global variables that are always
   *  declared "extern", presumably because their actual definition is in
   *  some pre-compiled code elsewhere */

  decl_name_map _synthetic;

  /** @brief Used internally: the current unit being processed */

  unitNode * current_unit;

public:

  /** @brief Create a new linker
   *
   *  The constructor does not perform the linking. */

  Linker();

  /** @brief Destructor
   *
   *  Destroy the linker object. In particular, destroy any synthetic
   *  declarations created. */

  ~Linker();

  /** @Brief Clear
   *
   *  Clear all the tables, delete all the synthetic declarations, in
   *  preparation for linking. */

  void clear();

  /** @brief Link
   *
   *  This is main function of the Linker class. It visits all the
   *  top-level declarations in all the translation units and makes sure
   *  that each symbol refers to the actual definition, regardless of which
   *  translation unit it actually resides in. */

  void link();

  /** @brief Access the list of procedure definitions */

  inline const proc_decl_map & procedures() const { return _procedure_declarations; }

  /** @brief Fix each idNode (the walker part of this class) */
  /** @brief Special case: function calls
   *
   *  The a function call can refer to an undeclared function. In this
   *  case, we create a declaration on the fly */

  virtual void at_id(idNode * the_id, Order ord);

  /** @brief Fix each threeAddrNode (the walker part of this class) */
  /** @brief Special case: function calls
   *
   *  The a function call can refer to an undeclared function. In this
   *  case, we create a declaration on the fly.  This function is used
   *  only for dismantled code.  */

  virtual void at_threeAddr(threeAddrNode * ta, Order ord);

  /** @brief Fix each callNode (the walker part of this class) */
  /** @brief Special case: function calls
   *
   *  The a function call can refer to an undeclared function. In this
   *  case, we create a declaration on the fly.  This function is used
   *  only for non-dismantled code.  */

  virtual void at_call(callNode * call, Order ord);


  /** @brief Look up a global variable */

  declNode * lookup_symbol(unitNode * current, string name, bool & is_synthetic_decl);

  /** @brief Lookup a procedure definition
   *
   *  Make sure that the input declNode comes from the lookup_symbol
   *  method. */

  procNode * lookup_procedure(declNode * decl);

  /** @brief Return the procedure declaration map */

  proc_decl_map & procedure_declarations();
private:

  bool create_synthetic(declNode * local_decl);

  void process_call(idNode * the_id, callNode * the_call);
};

#endif // CBZ_LINKER_H
