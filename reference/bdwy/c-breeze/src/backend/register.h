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
//  Paul Navratil
//  Charles Nevill
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

#ifndef _REGISTER_H
#define _REGISTER_H

#include "constant.h"
#include "lir_var_type.h"

// types of registers
// when assigning registers, we need to know whether it is integer (gpr), 
// floating point, or address register (e.g. frame ptr, stack ptr).
// if your machine has another type of register, add it here.
// register type is updated through class Register, in LIR.cc
enum Reg_type { 

	reg_unknown,
	reg_gpr,
	reg_fpr,
	reg_stack_ptr,
	reg_frame_ptr
};

// Class Register encapsulates functions to be performed on registers.
// Methods are all called from LIR.cc
// Overloaded operators are used throughout the code generator to assign and increment reg#
// The accessors are used when you need to directly compare the type or reg#
// Instantiate class register whenever you need a register (source, destination, register
// allocator)
class Register
{
	friend class LirInst;

private:

	// the identifier of this register
	unsigned int _num;

	// the type of this register
	Reg_type _type;

public:

	// default constructor
	Register(Reg_type type = reg_unknown)
	{ 
		_num = (unsigned int)-1; 
		_type = type;
	}

	// set-everything constructor
	Register(unsigned int n, Reg_type rt)
	{
		_num = n; 
		_type = rt; 
	}

	// copy constructor
	Register (const Register &r)
	{
		// use assignment operator
		*this = r;
	}

	// accessors
	bool isValid() const		{ return (_type != reg_unknown); }
	unsigned int num() const	{ return _num; }
	void num(unsigned int n) 	{ _num = n; }
	Reg_type type() const		{ return _type;}
	void type( Reg_type t )		{ _type = t;}

	// methods to change the type
	Register& makeInvalid()		{ _type = reg_unknown; return *this; }
	Register& makeGPR()			{ _type = reg_gpr; return *this; }
	Register& makeFPR()			{ _type = reg_fpr; return *this; }
	Register& makeStackPtr() 	{ _type = reg_stack_ptr; _num = (unsigned int)-1; return *this; }
	Register& makeFramePtr() 	{ _type = reg_frame_ptr; _num = (unsigned int)-1; return *this; }

	// figure this thing's type out based on a given variable type
	void setType( lir_var_type t );

	// assignment operators
	Register& operator=(const Register &rightR)
	{ 
		_type = rightR._type; 
		_num = rightR._num; 
		return *this; 
	}
	Register& operator=(unsigned int val)
	{ 
		_num = val; 
		return *this;
	}

	// compare for equality
	bool operator==(const Register & rightR) const
	{ 
		if ((_num == rightR._num) && (_type == rightR._type))
			return true;
		return false;
	}

	// compare for inequality
	bool operator!=(const Register & rightR) const
	{ 
		return ((_num != rightR._num) || (_type != rightR._type));
	}

	// compare for less/greater, so we can put this in a set (uses RB tree)
	bool operator <(const Register & rightR) const
	{
		// just use numbers
		return _num < rightR._num;
	}

	// to stringize this thing
	// wantPrefixAdd : add extra prefixes specified in arch spec
	// wantPrefixRemove : remove extra prefixes specified in arch spec
	string to_string(bool wantPrefixAdd = true, bool wantPrefixRemove = false) const;

	// get special registers
	static Register getRegSp();
	static Register getRegFp();
	static Register getRegRetValFixed();
	static Register getRegRetValFloat();

	// find the right return value register for the given type, or return false if not possible
	static bool getRegRetVal( lir_var_type vt, Register & reg );

};

// a vector of registers
typedef vector<Register> register_vector;
typedef register_vector::iterator register_vector_p;

// a set of register ids
typedef hash_set_ex<unsigned int> reg_id_set;
typedef reg_id_set::iterator reg_id_set_p;

// a set of registers
typedef hash_set_ex<Register> reg_set;
typedef reg_set::iterator reg_set_p;

// a map from lir instruction to set of registers
typedef map<LirInst*, reg_id_set> inst_to_reg_id_map;
typedef inst_to_reg_id_map::iterator inst_to_reg_id_map_p;


// a register or a constant value
struct reg_or_const
{
	reg_or_const() : _is_const( false )
	{
	}

	reg_or_const( const constant & con )
	{
		// save this
		*this = con;
	}

	reg_or_const( const Register & reg )
	{
		// save this
		*this = reg;
	}

	// whether or not we are constant
	bool		_is_const;

	// our constant value, if we are a constant...
	constant	_const;

	// our register, if we are a register...
	Register	_reg;

	// to set us more easily
	reg_or_const & operator = ( const Register & reg )
	{
		_is_const = false;
		_reg = reg;
		return (*this);
	}
	reg_or_const & operator = ( const constant & con )
	{
		_is_const = true;
		_const = con;
		return (*this);
	}

	// to stringize this thing
	string to_string( bool quote_strings, bool addAsmConstPrefix = false ) const;

};

#endif
