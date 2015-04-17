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
// optimize.cc
//
// An optimization phase including dead code elimination,
// constant propagation, constant folding and control flow
// simplification.  Soon to come: copy propagation.
//

#include <stdio.h>
#include "c_breeze.h"
#include "scope_walker.h"
#include "name_mangle_walker.h"
#include "goto_label_walker.h"
#include "dismantle.h"
#include "gc_walker.h"
#include "cfg.h"
#include "bits.h"
#include "reaching_genkill.h"
#include "reaching.h"
#include "constprop.h"
#include "optimize.h"
#include "defuse.h"
#include "live.h"
#include "dead.h"
#include "localcopyprop.h"

Node * CFS_Changer::at_proc (procNode *p, Order) {
	// after all ifs have been checked,
	// flatten (to remove unreachable code)
	// and put it back into a smaller control flow
	// graph
	if (changed) {
		UsedLabelWalker uw;
		p->walk (uw);
		FlattenDismantleChanger fc (uw.labels(), DEFAULT_DISMANTLER_FLAGS);
		p->change (fc);
		UsedLabelWalker uw1;
		p->walk (uw1);
		FlattenDismantleChanger fc1 (uw1.labels(), DEFAULT_DISMANTLER_FLAGS);
		p->change (fc1);
		cfg_changer cfgc;
		p->change (cfgc);
	}

	return p;
}

Node * CFS_Changer::at_if (ifNode *i, Order) {
	if (i->expr() && i->expr()->typ() == Const) {
		constNode *c = (constNode *) i->expr();

		if (c->value().Boolean()) {

			// if the constant is true, just return the
			// stmt; it's always executed.

			changed = true;
			return i->stmt();
		} else {

			// otherwise, return an empty stmt; 
			// i->stmt() is never executed

			changed = true;
			return new exprstmtNode (NULL);
		}
	}
	return i;
}

class CastRemover : public Changer {
public:
        CastRemover (void) : Changer (Preorder, Subtree, false) { }

        Node * at_cast (castNode *c, Order) {
	  if (c->is_implicit())
	    return c->expr();
	  else
	    return c;
        }

#ifdef FOO
	Node * at_binary (binaryNode *b, Order) {
		if (b->op()->id() == '=') {
			if (b->left()->typ() == Const) {
				return b->left();
			}
		}
		return b;
	}
#endif
};

Node * Optimizer::at_proc (procNode *p, Order) {
	fprintf (stderr, "%s: ", p->decl()->name().c_str());
	fflush (stderr);
	reachingDefinitionsWalker rdw;
	constantPropChanger cpc;
	constantFoldingChanger cfc;
	udChainRemover udr;
	livenessRemover lr;
	CFS_Changer cfsc;
	livenessWalker lw;
	deadCodeEliminationChanger dcc;
	unusedVariableCleanupChanger cc;
	LocalCopyPropChanger lcpc;
	CastRemover cr;

	int i = 0;

	// initially do constant folding; maybe the
	// program has constant folding opportunities
	// we can quickly exploit and maybe do fewer iterations
	// of the expensive stuff

	p->change (cfc);
	p->change (cr);
	do {
		cpc.changed = false;
		cfc.changed = false;

		// get ud-chain information for each use

		p->walk (rdw);

		// do constant propagation with ud-chain info

		fprintf (stderr, "C"); fflush (stdout);
		p->change (cpc);

		// do constant folding

		fprintf (stderr, "F"); fflush (stdout);
		p->change (cfc);
		p->change (cr);

		// do control flow simplification

		fprintf (stderr, "S"); fflush (stdout);
		p->change (cfsc);

		// local copy propagation
		// something's wrong with local
		// copyprop, I think.  
		// it's causing MPG123 to crash.
		fprintf (stderr, "P"); fflush (stdout);
		p->change (lcpc);

		// get liveness information for each basic block

		fprintf (stderr, "L"); fflush (stdout);
		p->walk (lw);

		// remove dead code

		fprintf (stderr, "D"); fflush (stdout);
		p->change (dcc);
		fprintf (stderr, "U"); fflush (stdout);
		p->change (cc);
		fprintf (stderr, "."); fflush (stdout);

		// don't blow away ud-chains or liveness info
		// on last iteration; someone else might want them.

#define MAX_ITER 4
		i++;

// I don't know why, but sometimes this doesn't converge, so we
// have to say arbitrarily "there will be no more than MAX_ITER iterations."

		if ((i < MAX_ITER) && 
			(cpc.changed || cfc.changed || cfsc.changed)) {
			p->walk (udr);
			p->walk (lr);
		}

// why don't we care whether dead code elimination changed something?
// because dead code couldn't generate any liveness (obviously) or
// any reaching definitions, and we don't care about any constants
// contained in dead code, so we couldn't improve anything if all we
// did was eliminate dead code.

	} while ((i < MAX_ITER) && 
		(cpc.changed || cfc.changed || cfsc.changed));
	fprintf (stderr, "\n");
	return p;
}

Node * Optimizer::at_unit (unitNode *u, Order) {
	cfg_changer::generate_cfg (u);
	return u;
}
