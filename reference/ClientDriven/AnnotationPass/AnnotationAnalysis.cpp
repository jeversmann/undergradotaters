#include "AnnotationAnalysis.h"
#include "../model/Blocks.h"
#include "../model/Mem_Table.h"
#include "llvm/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Instructions.h"
#include <iostream>
#include <iterator>
#include "../Utils.h"
#include "../model/Blocks.h"

using namespace std;
using namespace llvm;
using namespace llvm_bdwy;

void AnnotationAnalysis::at_lib_call (Proc_Loc *context, std::vector<mem_block_ls_t*>& args, Mem_Def *return_def) {
  
  ProcedureAnnotationInfo* procInfo = annPass->procAnnInfo;
  
  // Get the procedure annotation object from the function name
  
  const string& procName = context->get_function()->getName();
  
  if (!procInfo->lookupProcedure(procName)) return;
  
  /*ferrs() << "AnnotationAnalysis::at_lib_call: " << procName << "\n";
  for (int i = 0; i < args.size(); i++) {
    ferrs() << "\t" << *args[i] << "\n";
  }*/
  
  
  // Construct maps between the memory block lists and the strings for the arguments
  
  vector<string> paramMap;
  map<string, int> invParamMap;
  
  const var_list& params = procInfo->getProcParams(procName);
  int ix = 0;
  for(var_list::const_iterator it = params.begin(); it != params.end(); ++it, ++ix) {
    paramMap.push_back((*it)->name());
    invParamMap[(*it)->name()] = ix;
  }
  
  // Construct a map for all the of the pointer rules
  
  map<string, string> entryRules;
  
  const structure_list& entry = procInfo->getEntryRules(procName);
  for(structure_list::const_iterator it = entry.begin(); it != entry.end(); ++it) {
    entryRules[(*it)->target()->name()] = (*it)->source()->name();
  }
  
  /*map<string, string> exitRules;
  
  const structure_list& entry = procInfo->getExitRules(procName);
  for(structure_list::const_iterator it = entry.begin(); it != entry.end(); it++) {
    exitRules[(*it)->target()->name()] = (*it)->source()->name();
  } */ 
  
  // Get the list of analysis rules for this procedure and iterate through them
  
  list< pair<string, string> > ruleList;
  readAnalysisRules(procName, ruleList, invParamMap, entryRules, args);
  
  for (list< pair<string, string> >::iterator rule_it = ruleList.begin();
	rule_it != ruleList.end(); rule_it++) {
    
    string& lhs = rule_it->first;
    string& rhs = rule_it->second;
    
    if(property->isMayProperty()) {	// May property

      if (enumValueAnn* rhsProp = property->lookup(rhs)) {  	// The RHS is a property value

	// for each mb in lhs
	//   prop[mb] = prop[mb] union {p} 
      
	mem_block_ls_t *lhsBlocks = args[invParamMap[entryRules[lhs]]];    
	
	mem_block_ls_t::iterator it;
	for(it = lhsBlocks->begin(); it != lhsBlocks->end(); ++it) {
	  Mem_Block* mb = *it;
	  //enumValueAnn* lhsProp = propMap[mb];
	  
	  PropertyMap::iterator lhsPair = propMap.find(mb);
	  
	  if (lhsPair != propMap.end()) lhsPair->second = property->meet(lhsPair->second, rhsProp);
	  else propMap[mb] = property->meet(rhsProp, property->default_val());
	}
	
      } else { 	// The RHS is a mem block name

	// S = union prop[mb] for each mb in rhs
	// for each mb in lhs
	//   prop[mb] = prop[mb] union S
	
	mem_block_ls_t *lhsBlocks = args[invParamMap[entryRules[lhs]]];   
	mem_block_ls_t *rhsBlocks = args[invParamMap[entryRules[rhs]]];    
	
	enumValueAnn* temp = property->default_val();
	
	mem_block_ls_t::iterator it = rhsBlocks->begin();
	if (it == rhsBlocks->end()) continue; // Ignore empty mem_block sets
	
	PropertyMap::iterator tempPair = propMap.find(*it); ++it;
	if (tempPair != propMap.end()) temp = tempPair->second;
	
	for(; it != rhsBlocks->end(); ++it) {
	  Mem_Block* mb = *it;
	  
	  PropertyMap::iterator rhsPair = propMap.find(mb);
	  
	  if (rhsPair != propMap.end()) {
	    temp = property->meet(temp, rhsPair->second);
	  }
	}
	
	for(it = lhsBlocks->begin(); it != lhsBlocks->end(); ++it) {
	  Mem_Block* mb = *it;
	  
	  PropertyMap::iterator lhsPair = propMap.find(mb);
	  
	  if (lhsPair != propMap.end()) {
	    lhsPair->second = property->meet(lhsPair->second, temp);
	  } else {
	    propMap[mb] = temp;
	  }
	}
      }
    } else { // Must property
      // TODO
    }
  }
  
  const report_list& reportList = procInfo->getProcReports(procName);
  
  handleReports(context, reportList, invParamMap, entryRules, args);
  
  /*
  const rule_list& rules = procInfo->getProcAnalysisRules(procName, property);
  for(rule_list::const_iterator it = rules.begin(); it != rules.end(); it++) {
    
    // Get all the effects for the rule
    // These are all "Assign" expressions that define the consequences of the rule firing
    
    exprann_list effects = (*it)->effects();
    
    // Loop through the expressions corresponding to this rule
    
    exprann_list::iterator expr_it;
    for(expr_it = effects.begin(); expr_it != effects.end(); expr_it++) {
      enumPropertyExprAnn* expr = dynamic_cast<enumPropertyExprAnn*>(*expr_it);
    
      // NON-Null expr
      
      if (expr) {
	
	string lhs = expr->lhs_name();
	string rhs = expr->rhs_name();
	
	if (invParamMap.find(rhs) != invParamMap.end()) {
	  
	  // The RHS is a memory location
	  
	  // TODO
	} else {
	  
	  // The RHS is a property
	  
	  if(property->isMayProperty()) {
	    
	    // On a may property, union stuff
	    
	    mem_block_ls_t *lhblocks = args[invParamMap[ptrRules[lhs]]];
	    
	    mem_block_ls_t::iterator it;
	    for(it = lhblocks->begin(); it != lhblocks->end(); it++) {
	      Mem_Block* mb = *it;
	      propMap[mb].insert(rhs);
	    }
	    
	  }
	  else {
	    // TODO
	  }
	  
	}
      } else assert(false);
    }
  }*/
  
  dump_values();
  
  
  
  
}

void AnnotationAnalysis::handleReports(Proc_Loc* context, const report_list& reports, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args) {
  report_list::const_iterator it;
  for(it = reports.begin(); it != reports.end(); it++) {
    reportAnn* report = *it;
    
    const exprAnn* cond = report->getCondition();
    
    if(!cond || evalCondition(cond, invParamMap, entryRules, args)) {
      // Error
      if (report->isError()) {
	const report_element_list& eleList = report->getElementsList();
	for (report_element_list::const_iterator e_it = eleList.begin(); e_it != eleList.end(); ++e_it) {
	  
	  literalReportElementAnn* litReport = static_cast<literalReportElementAnn*>(*e_it);
	  
	  string errString = litReport->getString();
	  errors.push_back(errString);
	  
	  log() << "Error! " << *context->parent << ": " << errString << "\n";
	}
      }
      else { // Report
	const report_element_list& eleList = report->getElementsList();
	for (report_element_list::const_iterator e_it = eleList.begin(); e_it != eleList.end(); ++e_it) {
	  
	  literalReportElementAnn* litReport = static_cast<literalReportElementAnn*>(*e_it);
	  
	  string errString = litReport->getString();
	  warnings.push_back(errString);
	  
	  log() << "Warning - " << *context->parent << ": " << errString << "\n";
	}
      }
    }
    
  }
  
}

bool AnnotationAnalysis::evalCondition(const exprAnn* cond, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args) {
  
  // Again, ugly, but we only support one kind of condition
  
  const enumPropertyExprAnn* enumExpr = static_cast<const enumPropertyExprAnn*>(cond);
  
  string lhs = enumExpr->lhs_name();
  string rhs = enumExpr->rhs_name();
  
  enumValueAnn* rhsProp = property->lookup(rhs);
  if (!rhsProp) return false;
  
  set<string> possibleProps;
  mem_block_ls_t *lhsBlocks = args[invParamMap[entryRules[lhs]]];
  
  for (mem_block_ls_t::iterator it = lhsBlocks->begin(); it != lhsBlocks->end(); ++it) {
      
    Mem_Block* mb = *it;
    PropertyMap::iterator lhsPair = propMap.find(mb);
    
    if (lhsPair != propMap.end()) {
      possibleProps.insert(lhsPair->second->name());
    } else possibleProps.insert(property->default_val()->name());
  }
  
  if (possibleProps.empty()) possibleProps.insert(property->default_val()->name());
  
  switch (enumExpr->op()) {
    
    case Broadway::Is_Exactly:
	for (set<string>::iterator it = possibleProps.begin(); it != possibleProps.end(); ++it) {
	  if (*it != rhs) return false;
	}
	return true;
      break;
      
    case Broadway::Is_AtLeast:
	for (set<string>::iterator it = possibleProps.begin(); it != possibleProps.end(); ++it) {
	  if (!property->at_least(property->lookup(*it), rhsProp)) return false;
	}
	return true;
      break;
      
    case Broadway::Could_Be:	
	for (set<string>::iterator it = possibleProps.begin(); it != possibleProps.end(); ++it) {
	  enumValueAnn* lhsProp = property->lookup(*it);
	  enumValueAnn* meet = property->meet(lhsProp, rhsProp);
	  if (lhsProp->name() == meet->name()) {
	    return true;
	  }
	}
	return false;
      break;
      
    case Broadway::Is_AtMost:
	for (set<string>::iterator it = possibleProps.begin(); it != possibleProps.end(); ++it) {
	  if (!property->at_most(property->lookup(*it), rhsProp)) return false;
	}
	return true;
      break;
    
    default:
      return false;
  }
}


void AnnotationAnalysis::analysis_done() {

  log() << "Annotations Analysis Complete\n";
  dump_values();
}

void AnnotationAnalysis::dump_values() {
  
  log() << "Annotation Analysis Summary:" << "\n";
  log_push();
  
  for (PropertyMap::iterator it = propMap.begin(); it != propMap.end(); ++it) {
    if (it->second) {
      enumvalue_list propList = it->second->more_specific();
      raw_ostream& os = log();
      os << *(it->first) << " could be: { ";
      for (enumvalue_list::iterator it2 = propList.begin(); it2 != propList.end(); ++it2) {
	os << " " << (*it2)->name() << ",";
	//enumvalue_list newList = (*it2)->more_specific();
	//propList.insert(
      }
      os << " " << it->second->name() << " } \n";
    }
  }
  
  int n = 1;
  for (list<string>::iterator it = errors.begin(); it != errors.end(); ++it, ++n) {
    log() << "Error " << n << ": " << *it << "\n";
  }
  
  n = 1;
  for (list<string>::iterator it = warnings.begin(); it != warnings.end(); ++it, ++n) {
    log() << "Warning " << n << ": " << *it << "\n";
  }
  
  log_pop();
}

void AnnotationAnalysis::readAnalysisRules(string procName, list< pair<string, string> >& ruleList, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args) {
  
  // Iterate over rule list for this property?
  
  const rule_list& rules = annPass->procAnnInfo->getProcAnalysisRules(procName, property);
  for(rule_list::const_iterator it = rules.begin(); it != rules.end(); it++) {
    
    // Each rule consists of condition + effects
    
    const exprAnn* cond = (*it)->condition();
    
    if(!cond || evalCondition(cond, invParamMap, entryRules, args)) {
    
      const exprann_list& effects = (*it)->effects();
      exprann_list::const_iterator expr_it;
      for(expr_it = effects.begin(); expr_it != effects.end(); expr_it++) {
	
	exprAnn* expr = *expr_it;
	
	// Since we only handle enumProperties we're just casting to enumExpr
	// Ugly and probably fails on something but eh
	
	enumPropertyExprAnn* enumExpr = static_cast<enumPropertyExprAnn*>(expr);
	
	string lhs = enumExpr->lhs_name();
	string rhs = enumExpr->rhs_name();
	ruleList.push_back(make_pair(lhs, rhs));
      }
    
    }
  }
}
