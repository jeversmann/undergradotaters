// $Id: sue_complete_walker.h,v 1.5 2003/08/07 23:13:54 pnav Exp $
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

#ifndef CBZ_SUE_COMPLETE_WALKER_H
#define CBZ_SUE_COMPLETE_WALKER_H



/** @brief checks completeness of struct, union, and enum declarations.
    @ingroup Phases
*/
class sue_complete_walker : public Walker
{

public:

  static void check(Node * n)
  {
    sue_complete_walker w;
    n->walk(w);
  }

private:

public:

  sue_complete_walker()
    : Walker(Preorder, Subtree)
  { }

  /// At any node, do nothing
  virtual void at_node(Node * the_node, Order ord)
  { }

  /// At SUE nodes, check for completeness..
  virtual void at_suespec(suespecNode * the_suespec, Order ord)
  {
    //    if (the_suespec->typ() != Edcl) {
    // -- incomplete enums are always illegal and are
    //    caught by BuildEnum (K&R2 A8.4, p215)
    // -- ...not anymore

    if (! the_suespec->complete()) {
      string ty;

      switch (the_suespec->owner()) {
      case Struct:
	ty = string("structure");
	break;
      case Union:
	ty = string("union");
	break;
      case Enum:
	ty = string("enum");
	break;
      default:
	break;
      }

      if (the_suespec->name().empty())
	CBZ::SyntaxError("incomplete unnamed " + ty);
      else
	CBZ::SyntaxError("incomplete" + ty + " `" + the_suespec->name() + "'");
    }
  }
};



#endif // CBZ_SUE_COMPLETE_WALKER_H
