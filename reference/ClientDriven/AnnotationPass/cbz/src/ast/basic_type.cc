// $Id: basic_type.cc,v 1.5 2003/08/07 23:12:58 pnav Exp $
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
#include <climits>

// ------------------------------------------------------------
// Constant values
// ------------------------------------------------------------

// -- PRIVATE: Global values for basic type fields...

// -- Basic types
const unsigned int basic_type::NO_TYPE  = 0x0;
const unsigned int basic_type::INT      = 0x1;
const unsigned int basic_type::CHAR     = 0x2;
const unsigned int basic_type::FLOAT    = 0x3;
const unsigned int basic_type::VOID     = 0x4;
const unsigned int basic_type::ELLIPSIS = 0x5;

// -- Length
const unsigned int basic_type::NO_LENGTH = 0x0;
const unsigned int basic_type::SHORT     = 0x1;
const unsigned int basic_type::LONG      = 0x2;
const unsigned int basic_type::LONGLONG  = 0x3;

// -- Sign

const unsigned int basic_type::NO_SIGN  = 0x0;
const unsigned int basic_type::SIGNED   = 0x1;
const unsigned int basic_type::UNSIGNED = 0x2;

// -- Used in parsing only
const unsigned int basic_type::PARSE_ONLY = 0x1;

// --- PRIVATE: Global values for basic type size and alignment...

// -- target limits

const unsigned int basic_type::TARGET_MAX_UCHAR = 256;
const unsigned int basic_type::TARGET_MAX_INT   = INT_MAX;
const unsigned int basic_type::TARGET_MAX_UINT  = UINT_MAX;
const unsigned int basic_type::TARGET_MAX_LONG  = LONG_MAX;
const unsigned int basic_type::TARGET_MAX_ULONG = ULONG_MAX;

//-- Basic sizes and alignments */

const unsigned int basic_type::CHAR_SIZE =         sizeof(char);
const unsigned int basic_type::CHAR_ALIGN =        CHAR_SIZE;

const unsigned int basic_type::SHORT_SIZE =        sizeof(short);
const unsigned int basic_type::SHORT_ALIGN =       SHORT_SIZE;

const unsigned int basic_type::INT_SIZE =          sizeof(int);
const unsigned int basic_type::INT_ALIGN =         INT_SIZE;

const unsigned int basic_type::FLOAT_SIZE =        sizeof(float);
const unsigned int basic_type::FLOAT_ALIGN =       FLOAT_SIZE;

const unsigned int basic_type::DOUBLE_SIZE =       sizeof(double);
const unsigned int basic_type::DOUBLE_ALIGN =      DOUBLE_SIZE;

const unsigned int basic_type::LONGDOUBLE_SIZE =   sizeof(long double);
const unsigned int basic_type::LONGDOUBLE_ALIGN =  LONGDOUBLE_SIZE;

const unsigned int basic_type::LONG_SIZE =         sizeof(long);
const unsigned int basic_type::LONG_ALIGN =        LONG_SIZE;

/*
const unsigned int basic_type::LONGLONG_SIZE =     sizeof(long long);
const unsigned int basic_type::LONGLONG_ALIGN =    LONGLONG_SIZE;
*/

const unsigned int basic_type::POINTER_SIZE =      sizeof(void *);
const unsigned int basic_type::POINTER_ALIGN =     POINTER_SIZE;

const unsigned int basic_type::BYTE_LENGTH =   CHAR_BIT;
const unsigned int basic_type::WORD_LENGTH =   (INT_SIZE * BYTE_LENGTH);

// ------------------------------------------------------------
// Initialize global basic types
// ------------------------------------------------------------

// -- Now the types...

const basic_type basic_type::Void(0,0,basic_type::VOID);
const basic_type basic_type::Ellipsis(0,0,basic_type::ELLIPSIS);

const basic_type basic_type::SChar(basic_type::SIGNED, 0, basic_type::CHAR);
const basic_type basic_type::UChar(basic_type::UNSIGNED, 0, basic_type::CHAR);

const basic_type basic_type::SShort(basic_type::SIGNED, basic_type::SHORT, basic_type::INT);
const basic_type basic_type::UShort(basic_type::UNSIGNED, basic_type::SHORT, basic_type::INT);

const basic_type basic_type::SInt(basic_type::SIGNED, 0, basic_type::INT);
const basic_type basic_type::UInt(basic_type::UNSIGNED, 0, basic_type::INT);

const basic_type basic_type::SLong(basic_type::SIGNED, basic_type::LONG, basic_type::INT);
const basic_type basic_type::ULong(basic_type::UNSIGNED, basic_type::LONG, basic_type::INT);

const basic_type basic_type::SLongLong(basic_type::SIGNED, basic_type::LONGLONG, basic_type::INT);
const basic_type basic_type::ULongLong(basic_type::UNSIGNED, basic_type::LONGLONG, basic_type::INT);
const basic_type basic_type::Float(0, 0, basic_type::FLOAT);
const basic_type basic_type::Double(0, basic_type::LONG, basic_type::FLOAT);
const basic_type basic_type::LongDouble(0, basic_type::LONGLONG, basic_type::FLOAT);

// -- These are only used in the parser...

const basic_type basic_type::Char(0,0,basic_type::CHAR);
const basic_type basic_type::Int(0, 0, basic_type::INT);

const basic_type basic_type::Signed(basic_type::SIGNED, 0,0);
const basic_type basic_type::Unsigned(basic_type::UNSIGNED, 0,0);

const basic_type basic_type::Short(0,basic_type::SHORT,0);
const basic_type basic_type::Long(0,basic_type::LONG,0);

const basic_type basic_type::IntParseOnly(0,0,basic_type::INT, true);

// ------------------------------------------------------------
// MergePrimTypes from type.c
// (the "other" object is deleted
// ------------------------------------------------------------

void basic_type::merge_in(basic_type & bt2, Coord coord)
{
  basic_type & bt1 = * this;;

  // First merge base type (int, char, float, double, ...).
  // At most one base type may be specified. 

  if (! bt1.is_no_type() && ! bt2.is_no_type()) 
    CBZ::SyntaxError(coord,
		     string("conflicting type specifiers: `") + bt1.base_name() +
		     string("' and `") + bt2.base_name() + string("'"));
  else
    bt1._base_type += bt2._base_type;

  // Now merge signs.  At most one sign may be specified; it appears to
  // be legal in ANSI to repeat the same sign (as in 
  // "unsigned unsigned int"), but a warning is generated.

  if (! bt1.is_no_sign() && ! bt2.is_no_sign())
    if (bt1.sign() != bt2.sign())
      CBZ::SyntaxError(coord,
		       string("conflicting type specifiers: `") + bt1.sign_name() +
		       string("' and `") + bt2.sign_name() + string("'"));
    else
      CBZ::Warning(3, coord,
		   string("repeated type specifier: `") + bt1.sign_name() + string("'"));
  else
    bt1._sign += bt2._sign;
    
  // Check that the resulting sign is compatible with the resulting
  // base type.
  // Only int and char may have a sign specifier.

  if (! bt1.is_no_sign() && ! bt1.is_no_type())
    if ( ! bt1.is_int() && ! bt1.is_char()) {
      CBZ::SyntaxError(coord,
		       string("conflicting type specifiers: `") + bt1.base_name() +
		       string("' and `") + bt1.sign_name() + string("'"));
      bt1.set_no_sign();
    }

  // Merge lengths (short, long, long long).

  if ( ! bt1.is_no_length() && ! bt2.is_no_length()) {
    if (bt1.is_long() && bt2.is_long() && ! CBZ::ANSIOnly)
      bt1.set_longlong();
    else
      CBZ::SyntaxError(coord,
		       string("conflicting type specifiers: `") + bt1.length_name() +
		       string("' and `") + bt2.length_name() + string("'"));
  } else
    bt1._length += bt2._length;
    
  // Check that the resulting length is compatible with the resulting
  // base type.
  // Only int may have any length specifier; double and float may have long.

  if ( ! bt1.is_no_length() && ! bt1.is_no_type())
    if ( ! bt1.is_int() 
		&& ! (bt1.is_float() && bt1.is_long()) 
		&& ! (bt1.is_float() && bt1.is_longlong()) ) {
	/* bug here ### */
      CBZ::SyntaxError(coord,
		       string("conflicting type specifiers: `") + bt1.base_name() +
		       string("' and `") + bt1.length_name() + string("'"));
      bt1.set_no_length();
    }
}

// ------------------------------------------------------------
// Finish (FinishPrimType type.c)
// ------------------------------------------------------------

void basic_type::finish()
{
  // -- If no type is specified, then it's an int

  if (parse_only() || is_no_type())
    set_int();

  // -- If no sign is specified, then it's signed

  if ((is_int() || is_char()) && is_no_sign())
    set_signed();
}

// ------------------------------------------------------------
// Size and Alignment information
// ------------------------------------------------------------

int basic_type::size()
{
  switch (base_type()) {
  case CHAR: return CHAR_SIZE;

  case INT:
    switch (length()) {
    case SHORT: return SHORT_SIZE;
    case LONG:  return LONG_SIZE;
//    case LONGLONG: return LONGLONG_SIZE;
    default: return INT_SIZE;
    }
    break;

  case FLOAT:
    switch (length()) {
    case LONG: return DOUBLE_SIZE;
 //   case LONGLONG: return LONGDOUBLE_SIZE;
    default: return FLOAT_SIZE;
    }
    break;

  case VOID:
    CBZ::SyntaxError("Can't compute size of type void");
    break;

  default:
    CBZ::SyntaxError("Size received unknown primitive type");
    break;
  }

  return 0;
}

int basic_type::alignment()
{
  switch (base_type()) {
  case CHAR: return CHAR_ALIGN;

  case INT:
    switch (length()) {
    case SHORT: return SHORT_ALIGN;
    case LONG:  return LONG_ALIGN;
//    case LONGLONG: return LONGLONG_ALIGN;
    default: return INT_ALIGN;
    }
    break;

  case FLOAT:
    switch (length()) {
    case LONG: return DOUBLE_ALIGN;
    case LONGLONG: return LONGDOUBLE_ALIGN;
    default: return FLOAT_ALIGN;
    }
    break;

  case VOID:
    CBZ::SyntaxError("Can't compute alignment of type void");
    break;

  default:
    CBZ::SyntaxError("Align received unknown primitive type");
    break;
  }

  return 0;
}

// ------------------------------------------------------------
// String representation
// ------------------------------------------------------------

string basic_type::base_name() const
{
  switch (base_type()) {
  case VOID:     return string("void");
  case ELLIPSIS: return string("...");
  case FLOAT:
    if (length() == LONG || length() == LONGLONG)
                 return string("double");
    else
 		 return string("float");
  case CHAR:     return string("char"); 
  case INT:      return string("int");
  default:
    return string("");
  }
}

string basic_type::length_name() const
{
  if (base_type() == FLOAT)
    if (length() == LONGLONG)
      return string("long");
    else
      return string("");

  switch (length()) {
  case SHORT:     return string("short");
  case LONG:      return string("long");
  case LONGLONG:  return string("long long");
  default:
    return string("");
  }
}

string basic_type::sign_name() const
{
  switch (sign()) {
  case UNSIGNED:  return string("unsigned");
    // case SIGNED:    return string("signed");
  default:
    return string("");
  }
}

string basic_type::to_string() const
{
  unsigned int bt = base_type();
  unsigned int l = length();
  bool us = is_unsigned();
  bool not_empty = false;

  if (parse_only())
    return string("int");

  string bn = base_name();
  string ln = length_name();
  string sn = sign_name();
  string out;

  if (bt == FLOAT) {
    if (! ln.empty()) {
      out += ln;
      not_empty = true;
    }

    if (! bn.empty()) {
      if (not_empty)
	out += " ";
      out += bn;
      not_empty = true;
    }
  }

  if ((bt == VOID) || (bt == ELLIPSIS))
    out += bn;

  if ((bt == CHAR) || (bt == INT)) {
    if (! sn.empty()) {
      if (not_empty)
	out += " ";
      out += sn;
      not_empty = true;
    }

    if (! ln.empty()) {
      if (not_empty)
	out += " ";
      out += ln;
      not_empty = true;
    }

    if (! bn.empty()) {
      if (not_empty)
	out += " ";
      out += bn;
      not_empty = true;
    }
  }

  return out;
}

int basic_type::to_num() const
{
  int val = 0;

  val |= _base_type;

  val = val << 3; // Number of bits in the base type

  val |= _length;

  val = val << 2; // Number of bits in the length

  val |= _sign;

  val = val << 2; // Number of bits in the sign

  // -- Skip parse-only

  return val;
}
