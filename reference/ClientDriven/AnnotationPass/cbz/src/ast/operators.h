// $Id: operators.h,v 1.7 2003/08/07 23:13:10 pnav Exp $
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

#ifndef CBZ_OPERATORS_H
#define CBZ_OPERATORS_H

// The following values have been defined so that we can separate the
// representation of the AST from any particular aspect of the parser.

class Operator
{
public:

  typedef enum {SIZEOF = 300, // Make sure we are past regular chars
		UPLUS,
		UMINUS,
		INDIR,
		ADDRESS,
		POSTINC,
		POSTDEC,
		PREINC,
		PREDEC,
		ARROW,
		ICR,
		DECR,
		LS,
		RS,
		LE,
		GE,
		EQ,
		NE,
		ANDAND,
		OROR,
		MULTassign,
		DIVassign,
		MODassign,
		PLUSassign,
		MINUSassign,
		LSassign,
		RSassign,
		ANDassign,
		ERassign,
		ORassign,
		Index,
		FUNC_CALL
  } OperatorID;

private:

  unsigned int _id;
  int _unary_prec;
  int _binary_prec;
  bool _is_left_assoc;
  string _name;
  string _print;

public:

  Operator(unsigned int id, const char * print, const char * name, 
	   int unary_prec, int binary_prec,
	   bool is_left_assoc = true)
    : _id(id),
      _unary_prec(unary_prec),
      _binary_prec(binary_prec),
      _is_left_assoc(true),
      _name(string(name)),
      _print(string(print))
  {}

  inline unsigned int id() const { return _id; }
  inline int unary_prec() const { return _unary_prec; }
  inline int binary_prec() const { return _binary_prec; }
  inline bool is_left_assoc() const { return _is_left_assoc; }
  inline const string & name() const { return _name; }
  inline const string & print() const { return _print; }

  bool is_assignment() const;
  bool is_arithmetic() const;
  bool is_comparison() const;
  bool is_unary() const;
  bool is_binary() const;
  bool is_dismantled_binary() const;
};  

typedef map<unsigned int, Operator *, less< unsigned int > > operator_map;

class Operators
{
public:

  static operator_map table;

  static void init();
};


#endif // CBZ_OPERATORS_H
