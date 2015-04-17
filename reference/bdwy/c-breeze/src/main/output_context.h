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

#ifndef CBZ_OUTPUT_CONTEXT_H
#define CBZ_OUTPUT_CONTEXT_H

#include <set>

typedef set<suespecNode *> ssn_set;

typedef list< int > int_list;
typedef int_list::iterator int_list_p;

class output_context
{
private:

  ostream & _out;
  int _pos;
  int _line;

  int_list _parens;

  // -- Maintain statement depth

  int _depth;

  // -- Remember the last output character

  char _last;

  // pnav
  // set to remember output suespecNodes
  ssn_set _ssNodes;

  // -- Internal new-line function

  void _new_line();

  // -- Print some spaces

  void _indent(const int num);

public:

  output_context(ostream & out)
    : _out(out),
      _pos(0),
      _line(0),
      _parens(),
      _depth(0),
      _last('\n'),
      _ssNodes()
  {}

  // -- Output operators

  output_context & operator<<(const string & s);
  output_context & operator<<(const char * s);

  // -- IMPORTANT: this operator controls indenting information so
  // it's important to always output delimiters using it.

  output_context & operator<<(const char c);

  // -- Handle statement boundaries..

  void indent_in()  { ++_depth; }
  void indent_out() { --_depth; }

  // -- Start a new line anywhere

  void new_line();

  // -- Continue a line that was getting too long

  void continue_line();

  // -- Add a space, if necessary (based on the last character)

  void space();

  // -- Return the current line position

  int pos() const { return _pos; }

  // pnav
  // remember a suespecNode so it is not printed again
  void printed(suespecNode * ssn);

  // pnav
  // return whether a suespecNode has been printed
  bool isPrinted(suespecNode * ssn);

  // -- Width for line-wrapping...

  static int wrap_width;

};


#endif // CBZ_OUTPUT_CONTEXT_H
