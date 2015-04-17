%{

#include "broadway.h"
#include "language.tab.h"

// --- Fix the names from the C parser

#define cblval annlval
#define cbtext anntext
#define cbleng annleng

// --- Routines from the C parser for handling constants

extern exprNode *char_constant(const char *);
extern exprNode *int_constant(const char *, bool decimal, bool octal);
extern exprNode *wide_char_const(const char *);
extern exprNode *float_constant(const char *s);
extern char * convert_escape_sequences(char *string);
void parse_line_directive(char *s);
void save_pragma_directive(char *s);

void EnterC(void);
void ExitC(void);

// --- Line number offset (to deal with preprocessing the code). We
// update this number when we see #line directives.

void adjust_line_number(char * text);

int line_number_offset = 0;

%}

	/* -- Prefix all symbols with "ann" */

%option prefix="ann"
%option noyywrap
%option yylineno

	/* -- Define the four states: "BDWY" for all Broadway annotations
           language tokens, "C" for C code fragments, and "META" for
           meta-symbols. The meta-symbols always embedded in the C
           patterns. The "BDWYID" state is used for proecdure names, which
           could conflict with reserved words in the annotation language. */

%s BDWY
%s BDWYID
%s C
%s META

	/* -- Define some useful short-hands */

identifier ([a-zA-Z_][0-9a-zA-Z_.]*)
any_white ([ \011\013\014\015])
hwhite ([ \011])

exponent_part ([eE][-+]?[0-9]+)
fractional_constant ([0-9]*"."[0-9]+)|([0-9]+".")
floating_constant ((({fractional_constant}{exponent_part}?)|([0-9]+{exponent_part}))[FfLl]?)

integer_suffix_opt ([uU]?[lL]?[lL]?)|([lL][uU][lL]?)|([lL][lL][uU])
decimal_constant ("0"{integer_suffix_opt}|[1-9][0-9]*{integer_suffix_opt})
octal_constant ("0"[0-7]*{integer_suffix_opt})
hex_constant ("0"[xX][0-9a-fA-F]+{integer_suffix_opt})

simple_escape ([abfnrtv'"?\\])
octal_escape  ([0-7][^'\n]*)
hex_escape ("x"[^'\n]*)
bad_escape ([\\][^abfnrtv'"?\\x0-7\n ])

escape_sequence ([\\]({simple_escape}|{octal_escape}|{hex_escape}))
c_char ([^'\\\n]|{escape_sequence})
s_char ([^"\\\n]|{escape_sequence})

%%

	/* -- General rules for handling line number directives */

<INITIAL,C,BDWY,META>^{hwhite}*"#"{hwhite}*[0-9].*"\n" |
<INITIAL,C,BDWY,META>^{hwhite}*"#"{hwhite}*line.*"\n" { adjust_line_number(anntext); }

	/* -- State transition (context sensitivity) rules */

<INITIAL,BDWY>"%{"  { BEGIN(C); }
<C>"}%"             { BEGIN(BDWY); }

<C>"${"             { BEGIN(META); }
<META>"}"           { BEGIN(C); }

	/* -- General rules for handling white-space (including regular line number
              adjustment */

{any_white}+     {  }

"\n"             {  }

"//".*"\n"       {  }

<INITIAL>.       { annlval.t_tok = (annlineno - line_number_offset); return anntext[0]; }

	/* -- Broadway annotation tokens ------------------------------------------------------ */

<BDWY>"property"       { annlval.t_tok = (annlineno - line_number_offset); return tokMAYPROPERTY; }
<BDWY>"may-property"   { annlval.t_tok = (annlineno - line_number_offset); return tokMAYPROPERTY; }
<BDWY>"must-property"  { annlval.t_tok = (annlineno - line_number_offset); return tokMUSTPROPERTY; }
<BDWY>"initially"      { annlval.t_tok = (annlineno - line_number_offset); return tokINITIALLY; }
<BDWY>"diagnostic"     { annlval.t_tok = (annlineno - line_number_offset); return tokDIAGNOSTIC; /* TB new */ }

<BDWY>"global"         { annlval.t_tok = (annlineno - line_number_offset); return tokGLOBAL; }

<BDWY>"procedure"      { annlval.t_tok = (annlineno - line_number_offset);
                         BEGIN(BDWYID);
                         return tokPROCEDURE; }

<BDWY>"enable"         { annlval.t_tok = (annlineno - line_number_offset); return tokENABLE; }
<BDWY>"disable"        { annlval.t_tok = (annlineno - line_number_offset); return tokDISABLE; }

<BDWYID>{identifier}   { annlval.t_id = new parserID(anntext, (annlineno - line_number_offset));
                         BEGIN(BDWY);
                         return tokID; }

<BDWY>"access"         { annlval.t_tok = (annlineno - line_number_offset); return tokACCESS; }
<BDWY>"modify"         { annlval.t_tok = (annlineno - line_number_offset); return tokMODIFY; }

<BDWY>"analyze"        { annlval.t_tok = (annlineno - line_number_offset); return tokANALYZE; }
<BDWY>"default"        { annlval.t_tok = (annlineno - line_number_offset); return tokDEFAULT ; }
<BDWY>"if"             { annlval.t_tok = (annlineno - line_number_offset); return tokIF; }

<BDWY>"is-exactly"     { annlval.t_tok = (annlineno - line_number_offset); return tokIS_EXACTLY; }
<BDWY>"could-be"       { annlval.t_tok = (annlineno - line_number_offset); return tokCOULD_BE; }

<BDWY>"is-atleast"     { annlval.t_tok = (annlineno - line_number_offset); return tokIS_ATLEAST; }
<BDWY>"is-atmost"      { annlval.t_tok = (annlineno - line_number_offset); return tokIS_ATMOST; }
<BDWY>"is-??"          { annlval.t_tok = (annlineno - line_number_offset); return tokIS_BOTTOM; }

<BDWY>"@before"        { annlval.t_tok = (annlineno - line_number_offset); return tokBEFORE; }
<BDWY>"@after"         { annlval.t_tok = (annlineno - line_number_offset); return tokAFTER; }
<BDWY>"@always"        { annlval.t_tok = (annlineno - line_number_offset); return tokALWAYS; }
<BDWY>"@ever"          { annlval.t_tok = (annlineno - line_number_offset); return tokEVER; }
<BDWY>"@trace"         { annlval.t_tok = (annlineno - line_number_offset); return tokTRACE; }
<BDWY>"@confidence"    { annlval.t_tok = (annlineno - line_number_offset); return tokCONFIDENCE; }

<BDWY>"@forward"       { annlval.t_tok = (annlineno - line_number_offset); return tokFORWARD; }
<BDWY>"@backward"      { annlval.t_tok = (annlineno - line_number_offset); return tokBACKWARD; }

<BDWY>"is-aliasof"     { annlval.t_tok = (annlineno - line_number_offset); return tokIS_ALIASOF; }
<BDWY>"is-sameas"      { annlval.t_tok = (annlineno - line_number_offset); return tokIS_SAMEAS; }
<BDWY>"is-empty"       { annlval.t_tok = (annlineno - line_number_offset); return tokIS_EMPTY; }

<BDWY>"on_entry"       { annlval.t_tok = (annlineno - line_number_offset); return tokON_ENTRY; }
<BDWY>"on_exit"        { annlval.t_tok = (annlineno - line_number_offset); return tokON_EXIT; }
<BDWY>"new"            { annlval.t_tok = (annlineno - line_number_offset); return tokNEW; }
<BDWY>"-->"            { annlval.t_tok = (annlineno - line_number_offset); return tokARROW; }
<BDWY>"delete"         { annlval.t_tok = (annlineno - line_number_offset); return tokDELETE; }

<BDWY>"pattern"        { annlval.t_tok = (annlineno - line_number_offset); return tokPATTERN; }

<BDWY>"&&"             { annlval.t_tok = (annlineno - line_number_offset); return tokANDAND; }
<BDWY>"||"             { annlval.t_tok = (annlineno - line_number_offset); return tokOROR; }

<BDWY>"!="             { annlval.t_tok = (annlineno - line_number_offset); return tokNOT_EQUALS; }
<BDWY>"=="             { annlval.t_tok = (annlineno - line_number_offset); return tokEQUALS_EQUALS; }
<BDWY>"<="             { annlval.t_tok = (annlineno - line_number_offset); return tokLESS_EQUALS; }
<BDWY>">="             { annlval.t_tok = (annlineno - line_number_offset); return tokGREATER_EQUALS; }

<BDWY>"is-#"           { annlval.t_tok = (annlineno - line_number_offset); return tokIS_CONSTANT; }

<BDWY>"="              { annlval.t_tok = (annlineno - line_number_offset); return tokEQUALS; }

<BDWY>"<-"             { annlval.t_tok = (annlineno - line_number_offset); return tokASSIGN; }
<BDWY>"<-+"            { annlval.t_tok = (annlineno - line_number_offset); return tokWEAKASSIGN; }

<BDWY>"report"        { annlval.t_tok = (annlineno - line_number_offset); return tokREPORT; }
<BDWY>"error"         { annlval.t_tok = (annlineno - line_number_offset); return tokERROR; }
<BDWY>"++"            { annlval.t_tok = (annlineno - line_number_offset); return tokAPPEND; }

<BDWY>"when"          { annlval.t_tok = (annlineno - line_number_offset); return tokWHEN; }
<BDWY>"replace-with"  { annlval.t_tok = (annlineno - line_number_offset); return tokREPLACE; }
<BDWY>"inline"        { annlval.t_tok = (annlineno - line_number_offset); return tokINLINE; }

<BDWY>"{union-set}"     { annlval.t_tok = (annlineno - line_number_offset); return tokUNION_SET; }
<BDWY>"{intersect-set}" { annlval.t_tok = (annlineno - line_number_offset); return tokINTERSECT_SET; }
<BDWY>"{union-equivalence}" { annlval.t_tok = (annlineno - line_number_offset); return tokUNION_EQUIV; }
<BDWY>"{intersect-equivalence}" { annlval.t_tok = (annlineno - line_number_offset); return tokINTERSECT_EQUIV; }

<BDWY>"is-element-of" { annlval.t_tok = (annlineno - line_number_offset); return tokIS_ELEMENT_OF; }
<BDWY>"is-{}"         { annlval.t_tok = (annlineno - line_number_offset); return tokIS_EMPTYSET; }

<BDWY>"set-add"       { annlval.t_tok = (annlineno - line_number_offset); return tokADD; }

<BDWY>{identifier}     { annlval.t_id = new parserID(anntext, (annlineno - line_number_offset));
                         return tokID;
                       }

<BDWY>"I/O"           { annlval.t_tok = (annlineno - line_number_offset); return tokIO; }

<BDWY>{decimal_constant}  { exprNode * e = int_constant(anntext, true, false);
                            if (e->typ() == Const)
                              annlval.t_c_expression = (constNode *) e;
                            else
                              annlval.t_c_expression = 0;
                            return tokCONSTANT; }

<BDWY>{octal_constant}    { exprNode * e = int_constant(anntext, false, true);
                            if (e->typ() == Const)
                              annlval.t_c_expression = (constNode *) e;
                            else
                              annlval.t_c_expression = 0;
                            return tokCONSTANT; }

<BDWY>{hex_constant}      { exprNode * e = int_constant(anntext, false, false);
                            if (e->typ() == Const)
                              annlval.t_c_expression = (constNode *) e;
                            else
                              annlval.t_c_expression = 0;
                            return tokCONSTANT; }

<BDWY>{floating_constant}[^.] { yyless(annleng-1);
                                exprNode * e = float_constant(anntext);
                                if (e->typ() == Const)
                                  annlval.t_c_expression = (constNode *) e;
                                else
                                  annlval.t_c_expression = 0;
                                return tokCONSTANT; }


<BDWY>[']{c_char}+[']     { exprNode * e = char_constant(anntext);
                            if (e->typ() == Const)
                              annlval.t_c_expression = (constNode *) e;
                            else
                              annlval.t_c_expression = 0;
                            return tokCONSTANT; }

<BDWY>["]{s_char}*["]     { char * text = strdup(&anntext[1]);
                            int length = strlen(text);
                            text[length - 1] = '\0';
                            annlval.t_id = new parserID(convert_escape_sequences(text),
                                                        (annlineno - line_number_offset));
                            free(text);
                            return tokSTRING; }

<BDWY>.              { annlval.t_tok = (annlineno - line_number_offset); return anntext[0]; }

	/* -- C grammar tokens ---------------------------------------------------------------- */

<C>{any_white}+     {  }

<C>auto                { cblval.tok = get_pos(); return ctokAUTO;}
<C>break               { cblval.tok = get_pos(); return ctokBREAK;}
<C>case                { cblval.tok = get_pos(); return ctokCASE;}
<C>char                { cblval.tok = get_pos(); return ctokCHAR;}
<C>const        |
<C>__const             { cblval.tok = get_pos(); return ctokCONST;}
<C>continue            { cblval.tok = get_pos(); return ctokCONTINUE;}
<C>default             { cblval.tok = get_pos(); return ctokDEFAULT;}
<C>do                  { cblval.tok = get_pos(); return ctokDO;}
<C>double              { cblval.tok = get_pos(); return ctokDOUBLE;}
<C>else                { cblval.tok = get_pos(); return ctokELSE;}
<C>enum                { cblval.tok = get_pos(); return ctokENUM;}
<C>extern              { cblval.tok = get_pos(); return ctokEXTERN;}
<C>float               { cblval.tok = get_pos(); return ctokFLOAT;}
<C>for                 { cblval.tok = get_pos(); return ctokFOR;}
<C>goto                { cblval.tok = get_pos(); return ctokGOTO;}
<C>if                  { cblval.tok = get_pos(); return ctokIF;}
<C>int                 { cblval.tok = get_pos(); return ctokINT;}
<C>long                { cblval.tok = get_pos(); return ctokLONG;}
<C>register            { cblval.tok = get_pos(); return ctokREGISTER;}
<C>return              { cblval.tok = get_pos(); return ctokRETURN;}
<C>short               { cblval.tok = get_pos(); return ctokSHORT;}
<C>signed              { cblval.tok = get_pos(); return ctokSIGNED;}
<C>sizeof              { cblval.tok = get_pos(); return ctokSIZEOF;}
<C>static              { cblval.tok = get_pos(); return ctokSTATIC;}
<C>struct              { cblval.tok = get_pos(); return ctokSTRUCT;}
<C>switch              { cblval.tok = get_pos(); return ctokSWITCH;}
<C>typedef             { cblval.tok = get_pos(); return ctokTYPEDEF;}
<C>union               { cblval.tok = get_pos(); return ctokUNION;}
<C>unsigned            { cblval.tok = get_pos(); return ctokUNSIGNED;}
<C>void                { cblval.tok = get_pos(); return ctokVOID;}
<C>volatile            { cblval.tok = get_pos(); return ctokVOLATILE;}
<C>while               { cblval.tok = get_pos(); return ctokWHILE;}

<C>inline   |
<C>__inline |
<C>__inline__          { cblval.tok = get_pos(); return ctokINLINE; }
<C>__attribute__       { cblval.tok = get_pos(); return ctokATTRIBUTE; }


<C>{identifier} { cblval.tok = get_pos();
                  cblval.idN = new idNode(cbtext, cblval.tok);
                  if (CBZ::current_unit->types()->is_a_type(cblval.idN->name())) {
                    cblval.idN->decl(CBZ::current_unit->types()->lookup(cblval.idN->name()));
                    return TYPEDEFname;
                  }
                  else
                   return IDENTIFIER; }

<C>{decimal_constant}  { cblval.exprN = int_constant(cbtext, true, false);
                         return INTEGERconstant; }

<C>{octal_constant}    { cblval.exprN = int_constant(cbtext, false, true);
                         return OCTALconstant; }

<C>{hex_constant}      { cblval.exprN = int_constant(cbtext, false, false);
                         return HEXconstant; }

<C>{floating_constant}[^.] { yyless(cbleng-1);
                             cblval.exprN = float_constant(cbtext);
                             return FLOATINGconstant; }


<C>[']{c_char}+[']     { cblval.exprN = char_constant(cbtext);
                         return CHARACTERconstant; }

<C>[']{c_char}*[\n]    { CBZ::SyntaxError(string("Unmatched '"));
                         cblval.exprN = char_constant("'X'");
                         CBZ::current_line++;
                         return CHARACTERconstant; }


<C>['][^\\\n'][^'\n]+[']           |
<C>['][']                          |
<C>[']{bad_escape}[^'\n]*[']    { /* bad char constant */
                                  cblval.exprN = char_constant("'X'");
                                  CBZ::SyntaxError(
                                    string("invalid char constant ") + string(cbtext) +
                                    string(", treating as 'X'"));
                                  return CHARACTERconstant;
                                }


<C>"L"[']{c_char}+[']  { cblval.exprN = wide_char_const(cbtext);
                         return WIDECHARconstant;
                        }


<C>["]{s_char}*["]     { int length = strlen(cbtext);
                         char * text = strdup(cbtext);  /* save original */
                         cbtext[length-1] = 0;  /* remove trailing double quote */
		         cblval.tok = get_pos();
                         /* remove leading double quote */
                         cblval.exprN = new constNode(
			     constant(convert_escape_sequences(&cbtext[1])),
                             text, cblval.tok);
                         return STRINGliteral;}

<C>"L"["]{s_char}*["]  { int length = strlen(cbtext);
                         char * text = strdup(cbtext);  /* save original */
                         cbtext[length-1] = 0;  /* remove trailing double quote */
		         cblval.tok = get_pos();
                         /* remove leading double quote */
                         cblval.exprN = new constNode(
			     constant(convert_escape_sequences(&cbtext[1])),
                             text, cblval.tok);
                         return STRINGliteral;}

<C>["]{s_char}*{bad_escape}{s_char}*["]    { /* bad string constant */
            /* (unterminated string constants are caught by preprocessor) */
                      CBZ::SyntaxError(
                        string("string contains invalid escape code") +
                        string(cbtext));
		      cblval.tok = get_pos();
                      cblval.exprN = new constNode(constant(new string("")),
                                                   "\"\"",cblval.tok);
                      return STRINGliteral;
                    }


<C>^{hwhite}*"#"{hwhite}*[0-9].*"\n" |
<C>^{hwhite}*"#"{hwhite}*line.*"\n" {
                     parse_line_directive(cbtext); }

<C>^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"+"[Cc]{hwhite}*"\n" { EnterC(); CBZ::current_line++; }

<C>^{hwhite}*"#"{hwhite}*pragma{hwhite}+lang{hwhite}+"-"[Cc]{hwhite}*"\n" { ExitC(); CBZ::current_line++; }

<C>^{hwhite}*"#"{hwhite}*(pragma|ident).*"\n" {
                     save_pragma_directive(cbtext);
                     CBZ::current_line++; }

<C>"{"                  { cblval.tok = get_pos(); return (int)ctokLEFTBRACE;}
<C>"}"                  { cblval.tok = get_pos(); return (int)ctokRIGHTBRACE;}

<C>"["                  { cblval.tok = get_pos(); return (int)ctokLEFTBRACKET;}
<C>"]"                  { cblval.tok = get_pos(); return (int)ctokRIGHTBRACKET;}
<C>"."                  { cblval.tok = get_pos(); return (int)ctokDOT;}
<C>"&"                  { cblval.tok = get_pos(); return (int)ctokAND;}
<C>"*"                  { cblval.tok = get_pos(); return (int)ctokSTAR;}
<C>"+"                  { cblval.tok = get_pos(); return (int)ctokPLUS;}
<C>"-"                  { cblval.tok = get_pos(); return (int)ctokMINUS;}
<C>"~"                  { cblval.tok = get_pos(); return (int)ctokTILDE;}
<C>"!"                  { cblval.tok = get_pos(); return (int)ctokNOT;}
<C>"/"                  { cblval.tok = get_pos(); return (int)ctokDIVIDE;}
<C>"%"                  { cblval.tok = get_pos(); return (int)ctokPERCENT;}
<C>"<"                  { cblval.tok = get_pos(); return (int)ctokLESS;}
<C>">"                  { cblval.tok = get_pos(); return (int)ctokGREATER;}
<C>"^"                  { cblval.tok = get_pos(); return (int)ctokHAT;}
<C>"|"                  { cblval.tok = get_pos(); return (int)ctokOR;}
<C>"?"                  { cblval.tok = get_pos(); return (int)ctokQUESTION;}
<C>":"                  { cblval.tok = get_pos(); return (int)ctokCOLON;}
<C>";"                  { cblval.tok = get_pos(); return (int)ctokSEMICOLON;}
<C>"="                  { cblval.tok = get_pos(); return (int)ctokEQUALS;}

<C>"("                  { cblval.tok = get_pos(); return (int)ctokLEFTPAREN;}
<C>")"                  { cblval.tok = get_pos(); return (int)ctokRIGHTPAREN;}
<C>","                  { cblval.tok = get_pos(); return (int)ctokCOMMA;}

<C>"->"                 { cblval.tok = get_pos(); return ctokARROW;}
<C>"++"                 { cblval.tok = get_pos(); return ctokICR;}
<C>"--"                 { cblval.tok = get_pos(); return ctokDECR;}
<C>"<<"                 { cblval.tok = get_pos(); return ctokLS;}
<C>">>"                 { cblval.tok = get_pos(); return ctokRS;}
<C>"<="                 { cblval.tok = get_pos(); return ctokLE;}
<C>">="                 { cblval.tok = get_pos(); return ctokGE;}
<C>"=="                 { cblval.tok = get_pos(); return ctokEQ;}
<C>"!="                 { cblval.tok = get_pos(); return ctokNE;}
<C>"&&"                 { cblval.tok = get_pos(); return ctokANDAND;}
<C>"||"                 { cblval.tok = get_pos(); return ctokOROR;}
<C>"*="                 { cblval.tok = get_pos(); return ctokMULTassign;}
<C>"/="                 { cblval.tok = get_pos(); return ctokDIVassign;}
<C>"%="                 { cblval.tok = get_pos(); return ctokMODassign;}
<C>"+="                 { cblval.tok = get_pos(); return ctokPLUSassign;}
<C>"-="                 { cblval.tok = get_pos(); return ctokMINUSassign;}
<C>"<<="                { cblval.tok = get_pos(); return ctokLSassign;}
<C>">>="                { cblval.tok = get_pos(); return ctokRSassign;}
<C>"&="                 { cblval.tok = get_pos(); return ctokANDassign;}
<C>"^="                 { cblval.tok = get_pos(); return ctokERassign;}
<C>"|="                 { cblval.tok = get_pos(); return ctokORassign;}
<C>"..."                { cblval.tok = get_pos(); return ctokELLIPSIS;}

<C>.                    { char tmp[20];
                          CBZ::char_to_text(tmp, *cbtext);
                          CBZ::SyntaxError(string("Unexpected character: ") + string(tmp));
                        }

	/* -- Embedded meta-tokens ------------------------------------------------------------ */

<META>"expr"        { annlval.t_tok = (annlineno - line_number_offset); return metatokEXPR; }
<META>"stmt"        { annlval.t_tok = (annlineno - line_number_offset); return metatokSTMT; }
<META>"type"        { annlval.t_tok = (annlineno - line_number_offset); return metatokTYPE; }

<META>{identifier}     { annlval.t_id = new parserID(anntext, (annlineno - line_number_offset));
                         return metatokID;
                       }

<META>.                { annlval.t_tok = (annlineno - line_number_offset); return anntext[0]; }

%%

// --- Adapted from parse_line_directive in lex.l

void adjust_line_number(char * s)
{
 char *p;
  int val;

  // Read until a '#' sign
  for (p = s; *p != '\n' && *p != '#'; p++);
  assert(*p != '\n');

  // Read spaces and tabs
  for (p = p + 1; *p != '\n' && (*p == ' ' || *p == '\t'); p++);
  assert(*p != '\n');

  // Swallow "line" and any trailing whitespace
  if (strncmp(p, "line", 4) == 0) {
    p = p + 4;

    // There must be whitespace
    assert(*p == ' ' || *p == '\t');

    for (     ; *p != '\n' && (*p == ' ' || *p == '\t'); p++);
    assert(*p != '\n');
  }

  // Read an integer from the string
  if (isdigit(*p))
    for (val = 0; *p && isdigit(*p); p++)
      val = val * 10 + (*p - '0');
  else {
    CBZ::SyntaxError(string("#line error: expecting line number"));
    return;
  }

  line_number_offset = annlineno - val;
}
