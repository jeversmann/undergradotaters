
#ifndef BDWY_H
#define BDWY_H

#define TREE
#define REF

#include "c_breeze.h"
#include "pointers.h"
#include "id_lookup_walker.h"
#include "meta.h"

class exprAnn;
class enumPropertyExprAnn;
class procedureAnn;
class analyzeAnn;
class structureAnn;
class Annotations;
class Analyzer;
class propertyAnalyzer;
class idNodeAnn;
class annVariable;
class reportAnn;
class ruleAnn;
class pointerRuleAnn;
class actionAnn;

class propertyAnn;
class enumValueAnn;
class enumPropertyAnn;
class setPropertyAnn;

class enumvalue_set;

class Adaptor_Statistics;  // TB

class Broadway
{
public:

  /** @brief Kinds of operators */

  typedef enum { /* Logical connectives: */    And, Or, Not,
		 /* Property tests: */         Is_Exactly, Is_AtLeast, Could_Be, Is_AtMost, Is_Bottom, Report,
		 /* Numeric tests: */          Evaluate, Is_Constant,
		 /* Pointer alias tests: */    Is_AliasOf, Is_SameAs, Is_Empty,
		 /* Flow-value update: */      Assign,
		 /* Sets: */                   Is_Element_Of, Is_EmptySet, Add_Elements,
		 /* Equivalences: */           Is_Equivalent, Add_Equivalences

  } Operator;

  /** @brief Flow sensitivity modes */

  typedef enum { None, Before, After, Always, Ever,
		 Trace, Confidence
  } FlowSensitivity;
};

#include "ann.h"
#include "annvariable.h"

#include "callingcontext.h"

#include "property.h"
#include "enum_property.h"
#include "set_property.h"

#include "expr.h"

#include "structure.h"
#include "rule.h"
#include "pointerrule.h"
#include "report.h"
#include "action.h"
#include "analyze.h"
#include "procedure.h"

#include "annotations.h"

#include "propertyanalyzer.h"
#include "bdwy_liveness.h"
#include "analyzer.h"

#include "cpattern.h"

#include "actionchanger.h"

#include "adaptor_statistics.h"

#endif /* BDWY_H */
