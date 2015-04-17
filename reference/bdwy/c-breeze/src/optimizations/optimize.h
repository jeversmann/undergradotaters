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

#ifndef CBZ_OPTIMIZE_H
#define CBZ_OPTIMIZE_H

//
// optimize.h
//
// Optimization phase with several optimizations
//

/** @brief control flow simplification changer
    @ingroup Phases
*/
class CFS_Changer : public Changer {
public:
	bool changed;
	CFS_Changer (void) : 
		Changer (Postorder, Subtree, false),
		changed(false) { }

	Node * at_proc (procNode *, Order);
	Node * at_if (ifNode *, Order);
};

/** @brief Changer that performs several optimizations

    An optimization phase including dead code elimination,
    constant propagation, constant folding and control flow
    simplification.  Soon to come: copy propagation.
*/
class Optimizer : public Changer {
public:

	Optimizer (void) : Changer (Preorder, Subtree, false) { }

	Node * at_proc (procNode *, Order);
	Node * at_unit (unitNode *, Order);
};

#endif // CBZ_OPTIMIZE_H
