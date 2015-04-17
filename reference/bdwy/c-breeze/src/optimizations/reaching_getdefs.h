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

#ifndef CBZ_REACHING_GETDEFS_H
#define CBZ_REACHING_GETDEFS_H

//
// reaching_getdefs.h
//
// This file contains definitions for several
// classes 
/** @brief creates list of defines in a unit.
    @ingroup Phases
*/
class GetDefsWalker : public Walker {
private:

	// passed in

	map <stmtNode *, declNode *> * defines;

	// list of ambiguous definitions, passed in

	stmt_list * ambiguous_defs;

public:
	GetDefsWalker (map <stmtNode *, declNode *> *d, stmt_list * l) : 
		Walker (Preorder, Subtree),
		ambiguous_defs(l),
		defines(d) { }

	bool contains_call (exprNode *);

	void handle_exprstmt (exprstmtNode *);

	void at_stmt (stmtNode *, Order);
};

#endif // CBZ_REACHING_GETDEFS_H
