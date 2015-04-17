%{

#include "broadway.h"

extern int annlex(void);
extern int annlineno;
extern int line_number_offset;
extern char * anntext;

// -- Utility functions -- defined in c_breeze/src/main/parser.y

declNode::Storage_class merge_sc(declNode::Storage_class sc1,
				 declNode::Storage_class sc2,
				 const Coord c = Coord::Unknown);

struct _TQ merge_tq(struct _TQ ts1,
		    struct _TQ ts2);

// --- Global state variables ----------------

void annerror(const char * msg)
{
  Annotations::Current->Error(annlineno - line_number_offset, string(msg) + " near \"" +
			      string(anntext) + "\"");
}

%}

%union {

  /* --- Broadway --- */

  parserID *                  t_id;
  parserid_list *             t_ids;

  int                         t_tok;
  string *                    t_string;

  structureTreeAnn *          t_structure;
  structuretree_list *        t_structures;

  exprNode *                  t_c_expression;

  enumValueAnn *              t_enumvalue;
  enumvalue_list *            t_enumvalue_list;

  exprAnn *                   t_expr;
  exprann_list *              t_exprs;

  analyzeAnn *                t_analyze;

  ruleAnn *                   t_rule;
  rule_list *                 t_rules;

  reportAnn *                 t_report;
  report_element_list *       t_report_elements;
  reportElementAnn *          t_report_element;

  struct {
    int                              line;
    Broadway::Operator                op;
  } t_property_operator;

  struct {
    int                              line;
    Broadway::FlowSensitivity fs;
  } t_property_sensitivity;

  Direction                   t_direction;

  /* --- C Code --- */

  primNode *          primN;
  sueNode *           sueN;
  enumNode *          enumN;
  exprNode *          exprN;
  constNode *         constN;
  idNode *            idN;
  initializerNode *   initializerN;
  unaryNode *         unaryN;
  binaryNode *        binaryN;
  stmtNode *          stmtN;
  exprstmtNode *      exprstmtN;
  blockNode *         blockN;
  targetNode *        targetN;
  loopNode *          loopN;
  jumpNode *          jumpN;
  declNode *          declN;
  attribNode *        attribN;
  procNode *          procN;

  def_list  * defL;
  stmt_list * stmtL;
  decl_list * declL;
  attrib_list * attribL;
  expr_list * exprL;

  /* type: type nodes must also carry storage class */
  struct _TYPE typeN;

  /* tq: type qualifiers, storage class */
  struct _TQ tq;

  /* tok: token coordinates */
  simpleCoord  tok;
}

/* ------------------------------------------------------------

      Broadway

   ------------------------------------------------------------
*/

%token <t_id> tokID
%token <t_c_expression> tokCONSTANT

%token <t_tok> tokENABLE tokDISABLE

%token <t_tok> tokMAYPROPERTY
%token <t_tok> tokMUSTPROPERTY
%token <t_tok> tokGLOBAL
%token <t_tok> tokINITIALLY
%token <t_tok> tokDIAGNOSTIC /* TB new */

%token <t_tok> tokPROCEDURE
%token <t_tok> tokACCESS
%token <t_tok> tokMODIFY

%token <t_tok> tokANALYZE
%token <t_tok> tokDEFAULT
%token <t_tok> tokIF

%token <t_tok> tokIS_EXACTLY
%token <t_tok> tokIS_ATLEAST
%token <t_tok> tokCOULD_BE
%token <t_tok> tokIS_ATMOST
%token <t_tok> tokIS_BOTTOM

%token <t_tok> tokUNION_SET
%token <t_tok> tokINTERSECT_SET
%token <t_tok> tokUNION_EQUIV
%token <t_tok> tokINTERSECT_EQUIV

%token <t_tok> tokIS_ELEMENT_OF
%token <t_tok> tokIS_EMPTYSET
%token <t_tok> tokADD

%token <t_tok> tokALWAYS
%token <t_tok> tokEVER
%token <t_tok> tokBEFORE
%token <t_tok> tokAFTER
%token <t_tok> tokTRACE
%token <t_tok> tokCONFIDENCE

%token <t_tok> tokFORWARD
%token <t_tok> tokBACKWARD

%token <t_tok> tokIS_ALIASOF
%token <t_tok> tokIS_SAMEAS
%token <t_tok> tokIS_EMPTY

%token <t_tok> tokON_ENTRY
%token <t_tok> tokON_EXIT
%token <t_tok> tokNEW
%token <t_tok> tokARROW
%token <t_tok> tokDELETE

%token <t_tok> tokPATTERN

%token <t_tok> tokANDAND
%token <t_tok> tokOROR

%token <t_tok> tokNOT_EQUALS
%token <t_tok> tokEQUALS_EQUALS
%token <t_tok> tokLESS_EQUALS
%token <t_tok> tokGREATER_EQUALS

%token <t_tok> tokIS_CONSTANT

%token <t_tok> tokASSIGN
%token <t_tok> tokWEAKASSIGN
%token <t_tok> tokEQUALS

%token <t_tok> tokREPORT
%token <t_tok> tokERROR
%token <t_tok> tokAPPEND

%token <t_tok> tokWHEN
%token <t_tok> tokREPLACE
%token <t_tok> tokINLINE

%token <t_tok> tokIO

%token <t_id> tokSTRING

%token <t_tok> ':' '-' ',' ';' '!' '/' '^' '~' '(' ')' '{' '}'
%token <t_tok> '*' '&' '%' '+' '|' '.' '$' '@'

%nonassoc WEAK_REDUCE
%nonassoc LOWER_THAN_EXPRESSION
%left tokANDAND tokOROR
%left tokIS_EXACTLY tokIS_ATLEAST tokCOULD_BE tokIS_ATMOST tokIS_BOTTOM
%left tokEQUALS tokEQUALS_EQUALS tokNOT_EQUALS tokLESS_EQUALS tokGREATER_EQUALS
%left tokASSIGN tokWEAKASSIGN
%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%nonassoc LOWER_THAN_UNARY
%nonassoc '~' '!'
%nonassoc LOWER_THAN_POSTFIX
%right '@' '$'
%nonassoc '('
%nonassoc PARENTHESIZED

%type <t_structures> structures
%type <t_structure> structure

%type <t_ids> identifier_list optional_diagnostic;
%type <t_id> qualified_identifier
%type <t_c_expression> c_compare_expression c_expression

%type <t_property_operator> binary_property_operator
%type <t_property_sensitivity> property_sensitivity

%type <t_analyze> analyze_annotation

%type <t_enumvalue_list> user_defined_class;
%type <t_enumvalue_list> members
%type <t_enumvalue> member;

%type <t_direction> optional_direction;
%type <t_id> optional_default;

%type <t_rules> rules
%type <t_rule> rule
%type <t_exprs> effects
%type <t_expr> condition test effect

%type <t_report> report_annotation
%type <t_report_elements> report_element_list
%type <t_report_element> report_element

/* ------------------------------------------------------------

      C Code

   ------------------------------------------------------------
*/

/* Define terminal tokens */

%token <tok> ctokAND ctokSTAR ctokPLUS ctokMINUS ctokTILDE ctokNOT
%token <tok> ctokLESS ctokGREATER ctokOR ctokHAT ctokPERCENT ctokDIVIDE
%token <tok> ctokLEFTPAREN ctokRIGHTPAREN ctokDOT ctokQUESTION ctokSEMICOLON
%token <tok> ctokLEFTBRACE ctokRIGHTBRACE ctokCOMMA ctokLEFTBRACKET ctokRIGHTBRACKET ctokCOLON

/* ANSI keywords, extensions below */
%token <tok> ctokAUTO            ctokDOUBLE          ctokINT             ctokSTRUCT
%token <tok> ctokBREAK           ctokELSE            ctokLONG            ctokSWITCH
%token <tok> ctokCASE            ctokENUM            ctokREGISTER        ctokTYPEDEF
%token <tok> ctokCHAR            ctokEXTERN          ctokRETURN          ctokUNION
%token <tok> ctokCONST           ctokFLOAT           ctokSHORT           ctokUNSIGNED
%token <tok> ctokCONTINUE        ctokFOR             ctokSIGNED          ctokVOID
%token <tok> ctokDEFAULT         ctokGOTO            ctokSIZEOF          ctokVOLATILE
%token <tok> ctokDO              ctokIF              ctokSTATIC          ctokWHILE

/* unary op ctokens added by Eric Brewer */

%token <tok> ctokUPLUS ctokUMINUS ctokINDIR ctokADDRESS ctokPOSTINC ctokPOSTDEC ctokPREINC ctokPREDEC ctokBOGUS


/* ANSI Grammar suggestions */
%token <idN>    IDENTIFIER
%token <constN> STRINGliteral
%token <constN> FLOATINGconstant
%token <constN> INTEGERconstant OCTALconstant HEXconstant WIDECHARconstant
%token <constN> CHARACTERconstant

/* New Lexical element, whereas ANSI suggested non-terminal */

/* 
   Lexer distinguishes this from an identifier.
   An identifier that is CURRENTLY in scope as a typedef name is provided
   to the parser as a TYPEDEFname
*/
%token <idN> TYPEDEFname 

/* Multi-Character operators */
%token <tok>  ctokARROW            /*    ->                              */
%token <tok>  ctokICR ctokDECR         /*    ++      --                      */
%token <tok>  ctokLS ctokRS            /*    <<      >>                      */
%token <tok>  ctokLE ctokGE ctokEQ ctokNE      /*    <=      >=      ==      !=      */
%token <tok>  ctokANDAND ctokOROR      /*    &&      ||                      */
%token <tok>  ctokELLIPSIS         /*    ...                             */

/* modifying assignment operators */
%token <tok> ctokEQUALS
%token <tok> ctokMULTassign  ctokDIVassign    ctokMODassign   /*   *=      /=      %=      */
%token <tok> ctokPLUSassign  ctokMINUSassign              /*   +=      -=              */
%token <tok> ctokLSassign    ctokRSassign                 /*   <<=     >>=             */
%token <tok> ctokANDassign   ctokERassign     ctokORassign    /*   &=      ^=      |=      */

/* GCC extensions */
%token <tok> ctokINLINE
%token <tok> ctokATTRIBUTE

/* -- OLD Meta programming -- */

%token <tok> ctokMETA_TYPE_EXPR
%token <tok> ctokMETA_TYPE_STMT

%token <idN> ctokMETA_EXPR
%token <idN> ctokMETA_STMT

/* -- Generated list -- */

/* %type <programN> prog.start */

/* -- For macros -- */

%type <idN> identifier
%type <idN> typedefname

/* -- Expression -- */

%type <exprN> primary.expression
%type <exprN> postfix.expression
%type <exprL> argument.expression.list
%type <exprN> unary.expression
%type <unaryN> unary.operator
%type <exprN> cast.expression
%type <exprN> multiplicative.expression
%type <exprN> additive.expression
%type <exprN> shift.expression
%type <exprN> relational.expression
%type <exprN> equality.expression
%type <exprN> AND.expression
%type <exprN> exclusive.OR.expression
%type <exprN> inclusive.OR.expression
%type <exprN> logical.AND.expression
%type <exprN> logical.OR.expression
%type <exprN> conditional.expression
%type <exprN> assignment.expression
%type <binaryN> assignment.operator
%type <exprN> expression
%type <exprN> constant.expression
%type <exprN> expression.opt

/* -- Declaration -- */

%type <declL> declaration
%type <declL> declaring.list
%type <declL> default.declaring.list
%type <typeN> declaration.specifier
%type <typeN> basic.declaration.specifier
%type <typeN> sue.declaration.specifier
%type <typeN> typedef.declaration.specifier
%type <tq> declaration.qualifier.list
%type <tq> declaration.qualifier

/* -- Type specifier -- */

%type <typeN> type.specifier
%type <typeN> basic.type.specifier
%type <typeN> sue.type.specifier
%type <typeN> typedef.type.specifier
%type <tq> type.qualifier.list
%type <tq> pointer.type.qualifier.list
%type <typeN> elaborated.type.name

/* -- Declarator -- */

%type <declN> declarator
%type <declN> paren.typedef.declarator
%type <declN> paren.postfix.typedef.declarator
%type <declN> simple.paren.typedef.declarator
%type <declN> parameter.typedef.declarator
%type <declN> clean.typedef.declarator
%type <declN> clean.postfix.typedef.declarator

/* -- Abstract declarator -- */

%type <typeN> abstract.declarator
%type <typeN> unary.abstract.declarator
%type <typeN> postfix.abstract.declarator
%type <typeN> postfixing.abstract.declarator
%type <typeN> array.abstract.declarator

/* -- Identifier declarator -- */

%type <declN> identifier.declarator
%type <declN> unary.identifier.declarator
%type <declN> postfix.identifier.declarator
%type <declN> paren.identifier.declarator

/* -- Old-style function declarator -- */

%type <declN> old.function.declarator
%type <declN> postfix.old.function.declarator
%type <declL> identifier.list
%type <idN> identifier.or.typedef.name
%type <typeN> type.name

/* -- Attributes -- */

%type <attribL> attributes.opt
%type <attribL> attributes
%type <attribL> attribute
%type <attribL> attribute.list
%type <attribN> attrib
%type <idN> any.word
%type <exprN> initializer.opt
%type <exprN> initializer
%type <initializerN> initializer.list
%type <declL> parameter.type.list
%type <declL> parameter.list
%type <declN> parameter.declaration

/* -- Struct, union, enum -- */

%type <sueN> struct.or.union.specifier
%type <sueN> struct.or.union
%type <declL> struct.declaration.list
%type <declL> struct.declaration
%type <declL> struct.default.declaring.list
%type <declL> struct.declaring.list
%type <declN> struct.declarator
%type <declN> struct.identifier.declarator
%type <exprN> bit.field.size.opt
%type <exprN> bit.field.size
%type <enumN> enum.specifier
%type <declL> enumerator.list
%type <exprN> enumerator.value.opt
%type <tok> comma.opt

/* -- Statements -- */

%type <stmtN> statement
%type <targetN> labeled.statement
%type <blockN> compound.statement
%type <tok> ctokLEFT
%type <tok> ctokRIGHT
%type <tok> left.enter.pragma
%type <tok> right.exit.pragma
%type <blockN> compound.statement.no.new.scope
%type <declL> declaration.list
%type <stmtL> statement.list
%type <exprstmtN> expression.statement
%type <stmtN> selection.statement
%type <loopN> iteration.statement
%type <jumpN> jump.statement

/* -- Translation Unit -- */

/* %type <programN> translation.unit */
%type <defL> external.definition
%type <procN> function.definition
%type <declL> old.function.declaration.list

/* -- Little stuff -- */

%type <exprN> constant
%type <constN> string.literal.list
%type <tq> type.qualifier
%type <tq> pointer.type.qualifier
%type <tq> storage.class
%type <primN> basic.type.name

/* ------------------------------------------------------------

      C patterns

   ------------------------------------------------------------
*/

%token <t_id>  metatokID
%token <t_tok> metatokEXPR
%token <t_tok> metatokSTMT
%token <t_tok> metatokTYPE

%type <exprN> expr_pattern
%type <typeN> type_pattern
%type <stmtN> stmt_pattern

%%

annotation_file:
    annotation
  | annotation_file annotation
  ;

annotation:
    global
  | property
  | tokENABLE tokID
      { Annotations::Enabled_properties.push_back($2->name()); }
  | tokDISABLE tokID
      { Annotations::Disabled_properties.push_back($2->name()); }
  | procedure
  | pattern
  | header
  | analyze_annotation
      {
        Annotations::Current->init()->add_analysis($1);
      }
  ;

header:
    declaration.list
      { Annotations::Current->store_header($1); }
  ;

/* --- Global variables ----------------------- */

global:
    tokGLOBAL '{' structures '}'
      {
        Annotations::Current->add_globals( $3 );
        delete $3;
      }

  | tokGLOBAL structure
      {
        structuretree_list * temp = new structuretree_list();
	temp->push_back( $2);
        Annotations::Current->add_globals( temp );
      }
  ;

/* --- Property declarations ------------------ */

property:
    tokMAYPROPERTY tokID ':' optional_direction user_defined_class optional_diagnostic optional_default
      {
        Annotations::Current->add_enum_property(new enumPropertyAnn($2, $4, true, $7, $5, $6));
	delete $2;
	delete $5;
	delete $6;
      }

  | tokMUSTPROPERTY tokID ':' optional_direction user_defined_class optional_diagnostic optional_default
      {
        Annotations::Current->add_enum_property(new enumPropertyAnn($2, $4, false, $7, $5, $6));
	delete $2;
	delete $5;
	delete $6;
      }

  | tokMAYPROPERTY tokID ':' tokUNION_SET
      {
        Annotations::Current->add_set_property(new setPropertyAnn($2, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Union));
	delete $2;
      }

  | tokMUSTPROPERTY tokID ':' tokINTERSECT_SET
      {
        Annotations::Current->add_set_property(new setPropertyAnn($2, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Intersect));
	delete $2;
      }

  | tokMAYPROPERTY tokID ':' tokUNION_EQUIV
      {
        Annotations::Current->add_set_property(new setPropertyAnn($2, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Union));
	delete $2;
      }

  | tokMUSTPROPERTY tokID ':' tokINTERSECT_EQUIV
      {
        Annotations::Current->add_set_property(new setPropertyAnn($2, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Intersect));
	delete $2;
      }

  ;

optional_direction:
    {
      $$ = Forward;
    }

  | tokFORWARD
    {
      $$ = Forward;
    }

  | tokBACKWARD
    {
      $$ = Backward;
    }
;

optional_default:

      {
        $$ = (parserID *)0;
      }

  | tokINITIALLY tokID
      {
	$$ = $2;
      }
  ;

optional_diagnostic: /* TB new */
    { $$ = NULL; }
  | tokDIAGNOSTIC '{' identifier_list '}'
    { $$ = $3; }
  ;

user_defined_class:
    '{' members '}'
      {
	$$ = $2;
      }
  ;

members:
    member
      {
        $$ = new enumvalue_list();
	$$->push_back($1);
      }

  | members ',' member
      {
	$$ = $1;
	$$->push_back($3);
      }
  ;

member:
    tokID
      {
        $$ = new enumValueAnn($1, (enumvalue_list *) 0);
	delete $1;
      }

  | tokID user_defined_class
      {
	$$ = new enumValueAnn($1, $2);
	delete $1;
	delete $2;
      }
  ;

/* --- Procedure annotations ------------------ */

procedure:
    procedure_declaration '{' procedure_annotations '}'
      {
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      }

  | procedure_declaration '{' '}'
      {
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      }
  ;

procedure_declaration:
    tokPROCEDURE tokID '(' identifier_list ')'
      {
        procedureAnn::Current = new procedureAnn($2, $4, Annotations::Current, $1);
        delete $2;
      }

  | tokPROCEDURE tokID '(' ')'
      {
        procedureAnn::Current = new procedureAnn($2, (parserid_list *)0, Annotations::Current, $1);
        delete $2;
      }
  ;

procedure_annotations:
    procedure_annotation
  | procedure_annotations procedure_annotation
  ;

procedure_annotation:
    structure_annotation
      {
      }

  | behavior_annotation
      {
      }

  | analyze_annotation
      {
	procedureAnn::Current->add_analysis($1);
      }

  | report_annotation
      {
      }

  | action_annotation
      {
      }
  ;

/* --- Structure declarations ----------------- */

structure_annotation:
    tokON_ENTRY '{' structures '}'
      {
        procedureAnn::Current->add_on_entry($3);
      }

  | tokON_EXIT  '{' structures '}'
      {
        procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, $3, $1));
      }

  | tokON_EXIT '{' pointer_rule_list '}'
      {
      }

  | tokON_ENTRY '{' '}'
      {
      }

  | tokON_EXIT '{' '}'
      {
      }
  ;

pointer_rule_list:
    pointer_rule
  | pointer_rule_list pointer_rule
  ;

pointer_rule:
    tokIF '(' condition ')' '{' structures '}'
      {
	procedureAnn::Current->add_on_exit(new pointerRuleAnn($3, $6, $1));
      }

  | tokIF '(' condition ')' structure
      {
	structuretree_list * temp = new structuretree_list();
	temp->push_back($5);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn($3, temp, $1));
      }

  | tokDEFAULT '{' structures '}'
      {
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, $3, $1));
      }

  | tokDEFAULT structure
      {
	structuretree_list * temp = new structuretree_list();
	temp->push_back($2);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, temp, $1));
      }

  ;

structures:
    structure
      {
        $$ = new structuretree_list();
	$$->push_back($1);
      }

  | structures structure
      {
	$$ = $1;
	$$->push_back($2);
      }

  | structures ',' structure
      {
	$$ = $1;
	$$->push_back($3);
      }

  | tokDELETE qualified_identifier
      { 
        procedureAnn::Current->add_delete($2);
        delete $2;

        /* "delete" doesn't actually add anything to the structure tree, so
           we just pass an empty list back. Similarly for the next two
           productions. */

        $$ = new structuretree_list();
      }

  | structures tokDELETE qualified_identifier
      {
        procedureAnn::Current->add_delete($3);
        delete $3;
	$$ = $1;
      }

  | structures ',' tokDELETE qualified_identifier
      {
        procedureAnn::Current->add_delete($4);
        delete $4;
	$$ = $1;
      }
  ;

structure:
    qualified_identifier
      {
        $$ = new structureTreeAnn($1, (structuretree_list *)0,
                                  structureTreeAnn::None, false);
	delete $1;
      }

  | tokID tokARROW structure
      {
        $$ = new structureTreeAnn($1, $3, structureTreeAnn::Arrow, false);
	delete $1;
      }

  | tokID tokARROW tokNEW structure
      {
        $$ = new structureTreeAnn($1, $4, structureTreeAnn::Arrow, true);
	delete $1;
      }

  | tokID '{' structures '}'
      {
        $$ = new structureTreeAnn($1, $3, structureTreeAnn::Dot, false);
	delete $1;
      }
  ;

/* --- Behavior ------------------------------- */

behavior_annotation:

    tokACCESS '{' identifier_list '}'
      {
        procedureAnn::Current->add_uses( $3 );
	delete $3;
      }

  | tokACCESS tokID
      {
        parserid_list temp;
	temp.push_back( * $2);
        procedureAnn::Current->add_uses( & temp );
	delete $2;
      }

  | tokMODIFY '{' identifier_list '}'
      {
        procedureAnn::Current->add_defs( $3 );
	delete $3;
      }

  | tokMODIFY tokID
      {
        parserid_list temp;
	temp.push_back( * $2);
        procedureAnn::Current->add_defs( & temp );
	delete $2;
      }

  ;

/* --- Analyzer rules ------------------------- */

analyze_annotation:
    tokANALYZE tokID '{' rules '}'
      {
	$$ = new analyzeAnn(Annotations::Current, $2, $4, $1);
	delete $2;
      }

  | tokANALYZE tokID '{' effects '}'
      {
	rule_list * temp_rules = new rule_list();
	temp_rules->push_back(new ruleAnn((exprAnn *)0, $4, $3));
	$$ = new analyzeAnn(Annotations::Current, $2, temp_rules, $1);
	delete $2;
      }
  ;

rules:
    rule
      {
	$$ = new rule_list();
	$$->push_back($1);
      }

  | rules rule
      {
	$$ = $1;
	$$->push_back($2);
      }
  ;

rule:
    tokIF '(' condition ')' '{' effects '}'
      {
	$$ = new ruleAnn($3, $6, $1);
      }

  | tokIF '(' condition ')' effect
      {
	exprann_list * temp = new exprann_list();
	temp->push_back($5);
	$$ = new ruleAnn($3, temp, $1);
      }

  | tokDEFAULT '{' effects '}'
      {
	$$ = new ruleAnn((exprAnn *)0, $3, $1);
      }

  | tokDEFAULT effect
      {
	exprann_list * temp = new exprann_list();
	temp->push_back($2);
	$$ = new ruleAnn((exprAnn *)0, temp, $1);
      }

  ;

condition:
    test
      {
        $$ = $1;
      }

  | condition tokOROR condition
      {
        $$ = new connectiveExprAnn(Broadway::Or, $1, $3, $2);
      }

  | condition tokANDAND condition
      {
        $$ = new connectiveExprAnn(Broadway::And, $1, $3, $2);
      }

  | '!' condition
      {
        $$ = new connectiveExprAnn(Broadway::Not, $2, (exprAnn *)0, $1);
      }

  | '(' condition ')'
      {
        $$ = $2;
      }
  ;

test:
    /* --- Enum property tests --- */

    tokID ':' tokID property_sensitivity tokIS_BOTTOM
      {
        $$ = new enumPropertyExprAnn($4.fs, $1, $3, Broadway::Is_Bottom,
				     (const parserID *)0, $4.line);
	delete $1;
	delete $3;
      }

  | tokID property_sensitivity tokIS_BOTTOM
      {
        $$ = new enumPropertyExprAnn($2.fs, (const parserID *)0, $1, Broadway::Is_Bottom,
				     (const parserID *)0, $2.line);
	delete $1;
      }

  | tokID ':' tokID property_sensitivity binary_property_operator tokID
      {
        $$ = new enumPropertyExprAnn($4.fs, $1, $3, $5.op, $6, $5.line);
	delete $1;
	delete $3;
	delete $6;
      }

  | tokID property_sensitivity binary_property_operator tokID
      {
        $$ = new enumPropertyExprAnn($2.fs, (const parserID *)0, $1, $3.op, $4, $3.line);
	delete $1;
	delete $4;
      }

    /* --- Set property tests --- */

  | tokID tokID tokID
      {
        $$ = new setPropertyExprAnn(Broadway::Before, $2, $1, Broadway::Is_Equivalent, $3, $1->line());
	delete $1;
	delete $2;
	delete $3;
      }

  | tokID tokIS_ELEMENT_OF tokID
      {
        $$ = new setPropertyExprAnn(Broadway::Before, $3, $1, Broadway::Is_Element_Of,
				    (const parserID *)0, $2);
	delete $1;
	delete $3;
      }

  | tokID tokIS_EMPTYSET
      {
        $$ = new setPropertyExprAnn(Broadway::Before, $1, (const parserID *)0,
                                    Broadway::Is_EmptySet, (const parserID *)0, $2);

	delete $1;
      }

    /* --- Numeric tests --- */

  | c_compare_expression
      {
        $$ = new numericExprAnn($1, annlineno);
      }

  | tokID tokIS_CONSTANT
      {
        $$ = new numericExprAnn($1, annlineno);
      }

    /* --- Pointer tests --- */

  | tokID tokIS_ALIASOF tokID
      {
        $$ = new pointerExprAnn(Broadway::Is_AliasOf, $1, $3);
        delete $1;
        delete $3;
      }

  | tokID tokIS_SAMEAS tokID
      {
        $$ = new pointerExprAnn(Broadway::Is_SameAs, $1, $3);
        delete $1;
        delete $3;
      }

  | tokID tokIS_EMPTY
      {
        $$ = new pointerExprAnn(Broadway::Is_Empty, $1, (const parserID *)0);
        delete $1;
      }

  ;

effects:
    effect
      {
	$$ = new exprann_list();
	$$->push_back($1);
      }

  | effects effect
      {
	$$ = $1;
	$$->push_back($2);
      }
  ;

effect:

    tokID tokEQUALS c_expression
      {
	$$ = new numericExprAnn($1, $3, $2);
	delete $1;
      }

  | tokID tokASSIGN tokID
      {
        $$ = new enumPropertyExprAnn(Broadway::None,
				     (const parserID *)0, $1, Broadway::Assign,
				     $3, $2);
	delete $1;
	delete $3;
      }

  | tokID tokWEAKASSIGN tokID
      {
        enumPropertyExprAnn * tmp = new enumPropertyExprAnn(Broadway::None,
							    (const parserID *)0, $1, Broadway::Assign,
							    $3, $2);
	tmp->set_weak();
	$$ = tmp;
	delete $1;
	delete $3;
      }

  | tokADD tokID
      {
        $$ = new setPropertyExprAnn(Broadway::None,
				    (const parserID *)0, $2, Broadway::Add_Elements,
				    (const parserID *)0, $1);
	delete $2;
      }

  | tokID tokID tokID
      {
        $$ = new setPropertyExprAnn(Broadway::None,
				    $2, $1, Broadway::Add_Equivalences, $3, $1->line());
	delete $1;
	delete $2;
	delete $3;
      }
  ;

binary_property_operator:

     tokIS_EXACTLY
      { $$.line = $1;
        $$.op = Broadway::Is_Exactly; }

  |  tokIS_ATLEAST
      { $$.line = $1;
        $$.op = Broadway::Is_AtLeast; }

  |  tokCOULD_BE
      { $$.line = $1;
        $$.op = Broadway::Could_Be; }

  |  tokIS_ATMOST
      { $$.line = $1;
        $$.op = Broadway::Is_AtMost; }
  ;

property_sensitivity:

     tokBEFORE
       { $$.line = $1;
         $$.fs = Broadway::Before; }

  |  tokAFTER
       { $$.line = $1;
         $$.fs = Broadway::After; }

  |  tokALWAYS
       { $$.line = $1;
         $$.fs = Broadway::Always; }

  |  tokEVER
       { $$.line = $1;
         $$.fs = Broadway::Ever; }

  |  tokTRACE
       { $$.line = $1;
         $$.fs = Broadway::Trace; }

  |  tokCONFIDENCE
       { $$.line = $1;
         $$.fs = Broadway::Confidence; }

  |
       { $$.line =  0;
         $$.fs = Broadway::Before; }
  ;

/* --- Report --------------------------------- */

report_annotation:

     tokREPORT report_element_list ';'
       {
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, false, $2, $1));
       }

  |  tokREPORT tokIF '(' condition ')' report_element_list ';'
       {
         procedureAnn::Current->add_report(new reportAnn($4, false, $6, $1));
       }
  |  tokERROR report_element_list ';'
       {
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, true, $2, $1));
       }

  |  tokERROR tokIF '(' condition ')' report_element_list ';'
       {
         procedureAnn::Current->add_report(new reportAnn($4, true, $6, $1));
       }
  ;

report_element_list:

     report_element
       {
         $$ = new report_element_list();
         $$->push_back( $1 );
       }

  |  report_element_list tokAPPEND report_element
       {
         $$ = $1;
         $$->push_back($3);
       }

  ;

report_element:

     tokSTRING
       {
         $$ = new literalReportElementAnn($1);
         delete $1;
       }

  | tokID ':' tokID property_sensitivity
      {
        $$ = new expressionReportElementAnn($4.fs,
	                                    new enumPropertyExprAnn($4.fs, $1, $3, Broadway::Report,
                                                                (const parserID *)0, $1->line()),
                                            annlineno);
	delete $1;
	delete $3;
      }

  | c_expression property_sensitivity
      {
        $$ = new expressionReportElementAnn($2.fs,
                                            new numericExprAnn($1, $2.line),
                                            annlineno);
      }

  | '@' tokID
      {
        $$ = new locationReportElementAnn($2);
      }

  | '[' tokID ']'
      {
        $$ = new bindingReportElementAnn($2, false);
      }

  | '|' tokID '|'
      {
        $$ = new bindingReportElementAnn($2, true);
      }
  ;

/* --- Actions -------------------------- */

action_annotation:
    tokWHEN '(' condition ')' tokREPLACE statement
      {
        procedureAnn::Current->add_action(new actionAnn($3, $6, $1));
      }

  | tokWHEN '(' condition ')' tokINLINE ';'
      {
        procedureAnn::Current->add_action(new actionAnn($3, $1));
      }
  ;

/* --- Pattern -------------------------------- */

pattern:
    tokPATTERN
      {
      }
  ;

/* --- Miscellaneous -------------------------- */

identifier_list:
    qualified_identifier
      { $$ = new parserid_list();
        $$->push_back( * $1 );
	delete $1;
      } 

  | identifier_list ',' qualified_identifier
      { $$ = $1;
        $$->push_back( * $3);
        delete $3;
      }
  ;

qualified_identifier:
    tokID
      { $$ = $1; } 

  | tokIO tokID
      { $$ = $2;
        $$->set_io();
      } 
  ;

/* --- C constant expression ------------------ */

c_compare_expression:

    c_expression tokEQUALS_EQUALS c_expression
      { $$ = new binaryNode(Operator::EQ, $1, $3); }

  | c_expression tokNOT_EQUALS c_expression
      { $$ = new binaryNode(Operator::NE, $1, $3); }

  | c_expression '<' c_expression
      { $$ = new binaryNode('<', $1, $3); }

  | c_expression tokLESS_EQUALS c_expression
      { $$ = new binaryNode(Operator::LE, $1, $3); }

  | c_expression '>' c_expression
      { $$ = new binaryNode('>', $1, $3); }

  | c_expression tokGREATER_EQUALS c_expression
      { $$ = new binaryNode(Operator::GE, $1, $3); }
  ;


c_expression:
    tokCONSTANT
      { $$ = $1; }

  | tokID
      { $$ = new idNodeAnn($1);
        delete $1;
      }

  | '(' c_expression ')'
      { $$ = $2; $$->parenthesized(true); }

  | c_expression '+' c_expression
      { $$ = new binaryNode('+', $1, $3); }

  | c_expression '-' c_expression
      { $$ = new binaryNode('-', $1, $3); }

  | c_expression '|' c_expression
      { $$ = new binaryNode('|', $1, $3); }

  | c_expression '^' c_expression
      { $$ = new binaryNode('^', $1, $3); }

  | c_expression '&' c_expression
      { $$ = new binaryNode('&', $1, $3); }

  | c_expression '*' c_expression
      { $$ = new binaryNode('*', $1, $3); }

  | c_expression '/' c_expression
      { $$ = new binaryNode('/', $1, $3); }

  | c_expression '%' c_expression
      { $$ = new binaryNode('%', $1, $3); }

  | '-' c_expression
      { $$ = new unaryNode('-', $2); }

  | '+' c_expression
      { $$ = new unaryNode('+', $2); }

  | '~' c_expression
      { $$ = new unaryNode('~', $2); }
  ;

/* ============================================================

    C Grammar

   ============================================================
*/

/********************************************************************************
*										*
*                                EXPRESSIONS                                    *
*										*
********************************************************************************/

primary.expression:             /* P */ /* 6.3.1 EXTENDED */  
          /* A typedef name cannot be used as a variable.  Fill in type later */
          identifier            { $$ = $1; }
	/* djimenez - this is a bug!  a typedef name should be able
	 * to be used as a variable; but we get 6 reduce/reduce conflicts
	 * if we do that.  need to figure out how to get this to work.
	 */
        | constant
        | string.literal.list   { $$ = $1; }

        | ctokLEFTPAREN expression ctokRIGHTPAREN    { if ($2->typ() == Comma) $2->coord(Coord($1));
                                  $2->parenthesized(true);
                                  $$ = $2; }

          /* GCC-inspired non ANSI-C extension */

        | ctokLEFTPAREN ctokLEFT statement.list ctokRIGHT ctokRIGHTPAREN
            { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
               $$ = (exprNode *) /* This is BAD */
		 new blockNode((decl_list *)0, $3, Coord($1), Coord($4)); }

        | ctokLEFTPAREN ctokLEFT declaration.list statement.list ctokRIGHT ctokRIGHTPAREN
            { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
              $$ =  (exprNode *) /* This is BAD */
		new blockNode($3, $4, Coord($1), Coord($5)); }

        | expr_pattern    { $$ = $1; }
        ;

postfix.expression:             /* P */ /* 6.3.2 CLARIFICATION */
          primary.expression
            { $$ = $1; }

        | postfix.expression ctokLEFTBRACKET expression ctokRIGHTBRACKET
            { /* I don't like this (SZG)...
		 if ($1->typ() == Index)
	        { indexNode * atmp = (indexNode *)$1;
		  atmp->add_dim($3);
		  $$ = atmp; }
	      else */
	      { /* Only allow one dimension arg per indexNode */
		$$ = new binaryNode(Operator::Index, $1, $3, Coord($2));
	      }
	    }

        | postfix.expression ctokLEFTPAREN ctokRIGHTPAREN
            { $$ = new callNode($1, (expr_list *)0, Coord($2)); }

        | postfix.expression ctokLEFTPAREN argument.expression.list ctokRIGHTPAREN
            { $$ = new callNode($1, $3, Coord($2)); }

        | postfix.expression ctokDOT identifier
            { $$ = new binaryNode('.', $1, $3, Coord($2)); }

        | postfix.expression ctokARROW identifier
            { $$ = new binaryNode( Operator::ARROW, $1, $3, Coord($2)); }

        | postfix.expression ctokICR
            { $$ = new unaryNode( Operator::POSTINC, $1, Coord($2)); }

        | postfix.expression ctokDECR
            { $$ = new unaryNode( Operator::POSTDEC, $1, Coord($2)); }

          /* EXTENSION: typedefname can be used to name a struct/union field */
        | postfix.expression ctokDOT   typedefname
            { $$ = new binaryNode('.', $1, $3, Coord($2)); }

        | postfix.expression ctokARROW typedefname
            { $$ = new binaryNode( Operator::ARROW, $1, $3, Coord($2)); }
        ;

argument.expression.list:       /* P */ /* 6.3.2 */
          assignment.expression
            { $$ = new expr_list(); $$->push_back($1); }

        | argument.expression.list ctokCOMMA assignment.expression
            { $$ = $1; $$->push_back($3); }
        ;

unary.expression:               /* P */ /* 6.3.3 */
          postfix.expression
            { $$ = $1; }

        | ctokICR unary.expression
            { $$ = new unaryNode( Operator::PREINC, $2, Coord($1)); }

        | ctokDECR unary.expression
            { $$ = new unaryNode( Operator::PREDEC, $2, Coord($1)); }

        | unary.operator cast.expression
            { $1->expr($2);
              $$ = $1; }

        | ctokSIZEOF unary.expression
            { $$ = new unaryNode( Operator::SIZEOF, $2, Coord($1)); }

        | ctokSIZEOF ctokLEFTPAREN type.name ctokRIGHTPAREN
            { $$ = new unaryNode( Operator::SIZEOF, $3.ty, Coord($1)); }
        ;

unary.operator:                 /* P */ /* 6.3.3 */
          ctokAND     { $$ = new unaryNode('&', (exprNode *)0, Coord($1)); }
        | ctokSTAR     { $$ = new unaryNode('*', (exprNode *)0, Coord($1)); }
        | ctokPLUS     { $$ = new unaryNode('+', (exprNode *)0, Coord($1)); }
        | ctokMINUS     { $$ = new unaryNode('-', (exprNode *)0, Coord($1)); }
        | ctokTILDE     { $$ = new unaryNode('~', (exprNode *)0, Coord($1)); }
        | ctokNOT     { $$ = new unaryNode('!', (exprNode *)0, Coord($1)); }
        ;

cast.expression:                /* P */ /* 6.3.4 */
          unary.expression
        | ctokLEFTPAREN type.name ctokRIGHTPAREN cast.expression
            { $$ = new castNode($2.ty, $4, false, Coord($1)); }
        ;

multiplicative.expression:      /* P */ /* 6.3.5 */
          cast.expression
        | multiplicative.expression ctokSTAR cast.expression
            { $$ = new binaryNode('*', $1, $3, Coord($2)); }

        | multiplicative.expression ctokDIVIDE cast.expression
            { $$ = new binaryNode('/', $1, $3, Coord($2)); }

        | multiplicative.expression ctokPERCENT cast.expression
            { $$ = new binaryNode('%', $1, $3, Coord($2)); }
        ;

additive.expression:            /* P */ /* 6.3.6 */
          multiplicative.expression
        | additive.expression ctokPLUS multiplicative.expression
            { $$ = new binaryNode('+', $1, $3, Coord($2)); }

        | additive.expression ctokMINUS multiplicative.expression
            { $$ = new binaryNode('-', $1, $3, Coord($2)); }
        ;

shift.expression:               /* P */ /* 6.3.7 */
          additive.expression
        | shift.expression ctokLS additive.expression
            { $$ = new binaryNode( Operator::LS, $1, $3, Coord($2)); }

        | shift.expression ctokRS additive.expression
            { $$ = new binaryNode( Operator::RS, $1, $3, Coord($2)); }
        ;

relational.expression:          /* P */ /* 6.3.8 */
          shift.expression
        | relational.expression ctokLESS shift.expression
            { $$ = new binaryNode('<', $1, $3, Coord($2)); }

        | relational.expression ctokGREATER shift.expression
            { $$ = new binaryNode('>', $1, $3, Coord($2)); }

        | relational.expression ctokLE shift.expression
            { $$ = new binaryNode( Operator::LE, $1, $3, Coord($2)); }

        | relational.expression ctokGE shift.expression
            { $$ = new binaryNode( Operator::GE, $1, $3, Coord($2)); }
        ;

equality.expression:            /* P */ /* 6.3.9 */
          relational.expression
        | equality.expression ctokEQ relational.expression
            { $$ = new binaryNode( Operator::EQ, $1, $3, Coord($2)); }

        | equality.expression ctokNE relational.expression
            { $$ = new binaryNode( Operator::NE, $1, $3, Coord($2)); }
        ;

AND.expression:                 /* P */ /* 6.3.10 */
          equality.expression
        | AND.expression ctokAND equality.expression
            { $$ = new binaryNode('&', $1, $3, Coord($2)); }
        ;

exclusive.OR.expression:        /* P */ /* 6.3.11 */
          AND.expression
        | exclusive.OR.expression ctokHAT AND.expression
            { CBZ::WarnAboutPrecedence('^', $1);
              CBZ::WarnAboutPrecedence('^', $3);
	      $$ = new binaryNode('^', $1, $3, Coord($2)); }
        ;

inclusive.OR.expression:        /* P */ /* 6.3.12 */
          exclusive.OR.expression
        | inclusive.OR.expression ctokOR exclusive.OR.expression
            { CBZ::WarnAboutPrecedence('|', $1);
              CBZ::WarnAboutPrecedence('|', $3);
              $$ = new binaryNode('|', $1, $3, Coord($2)); }
        ;

logical.AND.expression:         /* P */ /* 6.3.13 */
          inclusive.OR.expression
        | logical.AND.expression ctokANDAND inclusive.OR.expression
            { $$ = new binaryNode( Operator::ANDAND, $1, $3, Coord($2)); }
        ;

logical.OR.expression:          /* P */ /* 6.3.14 */
          logical.AND.expression
        | logical.OR.expression ctokOROR logical.AND.expression
            { CBZ::WarnAboutPrecedence(ctokOROR, $1);
              CBZ::WarnAboutPrecedence(ctokOROR, $3);
              $$ = new binaryNode( Operator::OROR, $1, $3, Coord($2)); }
        ;

conditional.expression:         /* P */ /* 6.3.15 */
          logical.OR.expression
        | logical.OR.expression ctokQUESTION expression ctokCOLON conditional.expression
            { $$ = new ternaryNode($1, $3, $5, Coord($2), Coord($4)); }
        ;

assignment.expression:          /* P */ /* 6.3.16 */
          conditional.expression
        | unary.expression assignment.operator assignment.expression
            { $2->left($1);
	      $2->right($3);
	      $$ = $2; }
        ;

assignment.operator:            /* P */ /* 6.3.16 */
          ctokEQUALS
            { $$ = new binaryNode('=', (exprNode *)0, (exprNode *)0, Coord($1)); }

        | ctokMULTassign
            { $$ = new binaryNode( Operator::MULTassign, (exprNode *)0, (exprNode *)0, Coord($1));  }

        | ctokDIVassign
            { $$ = new binaryNode( Operator::DIVassign, (exprNode *)0, (exprNode *)0, Coord($1));   }

        | ctokMODassign
            { $$ = new binaryNode( Operator::MODassign, (exprNode *)0, (exprNode *)0, Coord($1));   }

        | ctokPLUSassign
            { $$ = new binaryNode( Operator::PLUSassign, (exprNode *)0, (exprNode *)0, Coord($1));  }

        | ctokMINUSassign
            { $$ = new binaryNode( Operator::MINUSassign, (exprNode *)0, (exprNode *)0, Coord($1)); }

        | ctokLSassign
            { $$ = new binaryNode( Operator::LSassign, (exprNode *)0, (exprNode *)0, Coord($1));    }

        | ctokRSassign
            { $$ = new binaryNode( Operator::RSassign, (exprNode *)0, (exprNode *)0, Coord($1));    }

        | ctokANDassign
            { $$ = new binaryNode( Operator::ANDassign, (exprNode *)0, (exprNode *)0, Coord($1));   }

        | ctokERassign
            { $$ = new binaryNode( Operator::ERassign, (exprNode *)0, (exprNode *)0, Coord($1));    }

        | ctokORassign
            { $$ = new binaryNode( Operator::ORassign, (exprNode *)0, (exprNode *)0, Coord($1));    }
        ;

expression:                     /* P */ /* 6.3.17 */
          assignment.expression
            { $$ = $1; }
        | expression ctokCOMMA assignment.expression
            {  
              if ($1->typ() == Comma) 
                { commaNode * ctmp = (commaNode *)$1;
		  ctmp->exprs().push_back($3);
		  $$ = $1; }
              else
                { expr_list * e = new expr_list();
		  e->push_back($1);
		  e->push_back($3);
		  $$ = new commaNode(e, $1->coord()); }
	    }
        ;

constant.expression:            /* P */ /* 6.4   */
          conditional.expression { $$ = $1; }
        ;

expression.opt:                 /* P */ /* For convenience */
          /* Nothing */   { $$ = (exprNode *) 0; }
        | expression      { $$ = $1; }
        ;

/********************************************************************************
*										*
*                               DECLARATIONS					*
*										*
*    The following is different from the ANSI C specified grammar.  The changes *
* were made to disambiguate typedef's presence in declaration.specifiers        *
* (vs. in the declarator for redefinition) to allow struct/union/enum tag       *
* declarations without declarators, and to better reflect the parsing of        *
* declarations (declarators must be combined with declaration.specifiers ASAP   *
* so that they are visible in scope).					        *
*										*
* Example of typedef use as either a declaration.specifier or a declarator:	*
*										*
*   typedef int T;								*
*   struct S { T T; }; / * redefinition of T as member name * /			*
*										*
* Example of legal and illegal statements detected by this grammar:		*
*										*
*   int;              / * syntax error: vacuous declaration      * /		*
*   struct S;         / * no error: tag is defined or elaborated * /		*
*										*
* Example of result of proper declaration binding:				*
*										*
*   /* Declare "a" with a type in the name space BEFORE parsing initializer * / *
*   int a = sizeof(a);								*
*										*
*   /* Declare "b" with a type before parsing "c" * /				*
*   int b, c[sizeof(b)];							*
*										*
********************************************************************************/

/*                        */    /* ? */ /* ?.?.? */
declaration: /*P*/
          declaring.list ctokSEMICOLON
            { $$ = $1; }

        | default.declaring.list ctokSEMICOLON
            { $$ = $1; }

          /* SZG: Make the sue type into a decl */
        | sue.declaration.specifier ctokSEMICOLON
            { $$ = new decl_list();
	      ((sueNode *)$1.ty)->force_new_and(Coord($2));
	      $$->push_back(new declNode($1.ty, $1.sc)); }

          /* SZG: Make the sue type into a decl */
        | sue.type.specifier ctokSEMICOLON
            { $$ = new decl_list();
	      ((sueNode *)$1.ty)->force_new_and(Coord($2));
	      $$->push_back(new declNode($1.ty, $1.sc)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
declaring.list: /*P*/
          declaration.specifier declarator
            { $2->set_type($1.ty, $1.sc, Redecl); }
          attributes.opt
            { $2->merge_attribs($4); }
          initializer.opt
            { $$ = new decl_list();
	      $$->push_back($2->set_init_and($6));
	    }

        | type.specifier declarator 
            { $2->set_type($1.ty, $1.sc, Redecl); }
          attributes.opt
            { $2->merge_attribs($4); }
          initializer.opt
            { $$ = new decl_list();
	      $$->push_back($2->set_init_and($6)); }

        | declaring.list ctokCOMMA declarator
            { $1->push_back($3->inherit_type_and($1,NoRedecl)); }
          attributes.opt 
            { $3->merge_attribs($5); }
          initializer.opt
            { $$ = $1;
	      $3->set_init($7); }

        /******** ERROR PRODUCTIONS ********/
        | /* error production: catch missing identifier */
          declaration.specifier error
            { CBZ::SyntaxError(string("declaration without a variable")); }
          attributes.opt
          initializer.opt
           { $$ = (decl_list *)0; /* empty list */ }

        | /* error production: catch missing identifier */
          type.specifier error
            { CBZ::SyntaxError(string("declaration without a variable")); }
          attributes.opt
          initializer.opt
            { $$ = (decl_list *)0; /* empty list */ }

        | declaring.list ctokCOMMA error
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Note that if a typedef were redeclared, then a decl-spec must be supplied */
default.declaring.list:  /*P*/ /* Can't  redeclare typedef names */
          declaration.qualifier.list identifier.declarator
            { $2->set_type(new primNode($1.tq), $1.sc, NoRedecl); }
          attributes.opt
            { $2->merge_attribs($4); }
	  initializer.opt
            { $$ = new decl_list();
	      $$->push_back($2->set_init_and($6)); }

        | type.qualifier.list identifier.declarator
            { $2->set_type(new primNode($1.tq), $1.sc, NoRedecl); }
          attributes.opt
            { $2->merge_attribs($4); }
	  initializer.opt
            { $$ = new decl_list();
	      $$->push_back($2->set_init_and($6)); }

        | default.declaring.list ctokCOMMA identifier.declarator
            { $1->push_back($3->inherit_type_and($1,NoRedecl)); }
          attributes.opt
            { $3->merge_attribs($5); }
	  initializer.opt
            { $$ = $1;
	      $3->set_init($7); }

        /********  ERROR PRODUCTIONS ********/
        | /* error production: catch missing identifier */
          declaration.qualifier.list error
            { CBZ::SyntaxError(string("declaration without a variable")); }
          attributes.opt
          initializer.opt
            { $$ = (decl_list *)0; /* empty list */ }

        | /* error production: catch missing identifier */
          type.qualifier.list error
            { CBZ::SyntaxError(string("declaration without a variable")); }
          attributes.opt
          initializer.opt
            { $$ = (decl_list *)0; /* empty list */ }

        | default.declaring.list ctokCOMMA error
        ;

/*                        */    /* ? */ /* ?.?.? */
declaration.specifier: /*P*/
          basic.declaration.specifier        /* Arithmetic or void */
            { primNode * p = (primNode *) $1.ty;
	      $$.ty = p->finish_and(); 
	      $$.sc = $1.sc; }

        | sue.declaration.specifier          /* struct/union/enum  */
            { $$ = $1; }

        | typedef.declaration.specifier      /* typedef            */
            { $$ = $1; }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* StorageClass + Arithmetic or void */
basic.declaration.specifier:  /*P*/
          basic.type.specifier storage.class
            { $$.ty = $1.ty;
	      $$.sc = merge_sc($1.sc, $2.sc, $1.ty->coord()); }

        | declaration.qualifier.list basic.type.name
            { $$.ty = (primNode *) $2->add_type_qualifiers_and($1.tq);
	      $$.ty->coord(Coord($1.coord)); 
	      $$.sc = $1.sc; }

        | basic.declaration.specifier declaration.qualifier
            { $$.ty = (primNode *) $1.ty->add_type_qualifiers_and($2.tq); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }

        | basic.declaration.specifier basic.type.name
            { $$.ty = ((primNode *)$1.ty)->merge_in_and($2); 
	      $$.sc = $1.sc; }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* StorageClass + struct/union/enum */
sue.declaration.specifier: /*P*/   
          sue.type.specifier storage.class
            { $$.ty = $1.ty;
	      $$.sc = merge_sc($1.sc, $2.sc, $1.ty->coord()); }

        | declaration.qualifier.list elaborated.type.name
            { $$.ty = $2.ty->add_type_qualifiers_and($1.tq);
	      $$.ty->coord(Coord($1.coord)); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($1.coord)); }

        | sue.declaration.specifier declaration.qualifier
            { $$.ty = $1.ty->add_type_qualifiers_and($2.tq); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Storage Class + typedef types */
typedef.declaration.specifier:  /*P*/      
          typedef.type.specifier storage.class
            { $$.ty = $1.ty->add_type_qualifiers_and($2.tq);
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }

        | declaration.qualifier.list typedefname
            { $$.ty = new tdefNode($2, $1.tq, $2->decl()->type());
	      $$.ty->coord(Coord($1.coord)); 
	      $$.sc = $1.sc; }

        | typedef.declaration.specifier declaration.qualifier
            { $$.ty = $1.ty->add_type_qualifiers_and($2.tq); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Type qualifier AND storage class */
declaration.qualifier.list:  /*P*/
          storage.class
        | type.qualifier.list storage.class
            { $$ = merge_tq($1, $2); }

        | declaration.qualifier.list declaration.qualifier
            { $$ = merge_tq($1, $2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
declaration.qualifier: /*P*/
          type.qualifier
        | storage.class
        ;

/*                        */    /* ? */ /* ?.?.? */
type.specifier: /*P*/
          basic.type.specifier               /* Arithmetic or void */
            { primNode * p = (primNode *) $1.ty;
	      $$.ty = p->finish_and();
	      $$.sc = $1.sc;}

        | sue.type.specifier                 /* Struct/Union/Enum  */
            { $$ = $1; }

        | typedef.type.specifier             /* Typedef            */

        | type_pattern
            { $$ = $1; }
        ;

/*                        */    /* ? */ /* ?.?.? */
basic.type.specifier: /*P*/
          basic.type.name            /* Arithmetic or void */
            { $$.ty = $1;  
	      $$.sc = declNode::NONE; }

        | type.qualifier.list basic.type.name
            { $$.ty = (primNode *) $2->add_type_qualifiers_and($1.tq);
	      $$.sc = $1.sc;
	      $$.ty->coord(Coord($1.coord)); }

        | basic.type.specifier type.qualifier
            { $$.ty = (primNode *) $1.ty->add_type_qualifiers_and($2.tq); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }

        | basic.type.specifier basic.type.name
            { $$.ty = ((primNode *)$1.ty)->merge_in_and($2);
	      $$.sc = $1.sc; }
        ;

/*                        */    /* ? */ /* ?.?.? */
sue.type.specifier: /*P*/
          elaborated.type.name              /* struct/union/enum */
            { $$.ty = $1.ty; 
	      $$.sc = declNode::NONE; }

        | type.qualifier.list elaborated.type.name
            { $$.ty = $2.ty->add_type_qualifiers_and($1.tq);
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($1.coord));
	      $$.ty->coord(Coord($1.coord)); }

        | sue.type.specifier type.qualifier
            { $$.ty = $1.ty->add_type_qualifiers_and($2.tq); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* typedef types */
typedef.type.specifier:  /*P*/             
          typedefname
            { $$.ty = new tdefNode($1, typeNode::NONE, $1->decl()->type());
	      $$.sc = declNode::NONE; }

        | type.qualifier.list typedefname
            { $$.ty = new tdefNode($2, $1.tq, $2->decl()->type());
	      $$.sc = $1.sc;
	      $$.ty->coord(Coord($1.coord)); }

        | typedef.type.specifier type.qualifier
            { $$.ty = $1.ty->add_type_qualifiers_and($2.tq);
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($2.coord)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
type.qualifier.list: /*P*/
          type.qualifier
        | type.qualifier.list type.qualifier
            { $$ = merge_tq($1, $2); }
        ;

pointer.type.qualifier.list:
          pointer.type.qualifier
        | pointer.type.qualifier.list pointer.type.qualifier
            { $$ = merge_tq($1, $2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
elaborated.type.name: /*P*/
          struct.or.union.specifier
	    { $$.ty = $1; 
	      $$.sc = declNode::NONE; }

        | enum.specifier
	    { $$.ty = $1;  
	      $$.sc = declNode::NONE; }
        ;

/*                        */    /* ? */ /* ?.?.? */
declarator: /*P*/
          paren.typedef.declarator       /* would be ambiguous as parameter */
        | parameter.typedef.declarator   /* not ambiguous as param          */
        | identifier.declarator
        | old.function.declarator
            {
	      CBZ::Warning(2, string("function prototype parameters must have types"));
              /* SZG Don't need this? $$ = AddDefaultParameterTypes($1); */
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
/* Redundant ctokLEFTPAREN placed immediately to the left of the typedefname  */
paren.typedef.declarator: /*P*/
          paren.postfix.typedef.declarator

        | ctokSTAR paren.typedef.declarator
            { $$ = $2->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord($1)), declNode::NONE, Redecl); }

        | ctokSTAR ctokLEFTPAREN simple.paren.typedef.declarator ctokRIGHTPAREN 
            { $$ = $3->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord($1)), declNode::NONE, Redecl); }

        | ctokSTAR pointer.type.qualifier.list ctokLEFTPAREN simple.paren.typedef.declarator ctokRIGHTPAREN 
            { $$ = $4->set_type_and(new ptrNode($2.tq,(typeNode *)0, Coord($1)), $2.sc, Redecl); }

        | ctokSTAR pointer.type.qualifier.list paren.typedef.declarator
            { $$ = $3->set_type_and(new ptrNode($2.tq,(typeNode *)0, Coord($1)), $2.sc, Redecl); }
        ;
        
/*                        */    /* ? */ /* ?.?.? */
/* Redundant ctokLEFTPAREN to left of typedefname */
paren.postfix.typedef.declarator: /*P*/ 
          ctokLEFTPAREN paren.typedef.declarator ctokRIGHTPAREN
            { $$ = $2; }

        | ctokLEFTPAREN simple.paren.typedef.declarator postfixing.abstract.declarator ctokRIGHTPAREN
            { $$ = $2->modify_type_and($3.ty); }

        | ctokLEFTPAREN paren.typedef.declarator ctokRIGHTPAREN postfixing.abstract.declarator
            { $$ = $2->modify_type_and($4.ty); }
        ;

/*                        */    /* ? */ /* ?.?.? */
simple.paren.typedef.declarator: /*P*/
          typedefname
            { $$ = new declNode($1, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }

        | ctokLEFTPAREN simple.paren.typedef.declarator ctokRIGHTPAREN
            { $$ = $2;  }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.typedef.declarator: /*P*/
          typedefname 
            { $$ = new declNode($1, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }

        | typedefname postfixing.abstract.declarator
            { $$ = new declNode($1, declNode::NONE, $2.ty, (exprNode *)0, (exprNode *)0); }

        | clean.typedef.declarator
        ;

/*
   The  following have at least one ctokSTAR. There is no (redundant) 
   ctokLEFTPAREN between the ctokSTAR and the typedefname. 
*/
/*                        */    /* ? */ /* ?.?.? */
clean.typedef.declarator: /*P*/
          clean.postfix.typedef.declarator

        | ctokSTAR parameter.typedef.declarator
            { $$ = $2->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord($1)), declNode::NONE, Redecl); }

        | ctokSTAR pointer.type.qualifier.list parameter.typedef.declarator  
            { $$ = $3->set_type_and(new ptrNode($2.tq, (typeNode *)0, Coord($1)), $2.sc, Redecl); }
        ;

/*                        */    /* ? */ /* ?.?.? */
clean.postfix.typedef.declarator: /*P*/
          ctokLEFTPAREN clean.typedef.declarator ctokRIGHTPAREN
            { $$ = $2; }

        | ctokLEFTPAREN clean.typedef.declarator ctokRIGHTPAREN postfixing.abstract.declarator
            { $$ = $2->modify_type_and($4.ty); }
        ;

/*                        */    /* ? */ /* ?.?.? */
abstract.declarator: /*P*/
          unary.abstract.declarator
        | postfix.abstract.declarator
        | postfixing.abstract.declarator
        ;

/*                        */    /* ? */ /* ?.?.? */
unary.abstract.declarator: /*P*/
          ctokSTAR 
            { $$.ty = new ptrNode(typeNode::NONE, (typeNode *)0, Coord($1)); 
	      $$.sc = declNode::NONE; }

        | ctokSTAR pointer.type.qualifier.list 
            { $$.ty = new ptrNode($2.tq, (typeNode *)0, Coord($1));  
	      $$.sc = $2.sc; }

        | ctokSTAR abstract.declarator
            { $$.ty = $2.ty->set_base_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord($1)));   
	      $$.sc = $2.sc; }

        | ctokSTAR pointer.type.qualifier.list abstract.declarator
            { $$.ty = $3.ty->set_base_type_and(new ptrNode($2.tq, (typeNode *)0, Coord($1)));   
	      $$.sc = merge_sc($2.sc, $3.sc, Coord($2.coord)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfix.abstract.declarator: /*P*/
          ctokLEFTPAREN unary.abstract.declarator ctokRIGHTPAREN
            { $$.ty = $2.ty; 
	      $$.sc = $2.sc; }

        | ctokLEFTPAREN postfix.abstract.declarator ctokRIGHTPAREN
            { $$.ty = $2.ty; 
	      $$.sc = $2.sc; }

        | ctokLEFTPAREN postfixing.abstract.declarator ctokRIGHTPAREN
            { $$.ty = $2.ty; 
	      $$.sc = $2.sc; }

        | ctokLEFTPAREN unary.abstract.declarator ctokRIGHTPAREN postfixing.abstract.declarator
            { $$.ty = $2.ty->set_base_type_and($4.ty); 
	      $$.sc = merge_sc($2.sc, $4.sc, $2.ty->coord()); }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfixing.abstract.declarator: /*P*/
          array.abstract.declarator
             { $$.ty = $1.ty;
	       $$.sc = $1.sc; }

        | ctokLEFTPAREN ctokRIGHTPAREN
             { $$.ty = new funcNode(typeNode::NONE, (decl_list *)0, (typeNode *)0, Coord($1)); 
	       $$.sc = declNode::NONE; }

        | ctokLEFTPAREN parameter.type.list ctokRIGHTPAREN
             { $$.ty = new funcNode(typeNode::NONE, $2, (typeNode *)0, Coord($1)); 
	       $$.sc = declNode::NONE; }
        ;

/*                        */    /* ? */ /* ?.?.? */
array.abstract.declarator: /*P*/
          ctokLEFTBRACKET ctokRIGHTBRACKET
            { $$.ty = new arrayNode(typeNode::NONE, (typeNode *)0, (exprNode *)0, Coord($1));
              $$.sc = declNode::NONE; }

        | ctokLEFTBRACKET constant.expression ctokRIGHTBRACKET
            { $$.ty = new arrayNode(typeNode::NONE, (typeNode *)0, $2, Coord($1)); 
              $$.sc = declNode::NONE; }

        | array.abstract.declarator ctokLEFTBRACKET constant.expression ctokRIGHTBRACKET
            { $$.ty = $1.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0, $3, Coord($2))); 
              $$.sc = $1.sc; }

        /******** ERROR PRODUCTIONS ********/
        | /* error production: catch empty dimension that isn't first */
          array.abstract.declarator ctokLEFTBRACKET ctokRIGHTBRACKET
            { 
              CBZ::SyntaxError(string("array declaration with illegal empty dimension"));
              $$.ty = $1.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0,
							     (exprNode *) new constNode(constant((int)1)),
							     Coord($2)));
	      $$.sc = declNode::NONE;
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
identifier.declarator: /*P*/
          unary.identifier.declarator
        | paren.identifier.declarator
        ;

/*                        */    /* ? */ /* ?.?.? */
unary.identifier.declarator: /*P293*/
          postfix.identifier.declarator

        | ctokSTAR identifier.declarator
            { $$ = $2->modify_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord($1))); }

        | ctokSTAR pointer.type.qualifier.list identifier.declarator
            { $$ = $3->modify_type_and(new ptrNode($2.tq, (typeNode *)0, Coord($1))); }
        ;
        
/*                        */    /* ? */ /* ?.?.? */
postfix.identifier.declarator: /*P296*/
          paren.identifier.declarator postfixing.abstract.declarator
            { $$ = $1->modify_type_and($2.ty); }

        | ctokLEFTPAREN unary.identifier.declarator ctokRIGHTPAREN
            { $$ = $2; }

        | ctokLEFTPAREN unary.identifier.declarator ctokRIGHTPAREN postfixing.abstract.declarator
            { $$ = $2->modify_type_and($4.ty); }
        ;

/*                        */    /* ? */ /* ?.?.? */
paren.identifier.declarator: /*P299*/
          identifier
            { $$ = new declNode($1, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
        | ctokLEFTPAREN paren.identifier.declarator ctokRIGHTPAREN
            { $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
old.function.declarator: /*P301*/
          postfix.old.function.declarator
            { 
/*              OldStyleFunctionDefinition = TRUE; */
              $$ = $1; 
            }

        | ctokSTAR old.function.declarator
            { $$ = $2->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord($1)), declNode::NONE, SU); }

        | ctokSTAR pointer.type.qualifier.list old.function.declarator
            { $$ = $3->set_type_and(new ptrNode($2.tq, (typeNode *)0, Coord($1)), declNode::NONE, SU); }
        ;

/*                        */    /* ? */ /* ?.?.? */
postfix.old.function.declarator: /*P*/
          paren.identifier.declarator ctokLEFTPAREN identifier.list ctokRIGHTPAREN  
            { funcNode *f = new funcNode(typeNode::NONE, $3, (typeNode *)0, Coord($2));
	      f->is_knr (true);
	      $$ = $1->modify_type_and(f); }

        | ctokLEFTPAREN old.function.declarator ctokRIGHTPAREN
            { $$ = $2; }

        | ctokLEFTPAREN old.function.declarator ctokRIGHTPAREN postfixing.abstract.declarator
            { $$ = $2->modify_type_and($4.ty); }
        ;

/* 
    ANSI C section 3.7.1 states  

      "An identifier declared as a typedef name shall not be redeclared 
       as a parameter".  

    Hence the following is based only on IDENTIFIERs 
*/
/*                        */    /* ? */ /* ?.?.? */
/* SZG: Make declNode right away; no need to fix it in procNode::procNode */
identifier.list: /*P*/ /* only used by postfix.old.function.declarator */
          identifier
            { $$ = new decl_list();
	      declNode * d = new declNode($1, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      $$->push_back(d);
	    }

        | identifier.list ctokCOMMA identifier
            { $$ = $1;
	      declNode * d = new declNode($3, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      $$->push_back(d);
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
identifier.or.typedef.name: /*P*/
          identifier
        | typedefname
        ;

/*                        */    /* ? */ /* ?.?.? */
type.name: /*P*/
          type.specifier
            { $$.ty = $1.ty->finish_and();
	      $$.sc = $1.sc; }

        | type.specifier abstract.declarator
            { $$.ty = $2.ty->set_base_type_and($1.ty)->finish_and(); 
	      $$.sc = merge_sc($1.sc, $2.sc, $2.ty->coord()); }

        | type.qualifier.list /* DEFAULT_INT */
            { $$.ty = new primNode($1.tq); 
	      $$.sc = $1.sc; }

        | type.qualifier.list /* DEFAULT_INT */ abstract.declarator
	    { $$.ty = $2.ty->set_base_type_and(new primNode($1.tq)); 
	      $$.sc = merge_sc($1.sc, $2.sc, Coord($1.coord)); }
        ;


/* Productions for __attribute__ adapted from the original in gcc 2.6.3. */

attributes.opt:
      /* empty */
  		{ $$ = (attrib_list *)0; }
	| attributes
		{ $$ = $1; }
	;

attributes:
          attribute
		{ $$ = $1; }
	| attributes attribute
		{ $$ = $1; 
		  $1->splice($1->end(), * $2); 
		  delete $2; }
	;

attribute:
      ctokATTRIBUTE ctokLEFTPAREN ctokLEFTPAREN attribute.list ctokRIGHTPAREN ctokRIGHTPAREN
		{ if (CBZ::ANSIOnly)
	            CBZ::SyntaxError(string("__attribute__ not allowed with -ansi switch"));
                  $$ = $4; }
	;

attribute.list:
          attrib
		{ $$ = new attrib_list(); $$->push_back($1); }
	| attribute.list ctokCOMMA attrib
		{ $$ = $1; $$->push_back($3); }
	;
 
attrib:
    /* empty */
		{ $$ = 0; }
	| any.word
		{ $$ = new attribNode($1, (exprNode *)0); }
	| any.word ctokLEFTPAREN expression ctokRIGHTPAREN
		{ $$ = new attribNode($1, $3); }
	;


any.word:
	  identifier
	| typedefname
	| ctokCONST { $$ = new idNode("const", Coord($1)); }
	;

/*                        */    /* ? */ /* ?.?.? */
initializer.opt: /*P*/
          /* nothing */                  { $$ = (exprNode *)0; }
        | ctokEQUALS initializer                { $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
initializer: /*P*/
          left.enter.pragma initializer.list right.exit.pragma       { $$ = $2; $$->coord($1); }
        | left.enter.pragma initializer.list ctokCOMMA right.exit.pragma   { $$ = $2; $$->coord($1); }
        | assignment.expression          { $$ = $1; }
        ;

/*                        */    /* ? */ /* ?.?.? */
initializer.list: /*P*/
          initializer
            { $$ = new initializerNode(new expr_list(), $1->coord());
	      $$->add_expr($1); }

        | initializer.list ctokCOMMA initializer
            { $$ = $1;
	      $$->add_expr($3);
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.type.list: /*P*/
          parameter.list

        | parameter.list ctokCOMMA ctokELLIPSIS   
            { $$ = $1;
	      $$->push_back(new declNode((typeNode *) new primNode(basic_type::Ellipsis), declNode::NONE)); }

        /******** ERROR PRODUCTIONS (EAB) ********/
        | ctokELLIPSIS
            { /* primNode * n = new primNode(qualifier($1), basic_type::Void); */
	      CBZ::SyntaxError(Coord($1), string("First argument cannot be `...'"));
              $$ = new decl_list();
	      $$->push_back(new declNode(new primNode(typeNode::NONE, basic_type::Void), declNode::NONE));
            }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.list: /*P*/
          parameter.declaration
            { $$ = new decl_list(); 
	      $$->push_back($1); }

        | parameter.list ctokCOMMA parameter.declaration
            { $$ = $1;
	      $$->push_back($3); }

        /******** ERROR PRODUCTIONS (EAB) ********/
        | parameter.declaration ctokEQUALS initializer
            { 
	      CBZ::SyntaxError($1->coord(), string("formals cannot have initializers"));
	      $$ = new decl_list(); 
	      $$->push_back($1);
            }

        | parameter.list ctokCOMMA error
            { $$ = $1; }
        ;

/*                        */    /* ? */ /* ?.?.? */
parameter.declaration: /*P*/
          declaration.specifier
            { $$ = new declNode($1.ty, $1.sc); }

        | declaration.specifier abstract.declarator
            { $$ = new declNode($2.ty->set_base_type_and($1.ty),
				merge_sc($1.sc, $2.sc, $2.ty->coord())); }

        | declaration.specifier identifier.declarator
            { $$ = $2->set_type_and($1.ty, $1.sc, Formal); }

        | declaration.specifier parameter.typedef.declarator
            { $$ = $2->set_type_and($1.ty, $1.sc, Formal); }

        | declaration.qualifier.list /* DEFAULT_INT */ 
            { $$ = new declNode(new primNode($1.tq), $1.sc); }

        | declaration.qualifier.list /* DEFAULT_INT */ abstract.declarator
            { $$ = new declNode($2.ty->set_base_type_and(new primNode($1.tq)),
				merge_sc($1.sc, $2.sc, Coord($1.coord))); }

        | declaration.qualifier.list /* DEFAULT_INT */ identifier.declarator
            { $$ = $2->set_type_and(new primNode($1.tq), $1.sc, Formal); }

        | type.specifier
            { $$ = new declNode($1.ty, $1.sc); }

        | type.specifier abstract.declarator
            { $$ = new declNode($2.ty->set_base_type_and($1.ty),
				merge_sc($1.sc, $2.sc, $2.ty->coord())); }

        | type.specifier identifier.declarator
            { $$ = $2->set_type_and($1.ty, $1.sc, Formal); }

        | type.specifier parameter.typedef.declarator
            { $$ = $2->set_type_and($1.ty, $1.sc, Formal); }

        | type.qualifier.list /* DEFAULT_INT */ 
            { $$ = new declNode(new primNode($1.tq), $1.sc); }

        | type.qualifier.list /* DEFAULT_INT */ abstract.declarator
            { $$ = new declNode($2.ty->set_base_type_and(new primNode($1.tq)),
				merge_sc($1.sc, $2.sc, Coord($1.coord))); }

        | type.qualifier.list /* DEFAULT_INT */ identifier.declarator
            { $$ = $2->set_type_and(new primNode($1.tq), $1.sc, Formal); }
        ;


/********************************************************************************
*										*
*                      STRUCTURES, UNION, and ENUMERATORS			*
*										*
********************************************************************************/

/*                        */    /* ? */ /* ?.?.? */
struct.or.union.specifier: /*P*/
          struct.or.union left.enter.pragma struct.declaration.list right.exit.pragma
            { $$ = $1->set_name_fields_and((idNode *)0, $3, true, $2, $4); }

        | struct.or.union identifier.or.typedef.name
          left.enter.pragma struct.declaration.list right.exit.pragma
            { $$ = $1->set_name_fields_and($2, $4, true, $3, $5); }

        | struct.or.union identifier.or.typedef.name
            { $$ = $1->set_name_and($2, $1->coord()); }

        /* EAB: create rules for empty structure declarations */
        | struct.or.union left.enter.pragma right.exit.pragma
            { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      $$ = $1->set_name_fields_and((idNode *)0, (decl_list *)0, true, $2, $3);
	    }

        | struct.or.union identifier.or.typedef.name left.enter.pragma right.exit.pragma
            { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      $$ = $1->set_name_fields_and($2, (decl_list *)0, true, $3, $4);
	    }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.or.union: /*P*/ 
          ctokSTRUCT   { $$ = new structNode(Coord($1)); }
        | ctokUNION    { $$ = new unionNode(Coord($1)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaration.list: /*P*/
          struct.declaration
        | struct.declaration.list struct.declaration
            { $$ = $1;
	      $$->splice($$->end(), * $2);
	      delete $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaration: /*P*/
          struct.declaring.list ctokSEMICOLON
        | struct.default.declaring.list ctokSEMICOLON
        ;

/* doesn't redeclare typedef */
/*                        */    /* ? */ /* ?.?.? */
struct.default.declaring.list: /*P*/        
          type.qualifier.list struct.identifier.declarator
            { $$ = new decl_list();
	      $$->push_back($2->set_type_and(new primNode($1.tq), $1.sc, SU)); }

        | struct.default.declaring.list ctokCOMMA struct.identifier.declarator
            { $$ = $1;
	      $$->push_back($3->inherit_type_and($1, SU)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.declaring.list:  /*P*/       
          type.specifier struct.declarator
            { $$ = new decl_list();
	      $$->push_back($2->set_type_and($1.ty, $1.sc, SU)); }

        | struct.declaring.list ctokCOMMA struct.declarator
            { $$ = $1;
	      $$->push_back($3->inherit_type_and($1, SU)); }
        ;


/*                        */    /* ? */ /* ?.?.? */
struct.declarator: /*P*/
          declarator bit.field.size.opt attributes.opt
            { $$ = $1;
	      $$->bitsize($2);
              $$->merge_attribs($3); }

        | bit.field.size attributes.opt
            { $$ = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, $1, $1->coord());
              $$->merge_attribs($2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
struct.identifier.declarator: /*P*/
          identifier.declarator bit.field.size.opt attributes.opt
            { $$ = $1;
	      $$->bitsize($2);
              $$->merge_attribs($3); }

        | bit.field.size attributes.opt
            { $$ = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, $1, $1->coord());
              $$->merge_attribs($2); }
        ;

/*                        */    /* ? */ /* ?.?.? */
bit.field.size.opt: /*P*/
          /* nothing */   { $$ = (exprNode *)0; }
        | bit.field.size
        ;

/*                        */    /* ? */ /* ?.?.? */
bit.field.size: /*P*/
          ctokCOLON constant.expression { $$ = $2; }
        ;

/*                        */    /* ? */ /* ?.?.? */
enum.specifier: /*P*/
          ctokENUM left.enter.pragma enumerator.list comma.opt right.exit.pragma
            { $$ = new enumNode((idNode *)0, $3, $1, Coord($2), Coord($5)); }

        | ctokENUM identifier.or.typedef.name left.enter.pragma enumerator.list comma.opt right.exit.pragma
            { $$ = new enumNode($2, $4, $1, Coord($3), Coord($6));   }

        | ctokENUM identifier.or.typedef.name
            { $$ = new enumNode($2, (decl_list *)0, $1, $2->coord(), $2->coord()); }
        ;

/*                        */    /* ? */ /* ?.?.? */
enumerator.list: /*P*/
          identifier.or.typedef.name enumerator.value.opt
            { $$ = new decl_list();
	      $$->push_back(new declNode($1, $2)); }

        | enumerator.list ctokCOMMA identifier.or.typedef.name enumerator.value.opt
            { $$ = $1; 
	      $$->push_back(new declNode($3, $4)); }
        ;

/*                        */    /* ? */ /* ?.?.? */
enumerator.value.opt: /*P*/
          /* Nothing */               { $$ = (exprNode *)0; }
        | ctokEQUALS constant.expression     { $$ = $2;   }
        ;

comma.opt: /* not strictly ANSI */
          /* Nothing */    { }
        | ctokCOMMA              { }
        ;

/********************************************************************************
*										*
*                                  STATEMENTS					*
*										*
********************************************************************************/

statement:                      /* P */ /* 6.6   */
          labeled.statement
	    { $$ = $1; }

        | compound.statement
	    { $$ = $1; }

        | expression.statement
	    { $$ = $1; }

        | selection.statement
	    { $$ = $1; }

        | iteration.statement
	    { $$ = $1; }

        | jump.statement
	    { $$ = $1; }

/*
        | '$' ctokSTATEMENT_PH
            { $$ = new ph_stmtNode($1, false); }

        | ctokSTATEMENT_MACRO ctokLEFTBRACE statement.list ctokRIGHTBRACE
            {  $$ = expand_stmt_macro($1, $3); }
	    */
          /******** ERROR PRODUCTIONS ********/
        | error ctokSEMICOLON
           {  $$ = (stmtNode *) 0; }

        | stmt_pattern ';'    { $$ = $1; }

        ;

labeled.statement:              /* P */ /* 6.6.1 */
          identifier ctokCOLON statement
           { $$ = new labelNode($1, $3); }

        | ctokCASE constant.expression ctokCOLON statement
	   { $$ = new caseNode($2, $4, Coord($1)); }

        | ctokDEFAULT ctokCOLON statement
	   { $$ = new caseNode(0, $3, Coord($1)); }

          /* Required extension to 6.6.1 */
        | typedefname ctokCOLON statement
           { $$ = new labelNode($1, $3); }

        ;

compound.statement:             /* P */ /* 6.6.2 */
          ctokLEFT ctokRIGHT
            { $$ = new blockNode((decl_list *)0, (stmt_list *)0, Coord($1), Coord($2)); }

        | ctokLEFT declaration.list ctokRIGHT
            { $$ = new blockNode($2, (stmt_list *)0, Coord($1), Coord($3)); }

        | ctokLEFT statement.list ctokRIGHT
            { $$ = new blockNode((decl_list *)0, $2, Coord($1), Coord($3)); }

        | ctokLEFT declaration.list statement.list ctokRIGHT
            { $$ = new blockNode($2, $3, Coord($1), Coord($4)); }
        ;

ctokLEFT:
    ctokLEFTBRACE  { $$ = $1; CBZ::current_unit->enter_scope(); }
    ;

ctokRIGHT:
    ctokRIGHTBRACE  { $$ = $1; CBZ::current_unit->exit_scope(); }
    ;

left.enter.pragma:  ctokLEFTBRACE
            { $$ =$1; CBZ::current_unit->enter_pragmas(); } ;
right.exit.pragma: ctokRIGHTBRACE
            { $$ = $1; CBZ::current_unit->exit_pragmas(); } ;

/* compound.statement.no.new.scope is used by function.definition,
   since the new scope is begun with formal argument declarations,
   not with the opening ctokLEFTBRACE */

compound.statement.no.new.scope:             /* P */ /* 6.6.2 */
          ctokLEFTBRACE ctokRIGHTBRACE
            { $$ = new blockNode((decl_list *)0, (stmt_list *)0, Coord($1), Coord($2)); }

        | ctokLEFTBRACE declaration.list ctokRIGHTBRACE
            { $$ = new blockNode($2, (stmt_list *)0, Coord($1), Coord($3)); }

        | ctokLEFTBRACE statement.list ctokRIGHTBRACE
            { $$ = new blockNode((decl_list *)0, $2, Coord($1), Coord($3)); }

        | ctokLEFTBRACE declaration.list statement.list ctokRIGHTBRACE
            { $$ = new blockNode($2, $3, Coord($1), Coord($4)); }

        ;

declaration.list:               /* P */ /* 6.6.2 */
          declaration
            { $$ = $1; }

        | declaration.list declaration
            { $$ = $1;
	      $$->splice($1->end(), * $2); 
	      delete $2;
	    }

        ;

statement.list:                 /* P */ /* 6.6.2 */
          statement
            { $$ = new stmt_list();
              $$->push_back($1); }

        | statement.list statement
            { $$ = $1;
              $$->push_back($2); }
/*
        | ctokSTMT_LIST_MACRO ctokLEFTBRACE statement.list ctokRIGHTBRACE
            { $$ = expand_stmt_list_macro($1, $3); }
	    */
        ;

expression.statement:           /* P */ /* 6.6.3 */
          expression.opt ctokSEMICOLON
            { $$ = new exprstmtNode($1); }
        ;

selection.statement:            /* P */ /* 6.6.4 */
          ctokIF ctokLEFTPAREN expression ctokRIGHTPAREN statement
            { $$ = new ifNode($3, $5, 0, Coord($1)); }

        | ctokIF ctokLEFTPAREN expression ctokRIGHTPAREN statement ctokELSE statement
            { $$ = new ifNode ($3, $5, $7, Coord($1), Coord($6)); }

        | ctokSWITCH ctokLEFTPAREN expression ctokRIGHTPAREN statement
            { $$ = new switchNode($3, $5, Coord($1)); }
        ;

iteration.statement:            /* P */ /* 6.6.5 */
          ctokWHILE ctokLEFTPAREN expression ctokRIGHTPAREN statement
            { $$ = new whileNode($3, $5, Coord($1)); }

        | ctokDO statement ctokWHILE ctokLEFTPAREN expression ctokRIGHTPAREN ctokSEMICOLON
            { $$ = new doNode($2, $5, Coord($1), Coord($3)); }

        | ctokFOR ctokLEFTPAREN expression.opt ctokSEMICOLON expression.opt ctokSEMICOLON expression.opt ctokRIGHTPAREN  statement
            { $$ = new forNode($3, $5, $7, $9, Coord($1)); }

        /******** ERROR PRODUCTIONS (EAB) ********/

        | ctokFOR ctokLEFTPAREN error ctokSEMICOLON expression.opt ctokSEMICOLON expression.opt ctokRIGHTPAREN  statement
            { $$ = new forNode((exprNode *)0, $5, $7, $9, Coord($1)); }

        | ctokFOR ctokLEFTPAREN expression.opt ctokSEMICOLON expression.opt ctokSEMICOLON error ctokRIGHTPAREN  statement
            { $$ = new forNode($3, $5, (exprNode *)0, $9, Coord($1)); }

        | ctokFOR ctokLEFTPAREN expression.opt ctokSEMICOLON error ctokSEMICOLON expression.opt ctokRIGHTPAREN  statement
            { $$ = new forNode($3, (exprNode *)0, $7, $9, Coord($1)); }

        | ctokFOR ctokLEFTPAREN error ctokRIGHTPAREN  statement
            { $$ = new forNode((exprNode *)0, (exprNode *)0, (exprNode *)0, $5, Coord($1)); }
        ;

jump.statement:                 /* P */ /* 6.6.6 */
          ctokGOTO identifier ctokSEMICOLON
            { $$ = new gotoNode($2, Coord($1)); }

        | ctokCONTINUE ctokSEMICOLON
            { $$ = new continueNode(Coord($1)); }

        | ctokBREAK ctokSEMICOLON
            { $$ = new breakNode(Coord($1)); }

        | ctokRETURN expression.opt ctokSEMICOLON
            { $$ = new returnNode($2, procNode::current(), Coord($1)); }

        /* Required extension/clarification to 6.6.6 */
        | ctokGOTO typedefname ctokSEMICOLON
            { $$ = new gotoNode($2, Coord($1)); }
        ;

/********************************************************************************
*										*
*                            EXTERNAL DEFINITIONS                               *
*										*
********************************************************************************/

translation.unit:               /* P */ /* 6.7   */
          external.definition
            { if ($1 != 0) CBZ::current_unit->merge_in($1); }

        | translation.unit external.definition   
            { CBZ::current_unit->merge_in($2); }
        ;

external.definition:            /* P */ /* 6.7   */
          declaration
            {
              $$ = new def_list();
	      while (! $1->empty()) {
		defNode * d = $1->front();
                d->grab_pragmas();
		$1->pop_front();
		$$->push_front(d);
	      }
	      delete $1;
            }
        | function.definition  
            { 
              $1->grab_pragmas();
              $$ = new def_list();
	      $$->push_back($1);
            }
        ;

function.definition:            /* P */ /* BASED ON 6.7.1 */
          identifier.declarator
            /*{ $$ = new procNode(false, $1->set_type_and(new primNode($1->coord()), declNode::NONE, Redecl)); } djimenez*/
            { $$ = new procNode(false, $1->set_type_and(new primNode($1->coord()), $1->storage_class(), Redecl)); }
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($3); }

        | identifier.declarator ctokBOGUS
            /* this rule is never used, it exists solely to force the parser to
	       read the ctokLEFTBRACE token for the previous rule, thus starting a create
	       scope in the correct place */
	    { $$ = new procNode(false, $1); }

        | declaration.specifier      identifier.declarator
            { $$ = new procNode(false, $2->set_type_and($1.ty, $1.sc, Redecl)); }
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | type.specifier             identifier.declarator
/* djimenez - does $1 have a storage class, or should we use $2? */

            { $$ = new procNode(false, $2->set_type_and($1.ty, $1.sc, Redecl)); }
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | declaration.qualifier.list identifier.declarator
            { $$ = new procNode(false, $2->set_type_and(new primNode($1.tq), $1.sc, Redecl)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }
        | type.qualifier.list        identifier.declarator
            { $$ = new procNode(false, $2->set_type_and(new primNode($1.tq), $1.sc, Redecl)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | old.function.declarator
            /*{ $$ = new procNode(true, $1->set_type_and(new primNode($1->coord()), declNode::NONE, Redecl)); }djimenez*/
            { $$ = new procNode(true, $1->set_type_and(new primNode($1->coord()), $1->storage_class(), Redecl)); }
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($3); }

        | declaration.specifier old.function.declarator 
            {  $$ = new procNode(true, 
				 $2->set_type_and($1.ty, $1.sc, Redecl)->
				     add_parameter_types_and((decl_list *)0)); }
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | type.specifier old.function.declarator
            { $$ = new procNode(true, 
				$2->set_type_and($1.ty, $1.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | declaration.qualifier.list old.function.declarator
            { $$ = new procNode(true,
				$2->set_type_and(new primNode($1.tq), $1.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0));
            } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | type.qualifier.list        old.function.declarator
            { $$ = new procNode(true,
				$2->set_type_and(new primNode($1.tq), $1.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | old.function.declarator old.function.declaration.list
            { $$ = new procNode(true,
				$1->set_type_and(new primNode($1->coord()), declNode::NONE, Redecl)->
				    add_parameter_types_and($2)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($4); }

        | declaration.specifier old.function.declarator old.function.declaration.list
            { $$ = new procNode(true,
				$2->set_type_and($1.ty, $1.sc, Redecl)->
				    add_parameter_types_and($3)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($5); }

        | type.specifier old.function.declarator old.function.declaration.list
	    { $$ = new procNode(true,
				$2->set_type_and($1.ty, $1.sc, Redecl)->
				    add_parameter_types_and($3)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($5); }

       | declaration.qualifier.list old.function.declarator old.function.declaration.list
            { $$ = new procNode(true,
				$2->set_type_and(new primNode($1.tq), $1.sc, Redecl)->
				    add_parameter_types_and($3)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($5); }

        | type.qualifier.list old.function.declarator old.function.declaration.list
            { $$ = new procNode(true, $2->set_type_and(new primNode($1.tq), $1.sc, Redecl)->
				          add_parameter_types_and($3)); } 
          compound.statement.no.new.scope
            { $$ = procNode::current()->define_and($5); }
        ;


old.function.declaration.list:
             { CBZ::OldStyleFunctionDefinition = true; }
             declaration.list
             { CBZ::OldStyleFunctionDefinition = false; 
	       $$ = $2; }
        ;

/********************************************************************************
*										*
*                          CONSTANTS and LITERALS                               *
*										*
********************************************************************************/

/* 
  CONSTANTS.  Note ENUMERATIONconstant is treated like a variable with a type
  of "enumeration constant" (elsewhere)
*/
constant: /*P*/
          FLOATINGconstant      { $$ = $1; }
        | INTEGERconstant       { $$ = $1; }
        | OCTALconstant         { $$ = $1; }
        | HEXconstant           { $$ = $1; }
        | CHARACTERconstant     { $$ = $1; }
        ;

/* STRING LITERALS */
string.literal.list: /*P*/
          STRINGliteral         { $$ = $1; }
        | string.literal.list STRINGliteral
            {
	      string s3 = string($1->value().Str()) + string($2->value().Str());
              /* append text(), get rid of the quotes in between. */
              string t1 = $1->text().substr(0,$1->text().length()-1),
                     t2 = $2->text().substr(1,string::npos);
	      $$ = new constNode(constant(s3.c_str()), (t1+t2).c_str(), $1->coord());
	      delete $1;
	      delete $2;
	    }
        ;

type.qualifier: /*P*/
          ctokCONST     { $$.tq = typeNode::CONST;    $$.sc = declNode::NONE; $$.coord = $1; }
        | ctokVOLATILE  { $$.tq = typeNode::VOLATILE; $$.sc = declNode::NONE; $$.coord = $1; }
        | ctokINLINE    { $$.tq = typeNode::INLINE;   $$.sc = declNode::NONE; $$.coord = $1; }
        ;

pointer.type.qualifier: /*P*/
          ctokCONST     { $$.tq = typeNode::CONST;    $$.sc = declNode::NONE; $$.coord = $1; }
        | ctokVOLATILE  { $$.tq = typeNode::VOLATILE; $$.sc = declNode::NONE; $$.coord = $1; }
        ;

storage.class: /*P*/
          ctokTYPEDEF   { $$.sc = declNode::TYPEDEF;  $$.tq = typeNode::NONE; $$.coord = $1; }
        | ctokEXTERN    { $$.sc = declNode::EXTERN;   $$.tq = typeNode::NONE; $$.coord = $1; }
        | ctokSTATIC    { $$.sc = declNode::STATIC;   $$.tq = typeNode::NONE; $$.coord = $1; }
        | ctokAUTO      { $$.sc = declNode::AUTO;     $$.tq = typeNode::NONE; $$.coord = $1; }
        | ctokREGISTER  { $$.sc = declNode::REGISTER; $$.tq = typeNode::NONE; $$.coord = $1; }
        ;

basic.type.name: /*P*/
          ctokVOID      { $$ = new primNode(basic_type::Void, Coord($1));    } 
        | ctokCHAR      { $$ = new primNode(basic_type::Char, Coord($1));    } 
        | ctokINT       { $$ = new primNode(basic_type::Int, Coord($1));     } 
        | ctokFLOAT     { $$ = new primNode(basic_type::Float, Coord($1));   } 
        | ctokDOUBLE    { $$ = new primNode(basic_type::Double, Coord($1));  } 

        | ctokSIGNED    { $$ = new primNode(basic_type::Signed, Coord($1));  } 
        | ctokUNSIGNED  { $$ = new primNode(basic_type::Unsigned, Coord($1));} 

        | ctokSHORT     { $$ = new primNode(basic_type::Short, Coord($1));   } 
        | ctokLONG      { $$ = new primNode(basic_type::Long, Coord($1));    }
        ;

identifier:
          IDENTIFIER
        ;

typedefname:
          TYPEDEFname
        ;

/* ============================================================

     Code patterns (meta-symbols)

   ============================================================
*/

expr_pattern:

    metatokID
      {
	$$ = new exprPatternNode($1);
	delete $1;
      }

  | metatokEXPR '(' metatokID ')'
      {
	$$ = new exprPatternNode($3);
	delete $3;
      }
  ;

stmt_pattern:

    metatokSTMT '(' metatokID ')'
      {
	$$ = new stmtPatternNode($3);
	delete $3;
      }
  ;

type_pattern:

    metatokTYPE '(' metatokID ')'
      {
	$$.ty = new typePatternNode($3);
	$$.sc = declNode::NONE;
	delete $3;
      }
  ;

%%
