// $Id: operators.cc,v 1.6 2003/08/07 23:13:10 pnav Exp $
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

operator_map Operators::table;

void Operators::init()
{
  table[Operator::ARROW]   = new Operator(Operator::ARROW,       "->",      "ARROW",       0, 15);
  table['.']        = new Operator('.',            ".",       "DOT",         0, 15);

  table['!']        = new Operator('!',            "!",       "not",        14,  0);
  table['~']        = new Operator('~',            "~",       "bitnot",     14,  0);
  table[Operator::ICR]     = new Operator(Operator::ICR,         "++",      "ICR",        14,  0);
  table[Operator::POSTINC] = new Operator(Operator::POSTINC,     "++",      "postinc",    14,  0);
  table[Operator::PREINC]  = new Operator(Operator::PREINC,      "++",      "preinc",     14,  0);
  table[Operator::DECR]    = new Operator(Operator::DECR,        "--",      "DECR",       14,  0);
  table[Operator::POSTDEC] = new Operator(Operator::POSTDEC,     "--",      "postdec",    14,  0);
  table[Operator::PREDEC]  = new Operator(Operator::PREDEC,      "--",      "predec",     14,  0);

  table[Operator::SIZEOF]  = new Operator(Operator::SIZEOF,      "sizeof",  "sizeof",     14,  0);

  table[Operator::ADDRESS] = new Operator(Operator::ADDRESS,     "&",       "addrof",     14,  0);
  table[Operator::INDIR]   = new Operator(Operator::INDIR,       "*",       "indir",      14,  0);
  table[Operator::UPLUS]   = new Operator(Operator::UPLUS,       "+",       "UPLUS",      14,  0);
  table[Operator::UMINUS]  = new Operator(Operator::UMINUS,      "-",       "neg",        14,  0);
    
  table['*']        = new Operator('*',            "*",       "mul",         0, 13);
  table['/']        = new Operator('/',            "/",       "div",         0, 13);
  table['%']        = new Operator('%',            "%",       "mod",         0, 13);

  table['+']        = new Operator('+',            "+",       "add",         0, 12);
  table['-']        = new Operator('-',            "-",       "sub",         0, 12);
    
  table[Operator::LS]      = new Operator(Operator::LS,          "<<",      "lsh",         0, 11);
  table[Operator::RS]      = new Operator(Operator::RS,          ">>",      "rsh",         0, 11);
    
  table['<']        = new Operator('<',            "<",       "lt",          0, 10);
  table['>']        = new Operator('>',            ">",       "gt",          0, 10);
  table[Operator::LE]      = new Operator(Operator::LE,          "<=",      "le",          0, 10);
  table[Operator::GE]      = new Operator(Operator::GE,          ">=",      "ge",          0, 10);
    
  table[Operator::EQ]      = new Operator(Operator::EQ,          "==",      "eq",          0,  9);
  table[Operator::NE]      = new Operator(Operator::NE,          "!=",      "ne",          0,  9);
    
  table['&']        = new Operator('&',            "&",       "band",        0,  8);
    
  table['^']        = new Operator('^',            "^",       "bxor",        0,  7);
    
  table['|']        = new Operator('|',            "|",       "bor",         0,  6);
    
  table[Operator::ANDAND]  = new Operator(Operator::ANDAND,      "&&",      "and",         0,  5);
  table[Operator::OROR]    = new Operator(Operator::OROR,        "||",      "or",          0,  4);
    
  table['=']             = new Operator('=',            "=",       "asgn" ,  0,  2, false);
  table[Operator::MULTassign]   = new Operator(Operator::MULTassign,  "*=",      "*=",     0,  2, false);
  table[Operator::DIVassign]    = new Operator(Operator::DIVassign,   "/=",      "/=",     0,  2, false);
  table[Operator::MODassign]    = new Operator(Operator::MODassign,   "%=",      "%=",     0,  2, false);
  table[Operator::PLUSassign]   = new Operator(Operator::PLUSassign,  "+=",      "+=",     0,  2, false);
  table[Operator::MINUSassign]  = new Operator(Operator::MINUSassign, "-=",      "-=",     0,  2, false);
  table[Operator::LSassign]     = new Operator(Operator::LSassign,    "<<=",     "<<=",    0,  2, false);
  table[Operator::RSassign]     = new Operator(Operator::RSassign,    ">>=",     ">>=",    0,  2, false);
  table[Operator::ANDassign]    = new Operator(Operator::ANDassign,   "&=",      "&=",     0,  2, false);
  table[Operator::ERassign]     = new Operator(Operator::ERassign,    "^=",      "^=",     0,  2, false);
  table[Operator::ORassign]     = new Operator(Operator::ORassign,    "|=",      "|=",     0,  2, false);

  table[Operator::Index] = new Operator(Operator::Index, "[]", "index", 0, 14 );
  table[Operator::FUNC_CALL] = new Operator(Operator::FUNC_CALL, "()", "call", 0, 14);
}


bool Operator::is_assignment() const
{
  switch (_id) {
  case '=':
  case Operator::MULTassign:
  case Operator::DIVassign:
  case Operator::MODassign:
  case Operator::PLUSassign:
  case Operator::MINUSassign:
  case Operator::LSassign:
  case Operator::RSassign:
  case Operator::ANDassign:
  case Operator::ERassign:
  case Operator::ORassign:
    return true;
  default:
    return false;
  }
}

bool Operator::is_comparison() const
{
  switch (_id) {
  case Operator::EQ:
  case Operator::NE:
  case '<':
  case Operator::LE:
  case '>':
  case Operator::GE:
    return true;
  default:
    return false;
  }
}

bool Operator::is_unary() const
{
  switch ( _id ) {
  case Operator::SIZEOF:
  case Operator::UPLUS:
  case Operator::UMINUS:
  case Operator::INDIR:
  case Operator::ADDRESS:
  case Operator::POSTINC:
  case Operator::POSTDEC:
  case Operator::PREINC:
  case Operator::PREDEC:
  case '!':
  case '~':
    return true;
  default:
    return false;
  }
}

bool Operator::is_arithmetic() const
{
  switch (_id) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '|':
  case '&':
  case '^':
  case Operator::ANDAND:
  case Operator::OROR:
  case Operator::LS:
  case Operator::RS:
    return true;
  default:
    return false;
  }
}

bool Operator::is_binary() const
{
  switch (_id) {
  case Operator::ARROW:
  case '.':
  case '*':
  case '/':
  case '%':
  case '+':
  case '-':
  case Operator::LS:
  case Operator::RS:
  case '<':
  case '>':
  case Operator::LE:
  case Operator::GE:
  case Operator::EQ:
  case Operator::NE:
  case '&':
  case '^':
  case '|':
  case Operator::ANDAND:
  case Operator::OROR:
  case '=':
  case Operator::MULTassign:
  case Operator::DIVassign:
  case Operator::MODassign:
  case Operator::PLUSassign:
  case Operator::MINUSassign:
  case Operator::LSassign:
  case Operator::RSassign:
  case Operator::ANDassign:
  case Operator::ERassign:
  case Operator::ORassign:
  case Operator::Index:
    return true;
  default:
    return false;
  }
}

bool Operator::is_dismantled_binary() const {
  switch (_id) {
  case '*':
  case '/':
  case '%':
  case '+':
  case '-':
  case Operator::LS:
  case Operator::RS:
  case '<':
  case '>':
  case Operator::LE:
  case Operator::GE:
  case Operator::EQ:
  case Operator::NE:
  case '&':
  case '^':
  case '|':
    return true;
  default:
    return false;
  }
}
