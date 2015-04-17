
#include "c_breeze.h"
#include "precision.h"

precisionAnalyzer::precisionAnalyzer()
  : analysisProblem(Forward),
    _pointer_targets(),
    _derefs(0)
{}

/** @brief At each field access
 *
 * Count the number of targets if this is an -> operator. */

void precisionAnalyzer::at_field_access(stmtLocation * current,
					binaryNode * binary,
					pointerValue & operand,
					idNode * field,
					pointerValue & result)
{
  if (binary->op()->id() != '.')
    record(current, operand, result);
}



/** @brief At each dereference
 *
 * Count the number of targets and store it in the deref map. */

void precisionAnalyzer::at_dereference(stmtLocation * current,
				       unaryNode * unary,
				       pointerValue & operand,
				       pointerValue & result)
{
  record(current, operand, result);
}

/** @brief At an array index
 *
 * Array index is a dereference. */

void precisionAnalyzer::at_index(stmtLocation * current,
				 binaryNode * binary,
				 pointerValue & left,
				 pointerValue & right,
				 pointerValue & result)
{
  record(current, left, result);
}

/** @brief Report
 *
 * Print the results of the precision analysis. */

void precisionAnalyzer::report(ostream & out)
{
  long int total_targets = 0;
  long int total_size = 0;
  long int def_targets = 0;
  long int def_size = 0;
  long int use_targets = 0;
  long int use_size = 0;

  typedef map< stmtNode *, int > stmt_size_map;
  typedef stmt_size_map::iterator stmt_size_map_p;

  stmt_size_map statements;

  for (pointer_targets_map_p p = _pointer_targets.begin();
       p != _pointer_targets.end();
       ++p)
    {
      stmt_pointer_pair spp = (*p).first;
      memoryblock_set & targets = (*p).second;
      stmtLocation * loc = spp.first;
      memoryBlock * ptr = spp.second;

      total_targets += targets.size();
      total_size++;

      // -- Get the max target set size at each statement (this removes
      // context-specific information).

      stmtNode * stmt = loc->stmt();
      stmt_size_map_p found = statements.find(stmt);
      int max = 0;

      if (found != statements.end())
	max = statements[stmt];

      if ((int)targets.size() > max)
	statements[stmt] = max;

      out << "------------------------------------------------------------" << endl;
      out << ptr->name() << "   " << * loc << "   " << targets.size() << endl;

      output_context oc(out);
      stmt->output(oc, 0);
      out << endl << endl;

      // -- See if this is a definition and print out the targets

      bool is_def = false;

      for (memoryblock_set_p q = targets.begin();
	   q != targets.end();
	   q++)
	{
	  memoryBlock * target = (*q);
	  out << "                     --> " << target->name() << endl;
	  if (target->find_def_at(loc) != 0)
	    is_def = true;
	}

      if (is_def) {
	def_targets += targets.size();
	def_size++;
      }
      else {
	use_targets += targets.size();
	use_size++;
      }

      // cout << "Report at " << spp.second->name() << ", size = " << targets.size();
    }

  double t = (double) total_targets;
  double s = (double) total_size;

  if (total_size != 0)
    out << "Average points-to size: " << (t/s)
	<< " = (" << t << "/" << s << ")" << endl;
  else
    out << "No dereferences" << endl;

  t = (double) def_targets;
  s = (double) def_size;

  if (def_size != 0)
    out << "Average points-to size at a definition: " << (t/s) 
	<< " = (" << t << "/" << s << ")" << endl;
  else
    out << "No dereferences at definitions" << endl;

  t = (double) use_targets;
  s = (double) use_size;

  if (use_size != 0)
    out << "Average points-to size at a use: " << (t/s) 
	<< " = (" << t << "/" << s << ")" << endl;
  else
    out << "No dereferences at uses" << endl;

  long int stmt_targets = 0;
  long int stmt_size = 0;

  for (stmt_size_map_p p = statements.begin();
       p != statements.end();
       ++p)
    {
      stmt_targets += (*p).second;
      stmt_size++;
    }

  t = (double) stmt_targets;
  s = (double) stmt_size;

  if (stmt_size != 0)
    out << "Average points-to size at each statement: " << (t/s) 
	<< " = (" << t << "/" << s << ")" << endl;
  else
    out << "No dereferences at statements" << endl;

  out << "Total deref cout: " << _derefs << endl;
}

void precisionAnalyzer::record(stmtLocation * current,
			       pointerValue & operand,
			       pointerValue & result)
{
  memoryblock_set & pointers = operand.blocks;
  memoryblock_set & targets = result.blocks;

  _derefs++;

  // -- Skip address objects

  if ( ! operand.is_address) {

    if (pointerOptions::Verbose) {
      if (pointers.empty()) {
	cout << "------------------------------------------------------------" << endl;
	cout << "WARNING: No pointer operand at " << * current << endl;
	stmtNode * stmt = current->stmt();
	output_context oc(cout);
	stmt->output(oc, 0);
	cout << endl << endl;
      }
    }

    for (memoryblock_set_p p = pointers.begin();
	 p != pointers.end();
	 ++p)
      {
	memoryBlock * pointer = *p;

	// -- The map key is the location and the pointer

	stmt_pointer_pair spp(current, pointer);

	// -- Get the target set

	memoryblock_set & target_set = _pointer_targets[spp];

	// -- Add the targets from this 

	target_set.insert(targets.begin(), targets.end());

	// cout << "At " << pointer->name() << ", size = " << targets.size() << endl;
      }
  }
}

