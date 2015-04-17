// $Id: output_context.cc,v 1.4 2003/08/07 23:14:02 pnav Exp $
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

int output_context::wrap_width = 80;

// ------------------------------------------------------------
// Output operators
// ------------------------------------------------------------

output_context & output_context::operator<<(const string & s)
{
  _out << s;

  int l = s.length();
  if (l > 0)
    _last = s[l-1];

  _pos += s.length();

  return * this;
}

output_context & output_context::operator<<(const char * s)
{
  _out << s;

  int l = strlen(s);
  if (l>0)
    _last = s[l-1];

  _pos += strlen(s);

  return * this;
}

output_context & output_context::operator<<(const char c)
{
  _out << c;

  if ((c == '(') || (c == '{'))
    _parens.push_front(_pos+1);

  if ((c == ')') || (c == '}'))
    _parens.pop_front();

  _last = c;

  _pos++;

  return * this;
}

// ------------------------------------------------------------
// Internal methods
// ------------------------------------------------------------

// --- Internal new-line function makes sure that the offset
// is reset;

void output_context::_new_line()
{
  _out << endl;

  _last = '\n';

  _pos = 0;
  _line++;
}

// --- Indent out by printing some spaces...

void output_context::_indent(const int num)
{
  for (int i = 0; i < num; ++i)
    _out << ' ';

  if (num > 0)
    _last = ' ';

  _pos += num;
}

// ------------------------------------------------------------
// Start a new line
// ------------------------------------------------------------

void output_context::new_line()
{
  _new_line();
  _indent(_depth * 2);
}

// -- Check the current line length. If longer that the threshold,
// insert a new-line and indent to the proper depth.

void output_context::continue_line()
{
  if (pos() > wrap_width) {
    if (! _parens.empty()) {
      _new_line();
      _indent(_parens.front());
    }
    else
      new_line();
  }
}

// -- Add a space, if it's needed. That need is determined by looking
// at the last character sent to output.

void output_context::space()
{
  if ((_last != ' ') &&
      (_last != '\n') &&
      (_last != '{') &&
      (_last != '(')) {
    _out << ' ';
    _last = ' ';
    _pos++;
  }
}


// pnav
// remember a suespecNode so it is not printed again
void output_context::printed(suespecNode * ssn) {
  _ssNodes.insert(ssn);
}

// pnav
// return whther a suespecNode has been printed
bool output_context::isPrinted(suespecNode * ssn) {
  return (_ssNodes.find(ssn) != _ssNodes.end());
}
