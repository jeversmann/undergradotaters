#ifndef LLVM_BDWY_PROC_ANN_INFO
#define LLVM_BDWY_PROC_ANN_INFO

#include <string>
#include <bdwy/broadway.h>

namespace llvm_bdwy {

class ProcedureAnnotationInfo
{
public:
    ProcedureAnnotationInfo(Annotations* A);
    const structure_list& getEntryRules(const std::string& procName);
    const structure_list getExitRules(const std::string& procName);
    const var_set& getUses(const std::string& procName);
    const var_set& getDefs(const std::string& procName);
    const var_list& getProcParams(const std::string& procName);
    const rule_list getProcAnalysisRules(const std::string& procName, enumPropertyAnn* property);
    const report_list & getProcReports(const std::string& procName);
    
    // Print
    void printEntryRules(const std::string& procName, std::ostream& os);
    void printExitRules(const std::string& procName, std::ostream& os);
    void printUses(const std::string& procName, std::ostream& os);
    void printDefs(const std::string& procName, std::ostream& os);
    
    procedureAnn* lookupProcedure(const std::string& procName);
private:
    Annotations* annotations;
    procedureAnn* lastProcedure;
};

}

#endif
