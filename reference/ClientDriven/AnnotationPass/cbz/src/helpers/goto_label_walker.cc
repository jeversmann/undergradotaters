// $Id: goto_label_walker.cc,v 1.4 2003/08/07 23:13:44 pnav Exp $
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
#include "goto_label_walker.h"

// ------------------------------------------------------------
// Entry points
// ------------------------------------------------------------

void goto_label_walker::fixup(procNode * proc)
{
  goto_label_walker g;

  // -- Two passes: the first collects the labels...

  proc->walk(g);
  g._have_labels = true;

  // -- The second fixes the gotos...

  proc->walk(g);

  // -- Report any unused symbols...

  g._labels.reset();
}

void goto_label_walker::fixup(unitNode * prog)
{
  def_list & ds = prog->defs();

  for (def_list_p p = ds.begin(); p != ds.end(); ++p)
    if ((*p)->typ() == Proc)
      fixup((procNode *)(*p));
}

// ------------------------------------------------------------
// Handle labels
// On the first pass, at a label we insert the label in the
// table.
// ------------------------------------------------------------

void goto_label_walker::at_label(labelNode * the_label, Order ord)
{
  if (! _have_labels) {

    // -- Remove any old goto references...

    the_label->references().clear();

    // -- Insert the label into the symbol table...

    labelNode * orig = _labels.insert(the_label->name(), the_label);

    if (orig) {
      CBZ::SyntaxError(the_label->coord(),
		       string("multiple definitions of label `") +
		       the_label->name() + string("'"));
      cerr << "\tPrevious definition: " << orig->coord() << endl;
    }
  }
}

// ------------------------------------------------------------
// Handle gotos
// On the second pass, we lookup the gotos and point them at
// their labels.
// ------------------------------------------------------------

void goto_label_walker::at_goto(gotoNode * the_goto, Order ord)
{
  if (_have_labels) {
    
    // -- Try to find a matching label

    labelNode * the_label = _labels.lookup(the_goto->name());

    if (! the_label)
      CBZ::SyntaxError(the_goto->coord(),
		       string("undefined label `") + 
		       the_goto->name() + string("'"));
    else {

      // -- Set the label field and add this "goto" to it's references

      the_goto->label(the_label);
      the_label->references().push_back(the_goto);
      the_label->references().sort();
      the_label->references().unique();
    }
  }
}
