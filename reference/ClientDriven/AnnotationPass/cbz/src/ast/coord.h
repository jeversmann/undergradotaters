// $Id: coord.h,v 1.5 2003/08/07 23:13:02 pnav Exp $
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

#ifndef CBZ_COORD_H
#define CBZ_COORD_H



// simpleCoord is used only during parsing because the %union in yacc
// cannot contain a type that requires a constructor.

typedef struct {
  int _line;
  short int _offset;
} simpleCoord;

simpleCoord get_pos();

// ------------------------------------------------------------
// Coord
// ------------------------------------------------------------

class Coord {
  
private:
  int       _line;
  short int _offset;
  short int _file;

public:
  // --- Global coordinate for "unknown location" in the source...

  static Coord Unknown;

  // --- Constructor

  Coord(int line, short int offset, short int file)
    : _line(line), 
      _offset(offset), 
      _file(file)
  {}

  Coord(simpleCoord s);

  inline int line() const { return _line; }
  inline void line(int line) { _line = line; }

  inline short int offset() const { return _offset; }
  inline void offset(short int offset) { _offset = offset; }

  inline short int file() const { return _file; }
  inline void file(short int file) { _file = file; }

  string & filename();

  void get();

  inline bool is_unknown() const { return (_file == -1); }

  friend ostream& operator << (ostream & , const Coord &);

  string to_string();
};



#endif // CBZ_COORD_H
