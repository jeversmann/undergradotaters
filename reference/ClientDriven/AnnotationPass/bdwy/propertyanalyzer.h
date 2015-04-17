

#ifndef BDWY_PROPERTY_ANALYZER_H
#define BDWY_PROPERTY_ANALYZER_H

#include "ipanalysis.h"
#include "constants.h"

/** @brief Property analyzer
 *
 *  */

class propertyAnalyzer : public analysisProblem
{
public:

  typedef map< memoryBlock *, setPropertyAnn *> set_property_variable_map;
  typedef set_property_variable_map::iterator set_property_variable_map_p;

private:

  /** @brief Annotations */

  Annotations * _annotations;

  /** @brief Constants
   *
   * This is a reference to the constantAnalyzer class from the pointer
   * analyzer. */

  constantAnalyzer & _constants;

  /** @brief Set property variable map
   *
   * This field stores a map from the memoryBlock that represents a
   * particular set property to the property object itself. We need this
   * during control-flow merges in order to determine which property is
   * being merged. */

  set_property_variable_map _set_property_variables;

public:

  /** @brief Create a new property analyzer
   *
   * This class is only created by the Analyzer::analyze_properties()
   * method. It supplies the constantAnalyzer needed. */

  propertyAnalyzer(Annotations * annotations,
		   constantAnalyzer & constants);

  /** @brief Is set property variable block?
   *
   * Check to see if this is special memory block used to represent the
   * state of a set property. */

  setPropertyAnn * is_set_property_variable_block(memoryBlock * block);

  /** @brief Is enum property variable block?
   *
   * Check to see if this is special memory block used to represent the
   * state of a enum property. */

  enumPropertyAnn * is_enum_property_variable_block(memoryBlock * block);

  /** @brief Name of the analysis
   *
   * Just for debug purposes. */

  virtual string name() {
    if (_direction == Forward)
      return string("Forward Properties");
    else
      return string("Backward Properties");
  }

  /** @brief Initialize
   *
   * Clear all the property analysis information in order to allow
   * re-analysis. Also, rebuild the list of property variable blocks. */

  void initialize();

  /** @brief Set the direction of analysis
   *
   * This indicates whether to compute the forward properties, or the
   * backward properties. */

  void set_direction(Direction d) { _direction = d; }

  /** @brief Compute constants
   *
   * Before analyzing a library procedure, we precompute the current
   * constant values for all the variables that are used in numeric
   * expressions (idNodeAnn objects). That way we can just use the built-in
   * exprNode::eval() method to compute their values.
   *
   * ALSO: call this procedure before any call to evaluate an exprAnn. */

  void compute_all_constants(procLocation * where,
			     procedureAnn * procedure);

  /** @brief Lookup one constant
   *
   * Return the constant value of a single annVariable at a particular
   * calling context. */

  const constant * lookup_constant(annVariable * var,
				   procLocation * where);

  /** @brief Analyze callsite
   *
   * Process the analysis annotations at a particular library call.
   *
   * Apply analyze annotations in three passes: (1) test all of the
   * conditions to determine which rules can fire, (2) for the selected
   * rules, process the effects to determine the new states that will
   * result, and (3) apply the new states. */

  void analyze_callsite(procLocation * where,
			procedureAnn * procedure,
			memoryblock_set & changes,
			pointerValue & return_val);

  /** @brief Update a constant value
   *
   * For the "constant" analysis, update a constant values for an object as
   * a result of a rule firing. */

  void update_constant(pointerValue & variable,
		       constant & value,
		       memoryblock_set & changes);

  // --- Transfer functions -----------------------

  // -- Non-pointer expressions

  virtual void at_id(stmtLocation * current, idNode * id,
		     pointerValue & value);

  virtual void at_unary(stmtLocation * current,
			stmtNode *t,
			pointerValue & operand,
			pointerValue & result);

  virtual void at_binary(stmtLocation * current,
			 stmtNode *t,
			 pointerValue & left,
			 pointerValue & right,
			 pointerValue & result);

  virtual void at_cast(stmtLocation * current, operandNode *operand,
		       pointerValue & operand_value,
		       pointerValue & result);

  virtual void at_const(stmtLocation * current, constNode * cons,
			pointerValue & result);

  // -- Pointer expressions

  virtual void at_field_access(stmtLocation * current,
			       operandNode *operand,
			       pointerValue & operand_value,
			       idNode * field,
			       pointerValue & result);

  virtual void at_dereference(stmtLocation * current,
			      operandNode *operand_value,
			      pointerValue & operand,
			      pointerValue & result);

  virtual void at_address(stmtLocation * current,
			  operandNode *operand,
			  pointerValue & operand_value,
			  pointerValue & result);

  virtual void at_index(stmtLocation * current,
			operandNode *operand,
			pointerValue & left,
			pointerValue & right,
			pointerValue & result);

  // -- Assignments

  virtual void at_assignment(stmtLocation * current,
			     pointerValue & left,
			     pointerValue & right,
			     pointerValue & result,
			     memoryblock_set & changes);

  virtual void at_parameter_pass(Location * current,
				 stmtLocation * parameter_callsite,
				 pointerValue & left,
				 pointerValue & right,
				 memoryblock_set & changes);

  virtual void at_self_assignment(Location * source,
				  Location * target,
				  memoryBlock * block,
				  memoryblock_set & changes,
          bool is_input);

  // -- Statement types

  virtual void at_return(stmtLocation * stmt,
			 returnNode * ret,
			 pointerValue & result,
			 pointerValue & return_val);

  // -- Process a merge point

  virtual void at_merge(Location * where,
			memoryBlock * block,
			memoryuse_list & phi_uses,
			pointerValue & result,
			memoryblock_set & changes);

  // -- Procedure boundaries

  virtual void at_conservative_procedure_call(stmtLocation * current,
					      operandNode * call,
					      operand_list & args,
					      pointerValue & call_target,
					      pointervalue_list & arguments,
					      memoryblock_set & reachable_blocks,
					      pointerValue & return_val,
					      memoryblock_set & changes);

protected:

  /** @brief Property assignment
   *
   * This method handles both regular statement-level assignments and
   * parameter passing. */

  void property_assignment(Location * current,
			   stmtLocation * parameter_callsite,
			   pointerValue & left,
			   pointerValue & right,
			   bool is_parameter,
			   memoryblock_set & changes);

  /** @brief Update the return value
   *
   * Since the bindings on each local annVariable are copies of the
   * original pointerValue objects, we need to transfer those values back
   * to the return value. Otherwise, we can't actually return any
   * values. */

  void update_return_value(procLocation * where,
			   procedureAnn * procedure,
			   pointerValue & return_val);

  // TB_unify
  virtual bool compare_property_value(Location *where,
                                      memoryBlock *property_block,
                                      procedureInfo *callee);

  // TB_unify
  virtual void record_input_to_value(procedureInfo *callee,
                                     memoryBlock *property_block,
                                     stmtLocation *callsite);
};

#endif /* BDWY_PROPERTY_ANALYZER_H */
