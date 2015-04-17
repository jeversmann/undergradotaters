// $Id: basic_type.h,v 1.9 2003/08/11 17:13:57 abrown Exp $
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

#ifndef CBZ_BASIC_TYPE_H
#define CBZ_BASIC_TYPE_H

#include "coord.h"

class basic_type
{

private:

  // -- PRIVATE: Global values for basic type fields...

  // -- Basic types
  static const unsigned int NO_TYPE, INT, CHAR, FLOAT, VOID, ELLIPSIS;

  // -- Length
  static const unsigned int NO_LENGTH, SHORT, LONG, LONGLONG;

  // -- Sign (NOTE: it defaults to signed)
  static const unsigned int NO_SIGN, SIGNED, UNSIGNED;

  // -- Used in parsing only
  static const unsigned int PARSE_ONLY;

public:

  // -- PUBLIC: Predefined global basic data types...

  static const basic_type Void;
  static const basic_type Ellipsis;
  static const basic_type SChar, UChar;
  static const basic_type SShort, UShort, SInt, UInt, SLong, ULong;
  static const basic_type SLongLong, ULongLong;
  static const basic_type Float, Double, LongDouble;

  // -- For parsing only...

  static const basic_type Char, Int;
  static const basic_type Signed, Unsigned;
  static const basic_type Short, Long, LongLong;
  static const basic_type IntParseOnly;

  // -- Sizes and alignments

  static const unsigned int TARGET_MAX_UCHAR, TARGET_MAX_INT, TARGET_MAX_UINT, 
    TARGET_MAX_LONG, TARGET_MAX_ULONG;

  static const unsigned int CHAR_SIZE, CHAR_ALIGN, SHORT_SIZE, SHORT_ALIGN, 
    INT_SIZE, INT_ALIGN, FLOAT_SIZE, FLOAT_ALIGN, DOUBLE_SIZE, DOUBLE_ALIGN,
    LONGDOUBLE_SIZE, LONGDOUBLE_ALIGN, LONG_SIZE, LONG_ALIGN, 
    /* LONGLONG_SIZE, LONGLONG_ALIGN, */ POINTER_SIZE, POINTER_ALIGN,
    BYTE_LENGTH, WORD_LENGTH;

private:

  unsigned int _base_type:3;
  unsigned int _length:2;
  unsigned int _sign:2;
  unsigned int _parse_only:1;

  inline unsigned int base_type() const { return _base_type; }
  inline void base_type(unsigned int bt) { _base_type = bt; }

  inline unsigned int length() const { return _length; }
  inline void length(unsigned int l) { _length = l; }

  inline unsigned int sign() const { return _sign; }
  inline void sign(unsigned int s) { _sign = s; }

public:

  basic_type()
    : _base_type(0), _length(0), _sign(0), _parse_only(0)
  {}

  basic_type(unsigned int sign, unsigned int length, 
	     unsigned int base_type, unsigned int parse_only = 0)
    : _base_type(base_type),
      _length(length),
      _sign(sign),
      _parse_only(parse_only)
  {}

  // -- Simple == operator

  bool operator==(const basic_type & bt2) const {
    return ((base_type() == bt2.base_type()) &&
	    (length() == bt2.length()) &&
	    (sign() == bt2.sign()));
  }

  bool operator!=(const basic_type & bt2) const {
    return ! operator==(bt2);
  }

  // -- Less-than operator for sets and maps

  bool operator<(const basic_type & bt2) const {
    if (base_type() == bt2.base_type())
      if (length() == bt2.length())
	if (sign() == bt2.sign())
	  return false;
	else
	  return (sign() < bt2.sign());
      else
	return (length() < bt2.length());
    else
      return (base_type() < bt2.base_type());
  }

  // -- Query and modify

  inline bool is_no_sign() const { return _sign == NO_SIGN; }
  inline void set_no_sign() { _sign = NO_SIGN; }

  inline bool is_signed() const { return _sign == SIGNED; }
  inline void set_signed() { _sign = SIGNED; }

  inline bool is_unsigned() const { return _sign == UNSIGNED; }
  inline void set_unsigned() { _sign = UNSIGNED; }

  inline bool parse_only() const { return (_parse_only != 0); }
  inline void parse_only(bool po) { _parse_only = po ? 1 : 0; }

  inline bool is_no_type()      const { return _base_type == NO_TYPE; }
  inline void set_no_type()      { _base_type = NO_TYPE; }

  inline bool is_int()      const { return _base_type == INT; }
  inline void set_int()      { _base_type = INT; }

  inline bool is_char()     const { return _base_type == CHAR; }
  inline void set_char()     { _base_type = CHAR; }

  inline bool is_float()    const { return _base_type == FLOAT; }
  inline void set_float()    { _base_type = FLOAT; }

  inline bool is_void()     const { return _base_type == VOID; }
  inline void set_void()     { _base_type = VOID; }

  inline bool is_ellipsis() const { return _base_type == ELLIPSIS; }
  inline void set_ellipsis() { _base_type = ELLIPSIS; }

  inline bool is_no_length()  const { return _length == NO_LENGTH; }
  inline void set_no_length()  { _length = NO_LENGTH; }

  inline bool is_short()    const { return _length == SHORT; }  
  inline void set_short()    { _length = SHORT; }

  inline bool is_long()     const { return _length == LONG; }  
  inline void set_long()     { _length = LONG; }

  inline bool is_longlong() const { return _length == LONGLONG; }  
  inline void set_longlong() { _length = LONGLONG; }

  // -- Type predicates

  inline bool is_integer() const { return ((_base_type == INT) ||
					   (_base_type == CHAR)); }

  inline bool is_arithmetic() const { return ((_base_type == INT) ||
					      (_base_type == CHAR) ||
					      (_base_type == FLOAT)); }

  inline bool is_scalar() const { return ((_base_type != VOID) &&
					  (_base_type != ELLIPSIS)); }

  // -- Return a string representation

  string to_string() const;
  string base_name() const;
  string length_name() const;
  string sign_name() const;

  // -- Size and alignment

  int size();
  int alignment();

  // -- Merge basic types (used in MergePrimTypes type.c)

  void merge_in(basic_type & other, Coord coord = Coord::Unknown);

  // -- Finish (used in FinishPrimType type.c)

  void finish();

  // -- Return the basic type as a single number

  int to_num() const;

};


#endif // CBZ_BASIC_TYPE_H
