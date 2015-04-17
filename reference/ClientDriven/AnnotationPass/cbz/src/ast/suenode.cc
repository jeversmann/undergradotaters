// $Id: suenode.cc,v 1.5 2003/09/19 17:04:39 toktb Exp $
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

sueNode::sueNode(NodeType typ, const Coord coord)
  : typeNode(typ, NONE, 0, coord),
    _spec((suespecNode *)0),
    _elaborated(false)
{}

// ------------------------------------------------------------
// --- Was SetSUnionNameFields from sue.c
// ------------------------------------------------------------

void sueNode::set_name_fields(idNode *id, decl_list * fields, 
                              bool elaborated,
			      const Coord left_coord, 
			      const Coord the_right_coord)
{
  string a("");
  string & my_name = a;

  if (id)
    my_name = id->name();

  // -- If fields are specified, then this sue is elaborated..

  if (fields || elaborated)
    _elaborated = true;

  // -- Look up the tag...

  // If current declaration is elaborated, should lookup only in current scope,
  // so that shadowing declarations can be detected.

  suespecNode * sp = CBZ::current_unit->tags()->lookup(my_name, _elaborated);

  // -- If it's there, update the information.

  if (sp)
    sp->update(fields, this, the_right_coord);
  else {

    // -- Tag does not exist; create a new sue...

    sp = new suespecNode(my_name.c_str(), fields, typ(), 
			 CBZ::current_unit, left_coord);

    // -- If there is no name, create an anonymous sue...

    if (my_name.empty())
      my_name = CBZ::current_unit->tags()->insert_unique(string("___sue"), sp);
    else
      suespecNode * unused = CBZ::current_unit->tags()->insert(my_name, sp);
  }

  // -- Make sure the name is set...

  sp->name(my_name);
  spec(sp);

  //delete id;
}

sueNode * sueNode::set_name_fields_and(idNode *id, decl_list * fields, 
                                       bool elaborated,
				       const Coord left_coord,
				       const Coord the_right_coord)
{
  set_name_fields(id, fields, elaborated, left_coord, the_right_coord);
  return this;
}

// ------------------------------------------------------------
// -- Was SetSUnionName in sue.c
// ------------------------------------------------------------

void sueNode::set_name(idNode * id, const Coord the_coord)
{
  set_name_fields(id, (decl_list *)0, false, the_coord, Coord::Unknown);
  _elaborated = false;
}

sueNode * sueNode::set_name_and(idNode * id, const Coord the_coord)
{
  set_name(id, the_coord);
  return this;
}

// ------------------------------------------------------------
// -- Was ForceNewSU in sue.c
// ------------------------------------------------------------

void sueNode::force_new(const Coord the_coord)
{
  // this procedure handles the "recondite" rule that says that
  //        struct.or.union identifier ';'
  //     creates a create struct/union even if the tag is defined in an outer
  //     scope.  See K&R2, p213

  // assume that this tag is not already in the innermost scope,
  //     1) create a create struct/union
  //     2) add it to the current scope
  //     3) if there's a conflict, then the assumption was false
  //        and we use the previous version instead

  assert(spec() && !spec()->name().empty());
  suespecNode * sp = CBZ::current_unit->tags()->lookup(spec()->name(), true);
  assert(!sp || sp == spec());
  if(!sp) {

    // -- Tag does not exist in current scope; create a new sue...

    string my_name = spec()->name();
    sp = new suespecNode(my_name.c_str(), 0, typ(), 
			 CBZ::current_unit, the_coord);
    spec(sp);
    CBZ::current_unit->tags()->insert(my_name, sp);

    _elaborated = false;
  }

  if (spec()->name().empty())
    return;

  // ignore sue->fields

  // new_sue = (sueNode *) clone();
  // coord(the_coord);

  // sueNode * orig = CBZ::current_unit->tags()->insert(name(), new_sue);
  // if (orig && (orig != new_sue)) {
  //   orig != new_sue  implies conflict and memory leak of new_sue */
  //   This always happens! CBZ::Fail(__FILE__, __LINE__, "force_new failed");
  //   delete new_sue;
  // }
}

sueNode * sueNode::force_new_and(const Coord the_coord)
{
  force_new(the_coord);
  return this;
}

// ------------------------------------------------------------
//  Type Equal
// ------------------------------------------------------------

bool sueNode::qualified_equal_to(typeNode * node2,
				 bool strict_toplevel, bool strict_recursive)
{
  sueNode * n2 = (sueNode *) node2;

  // -- Normal case

  if (this == n2)
    return true;

  // -- Check the tags also in case we are still just scanning

  return spec()->same_tag_as(n2->spec());
}

// ------------------------------------------------------------
// Output
// ------------------------------------------------------------

void sueNode::output_type(output_context & ct, Node * parent, Assoc context, Type_qualifiers q)
{
  if (context == Left) {
    const string & qs = type_qualifiers_name(q);
    const string & qs1 = type_qualifiers_name();

    ct << qs;
    ct.space();
    ct << qs1;
    ct.space();

    switch (typ()) {
    case Struct:
      ct << "struct ";
      break;
    case Union:
      ct << "union ";
      break;
    case Enum:
      ct << "enum ";
      break;
    default:
      CBZ::Fail(__FILE__, __LINE__, "output_type(): Unrecognized type.");
    }

    ct << spec()->name();

    if (elaborated())
      spec()->output_type(ct, this, Left, NONE);
  }
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------

sueNode::~sueNode()
{
}
