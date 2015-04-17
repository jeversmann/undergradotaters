// $Id: coord.cc,v 1.6 2003/08/07 23:13:02 pnav Exp $
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

extern char * cbtext;
extern char * lexer_line_start;

#define TAB_WIDTH 8

Coord Coord::Unknown(-1, -1, -1);

// ------------------------------------------------------------
// Convert simpleCoord to Coord
// ------------------------------------------------------------

Coord::Coord(simpleCoord s)
  : _line(s._line),
    _offset(s._offset),
    _file(CBZ::current_file)
{}

// ------------------------------------------------------------
// simpleCoord get_pos
// ------------------------------------------------------------

// Used only in the lexer

simpleCoord get_pos()
{
  simpleCoord sc;
  char * s;
  short int the_offset;

  the_offset = 0;
  for (s = lexer_line_start; s < cbtext; ++s) {
    if (*s == '\t')
      the_offset = ((the_offset / TAB_WIDTH) + 1) * TAB_WIDTH;
    else 
      ++ the_offset;
  }

  sc._offset = the_offset;
  sc._line = CBZ::current_line;

  return sc;
}

// ------------------------------------------------------------
// Get
// ------------------------------------------------------------

// Get the current file, line and offset from the lexer.

void Coord::get()
{
  char * s;
  short int the_offset;

  the_offset = 0;
  for (s = lexer_line_start; s < cbtext; ++s) {
    if (*s == '\t')
      the_offset = ((the_offset / TAB_WIDTH) + 1) * TAB_WIDTH;
    else 
      ++ the_offset;
  }

  file(CBZ::current_file);
  line(CBZ::current_line);
  offset(the_offset);
}

// ------------------------------------------------------------
// ------------------------------------------------------------

ostream& operator<< (ostream& lhs, const Coord & rhs) 
{
  if (rhs.file() == -1)
    lhs << "(unknown)";
  else {

    if (CBZ::PrintLineOffset)
      lhs << CBZ::Files[rhs.file()] << ":" << rhs.line() << ":" << rhs.offset();
    else
      lhs << CBZ::Files[rhs.file()] << ":" << rhs.line();
  }

  return lhs;
}

string Coord::to_string()
{
  ostringstream ost;

  if (file() == -1)
    ost << "(unknown)";
  else {

    if (CBZ::PrintLineOffset)
      ost << CBZ::Files[file()] << ":" << line() << ":" << offset();
    else
      ost << CBZ::Files[file()] << ":" << line();
  }

  return ost.str();
}

string & Coord::filename()
{
	return CBZ::Files[_file];
}
