// $Id: memorymodel.cc,v 1.27 2003/09/04 18:27:08 toktb Exp $
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
#include "ref_clone_changer.h"
#include "memorymodel.h"
#include "memoryblock.h"

// ----------------------------------------------------------------------
// Create a new memory model
// ----------------------------------------------------------------------

memoryModel::memoryModel()
  : _memory_blocks(),
    _ci_memory_blocks(),
    _string_constants(),
    Counter(0),
    Null(0),
    _string_constant(0),
    _unification_based(0)
{
  // -- Create the special "null" object

  declNode * null_decl = new declNode("null",
				      declNode::NONE,
				      (typeNode *)0,
				      (exprNode *)0,
				      (exprNode *)0);

  Null = new memoryBlock(null_decl, true, (memoryBlock *)0, (procNode *)0);

  // -- Don't allow "null" to be modified

  Null->set_write_protected();

  pointerOptions::Unify_objects = 0; // TB_unify
}

memoryModel::~memoryModel()
{
  // Delete all the memory blocks

  for (proc_memorymodel_map_p q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      memorymodel_map & mm = (*q).second;

      for (memorymodel_map_p p = mm.begin();
	   p != mm.end();
	   ++p) {
        if(p->second->unifyType()) { // TB_unify
          set<procNode*> procs = p->second->unifyType()->procs();
          for(set<procNode*>::iterator q=procs.begin(); q!=procs.end(); q++)
            if(_unification_based->proctype(*q))
              _unification_based->proctype(*q)->block(NULL);
         }
	delete (*p).second;
      }
    }

  for (ci_memorymodel_map_p p = _ci_memory_blocks.begin();
       p != _ci_memory_blocks.end();
       ++p) {
    if(p->second->unifyType()) { // TB_unify
      for(set<procNode*>::iterator q=p->second->unifyType()->procs().begin();
          q!=p->second->unifyType()->procs().end(); q++) // TB_unify
        if(_unification_based->proctype(*q))
          _unification_based->proctype(*q)->block(NULL);
    }
    delete (*p).second;
  }

  // Delete the null object

  delete Null;
}

void memoryModel::clear()
{
  for (proc_memorymodel_map_p q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      memorymodel_map & mm = (*q).second;

      for (memorymodel_map_p p = mm.begin();
	   p != mm.end();
	   ++p)
	(*p).second->clear();
    }

  for (ci_memorymodel_map_p p = _ci_memory_blocks.begin();
       p != _ci_memory_blocks.end();
       ++p)
    (*p).second->clear();

  Null->clear();
}

int memoryModel::size()
{
  int size = 0;

  for (proc_memorymodel_map_p q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      memorymodel_map & mm = (*q).second;

      size += mm.size();
    }

  return size;
}

// ----------------------------------------------------------------------
//   Look up memoryBlocks
// ----------------------------------------------------------------------

memoryBlock * memoryModel::lookup_variable(Location * location,
					   declNode * decl,
					   procNode * local_to)
{
  // For local variables, we use the location as part of the index. For
  // global variables, set the location and local_to to NULL.

  procNode * owner = 0;
  Location * owner_loc = 0;
  bool is_array = false;
  bool is_struct_init = false;
  typeNode * ty = 0;

  if (decl) {

    // -- Local variables: make sure to exclude static local variables
    // which behave like globals.

    if (((decl->decl_location() == declNode::BLOCK) ||
	 (decl->decl_location() == declNode::FORMAL)) &&
	(decl->storage_class() != declNode::STATIC))
      {
	owner = local_to;
	owner_loc = location;
      }

    ty = decl->no_tdef_type();
    if (ty && (ty->typ() == Array) &&
	(decl->decl_location() != declNode::FORMAL))
      is_array = true;

    if (ty && (ty->typ() == Struct) && decl->init())
      is_struct_init = true;
  }

  // See if it's already there

  memoryBlock * result = 0;

  if (pointerOptions::Context_insensitive_memory)
    result = lookup((stmtNode *)0, decl);
  else
    result = lookup(owner_loc, decl);

  if (result)
    return result;

  // --- Not found; create a new one. For local variables and formal
  // parameters, record the owning procedure.

  if (pointerOptions::Context_insensitive_memory)
    result = create_memory_object((stmtNode *)0, decl, false, 
				  (memoryBlock *) 0, owner);
  else
    result = create_memory_object(owner_loc, decl, false, 
				  (memoryBlock *) 0, owner);

  // -- Handle unification

  if(pointerOptions::Unification || _unification_based)
    result->set_unification(pointerOptions::Non_unify_types);

  // -- Handle flow sensitivity

  result->set_flow_sensitivity(pointerOptions::Flow_sensitive_objects);

  // --- Special case: for locally declared arrays, we need to create
  // memory blocks that represent the elements. For now, we'll have one
  // memory block for each dimension in the array.

  if (is_array)
    generate_array_elements_for(result);

  if (is_struct_init) {
    expr_list inits;
    inits.push_back(decl->init());

    sueNode * sue = (sueNode *) decl->no_tdef_type();
    initialize_struct(result, sue, inits);
  }

  return result;
}

memoryBlock *  memoryModel::lookup_heap_object(const string & name,
					       stmtLocation * allocation_site,
					       stmtNode * allocation_stmt,
					       declNode * decl,
					       bool synthetic_decl)
{
  // -- Look up the block

  memoryBlock * result = 0;

  if (0) // pointerOptions::Context_insensitive_memory)
    result = lookup(allocation_stmt, decl);
  else
    result = lookup(allocation_site, decl);

  if (result)
    return result;

  /*
  // --- Not found; create a new one.

  declNode * the_decl = decl;

  if (synthetic_decl) {

    // -- Copy the synthetic declaration so there's no sharing. Otherwise,
    // we'll crash in the memoryBlock destructor.

    the_decl = new declNode(decl->name().c_str(),
			    declNode::NONE,
			    (typeNode *)0,
			    (exprNode *)0,
			    (exprNode *)0);
  }
  */

  if (0) // pointerOptions::Context_insensitive_memory)
    result = create_memory_object(allocation_stmt, decl, synthetic_decl, 0, 0,
                                  true);
  else
    result = create_memory_object(allocation_site, decl, synthetic_decl, 0, 0,
                                  0, true);

  // -- Flag the object as a heap object

  result->set_heap_object(allocation_site);

  // -- Force the name

  // result->set_name(name);

  // -- Handle unification

  if(pointerOptions::Unification || _unification_based)
    result->set_unification(pointerOptions::Non_unify_types);

  // -- Handle flow sensitivity: NOTE: make sure we do this *after* setting
  // the heap allocation flag above.

  result->set_flow_sensitivity(pointerOptions::Flow_sensitive_objects);

  return result;
}

/** @brief Look up string constant
 *
 * String constants are indexed by their constNode. */

memoryBlock * memoryModel::lookup_string_constant(constNode * the_string)
{
  memoryBlock * result = 0;

  if (pointerOptions::One_string_constant) {

    if ( ! _string_constant) {

      // -- Make the one string constant object
      string fullname("__string_constant");

      declNode * the_decl = new declNode(fullname.c_str(),
					 declNode::NONE,
					 (typeNode *)0,
					 (exprNode *)0,
					 (exprNode *)0);

      // -- Create a memoryBlock

      bool is_synthetic = true;

      _string_constant = new memoryBlock(the_decl, is_synthetic,
					 (memoryBlock *)0, (procNode *)0);
    
      // -- The contents of string constants are not modifiable

      _string_constant->set_write_protected();
      _string_constant->set_flow_insensitive();
    }

    result = _string_constant;
  }
  else {

    // -- Per-string constants: look them up by constNode

    string_constant_map_p p = _string_constants.find(the_string);
    if (p == _string_constants.end()) {

      // -- Not there, create a synthetic declaration for it

      char buf[50];
      
      sprintf(buf, "_%d", Counter);
      Counter++;

      string fullname = string("__string_#") + buf;
      
      declNode * the_decl = new declNode(fullname.c_str(),
					 declNode::NONE,
					 (typeNode *)0,
					 (exprNode *)0,
					 (exprNode *)0);
      
      // -- Create a memoryBlock
      
      bool is_synthetic = true;
      
      Unify_ECR *ecr = NULL;
      if(pointerOptions::Unification) {
        Alpha *alpha = _unification_based->string_alpha(the_string);
        if(alpha) {
          Unify_ECR *ecr = alpha->tao();
          if(ecr) result = ecr->type()->block();
        }
      }

      if(! result) {
        result = new memoryBlock(the_decl, is_synthetic,
                                 (memoryBlock *)0, (procNode *)0);
        if(ecr) {
          ecr->type()->block(result);
          result->unifyType(ecr->type());
        }
      }
      
      // -- Store it in the mapping
      
      _string_constants[the_string] = result;
      
      // -- The contents of string constants are not modifiable
      
      result->set_write_protected();
      result->set_flow_insensitive();
    }
    else {
      
      // -- It's there, so return it
      
      result = (*p).second;
    }
  }

  if (result == 0) {
    cout << "THIS IS BAD" << endl;
    abort();
  }

  return result;
}

memoryBlock * memoryModel::generate_su_field(const string & field_name,
					     declNode * field_decl,
					     memoryBlock * container)
{
  // -- Set up the name as the concatenation of the container name plus the
  // field name

  string field_obj_name;

  if (field_decl)
    field_obj_name = field_decl->name(); // container->generate_su_field_name(field_decl->name());
  else
    field_obj_name = field_name; // container->generate_su_field_name(field_name);

  // -- Copy the field declaration so that we get a unique field object for
  // each struct/union.

  typeNode * type = 0;
  if (field_decl)
    type = (typeNode *) ref_clone_changer::clone(field_decl->type(), false);
  
  declNode * copy_decl = new declNode(field_obj_name.c_str(),
				      declNode::NONE,
				      type,
				      (exprNode *)0,
				      (exprNode *)0);

  if(field_decl) _copy_decl[copy_decl] = field_decl; // TB_unify

  // -- Create the new object, with the container argument

  memoryBlock * field =  create_memory_object((Location *)0, copy_decl, true,
					      container, (procNode *)0,
                                              field_decl);

  // -- Handle unification

  if(pointerOptions::Unification || _unification_based)
    field->set_unification(pointerOptions::Non_unify_types);

  // -- Handle flow sensitivity

  field->set_flow_sensitivity(pointerOptions::Flow_sensitive_objects);

  // -- Handle fields that are arrays

  bool recursive_array = false;
  // if we're in unification mode and adaptation after 1st pass, and if the
  // container is not unified, it is possible that the field array's element is
  // actually the same as the container. If we now create new element, we would
  // end up with field[].field[].field[]....
  if(_unification_based && !pointerOptions::Unification &&
      !container->unifyType() ) {
    // check if the container is field[]. If so, do not create field[].field[].
    if(container->container() &&
       _copy_decl[container->container()->decl()] == field_decl)
      recursive_array = true;
  }

  if (field_decl &&
      field_decl->type() &&
      (field_decl->no_tdef_type()->typ() == Array) &&
      (! pointerOptions::Unification || /* TB */
       field_decl->decl_location() != declNode::SU) &&
      // are we in adaptation mode?
      (!(_unification_based && !pointerOptions::Unification) ||
       _unification_based->ecr(field_decl) ||
       !container->unifyType() && !recursive_array) ) {
    // explanation of above: proceed only if we're not running with
    // unification mode, or if we're in adaptation mode after 1st pass
    // (unify), there must be an ecr for the field (ie. field is in annotation,
    // or the container itself is not unified and no recursive array is
    // detected. Why: if proceed under the last conditions, there will be no
    // bound on the objects created e.g.  field[].field[].field[].....
if(! pointerOptions::Unification &&
   field_decl->decl_location() == declNode::SU &&
   container->unifyType()) { // debug
  cout << "field_decl @" << field_decl->coord() << endl;
  if(_unification_based->ecr(field_decl))
    cout << * _unification_based->ecr(field_decl) << endl;
  cout << field << " field " << field->name() << endl;
  if(field->unifyType()) cout << * field->unifyType() << endl;
  cout << container << " container " << container->name() << endl;
  if(container->unifyType()) cout << * container->unifyType() << endl;
  pointerOptions::Verbose = true;
}
    generate_array_elements_for(field);
if(! pointerOptions::Unification &&
   field_decl->decl_location() == declNode::SU &&
   container->unifyType()) { // debug
cout << "after generate_array_elemement\n";
exit(1);
}
  }

  return field;
}

// ----------------------------------------------------------------------
//  Private methods
// ----------------------------------------------------------------------

memoryBlock * memoryModel::create_memory_object(Location * location,
						declNode * decl,
						bool synthetic_decl,
						memoryBlock * container,
						procNode * local_to,
                                                declNode * org_field_decl,
                                                bool is_alloc,
                                                bool is_array_elt)
{
  procNode * owner = 0;

  if (!decl)
    cout << "WARNING: Creating memoryBlock with no declaration." << endl;

  // --- Inherit the owner from the container, when appropriate

  if (container)
    owner = container->local_to();
  else
    owner = local_to;

  // --- Create the memory block. 

  memoryBlock * result = NULL;
  bool brand_new = false;

  Unify_ECR *ecr = NULL;
  if(_unification_based) {
    if(is_alloc) {
      stmtNode *site = ((stmtLocation*)location)->stmt();
      if(site->typ() == ThreeAddr) {
        ecr = _unification_based->ecr((threeAddrNode*) site);
      } else {
        ecr = _unification_based->ecr(decl);
      }
      assert(ecr);
    } else {
      ecr = _unification_based->ecr(decl);
      if(!ecr) {
        bool field_from_annotation = false;
        if(org_field_decl &&
           _unification_based->isField(org_field_decl, field_from_annotation)) {
/*if (pointerOptions::Verbose) {
cout << "container = " << container << " " << container->name() << " field="
     << org_field_decl->name() << " @" << org_field_decl->coord();
if(container->unifyType()) cout << " " << *container->unifyType()->ecr();
cout << endl;
}*/
          ecr = _unification_based->ecrField(container->unifyType(),
                                             org_field_decl,
                                             field_from_annotation);
/*if (pointerOptions::Verbose)
if(container->unifyType())
cout << "ecrField, struct " << container->name() << " now: "
     << *container->unifyType()->ecr() << endl; */
        } else if(is_array_elt && container->unifyType()) {
          // note: container has no unifyType if itself was created by
          // annotation.
          assert(container);
/*if (pointerOptions::Verbose)
cout << "array elt for " << decl->name()
     << " b4: " << * container->unifyType()->ecr() << endl;*/
          ecr = _unification_based->ecrDeref(container->unifyType());
/*if (pointerOptions::Verbose)
cout << " now: " << * container->unifyType()->ecr() << endl;*/
        }
      }
    }

    if(ecr) {
      bool non_unify;
      if(decl &&
         (decl->decl_location()==declNode::PROC ||
          decl->no_tdef_type() &&
          (decl->no_tdef_type()->typ() == Func ||
           decl->no_tdef_type()->typ() == Ptr &&
           decl->no_tdef_type()->no_tdef_type()->typ()==Func)))
        non_unify = true;
      else if(!pointerOptions::Unification) {
        if(is_alloc) {
          assert(location->kind() == Location::Statement);
          non_unify = is_in_non_unify_list(pointerOptions::Non_unify_types,
                                           ecr->type());
        } else if(container)
          non_unify = is_in_non_unify_list(pointerOptions::Non_unify_types,
                                           ecr->type());
        else
          non_unify = is_in_non_unify_list(pointerOptions::Non_unify_types,
                                           ecr->type());
      } else non_unify = false;
      if(non_unify) ecr=NULL; // do not use
    }

    if(ecr) {
      if(ecr->type()->is_bottom())
        _unification_based->ensure_no_bottom(ecr, decl, container ?
                                                        container->unifyType() :
                                                        NULL);
      result = ecr->type()->block();
    }

/*else if (pointerOptions::Verbose) {
  cout << "no ecr for " << decl->name() << endl;
  if(container) {
    cout << "  container " << container << " " << container->name() << endl;
    if(container->unifyType())
      cout << "  container type " << * container->unifyType() << endl;
  }
  if(org_field_decl) {
    bool dummy;
    cout << "  is field @" << org_field_decl->coord() << " "
         << _unification_based->isField(org_field_decl, dummy)
         << endl;
  }
}*/
  }

  if(!result) {
    result = new memoryBlock(decl, synthetic_decl, container, owner);
    if(_unification_based &&
       (pointerOptions::Verbose || pointerOptions::Show_stack)) {
      cout << "new memoryBlock " << result << " for " << decl->name() << ": "
           << result->name() << endl;
      if (pointerOptions::Verbose)
        if(ecr) cout << "   ecr " << *ecr << endl;
    }
    brand_new = true;
    if(ecr) result->unifyType(ecr->type());
  }

  if(ecr && !ecr->type()->block())
    ecr->type()->block(result);

  // --- Add it to the model

  if(brand_new) {
    procNode * proc = 0;

    if (location)
      proc = Location::procedure(location)->proc();
    memorymodel_map & mm = _memory_blocks[proc];

    memorymodel_key key(location, decl);
    mm[key] = result;
  }

  return result;
}

// Context_insensitive_memory
memoryBlock * memoryModel::create_memory_object(stmtNode * location,
						declNode * decl,
						bool synthetic_decl,
						memoryBlock * container,
						procNode * local_to,
                                                bool is_alloc)
{
  procNode * owner = 0;

  if (!decl)
    cout << "WARNING: Creating memoryBlock with no declaration." << endl;

  // --- Inherit the owner from the container, when appropriate

  if (container)
    owner = container->local_to();
  else
    owner = local_to;

  // --- Create the memory block. 

  memoryBlock * result = NULL;
  bool brand_new = false;

  Unify_ECR *ecr = NULL;
  if(_unification_based) {
    ecr = _unification_based->ecr(decl);
    if(ecr && !pointerOptions::Unification) {
      bool non_unify;
      assert(! is_alloc);
      if(decl &&
         (decl->decl_location()==declNode::PROC ||
          decl->no_tdef_type() &&
          (decl->no_tdef_type()->typ() == Func ||
           decl->no_tdef_type()->typ() == Ptr &&
           decl->no_tdef_type()->no_tdef_type()->typ()==Func)))
        non_unify = true;
      else if(container)
        non_unify = is_in_non_unify_list(pointerOptions::Non_unify_types,
                                         ecr->type());
      else
        non_unify = is_in_non_unify_list(pointerOptions::Non_unify_types,
                                         ecr->type());
      if(non_unify) ecr=NULL; // do not use
    }

    if(ecr) result = ecr->type()->block();
  }

  if(!result) {
    result = new memoryBlock(decl, synthetic_decl, container, owner);
    brand_new = true;
    if(ecr) result->unifyType(ecr->type());
  }

  if(ecr && !ecr->type()->block())
    ecr->type()->block(result);

  // --- Add it to the model

  if(brand_new) {
    ci_memorymodel_key key(location, decl);
    _ci_memory_blocks[key] = result;
  }

  return result;
}

/** @brief Generate array elements
 *
 * For built-in arrays, we generate a single object for each dimension of
 * the array. For example,
 *
 * int A[3][4][5]
 *
 * Results in
 *
 *   A --> A_el_1 --> A_el_2 --> A_el_3
 *
 * That way A[1][3] evaluates to A_el_2, as does *(A[1]) and **A.
 *
 * This structure is represented by storing the element as a field of the
 * array object. We then add a special def that also causes the array to
 * point-to it's element. */

void memoryModel::generate_array_elements_for(memoryBlock * array_object)
{
  declNode * decl = array_object->decl();

  if (decl) {
    typeNode * curtype = array_object->decl()->no_tdef_type();
    memoryBlock * curblock = array_object;
    expr_list inits;

    // -- Handle initializers: this won't be perfect because it assumes
    // that the structure of the initializer matches the shape of the type
    // (which isn't strictly required by C semantics).

    if (decl->init() &&
	(decl->init()->typ() == Initializer))
      {
	initializerNode * initializer = (initializerNode *) decl->init();
	inits.push_back(initializer);
      }

    string suffix("[]");

    // -- Loop over all the dimensions of the array

    while (curtype && (curtype->typ() == Array)) {

      // -- Create a synthetic declNode to represent the array element (get
      // the type right).

      typeNode * type_clone = (typeNode *) ref_clone_changer::clone(curtype->type(), false);

      string name = decl->name() + suffix;

      declNode * fake_decl = new declNode(name.c_str(),
					  declNode::NONE,
					  type_clone,
					  (exprNode *)0,
					  (exprNode *)0);

      if(decl) _copy_decl[fake_decl] = decl; // TB_unify

      // -- Create a new memory object using the fake declaration. Set up
      // the new object as the array element of the block above it.

      memoryBlock * contents = create_memory_object((Location *)0, fake_decl, true,
						    curblock, (procNode *)0,
                                                    0, false, true);

      curblock->set_array_element(contents);

      // -- This object represents all elements:

      contents->set_indexed();

      // -- Handle unification

      if(pointerOptions::Unification || _unification_based)
        contents->set_unification(pointerOptions::Non_unify_types);

      // -- Handle flow sensitivity

      contents->set_flow_sensitivity(pointerOptions::Flow_sensitive_objects);

      // -- Move down in the initializer structure (probably not the most
      // efficient algorithm).

      expr_list new_inits;
      for (expr_list_p ip = inits.begin();
	   ip != inits.end();
	   ++ip)
	{
	  exprNode * expr = (*ip);
	  if (expr->typ() == Initializer) {
	    initializerNode * initializer = (initializerNode *) expr;
	    new_inits.insert(new_inits.end(),
			     initializer->exprs().begin(),
			     initializer->exprs().end());
	  }
	}

      inits = new_inits;
      
      // -- Go to the next nesting level
    
      curtype = curtype->no_tdef_type();
      curblock = contents;
      suffix = suffix + "[]";
    }

    if (curtype &&
	( ! inits.empty())) {

      if (pointerOptions::Verbose)
	cout << "FOUND initializers for " << array_object->name() << endl;

      if (curtype->typ() == Struct) {
	initialize_struct(curblock, (sueNode *) curtype, inits);
      }
      else {

	// -- Initialize an array
	
	for (expr_list_p p = inits.begin();
	     p != inits.end();
	     ++p)
	  {
	    exprNode * the_init = (*p);
	    idNode * id = 0;
            bool is_addr = false;
	    
	    if (the_init->typ() == Cast) // 20030903
              the_init = ((castNode*) the_init)->expr();

	    if (the_init->typ() == Id)
	      id = (idNode *) the_init;

	    if (the_init->typ() == Unary) {
	      unaryNode * un = (unaryNode *) the_init;
	      if (un->expr()->typ() == Id)
		id = (idNode *) un->expr();
              is_addr = (un->op()->id() == Operator::ADDRESS);
	    }

	    if (id) {

	      declNode * decl_init = id->decl();
	      
	      if (decl_init) {
		
                if(decl_init->type()->typ() == Func)
                  is_addr = true; //treat func as addr
		
		memoryBlock * rhs = lookup_variable((Location *)0,
						    decl_init,
						    (procNode *)0);
		if (pointerOptions::Verbose)
		  cout << "Array init: Assign " << curblock->name() << " = " << rhs->name() << endl;
		
		memoryblock_set temp;
                if(is_addr)
                  temp.insert(rhs);
                else { // TB new. import from assignment_operator.
                  memoryUse * right_use = rhs->current_use();
                  if ( ! right_use)
                    right_use = rhs->use_at(procLocation::main());
                  memoryDef * rd = right_use->reaching_def();
                  if (rd) {
                    const memoryblock_set & rd_points_to = rd->points_to();
                    temp.insert(rd_points_to.begin(), rd_points_to.end());
                  }

                  // TB_unify
                  if(pointerOptions::Unification && _unification_based) {
                    UnifyType *right_type = rhs->unifyType();
                    if(right_type &&
                       (right_type->objTyp()==SIMPLE || right_type->objTyp()==OBJECT)) {
                      Unify_ECR *defer = _unification_based->ecrDeref(right_type);
                      if(defer && defer->type()->block())
                        temp.insert( defer->type()->block() );
                    }
                  }
                }

		bool ignore;
		memoryDef *def = curblock->def_at(procLocation::main(), ignore);
		def->add_pointers(temp);
	      }
	    }
	  }
      }
    }
  }
}
 
// --- Lookup an object, returning null if one doesn't exist

memoryBlock * memoryModel::lookup(Location * location, declNode * decl)
{
  procNode * proc = 0;

  if (location)
    proc = Location::procedure(location)->proc();

  memorymodel_map & mm = _memory_blocks[proc];

  memorymodel_key key(location, decl);

  memorymodel_map_p p = mm.find(key);
  if (p != mm.end())
    return (*p).second;
  else
    return 0;
}

memoryBlock * memoryModel::lookup(stmtNode * location, declNode * decl)
{
  ci_memorymodel_key key(location, decl);

  ci_memorymodel_map_p p = _ci_memory_blocks.find(key);
  if (p != _ci_memory_blocks.end())
    return (*p).second;
  else
    return 0;
}

bool memoryModel::is_in_non_unify_list(UnifyTypes & non_unify_types,
                                       UnifyType *obj)
{
  assert(obj);
  UnifyTypes visited;
  return UnificationBasedPtr::reachable(obj, non_unify_types, visited);
}



// --- Update all the def-use chains at one time (only call this
// once pointer analysis is complete)

void memoryModel::update_def_use_chains()
{
  // First pass: fix the def-use chains

  for (proc_memorymodel_map_p q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      memorymodel_map & mm = (*q).second;

      for (memorymodel_map_p p = mm.begin();
	   p != mm.end();
	   ++p)
	((*p).second)->update_def_use_chains();
    }

  for (ci_memorymodel_map_p p = _ci_memory_blocks.begin();
       p != _ci_memory_blocks.end();
       ++p)
    ((*p).second)->update_def_use_chains();

   // Second pass: prune out unnecessary defs and uses
  /*
  for (memorymodel_map_p p = _memory_blocks.begin();
       p != _memory_blocks.end();
       ++p)
    ((*p).second)->prune_defs_uses();
  */
}

// --- Print some statistics

int num_defs_histogram[200];

void memoryModel::stats(ostream & out)
{
  out << "Memory model: " << endl;
  int block_count = _memory_blocks.size() + _ci_memory_blocks.size();

  out << "  memoryBlock : " << block_count << " objects x " 
      << sizeof(memoryBlock) << " bytes = " << block_count * sizeof(memoryBlock) 
      << " bytes. " << endl;

  long int global_defs = 0;
  long int global_merge_defs = 0;
  long int global_weak_defs = 0;
  long int global_uses = 0;
  long int global_merge_uses = 0;

  bool first = true;

  for (proc_memorymodel_map_p q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      memorymodel_map & mm = (*q).second;

      for (memorymodel_map_cp p = mm.begin();
	   p != mm.end();
	   ++p)
	{
	  memoryBlock * block = (*p).second;
	  if (first) {
	    block->stats(out, true,
			 global_defs, global_merge_defs, global_weak_defs,
			 global_uses, global_merge_uses);
	    first = false;
	  }
	  block->stats(out, false,
		       global_defs, global_merge_defs, global_weak_defs,
		       global_uses, global_merge_uses);
	}
    }

  out << "-- Total def/use statistics ---- " << endl;
  out << "  Total defs:       " << global_defs << endl;
  out << "  Total merge defs: " << global_merge_defs << endl;
  out << "  Total weak defs:  " << global_weak_defs << endl;
  out << "  Total uses:       " << global_uses << endl;
  out << "  Total merge uses: " << global_merge_uses << endl;

  double d_defs = (double) global_defs;
  double d_weak = (double) global_weak_defs;
  double percent_weak = (d_weak / d_defs) * 100.0;

  out << "  Percent weak defs: " << percent_weak << endl;
  out << "-------------------------------- " << endl;

  /*
  for (int i = 0; i < 200; i++) num_defs_histogram[i] = 0;

  for (memorymodel_map_cp p = _memory_blocks.begin();
       p != _memory_blocks.end();
       ++p)
    {
      int index = ((*p).second)->num_defs();
      if (index > 199) index = 199;
      num_defs_histogram[index]++;
    }

  cout << " Number of defs/memoryBlock" << endl;

  for (int i = 0; i < 200; i++) {
    if (i%10 == 0)
      printf("\n%2d    ", i);
    printf("%5d ", num_defs_histogram[i]);
  }
  */
}

void memoryModel::print(ostream & o) const
{
  for (proc_memorymodel_map_cp q = _memory_blocks.begin();
       q != _memory_blocks.end();
       ++q)
    {
      const memorymodel_map & mm = (*q).second;

      for (memorymodel_map_cp p = mm.begin();
	   p != mm.end();
	   ++p)
	((*p).second)->print_def_use(o);
    }

  for (ci_memorymodel_map_cp p = _ci_memory_blocks.begin();
       p != _ci_memory_blocks.end();
       ++p)
    ((*p).second)->print_def_use(o);
}

/** @brief Initialize struct
 *
 * Assign static initializers to a newly created struct. */

void memoryModel::initialize_struct(memoryBlock * struct_object,
				    sueNode * sue_type,
				    expr_list & inits)
{
  if (pointerOptions::Verbose)
    cout << "FOUND initializers for " << struct_object->name() << endl;

  suespecNode * spec = sue_type->spec();
  decl_list & fields = spec->fields();

  for (expr_list_p ip = inits.begin();
       ip != inits.end();
       ++ip)
    {
      exprNode * expr = (*ip);
      if (expr->typ() == Initializer) {
	initializerNode * initializer = (initializerNode *) expr;
	expr_list & field_inits = initializer->exprs();

	// -- Loop over the fields and the field inits together

	decl_list_p fp = fields.begin();
	expr_list_p fip = field_inits.begin();
	while ((fp != fields.end()) &&
	       (fip != field_inits.end()))
	  {
	    declNode * field_decl = (*fp);
	    exprNode * field_init = (*fip);

	    idNode * id = 0;
            bool is_addr = false;
	    
	    if (field_init->typ() == Cast) // 20030903
              field_init = ((castNode*) field_init)->expr();

	    if (field_init->typ() == Id)
	      id = (idNode *) field_init;

	    if (field_init->typ() == Unary) {
	      unaryNode * un = (unaryNode *) field_init;
	      if (un->expr()->typ() == Id)
		id = (idNode *) un->expr();
              is_addr = (un->op()->id() == Operator::ADDRESS);
	    }

	    if (id) {

	      declNode * decl_init = id->decl();
	      
	      if (decl_init) {

                if(decl_init->type()->typ() == Func)
                  is_addr = true; //treat func as addr
		
		memoryblock_set results;
		struct_object->dot(field_decl->name(),
				   field_decl,
				   *this,
				   results);
		memoryBlock * field = * (results.begin());
		
		memoryBlock * rhs = lookup_variable((Location *)0,
						    decl_init,
						    (procNode *)0);
		if (pointerOptions::Verbose)
		  cout << "Field init: Assign " << field->name() << " = " << rhs->name() << endl;
		
		memoryblock_set temp;
                if(is_addr)
                  temp.insert(rhs);
                else { // TB new. import from assignment_operator.
                  memoryUse * right_use = rhs->current_use();
                  if ( ! right_use)
                    right_use = rhs->use_at(procLocation::main());
                  memoryDef * rd = right_use->reaching_def();
                  if (rd) {
                    const memoryblock_set & rd_points_to = rd->points_to();
                    temp.insert(rd_points_to.begin(), rd_points_to.end());
                  }

                  // TB_unify
                  if(pointerOptions::Unification && _unification_based) {
                    UnifyType *right_type = rhs->unifyType();
                    if(right_type &&
                       (right_type->objTyp()==SIMPLE || right_type->objTyp()==OBJECT)) {
                      Unify_ECR *defer = _unification_based->ecrDeref(right_type);
                      if(defer && defer->type()->block())
                        temp.insert( defer->type()->block() );
                    }
                  }
                }

                bool ignore;
                memoryDef * def = field->def_at(procLocation::main(), ignore);
                def->add_pointers(temp);
	      }
	    }

	    fp++;
	    fip++;
	  }
      }
    }
}
