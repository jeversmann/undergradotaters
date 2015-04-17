
#include "broadway.h"

// ----------------------------------------------------------------------
//  Property analyzer
// ----------------------------------------------------------------------

propertyAnalyzer::propertyAnalyzer(Annotations * annotations, 
				   constantAnalyzer & constants)
  : analysisProblem(Forward),
    _annotations(annotations),
    _constants(constants),
    _set_property_variables()
{}

/** @brief Is set property variable block?
 *
 * Check to see if this is special memory block used to represent the
 * state of a set property. */

setPropertyAnn * propertyAnalyzer::is_set_property_variable_block(memoryBlock * block)
{
  set_property_variable_map_p p = _set_property_variables.find(block);

  if (p != _set_property_variables.end())
    return (*p).second;
  else
    return 0;
}

/** @brief Is enum property variable block?
 *
 * Check to see if this is special memory block used to represent the
 * state of a enum property. */

enumPropertyAnn * propertyAnalyzer::is_enum_property_variable_block(memoryBlock * block)
{
  return block->property;
}

void propertyAnalyzer::initialize()
{
  // -- Build the list of property variable blocks

  _set_property_variables.clear();

  const set_property_map & set_properties = _annotations->set_properties();

  for (set_property_map_cp p = set_properties.begin();
       p != set_properties.end();
       ++p)
    {
      setPropertyAnn * set_property = (*p).second;

      // -- Set up a mapping from the block to the property it represents

      memoryBlock * block = set_property->property_variable_block();
      
      _set_property_variables[block] = set_property;
    }
}

void propertyAnalyzer::compute_all_constants(procLocation * where,
					     procedureAnn * procedure)
{
  // -- Visit all the idNodeAnn objects, computing their value in the
  // current context.

  idnodeann_list & ids = procedure->numeric_identifiers();

  for (idnodeann_list_p p = ids.begin();
       p != ids.end();
       ++p)
    {
      idNodeAnn * id = *p;

      // -- Get the annotation variable

      annVariable * var = id->variable();

      // -- Look up the constant value

      const constant * val = lookup_constant(var, where);

      // -- Store it on the idNodeAnn

      id->value(*val);
    }
}

const constant * propertyAnalyzer::lookup_constant(annVariable * var,
						   procLocation * where)
{
  // -- Lookup the current binding

  pointerValue & binding = var->get_binding();

  const constant * val = 0;

  if (var->is_formal()) {

    // -- Formal parameters already have the constant value set

    val = binding.constant_value;
  }
  else {

    // -- For other variables, we need to reconstruct the value

    val = _constants.rebuild_flowvalue(binding);
  }

  if (val)
    return val;
  else
    return _constants.bottom();
}
 
void propertyAnalyzer::analyze_callsite(procLocation * where,
					procedureAnn * procedure,
					memoryblock_set & changes,
					pointerValue & return_val)
{
  // -- Make sure all the constant values are pre-computed and stored on
  // the idNodeAnn objects.

  compute_all_constants(where, procedure);

  // -- Apply the various analyze annotations: test the conditions and
  // update the results.

  // -- First pass: test all the conditions to determine which rule fires

  const analyze_map analyses = procedure->analyses();

  if (Annotations::Verbose_properties) {
    cout << endl;
    cout << "[1] Test all conditions..." << endl;
  }

  for (analyze_map_cp p = analyses.begin();
       p != analyses.end();
       ++p)
    {
      analyzeAnn * analyze = (*p).second;
      analyze->test(where, this);
    }

  // -- Second pass: for the selected rule, compute the new states

  if (Annotations::Verbose_properties) {
    cout << endl;
    cout << "[2] Compute the next states..." << endl;
  }

  for (analyze_map_cp p = analyses.begin();
       p != analyses.end();
       ++p)
    {
      analyzeAnn * analyze = (*p).second;
      analyze->compute_next(where, this);
    }

  // -- Third pass: apply the new states, record if anything changes

  if (Annotations::Verbose_properties) {
    cout << endl;
    cout << "[3] Apply the next states..." << endl;
  }

  for (analyze_map_cp p = analyses.begin();
       p != analyses.end();
       ++p)
    {
      analyzeAnn * analyze = (*p).second;
      analyze->apply_next(where, this, changes);
    }

  // -- Transfer updates to the return value to the return_val object

  update_return_value(where, procedure, return_val);
}

void propertyAnalyzer::update_constant(pointerValue & variable,
				       constant & value,
				       memoryblock_set & changes)
{
  const constant * right_c = _constants.lookup(value); // _flowvalue(def);

  // -- Store the value on the LHS pointerValue; this is critical to the
  // return value of library routines. This is how assignment works in the
  // constants class.

  variable.constant_value = right_c;

  // -- For all blocks

  for (memoryblock_set_p p = variable.blocks.begin();
       p != variable.blocks.end();
       ++p)
    {
      memoryBlock * block = *p;

      // -- Get the current value at this location

      memoryDef * def = block->current_def();

      // -- Compute the new value

      const constant * newval = _constants.top();

      if (def->is_weak()) {
	// memoryDef * previous_def = def->previous();

	memoryUse * weak_use = block->current_use();
	memoryDef * previous_def = weak_use->reaching_def();

	if (previous_def) {
	  const constant * prev_val = _constants.lookup_flowvalue(previous_def);
	  newval = _constants.meet(right_c, prev_val);
	}
	else
	  newval = right_c;
      }
      else
	newval = right_c;

      bool change = _constants.update_flowvalue(newval, def);
      
      if (change)
	changes.insert(block);

      // Not needed?...
      // set_flowvalue(right_c, left);
    }
}

/** @brief Property assignment
 *
 * This method handles both regular statement-level assignments and
 * parameter passing. */

void propertyAnalyzer::property_assignment(Location * current,
					   stmtLocation * parameter_callsite,
					   pointerValue & left,
					   pointerValue & right,
					   bool is_parameter,
					   memoryblock_set & changes)
{
  // -- Special case: if the right-hand side is just an address, then
  // there's nothing to do.

  if (right.is_address)
    return;

  // -- Compile a list of the properties that are being assigned. The nice
  // thing about this approach is that we'll only assign those properties
  // that are actually present on the right-hand side.

  enum_property_set assigned_properties;

  // -- Figure out which properties are present

  for (memoryblock_set_p p = right.blocks.begin();
       p != right.blocks.end();
       ++p)
    {
      memoryBlock * right = *p;

      // -- For each possible property block

      for (memoryblock_vector_p q = right->property_blocks.begin();
	   q != right->property_blocks.end();
	   ++q)
	{
	  memoryBlock * property_block = *q;

	  // -- The property block could be null if the given object never
	  // has this property.

	  if (property_block) {

	    // -- Record the fact that this property is present on the
	    // right-hand side

	    enumPropertyAnn * the_property = property_block->property;

	    assigned_properties.insert(the_property);
	  }
	}
    }

  for (enum_property_set_p p = assigned_properties.begin();
       p != assigned_properties.end();
       ++p)
    {
      enumPropertyAnn * property = (*p);

      if (property->is_enabled() &&
	  (property != _annotations->constants_property()) &&
	  (property->direction() == direction())) { 

	if (Annotations::Verbose_properties) {
	  cout << endl;
	  cout << "  - Assign property " << property->name() << endl;
	}

	// -- Apply the assignment, and record any changes in the defs
	// set. NOTE: this method handles weak updates.

	property->apply_assignment(current, parameter_callsite, left, right, is_parameter, changes);
      }
    }
}


void propertyAnalyzer::update_return_value(procLocation * where,
					   procedureAnn * procedure,
					   pointerValue & return_val)
{
  // -- Get the return value variable

  annVariable * ret = procedure->lookup("return", false);

  // -- Look up the binding 

  pointerValue & ret_bind = ret->get_binding();

  // -- Get the constant value

  const constant * ret_val = ret_bind.constant_value;

  // -- Transfer the value to the return_val that goes back to the calling
  // procedure

  if (ret_val)
    return_val.constant_value = ret_val;
  else
    return_val.constant_value = _constants.bottom();

  if (Annotations::Verbose_properties) {
    if (ret_val)
      cout << "  propertyAnalyzer: Return value is " << ret_val->to_string() << endl;
  }   
}

// ------------------------------------------------------------
//  Transfer functions
// ------------------------------------------------------------

  // -- Non-pointer expressions

void propertyAnalyzer::at_id(stmtLocation * current, idNode * id,
			     pointerValue & value)
{}

void propertyAnalyzer::at_unary(stmtLocation * current,
				unaryNode * unary,
				pointerValue & operand,
				pointerValue & result)
{}

void propertyAnalyzer::at_binary(stmtLocation * current,
				 binaryNode * binary,
				 pointerValue & left,
				 pointerValue & right,
				 pointerValue & result)
{}

void propertyAnalyzer::at_cast(stmtLocation * current, castNode * cast,
			       pointerValue & operand,
			       pointerValue & result)
{}

void propertyAnalyzer::at_const(stmtLocation * current, constNode * cons,
				pointerValue & result)
{}

  // -- Pointer expressions

void propertyAnalyzer::at_field_access(stmtLocation * current,
				       binaryNode * binary,
				       pointerValue & operand,
				       idNode * field,
				       pointerValue & result)
{}

void propertyAnalyzer::at_dereference(stmtLocation * current,
				      unaryNode * unary,
				      pointerValue & operand,
				      pointerValue & result)
{}

void propertyAnalyzer::at_address(stmtLocation * current,
				  unaryNode * unary,
				  pointerValue & operand,
				  pointerValue & result)
{}

void propertyAnalyzer::at_index(stmtLocation * current,
				binaryNode * binary,
				pointerValue & left,
				pointerValue & right,
				pointerValue & result)
{}

  // -- Assignments

void propertyAnalyzer::at_assignment(stmtLocation * current,
				     binaryNode * binary,
				     pointerValue & left,
				     pointerValue & right,
				     pointerValue & result,
				     memoryblock_set & changes)
{
  // -- Assign the property objects

  property_assignment(current, (stmtLocation *)0, left, right, false, changes);
}

void propertyAnalyzer::at_parameter_pass(Location * current,
					 stmtLocation * parameter_callsite,
					 pointerValue & left,
					 pointerValue & right,
					 memoryblock_set & changes)
{
  // -- Assign the property objects

  property_assignment(current, parameter_callsite, left, right, true, changes);
}

void propertyAnalyzer::at_self_assignment(Location * source,
					  Location * target,
					  memoryBlock * block,
					  memoryblock_set & changes)
{
  // -- Case 1: Is this the memory block that represents a set property?

  if (block) {
    setPropertyAnn * set_property = is_set_property_variable_block(block);
    if (set_property) {

      if (set_property->is_enabled() &&
	  (set_property->direction() == direction())) { 
      
	if (Annotations::Verbose_properties)
	  cout << "  - Pass property " << set_property->name() << endl;

	set_property->self_assignment(changes);
      }
      
      return;
    }
  }

  // -- Case 2: Is this a property block for an enum property?

  enumPropertyAnn * enum_property = is_enum_property_variable_block(block);
  if (enum_property) {

    memoryBlock * property_block = block;

    if (enum_property->is_enabled() &&
	(enum_property->direction() == direction())) { 
      
      if (Annotations::Verbose_properties)
	cout << "  - Pass property " << enum_property->name() << endl;

      enum_property->self_assignment(source, target, property_block, changes);
    }

    return;
  }
}

  // -- Statement types

void propertyAnalyzer::at_return(stmtLocation * stmt,
				 returnNode * ret,
				 pointerValue & result,
				 pointerValue & return_val)
{}

  // -- Process a merge point

void propertyAnalyzer::at_merge(Location * where,
				memoryBlock * block,
				memoryuse_list & phi_uses,
				pointerValue & result,
				memoryblock_set & changes)
{
  // -- Special case: Is this the memory block that represents a set
  // property?

  setPropertyAnn * set_property = is_set_property_variable_block(block);
  if (set_property) {

    if (set_property->is_enabled() &&
	(set_property->direction() == direction())) { 
      
      if (Annotations::Verbose_properties)
	cout << "  - Merge property " << set_property->name() << endl;

      set_property->apply_merge(phi_uses, changes);
    }

    return;
  }

  // -- Special case: Is this a property block for an enum property?

  enumPropertyAnn * enum_property = is_enum_property_variable_block(block);
  if (enum_property) {

    memoryBlock * property_block = block;

    if (enum_property->is_enabled() &&
	(enum_property->direction() == direction())) { 
      
      if (Annotations::Verbose_properties)
	cout << "  - Merge property " << enum_property->name() << endl;

      enum_property->apply_merge(where, property_block, phi_uses, changes);
    }

    return;
  }
}

  // -- Procedure boundaries

void propertyAnalyzer::at_conservative_procedure_call(stmtLocation * current,
						      callNode * call,
						      pointerValue & call_target,
						      pointervalue_list & arguments,
						      memoryblock_set & reachable,
						      pointerValue & return_val,
						      memoryblock_set & changes)
{
  /*
  cout << "CONSERVATIVE call at " << * current << endl;
  output_context oc(cout);
  call->output(oc, 0);
  cout << endl;

  for (memoryblock_set_p p = reachable_blocks.begin();
       p != reachable_blocks.end();
       ++p)
    {
      memoryBlock * block = (*p);
      cout << "  + Conservative def of " << block->name() << " at " << *current << endl;
    }
  */

  // -- Conservative procedure call works like assignment, but forces
  // everything to bottom

  enum_property_set assigned_properties;

  // -- Figure out which properties are present

  for (memoryblock_set_p p = reachable.begin();
       p != reachable.end();
       ++p)
    {
      memoryBlock * block = *p;

      // -- For each possible property block

      for (memoryblock_vector_p q = block->property_blocks.begin();
	   q != block->property_blocks.end();
	   ++q)
	{
	  memoryBlock * property_block = *q;

	  // -- The property block could be null if the given object never
	  // has this property.

	  if (property_block) {

	    // -- Record the fact that this property is present on the
	    // right-hand side

	    enumPropertyAnn * the_property = property_block->property;

	    assigned_properties.insert(the_property);
	  }
	}
    }

  // -- For each of the properties, for the value to bottom

  pointerValue temp;
  temp.blocks = reachable;

  for (enum_property_set_p p = assigned_properties.begin();
       p != assigned_properties.end();
       ++p)
    {
      enumPropertyAnn * property = (*p);

      if (property->is_enabled() &&
	  (property != _annotations->constants_property()) &&
	  (property->direction() == direction())) { 

	if (Annotations::Verbose_properties) {
	  cout << endl;
	  cout << "  - Set to bottom property " << property->name() << endl;
	}

	property->conservative_procedure_call(current, temp, changes);
      }
    }
}

