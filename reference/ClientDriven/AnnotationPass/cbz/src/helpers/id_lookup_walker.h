// $Id: id_lookup_walker.h,v 1.8 2003/09/26 15:40:15 toktb Exp $
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

#ifndef CBZ_ID_LOOKUP_WALKER_H
#define CBZ_ID_LOOKUP_WALKER_H

#include "scope_walker.h"

/** @brief finds ids in current scope.
    @ingroup Phases
*/
class id_lookup_walker : public scope_walker
{
public:

  // -- Use this interface to access the walker..

  static void fixup(unitNode * u, bool verb);

  // -- This interface also returns the walker..

  static id_lookup_walker * fixup_and_return(unitNode * u, bool verb);

private:

  bool _verbose;

  bool _enum_in_scope;
  int  _su_in_scope;

  void conflict(const Coord & c, const string & msg);

public:

  id_lookup_walker(unitNode * u, bool verb);

  // -- Put declarations into the symbol table

  virtual void at_proc(procNode * the_proc, Order ord);
  virtual void at_decl(declNode * the_decl, Order ord);
  virtual void at_enum(enumNode * the_enum, Order ord);
  virtual void at_sue(sueNode * the_sue, Order ord);

  // -- Lookup identifiers in the symbol table

  virtual void at_call(callNode * the_call, Order ord);
  virtual void at_tdef(tdefNode * the_tdef, Order ord);
  virtual void at_id(idNode * the_id, Order ord);
  virtual void at_binary(binaryNode * the_binary, Order ord);
};


#endif // CBZ_ID_LOOKUP_WALKER_H
