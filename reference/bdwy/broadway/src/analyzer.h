
#ifndef BDWY_ANALYZER_H
#define BDWY_ANALYZER_H

/** @brief Analyzer
 *
 * The Analyzer is a subclass of the pointer analyzer, augmented to use the
 * dependence and pointer information given in the annotations. It is also
 * used as the analysis framework for the propertyAnalyzer class. */

class Analyzer : public Pointers
{
public:

  typedef map< memoryBlock *, enumPropertyAnn *> enum_property_variable_map;
  typedef enum_property_variable_map::iterator enum_property_variable_map_p;

private:

  /** @brief Property analyzer
   *
   * When non-null, then we are in the process of analyzing properties (and
   * constants). */

  propertyAnalyzer * _property_analyzer;

  /** @brief Liveness analyzer
   *
   * When non-null, then we are in the process of analyzing liveness. */

  bdwyLivenessAnalyzer * _liveness_analyzer;

  /** @brief All annotations */

  Annotations * _annotations;

  /** @brief Procedure info for init */

  procedureInfo * _init;

public:

  Analyzer(Annotations * annotations,
	   procNode * root,
	   Linker & l,
	   unitNode * unit);

  ~Analyzer();

  /** @brief Analyze pointers */

  void analyze_pointers();

  /** @brief Analyze properties */

  void analyze_properties();

  /** @brief Analyze liveness */

  void analyze_liveness(bdwyLivenessAnalyzer * live);

  /** @brief Return the property analyzer */

  inline propertyAnalyzer * property_analyzer() const { return _property_analyzer; }

  /** @brief Get the memory size */

  inline int memory_size() { return Memory.size(); }

  /** @brief Get the number of procedures */

  void number_of_procedures(int & total, int & analyzed,
			    int & context_insensitive,
			    int & recursive,
			    int & unanalyzed,
			    int & program_size);

  /** @brief Lookup a procedure
   *
   * Calls look up on the procedureDB. */

  procedureInfo * lookup_procedure(procNode * proc);

  /** @brief Setup all bindings
   *
   * This is a public interface to set up the bindings for a particular
   * call site. It is used by the procedureAnn class when it processes the
   * reports or actions. */

  void setup_all_bindings(procedureAnn * procedure,
			  callNode * call,
			  procLocation * libproc_location);

private:

  // --- Override the procedure call code to use the annotations

  virtual void procedure_call(procedureInfo * caller,
			      stmtLocation * current,
			      callNode * call,
			      pointerValue & call_target,
			      memoryBlock * target_mb,
			      pointervalue_list & arguments,
			      memoryblock_set & external_defs,
			      memoryuse_set & external_uses,
			      memoryblock_set & external_changes,
			      pointerValue & return_val,
			      bool & never_returns);

  // --- Process the different components 

  /** @brief Set up parameter bindings
   *
   * Basically like passing the parameters, except we just create bindings
   * to the actuals. */

  void setup_parameter_bindings(procedureInfo * info,
				procedureAnn * procedure,
				procLocation * where,
				procedureInfo * caller,
				stmtLocation * callsite,
				pointervalue_list & arguments,
				memoryblock_set & local_defs,
				memoryuse_set &  local_uses,
				memoryblock_set & local_changes);

  /** @brief Process the on_entry annotations
   *
   * This involves following the pointer structure given by the annotations
   * and storing the actual memoryBlocks in the bindings. Each binding
   * works like a pass-by-reference object: we apply the star operator to
   * find out what it actually represents. */

  void process_on_entry(procedureInfo * info,
			procLocation * where,
			procedureAnn * procedure,
			bool search_for_def,
			memoryblock_set & external_defs,
			memoryuse_set & external_uses,
			memoryblock_set & external_changes);

  /** @brief Deallocate heap objects
   *
   * For the deleted objects, go through and update the multiplicity. It's
   * important to do this before any other updates because it ensures that
   * updates are strong when possible. */

  void deallocate_heap_objects(procedureInfo * info,
			       procLocation * where,
			       procedureAnn * procedure,
			       memoryblock_set & defs,
			       memoryuse_set & uses,
			       memoryblock_set & changes);

  /** @brief Allocate heap objects
   *
   * Generate memoryBlocks for heap allocated objects (those that have the
   * "new" operator). We do this exactly like we handle malloc in the
   * normal pointer analyzer. */

  void allocate_heap_objects(procedureInfo * info,
			     procedureInfo * caller,
			     procLocation * where,
			     stmtLocation * callsite,
			     procedureAnn * procedure,
			     memoryblock_set & defs,
			     memoryuse_set & uses,
			     memoryblock_set & changes);

  /** @brief Generate uses and defs
   *
   * This includes the access and modify annotations, and creating defs and
   * uses for the special property objects. We also use this later during
   * the processing of reports and actions to make sure that the current
   * defs and uses are set. */

  void create_uses_and_defs(procedureInfo * info,
			    procLocation * where,
			    procedureAnn * procedure,
			    bool search_for_def,
			    memoryblock_set & defs,
			    memoryuse_set & uses,
			    memoryblock_set & changes);

  /** @brief Process the on_exit annotations
   *
   * Each on_exit annotation is an assertion about a points-to
   * setPropertieship. We use the bindings to figure out the LHS and RHS,
   * and then call the assignment operator. */

  void process_on_exit(procedureInfo * info,
		       procedureInfo * caller,
		       procLocation * where,
		       procedureAnn * procedure,
		       memoryblock_set & defs,
		       memoryuse_set & uses,
		       memoryblock_set & changes);

  /** @brief Initialize
   *
   * This method is overridden from the Pointers class so that we can
   * initialize the global variables. */

  virtual void initialize();

  /** @brief Look up property block
   *
   * Given a regular memory block, look up the special block that
   * represents the given property. When computing pointers we create these
   * blocks as necessary. */

  memoryBlock * lookup_property_block(memoryBlock * real_block,
				      enumPropertyAnn * property);

  /** @brief Generate one use
   *
   * Used by create_uses_and_defs and assignment_operator */

  void generate_one_use(procedureInfo * info,
			Location * def_use_location,
			bool search_for_def,
			memoryBlock * block,
			memoryuse_set & uses);

  /** @brief Generate property defs
   *
   * This method creates a def for each property block for the given set of
   * properties. Used by create_uses_and_defs and assignment_operator */

  void generate_property_defs(procedureInfo * info,
			      Location * def_use_location,
			      pointerValue & ptrs,
			      const enum_property_set & properties,
			      const enum_property_set & weak_properties,
			      memoryblock_set & defs,
			      memoryuse_set & uses,
			      memoryblock_set & changes);

  /** @brief Generate one def
   *
   * */

  void generate_one_def(procedureInfo * info,
			Location * def_use_location,
			memoryBlock * block,
			memoryBlock * real_block,
			bool unique_lhs,
			memoryblock_set & defs,
			memoryuse_set & uses,
			memoryblock_set & changes,
			bool force_weak_update);

  /** @name Override from Pointers
   *
   * We replace the following methods from Pointers with versions that
   * properly handle generating the uses and defs of the property
   * objects. */

  //{

  /** @brief Assignment "=" operator
   *
   * When the additive flag is set, the assignment will include the
   * previous value of the pointer at this location. This allows us to say
   * things like "p = &x and p = &y" without having to build a single
   * pointerValue that contains both x and y.
   * */

  virtual void assignment_operator(procedureInfo * info,
				   Location * current,
				   stmtLocation * parameter_callsite,
				   pointerValue & left_hand_side,
				   pointerValue & right_hand_side,
				   memoryblock_set & defs,
				   memoryuse_set & uses,
				   memoryblock_set & changes);

  //}
};


#endif /*  */
