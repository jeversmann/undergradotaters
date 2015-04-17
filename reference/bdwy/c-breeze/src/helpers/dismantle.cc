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
//
// dismantle.cc
//

#include <stdio.h>
#include "c_breeze.h"
#include "scope_walker.h"
#include "name_mangle_walker.h"
#include "goto_label_walker.h"
#include "dismantle.h"
#include "gc_walker.h"

#define IDEMPOTENCE_MODIFICATIONS 1

void Dismantle::dismantle (unitNode *u, unsigned int flags) {
	// dismantle (some) initializers

	InitializerDismantleChanger ic (flags);
	u->change (ic);

	// dismantle all loops

	LoopDismantleChanger lc (flags);
	u->change (lc);

	// convert if (a && b) into if (a) if (b)

	if (flags & CONVERT_LOGICALS_TO_IFS) {
		IfConverterChanger icc (flags);
		do {
			icc.change = false;
			u->change (icc);
		} while (icc.change);
	}

	// dismantle all selection stmts

	SelectionDismantleChanger sc (flags);
	u->change (sc);

	// dismantle all expressions	

	ExpressionDismantleChanger ec (flags);
	u->change (ec);

	// flatten the code out, putting declarations
	// at the top, removing redundant {}'s,
	// and removing unused labels

        // Below are the modifications done to implement idempotence
        // for the flattner

        // Below is an iteration based on a flag ,iterateflag.
        // the iteration happens as long as count is not 0 or
        // iterateflag is true
        // This iteration is being done to ensure that all empty bloocks
        // and useless labels and redundant gotos are removed
        // completely

        // The reason for iterating is that one flatten call causes a set of
        // block or dead code to be removed which produces opportunities to
        // do more dead code removal

        // So, we keep doing this until two consecutive runs donot produce any dead code
        
#ifdef IDEMPOTENCE_MODIFICATIONS
        
        int count = 2;
        
        while (true){
                UsedLabelWalker	uw1;
                u->walk (uw1);
                FlattenDismantleChanger	fc1 (uw1.labels(), flags);
                fc1.setflag(false);
                u->change (fc1);
                
                if (fc1 . getflag()){ // continue if dead code
                        fc1.setflag(false);
                        count = 2;
                        continue;
                        
                }
                else if (--count){ // continue for one more time
                        continue;
                        
                }
                else // at this point there is no useless label to be eliminated
                        break;
        }
#endif
                
	// fix up any name collisions

	name_mangle_walker::mangle(u);

	// fix up goto labels

	goto_label_walker::fixup(u);

 	// remove 'const' declarations
 	
 	RemoveConstChanger rmc;
 	u->change(rmc);

        // collect garbage

	gcWalker::collect();
}
