#ifndef ANNOTATION_ANALYSIS_H
#define ANNOTATION_ANALYSIS_H
#include "../ClientProblem.h"
#include "../Utils.h"
#include <map>
#include "broadway.h"

namespace llvm_bdwy {
  
  class AnnotationAnalysis;
  
}

#include "AnnotationPass.h"

namespace llvm_bdwy {
  
  class AnnotationAnalysis : public ClientProblem {
  public:
    
    typedef std::map<Mem_Block*, enumValueAnn* > PropertyMap;
    typedef std::map<Loc*, PropertyMap> PropertyAnalysis;
    
    PropertyMap propMap;
    
    AnnotationAnalysis(AnnotationPass* ap, enumPropertyAnn* p) : annPass(ap), property(p) { }
    virtual void at_lib_call (Proc_Loc *context, std::vector<mem_block_ls_t*>& args, Mem_Def *return_def);
    //rtual void at_stmt_entry (Stmt_Loc *context);
    //rtual void at_stmt_exit (Stmt_Loc *context);
    //rtual void at_return (Stmt_Loc *context, mem_block_ls_t *return_val);
    virtual void analysis_done ();
    void dump_values();
  private:
    
    AnnotationPass* annPass;
    enumPropertyAnn* property;
    list<string> errors;
    list<string> warnings;
    
    void readAnalysisRules(string procName, list< pair<string, string> >& ruleList, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args);
    void handleReports(Proc_Loc* context, const report_list& reports, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args);
    bool evalCondition(const exprAnn* cond, std::map<string, int>& invParamMap, map<string, string>& entryRules, std::vector<mem_block_ls_t*>& args);
  };
}
#endif
