#include <ProcedureAnnotationInfo.h>

using namespace std;
using namespace llvm_bdwy;

ProcedureAnnotationInfo::ProcedureAnnotationInfo(Annotations* A) : annotations(A) {}


procedureAnn* ProcedureAnnotationInfo::lookupProcedure(const std::string& procName) {
  if(!lastProcedure || procName != lastProcedure->name()) {
    lastProcedure = annotations->lookup_procedure(procName);
  }
  
  return lastProcedure;
}

// Access Functions

const structure_list& ProcedureAnnotationInfo::getEntryRules(const string& procName) {

  procedureAnn* p = lookupProcedure(procName);
  return p->on_entry();
}

const structure_list ProcedureAnnotationInfo::getExitRules(const string& procName) {

  procedureAnn* p = lookupProcedure(procName);
  structure_list rules;
  
  pointerrule_list prules = p->pointer_rules();
  for (pointerrule_list::iterator it = prules.begin(); it != prules.end(); it++) {
    rules.insert(rules.end(), (*it)->effects().begin(), (*it)->effects().end());
  }
  
  rules.insert(rules.end(), p->on_exit().begin(), p->on_exit().end());
  
  return rules;
}

const var_set& ProcedureAnnotationInfo::getUses(const string& procName) {

  procedureAnn* p = lookupProcedure(procName);
  return p->uses();
}

const var_set& ProcedureAnnotationInfo::getDefs(const string& procName) {

  procedureAnn* p = lookupProcedure(procName);
  return p->defs();
}

const var_list& ProcedureAnnotationInfo::getProcParams(const std::string& procName) {

  procedureAnn* p = lookupProcedure(procName);
  return p->formal_params();
}

const rule_list ProcedureAnnotationInfo::getProcAnalysisRules(const std::string& procName, enumPropertyAnn* property) {
  
    procedureAnn* p = lookupProcedure(procName);
    analyze_map::const_iterator it = p->analyses().find(property);
    
    if (it != p->analyses().end()) {
      return it->second->getRules();
    } else {
      rule_list rl;
      return rl;
    }
}

const report_list& ProcedureAnnotationInfo::getProcReports(const std::string& procName) {
    procedureAnn* p = lookupProcedure(procName);
    return p->reports();
}


// Print Functions

void ProcedureAnnotationInfo::printEntryRules(const string& procName, std::ostream& os) {
  procedureAnn* proc = lookupProcedure(procName);
  const structure_list& entry = proc->on_entry();
  structure_list::const_iterator it;

  os << "Enter Rules for Procedure: " << proc->name() << endl;
  for( it = entry.begin(); it != entry.end(); it++) {
	  os << "\t" << (**it) << endl;
  }
}

void ProcedureAnnotationInfo::printExitRules(const string& procName, std::ostream& os) {
  procedureAnn* proc = lookupProcedure(procName);
  const structure_list& exit = proc->on_exit();
  structure_list::const_iterator it;

  os << "Exit Rules for Procedure: " << proc->name() << endl;
  for( it = exit.begin(); it != exit.end(); it++) {
	  os << "\t" << (**it) << endl;
  }
}

void ProcedureAnnotationInfo::printUses(const string& procName, std::ostream& os) {
  procedureAnn* proc = lookupProcedure(procName);
  const var_set& uses = proc->uses();
  var_set::const_iterator it;

  os << "Uses for Procedure: " << proc->name() << endl;
  for( it = uses.begin(); it != uses.end(); it++) {
	  os << "\t" << (*it)->name() << endl;
  }

}

void ProcedureAnnotationInfo::printDefs(const string& procName, std::ostream& os) {
  procedureAnn* proc = lookupProcedure(procName);
  const var_set& defs = proc->defs();
  var_set::const_iterator it;

  os << "Defs for Procedure: " << proc->name() << endl;
  for( it = defs.begin(); it != defs.end(); it++) {
	  os << "\t" << (*it)->name() << endl;
  }
}
