
#include "broadway.h"

// ---------------------------------------------------------------------
//  Property value
//  Element of the lattice
// ---------------------------------------------------------------------

enumValueAnn::enumValueAnn(const parserID * id,
			   enumvalue_list * more_specific)
  : Ann(id->line()),
    _name(id->name()),
    _more_general(0),
    _more_specific(),
    _id(0),
    _is_leaf(false)
{
  if (more_specific) {
    _more_specific.swap(* more_specific);

    for (enumvalue_list_p p = _more_specific.begin();
	 p != _more_specific.end();
	 ++p)
      (*p)->_more_general = this;
  }
  else
    _is_leaf = true;
}

/** @brief Collect all more specific values
 *
 * Collect all the values that are more specific that this one, excluding
 * top.*/

void enumValueAnn::collect_atleast_values(enumvalue_set & collect)
{
  // -- Skip top

  if (_more_specific.empty())
    return;

  // -- Insert the current value

  collect.insert(this);

  // -- Visit the values above

  for (enumvalue_list_p p = _more_specific.begin();
	 p != _more_specific.end();
	 ++p)
    (*p)->collect_atleast_values(collect);
}
  

// ---------------------------------------------------------------------
//  Analysis property
// ---------------------------------------------------------------------

enumPropertyAnn::enumPropertyAnn(const parserID * id,
				 Direction direction,
				 bool is_may_property,
				 const parserID * default_name,
				 enumvalue_list * lowest_values)
  : propertyAnn(id, direction, EnumProperty),
    _is_may_property(is_may_property),
    _enforce_weak_updates(true),
    _values(0),
    _top(0),
    _value_index(),
    _now_values(),
    _ever_values(),
    _flow_sensitive_objects(),
    _tested_objects(),
    _accuracy(),
    _error_statements(),
    _number_of_flowvalues(0),
    _total_order(false)
{
  // -- Create a special "bottom" element from which all others are
  // reachable

  parserID temp("(bottom)", id->line());
  _values = new enumValueAnn(&temp, lowest_values);

  // -- Create a special "top" element

  parserID temp2("(top)", id->line());
  _top = new enumValueAnn(&temp2, 0);

  // -- Give "bottom" the lowest ID

  _values->id(0);
  _value_index.push_back(_values);

  // -- Assign a unique ID and a height in the lattice to each element.

  int cur_id = 1;
  int max_height = 0;
  number_values(_values, cur_id, 0, max_height);

  // -- Give "top" the highest ID and a height greater than all other
  // elements.

  _top->id(cur_id);
  _top->height(max_height+1);
  _value_index.push_back(_top);

  _number_of_flowvalues = (double) (_value_index.size() - 3);

  // -- If there is a default value, then look it up. Otherwise, the
  // default is top.

  if (default_name)
    _default = lookup(default_name->name());
  else
    _default = _top;


  // -- Decide if it's a total order (must be called after number_values)

  if (max_height == _number_of_flowvalues)
    _total_order = true;
}

void enumPropertyAnn::number_values(enumValueAnn * prop,
				    int & cur_index, int height, int & max_height)
{
  // -- Set the height and index fields for the current property value

  prop->id(cur_index);
  prop->height(height);

  // -- Keep track of the maximum height

  if (height > max_height)
    max_height = height;

  // -- Put it on the index list in the right place

  _value_index.push_back(prop);

  // -- Increment the index counter

  cur_index++;

  enumvalue_list & more_specific = prop->more_specific();

  if (more_specific.empty()) {

    // If there are no more elements above this one, then insert "top"

    more_specific.push_back(_top);
  }
  else {

  // -- Visit the more specific values, increasing the height by one

    for (enumvalue_list_p p = more_specific.begin();
	 p != more_specific.end();
	 ++p)
      number_values(*p, cur_index, height+1, max_height);
  }
}

void enumPropertyAnn::clear()
{
  // -- Clear all of the store values

  _now_values.clear();
  _ever_values.clear();
}

enumValueAnn * enumPropertyAnn::lookup(const string & name)
{
  for (enumvalue_vec_p p = _value_index.begin();
       p != _value_index.end();
       ++p)
    if ((*p)->name() == name)
      return *p;

  return 0;
}

enumValueAnn * enumPropertyAnn::meet(enumValueAnn * one,
				     enumValueAnn * two)
{
  // X ^ BOTTOM == BOTTOM

  if ((one == bottom()) ||
      (two == bottom()))
    return bottom();

  // X ^ TOP == X

  if (one == top())
    return two;

  if (two == top())
    return one;

  // X ^ X == X

  if (one == two)
    return one;

  // Otherwise, move down in the lattice until one of the above cases
  // holds.

  if (one->height() > two->height())
    return meet(one->more_general(), two);

  if (one->height() < two->height())
    return meet(one, two->more_general());

  return meet(one->more_general(), two->more_general());
}

/** @brief Set meet function
 *
 * Depending on whether this is a "may" or "must" property. Side-effects
 * the value of the first parameter. */

void enumPropertyAnn::meet_with(enumvalue_set & first,
				const enumvalue_set & second)
{
  if (_is_may_property)
    first.set_union(second);
  else
    first.set_intersection(second);
}


bool enumPropertyAnn::at_least(enumValueAnn * left,
			       enumValueAnn * right)
{
  // left >= right  <=> left ^ right == right
  // Also, we'll exclude top because it seems non-intuitive

  return (meet(left, right) == right) && (left != top());
}

bool enumPropertyAnn::at_most(enumValueAnn * left,
			      enumValueAnn * right)
{
  // left <= right  <=> left ^ right == left

  return (meet(left, right) == left);
}

/** @brief Look up a sub-block
 *
 * Given a real memoryBlock, look up the corresponding sub-block for this
 * property. Returns null if none exists. */

memoryBlock * enumPropertyAnn::lookup_property_block(memoryBlock * real_block)
{
  // -- New version: property blocks are stored directly on the real block.

  if (real_block->property_blocks.empty())
    return 0;

  return real_block->property_blocks[get_id()];
}

/** @brief Add a sub-block
 *
 * This method is called by the analyzer during pointer analysis to tell
 * the property about the sub-blocks that it's creating for it. */

void enumPropertyAnn::add_property_block(memoryBlock * real_block, memoryBlock * property_block)
{
  // -- New version: we're storing the property block on the real block
  // itself. We assume that Analyzer::lookup_property_block has already set
  // up the vector.

  real_block->property_blocks[get_id()] = property_block;
}

bool enumPropertyAnn::test(Broadway::Operator op, Broadway::FlowSensitivity flow_sensitivity,
			   Location * where,
			   pointerValue & lhs, string & lhs_name,
			   pointerValue & rhs, string & rhs_name)
{
  bool result = false;
  enumvalue_set rhs_value_set;
  bool lost_information = false;
  memoryblock_set complicit_property_blocks;

  switch (flow_sensitivity) {

  case Broadway::Before:

    // -- Construct the value

    rhs_value_set = construct_now_value(where, rhs, rhs_name, lost_information, complicit_property_blocks);

    // -- Perform the test

    result = test(op, flow_sensitivity, where, lhs, lhs_name, rhs_value_set);

    // -- Record tested objects

    record_tested_objects(where, rhs, rhs_value_set, complicit_property_blocks);

    break;

  case Broadway::After:
    rhs_value_set = construct_after_value(rhs, rhs_name);
    result = test(op, flow_sensitivity, where, lhs, lhs_name, rhs_value_set);
    break;

  case Broadway::Always:
    rhs_value_set = construct_ever_value(rhs, rhs_name);
    result = test(op, flow_sensitivity, where, lhs, lhs_name, rhs_value_set);    
    break;

  case Broadway::Ever:
    {
      rhs_value_set = construct_ever_value(rhs, rhs_name);

      for (int i = 0; i < rhs_value_set.size(); i++)
	{
	  if (rhs_value_set.test(i)) {

	    enumValueAnn * value = _value_index[i];

	    enumvalue_set temp(value);

	    bool one_result = test(op, flow_sensitivity, where, lhs, lhs_name, temp);
	    result = result || one_result;
	  }
	}
    }
    break;

  default:
    cerr << "ERROR: enumPropertyAnn: invalid flow sensitivity mode passed to test()" << endl;
  }

  return result;
}

bool enumPropertyAnn::test(Broadway::Operator op, Broadway::FlowSensitivity flow_sensitivity,
			   Location * where,
			   pointerValue & lhs, string & lhs_name,
			   enumvalue_set rhs_value_set)
{
  bool result = false;
  enumvalue_set lhs_value_set;
  bool lost_information = false;
  memoryblock_set complicit_property_blocks;

  switch (flow_sensitivity) {

  case Broadway::Before:

    // -- Construct the value

    lhs_value_set = construct_now_value(where, lhs, lhs_name, lost_information, complicit_property_blocks);

    // -- Perform the test

    result = test(op, lhs_value_set, rhs_value_set);

    // -- Record tested objects

    record_tested_objects(where, lhs, lhs_value_set, complicit_property_blocks);

    break;

  case Broadway::After:
    lhs_value_set = construct_after_value(lhs, lhs_name);
    result = test(op, lhs_value_set, rhs_value_set);
    break;

  case Broadway::Always:
    lhs_value_set = construct_ever_value(lhs, lhs_name);
    result = test(op, lhs_value_set, rhs_value_set);    
    break;

  case Broadway::Ever:
    {
      lhs_value_set = construct_ever_value(lhs, lhs_name);

      for (int i = 0; i < lhs_value_set.size(); i++)
	{
	  if (lhs_value_set.test(i)) {

	    enumValueAnn * value = _value_index[i];

	    enumvalue_set temp(value);

	    bool one_result = test(op, temp, rhs_value_set);
	    result = result || one_result;
	  }
	}
    }
    break;

  default:
    cerr << "ERROR: enumPropertyAnn: invalid flow sensitivity mode passed to test()" << endl;
  }

  return result;
}

bool enumPropertyAnn::test(Broadway::Operator op,
			   enumvalue_set lhs_value_set, enumvalue_set rhs_value_set)
{
  bool result = false;

  // -- First combine the values into a single one

  enumValueAnn * lhs_value = merge_enumvalue_set(lhs_value_set);
  enumValueAnn * rhs_value = merge_enumvalue_set(rhs_value_set);

  // -- Then perform the specified test

  if (Annotations::Verbose_properties)
    cout << "     -> " << name() << " : " << to_string(lhs_value_set);

  switch (op) {

  case Broadway::Is_Exactly:
    result = lhs_value == rhs_value;

    if (Annotations::Verbose_properties)
      cout << " is-exactly " << to_string(rhs_value_set);

    break;

  case Broadway::Is_AtLeast:
    result = at_least(lhs_value, rhs_value);

    if (Annotations::Verbose_properties)
      cout << " is-atleast " << to_string(rhs_value_set);

    break;

  case Broadway::Could_Be:
    {
      enumvalue_set temp = lhs_value_set;
      temp.set_intersection(rhs_value_set);

      result = (temp.count() != 0);
    }
    break;

  case Broadway::Is_AtMost:
    result = at_most(lhs_value, rhs_value);

    if (Annotations::Verbose_properties)
      cout << " is-atmost " << to_string(rhs_value_set);

    break;

  case Broadway::Is_Bottom:
    result = (lhs_value == bottom());

    if (Annotations::Verbose_properties)
      cout << " is-bottom ";

    break;

  default:
    cerr << "ERROR: enumPropertyAnn: invalid operator passed to test()" << endl;
  }
  
  if (Annotations::Verbose_properties) {
    cout << " == ";
    if (result)
      cout << "true";
    else
      cout << "false";
    cout << endl;
  }

  return result;
}

enumvalue_set enumPropertyAnn::lookup_now_value(memoryBlock * property_block,
						memoryAccess * def_or_use)
{
  // -- Look up this def or use

  defuse_property_map_p p = _now_values.find(def_or_use);

  if (p == _now_values.end())
    return enumvalue_set();
  else
    return (*p).second;
}

bool enumPropertyAnn::update_now_value(Location * where,
				       memoryBlock * property_block,
				       memoryAccess * def_or_use,
				       enumvalue_set newval,
				       bool & lost_information)
{
  bool changed = false;
  lost_information = false;

  // -- Get the old value currently associated with this def/use

  enumvalue_set oldval = lookup_now_value(property_block, def_or_use);

  // -- Special rule for total orders: perform the "real" meet function,
  // where we find the lowest value.

  if (0) { // _total_order) {

    // -- Recompute the new value by first combining the flow values. This
    // effectively sets the new value to the lowest of the inputs.

    enumValueAnn * new_lowest = merge_enumvalue_set(newval);

    // -- Recompute the old value in the same way

    enumValueAnn * old_lowest = merge_enumvalue_set(oldval);

    // -- Meet the two values

    new_lowest = meet(new_lowest, old_lowest);

    // -- Check for change

    changed = (new_lowest != old_lowest);

    // -- Store the result

    enumvalue_set result(new_lowest);
    _now_values[def_or_use] = result;

    if (Annotations::Verbose_properties) {
      cout << "      + Set \"now\" " << property_block->name() << " : "
	   << old_lowest->name() << " => " << new_lowest->name();
      
      if (def_or_use->is_weak())
	cout << " (weak)";
      
      if (changed)
	cout << " -- changed" << endl;
      else
	cout << " -- unchanged" << endl;
    }

    // -- Different monitor: check for the value going down

    if ((old_lowest != _top) &&
	changed)
      lost_information = true;
  } 

  // -- Meet the new value with the old value

  meet_with(newval, oldval);

  // -- Check for change

  changed = (newval != oldval);

  if (newval.test(top()->id()))
    cout << "BAD: update_now_value includes top at" << * where << endl;

  // -- Store the result

  _now_values[def_or_use] = newval;

  if (Annotations::Verbose_properties) {
    cout << "      + Set \"now\" " << property_block->name() << " : "
	 << to_string(oldval) << " => " << to_string(newval);
    
    if (def_or_use->is_weak())
      cout << " (weak)";
    
    if (changed)
      cout << " -- changed" << endl;
    else
      cout << " -- unchanged" << endl;
  }

  if (0) { // _total_order) {

    // -- Monitor: total order rule: if the old value was higher than the
    // new value, then information was lost.

    enumValueAnn * merged_new = merge_enumvalue_set(newval);
    enumValueAnn * merged_old = merge_enumvalue_set(oldval);

    if (merged_new != merged_old)
      lost_information = true;
  }
  else {

    // -- Monitor: information is lost when the number of potential values
    // goes from 1 (the best information) to greater than 1. We adjust the
    // old value to exclude the weird case when the only value is the
    // default value.
    
    int old_count = oldval.count();

    /*
      if ((old_count == 1) &&
      (oldval.test(default_val()->id())))
      old_count = 0;
    */

    if ( // (old_count > 0) &&
	(newval.count() > 1) &&
	(newval.count() > old_count))
      lost_information = true;
  }

  /*
  if (lost_information &&
      (property_block->name() == "incline[].Trust")) {
    cout << "SET: " << property_block->name() << " at " << * where << endl;
    cout << "   was: " << to_string(oldval) << "    now: " << to_string(newval) << endl;
  }
  */

  return changed;
}

bool enumPropertyAnn::update_ever_value(memoryBlock * property_block,
					enumvalue_set newval)
{
  enumvalue_set cur_vals = _ever_values[property_block];
  enumvalue_set temp = cur_vals;

  bool result = false;

  // -- Add in the new values
    
  meet_with(temp, newval);

  // -- Check for change

  if (temp != cur_vals) {
    result = true;

    // -- If changed, store the new value

    _ever_values[property_block] = temp;
  }

  return result;
} 

enumvalue_set enumPropertyAnn::construct_now_value(Location * where,
						   pointerValue & variable,
						   string & variable_name,
						   bool & lost_information,
						   memoryblock_set & complicit_property_blocks)
{
  enumvalue_set result;

  lost_information = false;

  if (Annotations::Verbose_properties)
    cout << "      + Construct \"now\" value for " << variable_name << endl;

  // -- For each memory block ...

  int min_local_count = 9999;
  enumValueAnn * highest_value = 0;

  for (memoryblock_set_p p = variable.blocks.begin();
       p != variable.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {
	enumvalue_set local_val = construct_now_value(where, property_block, false);

	// -- Monitor: Special rule for total orders:

	if (_total_order) {

	  // -- Merge the values together using the "real" meet function
	  
	  enumValueAnn * merged_val = merge_enumvalue_set(local_val);

	  // -- Keep track of the best value we see

	  if ( ! highest_value )
	    highest_value = merged_val;
	  else {

	    if ((merged_val != highest_value) &&
		at_least(merged_val, highest_value))
	      highest_value = merged_val;
	  }
	}
	else {
	  
	  // -- Regular mode: the best value is the lowest count

	  int count = local_val.count();
	  if ((count != 0) && 
	      (count < min_local_count))
	    min_local_count = count;

	  if (count > 1)
	    complicit_property_blocks.insert(property_block);
	}

	// -- Add into the result

	meet_with(result, local_val);
      }
    }

  /*
  if ((min_local_count > 0) &&
      (result.count() > min_local_count))
    lost_information = true;
  */

  if (0) { // _total_order) {

    enumValueAnn * merged_val = merge_enumvalue_set(result);

    if (merged_val != highest_value)
      lost_information = true;
  }
  else {

    if (result.count() > min_local_count)
      lost_information = true;
  }

  if (Annotations::Verbose_properties) {
    cout << "       = " << to_string(result) << endl;
    cout << "         [ min count = " << min_local_count << ", result count = " << result.count() << "]" << endl;
  }

  return result;
}

enumvalue_set enumPropertyAnn::construct_now_value(Location * where,
						   memoryBlock * property_block,
						   bool default_to_top)
{
  enumvalue_set result;

  if (Annotations::Verbose_properties)
    cout << "        - Lookup \"Now\" value of " << property_block->name()
	 << " real block = " << property_block->container()
	 << " real decl = " << property_block->container()->decl() << endl;

  // -- Forward or backward

  if (direction() == Forward) {

    // -- Forward analysis: the now value is constructed by getting the
    // current use and looking up the reaching definition.

    if (Annotations::Verbose_properties)
      cout << "          use at ";

    memoryUse * use = property_block->current_use();
    if (use) {
      memoryDef * def = use->reaching_def();

      if (Annotations::Verbose_properties) {
	cout << * (use->where()) << endl;
	cout << "          reaching def at ";
      }

      // -- If the def has a property value, look it up. If there is no
      // reaching def, use the default value.

      if (def) {
	result = lookup_now_value(property_block, def);

	if (Annotations::Verbose_properties) {
	  cout << * (def->where()) << endl;
	  cout << "         = " << to_string(result) << endl;
	}
      }
      else {

	if (Annotations::Verbose_properties) {
	  cout << "(no reaching def)" << endl;
	  cout << "         = " << to_string(result) << endl;
	}

	// -- Weird special case: when a write-protected object has no
	// reaching def (for example, the null object), then we'll return
	// top.

	// if (property_block->container()->write_protected())
	//  result = top();
      }
    }
    else {

      // -- This is bad: the variable has no use here (i.e., it is
      // probably missing an access annotation).

      if (Annotations::Verbose_properties)
	cout << " NO USE " << endl;

      // result = 0;
    }
  }
  else {

    // -- Backward analysis: the now value is constructed by getting
    // the current definition, and meeting together the values from all
    // of the uses that it reaches.

    if (Annotations::Verbose_properties)
      cout << "          def at ";

    memoryDef * current_def = property_block->current_def();
    if (current_def) {

      if (Annotations::Verbose_properties) {
	cout << * (current_def->where()) << endl;
	cout << "          uses reached:" << endl;
      }

      if (current_def->uses().empty()) {

	// -- Special case: if the def has no uses (this can happen with
	// merge points), then return the default value.

	// result = 0;

	if (Annotations::Verbose_properties)
	  cout << "              (no uses reached)" << endl;

	// -- Weird special case: when a write-protected object has no
	// reaching def (for example, the null object), then we'll return
	// top.

	// if (property_block->container()->write_protected())
	//  result = top();
      }
      else {

	// -- Otherwise, visit all the uses reached by the def

	for (memoryuse_list_p u = current_def->uses().begin();
	     u != current_def->uses().end();
	     ++u)
	  {
	    memoryUse * use = *u;

	    enumvalue_set local_val = lookup_now_value(property_block, use);
	    meet_with(result, local_val);
	    if (Annotations::Verbose_properties)
	      cout << "              Use at " << * (use->where()) << " value = " << to_string(local_val) << endl;
	  }
      }
    }
    else {

      // -- This is bad: the variable has no def here (i.e., it is
      // probably missing a modifies annotation).

      if (Annotations::Verbose_properties)
	cout << " NO DEF " << endl;
      // result = 0;
    }
  }

  // -- Did we find anything?

  // if (no_result) {

  if (result.none()) {

    // -- Decide on the default behavior

    if (default_val() != top())
      result = enumvalue_set(default_val());
  }

  if (result.test(top()->id()))
    cout << "BAD: construct_now_value includes top at " << * where << endl;

  return result;
}

enumvalue_set enumPropertyAnn::construct_after_value(pointerValue & variable,
						     string & name)
{
  enumvalue_set result;

  if (Annotations::Verbose_properties)
    cout << "      + Construct \"after\" value for " << name << endl;

  // -- For each memory block ...

  for (memoryblock_set_p p = variable.blocks.begin();
       p != variable.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {
	enumvalue_set local_val = construct_after_value(property_block);
	meet_with(result, local_val);
      }
    }

  if (Annotations::Verbose_properties)
    cout << "       = " << to_string(result) << endl;

  return result;
}

enumvalue_set enumPropertyAnn::construct_after_value(memoryBlock * property_block)
{
  enumvalue_set result;

  if (Annotations::Verbose_properties)
    cout << "        - Lookup \"After\" value of " << property_block->name();

   // -- Forward or backward

  if (direction() == Forward) {

    // -- Forward analysis: the new property value is associated with
    // the current definition.

    if (Annotations::Verbose_properties)
      cout << " def at ";

    memoryDef * def = property_block->current_def();

    // -- If the def has a property value, collect it into the return
    // value using the meet function.

    if (def) {
      if (Annotations::Verbose_properties)
	cout << * (def->where()) << endl;
 
      enumvalue_set val = lookup_now_value(property_block, def);
      meet_with(result, val);
    }
    else {

      if (Annotations::Verbose_properties)
	cout << "NO CURRENT DEF" << endl;
 
      // -- This is bad: the variable has no def here (i.e., it is
      // probably missing a modify annotation).
    }
  }
  else {

    // -- Backward analysis: the new property value is associated with
    // the current use.

    if (Annotations::Verbose_properties)
      cout << " use at ";

    memoryUse * use = property_block->current_use();

    // -- If the use has a property value, collect it into the return
    // value using the meet function.

    if (use) {
      if (Annotations::Verbose_properties)
	cout << * (use->where()) << endl;

      enumvalue_set temp = lookup_now_value(property_block, use);
      meet_with(result, temp);
    }
    else {

      if (Annotations::Verbose_properties)
	cout << "NO CURRENT USE" << endl;
 
      // -- This is bad: the variable has no use here (i.e., it is
      // probably missing an access annotation).
    }
  }

  return result;
}

enumvalue_set enumPropertyAnn::construct_ever_value(pointerValue & variable,
						    string & name)
{
  enumvalue_set result;

  if (Annotations::Verbose_properties)
    cout << "      + Construct \"ever\" values for " << name << endl;

  // -- For each memory block...

  for (memoryblock_set_p p = variable.blocks.begin();
       p != variable.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {

	if (Annotations::Verbose_properties)
	  cout << "        - Lookup \"Ever\" value of " << property_block->name()
	       << " = { ";

	// -- Look up the set of values

	block_propertyset_map_p q = _ever_values.find(property_block);
	if (q != _ever_values.end()) {
	  enumvalue_set & vals = (*q).second;

	  // -- Collect those values into the return set

	  meet_with(result, vals);

	  if (Annotations::Verbose_properties) {
	    cout << to_string(vals);
	  }
	}

	if (Annotations::Verbose_properties)
	  cout << "}" << endl;
      }
    }

  if (Annotations::Verbose_properties) {
    cout << "          = { " << to_string(result) << "}" << endl;
  }

  return result;
}

enumvalue_set enumPropertyAnn::construct_weak_now_value(Location * where,
							pointerValue & variable, string & name)
{
  enumvalue_set result;

  if (Annotations::Verbose_properties)
    cout << "      + Construct \"weak now\" value..." << endl;

  // -- For each memory block ...

  for (memoryblock_set_p p = variable.blocks.begin();
       p != variable.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {

	enumvalue_set local_val;

	// -- Get the now value

	local_val = construct_now_value(where, property_block, false);

	// -- Add it in the collected value

	meet_with(result, local_val);

	// -- If the block was weakly updated, then add in the after value

	if (property_block->current_def() &&
	    property_block->current_def()->is_weak()) {

	  local_val = construct_after_value(property_block);

	  meet_with(result, local_val);
	}
      }
    }

  if (Annotations::Verbose_properties)
    cout << "       = " << to_string(result) << endl;

  return result;
}


/** @brief Compute next state
 *
 * This is called in enumPropertyExprAnn::compute_next(), but it's
 * basically just a wrapper around contruct_now_value(), with the
 * addition of the assignment management. */

enumvalue_set enumPropertyAnn::compute_next(Location * where,
					    ruleAnn * rule,
					    exprAnn * expr,
					    pointerValue & right,
					    string & right_name,
					    bool & rhs_lost_information,
					    memoryblock_set & complicit_property_blocks,
					    enumvalue_set & ever_values)
{
  // -- The new property value is just the value of the right-hand side

  enumvalue_set result = construct_now_value(where, right, right_name,
					     rhs_lost_information, complicit_property_blocks);

  // -- Also, collect the ever values

  ever_values = construct_ever_value(right, right_name);

  return result;
}

/** @brief Change state
 *
 *  Update the state of the given variable to have the given property
 *  value. This method updates all three kinds of flow information. Put any
 *  changes into the changes set (NOTE: these will be the property blocks,
 *  not the real blocks). */

void enumPropertyAnn::apply_next(Location * where,
				 stmtLocation * parameter_callsite,
				 ruleAnn * rule, enumPropertyExprAnn * expr,
				 pointerValue & left,
				 string & left_name,
				 pointerValue & right,
				 enumvalue_set proposed_new_value,
				 bool rhs_lost_information,
				 memoryblock_set & complicit_property_blocks,
				 enumvalue_set ever_values,
				 memoryblock_set & changes)
{
  memoryblock_set right_property_blocks;

  // -- For each block in the pointer

  for (memoryblock_set_p p = left.blocks.begin();
       p != left.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;

      memoryDef * change_def = 0;
      memoryAccess * attach_value_to = 0;
      enumvalue_set new_value = proposed_new_value;

      bool lost_information = false;

      // -- Skip write-protected objects

      if ( ! real_block->write_protected()) {
	
	memoryBlock * property_block = lookup_property_block(real_block);

	if (property_block) {

	  if (Annotations::Verbose_properties) {
	    cout << "    + Apply: " << property_block->name()
		 << " <- " << to_string(new_value) << endl;
	  }
	  
	  // -- Record this right-hand side

	  right_property_blocks.insert(property_block);

	  // -- Set up forward or backward update

	  if (direction() == Forward) {

	    // -- Forward analysis: propagate the new states to the current
	    // definition

	    memoryDef * def = property_block->current_def();
	      
	    change_def = def;
	    attach_value_to = def;
	  }
	  else {

	    // -- Backward analysis: propagate the new states to the
	    // current use.

	    memoryUse * use = property_block->current_use();

	    // -- We record the reaching def as the changed definition

	    change_def = use->reaching_def();
	
	    // -- Attach the value to the use

	    attach_value_to = use;
	  }

	  bool weak_update_lost_information = false;

	  if (enforce_weak_updates()) {

	    // -- Handle weak updates. Special case: never apply a weak
	    // update when there is no "now" value (in other words, don't
	    // just mindlessly merge in the default value). If there are no
	    // reaching defs/uses, then just leave the value alone.

	    if (attach_value_to->is_weak()) {

	      // -- Get the value that reaches this location (the previous value)

	      enumvalue_set previous_value = construct_now_value(where, property_block, true);

	      // -- Meet it in with the new value

	      meet_with(new_value, previous_value);

	      // -- Check to see if the weak update lost information

	      if (new_value != proposed_new_value)
		weak_update_lost_information = true;
	    }
	  }

	  // -- Set the "now" value

	  bool change_now = update_now_value(where, property_block, attach_value_to, new_value, lost_information);

	  // -- Update the flow-insensitive "ever" value

	  bool change_ever = update_ever_value(property_block, new_value);

	  // -- Pass on any "ever" values

	  // bool pass_evers = false;
	  bool pass_evers = update_ever_value(property_block, ever_values);

	  // -- If anything changed, record where it happened

	  if (change_now && change_def) // || change_always || change_ever || pass_evers) && change_def)
	    changes.insert(property_block);

	  // -- Monitor: Diagnose the information loss

	  if (pointerOptions::Monitor_precision) {

	    // -- If this is a parameter being passed, then record the
	    // reaching defs.

	    if ((parameter_callsite) &&
		(where->kind() == Location::Procedure))
	      {
		procLocation * procloc = (procLocation *) where;

		memorydef_set reaching_defs;

		property_block->add_parameter_assignment(procloc->proc(),
							 parameter_callsite,
							 right_property_blocks);
	      }
	    
      	    // -- If the right-hand-side pointer dereference lost
	    // information, then add the complicit assignment information.

	    if (rhs_lost_information)
	      property_block->add_complicit_assignment(where, right.dereferenced);

	    // -- Check if the assignment lost precision

	    if (lost_information || weak_update_lost_information) {

	      // -- Add complicit objects from the RHS

	      property_block->add_complicit_assignment(where, complicit_property_blocks);

	      // -- Check for weak or strong update

	      if (attach_value_to->is_weak()) {

		// -- Special case: we don't care about the left-hand side
		// for weak updates that were forced by the annotations, or
		// that occured because the objects are flow insensitive.

		bool forced_weak = false;

		if ( expr &&
		     expr->is_weak())
		  forced_weak = true;

		if ( ! forced_weak) {

		  if (weak_update_lost_information) {

		    // -- If a weak update occured, check to see if it was caused by
		    // multiple left-hand-sides or by multiplicity.

		    if (left.blocks.size() > 1) {

		      // -- Multiple left-hand-sides: the problem is the
		      // left-hand dereferenced pointers.

		      property_block->add_complicit_assignment(where, left.dereferenced);
		    }
		    else {

		      // -- High multiplicity: if it wasn't a pointer-induced
		      // weak update, then it must have been caused by
		      // multiplicity. If it's a heap block, then blame the
		      // alloc object.

		      memoryBlock * alloc_object = property_block->allocation_object();
		      if (alloc_object)
			property_block->add_complicit_assignment(where, alloc_object);

		      // -- NOTE: We always add the destructive assignment to
		      // force this object to become flow
		      // sensitive. Otherwise we'll never track the
		      // multiplcity accurately.

		      property_block->add_destructive_assignment(where, memoryBlock::Weak_update);
		    }
		  }
		}
	      }
	      else {

		// -- Strong update

		if ((where->kind() == Location::Statement) ||
		    (rule != 0)) {

		  // -- Regular statement: this can only happend with an
		  // additive assignment caused by flow-insensitivity.
		    
		  property_block->add_destructive_assignment(where, memoryBlock::Additive);
		}
		else {

		  // -- Parameter pass: this happens when the procedure
		  // is context-insensitive and it gets different values
		  // in the different contexts.

		  property_block->add_destructive_assignment(where, memoryBlock::Parameter_pass);
		}
	      }
	    }
	  } // -- END monitor
	} // -- END if property_block
      } // -- END not write-protected
    } // -- END for all objects
}

void enumPropertyAnn::apply_merge(Location * where,
				  memoryBlock * property_block,
				  memoryuse_list & phi_uses,
				  memoryblock_set & changes)
{
  if (Annotations::Verbose_properties)
    cout << "  + Merge " << property_block->name() << endl;

  enumvalue_set oldval;
  enumvalue_set merged_val;

  // -- Record if we lose information

  bool lost_information = false;
  bool throw_away;

  // -- Forwards or backwards

  if (direction() == Forward) {

    // -- Forward analysis: collect the values that reach the merge uses,
    // meet them together and propagate that value to the def.

    // -- Get the old value

    memoryDef * def = property_block->current_def();

    if (def)
      oldval = lookup_now_value(property_block, def);

    // -- Merge together the uses that reach this merge point

    int min_reaching_count = 9999;

    for (memoryuse_list_p p = phi_uses.begin();
	 p != phi_uses.end();
	 ++p)
      {
	memoryUse * phi_use = *p;
	memoryDef * reaching_def = phi_use->reaching_def();

	// -- Find the reaching value, handling the default case as well

	enumvalue_set reaching_val;

	if (reaching_def) {

	  // -- There is a reaching def, look it up

	  reaching_val = lookup_now_value(property_block, reaching_def);

	  if (Annotations::Verbose_properties)
	    cout << "   = " << to_string(reaching_val) << " at " << * (reaching_def->where()) << endl;
	}
	else {

	  // -- No reaching def, use the default value

	  if (default_val() != top())
	    reaching_val = enumvalue_set(default_val());

	  if (Annotations::Verbose_properties)
	    cout << "   = default value " << to_string(reaching_val) << endl;
	}

	int count = reaching_val.count();
	if (count < min_reaching_count)
	  min_reaching_count = count;

	meet_with(merged_val, reaching_val);
      }

    // -- Check for information loss

    if ((min_reaching_count == 1) &&
	(merged_val.count() > 1))
      lost_information = true;

    // -- Did anything change?

    bool changed = update_now_value(where, property_block, def, merged_val, throw_away);
    if (changed) {

      changes.insert(property_block);

      if (Annotations::Verbose_properties)
	cout << "   -> merge changed " << property_block->name() << endl;
    }
  }
  else {

    // -- Backward analysis: get the value of the definition and just copy
    // it to all the merge uses.

    merged_val = construct_now_value(where, property_block, false);

    for (memoryuse_list_p p = phi_uses.begin();
	 p != phi_uses.end();
	 ++p)
      {
	memoryUse * phi_use = *p;

	// oldval = lookup_now_value(block, phi_use);

	// -- Did anything change?

	bool changed = update_now_value(where, property_block, phi_use, merged_val, lost_information);
	if (changed && phi_use->reaching_def()) {

	  changes.insert(property_block);

	  if (Annotations::Verbose_properties)
	    cout << "   -> merge changed " << property_block->name() << endl;
	}
      }
  }

  // -- Monitor: see if the merge caused a loss of information

  if (pointerOptions::Monitor_precision) {

    if (lost_information)
      property_block->add_destructive_assignment(where, memoryBlock::Control_flow);
  }

  // -- NOTE that a merge will never change the "always" or "ever" values,
  // so there is no point touching them.
}

/** @brief Apply assignment
 *
 * At a normal assignment, build the "now" value for the right-hand side,
 * and assign it to the left-hand side. Weak updates are handled by the
 * apply_next() method. */

void enumPropertyAnn::apply_assignment(Location * where,
				       stmtLocation * parameter_callsite,
				       pointerValue & left,
				       pointerValue & right,
				       bool is_parameter,
				       memoryblock_set & changes)
{
  enumvalue_set new_value;
  enumvalue_set ever_values;
  pointerValue * to_update = 0;
  bool rhs_lost_information = false;
  memoryblock_set complicit_property_blocks;

  // -- Skip expressions where the right-hand-side is an address:
  // (e.g., "p = &x" does not propagate the value to or from x).

  if ( ! right.is_address) {

    string rhs_name("right-hand side");
    string lhs_name("left-hand side");

    // -- Forward or backward: figure out what the new value is, and which
    // blocks to update.

    if (direction() == Forward) {

      // -- Forward: new value comes the right, and updates the left.

      new_value = construct_now_value(where, right, rhs_name, rhs_lost_information, complicit_property_blocks);
      ever_values = construct_ever_value(right, rhs_name);

      // -- Perform the actual update

      apply_next(where, parameter_callsite,
		 (ruleAnn *)0, (enumPropertyExprAnn *)0,
		 left, lhs_name,
		 right, new_value,
		 rhs_lost_information, complicit_property_blocks,
		 ever_values, changes);
    }
    else {

      // -- Backward: new value comes from the left and updates the
      // right. Note that we make sure to get the value *after* any weak
      // updates.

      new_value = construct_weak_now_value(where, left, lhs_name);
      ever_values = construct_ever_value(left, lhs_name);

      // -- Perform the actual update

      apply_next(where, parameter_callsite,
		 (ruleAnn *)0, (enumPropertyExprAnn *)0,
		 right, rhs_name,
		 left, new_value,
		 rhs_lost_information, complicit_property_blocks,
		 ever_values, changes);
    }
  }
}

/** @brief Self assignment
 *
 * Now that properties are associated with special property blocks, we need
 * to explicitly handle self-assignment caused by passing parameters and
 * external inputs and outputs. */

void enumPropertyAnn::self_assignment(Location * source,
				      Location * target,
				      memoryBlock * property_block, 
				      memoryblock_set & changes)
{
  if (Annotations::Verbose_properties)
    cout << "  + Pass " << name() << endl;

  // -- Get the current reaching value

  enumvalue_set current_value = construct_now_value(source, property_block, true);

  // -- Figure out the direction 

  memoryAccess * attach_value_to = 0;

  if (direction() == Forward) {

    // -- Forward analysis: propagate the new states to the current
    // definition

    attach_value_to = property_block->current_def();
  }
  else {

    // -- Backward analysis: propagate the new states to the current
    // use. 

    attach_value_to = property_block->current_use();
  }

  // -- Assign it to itself

  bool lost_information = false;
  bool change = update_now_value(target, property_block, attach_value_to, current_value, lost_information);

  // -- Record any change

  if (change)
    changes.insert(property_block);

  // -- Monitor: check for parameter pass loss of information

  /*
  if (pointerOptions::Monitor_precision) {

    if (lost_information)
      property_block->add_destructive_assignment(target, memoryBlock::Parameter_pass);

    // -- Add parameter assignment

    if ((target->kind() == Location::Procedure) &&
	(source->kind() == Location::Procedure))
    {
      procLocation * procloc = (procLocation *) target;
      stmtLocation * callsite = (stmtLocation *) source;

      memoryUse * use = property_block->current_use();
      if (use) {
	memoryDef * reaching_def = use->reaching_def();

	if (reaching_def)
	  property_block->add_parameter_assignment(procloc->proc(),
						   callsite,
						   reaching_def);
      }
    }
  }
  */
}

/** @brief Conservative procedure call
 *
 * Record a conservative procedure call, primarily for back-trace
 * purposes. */

void enumPropertyAnn::conservative_procedure_call(stmtLocation * current,
						  pointerValue & reachable,
						  memoryblock_set & changes)
{
  // -- Conservative call: set everything to bottom
  /*
  for (memoryblock_set_p p = reachable.blocks.begin();
       p != reachable.blocks.end();
       ++p)
    {
      memoryBlock * real_block = *p;

      // -- Use the apply_next() method because it does the right thing

      apply_next(real_block, bottom(), changes);
    }
  */
}

void enumPropertyAnn::report(ostream & out,
			     bool is_error,
			     procLocation * where, 
			     Broadway::FlowSensitivity flow_sensitivity,
			     pointerValue & lhs, string & lhs_name)
{
  enumvalue_set lhs_value_set;
  enumValueAnn * lhs_value = 0;
  bool lost_information = false;
  memoryblock_set complicit_property_blocks;

  switch (flow_sensitivity) {

  case Broadway::Before:
    lhs_value_set = construct_now_value(where, lhs, lhs_name, lost_information, complicit_property_blocks);
    lhs_value = merge_enumvalue_set(lhs_value_set);

    if (! Annotations::Quiet_reports) {
      out << lhs_value->name();
      out << " (" << to_string(lhs_value_set) << ")";
    }

    // -- Record tested objects

    record_tested_objects(where, lhs, lhs_value_set, complicit_property_blocks);

    break;

  case Broadway::After:
    lhs_value_set = construct_after_value(lhs, lhs_name);
    lhs_value = merge_enumvalue_set(lhs_value_set);

    if (! Annotations::Quiet_reports)
      out << lhs_value->name();

    break;

  case Broadway::Always:
    lhs_value_set = construct_ever_value(lhs, lhs_name);
    lhs_value = merge_enumvalue_set(lhs_value_set);

    if (! Annotations::Quiet_reports)
      out << lhs_value->name();

    break;

  case Broadway::Ever:
    {
      lhs_value_set = construct_ever_value(lhs, lhs_name);

      if (! Annotations::Quiet_reports)
	out << to_string(lhs_value_set);
    }
    break;

  case Broadway::Trace:
    {
      if (Annotations::Skip_traces)
	out << "(skipping trace)";
      else {

	for (memoryblock_set_p p = lhs.blocks.begin();
	     p != lhs.blocks.end();
	     ++p)
	  {
	    memoryBlock * real_block = *p;
	    memoryBlock * property_block = lookup_property_block(real_block);

	    if (! Annotations::Quiet_reports) {
	      out << endl << "TRACE: " << property_block->name() << endl;

	      memoryblock_set already_seen;
	      string indent = "  ";

	      trace_object(out, property_block, already_seen, indent);
	    }
	  }
      }
    }
    break;

  case Broadway::Confidence:
    {
      lhs_value_set = construct_now_value(where, lhs, lhs_name, lost_information, complicit_property_blocks);

      // -- Count the number of possible values.

      double dsize = (double) lhs_value_set.count();

      // -- Check for top

      double confidence;

      if (dsize == 0)
	confidence = 0.0;
      else {
	confidence = (_number_of_flowvalues - dsize) / (_number_of_flowvalues - 1.0);
	// OLD value confidence = (1.0 / dsize );
      }

      if (! Annotations::Quiet_reports)
	out << confidence;

      if (dsize != 0) {
	
	// -- Also, store the confidence values in order to compute accuracy

	accuracy_map_p p = _accuracy.find(where);
	if (p == _accuracy.end()) {

	  _accuracy[where] = count_accuracy_pair(1, confidence);
	}
	else {
	  count_accuracy_pair & cap = (*p).second;
	  cap.first++;
	  cap.second += confidence;
	}

	// -- If this is an error report, record the statement where it
	// occured

	if (is_error)
	  _error_statements.insert(where->stmt_location()->stmt());
      }
    }
    break;

  default:
    cerr << "ERROR: enumPropertyAnn: invalid flow sensitivity mode passed to report()" << endl;
  }
}


void enumPropertyAnn::print(ostream & o) const
{
  o << "property " << name();

  o << "{" << endl;
  print(o, values(), 2);
  o << "}" << endl;
}

void enumPropertyAnn::print(ostream & o, enumValueAnn * prop, int depth) const
{
  for (int i = 0; i < depth; i++)
    o << " ";

  o << prop->name() << ", id = " << prop->id() << ", height = " << prop->height() << endl;

  const enumvalue_list & vals = prop->more_specific();
  for (enumvalue_list_cp p = vals.begin();
       p != vals.end();
       ++p)
    print(o, *p, depth+2);
}

/** @brief Merge enumvalue_set
 *
 * Visit the values in an enumvalue_set and merge them into a single value
 * using the meet function. */

enumValueAnn * enumPropertyAnn::merge_enumvalue_set(enumvalue_set value_set)
{
  enumValueAnn * result = top();

  // -- Visit all the values in the set and meet them together

  for (int i = 0; i < value_set.size(); i++) {
      
    if (value_set.test(i)) {

      enumValueAnn * value = _value_index[i];

      result = meet(result, value);
    }
  }

  return result;
}

/** @brief Convert enum values to string */

string enumPropertyAnn::to_string(enumvalue_set value_set)
{
  bool first = true;
  string s;

  if (value_set.count() == 0)
    s = "(empty)";
  else {

    for (int i = 0; i < value_set.size(); i++) {

      if (value_set.test(i)) {

	enumValueAnn * value = _value_index[i];
	if ( ! first ) s += ", ";
	s += value->name();
	first = false;
      }
    }
  }

  return s;
}  

// ----------------------------------------------------------------------
//  Precision analysis
// ----------------------------------------------------------------------

/** @brief Precision analysis
 *
 * Post-process the results of analysis to determine a list of variables
 * to make flow sensitive. */

void enumPropertyAnn::precision_analysis(memoryModel * memory_model,
					 memoryblock_set & flow_sensitive)
{
  const memoryModel::memorymodel_map & memory = memory_model->memory_blocks();

  if (Annotations::Adaptivity == 1) {

    // -- Adaptivity Option 1: post-process the flow-insensitive
    // information to determine variables that should be flow sensitive.

    // -- Visit all memory blocks and perform the precision analysis

    block_propertyset_map already_seen;
    memoryblock_set property_flow_sensitive;

    string indent = "  ";

    for (memoryModel::memorymodel_map_cp p = memory.begin();
	 p != memory.end();
	 ++p)
      {
	memoryBlock * block = (*p).second;

	enumvalue_set values;

	// -- Skip the property blocks themselves; we'll reach them through
	// the real blocks.

	if (! block->property)
	  analyze_one_memoryblock(block, values, already_seen, property_flow_sensitive, indent);
      }

    // -- Print the results

    cout << "Property " << name() << ": " << endl;
    cout << "  Make " << property_flow_sensitive.size() << " out of " << memory.size() << " flow sensitive:" << endl;

    for (memoryblock_set_p p = property_flow_sensitive.begin();
	 p != property_flow_sensitive.end();
	 ++p)
      {
	memoryBlock * block = *p;

	cout << "    + " << block->name() << endl;

	block->add_to_flow_sensitive_list(pointerOptions::Flow_sensitive_objects);
      }
  }

    /*
  if (Annotations::Adaptivity == 2) {

     // -- Adaptivity Option 2: decide online which objects to make flow-sensitive
    cout << "Property " << name() << ": " << endl;
    cout << "  Make " << _flow_sensitive_objects.size() << " out of " << memory.size() << " flow sensitive:" << endl;

    for (memoryblock_set_p p = _flow_sensitive_objects.begin();
	 p != _flow_sensitive_objects.end();
	 ++p)
      {
	memoryBlock * block = *p;

	cout << "    + " << block->name() << endl;

	pointerOptions::add_flow_sensitive_object(block);
      }

  }

  _flow_sensitive_objects.clear();
    */
}

/** @brief NEW Precision analysis
 */

void enumPropertyAnn::precision_analysis()
{
  if (Annotations::Adaptivity == 3) {

    // -- Adaptivity Option 3: Use the pointer monitor to track the tested
    // objects back to destructive assignments.

    memoryblock_set made_flow_sensitive;

    memoryblock_set already_seen;

    string indent(" ");
    for (tested_objects_map_p p = _tested_objects.begin();
	 p != _tested_objects.end();
	 ++p)
      {
	Location * target = (*p).first;
	memoryblock_set & objects = (*p).second;

	for (memoryblock_set_p q = objects.begin();
	     q != objects.end();
	     ++q)
	  {
	    memoryBlock * real_block = *q;
	    memoryBlock * property_block = lookup_property_block(real_block);

	    if (Annotations::Show_adaptivity)
	      cout << "TRACK " << real_block->name() << " for property " << name()
		   << " at " << * target << endl;

	    if (property_block &&
		! real_block->write_protected()) {
	      track_destructive_assignments(target, property_block, already_seen, made_flow_sensitive, indent);
	    }
	  }
      }

    if (Annotations::Show_adaptivity) {
      cout << "Property " << name() << ":" << endl;
      for (memoryblock_set_p p = made_flow_sensitive.begin();
	   p != made_flow_sensitive.end();
	   ++p)
	{
	  cout << "  + " << (*p)->name() << endl;
	}
    }
  }

  // -- Clear the tested objects set

  _tested_objects.clear();
}

double enumPropertyAnn::compute_accuracy(Analyzer * analyzer)
{
  // -- Collect the information by statement

  stmt_accuracy_map by_statement;

  for (accuracy_map_p p = _accuracy.begin();
       p != _accuracy.end();
       ++p)
    {
      procLocation * where = (*p).first;
      count_accuracy_pair & cap = (*p).second;

      // -- Count the number of ways to reach this location. First, we move
      // up the call chain to skip over any context-sensitive procedures.

      procLocation * cur = where;

      while (cur->stmt_location())
	cur = cur->stmt_location()->block_location()->proc_location();

      // -- Look up the procedure

      procedureInfo * info = analyzer->lookup_procedure(cur->proc());

      // -- Now get the context count

      int count = info->count_calling_contexts();

      // -- Construct accuracy information that takes into account the
      // number of contexts.

      count_accuracy_pair new_cap;

      new_cap.first  = cap.first * count;
      new_cap.second = cap.second * ((double)count);

      // -- Get the statement itself

      stmtNode * stmt = where->stmt_location()->stmt();

      // -- Add the values in to the total

      stmt_accuracy_map_p w = by_statement.find(stmt);
      if (w == by_statement.end())
	by_statement[stmt] = new_cap;
      else {
	count_accuracy_pair & existing_cap = (*w).second;

	existing_cap.first  += new_cap.first;
	existing_cap.second += new_cap.second;
      }
    }

  // -- Print out the results, according to the call site and compute
  // aggregate accuracy

  double total = 0.0;
  int count = 0;

  double error_total = 0.0;
  int error_count = 0;

  for (stmt_accuracy_map_p p = by_statement.begin();
	 p != by_statement.end();
       ++p)
    {
      stmtNode * stmt = (*p).first;
      count_accuracy_pair & cap = (*p).second;

      callNode * call = reportElementAnn::callnode(stmt);
      
      double local_accuracy = (cap.second / ((double) cap.first));

      cout << "ACCURACY: " << name() <<  " at " << call->coord() << " = " << local_accuracy << endl;

      count++;
      total += local_accuracy;

      // -- Sum up error accuracy separately

      if (_error_statements.find(stmt) != _error_statements.end()) {
	error_count++;
	error_total += local_accuracy;
      }
    }

  double overall_accuracy = 0.0;

  if (count > 0) {

    overall_accuracy = (total / ((double)count));

    cout << "STAT-accuracy-property-all-" << name() << " " << overall_accuracy << endl;
  }
  
  if (error_count > 0)
    cout << "STAT-accuracy-property-errors-" << name() << " " << (error_total / ((double)error_count)) << endl;

  // -- Clear information

  _accuracy.clear();
  _error_statements.clear();

  return overall_accuracy;
}

/** @brief Add flow-sensitive object
 *
 * Record that in the future, this property block should be flow
 * sensitive. */

void enumPropertyAnn::add_flow_sensitive_object(memoryBlock * property_block)
{
  // -- Add it to the local list, keying off the real block

  memoryBlock * real_block = property_block->container();

  real_block->add_to_flow_sensitive_list(_flow_sensitive_objects);
}

/** @brief Set flow sensitivity
 *
 * For a given real blocks, see if it's property block for this property
 * needs to be flow sensitive. Called by
 * Analyzer::lookup_property_block(). */

void enumPropertyAnn::set_flow_sensitivity(memoryBlock * real_block)
{
  // -- Set flow sensitivity on the property block

  memoryBlock * property_block = lookup_property_block(real_block);

  // -- Figure out how to set flow sensitivity

  if (Annotations::Adaptivity) {

    // -- Look the in the property-speicif flow-sensitive list for the real
    // block

    if (real_block->is_in_flow_sensitive_list(_flow_sensitive_objects)) {

      // -- Found it: make the property block flow sensitive

      property_block->set_flow_sensitive();
    }
    else {

      // -- Not found: inherit from the real_block

      if (real_block->is_flow_sensitive())
	property_block->set_flow_sensitive();
      else
	property_block->set_flow_insensitive();
    }
  }
  else {

    // -- Other modes...

    if (Annotations::Flow_insensitive_properties)
      property_block->set_flow_insensitive();
    else
      property_block->set_flow_sensitive();
  }
}
    

/** @brief Analyze one memory block
 *
 * */

void enumPropertyAnn::analyze_one_memoryblock(memoryBlock * real_block,
					      enumvalue_set & values,
					      block_propertyset_map & already_seen,
					      memoryblock_set & flow_sensitive,
					      string & indent)
{
  // -- Make sure it's flow insensitive

  if (real_block->is_flow_sensitive())
    return;

  // -- Check to see if it's already been seen: this keeps from getting
  // into infinite recursion on recursive data structures.

  block_propertyset_map_p f = already_seen.find(real_block);

  if (f != already_seen.end()) {

    // -- Already seen, so just get the values previously stored

    enumvalue_set & old_vals = (*f).second;

    meet_with(values, old_vals);

    return;
  }

  // -- Not there, so analyze it

  enumvalue_set local_values;

  // -- First, get its "ever" values

  memoryBlock * property_block = lookup_property_block(real_block);

  block_propertyset_map_p q = _ever_values.find(property_block);
  if (q != _ever_values.end()) {

    enumvalue_set & vals = (*q).second;

    meet_with(local_values, vals);

    /*
    cout << indent << real_block->name() << " = ";
    print_enumvalue_set(cout, vals);
    cout << endl;
    */
  }
  else {
    // cout << indent << real_block->name() << " = (no values)" << endl;
  }

  // -- Store the ever values for memoization

  already_seen[real_block] = local_values;

  // -- If it ever takes on multiple values, make it flow-sensitive

  if (local_values.size() > 1) {
    flow_sensitive.insert(real_block);
  }

  // -- Now follow any pointers

  memoryDef * def = real_block->only_def();
  if (def) {

    // -- Get the points-to set

    const memoryblock_set & points_to = def->points_to();

    if (points_to.size() > 1) {
      
      // -- Collect the values into a single set

      enumvalue_set collect;

      // -- Record if the different pointer targets point to different sets
      // of values.

      bool different_targets = false;

      // -- Visit each object it points to and collect its ever values

      bool first = true;
      
      string indent2 = indent + "  ";

      int min_count = 9999;

      for (memoryblock_set_cp p = points_to.begin();
	   p != points_to.end();
	   ++p)
	{
	  memoryBlock * target = *p;

	  enumvalue_set target_values;

	  // -- Call this function recursively to get values for the
	  // pointer target.

	  analyze_one_memoryblock(target, target_values, already_seen, flow_sensitive, indent2);

	  // -- Add the values into the collected set

	  meet_with(collect, target_values);

	  // -- Min count

	  int count = target_values.count();
	  if (count < min_count)
	    min_count = count;

	  first = false;
	}

      if (collect.count() > min_count)
	different_targets = true;

      // -- Make a pointer flow sensitive if it has more that one target, and
      // those targets take on different values.

      if (different_targets) {
	/*
	cout << indent << "  -> " << real_block->name() << " points-to ";
	print_enumvalue_set(cout, collect);
	cout << endl;
	*/

	flow_sensitive.insert(real_block);
      }

      // -- Pass these values back up

      meet_with(values, collect);

      // -- Save the collected values to memoize the computation
      /*
      cout << indent << "  From points-to: ";
      print_enumvalue_set(cout, collect);
      cout << endl;
      */

      meet_with(already_seen[real_block], collect);

      // bool xx = update_ever_value(property_block, collect);
    }
  }
}

/** @brief Track destructive assignments
 */

bool enumPropertyAnn::track_destructive_assignments(Location * target,
						    memoryBlock * block,
						    memoryblock_set & already_seen,
						    memoryblock_set & made_flow_sensitive,
						    string & indent)
{
  if (already_seen.find(block) != already_seen.end()) {

    // -- Already seen this block

    // -- Check to see if it was made flow sensitive, return the bool as
    // needed

    bool fs = (made_flow_sensitive.find(block) != made_flow_sensitive.end());

    if (Annotations::Show_adaptivity) {
      if (fs)
	cout << indent << "+ (Already made FS: block " << block->name() << ")" << endl;
      else
	cout << indent << "+ (Already seen: block " << block->name() << ")" << endl;
    }

    return fs;
  }

  already_seen.insert(block);

  if (Annotations::Show_adaptivity)
    cout << indent << "+ Block " << block->name() << endl;

  // -- Get the destructive assignment list

  const memoryBlock::destructive_assignment_map & destructive = block->destructive_assignments();

  // -- Make recommendations for any destructive assignments

  if (Annotations::Show_adaptivity)
    if (destructive.empty())
      cout << indent << " (No destructive assignments)" << endl;

  bool make_chain_flow_sensitive = false;

  for (memoryBlock::destructive_assignment_map_cp p = destructive.begin();
       p != destructive.end();
       ++p)
    {
      Location * where = (*p).first;
      memoryBlock::DestructiveKind kind = (*p).second;

      switch (kind) {

      case memoryBlock::Control_flow:
	{
	  if (Annotations::Show_adaptivity)
	    cout << indent << "  --> needs path sensitivity at " << *where << endl;

	  // -- Find and record the controlling branch

	  basicblockLocation * basicblock_loc = (basicblockLocation *) where;
	  basicblockNode * basicblock = basicblock_loc->block();

	  pointerOptions::Path_sensitive_branches.insert(basicblock);

	  // make_chain_flow_sensitive = true;
	}
	break;

      case memoryBlock::Parameter_pass:
	{
	  procLocation * procloc = 0;

	  // -- Make sure to handle external outputs correctly

	  if (where->kind() == Location::Statement) {

	    // -- External outputs are assigned at the call site

	    stmtLocation * callsite = (stmtLocation *) where;
	    procloc = callsite->calls();
	  }
	  else {

	    // -- Inputs 

	    procloc = Location::procedure(where);

	    procNode * proc = procloc->proc();
	    
	    if (Annotations::Show_adaptivity)
	      cout << indent << "  --> procedure " << proc->decl()->name() << " needs context-sensitivity;"
		   << " parameter passed at " << *where << endl;

	    bool make_context_sensitive = false;

	    // -- Test reachability
	    /*
	    bool reachable = true;

	    if (Annotations::Aggressive_pruning)
	      reachable = is_location_reachable(where, target, block);

	    if (Annotations::Show_adaptivity) {
	      if (reachable)
		cout << indent << "    [ " << * target << " is reachable from " << * where << " ]" << endl;
	      else
		cout << indent << "    [ " << * target << " is NOT reachable from " << * where << " ]" << endl;
	    }

	    if (reachable) {
	    */

	    // -- Retrieve the reaching defs that reach this call

	    if (Annotations::Show_adaptivity)
	      cout << indent << "    Callsites:" << endl;

	    const callsite_objects_map & assignments = block->parameter_assignments(proc);

	    // -- Validate the decision to make this procedure context sensitive

	    if (Annotations::Prune_context_sensitive) {

	      // -- Prune out procedures by re-checking their inputs to
	      // make sure what context sensitivity would yield additional
	      // accuracy.

	      /*
		if (block->decl()->decl_location() == declNode::FORMAL) {

		// -- For formal parameters, always make the procedure context sensitive
	      
		make_context_sensitive = true;

		if (Annotations::Show_adaptivity)
		cout << indent << "    ==> (Formal parameter: leave it context-insensitive)" << endl;
		}
		else {
	      */

	      // -- Check the parameter assignments: make sure that
	      // information really is lost.

	      if (assignments.size() <= 1) {
		
		// -- Special case: if there is only one callsite, then
		// context sensitivity cannot possibly help.

		make_context_sensitive = false;
		
		if (Annotations::Show_adaptivity)
		  cout << indent << "    ==> (One callsite: leave it context-insensitive)" << endl;
	      }
	      else {
		
		// -- Re-check for information loss depending on the kind
		// of block:

		if (block->property) {

		  // -- Property block

		  make_context_sensitive = validate_property_cs(block, assignments, indent);

		}
		else
		  if (block->is_allocation_object()) {

		    // -- Allocation object: check multiplicity

		    make_context_sensitive = validate_multiplicity_cs(block, assignments, indent);

		  }
		  else {

		    // -- Regular pointer object: count points-to sets

		    make_context_sensitive = validate_pointer_cs(block, assignments, already_seen, indent);
		  }
	      }
	    }
	    else
	      make_context_sensitive = true;

	      // -- Make the procedure context-sensitive

	      // make_chain_flow_sensitive = true;

	    if (make_context_sensitive)
	      pointerOptions::Context_sensitive_procedures.insert(proc->decl()->name());
	    else
	      Annotations::Pruned_procedures.insert(proc->decl()->name());
	  }
	}
	break;

      case memoryBlock::Weak_update:
	{
	  if (Annotations::Show_adaptivity)
	    cout << indent << "  --> needs lower multiplicity at " << *where << endl;

	  // -- If it's a heap object, make it flow-sensitive

	  make_chain_flow_sensitive = true;

	  if (block->is_heap_object()) {
	    memoryBlock * real_block = block;
	    if (block->property)
	      real_block = block->container();
	    real_block->add_to_flow_sensitive_list(pointerOptions::Flow_sensitive_objects);
	  }
	}
	break;

      case memoryBlock::Additive:

	if (Annotations::Show_adaptivity)
	  cout << indent << "  --> needs flow sensitivity at " << *where << endl;

	make_chain_flow_sensitive = true;

	break;

      default:
	if (Annotations::Show_adaptivity)
	  cout << indent << "  --> cause unknown at " << * where << endl;
      }
    }

  // -- Get the complicit assignment list

  const memoryBlock::complicit_assignment_map & complicit = block->complicit_assignments();
  
  // -- Visit the complicit objects

  if (Annotations::Show_adaptivity)
    if (complicit.empty())
      cout << indent << " (No complicit assignments)" << endl;

  string new_indent = indent + "    ";

  for (memoryBlock::complicit_assignment_map_cp p = complicit.begin();
       p != complicit.end();
       ++p)
    {
      Location * where = (*p).first;
      const memoryblock_set & blocks = (*p).second;

      if (Annotations::Show_adaptivity)
	cout << indent << "  - Complicit at " << *where << endl;

      for (memoryblock_set_cp q = blocks.begin();
	   q != blocks.end();
	   ++q)
	{
	  memoryBlock * comp = *q;

	  // -- Test reachability

	  bool reachable = true;

	  if (Annotations::Aggressive_pruning)
	    reachable = is_location_reachable(where, target, comp);

	  if (Annotations::Show_adaptivity) {
	    if (reachable)
	      cout << indent << "    [ " << * target << " is reachable from " << * where << " ]" << endl;
	    else
	      cout << indent << "    [ " << * target << " is NOT reachable from " << * where << " ]" << endl;
	  }

	  if (reachable) {
	    bool chain_flow_sensitivity = track_destructive_assignments(where, comp, already_seen, made_flow_sensitive, new_indent);
	    if (chain_flow_sensitivity)
	      make_chain_flow_sensitive = true;
	  }
	}
    }

  if (make_chain_flow_sensitive) {

    // -- Make the block flow-sensitive: this depends on whether it is a
    // property block or a real block.

    if (block->property) {

      // -- Make a property block flow sensitive. NOTE: it might not be
      // this property!

      block->property->add_flow_sensitive_object(block);
    }
    else {

      // -- Regular object: make it flow sensitive in the global list

      block->add_to_flow_sensitive_list(pointerOptions::Flow_sensitive_objects);
    }

    // -- Keep a list of objects that we make flow sensitive

    made_flow_sensitive.insert(block);
  }

  return make_chain_flow_sensitive;
}

/** @brief Record tested objects
 *
 * This method is called by the test functions to record which object are
 * tested. It also sets up any complicit assignments that start the
 * adaptation process. */

void enumPropertyAnn::record_tested_objects(Location * where,
					    pointerValue & ptr,
					    enumvalue_set & value_set,
					    memoryblock_set & complicit_property_blocks)
{
  if ((Annotations::Adaptivity == 3) &&
      Annotations::Record_tested_objects) {

    // -- Remember that these objects were tested

    for (memoryblock_set_p q = complicit_property_blocks.begin();
	 q != complicit_property_blocks.end();
	 ++q)
      {
	memoryBlock * property_block = *q;
	memoryBlock * real_block = property_block->container();

	_tested_objects[where].insert(real_block);
      }
    
    if (value_set.count() > 1) {

      // -- If constructing the value lost information, then record that fact

      for (memoryblock_set_p p = ptr.blocks.begin();
	   p != ptr.blocks.end();
	   ++p)
	{
	  memoryBlock * real_block = *p;
	  memoryBlock * property_block = lookup_property_block(real_block);
	  
	  property_block->add_complicit_assignment(where, ptr.dereferenced);
	}
    }
  }
}


/** @brief Trace object
 *
 * Trace the assignments to a property, generate a report. */

void enumPropertyAnn::trace_object(ostream & out,
				   memoryBlock * property_block,
				   memoryblock_set & already_seen,
				   string & indent)
{
  // -- Check for cycles

  if (already_seen.find(property_block) != already_seen.end()) {
    out << indent << " (already seen)" << endl;
    return;
  }

  already_seen.insert(property_block);

  // -- Get the destructive assignment list

  const memoryBlock::destructive_assignment_map & destructive = property_block->destructive_assignments();

  // -- Get the complicit assignment list

  const memoryBlock::complicit_assignment_map & complicit = property_block->complicit_assignments();
  
  // -- Visit all the defs and report
  
  const memorydef_list defs = property_block->defs();
  for (memorydef_list_cp mp = defs.begin();
       mp != defs.end();
       ++mp)
    {
      memorydef_location_pair mlp = (*mp);
      Location * where = mlp.first;
      memoryDef * def = mlp.second;
      
      enumvalue_set val = lookup_now_value(property_block, def);

      out << indent << " + ";

      // -- Was it destructive?

      memoryBlock::destructive_assignment_map_cp dp = destructive.find(where);
      if (dp != destructive.end()) {
	memoryBlock::DestructiveKind kind = (*dp).second;
	switch (kind) {
	case memoryBlock::Control_flow:   out << "[D: phi]";
	  break;
	case memoryBlock::Parameter_pass: out << "[D: par]";
	  break;
	case memoryBlock::Weak_update:    out << "[D: mul]";
	  break;
	case memoryBlock::Additive:       out << "[D: fi ]";
	  break;
	default:
	  break;
	}
      }

      // -- Print out the location and value

      out << " Def at " << * (def->where()) << " = " << to_string(val) << endl;

      // -- Was it complicit?

      memoryBlock::complicit_assignment_map_cp cp = complicit.find(where);
      if (cp != complicit.end()) {

	string new_indent = indent + "    ";

	const memoryblock_set & blocks = (*cp).second;

	for (memoryblock_set_p q = blocks.begin();
	     q != blocks.end();
	     ++q)
	  {
	    memoryBlock * cblock = (*q);

	    if (cblock->property == this) {
	      out << indent << "    = " << cblock->name() << endl;

	      trace_object(out, cblock, already_seen, new_indent);
	    }
	    else
	      out << indent << "    Complicit: " << cblock->name() << endl;
	  }
      }
    }
}

/** @brief Validate context sensitivity for property
 *
 * */

bool enumPropertyAnn::validate_property_cs(memoryBlock * block,
					   const callsite_objects_map & assignments,
					   string & indent)
{
  int min = 999999;
  enumValueAnn * highest_value = 0;
  
  enumvalue_set collected_vals;

  if (Annotations::Show_adaptivity)
    cout << indent << "    Property validation for block " << block->name() << endl;

  // -- Visit each callsite 
		  
  for (callsite_objects_map_cp rdp = assignments.begin();
       rdp != assignments.end();
       ++rdp)
    {
      stmtLocation * callsite = (*rdp).first;
      procLocation * attach_to = callsite->calls();

      const memoryblock_set & reaching_blocks = (*rdp).second;

      // -- Collect all the values that reach this call site. There may be
      // multiple reaching definitions, so we just combine them

      enumvalue_set vals;

      for (memoryblock_set_cp mdp = reaching_blocks.begin();
	   mdp != reaching_blocks.end();
	   ++mdp)
	{
	  memoryBlock * property_block = (*mdp);

	  property_block->set_current_def_use(attach_to);
	  enumvalue_set local_val = construct_now_value(attach_to, property_block, false);

	  meet_with(vals, local_val);

	  if (Annotations::Show_adaptivity) {
	    cout << indent << "      Callsite " << * callsite
		 << "  block " << property_block->name()
		 << " = " <<  property_block->property->to_string(local_val) << endl;
	  }		      
	}

      // -- Add the values from this callsite into the collected set
		      
      meet_with(collected_vals, vals);
		      
      // -- Keep track of the callsite with the best information
      
      int count = vals.count();
      if ((count > 0) &&
	  (count < min)) min = count;

      // -- Special rule for total orders:

      if (_total_order) {

	// -- Merge the values together using the "real" meet function
	  
	enumValueAnn * merged_val = merge_enumvalue_set(vals);

	// -- Keep track of the best value we see

	if ( ! highest_value )
	  highest_value = merged_val;
	else {
	  
	  if ((merged_val != highest_value) &&
	      at_least(merged_val, highest_value))
	    highest_value = merged_val;
	}
      }
    }
		  
  if (Annotations::Show_adaptivity)
    cout << indent << "    ==> Property " << block->property->name() 
	 << " : Min = " << min << " , Vals = " 
	 << to_string(collected_vals) << endl;

  bool result;
  
  if (_total_order) {

    enumValueAnn * merged_val = merge_enumvalue_set(collected_vals);

    if (merged_val != highest_value)
      result = true;
    else
      result = false;
  }
  else {

    if (collected_vals.count() > min)
      result = true;
    else
      result = false;
  }

  if (Annotations::Show_adaptivity) {
    if (result)
      cout << indent << "      -> Validated" << endl;
    else
      cout << indent << "      -> Prune" << endl;
  }

  return result;
}

/** @brief Validate context sensitivity for multiplicity
 *
 * */

bool enumPropertyAnn::validate_multiplicity_cs(memoryBlock * block,
					       const callsite_objects_map & assignments,
					       string & indent)
{
  bool any_single = false;
  bool any_unbounded = false;

  if (Annotations::Show_adaptivity)
    cout << indent << "    Multiplicity validation for block " << block->name() << endl;

  for (callsite_objects_map_cp rdp = assignments.begin();
       rdp != assignments.end();
       ++rdp)
    {
      stmtLocation * callsite = (*rdp).first;
      procLocation * attach_to = callsite->calls();
      const memoryblock_set & reaching_blocks = (*rdp).second;

      // -- Collect all the values that reach this call
      // site: for multiplicity, this should be only one.

      Multiplicity reaching_multiplicity = Unallocated;

      for (memoryblock_set_cp mdp = reaching_blocks.begin();
	   mdp != reaching_blocks.end();
	   ++mdp)
	{
	  memoryBlock * reaching_block = (*mdp);

	  reaching_block->set_current_def_use(attach_to);

	  memoryUse * use = reaching_block->current_use();
	  if (use &&
	      use->reaching_def()) {

	    memoryDef * reaching_def = use->reaching_def();

	    if (Annotations::Show_adaptivity) {
	      cout << indent << "      Callsite " << * callsite;
	      if (reaching_def)
		cout << " reached by " << * (reaching_def->where()) << endl;
	      else
		cout << " (no reaching def)" << endl;
	    }
	    
	    reaching_multiplicity = reaching_def->multiplicity();
	    
	    if (reaching_multiplicity == Unbounded)
	      any_unbounded = true;

	    if (reaching_multiplicity == Single)
	      any_single = true;
	  }
	}
    }
  
  if (Annotations::Show_adaptivity)
    cout << indent << "    ==> Any single = " << any_single 
	 << ", any unbounded = " << any_unbounded << endl;

  bool result;

  if (any_single && any_unbounded)
    result = true;
  else
    result = false;

  if (Annotations::Show_adaptivity) {
    if (result)
      cout << indent << "      -> Validated" << endl;
    else
      cout << indent << "      -> Prune" << endl;
  }

  return result;
}

/** @brief Validate context sensitivity for regular pointer variable
 *
 * */

bool enumPropertyAnn::validate_pointer_cs(memoryBlock * block,
					  const callsite_objects_map & assignments,
					  memoryblock_set & already_seen,
					  string & indent)
{
  // memoryblock_set collected_points_to;
  enumvalue_set collected_vals;

  int min_size = 99999;
  int min = 99999;
  enumValueAnn * highest_value = 0;

  if (Annotations::Show_adaptivity)
    cout << indent << "    Points-to validation for block " << block->name() << endl;

  // memoryblock_set flow_insensitive;

  for (callsite_objects_map_cp rdp = assignments.begin();
       rdp != assignments.end();
       ++rdp)
    {
      stmtLocation * callsite = (*rdp).first;
      procLocation * attach_to = callsite->calls();

      const memoryblock_set & reaching_blocks = (*rdp).second;

      // -- Add the values from this callsite into the collected set
		      
      enumvalue_set vals = reachable_values(callsite, reaching_blocks, already_seen);

      // -- Special rule for total orders:

      if (_total_order) {

	// -- Merge the values together using the "real" meet function
	  
	enumValueAnn * merged_val = merge_enumvalue_set(vals);

	// -- Keep track of the best value we see

	if ( ! highest_value )
	  highest_value = merged_val;
	else {
	  
	  if ((merged_val != highest_value) &&
	      at_least(merged_val, highest_value))
	    highest_value = merged_val;
	}
      }

      meet_with(collected_vals, vals);

      if (Annotations::Show_adaptivity) {
	cout << indent << "      Callsite " << * callsite
	     << " : [ Values = " << to_string(vals) << " -- blocks = ";
	for (memoryblock_set_cp mbp = reaching_blocks.begin();
	     mbp != reaching_blocks.end();
	     ++mbp)
	  cout << (*mbp)->name() << " ";
	cout << "]" << endl;
      }

      // -- Keep track of the callsite with the best information
      
      int count = vals.count();
      if ((count > 0) &&
	  (count < min)) min = count;
    }

  /*
  if (Annotations::Show_adaptivity)
    cout << indent << "    ==> Min points-to = " << min_size
	 << ", collected points-to = " << collected_points_to.size() << endl;
  */

  if (Annotations::Show_adaptivity)
    cout << indent << "    ==> [ Values = " << to_string(collected_vals) << " ]" << endl;

  bool result;

  /*
  if ( ! flow_insensitive.empty()) {

    if (Annotations::Show_adaptivity)
      cout << indent << "    ==> Found " << flow_insensitive.size()
	   << " flow insensitive inputs" << endl;

    for (memoryblock_set_p p = flow_insensitive.begin();
	 p != flow_insensitive.end();
	 ++p)
      {
	memoryBlock * fi_block = *p;
	
	fi_block->add_to_flow_sensitive_list(pointerOptions::Flow_sensitive_objects);
      }

    result = false;
  }
  else {
  */

  if (_total_order) {

    enumValueAnn * merged_val = merge_enumvalue_set(collected_vals);

    if (merged_val != highest_value)
      result = true;
    else
      result = false;
  }
  else {

    if (collected_vals.count() > min)
      result = true;
    else
      result = false;
  }

  /*
  if ((min_size > 0) &&
      (collected_points_to.size() > min_size))
    result = true;
  else
    result = false;
  */

  if (Annotations::Show_adaptivity) {
    if (result)
      cout << indent << "      -> Validated" << endl;
    else
      cout << indent << "      -> Prune" << endl;
  }

  return result;
}

bool enumPropertyAnn::is_location_reachable(Location * source, Location * target, memoryBlock * block)
{
  memoryDef * source_def = block->find_def_at(source);
  memoryDef * target_def = block->nearest_def_at(target);

  if ( ! source_def  || ! target_def )
    return true;

  if (source_def == target_def)
    return true;

  memorydef_set defs;
  memoryuse_set uses;

  memorydef_set already_seen;

  bool not_done = true;

  defs.insert(target_def);

  while (not_done) {

    // -- Assume we're done

    not_done = false;

    // -- Visit all the defs and collect uses at the same location
    // (presumably, the right-hand sides)

    for (memorydef_set_p p = defs.begin();
	 p != defs.end();
	 p++)
      {
	memoryDef * cur_def = *p;

	// -- Find all the right-hand-side uses

	block->find_uses_at(cur_def->where(), uses);

	memoryUse * use = cur_def->self_assign_use();
	if (use)
	  uses.insert(use);
      }
    
    // -- Clear the set of defs

    defs.clear();

    // -- For each use, get it's reaching definition

    for (memoryuse_set_p w = uses.begin();
	 w != uses.end();
	 ++w)
      {
	memoryUse * cur_use = *w;
	memoryDef * cur_def = cur_use->reaching_def();

	if (cur_def) {

	  // -- If this is the def we're looking for, then exit

	  if (cur_def == source_def)
	    return true;

	  if (already_seen.find(cur_def) == already_seen.end()) {

	    // -- Found a new def, keep going

	    already_seen.insert(cur_def);

	    defs.insert(cur_def);

	    not_done = true;
	  }
	}
      }

    // -- Clear the uses and continue

    uses.clear();
  }

  // -- None of the defs reaches the targets

  return false;
}

enumvalue_set enumPropertyAnn::reachable_values(stmtLocation * where,
						const memoryblock_set & blocks,
						memoryblock_set & already_seen)
{
  enumvalue_set result;

  memoryblock_list worklist;
  memoryblock_set found;

  for (memoryblock_set_cp p = blocks.begin();
       p != blocks.end();
       ++p)
    {
      worklist.push_back(*p);
    }

  // -- First, find all the blocks reachable from this one...

  while ( ! worklist.empty()) {

    // -- Remove the first element from the worklist..

    memoryBlock * mb = worklist.front();
    worklist.pop_front();

    // -- Add all the immediately reachable blocks (that we haven't seen yet)
    
    mb->reachable_blocks(where, true, worklist, found, (memoryBlock *)0);
  }

  // -- Include the blocks themselves

  found.insert(blocks.begin(),
	       blocks.end());

  // -- Collect all the property blocks...

  memoryblock_set property_blocks;

  for (memoryblock_set_p p = found.begin();
       p != found.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {

	// -- Make sure this is a block we care about

	// memoryblock_set_p found = already_seen.find(property_block);

	//if (found != already_seen.end())

	property_blocks.insert(property_block);
      }
    }

  // -- Check for no objects, return default
  /*
  if (property_blocks.empty()) {
    
    if (default_val() != top())
      result = enumvalue_set(default_val());

    return result;
  }
  */

  // -- Collect all the values

  for (memoryblock_set_p p = property_blocks.begin();
       p != property_blocks.end();
       ++p)
    {
      memoryBlock * property_block = *p;

      property_block->set_current_def_use(where);
      enumvalue_set local_val = construct_now_value(where, property_block, false);

      meet_with(result, local_val);
    }

  return result;
}

