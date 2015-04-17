%{

#include "c_breeze.h"
#include "meta.h"
#include "parser.tab.h"

exprNode *char_constant(const char *);
exprNode *int_constant(const char *, bool decimal, bool octal);
exprNode *wide_char_const(const char *);
exprNode *float_constant(const char *s);
char * convert_escape_sequences(char *string);
void parse_line_directive(char *s);
void save_pragma_directive(char *s);

void EnterC(void);
void ExitC(void);

#define MAX_FILENAME 400

char * lexer_line_start;

#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) \
    if (fgets(buf, max_size, cbin)==0) { result=0; } \
    else { lexer_line_start = (char *)buf; result = strlen(buf); }

#undef cbwrap

%}

%s C

identifier ([a-zA-Z_][0-9a-zA-Z_]*)

exponent_part ([eE][-+]?[0-9]+)
fractional_constant ([0-9]*"."[0-9]+)|([0-9]+".")
floating_constant ((({fractional_constant}{exponent_part}?)|([0-9]+{exponent_part}))[FfLl]?)

integer_suffix_opt ([uU]?[lL]?[lL]?)|([lL][uU][lL]?)|([lL][lL][uU])
decimal_constant ("0"{integer_suffix_opt}|[1-9][0-9]*{integer_suffix_opt})
octal_constant ("0"[0-7]*{integer_suffix_opt})
hex_constant ("0"[xX][0-9a-fA-F]+{integer_suffix_opt})

simple_escape ([abfnrtv'"?\\])

/* Escape sequences: These two escape sequences seem completely weird. The
second component of the regexp is eating up the entire rest of the
string. I just fixed a freaky bug caused by the fact that the original
version didn't have double quote!
SZG.
*/

octal_escape  ([0-7][^'"\n]*)
hex_escape ("x"[^'"\n]*)


bad_escape ([\\][^abfnrtv'"?\\x0-7\n ])

escape_sequence ([\\]({simple_escape}|{octal_escape}|{hex_escape}))
c_char ([^'\\\n]|{escape_sequence})
s_char ([^"\\\n]|{escape_sequence})

any_white ([ \011\013\014\015])
hwhite ([ \011])

%%

	BEGIN(C);

<C>{any_white}+     {  }

<C>"\n"             {  CBZ::current_line++; }

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

<C>"$expr"              { cblval.tok = get_pos(); return ctokMETA_TYPE_EXPR; }
<C>"$stmt"              { cblval.tok = get_pos(); return ctokMETA_TYPE_STMT; }
<C>"$"{identifier}      { if (metaexprNode::is_meta_expr(cbtext+1)) {
                            cblval.tok = get_pos();
                            cblval.idN = new idNode(cbtext, cblval.tok);
                            return ctokMETA_EXPR;
                          }
                          else if(metastmtNode::is_meta_stmt(cbtext+1)) {
			    cblval.tok = get_pos();
                            cblval.idN = new idNode(cbtext, cblval.tok);
                            return ctokMETA_STMT;
		          }
                                   
                        }

<C>.                    { char tmp[20];
                          CBZ::char_to_text(tmp, *cbtext);
                          CBZ::SyntaxError(string("Unexpected character: ") + string(tmp));
                        }

%%

/*  I won't bother to provide any error recovery. I won't even handle
unknown characters */

/*******************************************************************/


int hex_digit(char in)
{
    if (in >= '0' && in <= '9') return(in - '0');
    if (in >= 'a' && in <= 'f') return(in - 'a' + 10);
    if (in >= 'A' && in <= 'F') return(in - 'A' + 10);
    return -1;
}

int octal_digit(char in)
{
    if (in >= '0' && in <= '7') return(in - '0');
    return -1;
}

int parse_char_constant(const char * s, int * actual_read)
{
    assert(s[0] == '\\');
    *actual_read = 2;

    switch (s[1]) {
      case 'n': return '\n';
      case 't': return '\t';
      case 'v': return '\v';
      case 'b': return '\b';
      case 'r': return '\r';
      case 'f': return '\f';
      case 'a': return '\a';
      case '\\': return '\\';
      case '?': return '\?';
      case '\'': return '\'';
      case '\"': return '\"';
      case 'x': {
	  int val=0, tmp, i;

	  for (i=2; (tmp = hex_digit(s[i])) >= 0; i++) {
	      val = 16 * val + tmp;
	  }
	  *actual_read = i;
	  if (i==2) {
	      CBZ::SyntaxError(string("expecting hex digits after \\x"));
	      return(-1);
	  }
	  if (val > basic_type::TARGET_MAX_UCHAR) {
	      CBZ::SyntaxError(string("explicit hex character (\\x) out of range"));
	      return(-1);
	  }
	  return(val);
	  break;
      }
      default:
	if (octal_digit(s[1]) >= 0) {  /* octal constant */
	    int val=0, tmp, i;
	
	    for (i=1; (tmp = octal_digit(s[i])) >= 0  && i<4; i++) {
		val = 8 * val + tmp;
	    }
	    if (val > basic_type::TARGET_MAX_UCHAR) {
		CBZ::SyntaxError(string("explicit octal character out of range"));
		return(-1);
	    }
	    *actual_read = i;
	    return(val);
	} else {
	    CBZ::SyntaxError(string("unknown special character \"\\") + string(&s[1]) + string("\""));
	    return(-1);
	}
    }
    /* unreachable */
}

exprNode *char_constant(const char *s)
{
  int read, value;

  assert(s[0] == '\'');
  if (s[1] == '\\') {
    value = parse_char_constant(&s[1], &read);
    if (value < 0) value = 'X';
	
    if (s[1+read] != '\'')
      CBZ::Warning(1,
		   string("ignoring extra characters at end of char constant ") + string(s));
  } else {
    value = s[1];
    if (s[2] != '\'')
      CBZ::Warning(1,
		   string("ignoring extra characters at end of char constant ") + string(s));
  }

  cblval.tok = get_pos();

  return new constNode(constant((char)value), s, Coord(cblval.tok));
}

exprNode *wide_char_const(const char *s)
{
  assert(s[0]=='L' && s[1]== '\'');
  return(char_constant(&s[1]));
}


char *convert_escape_sequences(char *string)
{
  char *read, *write;
  int actual_read, val;

  write = read = string;

  while (*read != 0) {
    if (*read == '\\') {
      val = parse_char_constant(read, &actual_read);
      if (val < 0) {
	*write++ = 'X';
      } else {
	*write++ = (char) val;
      }
      read += actual_read;
    } else {
      *write++ = *read++;
    }
  }
  *write = 0;

  return(string);
}

#ifdef NO_STRTOUL
unsigned long strtoul(const char *c,char **p,int base) {
  return strtol(c,p,base);
}
#endif


exprNode * int_constant(const char *s, bool decimal, bool octal)
{
  exprNode * c;
  char ch;
  const char *p;
  bool is_unsigned;
  int longness;  /* 0 == not, 1==long, 2==long long */
  unsigned long value;

  longness = 0;
  is_unsigned = !decimal;

  p = &s[strlen(s) - 1];  /* address of last char in string */
  while (!isxdigit(*p)) {
    ch = toupper(*p);
    if (ch == 'U') {
      is_unsigned = true;
    } else if (ch == 'L') {
      ++longness;
    }
    p--;
  }

  cblval.tok = get_pos();

  // the following breaks if TARGET_ULONG is 'long long'
  value = (unsigned long) strtoul(s, NULL, 0);

  if (longness==0 && !is_unsigned && value <= basic_type::TARGET_MAX_INT)
    c = new constNode(constant((int)value), s, Coord(cblval.tok));
  else
    if (longness==0 && value <= basic_type::TARGET_MAX_UINT)
      c = new constNode(constant((unsigned int) value), s, Coord(cblval.tok));
    else
      if (longness==1 && !is_unsigned && value <= basic_type::TARGET_MAX_LONG)
	c = new constNode(constant((long)value), s, Coord(cblval.tok));
      else
	if (longness==1) {
	  if (value > basic_type::TARGET_MAX_ULONG) {
	    CBZ::SyntaxError(string("Constant `") + s + string("' is out of range"));
	  }
	  c = new constNode(constant((unsigned long) value), s, Coord(cblval.tok));
	} else {
	  assert(longness==2);

	  if (CBZ::ANSIOnly)
	    CBZ::SyntaxError(Coord(cblval.tok), "long long constants not allowed with -ansi switch");

	  // store constant value as unsigned long, but cast it to
	  //   appropriate long long type

	  c = new castNode((typeNode *) (is_unsigned ?
			    new primNode(basic_type::ULongLong) : new primNode(basic_type::SLongLong)),
			    new constNode(constant((unsigned long) value), s,
					  Coord(cblval.tok)),
			    true, Coord(cblval.tok));
	}

  return(c);
}


exprNode *float_constant(const char *s)
{
  exprNode * c;

  char ch; const char *p;
  bool is_long = false, is_float = false;
  double value;

  p = &s[strlen(s) - 1];  /* address of last char in string */
  while (!isdigit(*p)) {
    ch = toupper(*p);
    if (ch == 'F') {
      is_float = true;
    } else if (ch == 'L') {
      is_long = true;
    }
    p--;
  }

  cblval.tok = get_pos();

  value = atof(s);

  if (is_float)
      c = new constNode(constant((float) value), s, Coord(cblval.tok));
  else
    if (is_long)
      // we don't directly represent long double constants -- store it
      // as a double constant implicitly cast to long double

      // SZG: Dangerous cast...

      c = new castNode((typeNode *) new primNode(basic_type::LongDouble),
		       new constNode(constant((double) value), s, Coord(cblval.tok)),
		       true, Coord(cblval.tok));
    else
      c = new constNode(constant((double) value), s, Coord(cblval.tok));

  return(c);
}

// List *pragmas = NULL;


//   Parse a traditional or ANSI-C line number specification.
//
//   ANSI-C:   # line <n> \n
//             # line <n> "<filename>" \n
//   K&R       # <n> "<filename>" \n

void parse_line_directive(char *s)
{ char *p;
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
    CBZ::current_line++;
    return;
  }

  // Set this to true when the line directive starts a new file...
  bool new_file = false;

  // Skip whitespace
  for (     ; *p != '\n' && (*p == ' ' || *p == '\t'); p++);

  // Should be pointing at a '"' to start the filename or end of line
  if (*p != '\n' && *p != '\"') {
    CBZ::SyntaxError(string("#line error: expecting a filename string"));
    CBZ::current_line++;
    return;
  }
  else if (*p == '\"') {
    char fname[MAX_FILENAME + 1];
    int  f;

    for (f = 0, p++; *p != '\n' && *p != '\"' && f < MAX_FILENAME; p++, f++)
      fname[f] = *p;
    fname[f] = '\0';

    if (*p != '"') {
      CBZ::SyntaxError(string("#line error: Filename string missing close `\"'"));
      CBZ::current_line++;
      return;
    }
    else
      if (f == MAX_FILENAME)
	CBZ::Warning(1, string("#line error: filename truncated to 400 characters"));

    // Skip white space again
    for (p++; *p != '\n' && (*p == ' ' || *p == '\t'); p++);

    // Look at the number after the file name...
    if (*p == '1' || *p == '\n')
      new_file = true;

    if (! CBZ::IgnoreLineDirectives &&
        ! CBZ::is_file_included() &&
	(CBZ::current_unit->input_file() != fname)) {

      // SetFile will change file mode to included.  Generate an include
      //   that includes this new file.

      char line[MAX_FILENAME + 13];

      sprintf(line,"#include \"%s\"", fname);

      CBZ::pragmas.push_back(new textNode(line, true, Coord(get_pos())));
    }

    // --- Switch to the file specified

    if (! CBZ::IgnoreLineDirectives)
      CBZ::set_file(fname, val, new_file);
  }

  if (CBZ::IgnoreLineDirectives)
    CBZ::current_line++;
  else
    CBZ::current_line = val;
}

// Save pragma directive for later collection into source tree.

void save_pragma_directive(char *s)
{
   if (CBZ::is_file_included())
     return;

   CBZ::pragmas.push_back(new textNode(s, true, Coord(get_pos())));
}

int cbwrap()
{
   return 1;
}

void EnterC()
{
   BEGIN(C);
}

void ExitC()
{
   BEGIN(INITIAL);
}


