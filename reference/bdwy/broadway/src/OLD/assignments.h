// ----------------------------------------------------------------------
//  Property Assignment Management
// ----------------------------------------------------------------------

class propertyAssignment
{
public:

  typedef enum { Statement, TransferFunction, Merge, Parameter, Conservative } Kind;

private:

  /** @brief Location of the assignment */

  Location * _where;

  /** @brief Kind */

  Kind _kind;

  /** @brief The property */

  enumPropertyAnn * _property;

  /** @brief The right-hand side, which is a set of uses */

  memoryuse_set _uses;

  /** @brief The left-hand side, which is a set of defs */

  memorydef_set _defs;

  /** @brief The procedure (if there is one) */

  procedureAnn * _procedure;

  /** @brief The analysis rule that cause the assignment (if there is one) */

  ruleAnn * _rule;

  /** @brief The expression within the rule that specifies the assignment */

  exprAnn * _expr;

public:

  /** @brief Create a new assignment */

  propertyAssignment(Location * where, Kind kind,
		     enumPropertyAnn * property,
		     procedureAnn * procedure,
		     ruleAnn * rule,
		     exprAnn * expr)
    : _where(where),
      _kind(kind),
      _property(property),
      _uses(),
      _defs(),
      _procedure(procedure),
      _rule(rule),
      _expr(expr)
  {}

  /** @brief Location of the assignment */

  inline Location * where() const { return _where; }

  /** @brief Kind */

  inline Kind kind() const { return _kind; }

  /** @brief Uses */

  inline const memoryuse_set & uses() const { return _uses; }

  /** @brief Defs */

  inline const memorydef_set & defs() const { return _defs; }

  /** @brief Rule */

  inline ruleAnn * rule() const { return _rule; }

  /** @brief Expression */

  inline exprAnn * expr() const { return _expr; }

  /** @brief Add a def */

  inline void add_def(memoryDef * def) { _defs.insert(def); }

  /** @brief Add a use */

  inline void add_use(memoryUse * use) { _uses.insert(use); }

  /** @brief Set kind to conservative */

  inline void set_conservative() { _kind = Conservative; }

};

typedef set< propertyAssignment *> assignment_set;
typedef assignment_set::iterator assignment_set_p;

class propertyAssignmentSet
{
public:

  typedef map< Location *, propertyAssignment *> assignment_map;
  typedef assignment_map::iterator assignment_map_p;

  typedef pair< Location *, memoryBlock *> location_block_pair;
  typedef map< location_block_pair, propertyAssignment * > special_assignment_map;
  typedef special_assignment_map::iterator special_assignment_map_p;

  typedef pair< Location *, exprAnn *> location_expr_pair;
  typedef map< location_expr_pair, propertyAssignment * > rule_assignment_map;
  typedef rule_assignment_map::iterator rule_assignment_map_p;

  typedef map< memoryDef *, propertyAssignment * > def_assignment_map;
  typedef def_assignment_map::iterator def_assignment_map_p;

private:

  /** @brief The property */

  enumPropertyAnn * _property;

  /** @brief Property assignments
   *
   * This structure records all of the assignments of the given
   * property. They are indexed according to the location at which they
   * occur. */

  assignment_map _assignments;

  /** @brief Special property assignments
   *
   * This structure holds all of the assignments of properties that occur
   * outside regular assignment statements, such as merges, parameters
   * passing (indexed by the left-hand side variable), and self-assignment
   * (external inputs and outputs). */

  special_assignment_map _special_assignments;

  /** @brief Rule-triggered property assignments
   *
   * This structure holds the state changes and assignments that occur in
   * transfer function annotations. */

  rule_assignment_map _rule_assignments;

  /** @brief Map from defs back to assignments
   *
   * This maps defs of the property blocks back to the assignments so that
   * we can trace the state of the variable backwards. */

  def_assignment_map _def_assignments;

public:

  propertyAssignmentSet(enumPropertyAnn * property)
    : _property(property),
      _assignments(),
      _special_assignments(),
      _rule_assignments(),
      _def_assignments()
  {}

  /** @brief Destructor
   *
   * Delete all of the assignments. */

  ~propertyAssignmentSet();

  /** @brief Clear
   *
   * Delete the assignments. */

  void clear();

  /** @brief Lookup regular assignment
   *
   * Create it if it's not there */

  propertyAssignment * lookup_regular_assignment(Location * where);

  /** @brief Lookup special assignment
   *
   * Create it if it's not there */

  propertyAssignment * lookup_special_assignment(Location * where,
						 memoryBlock * property_block,
						 propertyAssignment::Kind kind);

  /** @brief Lookup rule assignment
   *
   * Create it if it's not there */

  propertyAssignment * lookup_rule_assignment(Location * where,
					      exprAnn * expr,
					      ruleAnn * rule);
  /** @brief Add a def
   *
   * Update the map from defs to assignments */

  void add_def(propertyAssignment * assignment,
	       memoryDef * def);

  /** @brief Add a use */

  void add_use(propertyAssignment * assignment,
	       memoryUse * use);


  /** @brief Trace assignment
   *
   * Find the assignment to which a def belongs. */

  propertyAssignment * trace_def(memoryDef * def);
};

// ---------------------------------------------------------------------
//  Assignments
// ---------------------------------------------------------------------

/** @brief Destructor
 *
 * Delete all of the assignments. */

propertyAssignmentSet::~propertyAssignmentSet()
{
  clear();
}

/** @brief Clear
 *
 * Delete the assignments. */

void propertyAssignmentSet::clear()
{
  // -- Delete all the regular assignments

  for (assignment_map_p p = _assignments.begin();
       p != _assignments.end();
       ++p)
    delete (*p).second;

  _assignments.clear();

  // -- Delete all the special assignments

  for (special_assignment_map_p p = _special_assignments.begin();
       p != _special_assignments.end();
       ++p)
    delete (*p).second;

  _special_assignments.clear();

  // -- Delete all the rule assignments

  for (rule_assignment_map_p p = _rule_assignments.begin();
       p != _rule_assignments.end();
       ++p)
    delete (*p).second;

  _rule_assignments.clear();
}

/** @brief Lookup regular assignment
 *
 * Create it if it's not there */

propertyAssignment * propertyAssignmentSet::lookup_regular_assignment(Location * where)
{
  propertyAssignment * result = 0;

  // -- See if it's there

  assignment_map_p p = _assignments.find(where);
  if (p == _assignments.end()) {

    // -- Not there, so create a new one

    result = new propertyAssignment(where, propertyAssignment::Statement, 
				    _property,
				    (procedureAnn *)0,
				    (ruleAnn *)0,
				    (exprAnn *)0);

    _assignments[where] = result;
  }
  else
    result = (*p).second;

  return result;
}

/** @brief Lookup special assignment
 *
 * Create it if it's not there */

propertyAssignment * propertyAssignmentSet::lookup_special_assignment(Location * where,
								      memoryBlock * property_block,
								      propertyAssignment::Kind kind)
{
  propertyAssignment * result = 0;

  // -- See if it's there

  location_block_pair key(where, property_block);

  special_assignment_map_p p = _special_assignments.find(key);
  if (p == _special_assignments.end()) {

    // -- Not there, so create a new one

    result = new propertyAssignment(where, kind,
				    _property,
				    (procedureAnn *)0,
				    (ruleAnn *)0,
				    (exprAnn *)0);

    _special_assignments[key] = result;
  }
  else
    result = (*p).second;

  return result;
}

/** @brief Lookup rule assignment
 *
 * Create it if it's not there */

propertyAssignment * propertyAssignmentSet::lookup_rule_assignment(Location * where,
								   exprAnn * expr,
								   ruleAnn * rule)
{
  propertyAssignment * result = 0;

  // -- See if it's there

  location_expr_pair key(where, expr);

  rule_assignment_map_p p = _rule_assignments.find(key);
  if (p == _rule_assignments.end()) {

    // -- Not there, so create a new one

    result = new propertyAssignment(where, propertyAssignment::TransferFunction,
				    _property,
				    (procedureAnn *)0,
				    rule, expr);

    _rule_assignments[key] = result;
  }
  else
    result = (*p).second;

  return result;
}

/** @brief Add a def
 *
 * Update the map from defs to assignments */

void propertyAssignmentSet::add_def(propertyAssignment * assignment,
				    memoryDef * def)
{
  // -- Add it to the assignment

  assignment->add_def(def);

  // -- Record the backwards mapping

  _def_assignments[def] = assignment;
}

/** @brief Add a use */

void propertyAssignmentSet::add_use(propertyAssignment * assignment,
				    memoryUse * use)
{
  // -- Just add to the uses

  assignment->add_use(use);
}

/** @brief Trace assignment
 *
 * Find the assignment to which a def belongs. */

propertyAssignment * propertyAssignmentSet::trace_def(memoryDef * def)
{
  def_assignment_map_p p = _def_assignments.find(def);

  if (p != _def_assignments.end())
    return (*p).second;

  return 0;
}

  /** @brief Better trace */

  bool trace_a_def(ostream & out,
		   memoryDef * def,
		   string & indent,
		   int depth,
		   assignment_set & already_seen);


bool enumPropertyAnn::trace_a_def(ostream & out,
				  memoryDef * def,
				  string & indent,
				  int depth,
				  assignment_set & already_seen)
{
  // -- Find the assignment

  propertyAssignment * assignment = _assignments.trace_def(def);

  if ( ! assignment ) {
    out << indent << "  - No assignment" << endl;
    return false;
  }

  if (already_seen.find(assignment) != already_seen.end()) {
    // out << " : already seen (part of a loop)" << endl;
    return false;
  }

  // -- Push

  depth++;
  already_seen.insert(assignment);

  bool found = false;

  memoryuse_set uses_found;
  enumvalue_set values_found;

  for (memoryuse_set_cp p = assignment->uses().begin();
       p != assignment->uses().end();
       ++p)
    {
      memoryUse * cur_use = *p;
      memoryDef * reaching_def = cur_use->reaching_def();

      if (reaching_def) {

	bool found_one = trace_a_def(out, reaching_def, indent, depth+1, already_seen);

	if (found_one) {
	  found = true;
	  uses_found.insert(cur_use);
	}

	enumValueAnn * in_val = lookup_now_value(reaching_def->owner(), reaching_def);
	if (in_val != top())
	  values_found.insert(in_val);
      }
    }

  enumValueAnn * cur_val = lookup_now_value(def->owner(), def);

  switch (assignment->kind()) {
  case propertyAssignment::Statement :
    if (found) {

      out << indent << "[" << depth << "]" << "Assign " << def->owner()->name() << " = " << cur_val->name() << " --- at " << * (def->where());

      if (def->is_weak())
	out << " (weak)";

      out << endl;
    }
    break;

  case propertyAssignment::TransferFunction :
    {
      ruleAnn * rule;
      enumPropertyExprAnn * enum_expr = 0;

      rule = assignment->rule();

      if (rule)
	enum_expr = (enumPropertyExprAnn *) assignment->expr();

      if (enum_expr &&
	  enum_expr->rhs_is_property_value()) {
	found = true;
	out << "--- FOUND ----------->" << endl;
      }

      if (found) {

	out << indent << "[" << depth << "]" << "Rule " << def->owner()->name() << " = " << cur_val->name() << " --- at " << * (def->where());

	if (def->is_weak())
	  out << " (weak)";

	out << endl;

	if (rule) {

	  out << indent << "    if (";

	  if (rule->condition())
	    rule->condition()->print(out);
	  else
	    out << "true";

	  out << ")" << endl;
	  out << "       ";
	  enum_expr->print(out);
	  out << endl;
	}
      }
    }
  break;

  case propertyAssignment::Merge :
    if (found) {
      out << indent << "[" << depth << "]" << "Merge " << def->owner()->name() << " = " << cur_val->name() << " --- at " << * (def->where()) << endl;
    }
    break;

  case propertyAssignment::Parameter :
    if (found) {
      out << indent << "[" << depth << "]" << "Pass " << def->owner()->name() << " = " << cur_val->name() << " --- at " << * (def->where());
	
      if (def->is_weak())
	out << " (weak)";

      out << endl;

      if (def->where()->kind() == Location::Statement) {

	out << indent << "    Return from ";

	stmtLocation * callsite = (stmtLocation *) def->where();
	stmtNode * stmt = callsite->stmt();
	output_context oc(out);
	oc.indent_in();
	oc.indent_in();
	stmt->output(oc, 0);
      }

      if (def->where()->kind() == Location::BasicBlock) {

	for (memoryuse_set_p q = uses_found.begin();
	     q != uses_found.end();
	     ++q)
	  {
	    memoryUse * at_callsite = *q;

	    out << indent << "    Pass in to ";
	    procLocation * procloc = (procLocation *) at_callsite->where();
	    stmtLocation * callsite = procloc->stmt_location();
	    stmtNode * stmt = callsite->stmt();
	    output_context oc(out);
	    oc.indent_in();
	    oc.indent_in();
	    stmt->output(oc, 0);
	  }
      }
      
      out << endl;
    }
    break;

  case propertyAssignment::Conservative :
    if (found) {
      out << indent << "[" << depth << "]" << "Conservative " << def->owner()->name() << " = " << cur_val->name() << " --- at " << * (def->where());

      if (def->where()->kind() == Location::Statement) {

	stmtLocation * stmt_loc = (stmtLocation *) def->where();
	stmtNode * stmt = stmt_loc->stmt();
	output_context oc(out);
	oc.indent_in();
	oc.indent_in();
	stmt->output(oc, 0);
	out << endl;
      }
    }

    break;

  default:
    if (found) {
      out << indent << "[" << depth << "]" << " (unknown) ";
    }
    break;
  }

  /*
  if (values_found.size() > 1) {
    out << indent << "  => Information merge: ";
    for (enumvalue_set_p w = values_found.begin();
	 w != values_found.end();
	 ++w)
      {
	out << (*w)->name() << " ";
      }
    out << endl;
  }
  */

  if (found) {
    for (memoryuse_set_cp p = assignment->uses().begin();
	 p != assignment->uses().end();
	 ++p)
      {
	memoryUse * cur_use = *p;
	memoryDef * reaching_def = cur_use->reaching_def();

	if (reaching_def) {
	  enumValueAnn * in_val = lookup_now_value(reaching_def->owner(), reaching_def);

	  out << indent << "   ^= " << cur_use->owner()->name() << " = " << in_val->name() << " at " << * (reaching_def->where());
	  if (uses_found.find(cur_use) != uses_found.end())
	    out << " TRACED -----^" << endl;
	  else
	    out << " (untraced)" << endl;
	}
      }
  }

  // -- Pop
 
  // assignment_set_p t = already_seen.find(assignment);
  // already_seen.erase(t);

  return found;
}

  /** @brief Variable trace
   *
   * */

  void trace_variable(ostream & out,
		      const memoryuse_set & uses,
		      assignment_set & already_seen);


/** @brief Variable trace
 *
 * Trace the value of this variable through all of it's defs. */

void enumPropertyAnn::trace_variable(ostream & out,
				     const memoryuse_set & uses,
				     assignment_set & already_seen)
{
  // -- First find the lowest value use

  memoryUse * worst_use = 0;
  enumValueAnn * worst_val = top();

  for (memoryuse_set_cp p = uses.begin();
       p != uses.end();
       ++p)
    {
      memoryUse * cur_use = *p;

      memoryDef * reaching_def = cur_use->reaching_def();

      cout << " + Use at " << * (cur_use->where());

      if (reaching_def) {
	enumValueAnn * cur = lookup_now_value(reaching_def->owner(),
					      reaching_def);
	if (meet(cur, worst_val) == cur) {
	  worst_val = cur;
	  worst_use = cur_use;
	}

	cout << ", reaching def at " << * (reaching_def->where()) << ", value = " << cur->name() << endl;
      }
      else
	cout << ", no reaching def" << endl;
    }

  if ( ! worst_use) {
    out << "  => No uses" << endl;
    return;
  }

  memoryDef * def = worst_use->reaching_def();

  out << " Use of " << worst_use->owner()->container()->name() 
      << " at " << * (worst_use->where()) 
      << " with reaching def at " << * (def->where()) 
      << " value = " << worst_val->name() << endl;

  // -- Find the assignment

  propertyAssignment * assignment = _assignments.trace_def(def);

  if ( ! assignment ) {
    out << " No assignment" << endl;
    return;
  }

  out << " Assignment at " << * (assignment->where());

  if (already_seen.find(assignment) != already_seen.end()) {
    out << " : already seen (part of a loop)" << endl;
    return;
  }

  already_seen.insert(assignment);

  out << " of type ";

  switch (assignment->kind()) {
  case propertyAssignment::Statement : out << " statement ";
    break;
  case propertyAssignment::TransferFunction : out << " transfer function ";
    break;
  case propertyAssignment::Merge : out << " merge ";
    break;
  case propertyAssignment::Parameter : out << " parameter ";
    break;
  case propertyAssignment::Conservative : out << " conservative ";
    break;
  default:
    out << " (unknown) ";
    break;
  }

  out << endl;

  trace_variable(out, assignment->uses(), already_seen);
}

  /** @brief Add all defs
   *
   * Visit the left-hand side of some assignment or update and add all the
   * current defs to the given assignment object. Also, be careful to get
   * the property blocks. */

  void add_all_defs(propertyAssignment * assignment, memoryblock_set & left);

  /** @brief Add all uses
   *
   * Visit the right-hand side of some assignment or update and add all the
   * current uses to the given assignment object. Also, be careful to get
   * the property blocks. */

  void add_all_uses(propertyAssignment * assignment, memoryblock_set & right);

/** @brief Add all defs
 *
 * Visit the left-hand side of some assignment or update and add all the
 * current defs to the given assignment object. Also, be careful to get the
 * property blocks. */

void enumPropertyAnn::add_all_defs(propertyAssignment * assignment, memoryblock_set & left)
{
  for (memoryblock_set_p p = left.begin();
       p != left.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {
	memoryDef * def = property_block->current_def();
	if (def) {
	  _assignments.add_def(assignment, def);

	  if (def->is_weak())
	    _assignments.add_use(assignment, property_block->current_use());
	}
      }
    }
}

/** @brief Add all uses
 *
 * Visit the right-hand side of some assignment or update and add all the
 * current uses to the given assignment object. Also, be careful to get
 * the property blocks. */

void enumPropertyAnn::add_all_uses(propertyAssignment * assignment, memoryblock_set & right)
{
  for (memoryblock_set_p p = right.begin();
       p != right.end();
       ++p)
    {
      memoryBlock * real_block = *p;
      memoryBlock * property_block = lookup_property_block(real_block);

      if (property_block) {
	memoryUse * use = property_block->current_use();
	if (use)
	  _assignments.add_use(assignment, use);
      }
    }
}

