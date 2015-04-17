
#ifndef BDWY_ENUM_PROPERTY_H
#define BDWY_ENUM_PROPERTY_H

typedef list< enumValueAnn * > enumvalue_list;
typedef enumvalue_list::iterator enumvalue_list_p;
typedef enumvalue_list::const_iterator enumvalue_list_cp;

typedef map< string, enumPropertyAnn *> enum_property_map;
typedef enum_property_map::iterator enum_property_map_p;
typedef enum_property_map::const_iterator enum_property_map_cp;

/** @brief A dataflow analysis lattice element
 *
 */

class enumValueAnn : public Ann
{
private:

  /** @brief Name of the value */

  string _name;

  /** @brief Pointer to the lower lattice element */

  enumValueAnn * _more_general;

  /** @brief List of higher lattice elements  */

  enumvalue_list _more_specific;

  /** @brief Height in the lattice */

  int _height;

  /** @brief Unique id number */

  int _id;

  /** @brief Is leaf
   *
   * True if the only value higher in the lattice is TOP. */

  bool _is_leaf;

public:

  /** @brief Create a new property value */

  enumValueAnn(const parserID * name,
		   enumvalue_list * more_specific);

  // Fields

  inline string & name() { return _name; }
  inline const string & name() const { return _name; }

  inline enumValueAnn * more_general() const { return _more_general; }

  inline enumvalue_list & more_specific() { return _more_specific; }
  inline const enumvalue_list & more_specific() const { return _more_specific; }

  inline int height() const { return _height; }
  inline void height(int new_height) { _height = new_height; }

  inline int id() const { return _id; }
  inline void id(int new_id) { _id = new_id; }

  inline bool is_leaf() const { return _is_leaf; }

  /** @brief Collect atleast values
   *
   * Collect all the values that are at least as specific as this one,
   * excluding top. */

  void collect_atleast_values(enumvalue_set & collect);
};

// ----------------------------------------------------------------------
//  Enum Value Set
// ----------------------------------------------------------------------

#define MAX_ENUMVALUES 32

typedef bitset< MAX_ENUMVALUES > bitset32;

class enumvalue_set : public  bitset32
{
public:

  /** @brief Construct an empty set */

  enumvalue_set()
    : bitset32()
  {}

  /** @brief Construct a single element set
   *
   * Contains the single given element */

  enumvalue_set(enumValueAnn * val)
    : bitset32()
  {
    set(val->id());
  }

  /** @brief Insert into the set
   */

  inline void insert(enumValueAnn * val) { set(val->id()); }

  /** @brief Inclusion test
   *
   * Test to see if the input set is wholly included in this set. The test
   * is basically to see if the this intersected with other equals
   * other. */

  inline bool includes(enumvalue_set & other) {
    enumvalue_set temp(*this);
    temp &= other;
    return (temp != other);
  }

  /** @brief Set union */

  inline void set_union(const enumvalue_set & other) { (*this) |= other; }

  /** @brief Set intersection */

  inline void set_intersection(const enumvalue_set & other) { (*this) &= other; }
};

// ----------------------------------------------------------------------
//  Dataflow analysis property
// ----------------------------------------------------------------------

/** @brief A dataflow analysis property
 *
 */

class enumPropertyAnn : public propertyAnn
{
public:

  typedef vector< enumValueAnn *> enumvalue_vec;
  typedef enumvalue_vec::iterator enumvalue_vec_p;
  typedef enumvalue_vec::const_iterator enumvalue_vec_cp;

  typedef map< memoryAccess *, enumvalue_set> defuse_property_map;
  typedef defuse_property_map::iterator defuse_property_map_p;

  typedef map< memoryBlock *, enumvalue_set > block_propertyset_map;
  typedef block_propertyset_map::iterator block_propertyset_map_p;

  typedef pair< int, double > count_accuracy_pair;
  typedef map< procLocation *, count_accuracy_pair > accuracy_map;
  typedef accuracy_map::iterator accuracy_map_p;

  typedef map< Location *, memoryblock_set > tested_objects_map;
  typedef tested_objects_map::iterator tested_objects_map_p;

  typedef map< stmtNode *, count_accuracy_pair > stmt_accuracy_map;
  typedef stmt_accuracy_map::iterator stmt_accuracy_map_p;

private:

  /** @brief May or must property
   *
   * When true, the sets of value are combined using union, making the
   * property a "may" property. When false, the sets are combined using
   * intersection, making this a "must" property. */

  bool _is_may_property;

  /** @brief Strong or weak
   *
   * We can have properties that enforce weak updates, or not. It's up to
   * the user to make sure that these are correct. */

  bool _enforce_weak_updates;

  /** @brief Pointer to the lattice elements
   *
   *  This pointer actually refers to the "bottom" element. All other
   *  elements are reachable by following the "more_specific" pointers. */

  TREE enumValueAnn * _values;

  /** @brief Lattice top */

  TREE enumValueAnn * _top;

  /** @brief Default value
   *
   * This is the value we'll use whenever there isn't a reaching definition
   * (or use, depending on the direction). If none is provided, then TOP is
   * used. */

  REF enumValueAnn * _default;

  /** @brief Lattice elements ordered by index */

  REF enumvalue_vec _value_index;

  /** @brief Flow-sensitive "must" values */

  defuse_property_map _now_values;

  /** @brief Flow-insensitive "may" values */

  block_propertyset_map _ever_values;

  /** @brief Objects that need flow-sensitivity
   *
   * Store a list of the real blocks that need flow sensitivity for this
   * property. */

  flow_sensitive_set _flow_sensitive_objects;

  /** @brief Tested objects
   *
   * For trace-back adaptivity, keep track of all objects whose value is
   * tested. */

  tested_objects_map _tested_objects;

  /** @brief Accuracy measurements
   *
   * Store information about the accuracy of flow values at locations where
   * the property is tested. We can use this information to compute an
   * aggregate accuracy for the property. */

  accuracy_map _accuracy;

  /** @brief Error statements
   *
   * Keep a list of the statements that have errors that depend on this
   * property. */

  stmt_set _error_statements;

  /** @brief Number of flow values
   *
   * The number of possible flow values, exluding top and bottom. */

  double _number_of_flowvalues;

  /** @brief Total order
   *
   * When this boolean is true, the property is a total order. In this
   * case, we use a special rule when updating the now values. */

  bool _total_order;

public:

  /** @brief Create a new property */

  enumPropertyAnn(const parserID * name,
		  Direction direction,
		  bool enforce_weak_updates,
		  const parserID * default_name,
		  enumvalue_list * lowest_values);

  /** @brief Clear the analysis information
   *
   * Call this method to re-initialize the analysis information so that the
   * property analyzer can be run again. */

  void clear();

  /** @brief Query whether it enforces weak updates */

  inline bool enforce_weak_updates() const { return _enforce_weak_updates; }

  /** @brief Lattice */

  inline enumValueAnn * values() const { return _values; }

  /** @brief Lookup a value by name */

  enumValueAnn * lookup(const string & name);

  /** @brief Return the bottom element */

  inline enumValueAnn * bottom() const { return _values; }

  /** @brief Return the top element */

  inline enumValueAnn * top() const { return _top; }

  /** @brief Return the default element */

  inline enumValueAnn * default_val() const { return _default; }

  /** @brief Meet function
   *
   * This version is for meeting individual elements. */

  enumValueAnn * meet(enumValueAnn * one,
		      enumValueAnn * two);

  /** @brief Set meet function
   *
   * Depending on whether this is a "may" or "must" property. Side-effects
   * the value of the first parameter. */

  void meet_with(enumvalue_set & first,
		 const enumvalue_set & second);

  /** @brief At-least function
   *
   * Return true if left >= right ( left ^ right == right). Includes the
   * special case that if right is top, then return false. */

  bool at_least(enumValueAnn * left,
		enumValueAnn * right);

  /** @brief At-most function
   *
   * Return true if left <= right ( left ^ right == left) */

  bool at_most(enumValueAnn * left,
	       enumValueAnn * right);

  /** @brief Look up a sub-block
   *
   * Given a real memoryBlock, look up the corresponding sub-block for this
   * property. Returns null if none exists. */

  memoryBlock * lookup_property_block(memoryBlock * real_block);

  /** @brief Add a sub-block
   *
   * This method is called by the analyzer during pointer analysis to tell
   * the property about the sub-blocks that it's creating for it. */

  void add_property_block(memoryBlock * real_block, memoryBlock * property_block);

  /** @brief Test two variables
   *
   *  Test a particular property condition between two variables. */

  bool test(Broadway::Operator op, Broadway::FlowSensitivity flow_sensitivity,
	    Location * where,
	    pointerValue & lhs, string & lhs_name,
	    pointerValue & rhs, string & rhs_name);

  /** @brief Test one variable
   *
   *  Apply the specified test operator to the given variable and property
   *  value. */

  bool test(Broadway::Operator op, Broadway::FlowSensitivity flow_sensitivity,
	    Location * where,
	    pointerValue & lhs, string & lhs_name,
	    enumvalue_set rhs_value_set);

  /** @brief Property test
   *
   *  Apply the specified test operator to the two given property values. */

  bool test(Broadway::Operator op,
	    enumvalue_set lhs_value_set, enumvalue_set rhs_value_set);

  /** @brief Lookup flow-sensitive "now" value
   *
   * Defaults to "top" */

  enumvalue_set lookup_now_value(memoryBlock * property_block,
				  memoryAccess * def_or_use);

  /** @brief Set flow-sensitive "now" value
   *
   * We don't store "top" values, because the lookup method above defaults
   * to "top". Return true if this changed the value. */

  bool update_now_value(Location * where,
			memoryBlock * property_block,
			memoryAccess * def_or_use,
			enumvalue_set value,
			bool & lost_information);

  /** @brief Set flow-insensitive must "ever" value
   *
   * We don't store "top" values, because the lookup method above defaults
   * to "top". Return true if this changed the value. */

  bool update_ever_value(memoryBlock * property_block, enumvalue_set values);

  /** @name Value construction methods */
  //@{

  /** @brief Construct flow-sensitive "now" value
   *
   * For each memoryblock in the pointer value, get the property value
   * associated with the most recent dominating def. Meet all of these
   * values together. If there is no reaching def/use we return the default
   * property value. */

  enumvalue_set construct_now_value(Location * where,
				    pointerValue & variable,
				    string & variable_name,
				    bool & lost_information,
				    memoryblock_set & complicit_property_blocks);

  /** @brief Construct flow-sensitive "now" value
   *
   * 
   * Same method as above, but for a single memoryBlock. Also, if there is
   * no reaching def/use, return null. */

  enumvalue_set construct_now_value(Location * where,
				    memoryBlock * real_block,
				    bool default_to_top);

  /** @brief Construct flow-sensitive "after now" value
   *
   * For each memoryblock in the pointer value, get the property value
   * associated with modifications in the transfer functions (defs at this
   * location). Meet all these values together. */

  enumvalue_set construct_after_value(pointerValue & variable, string & name);

  /** @brief Construct flow-sensitive "after now" value
   *
   * Same method as above, but for a single memoryBlock. Also, if there is
   * no reaching def/use, return null. */

  enumvalue_set construct_after_value(memoryBlock * real_block);

  /** @brief Construct flow-insensitive may "ever" value
   *
   * For each memoryblock in the pointer value, union the sets of property
   * values together. */

  enumvalue_set construct_ever_value(pointerValue & variable, string & name);

  /** @brief Construct weak value
   *
   * During backwards analysis, when we construct the value for the LHS, we
   * need to take into account the previous value of objects that were
   * weakly updated. Therefore, this is a special version of
   * construct_now_value() that meets the now value with the after value
   * for weakly updated blocks. */

  enumvalue_set construct_weak_now_value(Location * where,
					 pointerValue & variable, string & name);

  //@}

  /** @brief Compute next state
   *
   * This is called in enumPropertyExprAnn::compute_next(), but it's
   * basically just a wrapper around contruct_now_value(), with the
   * addition of the assignment management. */

  enumvalue_set compute_next(Location * where,
			     ruleAnn * rule,
			     exprAnn * expr,
			     pointerValue & right,
			     string & right_name,
			     bool & rhs_lost_information,
			     memoryblock_set & complicit_property_blocks,
			     enumvalue_set & save_ever_values);

  /** @brief Change state
   *
   *  Update the state of the given variable to have the given property
   *  value. This method updates all three kinds of flow information. Put
   *  any changes into the changes set (NOTE: these will be the property
   *  blocks, not the real blocks). */

  void apply_next(Location * where,
		  stmtLocation * parameter_callsite,
		  ruleAnn * rule,
		  enumPropertyExprAnn * expr,
		  pointerValue & left,
		  string & left_name,
		  pointerValue & right,
		  enumvalue_set new_value,
		  bool rhs_lost_information,
		  memoryblock_set & complicit_property_blocks,
		  enumvalue_set ever_values,
		  memoryblock_set & changes);

  /** @brief Apply merge
   *
   * At a control-flow merge, get the reaching "now" values for the given
   * object and merge them together. Return true if that results in a
   * change. */

  void apply_merge(Location * where,
		   memoryBlock * property_block, memoryuse_list & phi_uses,
		   memoryblock_set & changes);

  /** @brief Apply assignment
   *
   * At a normal assignment, build the "now" value for the right-hand side,
   * and assign it to the left-hand side. We'll also handle weak updates by
   * calling the weak_update() method below. */

  void apply_assignment(Location * where,
			stmtLocation * parameter_callsite,
			pointerValue & left, pointerValue & right,
			bool is_parameter,
			memoryblock_set & changes);

  /** @brief Self assignment
   *
   * Now that properties are associated with special property blocks, we
   * need to explicitly handle self-assignment caused by passing parameters
   * and external inputs and outputs. */

  void self_assignment(Location * source,
		       Location * target,
		       memoryBlock * property_block, 
		       memoryblock_set & changes);

  /** @brief Conservative procedure call
   *
   * Record a conservative procedure call, primarily for back-trace
   * purposes. */

  void conservative_procedure_call(stmtLocation * current,
				   pointerValue & reachable,
				   memoryblock_set & changes);

  /** @brief Report
   *
   * Retrieve the value for the given variable and flow-sensitivity mode,
   * print out the results. */

  void report(ostream & out,
	      bool is_error, 
	      procLocation * where,
	      Broadway::FlowSensitivity flow_sensitivity,
	      pointerValue & lhs, string & lhs_name);

  /** @brief Output operator */

  friend ostream& operator<<(ostream & o, const enumPropertyAnn & anns) {
    anns.print(o);
    return o;
  }

  /** @brief Precision analysis
   *
   * Post-process the results of analysis to determine a list of variables
   * to make flow sensitive. */

  void precision_analysis(memoryModel * memory_model,
			  memoryblock_set & flow_sensitive);

  /** @brief NEW Precision analysis
   */

  void precision_analysis();

  /** @brief Compute accuracy
   *
   * Print out accuracy information and return overall value. */

  double compute_accuracy(Analyzer * analyzer);

  /** @brief Add flow-sensitive object
   *
   * Record that in the future, this property block should be flow
   * sensitive. */

  void add_flow_sensitive_object(memoryBlock * property_block);

  /** @brief Set flow sensitivity
   *
   * For a given real blocks, see if it's property block for this
   * property. Called by Analyzer::lookup_property_block() */

  void set_flow_sensitivity(memoryBlock * real_block);

  /** @brief Count flow sensitive objects
   */

  inline int count_flow_sensitive_objects() { return _flow_sensitive_objects.size(); }

  /** @brief Output method */

  void print(ostream & o) const;  

  /** @brief Merge enumvalue_set
   *
   * Visit the values in an enumvalue_set and merge them into a single
   * value using the meet function. */

  enumValueAnn * merge_enumvalue_set(enumvalue_set value_set);

  /** @brief Convert enum values to string */

  string to_string(enumvalue_set value_set);

private:

  /** @brief Print out this property */

  void print(ostream & o, enumValueAnn * prop, int depth) const;

  /** @brief Number values */

  void number_values(enumValueAnn * prop,
		     int & cur_index, int height, int & max_height);

  /** @brief OLD Variable trace
   *
   * Trace the value of this variable through all of it's defs. */

  void old_trace_variable(ostream & out,
			  memoryBlock * block);

  /** @brief Analyze one memory block
   *
   * */

  void analyze_one_memoryblock(memoryBlock * block,
			       enumvalue_set & values,
			       block_propertyset_map & already_seen,
			       memoryblock_set & flow_sensitive,
			       string & indent);

  /** @brief Track destructive assignments
   */

  bool track_destructive_assignments(Location * where,
				     memoryBlock * block,
				     memoryblock_set & already_seen,
				     memoryblock_set & made_flow_sensitive,
				     string & indent);

  /** @brief Record tested objects
   *
   * This method is called by the test functions to record which object are
   * tested. It also sets up any complicit assignments that start the
   * adaptation process. */

  void record_tested_objects(Location * where,
			     pointerValue & ptr,
			     enumvalue_set & value_set,
			     memoryblock_set & complicit_property_blocks);

  /** @brief Trace object
   *
   * Trace the assignments to a property, generate a report. */

  void trace_object(ostream & out,
		    memoryBlock * property_block,
		    memoryblock_set & already_seen,
		    string & indent);

  /** @brief Validate context sensitivity for property
   *
   * */

  bool validate_property_cs(memoryBlock * block,
			    const callsite_objects_map & assignments,
			    string & indent);

    /** @brief Validate context sensitivity for multiplicity
   *
   * */

  bool validate_multiplicity_cs(memoryBlock * block,
				const callsite_objects_map & assignments,
				string & indent);

  /** @brief Validate context sensitivity for regular pointer variable
   *
   * */

  bool validate_pointer_cs(memoryBlock * block,
			   const callsite_objects_map & assignments,
			   memoryblock_set & already_seen,
			   string & indent);

  /** @brief Is location reachable?
   *
   * Check to see if a def of the block at the source could reach the
   * target. */

  bool is_location_reachable(Location * source, Location * target, memoryBlock * block);

  /** @brief Reachable values
   *
   * Find all the reachable property blocks and gather their values. */
  
  enumvalue_set reachable_values(stmtLocation * where,
				 const memoryblock_set & blocks,
				 memoryblock_set & already_seen);
};

#endif /* BDWY_ENUM_PROPERTY_H */
