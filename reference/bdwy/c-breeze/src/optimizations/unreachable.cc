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
#include "unreachable.h"

void Unreachable::remove(procNode * proc)
{
  Unreachable un(proc);
  un.remove_unreachable();
}

void Unreachable::remove(unitNode * unit)
{
  for (def_list_p dp = unit->defs().begin();
       dp != unit->defs().end();
       ++dp)
    {
      defNode * dn = (*dp);
      if (dn->typ() == Proc) {
	procNode * proc = (procNode *) dn;
	Unreachable un(proc);
	un.remove_unreachable();
      }
    }
}

// --- Unreachable code elimination

void Unreachable::remove_unreachable()
{
  stmt_list & blocks = _proc->body()->stmts();

  basicblockNode * entry = _proc->entry();

  // Visit each basic block

  stmt_list_p block_p = blocks.begin();
  while (block_p != blocks.end()) {
    basicblockNode * cur = (basicblockNode *) * block_p;

    // A block should be removed if it has no predecessors, but is not
    // the entry node.

    if (cur->preds().empty() && (cur != entry)) {

      // Remove references from any successors (this is what was
      // causing the dominators algorithm to crash).

      basicblock_list & succs = cur->succs();
      for (basicblock_list_p succ_p = succs.begin();
	   succ_p != succs.end();
	   ++succ_p)
	{
	  basicblockNode * succ = *succ_p;
	  basicblock_list & preds = succ->preds();

	  basicblock_list_p q = find(preds.begin(), preds.end(), cur);
	  if (q != preds.end())
	    preds.erase(q);
	}

      // Remove the block itself

      stmt_list_p tmp = block_p;
      block_p++;
      blocks.erase(tmp);
      // delete cur;
    }
    else
      block_p++;
  }
}

