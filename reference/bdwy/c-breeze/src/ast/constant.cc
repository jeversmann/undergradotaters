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
#include <stdio.h>
#include "c_breeze.h"

// ------------------------------------------------------------
// Copy constructor
// ------------------------------------------------------------

constant::constant(const constant & other)
  : _v(other._v),
    _bt(other._bt),
    _is_str(other._is_str),
    _is_ptr(other._is_ptr),
    _no_val(other._no_val)
{
  if (_is_str)
    _v.Str = strdup(other._v.Str);
}

// ------------------------------------------------------------
// Assignment operator
// ------------------------------------------------------------

constant & constant::operator=(const constant & rhs)
{
  if (this != &rhs) {
    if (rhs.is_str()) {
	  if (_is_str)
        delete _v.Str;
      _v.Str = strdup(rhs._v.Str);
    }
    else
      _v = rhs._v;

    _bt     = rhs._bt;
    _is_str = rhs._is_str;
    _is_ptr = rhs._is_ptr;
    _no_val = rhs._no_val;
  }

  return * this;
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

constant::~constant()
{
  if (_is_str)
    free(_v.Str);
}

// ------------------------------------------------------------
// Access values
// ------------------------------------------------------------

unsigned long constant::Integer() const
{
  if (_bt == basic_type::SShort)
    return (unsigned long) _v.SShort;

  if (_bt == basic_type::UShort)
    return (unsigned long) _v.UShort;

  if (_bt == basic_type::SInt)
    return (unsigned long) _v.SInt;

  if (_bt == basic_type::UInt)
    return (unsigned long) _v.UInt;

  if (_bt == basic_type::SLong)
    return (unsigned long) _v.SLong;

  if (_bt == basic_type::ULong)
    return (unsigned long) _v.ULong;

  // -- Should never get here...

  return 0;
}

bool constant::Boolean() const
{
  if (is_str())
    return true;

  if (is_ptr())
    return _v.Ptr != 0;
  
  if (_bt == basic_type::SShort)
    return _v.SShort != 0 ;

  if (_bt == basic_type::UShort)
    return _v.UShort != 0 ;

  if (_bt == basic_type::SInt)
    return _v.SInt != 0 ;

  if (_bt == basic_type::UInt)
    return _v.UInt != 0 ;

  if (_bt == basic_type::SLong)
    return _v.SLong != 0 ;

  if (_bt == basic_type::ULong)
    return _v.ULong != 0 ;

  if (_bt == basic_type::Float)
    return _v.Float != 0 ;

  if (_bt == basic_type::Double)
    return _v.Double != 0 ;

  // -- Should never get here...
  // pnav -- got here...
  // assert( false );


  // pnav -- gets here when _bt == basic_type::NO_TYPE
  return false;
}

bool constant::is_zero() const
{
  return ! Boolean();
}

void constant::make_zero()
{
  if (is_str()) {
    if ( _v.Str )
		free( _v.Str );
	_v.Str = 0;
  }

  else if (is_ptr())
    _v.Ptr = 0;
  
  else if (_bt == basic_type::SShort)
    _v.SShort = 0;

  else if (_bt == basic_type::UShort)
    _v.UShort = 0;

  else if (_bt == basic_type::SInt)
    _v.SInt = 0;

  else if (_bt == basic_type::UInt)
    _v.UInt = 0;

  else if (_bt == basic_type::SLong)
    _v.SLong = 0;

  else if (_bt == basic_type::ULong)
    _v.ULong = 0;

  else if (_bt == basic_type::Float)
    _v.Float = 0;

  else if (_bt == basic_type::Double)
    _v.Double = 0;

  else {
	  // make us a simple integer zero
	  SInt( 0 );
  }

  return;
}

void constant::make_zero( lir_var_type type )
{
	switch ( type )
	{
	case vt_char:		SChar( 0 ); break;
	case vt_uchar:		UChar( 0 ); break;
	case vt_short:		SShort( 0 ); break;
	case vt_ushort:		UShort( 0 ); break;
	case vt_int:		SInt( 0 ); break;
	case vt_uint:		UInt( 0 ); break;
	case vt_long:		SLong( 0 ); break;
	case vt_ulong:		ULong( 0 ); break;
	case vt_float:		Float( 0 ); break;
	case vt_double:		Double( 0 ); break;
	case vt_func_decl:	ULong( 0 ); break;
	case vt_pointer:	ULong( 0 ); break;

	// no other type has a sensical zero
	default:
		cerr << "Can't generate zero value for low-level type " << type << endl;
		assert( false );
	}
}

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------

bool constant::is_equal_to(const constant & other) const
{
  if (is_str() &&
      other.is_str())
    return (strcmp(Str(),other.Str()) == 0);

  if (is_ptr() &&
      other.is_ptr())
    return Ptr() == other.Ptr();

  if (basic().is_integer() &&
      other.basic().is_integer())
    return Integer() == other.Integer();

  if ((basic() == basic_type::Float) &&
      (other.basic() == basic_type::Float))
    return  Float() == other.Float();

  if ((basic() == basic_type::Double) &&
      (other.basic() == basic_type::Double))
    return  Double() == other.Double();

  return false;
}

bool constant::operator<(const constant & other) const
{
  // -- CASE 1: Non value is the smallest element

  if (no_val() && other.no_val())
    return false;

  if (no_val())
    return true;

  if (other.no_val())
    return false;

  // -- CASE 2: Strings

  if (is_str() &&
      other.is_str())
    return (strcmp(Str(),other.Str()) < 0);

  // -- CASE 3: Pointers

  if (is_ptr() &&
      other.is_ptr())
    return Ptr() < other.Ptr();

  // -- CASE 4: Regular numeric types

  if (basic() == other.basic()) {
    constant result = constant::eval(Operators::table['<'], *this, other);
    return result.Boolean();
  }
  else {

    // -- CASE 5: Two different basic types: Always return false?

    return basic() < other.basic();
  }
}

// ------------------------------------------------------------
// Expression evaluation
// (static functions)
// ------------------------------------------------------------

// There are several conditions on this function working:
//  (1) The semcheck_visitor has been run.
//  (2) The operands are of the same basic_type
//  (3) Integral promotion has removed any char/short types.
//  (4) Only arithmetic operations are allowed
//  (5) No pointer operations are supported

constant constant::eval(const Operator * op,
			const constant & oper1,
			const constant & oper2)
{
  // -- If either input is a non-value, or the basic types don't
  // match, then simply return another non-value.

  // pnav -- is a null pointer here an error, or just another case for this guard?
  if (!&oper1 || !&oper2 ||
      oper1.no_val() ||
      oper2.no_val() ||
      ( ! (oper1.basic() == oper2.basic())))
    return constant();

  // -- Big switch...

  const basic_type & bt = oper1.basic();

  switch (op->id()) {
  case '*':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() * oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() * oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() * oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() * oper2.ULong());

    if (bt == basic_type::Float)
      return constant(oper1.Float() * oper2.Float());

    if (bt == basic_type::Double)
      return constant(oper1.Double() * oper2.Double());

    break;

  case '/':
    if (bt == basic_type::SInt) {
      if (oper2.SInt() == 0)
	return constant();
      else
	return constant(oper1.SInt() / oper2.SInt());
    }

    if (bt == basic_type::UInt) {
      if (oper2.UInt() == 0)
	return constant();
      else
	return constant(oper1.UInt() / oper2.UInt());
    }

    if (bt == basic_type::SLong) {
      if (oper2.SLong() == 0)
	return constant();
      else
	return constant(oper1.SLong() / oper2.SLong());
    }

    if (bt == basic_type::ULong) {
      if (oper2.ULong() == 0)
	return constant();
      else
	return constant(oper1.ULong() / oper2.ULong());
    }

    if (bt == basic_type::Float) {
      if (oper2.Float() == 0.0)
	return constant();
      else
	return constant(oper1.Float() / oper2.Float());
    }

    if (bt == basic_type::Double) {
      if (oper2.Double() == 0.0)
	return constant();
      else
	return constant(oper1.Double() / oper2.Double());
    }

    break;

  case '%':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() % oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() % oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() % oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() % oper2.ULong());

    break;

  case '+':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() + oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() + oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() + oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() + oper2.ULong());

    if (bt == basic_type::Float)
      return constant(oper1.Float() + oper2.Float());

    if (bt == basic_type::Double)
      return constant(oper1.Double() + oper2.Double());

    break;

  case '-':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() - oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() - oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() - oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() - oper2.ULong());

    if (bt == basic_type::Float)
      return constant(oper1.Float() - oper2.Float());

    if (bt == basic_type::Double)
      return constant(oper1.Double() - oper2.Double());

    break;

  case Operator::LS:
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() << oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() << oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() << oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() << oper2.ULong());

    break;

  case Operator::RS:
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() >> oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() >> oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() >> oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() >> oper2.ULong());

    break;

  case '<':
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() < oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() < oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() < oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() < oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() < oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() < oper2.Double()));

    break;

  case '>':
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() > oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() > oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() > oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() > oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() > oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() > oper2.Double()));

    break;

  case Operator::GE:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() >= oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() >= oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() >= oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() >= oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() >= oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() >= oper2.Double()));

    break;

  case Operator::LE:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() <= oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() <= oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() <= oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() <= oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() <= oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() <= oper2.Double()));

    break;

  case Operator::EQ:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() == oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() == oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() == oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() == oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() == oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() == oper2.Double()));

    break;

  case Operator::NE:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() != oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() != oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() != oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() != oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() != oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() != oper2.Double()));

    break;

  case '&':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() & oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() & oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() & oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() & oper2.ULong());

    break;

  case '^':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() ^ oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() ^ oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() ^ oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() ^ oper2.ULong());

    break;

  case '|':
    if (bt == basic_type::SInt)
      return constant(oper1.SInt() | oper2.SInt());

    if (bt == basic_type::UInt)
      return constant(oper1.UInt() | oper2.UInt());

    if (bt == basic_type::SLong)
      return constant(oper1.SLong() | oper2.SLong());

    if (bt == basic_type::ULong)
      return constant(oper1.ULong() | oper2.ULong());

    break;

  case Operator::ANDAND:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() && oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() && oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() && oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() && oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() && oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() && oper2.Double()));

    break;

  case Operator::OROR:
    if (bt == basic_type::SInt)
      return constant( (int) (oper1.SInt() || oper2.SInt()));

    if (bt == basic_type::UInt)
      return constant( (int) (oper1.UInt() || oper2.UInt()));

    if (bt == basic_type::SLong)
      return constant( (int) (oper1.SLong() || oper2.SLong()));

    if (bt == basic_type::ULong)
      return constant( (int) (oper1.ULong() || oper2.ULong()));

    if (bt == basic_type::Float)
      return constant( (int) (oper1.Float() || oper2.Float()));

    if (bt == basic_type::Double)
      return constant( (int) (oper1.Double() || oper2.Double()));

    break;
  default:
    break;
  }

  // -- If none of the cases match, return non-value

  return constant();
}

constant constant::eval(const Operator * op,
			const constant & oper)
{
  // -- If the input is a non-value, return a non-value

  if (oper.no_val())
    return constant();

  // -- Big switch...

  const basic_type & bt = oper.basic();

  switch (op->id()) {
  case Operator::UPLUS:
    if (bt == basic_type::SInt)
      return constant( + oper.SInt());

    if (bt == basic_type::UInt)
      return constant( + oper.UInt());

    if (bt == basic_type::SLong)
      return constant( + oper.SLong());

    if (bt == basic_type::ULong)
      return constant( + oper.ULong());

    if (bt == basic_type::Float)
      return constant( + oper.Float());

    if (bt == basic_type::Double)
      return constant( + oper.Double());

    break;

  case Operator::UMINUS:
    if (bt == basic_type::SInt)
      return constant( - oper.SInt());

    if (bt == basic_type::UInt)
      return constant( - oper.UInt());

    if (bt == basic_type::SLong)
      return constant( - oper.SLong());

    if (bt == basic_type::ULong)
      return constant( - oper.ULong());

    if (bt == basic_type::Float)
      return constant( - oper.Float());

    if (bt == basic_type::Double)
      return constant( - oper.Double());

    break;

  case '~':
    if (bt == basic_type::SInt)
      return constant( ~ oper.SInt());

    if (bt == basic_type::UInt)
      return constant( ~ oper.UInt());

    if (bt == basic_type::SLong)
      return constant( ~ oper.SLong());

    if (bt == basic_type::ULong)
      return constant( ~ oper.ULong());

    break;

  case '!':
    if (bt == basic_type::SInt)
      return constant( ! oper.SInt());

    if (bt == basic_type::UInt)
      return constant( ! oper.UInt());

    if (bt == basic_type::SLong)
      return constant( ! oper.SLong());

    if (bt == basic_type::ULong)
      return constant( ! oper.ULong());

    if (bt == basic_type::Float)
      return constant( ! oper.Float());

    if (bt == basic_type::Double)
      return constant( ! oper.Double());

    break;

  default:
    break;
  }

  // -- If none of the cases match, return non-value

  return constant();
}

constant constant::cast(const basic_type & new_bt, const constant & con)
{
  // -- If the input is a non-value, return a non-value

  if (con.no_val())
    return constant();

  // -- Consider all possible combinations of primitive types

  const basic_type & bt = con.basic();

  if (bt == basic_type::SChar) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.SChar());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.SChar());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.SChar());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.SChar());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.SChar());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.SChar());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.SChar());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.SChar());

    if (new_bt == basic_type::Float)
      return constant( (float) con.SChar());

    if (new_bt == basic_type::Double)
      return constant( (double) con.SChar());
  }

  if (bt == basic_type::UChar) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.UChar());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.UChar());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.UChar());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.UChar());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.UChar());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.UChar());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.UChar());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.UChar());

    if (new_bt == basic_type::Float)
      return constant( (float) con.UChar());

    if (new_bt == basic_type::Double)
      return constant( (double) con.UChar());
  }

  if (bt == basic_type::SShort) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.SShort());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.SShort());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.SShort());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.SShort());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.SShort());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.SShort());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.SShort());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.SShort());

    if (new_bt == basic_type::Float)
      return constant( (float) con.SShort());

    if (new_bt == basic_type::Double)
      return constant( (double) con.SShort());
  }

  if (bt == basic_type::UShort) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.UShort());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.UShort());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.UShort());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.UShort());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.UShort());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.UShort());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.UShort());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.UShort());

    if (new_bt == basic_type::Float)
      return constant( (float) con.UShort());

    if (new_bt == basic_type::Double)
      return constant( (double) con.UShort());
  }

  if (bt == basic_type::SInt) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.SInt());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.SInt());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.SInt());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.SInt());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.SInt());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.SInt());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.SInt());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.SInt());

    if (new_bt == basic_type::Float)
      return constant( (float) con.SInt());

    if (new_bt == basic_type::Double)
      return constant( (double) con.SInt());
  }

  if (bt == basic_type::UInt) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.UInt());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.UInt());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.UInt());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.UInt());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.UInt());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.UInt());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.UInt());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.UInt());

    if (new_bt == basic_type::Float)
      return constant( (float) con.UInt());

    if (new_bt == basic_type::Double)
      return constant( (double) con.UInt());
  }

  if (bt == basic_type::SLong) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.SLong());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.SLong());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.SLong());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.SLong());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.SLong());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.SLong());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.SLong());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.SLong());

    if (new_bt == basic_type::Float)
      return constant( (float) con.SLong());

    if (new_bt == basic_type::Double)
      return constant( (double) con.SLong());
  }

  if (bt == basic_type::ULong) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.ULong());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.ULong());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.ULong());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.ULong());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.ULong());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.ULong());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.ULong());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.ULong());

    if (new_bt == basic_type::Float)
      return constant( (float) con.ULong());

    if (new_bt == basic_type::Double)
      return constant( (double) con.ULong());
  }

  if (bt == basic_type::Float) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.Float());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.Float());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.Float());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.Float());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.Float());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.Float());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.Float());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.Float());

    if (new_bt == basic_type::Float)
      return constant( (float) con.Float());

    if (new_bt == basic_type::Double)
      return constant( (double) con.Float());
  }

  if (bt == basic_type::Double) {
    if (new_bt == basic_type::SChar)
      return constant( (signed char) con.Double());

    if (new_bt == basic_type::UChar)
      return constant( (unsigned char) con.Double());

    if (new_bt == basic_type::SShort)
      return constant( (signed short int) con.Double());

    if (new_bt == basic_type::UShort)
      return constant( (unsigned short int) con.Double());

    if (new_bt == basic_type::SInt)
      return constant( (signed int) con.Double());

    if (new_bt == basic_type::UInt)
      return constant( (unsigned int) con.Double());

    if (new_bt == basic_type::SLong)
      return constant( (signed long int) con.Double());

    if (new_bt == basic_type::ULong)
      return constant( (unsigned long int) con.Double());

    if (new_bt == basic_type::Float)
      return constant( (float) con.Double());

    if (new_bt == basic_type::Double)
      return constant( (double) con.Double());
  }

  return constant();
}
  

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

string constant::to_string(bool wantQuoteChar) const
{
  ostringstream ost;

  if (is_str()) {
    ost << "\"";
    const string & s = Str();
    int len = s.length();

    for (int i=0 ; i<len ; ++i) {
      int ch = s[i];
      print_char(s[i], ost);
    }

    ost << "\"";
  }

  else if (is_ptr())
    ost << Ptr();

  else if (_bt.is_char() && wantQuoteChar) {
    int value;

    ost << '\'';

    if (_bt == basic_type::SChar)
      value = _v.SChar;

    if (_bt == basic_type::UChar)
      value = _v.UChar;

    print_char(value, ost);

    ost << '\'';
  }

  else if (_bt == basic_type::SChar)
    ost << (int)_v.SChar;

  else if (_bt == basic_type::UChar)
    ost << (int)_v.UChar;

  else if (_bt == basic_type::SShort)
    ost << _v.SShort;

  else if (_bt == basic_type::UShort)
    ost << _v.UShort << "U";

  else if (_bt == basic_type::SInt)
    ost << _v.SInt;

  else if (_bt == basic_type::UInt)
    ost << _v.UInt << "U";

  else if (_bt == basic_type::SLong)
    ost << _v.SLong << "L";

  else if (_bt == basic_type::ULong)
    ost << _v.ULong << "UL";

  else if (_bt == basic_type::Float)
    ost << _v.Float;

  else if (_bt == basic_type::Double)
    ost << _v.Double;

  return ost.str();
}

// -- This function returns a printable representation of a char. It
// handles converting non-printable characters back into their
// printable representations.

void constant::print_char(int value, ostringstream & ost)
{
  switch(value) {
  case '\n': ost << "\\n"; break;
  case '\t': ost << "\\t"; break;
  case '\v': ost << "\\v"; break;
  case '\b': ost << "\\b"; break;
  case '\r': ost << "\\r"; break;
  case '\f': ost << "\\f"; break;
  case '\a': ost << "\\a"; break;
  case '\\': ost << "\\\\"; break;
  case '\?': ost << "\\\?"; break;
  case '\"': ost << "\\\""; break;
  case '\'': ost << "\\\'"; break;
  default:
    if (isprint(value)) {
      ost << (char) value;
    } else {
      char s[100];
      // djimenez - this wasn't converting to octal; now it is
      // djimenez - now it's converting to exactly 3-digit octal.
      // why?  consider "\0021" in an input program.  It means
      // ASCII 2 followed by the digit 1 (i.e., 0x31)
      // if we print "\21", we're saying ASCII 17.  f2c is
      // full of this type of thing in FORMAT stmts...
      sprintf (s, "%0.3o", value & 0xff);
      ost << "\\" << s;
    }
  }
}
