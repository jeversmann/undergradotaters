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

#ifndef CBZ_CONSTPROP_H
#define CBZ_CONSTPROP_H

/** @brief performs global (intraprocedural) constant propagation.
    @ingroup Phases
*/
class constantPropChanger : public Changer {
public:
	bool changed;

	constantPropChanger (void) : 
		Changer (Preorder, Subtree, false),
		changed (false) { }

	Node *at_id (idNode *, Order);
};

/** @brief Folds constants in binary expressions
    @ingroup Phases
*/
class constantFoldingChanger : public Changer {
public:
	bool changed;

	constantFoldingChanger (void) : 
		Changer (Preorder, Subtree, false),
		changed (false) { }

	Node * at_binary (binaryNode *, Order);
	Node * at_unary (unaryNode *, Order);
};

#endif // CBZ_CONSTPROP_H