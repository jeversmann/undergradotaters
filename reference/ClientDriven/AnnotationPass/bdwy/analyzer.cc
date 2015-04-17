
#include "broadway.h"
#include "unification_ann.h"

#include <sys/time.h>
#include <unistd.h>

Analyzer::Analyzer(Annotations * annotations,
		   procNode * root, 
		   Linker & l,
		   unitNode * unit)
  : Pointers(root, l, unit),
    _property_analyzer(0),
    _liveness_analyzer(0),
    _annotations(annotations)
{
  // Register the annotated procedures (actually the synthetically created
  // procedure bodies) with the procedure database

  const procedures_map & procedures = annotations->procedures();
  for (procedures_map_cp p = procedures.begin();
       p != procedures.end();
       ++p)
    {
      procedureAnn * proc_ann = (*p).second;

      // -- Create a procedureInfo object for this library routine

      Procedures.add_procedure(proc_ann->proc());

      // -- Make sure it's context-sensitive

      procedureInfo * info = Procedures.lookup(proc_ann->proc());
      info->set_prefer_context_sensitive();
    }

  // Create and store a procedureInfo object for the init procedure

  Procedures.add_procedure(annotations->init()->proc());
  _init = Procedures.lookup(annotations->init()->proc());
}

Analyzer::~Analyzer()
{
  // -- Make sure to delete the property analyzer

  if (_property_analyzer)
    delete _property_analyzer;
}

void Analyzer::run_unification() {
  if(pointerOptions::Unification) {
    if(pointerOptions::Show_Unification || pointerOptions::Verbose)
      cout << "Start unification-based analysis\n";
    _unification_based = UnificationBasedPtr_Ann::analyze_all(linker,
                                                              _annotations);
    Memory.unification(_unification_based);
    if(pointerOptions::Show_Unification || pointerOptions::Verbose)
      cout << "Unification done\n";
    if(pointerOptions::Show_Unification)
      _unification_based->print_ecr();
  }
} // run_unification

// ------------------------------------------------------------
//  Analysis entry points
// ------------------------------------------------------------

/** @brief Analyze pointers */

void Analyzer::analyze_pointers()
{
  // -- Clear the annotations

  _annotations->clear();

  // Set up the binding for the "null" objects

  annVariable * nullvar = _annotations->lookup_global("null");
  // nullvar->create_binding(Memory.null());
  nullvar->clear_all();

  // -- Call the superclass method

  Pointers::analyze();
}

/** @brief Analyze properties */

void Analyzer::analyze_properties()
{
  // -- Delete any previous analyzer

  if (_property_analyzer)
    delete _property_analyzer;

  // -- Create a property analyzer object

  _property_analyzer = new propertyAnalyzer(_annotations,
					    _constants);

  // -- Clear the annotations

  _annotations->clear();

  // -- Initialize the property analyzer

  _property_analyzer->initialize();

  // -- Clear the error reports

  Annotations::Error_reports.clear();

  // Set up the binding for the "null" objects

  annVariable * nullvar = _annotations->lookup_global("null");
  // nullvar->create_binding(Memory.null());
  nullvar->clear_all();

  // -- Peform all the forward analyses

  if (pointerOptions::Verbose)
    cout << "=== Analyze forward properties ===================== " << endl;

  _property_analyzer->set_direction(Forward);

  bool old_verbose_val = pointerOptions::Verbose;
  if (Annotations::Verbose_properties)
    pointerOptions::Verbose = true;

  // struct timeval tv_start;
  // struct timeval tv_end;
  // struct timezone tz;

  // -- Start timer

  // gettimeofday(&tv_start, &tz);

  // -- Run the forward analyses

  analyze(_property_analyzer);

  // -- Decide if there are any backwards analyses, and only run the
  // analyzer if there are.

  const enum_property_map & props = _annotations->enum_properties();

  bool any_backward_properties = false;

  for (enum_property_map_cp p = props.begin();
       p != props.end();
       ++p)
    if (((*p).second->direction() == Backward) &&
	(*p).second->is_enabled())
      any_backward_properties = true;

  if (any_backward_properties) {
    if (pointerOptions::Verbose)
      cout << "=== Analyze backward properties ==================== " << endl;

    _property_analyzer->set_direction(Backward);
    analyze(_property_analyzer);
  }

  // -- End timer

  // gettimeofday(&tv_end, &tz);

  pointerOptions::Verbose = old_verbose_val;

  /*
  cout << "Start time: " << tv_start.tv_sec << "s " << tv_start.tv_usec << "us" << endl;
  cout << "End time:   " << tv_end.tv_sec << "s " << tv_end.tv_usec << "us" << endl;

  double start_time = (double) tv_start.tv_sec + ((double)tv_start.tv_usec)/1000000.0;
  double end_time = (double) tv_end.tv_sec + ((double)tv_end.tv_usec)/1000000.0;
  double total = end_time - start_time;

  Annotations::Analysis_time += total;

  cout << "STAT-time-analysis " << Annotations::Analysis_time << endl;
  */

  // -- Ending break point

  unsigned long int end_brk = (unsigned long int) sbrk(0);
  cout << "STAT-memory-analysis " << (end_brk - Annotations::Start_brk) << endl;

  // -- In adaptive mode: post-process the analysis results to make some
  // variables flow-sensitive:
  /*
  if ((Annotations::Adaptivity == 1) ||
      (Annotations::Adaptivity == 2))
    {
      // -- Visit all the properties to generate a list of flow-sensitive
      // variables

      memoryblock_set flow_sensitive;

      for (enum_property_map_cp p = props.begin();
	   p != props.end();
	   ++p)
	{
	  enumPropertyAnn * property = (*p).second;
	  if (property->is_enabled() &&
	      (property != _annotations->constants_property()))
	    property->precision_analysis(&Memory, flow_sensitive);
	}
    }
  */
}

/** @brief Analyze liveness */

void Analyzer::analyze_liveness(bdwyLivenessAnalyzer * live)
{
  if (pointerOptions::Verbose)
    cout << "=== Analyze liveness =============================== " << endl;

  _liveness_analyzer = live;

  live->clear();
  analyze(live);

  _liveness_analyzer = 0;
}

// ------------------------------------------------------------
// Procedure call 
// ------------------------------------------------------------

void Analyzer::procedure_call(procedureInfo * caller,
			      stmtLocation * callsite,
			      operandNode *call,
			      operand_list & args,
			      pointerValue & call_target,
			      memoryBlock * target_mb,
			      pointervalue_list & arguments,
			      memoryblock_set & external_defs,
			      memoryuse_set & external_uses,
			      memoryblock_set & external_changes,
			      pointerValue & return_val,
			      bool & never_returns)
{
  bool old_verbose_pointers_val = pointerOptions::Verbose;
  bool old_verbose_properties_val = Annotations::Verbose_properties;
  bool old_verbose_constants_val = pointerOptions::Verbose_constants;

  // -- Look it up in the annotations

  procedureAnn * procedure = _annotations->lookup_procedure(target_mb->decl()->name());

  // -- Look up the procNode

  procNode * callee = 0;
  if (procedure)
    callee = procedure->proc();
  else
    callee = linker.lookup_procedure(target_mb->decl());

  // -- Look up the procedureInfo

  procedureInfo * info = 0;
  if (callee)
    info = Procedures.lookup(callee);

  // --- Check the monitor, set the verbose flags

  if (info && info->is_verbose()) {
    pointerOptions::Verbose = true;
    Annotations::Verbose_properties = true;
    pointerOptions::Verbose_constants = true;
  }

  if (procedure == 0) {

    // --- No annotations for this procedure, call the regular
    // interprocedural analyzer.

    Pointers::procedure_call(caller, callsite,
			     call, args, call_target, target_mb, arguments,
			     external_defs, external_uses, external_changes,
			     return_val, never_returns);

    // -- Restore verbose flags

    if (info && info->is_verbose()) {
      pointerOptions::Verbose = old_verbose_pointers_val;
      Annotations::Verbose_properties = old_verbose_properties_val;
      pointerOptions::Verbose_constants = old_verbose_constants_val;
    }

    if(pointerOptions::Slice_analysis && Problem &&
       pointerOptions::No_Skip_procs.find(info) ==
       pointerOptions::No_Skip_procs.end() ) {
      // TB: if external_chanegs contains a P, or return value can reach P,
      // where P is a property with error value, then insert info into
      // No_Skip_procs.
    }

    return;
  }

  // -- Check for multiple targets

  bool multiple_call_targets = false;
  if (call_target.blocks.size() > 1) {

    /* TB
    // -- Can't handle that right now
    if (pointerOptions::Verbose)
      cout << "ERROR: Cannot handle multiple targets for annotated procedures: at " 
	   << call->coord() << " -- " << * callsite << endl;

    return; */
    multiple_call_targets = true;
    callsite->setup_cs_call(info->proc()); // treat as if only callee.
  }

  // -- Record the call at the callee

  // TB: note: even with multuple_call_targets, force call to be
  // context-sensitive by using false as 4th argument below.
  info->setup_call_at(caller, callsite, false, false);

  // -- Push this call on the call stack
      
  Procedures.call_to(callsite, info);

  // -- Get the callee path: all of the variable bindings are indexed by this location.

  procLocation * libproc_location = info->procedure_location(callsite);

  // -- Visit the basic block: compute dominator tree numbering

  basicblockNode * one_block = (basicblockNode *) callee->body()->stmts().front();
  basicblockLocation * block_loc = libproc_location->lookup_block(one_block);

  block_loc->visit();

  // -- Visit the statements: compute dominator tree numbering

  stmtLocation * stmt_loc = 0;

  stmt_location_vec & statements = block_loc->statements();
  for (stmt_location_vec_p p = statements.begin();
       p != statements.end();
       ++p)
    {
      stmt_loc = & (*p);
      stmt_loc->visit();
    }

  // -- Reset the call site

  callsite->visit();

  // -- Record the calling context

  procedure->add_calling_context(call, libproc_location);

  if (pointerOptions::Verbose) {
    cout << "=== Annotated  " <<  * libproc_location << " =================================" << endl;
  }

  // -- Record local defs, uses, changes -- transfer them at the end.

  memoryblock_set local_defs;
  memoryuse_set   local_uses;
  memoryblock_set local_changes;

  // -- Pass the parameters

  setup_parameter_bindings(info, procedure, libproc_location,
			   caller, callsite, arguments,
			   local_defs, local_uses, local_changes);

  // --- Set up the binding for the return variable

  annVariable * return_var = procedure->lookup("return", false);
  declNode * return_decl = return_var->decl();

  return_decl->decl_location(declNode::BLOCK);
  memoryBlock * return_block = Memory.lookup_variable(libproc_location, return_decl, callee);
  return_block->set_return_value();

  return_var->set_binding(return_block, libproc_location);

  // --- Run analysis

  progress_meter(caller, false);

  // --- Process on-entry

  process_on_entry(info, libproc_location, procedure, true,
		   local_defs, local_uses, local_changes);

  // --- Deallocate any deleted heap objects: we do this before allocation
  // because it makes more sense. TBD: do we need to distinguish between
  // deletes that are on_entry versus on_exit?

  deallocate_heap_objects(info, libproc_location, procedure,
			  local_defs, local_uses, local_changes);

  // --- Allocate any new objects generated by the annotations

  allocate_heap_objects(info, caller, libproc_location, callsite, procedure,
			local_defs, local_uses, local_changes);

  // --- Make sure all memoryUse and memoryDef objects are now created. We
  // do this regardless of the computePointers flag because it sets the
  // current_use and current_def fields on each memoryBlock.

  create_uses_and_defs(info, libproc_location, procedure, true,
		       local_defs, local_uses, local_changes);

  // --- Perform property analysis, if necessary:

  if (_property_analyzer)
    _property_analyzer->analyze_callsite(libproc_location, procedure,
					 local_changes, return_val);

  // --- Perform liveness analysis, if necessary:

  if (_liveness_analyzer)
    _liveness_analyzer->record_defs(libproc_location, procedure);

  // --- We only need to handle processing the
  // on_exit annotations while we are building the pointer
  // information. NOTE that all defs are external defs because we never
  // introduce variables that are local to the annotated library function.

  if (1) { // computePointers) {

    // --- Process on-exit
    
    process_on_exit(info, caller, libproc_location, procedure,
		    local_defs, local_uses, local_changes);
  }

  // -- For the return value, set up the special use that will be visible
  // at the call site. This is a little wierd because the use and it's
  // reaching def will both be at the call site.

  stmtLocation * def_use_loc = procedure->def_use_location(libproc_location);

  memoryUse * return_use = return_block->use_at(def_use_loc);
  if (1) // computePointers)
    if (return_var->is_defined()) {
      memoryDef * def = return_block->find_def_at(def_use_loc);
      return_use->reaching_def(def);
    }

  return_val.blocks.insert(return_block);

  // -- Transfer the changes: the main thing this does is filter out all of
  // the return value changes.

  for (memoryblock_set_p ch = local_changes.begin();
       ch != local_changes.end();
       ch++)
    {
      memoryBlock * block = (*ch);

      if(! multiple_call_targets) { // TB
        if ( ! block->write_protected() &&
             (block->local_to() != callee)) {
          external_changes.insert(block);
        }
      } else { // TB: multiple_call_targets
        cs_pass_one_external_output(info, caller, callsite, block,
                                    block==return_block);
      }
    }

  for (memoryuse_set_p p = local_uses.begin();
       p != local_uses.end();
       ++p)
    {
      memoryUse * use = *p;
      memoryBlock * block = use->owner();

      if ( ! block->write_protected() &&
	   (block->local_to() != callee)) {
	external_uses.insert(use);
      }
    }

  // -- Leave external defs empty: why are we computing them anyway?

  // -- Indicate that we're returning.

  Procedures.return_from();

  if (pointerOptions::Verbose) {
    cout << "=== Return to " << * callsite << " =================================" << endl;
    print_memoryblock_set("   Returning: ", return_val.blocks, cout);
    if (return_val.constant_value)
      cout << " = " << return_val.constant_value->to_string() << endl;
    else
      cout << " (no constant value)" << endl;

    return_block->print_def_use(cout);
  }

  // -- Restore verbose flags

  if (info->is_verbose()) {
    pointerOptions::Verbose = old_verbose_pointers_val;
    Annotations::Verbose_properties = old_verbose_properties_val;
    pointerOptions::Verbose_constants = old_verbose_constants_val;
  }
}


/** @brief Set up parameter bindings
 *
 * Basically like passing the parameters, except we just create bindings to
 * the actuals. */

void Analyzer::setup_parameter_bindings(procedureInfo * info,
					procedureAnn * procedure,
					procLocation * where,
					procedureInfo * caller,
					stmtLocation * callsite,
					pointervalue_list & arguments,
					memoryblock_set & local_defs,
					memoryuse_set &  local_uses,
					memoryblock_set & local_changes)
{
  procNode * callee = info->proc();

  // -- Perform the parameter passing

  funcNode * f = (funcNode *) callee->decl()->type();
  if (f && (f->typ() != Func)) {
    if (pointerOptions::Verbose)
      cout << "INTERNAL ERROR: Attempting to call a non-procedure." << endl;

    return;
  }

  if (f && (! f->is_void_args())) {

    decl_list & params = f->args();

    // In the annotations, passing of parameters is NOT handled as an
    // assignment from actuals to formals. Instead, we just look up
    // the formals in the annotations and create a binding to the
    // actuals.

    pointervalue_list_p args_p = arguments.begin();
    decl_list_p header_params_p = params.begin();

    // Loop over the parameters given in the annotation, not the ones
    // in the function declaration.

    const var_list & formal_params = procedure->formal_params();
    for (var_list_cp params_p = formal_params.begin();
	 params_p != formal_params.end();
	 ++params_p)
      {
	annVariable * formal = * params_p;
	declNode * header_formal = * header_params_p;

	// -- Set up a fresh (empty) binding for the formal parameter
	
	pointerValue & formal_binding = formal->fresh_binding(where);

	// -- Look out for ellipsis arguments

	if (header_formal->type()->is_ellipsis()) {

	  // -- Here, we'll actually handle ellipsis, by merging all the
	  // remaining arguments into one binding. NOTE THIS ASSUMPTION:
	  // there is never an argument to the call of the form "&
	  // expr". That would cause one or more of the pointerValue inputs
	  // to have the is_address flag true.

	  while (args_p != arguments.end()) {

	    pointerValue & actual = * args_p;
	    for (memoryblock_set_p qq = actual.blocks.begin();
		 qq != actual.blocks.end();
		 ++qq)
	      formal_binding.blocks.insert(*qq);

	    args_p++;
	  }

	  if (pointerOptions::Verbose) {
	    cout << "Ellipsis binding of " << formal->name();
	    print_memoryblock_set(" : ", formal_binding.blocks, cout);
	  }

	  // -- Break out of the loop

	  break;
	}
	else
	  if (is_va_list(header_formal)) {

	    // -- For routines that take a va_list as an argument, we need
	    // to apply an extra dereference.

	    pointerValue & va_list_actual = * args_p;

	    star_operator(caller, callsite, va_list_actual,
			  local_defs, local_uses, local_changes, formal_binding);

	    if (pointerOptions::Verbose) {
	      cout << "Binding of va_list " << formal->name();
	      print_memoryblock_set(" : ", formal_binding.blocks, cout);
	    }
	  }
	  else {

	    // -- Get the actual parameter (and value)

	    if (args_p == arguments.end()) {
	      cout << "ERROR: Too few arguments to " << callee->decl()->name() << 
		" at " << * callsite << endl;
	      break;
	    }

	    pointerValue & actual = * args_p;

	    // -- Save the bindings:

	    formal->get_binding(where) = actual;

	    if (pointerOptions::Verbose) {
	      cout << "Binding of " << formal->name();
              if(actual.is_address) // TB
                print_memoryblock_set(" : (&) ", actual.blocks, cout);
              else
                print_memoryblock_set(" : ", actual.blocks, cout);
	    }
	  }

	++args_p;
	++header_params_p;
      }
  }
}

// --- Process the on_entry annotations. This involves following the
// pointer structure given by the annotations and storing the actual
// memoryBlocks in the bindings. Each binding works like a
// pass-by-reference object: we apply the star operator to find out
// what it actually represents.

void Analyzer::process_on_entry(procedureInfo * info,
				procLocation * where,
				procedureAnn * procedure,
				bool search_for_def,
				memoryblock_set & defs,
				memoryuse_set & uses,
				memoryblock_set & changes)
{
  const structure_list & on_entry = procedure->on_entry();

  for (structure_list_cp p = on_entry.begin();
       p != on_entry.end();
       ++p)
    {
      structureAnn * s = *p;

      if (pointerOptions::Verbose) {
	cout << "-- Process on_entry -- " << *s << endl;
      }

      // -- Look up the source binding for this callsite

      annVariable * source_var = s->source();
      pointerValue & source = source_var->get_binding(where);

      /* We'll allow this, then provide a way for the annotations to test for it
      if (source.blocks.empty()) {
	_annotations->Error(s->line(), string("No bindings for ") + s->source()->name());
      }
      */

      // -- This pass serves as the generate_uses() method in
      // Pointers. When we are first computing the pointers, we need to
      // force each use of a memoryBlock to find it's reaching definition.

      memoryuse_set unused_uses;

      stmtLocation * def_use_location = procedure->def_use_location(where);

      if(! source.is_address) // TB
        for (memoryblock_set_p q = source.blocks.begin();
             q != source.blocks.end();
             ++q)
          {
            memoryBlock * block = *q;
            generate_one_use(info, def_use_location, search_for_def, block, unused_uses);
          }

      // -- Compute the target by applying either "star" or "dot" depending
      // on the type of operation. Since the "target" pointerValue is a
      // reference, we are storing the results right into the binding of
      // the variable.

      annVariable * target_var = s->target();
      pointerValue & target = target_var->fresh_binding(where);

      // -- Clear the bindings for the target and then compute the star of
      // dot.

      target.blocks.clear();

      if (s->field_name().empty()) {

	// --- No field name, just derefence

	star_operator(info, def_use_location, source, defs, uses, changes, target);
      }
      else {

	// --- Field name, apply the "dot" operator

	dot_operator(def_use_location, s->field_name(),
                     target_var->decl() /*TB 0*/, source, uses, target);
      }

      if (pointerOptions::Verbose) {
	cout << " + Binding of " << s->target()->name();
        if(target.is_address) // TB
          print_memoryblock_set(" : (&) ", target.blocks, cout);
        else
          print_memoryblock_set(" : ", target.blocks, cout);
      }
    }
}

void Analyzer::deallocate_heap_objects(procedureInfo * info,
				       procLocation * where,
				       procedureAnn * procedure,
				       memoryblock_set & defs,
				       memoryuse_set & uses,
				       memoryblock_set & changes)
{
  if (pointerOptions::Verbose)
    cout << "-- Perform deallocations --" << endl;

  const var_map & variables = procedure->variables();

  // -- Attach deallocations to the special deallocation location

  stmtLocation * dealloc_location = procedure->dealloc_location(where);

  // -- For each variable...

  for (var_map_cp p = variables.begin();
       p != variables.end();
       ++p)
    {
      annVariable * var = (*p).second;

      // -- If it is marked to be deleted...

      if (var->is_deleted()) {

	pointerValue & ptrs = var->get_binding(where);

	// -- Call the deallocation routine

	at_deallocation(info, dealloc_location, ptrs, defs, uses, changes);
      }
    }
}

// --- Generate memoryBlocks for heap allocated objects (those that
// have the "new" operator). We do this exactly like we handle malloc
// in the normal pointer analyzer.

void Analyzer::allocate_heap_objects(procedureInfo * info,
				     procedureInfo * caller,
				     procLocation * where,
				     stmtLocation * callsite,
				     procedureAnn * procedure,
				     memoryblock_set & defs,
				     memoryuse_set & uses,
				     memoryblock_set & changes)
{
  if (pointerOptions::Verbose)
    cout << "-- Perform allocations --" << endl;

  // -- Get the special allocation location

  stmtLocation * alloc_location = procedure->alloc_location(where);

  // -- New on-exit implementation: pointer rules

  const pointerrule_list & pointer_rules = procedure->pointer_rules();

  // -- This kinda sucks: this will perform the allocations for *all*
  // pointer rules, even they don't turn out to be true. The reason is that
  // it's too complicated to make sure that all the right things are
  // used/def'd at this point.
  //
  // So, certain things may behave strangely:
  //
  // on_exit {
  //  if (blah) x --> new foo
  //  if (yah)  y --> new bar
  // }
  //
  // This annotation will always result in foo and bar being allocated.

  for (pointerrule_list_cp p = pointer_rules.begin();
       p != pointer_rules.end();
       ++p)
    {
      pointerRuleAnn * cur = (*p);

      const structure_list & on_exit = cur->effects();

      for (structure_list_cp p = on_exit.begin();
	   p != on_exit.end();
	   ++p)
	{
	  structureAnn * s = *p;

	  // -- Look for new targets

	  annVariable * source_var = s->source();
	  annVariable * target_var = s->target();

	  if (target_var->is_new()) {

	    // -- First, see if the target needs a binding for this context:

	    // -- No binding here, so create one...

	    if (pointerOptions::Verbose)
	      cout << " + Allocate heap objects: " << s->target()->name() << endl;

	    // -- First figure out what the target target_var will be bound
	    // to. If the operator is "arrow", then generate a new
	    // memoryBlock. If the operator is "dot", apply the dot
	    // operator to create a new memoryBlock.
	
	    memoryBlock * new_binding = 0;

	    if (s->field_name().empty()) {

	      // CASE 1: The operator is "arrow", so we treat this as a
	      // malloc. For example "a --> new b" implies we allocate a
	      // new object "b" on the heap.

	      pointerValue dummy;

	      string name = caller->name() + "/" + info->name() + "::" + target_var->name();

              // TB_unify
              if(/*pointerOptions::Unification &&*/ _unification_based)
                _unification_based->mark_alloc(callsite->stmt(),
                                               source_var->decl(),
                                               target_var->decl());

	      new_binding = at_allocation(name, info,
					  alloc_location, callsite->stmt(), target_var->decl(),
					  defs, uses, changes, dummy);

	      // -- Now set up the new binding

	      target_var->set_binding(new_binding, where);
	    }
	    else {

	      // CASE 2: The operator is "dot". Apply the dot operator to
	      // force creation of the field.

	      // if ( ! target_var->has_binding(where)) {

		pointerValue & source = source_var->get_binding(where);

		/* Allow this:
		   if (source.blocks.empty()) {
		   _annotations->Error(s->line(), string("No bindings for ") + s->source()->name());
		   }
		*/

		// -- Compute the "dot" right into the binding

		pointerValue & after_dot = target_var->fresh_binding(where);
		memoryuse_set uses;
		dot_operator(alloc_location, s->field_name(),
                             target_var->decl() /*0*/, source, uses, after_dot);

		//}
	    }
	  } // END is new
	} // END for all structures
    }// END for all pointer rules
}

// --- Generate all the uses and defs for the variables

void Analyzer::create_uses_and_defs(procedureInfo * info,
				    procLocation * where,
				    procedureAnn * procedure,
				    bool search_for_def,
				    memoryblock_set & defs,
				    memoryuse_set & uses,
				    memoryblock_set & changes)
{
  if (pointerOptions::Verbose)
    cout << "-- Generate uses and defs --" << endl;

  // -- Get the def use location

  Location * def_use_location = 0;

  // -- Special case for the "init" procedure: just use the root location
  // as the def-use location:

  if (where == root_location)
    def_use_location = where;
  else
    def_use_location = procedure->def_use_location(where);

  // -- Visit all the access uses

  const var_set & var_uses = procedure->uses();

  for (var_set_cp p = var_uses.begin();
       p != var_uses.end();
       ++p)
    {
      annVariable * var = (*p);

      if (pointerOptions::Verbose)
	cout << " + Use of " << var->name() << endl;

      // -- Get the current variable binding

      pointerValue & ptrs = var->get_binding(where);

      // -- This pass serves as the generate_uses() method in
      // Pointers. When we are first computing the pointers, we need to
      // force each use of a memoryBlock to find it's reaching definition.

      for (memoryblock_set_p q = ptrs.blocks.begin();
	   q != ptrs.blocks.end();
	   ++q)
	{
	  memoryBlock * block = *q;

	  // -- Generate a use of the variable

	  generate_one_use(info, def_use_location, search_for_def, block, uses);
	}
    }

  // -- Generate all the uses of property blocks

  const variable_property_map & property_uses = procedure->property_uses();

  // -- For each variable whose properties are accessed

  for (variable_property_map_cp p = property_uses.begin();
       p != property_uses.end();
       ++p)
    {
      annVariable * var = (*p).first;
      const enum_property_set & properties = (*p).second;

      if (pointerOptions::Verbose)
	cout << " + Property uses of " << var->name() << endl;

      // -- Get the current variable binding

      pointerValue & ptrs = var->get_binding(where);

      // -- For each memory block in the binding

      for (memoryblock_set_p q = ptrs.blocks.begin();
	   q != ptrs.blocks.end();
	   ++q)
	{
	  memoryBlock * block = *q;

	  // -- Generate a use for each property accessed

	  for (enum_property_set_p w = properties.begin();
	       w != properties.end();
	       w++)
	    {
	      enumPropertyAnn * property = *w;

	      if (property->is_enabled() &&
		  (property != _annotations->constants_property()))
		{
		  // -- Get the property block

		  memoryBlock * property_block = lookup_property_block(block, property);

		  // -- Generate a use of the property block

		  generate_one_use(info, def_use_location, search_for_def, property_block, uses);
		}
	    }
	}
    }

  // -- Get the list of modify defs

  const var_set & var_defs = procedure->defs();

  // -- Generate a def for each one

  for (var_set_cp p = var_defs.begin();
       p != var_defs.end();
       ++p)
    {
      annVariable * var = (*p);

      if (pointerOptions::Verbose)
	cout << " + Def of " << var->name() << endl;

      // -- Get the current variable binding

      pointerValue & ptrs = var->get_binding(where);

      // -- If there is only one object in the binding, then it is a
      // strong update.

      bool unique_lhs = (ptrs.blocks.size() == 1);

      // -- For each def'd object...

      for (memoryblock_set_p q = ptrs.blocks.begin();
	   q != ptrs.blocks.end();
	   ++q)
	{
	  memoryBlock * block = *q;

	  bool force_weak_update = false;

	  generate_one_def(info, def_use_location, block, block, unique_lhs,
			   defs, uses, changes,force_weak_update);
	}
    }

  // -- Get information about weak updates

  const variable_property_map & weak_property_defs = procedure->property_weak_defs();

  // -- Get the list of defs of property blocks
  
  const variable_property_map & property_defs = procedure->property_defs();

  // -- For each variable whose properties are accessed

  for (variable_property_map_cp p = property_defs.begin();
       p != property_defs.end();
       ++p)
    {
      annVariable * var = (*p).first;
      const enum_property_set & properties = (*p).second;

      if (pointerOptions::Verbose)
	cout << " + Property defs of " << var->name() << endl;

      // -- Get the current variable binding

      pointerValue & ptrs = var->get_binding(where);

      // -- Find any weakly updated properties

      variable_property_map_cp w = weak_property_defs.find(var);

      // -- Generate the property block defs 

      if (w != weak_property_defs.end()) {

	const enum_property_set & weak_properties = (*w).second;
	
	generate_property_defs(info, def_use_location, ptrs,
			       properties, weak_properties,
			       defs, uses, changes);
      }
      else {

	enum_property_set dummy;
	
	generate_property_defs(info, def_use_location, ptrs,
			       properties, dummy,
			       defs, uses, changes);
      }
    }
}


// --- Process the on_exit annotations. Each on_exit annotation is an
// assertion about a points-to setPropertieship. We use the bindings to figure
// out the LHS and RHS, and then call the assignment operator
// appropriately.

void Analyzer::process_on_exit(procedureInfo * info,
			       procedureInfo * caller,
			       procLocation * where,
			       procedureAnn * procedure,
			       memoryblock_set & defs,
			       memoryuse_set & uses,
			       memoryblock_set & changes)
{
  if (pointerOptions::Verbose)
    cout << endl << "-- Evaluate on_exit rules -- " << endl;

  // -- We will allow multiple on_exit assignments to a single variable
  // using a weird system: if we see the same variable as a source a second
  // time, we'll make the assignment additive.

  var_set already_seen;

  // -- Get the def use location

  Location * def_use_location = 0;

  // -- Special case for the "init" procedure: just use the root location
  // as the def-use location:

  if (where == root_location)
    def_use_location = where;
  else
    def_use_location = procedure->def_use_location(where);

  // -- New on-exit implementation: pointer rules

  const pointerrule_list & pointer_rules = procedure->pointer_rules();

  // -- Make sure all the constant values are pre-computed and stored on
  // the idNodeAnn objects.

  if (_property_analyzer)
    _property_analyzer->compute_all_constants(where, procedure);

  // -- Find the rule to use:

  pointerRuleAnn * pointer_rule = 0;

  for (pointerrule_list_cp p = pointer_rules.begin();
       p != pointer_rules.end();
       ++p)
    {
      pointerRuleAnn * cur = (*p);

      // -- Skip the default rule, for the moment

      if (cur != procedure->default_pointer_rule()) {

	// -- Evaluate each rule, stop when we find one that's true

	cur->test(where, _property_analyzer);
	if (cur->is_condition_true()) {
	  pointer_rule = cur;
	  break;
	}
      }
    }

  // -- See if we need to use the default

  if ( ! pointer_rule)
    pointer_rule = procedure->default_pointer_rule();

  // -- Still no rule? Bail out...

  if ( ! pointer_rule)
    return;

  // -- OK, now proceed like we used to:

  if (pointerOptions::Verbose)
    cout << endl << " + Apply on_exit rule: " << endl;

  // const structure_list & on_exit = procedure->on_exit();

  const structure_list & on_exit = pointer_rule->effects();

  for (structure_list_cp p = on_exit.begin();
       p != on_exit.end();
       ++p)
    {
      structureAnn * s = *p;

      // Here we only deal with "arrow" operations. "Dot" has already been
      // dealt with in the allocation of heap objects. Note also that in
      // the semantics of C we cannot replace fields of a struct.

      if (s->field_name().empty()) {

	if (pointerOptions::Verbose)
	  cout << endl << "    + on_exit: " << *s << endl;

	// -- Look up the target binding

	annVariable * target_var = s->target();
	pointerValue & target = target_var->get_binding(where);

	/* Allow this:
	if (target.blocks.empty()) {
	  _annotations->Error(s->line(), string("No bindings for ") + s->target()->name());
	}
	*/
	
	// -- Look up the source variable

	annVariable * source_var = s->source();

	// CASE 2: Regular assignment. Invoke the assignment operator
	// between the source and target.

	pointerValue & source = source_var->get_binding(where);

	/* Allow this:
	   if (source.blocks.empty()) {
	   _annotations->Error(s->line(), string("No bindings for ") + s->source()->name());
	   }
	*/

	// -- In order to create "A --> B", we translate this into "A = &B".

	target.is_address = true;

	already_seen.insert(source_var);

	// -- Invoke the assignment operator

	if (pointerOptions::Verbose) { // debug move down
	  print_memoryblock_set("    Sources: ", source.blocks, cout);
	  print_memoryblock_set("    Targets: ", target.blocks, cout);
	}

	assignment_operator(caller, def_use_location, (stmtLocation *)0,
			    source, target, defs, uses, changes);
      }
    } // -- END for all on_exit
}

void Analyzer::initialize()
{
  // Set up the bindings for the external global variables. There are two
  // type of bindings. For synthetic global variables (ones that exist only
  // in the annotation file), we use the synthetic declNode to create a
  // corresponding memoryBlock. For global variables that represent real
  // global variables in the header files, we use the actual declNode.

  const var_map & globals = _annotations->globals();
  for (var_map_cp p = globals.begin();
       p != globals.end();
       ++p)
    {
      annVariable * glob = (*p).second;
      declNode * decl = 0;

      if (glob->name() != "null") {

	// If this is a global that represents an actual global variable in
	// the application..

	bool unused;
	if (glob->is_external()) {
	  decl = linker.lookup_symbol((unitNode *)0, glob->name(), unused);

	  // -- See if it's right in the header

	  if ( ! decl)
	    decl = _annotations->lookup_header_decl(glob->name());
	}
	else
	  decl = glob->decl();

	if ( decl ) {
	  memoryBlock * mb = Memory.lookup_variable((Location *)0,
						    decl,
						    (procNode *)0);

	  // Bind the variable to that memoryBlock

	  glob->set_binding(mb, 0);

	  // -- If this variable is a property variable..

	  if (glob->is_property_variable()) {

	    // -- Set up the mapping from the memory block back to the property

	    setPropertyAnn * property = _annotations->lookup_set_property(glob->name());
	    if (property) {

	      // -- And tell the property itself

	      property->set_property_variable_block(mb);
	    }
	    else
	      _annotations->Error(0, string("Internal error: property variable \"") + 
				  glob->name() + "\" doesn't match any existing property.\n");
	  }
	}
      }
    }

  // -- Apply the initial analysis information, which resides in the
  // special __init() procedure annotations.

  procedureAnn * init = _annotations->init();

  // -- We should not need a procedureInfo object here

  procedureInfo * info = 0;

  memoryblock_set external_defs;
  memoryuse_set external_uses;
  memoryblock_set external_changes;

  // --- Process on-entry

  process_on_entry(info, root_location, init, true,
		   external_defs, external_uses, external_changes);

  // -- Generate defs and uses (all of these sets are dummies)

  pointerValue return_val;

  create_uses_and_defs(info, root_location, init, false,
		       external_defs, external_uses, external_changes);

  // --- Perform property analysis, if necessary:

  if (_property_analyzer)
    _property_analyzer->analyze_callsite(root_location, init,
					 external_changes, return_val);

  // --- Process on-exit

  process_on_exit(info, (procedureInfo *)0, root_location, init,
		  external_defs, external_uses, external_changes);
}

/** @brief Look up property block
 *
 * Given a regular memory block, look up the special block that
 * represents the given property. When computing pointers we create these
 * blocks as necessary. */

memoryBlock * Analyzer::lookup_property_block(memoryBlock * real_block,
					      enumPropertyAnn * property)
{
  memoryBlock * property_block = property->lookup_property_block(real_block);

  if ( ! property_block) {

    // -- Create a new property block. We'll create it as if it were a
    // struct field of the real block.

    property_block = Memory.generate_su_field(property->name(),
					      (declNode *) 0,
					      real_block);

    // -- Store the secret handle to the property

    property_block->property = property;

    // TB_unify
    if(real_block->is_unify() && !property_block->is_unify()) {
      property_block->set_unify(true);
      pointerOptions::Unify_objects++;
    }

    // -- Store the property block on the real block

    // -- Make sure the vector is initialized

    if (real_block->property_blocks.empty()) {

      // -- Put in a null pointer for each possible enum property

      int size = _annotations->enum_properties().size();
      for (int i = 0; i < size; i++)
	real_block->property_blocks.push_back(0);
    }

    property->add_property_block(real_block, property_block);    

    // -- Propagate the write-protect flag

    if (real_block->write_protected())
      property_block->set_write_protected();

    // -- Figure out how to set flow sensitivity

    property->set_flow_sensitivity(real_block);
  }

  return property_block;
}

/** @brief Lookup a procedure
 *
 * Calls look up on the procedureDB. */

procedureInfo * Analyzer::lookup_procedure(procNode * proc)
{
  return Procedures.lookup(proc);
}


/** @brief Generate one use
 *
 * Used by create_uses_and_defs */

void Analyzer::generate_one_use(procedureInfo * info,
				Location * def_use_location,
				bool search_for_def,
				memoryBlock * block,
				memoryuse_set & uses)
{
  memoryUse * use = block->use_at(def_use_location);

  if (search_for_def) { // computePointers) {
    memoryDef * def = nearest_def_at(info, block, def_use_location);
    use->reaching_def(def);
  }

  if ( ! block->is_return_value())
    uses.insert(use);

  if (pointerOptions::Verbose) {
    cout << "    + Use of " << block->name();
    if (use->reaching_def()) {
      cout << " def at " << * (use->reaching_def()->where()) << endl;
    }
    else
      cout << " no reaching def" << endl;
  }
}

/** @brief Generate property defs
 *
 * This method creates a def for each property block for the given set of
 * properties. Used by create_uses_and_defs and assignment_operator */

void Analyzer::generate_property_defs(procedureInfo * info,
				      Location * def_use_location,
				      pointerValue & ptrs,
				      const enum_property_set & properties,
				      const enum_property_set & weak_properties,
				      memoryblock_set & defs,
				      memoryuse_set & uses,
				      memoryblock_set & changes)
{
  // -- If there is only one object in the binding, then it is a strong
  // update. We'll pass this same value down to the property block that
  // it behaves like the real block.

  bool unique_lhs = (ptrs.blocks.size() == 1);

  // -- For each memory block in the binding

  for (memoryblock_set_p q = ptrs.blocks.begin();
       q != ptrs.blocks.end();
       ++q)
    {
      memoryBlock * block = *q;

      // -- Skip write-protected blocks

      if ( ! block->write_protected()) {

	// -- Generate a def for each property accessed

	for (enum_property_set_cp w = properties.begin();
	     w != properties.end();
	     w++)
	  {
	    enumPropertyAnn * property = *w;

	    if (property->is_enabled() &&
		(property != _annotations->constants_property()))
	      {
		// -- Get the property block

		memoryBlock * property_block = lookup_property_block(block, property);

		// -- See if it's a weak update

		bool force_weak_update = false;

		if (weak_properties.find(property) != weak_properties.end())
		  force_weak_update = true;

		// -- Generate a def of the property block

		generate_one_def(info, def_use_location, property_block, block, unique_lhs,
				 defs, uses, changes, force_weak_update);
	      }
	  }
      }
    }
}

/** @brief Generate one def
 *
 * */

void Analyzer::generate_one_def(procedureInfo * info,
				Location * def_use_location,
				memoryBlock * block,
				memoryBlock * real_block,
				bool unique_lhs,
				memoryblock_set & defs,
				memoryuse_set & uses,
				memoryblock_set & changes,
				bool force_weak_update)
{
  // -- Don't allow defs of write-protected objects

  if ( ! block->write_protected()) {

    bool is_new;
    memoryDef * def = block->def_at(def_use_location, is_new);

#ifdef COLLECT_DEFS
    defs.insert(block);
#endif

    // -- This code for handling weak updates is taken from the
    // Pointers::assignment_operator code. We need to make sure that the
    // "modify" annotation handles weak updates properly.

    // CAREFUL: for property blocks, check the multiplicity on the
    // container, not the property block itself.

    Multiplicity multiplicity = current_multiplicity(info, def_use_location, real_block, uses);

    bool strong_update = unique_lhs && ((multiplicity == Unallocated) ||
					(multiplicity == Deallocated) ||
					(multiplicity == Single));
    // Standard verions: bool strong_update = unique_lhs && (multiplicity == Single);

    if (( ! strong_update) || force_weak_update) {

      /* I'm not sure why we needed the callers info here:
	 memoryDef * previous_def = nearest_def_at(info->current_caller(), block,
	 info->current_where());
      */

      memoryDef * previous_def = nearest_def_at(info, block, def_use_location);

      // def->set_previous(previous_def);
		  
      block->apply_weak_update(def_use_location, previous_def, uses);
    }

    // -- If its a new def, put it on the def list

    if (is_new) {
      changes.insert(block);

      if (pointerOptions::Verbose && ! computePointers) {
	cout << "INTERNAL ERROR no new defs of " << block->name() <<
	  " should be created at " << * def_use_location << endl;
	Procedures.print_call_stack(cout);

	cout << endl;

	block->print_def_use(cout);

	cout << endl;
      }
    }

    if (pointerOptions::Verbose)
      cout << "    + Def of " << block->name() << endl;
  }
}

// ----------------------------------------------------------------------
//  Override Pointers methods
// ----------------------------------------------------------------------

// We override these methods so that when assignments occur we can properly
// assign the special property blocks.

/** @brief Assignment "=" operator
 *
 * When the additive flag is set, the assignment will include the
 * previous value of the pointer at this location. This allows us to say
 * things like "p = &x and p = &y" without having to build a single
 * pointerValue that contains both x and y.
 * */

void Analyzer::assignment_operator(procedureInfo * info,
				   Location * current,
				   stmtLocation * parameter_callsite,
				   pointerValue & left_hand_side,
				   pointerValue & right_hand_side,
				   memoryblock_set & defs,
				   memoryuse_set & uses,
				   memoryblock_set & changes)
{
  // -- First call the superclass version

  Pointers::assignment_operator(info, current, parameter_callsite,
				left_hand_side, right_hand_side,
				defs, uses, changes);

  if (pointerOptions::Verbose)
    cout << "Generate property uses and defs:" << endl;

  // -- Special case: if the right-hand side is just an address, then
  // there's nothing to do.

  if (right_hand_side.is_address)
    return;

  // -- Compile a list of the properties that are being assigned. The nice
  // thing about this approach is that we'll only assign those properties
  // that are actually present on the right-hand side.

  enum_property_set assigned_properties;

  // -- Generate the uses for the right-hand side property blocks

  for (memoryblock_set_p p = right_hand_side.blocks.begin();
       p != right_hand_side.blocks.end();
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

	    // -- Generate a use of the property block

	    generate_one_use(info, current, true, property_block, uses);

	    // -- Record the fact that this property is present on the
	    // right-hand side

	    enumPropertyAnn * the_property = property_block->property;

	    assigned_properties.insert(the_property);
	  }
	}
    }

  // -- Generate the defs for the left-hand side. Here the approach is
  // different: we force each variable on the left-hand side to have a
  // property block for the properties we found on the right.

  enum_property_set dummy;
  generate_property_defs(info, current, left_hand_side,
			 assigned_properties, dummy,
			 defs, uses, changes);
}

/** @brief Get the number of procedures */

void Analyzer::number_of_procedures(int & total, int & analyzed,
				    int & context_insensitive,
				    int & recursive,
				    int & unanalyzed,
				    int & program_size)
{
  Procedures.number_of_procedures(total, analyzed,
				  context_insensitive,
				  recursive,
				  unanalyzed,
				  program_size);
}


/** @brief Setup all bindings
 *
 * This is a public interface to set up the bindings for a particular
 * call site. It is used by the procedureAnn class when it processes the
 * reports or actions. */

void Analyzer::setup_all_bindings(procedureAnn * procedure,
				  operandNode * call,
			    operand_list call_args,
				  procLocation * libproc_location)
{
  // -- Move up to the call site

  stmtLocation * callsite = libproc_location->stmt_location();

  // -- Look up the procedureInfo

  procedureInfo * info = Procedures.lookup(procedure->proc());

  // -- We do need the caller

  procLocation * caller_procloc = Location::procedure(callsite);
  procedureInfo * caller = Procedures.lookup(caller_procloc->proc());

  // -- Make sure we don't alter the pointer information

  computePointers = false;

  // -- Dummy sets

  memoryblock_set local_defs;
  memoryuse_set   local_uses;
  memoryblock_set local_changes;

  // Evaluate the actual arguments, store the results in a list of
  // pointerValues.

  pointervalue_list arguments;
  bool never_returns = false;

  for (operand_list_p p = call_args.begin();
       p != call_args.end();
       ++p)
    {
      operandNode * actual = * p;
      arguments.push_back(pointerValue());
      pointerValue & actual_results = arguments.back();

      // Evaluate the argument and save the results

      actual_results.is_a_use = true;
      eval(caller, callsite, actual, NULL,
	   local_defs, local_uses, local_changes,
	   actual_results, never_returns);
    }

  // -- Pass the parameters

  setup_parameter_bindings(info, procedure, libproc_location,
			   caller, callsite, arguments,
			   local_defs, local_uses, local_changes);

  // --- Set up the binding for the return variable

  annVariable * return_var = procedure->lookup("return", false);
  declNode * return_decl = return_var->decl();

  return_decl->decl_location(declNode::BLOCK);
  memoryBlock * return_block = Memory.lookup_variable(libproc_location, return_decl, procedure->proc());
  return_block->set_return_value();

  return_var->set_binding(return_block, libproc_location);

  // -- Process on-entry

  process_on_entry(info, libproc_location, procedure, false,
		   local_defs, local_uses, local_changes);

  // -- Allocate any new objects generated by the annotations

  allocate_heap_objects(info, caller, libproc_location, callsite, procedure,
			local_defs, local_uses, local_changes);

  // -- Make sure all memoryUse and memoryDef objects are now created. We
  // do this regardless of the computePointers flag because it sets the
  // current_use and current_def fields on each memoryBlock.

  create_uses_and_defs(info, libproc_location, procedure, false,
		       local_defs, local_uses, local_changes);
}

void Analyzer::cs_pass_one_external_output(procedureInfo * callee,
                                           procedureInfo * caller,
                                           stmtLocation * current_callsite,
                                           memoryBlock * block_to_pass,
                                           bool is_return_value) {
  // almost same as Pointers::pass_one_external_output(), except:
  // - use procedure_location() instead of get_context() to obtain
  //   last_stmt_of_proc
  // - remove loop that goes through all callsites; deal only with
  //   current_callsite.
  // - do not do anything on pending_changes.

  // -- Generate a special use inside the procedure (actually at the last
  // statement of the exit block).

  stmtLocation * last_stmt_of_proc
    = callee->procedure_location(current_callsite)->last();

  memoryUse * use = block_to_pass->use_at(last_stmt_of_proc);

  // -- When computing pointers, we also search for the def that reaches
  // the exit of the procedure

  memoryDef * internal_reaching_def = 0;

  if (computePointers) {
    internal_reaching_def = block_to_pass->nearest_def_at(last_stmt_of_proc);
    use->reaching_def(internal_reaching_def);
  }

  internal_reaching_def = use->reaching_def();

  if (pointerOptions::Verbose) {

    cout << "  Interal use at " << * (use->where())
	 << " with reaching def at ";

    memoryDef * def = use->reaching_def();
    if (def)
      cout << * (def->where()) << endl;
    else
      cout << "(no reaching def)" << endl;
  }

  // -- Skip flow insensitive objects

  if (! block_to_pass->is_flow_sensitive()) {
    if (pointerOptions::Verbose)
      cout << endl << "  Skip flow-insensitive external output " << block_to_pass->name() << endl;
    return;
  }

  // -- Test to see if the given memoryBlock is actually visible in the
  // caller's scope. The exception is the return value variable, which
  // must be passed back.

  if (is_return_value ||
      Procedures.is_visible_to(caller, block_to_pass)) {

    if (pointerOptions::Verbose)
      cout << "+ Back to callsite " << * current_callsite << endl;

    // -- Use the assignment operator to pass the value back.

    memoryblock_set pending_changes;

    self_assignment(last_stmt_of_proc, current_callsite,
                    block_to_pass, pending_changes);

    // -- Constant propagation
    
    _constants.at_self_assignment(last_stmt_of_proc, current_callsite,
                                  block_to_pass, pending_changes);
    
    // -- User-defined analysis
    
    if (Problem)
      Problem->at_self_assignment(last_stmt_of_proc, current_callsite,
                                  block_to_pass, pending_changes, false);
  }
  else {
    
    // -- Variable not visible
    
    if (pointerOptions::Verbose)
      cout << "+ Not visible at callsite " << * current_callsite << endl;
  }

  // -- Reset the current def/use information: THIS IS CRITICAL: don't
  // reset the current use and def for the return value, otherwise the
  // assignment won't be able to find the reaching definition.

  if ( ! is_return_value)
    block_to_pass->reset_current_def_use((Location *)0);
} // pass_libfn_external_outputs

