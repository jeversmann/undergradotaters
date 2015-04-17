// $Id: constant.h,v 1.12 2003/08/11 17:15:42 abrown Exp $
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

#ifndef CBZ_CONSTANT_H
#define CBZ_CONSTANT_H

#include "basic_type.h"
#include "operators.h"
#include <cstring>
#include <cstdlib>

class constant {

private:

  union {
    signed char SChar;
    unsigned char UChar;
    signed short int SShort;
    unsigned short int UShort;
    signed int SInt;
    unsigned int UInt;
    signed long int SLong;
    unsigned long int ULong;
    float Float;
    double Double;

    char * Str;
    void * Ptr;
  } _v;

  basic_type _bt;
  bool _is_ptr;
  bool _is_str;
  bool _no_val;

public:

  inline const basic_type & basic() const { return _bt; }

  constant(signed char SChar)
    : _bt(basic_type::SChar), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.SChar = SChar; }

  inline signed char SChar() const { return _v.SChar; }
  inline void SChar(signed char val) { *this = constant(val); }

  constant(unsigned char UChar)
    : _bt(basic_type::UChar), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.UChar = UChar; }

  inline unsigned char UChar() const { return _v.UChar; }
  inline void UChar(unsigned char val) { *this = constant(val); }

  constant(signed short int SShort)
    : _bt(basic_type::SShort), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.SShort = SShort; }

  inline signed short int SShort() const { return _v.SShort; }
  inline void SShort(signed short int val) { *this = constant(val); }

  constant(unsigned short int UShort)
    : _bt(basic_type::UShort), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.UShort = UShort; }

  inline unsigned short int UShort() const { return _v.UShort; }
  inline void UShort(unsigned short int val) { *this = constant(val); }

  constant(signed int SInt)
    : _bt(basic_type::SInt), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.SInt = SInt; }

  inline signed int SInt() const { return _v.SInt; }
  inline void SInt(signed int val) { *this = constant(val); }

  constant(unsigned int UInt)
    : _bt(basic_type::UInt), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.UInt = UInt; }

  inline unsigned int UInt() const { return _v.UInt; }
  inline void UInt(unsigned int val) { *this = constant(val); }

  constant(signed long int SLong)
    : _bt(basic_type::SLong), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.SLong = SLong; }

  inline signed long int SLong() const { return _v.SLong; }
  inline void SLong(signed long int val) { *this = constant(val); }

  constant(unsigned long int ULong)
    : _bt(basic_type::ULong), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.ULong = ULong; }

  inline unsigned long int ULong() const { return _v.ULong; }
  inline void ULong(unsigned long int val) { *this = constant(val); }

  constant(float Float)
    : _bt(basic_type::Float), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.Float = Float; }

  inline float Float() const { return _v.Float; }
  inline void Float(float val) { *this = constant(val); }

  constant(double Double)
    : _bt(basic_type::Double), _is_ptr(false), _is_str(false), _no_val(false)
  { _v.Double = Double; }

  inline double Double() const { return _v.Double; }
  inline void Double(double val) { *this = constant(val); }

  constant(const char * Str)
    : _bt(), _is_ptr(false), _is_str(true), _no_val(false)
  { _v.Str = strdup(Str); }

  inline char * Str() const { return _v.Str; }
  inline bool is_str() const { return _is_str; }

  constant(void * Ptr)
    : _bt(), _is_ptr(true), _is_str(false), _no_val(false)
  { _v.Ptr = Ptr; }

  inline void * Ptr() const { return _v.Ptr; }
  inline bool is_ptr() const { return _is_ptr; }

  constant()
    : _bt(), _is_ptr(false), _is_str(false), _no_val(true)
  {}

  inline bool no_val() const { return _no_val; }
  inline void set_no_val() { _no_val = true; }

  // -- Copy constructor

  constant(const constant & other);

  // -- Destructor

  ~constant();

  // -- Assignment operators

  constant & operator=(const constant & rhs);

  // -- More general operations..

  unsigned long Integer() const;
  bool Boolean() const;
  bool is_zero() const;
  void make_zero();

  // -- Comparison

  bool is_equal_to(const constant & other) const;

  // -- Less-than operator, for sets and maps

  bool operator<(const constant & other) const;

  // -- Evaluate expressions

  static constant eval(const Operator * op,
		       const constant & operand1,
		       const constant & operand2);

  static constant eval(const Operator * op,
		       const constant & operand);

  static constant cast(const basic_type & new_bt, const constant & con);

  // -- Output as a string...

  // wantQuoteChar - if true, format char as 'x'
  //                 if false, format char as a number, like any other
  string to_string(bool wantQuoteChar = true) const;

private:

  static void print_char(int value, ostringstream & ost);

};



#endif // CBZ_CONSTANT_H
