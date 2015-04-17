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

// --------------------------------------------------------------------
// Constructors
// --------------------------------------------------------------------

suespecNode::suespecNode(const char * name, decl_list * fields,
			 NodeType owner,
			 unitNode * u,
			 const Coord coord)
  : typeNode(sueSpec, NONE, 0, coord),
    _name(string(name)),
    _fields(),
    _complete(false),
    _visited(false),
    _size(0),
    _align(1),
    _owner(owner),
    _scope_output(0)
{
  if (fields) {
    _fields.swap(*fields);
    _complete = true;
    //delete fields;
  }
  // -- Keep all suespecNodes in a global list, since none of
  // the struct/union/enum nodes actually "own" them.

  u->suespecs().push_back(this);
}

// --------------------------------------------------------------------
// Tag operations
// --------------------------------------------------------------------

bool suespecNode::same_tag_as(suespecNode * other)
{
  if (name().empty() || other->name().empty())
    return false;
  else
    return name() == other->name();
}

// --- tag_conflict from sue.c
// What this function does now is update the suespecNode whenever a new
// sue-specifier is encountered.

void suespecNode::update(decl_list * fields, sueNode * sue, const Coord right)
{
  if (sue->typ() != owner()) {
    CBZ::SyntaxError(sue->coord(),
                     string("redeclaration of structure/union/enum tag `") +
                     name() + string("'"));
    cerr << "\tPrevious declaration: " << coord() << endl;

    return;
  }

  if (fields)
    if (complete()) {

      // -- Error: fields already specified...

      string tp;
      switch (sue->typ()) {
      case Struct:
	tp = string("structure");
	break;
      case Union:
	tp = string("union");
	break;
      case Enum:
	tp = string("enum");
	break;
      default:
	break;
      }

      CBZ::SyntaxError(sue->coord(),
		       string("multiple declarations of ") +
		       tp + string(" `") +
		       name() + string("'"));
      cerr << "\tPrevious definition: " << coord() << endl;
    }
    else {

      // -- Update the suespec with the fields...

      _fields.swap(*fields);
      //delete fields;
      complete(true);
      coord(right);
    }
}

/** @brief Find a field by name
 *
 *  Search the _fields list for the given name, returning the
 *  declaration, if found, or null if not found. (Was SUE_FindField in
 *  sue.c).
 *
 *  @param name the field name to look for
 *  @return the declaration, if found, or null */

declNode * suespecNode::find_field(const string & name)
{
  for (decl_list_p p = fields().begin();
       p != fields().end();
       ++p)
    {
      if ((*p)->name() == name)
	return (*p);
    }

  return 0;
}


// ------------------------------------------------------------
//  Walker
// ------------------------------------------------------------

void suespecNode::visit(Visitor * the_visitor) 
{
  the_visitor->at_suespec(this);
}

void suespecNode::walk(Walker & the_walker)
{
  Walker::Order ord = the_walker.order(); 

  if (ord == Walker::Preorder || ord == Walker::Both)
    the_walker.at_suespec(this, Walker::Preorder);

  if (the_walker.depth() == Walker::Subtree) {
    // -- Visit the children 

    list_walker(fields(), the_walker);
  }

  if (ord == Walker::Postorder || ord == Walker::Both)
    the_walker.at_suespec(this, Walker::Postorder);
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void suespecNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (owner() == Enum) {
    ct.space();
    if (!ct.isPrinted(this)) {
	ct << '{';
	ct.space();
	output_delim_list(fields(), ct, this, ',');
	ct.space();
	ct << '}';
	ct.printed(this);
        scope_output(CBZ::current_unit->symbol_level());
    }
  }
  else {
    // djimenez
    // if this body has already been output, don't output
    // it again; many compilers will reject an identical definition
    // as a redefinition

    if (!ct.isPrinted(this)) {
	ct.new_line();
	ct << '{';
	ct.indent_in();
	output_list(fields(), ct, this);
	ct.indent_out();
	ct.new_line();
	ct << '}';
	ct.printed(this);
        scope_output(CBZ::current_unit->symbol_level());
    }
  }
}

// ------------------------------------------------------------
//  Changer
// ------------------------------------------------------------

Node * suespecNode::change(Changer & the_changer, bool redispatch)
{
  Changer::Order ord = the_changer.order(); 
  suespecNode * the_suespec = this;

  if ((ord == Changer::Preorder || ord == Changer::Both) && ! redispatch)
    the_suespec = (suespecNode *) the_changer.at_suespec(the_suespec, Changer::Preorder);

  if (the_suespec) {

    if (the_suespec != this)
      return the_suespec->change(the_changer, true);

    change_list(the_suespec->fields(), the_changer);

  }

  if ((ord == Changer::Postorder || ord == Changer::Both) && ! redispatch)
    the_suespec = (suespecNode *) the_changer.at_suespec(the_suespec, Changer::Postorder);

  return the_suespec;
}


// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

suespecNode::~suespecNode()
{
  //delete_list(_fields);
}
