/* A Bison parser, made from language.y, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse annparse
#define yylex   annlex
#define yyerror annerror
#define yylval  annlval
#define yychar  annchar
#define yydebug anndebug
#define yynerrs annnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tokID = 258,
     tokCONSTANT = 259,
     tokENABLE = 260,
     tokDISABLE = 261,
     tokMAYPROPERTY = 262,
     tokMUSTPROPERTY = 263,
     tokGLOBAL = 264,
     tokINITIALLY = 265,
     tokPROCEDURE = 266,
     tokACCESS = 267,
     tokMODIFY = 268,
     tokANALYZE = 269,
     tokDEFAULT = 270,
     tokIF = 271,
     tokIS_EXACTLY = 272,
     tokIS_ATLEAST = 273,
     tokCOULD_BE = 274,
     tokIS_ATMOST = 275,
     tokIS_BOTTOM = 276,
     tokUNION_SET = 277,
     tokINTERSECT_SET = 278,
     tokUNION_EQUIV = 279,
     tokINTERSECT_EQUIV = 280,
     tokIS_ELEMENT_OF = 281,
     tokIS_EMPTYSET = 282,
     tokADD = 283,
     tokALWAYS = 284,
     tokEVER = 285,
     tokBEFORE = 286,
     tokAFTER = 287,
     tokTRACE = 288,
     tokCONFIDENCE = 289,
     tokFORWARD = 290,
     tokBACKWARD = 291,
     tokIS_ALIASOF = 292,
     tokIS_SAMEAS = 293,
     tokIS_EMPTY = 294,
     tokON_ENTRY = 295,
     tokON_EXIT = 296,
     tokNEW = 297,
     tokARROW = 298,
     tokDELETE = 299,
     tokPATTERN = 300,
     tokANDAND = 301,
     tokOROR = 302,
     tokNOT_EQUALS = 303,
     tokEQUALS_EQUALS = 304,
     tokLESS_EQUALS = 305,
     tokGREATER_EQUALS = 306,
     tokIS_CONSTANT = 307,
     tokASSIGN = 308,
     tokWEAKASSIGN = 309,
     tokEQUALS = 310,
     tokREPORT = 311,
     tokERROR = 312,
     tokAPPEND = 313,
     tokWHEN = 314,
     tokREPLACE = 315,
     tokINLINE = 316,
     tokIO = 317,
     tokSTRING = 318,
     WEAK_REDUCE = 319,
     LOWER_THAN_EXPRESSION = 320,
     LOWER_THAN_UNARY = 321,
     LOWER_THAN_POSTFIX = 322,
     PARENTHESIZED = 323,
     ctokAND = 324,
     ctokSTAR = 325,
     ctokPLUS = 326,
     ctokMINUS = 327,
     ctokTILDE = 328,
     ctokNOT = 329,
     ctokLESS = 330,
     ctokGREATER = 331,
     ctokOR = 332,
     ctokHAT = 333,
     ctokPERCENT = 334,
     ctokDIVIDE = 335,
     ctokLEFTPAREN = 336,
     ctokRIGHTPAREN = 337,
     ctokDOT = 338,
     ctokQUESTION = 339,
     ctokSEMICOLON = 340,
     ctokLEFTBRACE = 341,
     ctokRIGHTBRACE = 342,
     ctokCOMMA = 343,
     ctokLEFTBRACKET = 344,
     ctokRIGHTBRACKET = 345,
     ctokCOLON = 346,
     ctokAUTO = 347,
     ctokDOUBLE = 348,
     ctokINT = 349,
     ctokSTRUCT = 350,
     ctokBREAK = 351,
     ctokELSE = 352,
     ctokLONG = 353,
     ctokSWITCH = 354,
     ctokCASE = 355,
     ctokENUM = 356,
     ctokREGISTER = 357,
     ctokTYPEDEF = 358,
     ctokCHAR = 359,
     ctokEXTERN = 360,
     ctokRETURN = 361,
     ctokUNION = 362,
     ctokCONST = 363,
     ctokFLOAT = 364,
     ctokSHORT = 365,
     ctokUNSIGNED = 366,
     ctokCONTINUE = 367,
     ctokFOR = 368,
     ctokSIGNED = 369,
     ctokVOID = 370,
     ctokDEFAULT = 371,
     ctokGOTO = 372,
     ctokSIZEOF = 373,
     ctokVOLATILE = 374,
     ctokDO = 375,
     ctokIF = 376,
     ctokSTATIC = 377,
     ctokWHILE = 378,
     ctokUPLUS = 379,
     ctokUMINUS = 380,
     ctokINDIR = 381,
     ctokADDRESS = 382,
     ctokPOSTINC = 383,
     ctokPOSTDEC = 384,
     ctokPREINC = 385,
     ctokPREDEC = 386,
     ctokBOGUS = 387,
     IDENTIFIER = 388,
     STRINGliteral = 389,
     FLOATINGconstant = 390,
     INTEGERconstant = 391,
     OCTALconstant = 392,
     HEXconstant = 393,
     WIDECHARconstant = 394,
     CHARACTERconstant = 395,
     TYPEDEFname = 396,
     ctokARROW = 397,
     ctokICR = 398,
     ctokDECR = 399,
     ctokLS = 400,
     ctokRS = 401,
     ctokLE = 402,
     ctokGE = 403,
     ctokEQ = 404,
     ctokNE = 405,
     ctokANDAND = 406,
     ctokOROR = 407,
     ctokELLIPSIS = 408,
     ctokEQUALS = 409,
     ctokMULTassign = 410,
     ctokDIVassign = 411,
     ctokMODassign = 412,
     ctokPLUSassign = 413,
     ctokMINUSassign = 414,
     ctokLSassign = 415,
     ctokRSassign = 416,
     ctokANDassign = 417,
     ctokERassign = 418,
     ctokORassign = 419,
     ctokINLINE = 420,
     ctokATTRIBUTE = 421,
     ctokMETA_TYPE_EXPR = 422,
     ctokMETA_TYPE_STMT = 423,
     ctokMETA_EXPR = 424,
     ctokMETA_STMT = 425,
     metatokID = 426,
     metatokEXPR = 427,
     metatokSTMT = 428,
     metatokTYPE = 429
   };
#endif
#define tokID 258
#define tokCONSTANT 259
#define tokENABLE 260
#define tokDISABLE 261
#define tokMAYPROPERTY 262
#define tokMUSTPROPERTY 263
#define tokGLOBAL 264
#define tokINITIALLY 265
#define tokPROCEDURE 266
#define tokACCESS 267
#define tokMODIFY 268
#define tokANALYZE 269
#define tokDEFAULT 270
#define tokIF 271
#define tokIS_EXACTLY 272
#define tokIS_ATLEAST 273
#define tokCOULD_BE 274
#define tokIS_ATMOST 275
#define tokIS_BOTTOM 276
#define tokUNION_SET 277
#define tokINTERSECT_SET 278
#define tokUNION_EQUIV 279
#define tokINTERSECT_EQUIV 280
#define tokIS_ELEMENT_OF 281
#define tokIS_EMPTYSET 282
#define tokADD 283
#define tokALWAYS 284
#define tokEVER 285
#define tokBEFORE 286
#define tokAFTER 287
#define tokTRACE 288
#define tokCONFIDENCE 289
#define tokFORWARD 290
#define tokBACKWARD 291
#define tokIS_ALIASOF 292
#define tokIS_SAMEAS 293
#define tokIS_EMPTY 294
#define tokON_ENTRY 295
#define tokON_EXIT 296
#define tokNEW 297
#define tokARROW 298
#define tokDELETE 299
#define tokPATTERN 300
#define tokANDAND 301
#define tokOROR 302
#define tokNOT_EQUALS 303
#define tokEQUALS_EQUALS 304
#define tokLESS_EQUALS 305
#define tokGREATER_EQUALS 306
#define tokIS_CONSTANT 307
#define tokASSIGN 308
#define tokWEAKASSIGN 309
#define tokEQUALS 310
#define tokREPORT 311
#define tokERROR 312
#define tokAPPEND 313
#define tokWHEN 314
#define tokREPLACE 315
#define tokINLINE 316
#define tokIO 317
#define tokSTRING 318
#define WEAK_REDUCE 319
#define LOWER_THAN_EXPRESSION 320
#define LOWER_THAN_UNARY 321
#define LOWER_THAN_POSTFIX 322
#define PARENTHESIZED 323
#define ctokAND 324
#define ctokSTAR 325
#define ctokPLUS 326
#define ctokMINUS 327
#define ctokTILDE 328
#define ctokNOT 329
#define ctokLESS 330
#define ctokGREATER 331
#define ctokOR 332
#define ctokHAT 333
#define ctokPERCENT 334
#define ctokDIVIDE 335
#define ctokLEFTPAREN 336
#define ctokRIGHTPAREN 337
#define ctokDOT 338
#define ctokQUESTION 339
#define ctokSEMICOLON 340
#define ctokLEFTBRACE 341
#define ctokRIGHTBRACE 342
#define ctokCOMMA 343
#define ctokLEFTBRACKET 344
#define ctokRIGHTBRACKET 345
#define ctokCOLON 346
#define ctokAUTO 347
#define ctokDOUBLE 348
#define ctokINT 349
#define ctokSTRUCT 350
#define ctokBREAK 351
#define ctokELSE 352
#define ctokLONG 353
#define ctokSWITCH 354
#define ctokCASE 355
#define ctokENUM 356
#define ctokREGISTER 357
#define ctokTYPEDEF 358
#define ctokCHAR 359
#define ctokEXTERN 360
#define ctokRETURN 361
#define ctokUNION 362
#define ctokCONST 363
#define ctokFLOAT 364
#define ctokSHORT 365
#define ctokUNSIGNED 366
#define ctokCONTINUE 367
#define ctokFOR 368
#define ctokSIGNED 369
#define ctokVOID 370
#define ctokDEFAULT 371
#define ctokGOTO 372
#define ctokSIZEOF 373
#define ctokVOLATILE 374
#define ctokDO 375
#define ctokIF 376
#define ctokSTATIC 377
#define ctokWHILE 378
#define ctokUPLUS 379
#define ctokUMINUS 380
#define ctokINDIR 381
#define ctokADDRESS 382
#define ctokPOSTINC 383
#define ctokPOSTDEC 384
#define ctokPREINC 385
#define ctokPREDEC 386
#define ctokBOGUS 387
#define IDENTIFIER 388
#define STRINGliteral 389
#define FLOATINGconstant 390
#define INTEGERconstant 391
#define OCTALconstant 392
#define HEXconstant 393
#define WIDECHARconstant 394
#define CHARACTERconstant 395
#define TYPEDEFname 396
#define ctokARROW 397
#define ctokICR 398
#define ctokDECR 399
#define ctokLS 400
#define ctokRS 401
#define ctokLE 402
#define ctokGE 403
#define ctokEQ 404
#define ctokNE 405
#define ctokANDAND 406
#define ctokOROR 407
#define ctokELLIPSIS 408
#define ctokEQUALS 409
#define ctokMULTassign 410
#define ctokDIVassign 411
#define ctokMODassign 412
#define ctokPLUSassign 413
#define ctokMINUSassign 414
#define ctokLSassign 415
#define ctokRSassign 416
#define ctokANDassign 417
#define ctokERassign 418
#define ctokORassign 419
#define ctokINLINE 420
#define ctokATTRIBUTE 421
#define ctokMETA_TYPE_EXPR 422
#define ctokMETA_TYPE_STMT 423
#define ctokMETA_EXPR 424
#define ctokMETA_STMT 425
#define metatokID 426
#define metatokEXPR 427
#define metatokSTMT 428
#define metatokTYPE 429




/* Copy the first part of user declarations.  */
#line 1 "language.y"


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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#ifndef YYSTYPE
#line 29 "language.y"
typedef union {

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
} yystype;
/* Line 193 of /lusr/gnu/share/bison/yacc.c.  */
#line 537 "language.tab.c"
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */


/* Line 213 of /lusr/gnu/share/bison/yacc.c.  */
#line 558 "language.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];	\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  80
#define YYLAST   2895

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  199
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  162
/* YYNRULES -- Number of rules. */
#define YYNRULES  505
/* YYNRULES -- Number of states. */
#define YYNSTATES  879

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   429

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    68,     2,     2,    82,    78,    77,     2,
      72,    73,    76,    79,    66,    65,    81,    69,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    67,
     197,     2,   198,     2,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   195,     2,   196,    70,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,    80,    75,    71,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    15,    18,    20,
      22,    24,    26,    28,    33,    36,    43,    50,    55,    60,
      65,    70,    71,    73,    75,    76,    79,    83,    85,    89,
      91,    94,    99,   103,   109,   114,   116,   119,   121,   123,
     125,   127,   129,   134,   139,   144,   148,   152,   154,   157,
     165,   171,   176,   179,   181,   184,   188,   191,   195,   200,
     202,   206,   211,   216,   221,   224,   229,   232,   238,   244,
     246,   249,   257,   263,   268,   271,   273,   277,   281,   284,
     288,   294,   298,   305,   310,   314,   318,   321,   323,   326,
     330,   334,   337,   339,   342,   346,   350,   354,   357,   361,
     363,   365,   367,   369,   371,   373,   375,   377,   379,   381,
     382,   386,   394,   398,   406,   408,   412,   414,   419,   422,
     425,   429,   433,   440,   447,   449,   451,   455,   457,   460,
     464,   468,   472,   476,   480,   484,   486,   488,   492,   496,
     500,   504,   508,   512,   516,   520,   524,   527,   530,   533,
     535,   537,   539,   543,   549,   556,   558,   560,   565,   569,
     574,   578,   582,   585,   588,   592,   596,   598,   602,   604,
     607,   610,   613,   616,   621,   623,   625,   627,   629,   631,
     633,   635,   640,   642,   646,   650,   654,   656,   660,   664,
     666,   670,   674,   676,   680,   684,   688,   692,   694,   698,
     702,   704,   708,   710,   714,   716,   720,   722,   726,   728,
     732,   734,   740,   742,   746,   748,   750,   752,   754,   756,
     758,   760,   762,   764,   766,   768,   770,   774,   776,   777,
     779,   782,   785,   788,   791,   792,   793,   800,   801,   802,
     809,   810,   811,   819,   820,   826,   827,   833,   837,   838,
     839,   846,   847,   848,   855,   856,   857,   865,   866,   872,
     873,   879,   883,   885,   887,   889,   892,   895,   898,   901,
     904,   907,   910,   913,   916,   919,   921,   924,   927,   929,
     931,   933,   935,   937,   939,   941,   944,   947,   950,   952,
     955,   958,   960,   963,   966,   968,   971,   973,   976,   978,
     980,   982,   984,   986,   988,   990,   993,   998,  1004,  1008,
    1012,  1017,  1022,  1024,  1028,  1030,  1033,  1035,  1037,  1040,
    1044,  1048,  1053,  1055,  1057,  1059,  1061,  1064,  1067,  1071,
    1075,  1079,  1083,  1088,  1090,  1093,  1097,  1100,  1104,  1109,
    1113,  1115,  1117,  1119,  1122,  1126,  1129,  1133,  1138,  1140,
    1144,  1146,  1149,  1153,  1158,  1162,  1167,  1169,  1173,  1175,
    1177,  1179,  1182,  1184,  1187,  1188,  1190,  1192,  1195,  1202,
    1204,  1208,  1209,  1211,  1216,  1218,  1220,  1222,  1223,  1226,
    1230,  1235,  1237,  1239,  1243,  1245,  1249,  1251,  1253,  1257,
    1261,  1265,  1267,  1270,  1273,  1276,  1278,  1281,  1284,  1286,
    1289,  1292,  1295,  1297,  1300,  1303,  1308,  1314,  1317,  1321,
    1326,  1328,  1330,  1332,  1335,  1338,  1341,  1344,  1348,  1351,
    1355,  1359,  1362,  1366,  1369,  1370,  1372,  1375,  1381,  1388,
    1391,  1394,  1399,  1400,  1403,  1404,  1406,  1408,  1410,  1412,
    1414,  1416,  1418,  1421,  1424,  1428,  1433,  1437,  1441,  1444,
    1448,  1452,  1457,  1459,  1461,  1463,  1466,  1468,  1471,  1474,
    1480,  1488,  1494,  1500,  1508,  1518,  1528,  1538,  1548,  1554,
    1558,  1561,  1564,  1568,  1572,  1574,  1576,  1578,  1580,  1582,
    1584,  1587,  1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,
    1605,  1607,  1609,  1611,  1613,  1615,  1617,  1619,  1621,  1623,
    1625,  1627,  1629,  1631,  1636,  1641
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     200,     0,    -1,   201,    -1,   200,   201,    -1,   203,    -1,
     204,    -1,     5,     3,    -1,     6,     3,    -1,   210,    -1,
     233,    -1,   202,    -1,   220,    -1,   344,    -1,     9,    74,
     217,    75,    -1,     9,   218,    -1,     7,     3,    64,   205,
     207,   206,    -1,     8,     3,    64,   205,   207,   206,    -1,
       7,     3,    64,    22,    -1,     8,     3,    64,    23,    -1,
       7,     3,    64,    24,    -1,     8,     3,    64,    25,    -1,
      -1,    35,    -1,    36,    -1,    -1,    10,     3,    -1,    74,
     208,    75,    -1,   209,    -1,   208,    66,   209,    -1,     3,
      -1,     3,   207,    -1,   211,    74,   212,    75,    -1,   211,
      74,    75,    -1,    11,     3,    72,   234,    73,    -1,    11,
       3,    72,    73,    -1,   213,    -1,   212,   213,    -1,   214,
      -1,   219,    -1,   220,    -1,   229,    -1,   232,    -1,    40,
      74,   217,    75,    -1,    41,    74,   217,    75,    -1,    41,
      74,   215,    75,    -1,    40,    74,    75,    -1,    41,    74,
      75,    -1,   216,    -1,   215,   216,    -1,    16,    72,   223,
      73,    74,   217,    75,    -1,    16,    72,   223,    73,   218,
      -1,    15,    74,   217,    75,    -1,    15,   218,    -1,   218,
      -1,   217,   218,    -1,   217,    66,   218,    -1,    44,   235,
      -1,   217,    44,   235,    -1,   217,    66,    44,   235,    -1,
     235,    -1,     3,    43,   218,    -1,     3,    43,    42,   218,
      -1,     3,    74,   217,    75,    -1,    12,    74,   234,    75,
      -1,    12,     3,    -1,    13,    74,   234,    75,    -1,    13,
       3,    -1,    14,     3,    74,   221,    75,    -1,    14,     3,
      74,   225,    75,    -1,   222,    -1,   221,   222,    -1,    16,
      72,   223,    73,    74,   225,    75,    -1,    16,    72,   223,
      73,   226,    -1,    15,    74,   225,    75,    -1,    15,   226,
      -1,   224,    -1,   223,    47,   223,    -1,   223,    46,   223,
      -1,    68,   223,    -1,    72,   223,    73,    -1,     3,    64,
       3,   228,    21,    -1,     3,   228,    21,    -1,     3,    64,
       3,   228,   227,     3,    -1,     3,   228,   227,     3,    -1,
       3,     3,     3,    -1,     3,    26,     3,    -1,     3,    27,
      -1,   236,    -1,     3,    52,    -1,     3,    37,     3,    -1,
       3,    38,     3,    -1,     3,    39,    -1,   226,    -1,   225,
     226,    -1,     3,    55,   237,    -1,     3,    53,     3,    -1,
       3,    54,     3,    -1,    28,     3,    -1,     3,     3,     3,
      -1,    17,    -1,    18,    -1,    19,    -1,    20,    -1,    31,
      -1,    32,    -1,    29,    -1,    30,    -1,    33,    -1,    34,
      -1,    -1,    56,   230,    67,    -1,    56,    16,    72,   223,
      73,   230,    67,    -1,    57,   230,    67,    -1,    57,    16,
      72,   223,    73,   230,    67,    -1,   231,    -1,   230,    58,
     231,    -1,    63,    -1,     3,    64,     3,   228,    -1,   237,
     228,    -1,    83,     3,    -1,   195,     3,   196,    -1,    80,
       3,    80,    -1,    59,    72,   223,    73,    60,   339,    -1,
      59,    72,   223,    73,    61,    67,    -1,    45,    -1,   235,
      -1,   234,    66,   235,    -1,     3,    -1,    62,     3,    -1,
     237,    49,   237,    -1,   237,    48,   237,    -1,   237,   197,
     237,    -1,   237,    50,   237,    -1,   237,   198,   237,    -1,
     237,    51,   237,    -1,     4,    -1,     3,    -1,    72,   237,
      73,    -1,   237,    79,   237,    -1,   237,    65,   237,    -1,
     237,    80,   237,    -1,   237,    70,   237,    -1,   237,    77,
     237,    -1,   237,    76,   237,    -1,   237,    69,   237,    -1,
     237,    78,   237,    -1,    65,   237,    -1,    79,   237,    -1,
      71,   237,    -1,   356,    -1,   350,    -1,   351,    -1,   101,
     257,   102,    -1,   101,   342,   345,   343,   102,    -1,   101,
     342,   344,   345,   343,   102,    -1,   358,    -1,   238,    -1,
     239,   109,   257,   110,    -1,   239,   101,   102,    -1,   239,
     101,   240,   102,    -1,   239,   103,   356,    -1,   239,   162,
     356,    -1,   239,   163,    -1,   239,   164,    -1,   239,   103,
     357,    -1,   239,   162,   357,    -1,   255,    -1,   240,   108,
     255,    -1,   239,    -1,   163,   241,    -1,   164,   241,    -1,
     242,   243,    -1,   138,   241,    -1,   138,   101,   312,   102,
      -1,    89,    -1,    90,    -1,    91,    -1,    92,    -1,    93,
      -1,    94,    -1,   241,    -1,   101,   312,   102,   243,    -1,
     243,    -1,   244,    90,   243,    -1,   244,   100,   243,    -1,
     244,    99,   243,    -1,   244,    -1,   245,    91,   244,    -1,
     245,    92,   244,    -1,   245,    -1,   246,   165,   245,    -1,
     246,   166,   245,    -1,   246,    -1,   247,    95,   246,    -1,
     247,    96,   246,    -1,   247,   167,   246,    -1,   247,   168,
     246,    -1,   247,    -1,   248,   169,   247,    -1,   248,   170,
     247,    -1,   248,    -1,   249,    89,   248,    -1,   249,    -1,
     250,    98,   249,    -1,   250,    -1,   251,    97,   250,    -1,
     251,    -1,   252,   171,   251,    -1,   252,    -1,   253,   172,
     252,    -1,   253,    -1,   253,   104,   257,   111,   254,    -1,
     254,    -1,   241,   256,   255,    -1,   174,    -1,   175,    -1,
     176,    -1,   177,    -1,   178,    -1,   179,    -1,   180,    -1,
     181,    -1,   182,    -1,   183,    -1,   184,    -1,   255,    -1,
     257,   108,   255,    -1,   254,    -1,    -1,   257,    -1,   261,
     105,    -1,   270,   105,    -1,   281,   105,    -1,   287,   105,
      -1,    -1,    -1,   279,   292,   262,   313,   263,   319,    -1,
      -1,    -1,   285,   292,   264,   313,   265,   319,    -1,    -1,
      -1,   261,   108,   292,   266,   313,   267,   319,    -1,    -1,
     279,     1,   268,   313,   319,    -1,    -1,   285,     1,   269,
     313,   319,    -1,   261,   108,     1,    -1,    -1,    -1,   283,
     304,   271,   313,   272,   319,    -1,    -1,    -1,   289,   304,
     273,   313,   274,   319,    -1,    -1,    -1,   270,   108,   304,
     275,   313,   276,   319,    -1,    -1,   283,     1,   277,   313,
     319,    -1,    -1,   289,     1,   278,   313,   319,    -1,   270,
     108,     1,    -1,   280,    -1,   281,    -1,   282,    -1,   286,
     354,    -1,   283,   355,    -1,   280,   284,    -1,   280,   355,
      -1,   287,   354,    -1,   283,   291,    -1,   281,   284,    -1,
     288,   354,    -1,   283,   357,    -1,   282,   284,    -1,   354,
      -1,   289,   354,    -1,   283,   284,    -1,   352,    -1,   354,
      -1,   286,    -1,   287,    -1,   288,    -1,   360,    -1,   355,
      -1,   289,   355,    -1,   286,   352,    -1,   286,   355,    -1,
     291,    -1,   289,   291,    -1,   287,   352,    -1,   357,    -1,
     289,   357,    -1,   288,   352,    -1,   352,    -1,   289,   352,
      -1,   353,    -1,   290,   353,    -1,   325,    -1,   335,    -1,
     293,    -1,   296,    -1,   304,    -1,   308,    -1,   294,    -1,
      90,   293,    -1,    90,   101,   295,   102,    -1,    90,   290,
     101,   295,   102,    -1,    90,   290,   293,    -1,   101,   293,
     102,    -1,   101,   295,   302,   102,    -1,   101,   293,   102,
     302,    -1,   357,    -1,   101,   295,   102,    -1,   357,    -1,
     357,   302,    -1,   297,    -1,   298,    -1,    90,   296,    -1,
      90,   290,   296,    -1,   101,   297,   102,    -1,   101,   297,
     102,   302,    -1,   300,    -1,   301,    -1,   302,    -1,    90,
      -1,    90,   290,    -1,    90,   299,    -1,    90,   290,   299,
      -1,   101,   300,   102,    -1,   101,   301,   102,    -1,   101,
     302,   102,    -1,   101,   300,   102,   302,    -1,   303,    -1,
     101,   102,    -1,   101,   322,   102,    -1,   109,   110,    -1,
     109,   258,   110,    -1,   303,   109,   258,   110,    -1,   303,
     109,   110,    -1,   305,    -1,   307,    -1,   306,    -1,    90,
     304,    -1,    90,   290,   304,    -1,   307,   302,    -1,   101,
     305,   102,    -1,   101,   305,   102,   302,    -1,   356,    -1,
     101,   307,   102,    -1,   309,    -1,    90,   308,    -1,    90,
     290,   308,    -1,   307,   101,   310,   102,    -1,   101,   308,
     102,    -1,   101,   308,   102,   302,    -1,   356,    -1,   310,
     108,   356,    -1,   356,    -1,   357,    -1,   285,    -1,   285,
     299,    -1,   289,    -1,   289,   299,    -1,    -1,   314,    -1,
     315,    -1,   314,   315,    -1,   186,   101,   101,   316,   102,
     102,    -1,   317,    -1,   316,   108,   317,    -1,    -1,   318,
      -1,   318,   101,   257,   102,    -1,   356,    -1,   357,    -1,
     128,    -1,    -1,   174,   320,    -1,   106,   321,   107,    -1,
     106,   321,   108,   107,    -1,   255,    -1,   320,    -1,   321,
     108,   320,    -1,   323,    -1,   323,   108,   173,    -1,   173,
      -1,   324,    -1,   323,   108,   324,    -1,   324,   174,   320,
      -1,   323,   108,     1,    -1,   279,    -1,   279,   299,    -1,
     279,   304,    -1,   279,   296,    -1,   283,    -1,   283,   299,
      -1,   283,   304,    -1,   285,    -1,   285,   299,    -1,   285,
     304,    -1,   285,   296,    -1,   289,    -1,   289,   299,    -1,
     289,   304,    -1,   326,   106,   327,   107,    -1,   326,   311,
     106,   327,   107,    -1,   326,   311,    -1,   326,   106,   107,
      -1,   326,   311,   106,   107,    -1,   115,    -1,   127,    -1,
     328,    -1,   327,   328,    -1,   330,   105,    -1,   329,   105,
      -1,   289,   332,    -1,   329,   108,   332,    -1,   285,   331,
      -1,   330,   108,   331,    -1,   292,   333,   313,    -1,   334,
     313,    -1,   304,   333,   313,    -1,   334,   313,    -1,    -1,
     334,    -1,   111,   258,    -1,   121,   106,   336,   338,   107,
      -1,   121,   311,   106,   336,   338,   107,    -1,   121,   311,
      -1,   311,   337,    -1,   336,   108,   311,   337,    -1,    -1,
     174,   258,    -1,    -1,   108,    -1,   340,    -1,   341,    -1,
     346,    -1,   347,    -1,   348,    -1,   349,    -1,     1,   105,
      -1,   359,    67,    -1,   356,   111,   339,    -1,   120,   258,
     111,   339,    -1,   136,   111,   339,    -1,   357,   111,   339,
      -1,   342,   343,    -1,   342,   344,   343,    -1,   342,   345,
     343,    -1,   342,   344,   345,   343,    -1,   106,    -1,   107,
      -1,   260,    -1,   344,   260,    -1,   339,    -1,   345,   339,
      -1,   259,   105,    -1,   141,   101,   257,   102,   339,    -1,
     141,   101,   257,   102,   339,   117,   339,    -1,   119,   101,
     257,   102,   339,    -1,   143,   101,   257,   102,   339,    -1,
     140,   339,   143,   101,   257,   102,   105,    -1,   133,   101,
     259,   105,   259,   105,   259,   102,   339,    -1,   133,   101,
       1,   105,   259,   105,   259,   102,   339,    -1,   133,   101,
     259,   105,   259,   105,     1,   102,   339,    -1,   133,   101,
     259,   105,     1,   105,   259,   102,   339,    -1,   133,   101,
       1,   102,   339,    -1,   137,   356,   105,    -1,   132,   105,
      -1,   116,   105,    -1,   126,   259,   105,    -1,   137,   357,
     105,    -1,   155,    -1,   156,    -1,   157,    -1,   158,    -1,
     160,    -1,   154,    -1,   351,   154,    -1,   128,    -1,   139,
      -1,   185,    -1,   128,    -1,   139,    -1,   123,    -1,   125,
      -1,   142,    -1,   112,    -1,   122,    -1,   135,    -1,   124,
      -1,   114,    -1,   129,    -1,   113,    -1,   134,    -1,   131,
      -1,   130,    -1,   118,    -1,   153,    -1,   161,    -1,   191,
      -1,   192,    72,   191,    73,    -1,   193,    72,   191,    73,
      -1,   194,    72,   191,    73,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   487,   487,   489,   492,   494,   495,   497,   499,   500,
     501,   502,   508,   515,   522,   532,   541,   549,   557,   565,
     573,   583,   588,   593,   599,   605,   611,   618,   625,   632,
     639,   649,   656,   663,   670,   677,   679,   682,   687,   691,
     696,   700,   707,   713,   718,   722,   726,   731,   733,   736,
     742,   749,   754,   763,   770,   776,   782,   794,   801,   809,
     817,   823,   829,   838,   846,   854,   860,   872,   879,   888,
     895,   902,   908,   915,   920,   929,   935,   940,   945,   950,
     956,   967,   974,   982,   991,   999,  1007,  1017,  1022,  1029,
    1036,  1043,  1051,  1058,  1065,  1073,  1082,  1093,  1101,  1111,
    1117,  1121,  1125,  1130,  1136,  1140,  1144,  1148,  1152,  1156,
    1163,  1170,  1174,  1179,  1185,  1193,  1201,  1209,  1219,  1226,
    1231,  1236,  1244,  1250,  1258,  1266,  1273,  1280,  1284,  1292,
    1297,  1300,  1303,  1306,  1309,  1314,  1318,  1323,  1326,  1329,
    1332,  1335,  1338,  1341,  1344,  1347,  1350,  1353,  1356,  1373,
    1380,  1381,  1383,  1389,  1395,  1401,  1404,  1408,  1420,  1423,
    1426,  1429,  1432,  1435,  1439,  1442,  1446,  1450,  1454,  1458,
    1461,  1464,  1468,  1471,  1475,  1477,  1478,  1479,  1480,  1481,
    1484,  1486,  1490,  1492,  1495,  1498,  1502,  1504,  1507,  1511,
    1513,  1516,  1520,  1522,  1525,  1528,  1531,  1535,  1537,  1540,
    1544,  1546,  1550,  1552,  1558,  1560,  1566,  1568,  1572,  1574,
    1580,  1582,  1586,  1588,  1594,  1598,  1601,  1604,  1607,  1610,
    1613,  1616,  1619,  1622,  1625,  1629,  1632,  1646,  1650,  1652,
    1687,  1691,  1695,  1701,  1710,  1712,  1708,  1719,  1721,  1718,
    1727,  1729,  1726,  1737,  1735,  1744,  1742,  1749,  1756,  1758,
    1754,  1764,  1766,  1763,  1772,  1774,  1771,  1782,  1780,  1789,
    1787,  1794,  1798,  1804,  1807,  1813,  1818,  1823,  1827,  1834,
    1839,  1844,  1851,  1856,  1861,  1868,  1870,  1873,  1878,  1880,
    1884,  1890,  1893,  1895,  1900,  1905,  1910,  1914,  1920,  1925,
    1930,  1937,  1942,  1947,  1953,  1955,  1959,  1961,  1966,  1971,
    1977,  1979,  1980,  1981,  1990,  1993,  1996,  1999,  2002,  2008,
    2012,  2015,  2020,  2025,  2030,  2035,  2038,  2046,  2049,  2052,
    2057,  2061,  2066,  2068,  2069,  2073,  2078,  2082,  2086,  2092,
    2097,  2101,  2105,  2111,  2116,  2120,  2126,  2131,  2135,  2140,
    2152,  2154,  2158,  2161,  2164,  2169,  2173,  2176,  2181,  2185,
    2190,  2197,  2200,  2205,  2211,  2214,  2228,  2238,  2249,  2251,
    2255,  2260,  2264,  2268,  2276,  2279,  2283,  2286,  2292,  2299,
    2302,  2306,  2309,  2311,  2316,  2318,  2319,  2323,  2325,  2329,
    2331,  2332,  2336,  2341,  2348,  2351,  2356,  2365,  2370,  2375,
    2382,  2387,  2391,  2395,  2398,  2401,  2404,  2408,  2411,  2414,
    2418,  2421,  2424,  2427,  2431,  2443,  2447,  2451,  2455,  2462,
    2471,  2473,  2477,  2479,  2486,  2488,  2493,  2498,  2504,  2509,
    2516,  2522,  2529,  2535,  2542,  2544,  2548,  2553,  2557,  2560,
    2565,  2570,  2576,  2578,  2581,  2583,  2592,  2596,  2599,  2602,
    2605,  2608,  2619,  2622,  2626,  2630,  2633,  2637,  2642,  2646,
    2649,  2652,  2656,  2660,  2683,  2687,  2695,  2700,  2709,  2714,
    2718,  2721,  2725,  2729,  2732,  2737,  2740,  2743,  2746,  2750,
    2754,  2757,  2760,  2764,  2922,  2924,  2925,  2926,  2927,  2931,
    2933,  2942,  2944,  2945,  2948,  2950,  2953,  2955,  2956,  2957,
    2958,  2961,  2963,  2964,  2965,  2966,  2968,  2969,  2971,  2972,
    2975,  2979,  2990,  2998,  3005,  3014
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tokID", "tokCONSTANT", "tokENABLE", 
  "tokDISABLE", "tokMAYPROPERTY", "tokMUSTPROPERTY", "tokGLOBAL", 
  "tokINITIALLY", "tokPROCEDURE", "tokACCESS", "tokMODIFY", "tokANALYZE", 
  "tokDEFAULT", "tokIF", "tokIS_EXACTLY", "tokIS_ATLEAST", "tokCOULD_BE", 
  "tokIS_ATMOST", "tokIS_BOTTOM", "tokUNION_SET", "tokINTERSECT_SET", 
  "tokUNION_EQUIV", "tokINTERSECT_EQUIV", "tokIS_ELEMENT_OF", 
  "tokIS_EMPTYSET", "tokADD", "tokALWAYS", "tokEVER", "tokBEFORE", 
  "tokAFTER", "tokTRACE", "tokCONFIDENCE", "tokFORWARD", "tokBACKWARD", 
  "tokIS_ALIASOF", "tokIS_SAMEAS", "tokIS_EMPTY", "tokON_ENTRY", 
  "tokON_EXIT", "tokNEW", "tokARROW", "tokDELETE", "tokPATTERN", 
  "tokANDAND", "tokOROR", "tokNOT_EQUALS", "tokEQUALS_EQUALS", 
  "tokLESS_EQUALS", "tokGREATER_EQUALS", "tokIS_CONSTANT", "tokASSIGN", 
  "tokWEAKASSIGN", "tokEQUALS", "tokREPORT", "tokERROR", "tokAPPEND", 
  "tokWHEN", "tokREPLACE", "tokINLINE", "tokIO", "tokSTRING", "':'", 
  "'-'", "','", "';'", "'!'", "'/'", "'^'", "'~'", "'('", "')'", "'{'", 
  "'}'", "'*'", "'&'", "'%'", "'+'", "'|'", "'.'", "'$'", "'@'", 
  "WEAK_REDUCE", "LOWER_THAN_EXPRESSION", "LOWER_THAN_UNARY", 
  "LOWER_THAN_POSTFIX", "PARENTHESIZED", "ctokAND", "ctokSTAR", 
  "ctokPLUS", "ctokMINUS", "ctokTILDE", "ctokNOT", "ctokLESS", 
  "ctokGREATER", "ctokOR", "ctokHAT", "ctokPERCENT", "ctokDIVIDE", 
  "ctokLEFTPAREN", "ctokRIGHTPAREN", "ctokDOT", "ctokQUESTION", 
  "ctokSEMICOLON", "ctokLEFTBRACE", "ctokRIGHTBRACE", "ctokCOMMA", 
  "ctokLEFTBRACKET", "ctokRIGHTBRACKET", "ctokCOLON", "ctokAUTO", 
  "ctokDOUBLE", "ctokINT", "ctokSTRUCT", "ctokBREAK", "ctokELSE", 
  "ctokLONG", "ctokSWITCH", "ctokCASE", "ctokENUM", "ctokREGISTER", 
  "ctokTYPEDEF", "ctokCHAR", "ctokEXTERN", "ctokRETURN", "ctokUNION", 
  "ctokCONST", "ctokFLOAT", "ctokSHORT", "ctokUNSIGNED", "ctokCONTINUE", 
  "ctokFOR", "ctokSIGNED", "ctokVOID", "ctokDEFAULT", "ctokGOTO", 
  "ctokSIZEOF", "ctokVOLATILE", "ctokDO", "ctokIF", "ctokSTATIC", 
  "ctokWHILE", "ctokUPLUS", "ctokUMINUS", "ctokINDIR", "ctokADDRESS", 
  "ctokPOSTINC", "ctokPOSTDEC", "ctokPREINC", "ctokPREDEC", "ctokBOGUS", 
  "IDENTIFIER", "STRINGliteral", "FLOATINGconstant", "INTEGERconstant", 
  "OCTALconstant", "HEXconstant", "WIDECHARconstant", "CHARACTERconstant", 
  "TYPEDEFname", "ctokARROW", "ctokICR", "ctokDECR", "ctokLS", "ctokRS", 
  "ctokLE", "ctokGE", "ctokEQ", "ctokNE", "ctokANDAND", "ctokOROR", 
  "ctokELLIPSIS", "ctokEQUALS", "ctokMULTassign", "ctokDIVassign", 
  "ctokMODassign", "ctokPLUSassign", "ctokMINUSassign", "ctokLSassign", 
  "ctokRSassign", "ctokANDassign", "ctokERassign", "ctokORassign", 
  "ctokINLINE", "ctokATTRIBUTE", "ctokMETA_TYPE_EXPR", 
  "ctokMETA_TYPE_STMT", "ctokMETA_EXPR", "ctokMETA_STMT", "metatokID", 
  "metatokEXPR", "metatokSTMT", "metatokTYPE", "'['", "']'", "'<'", "'>'", 
  "$accept", "annotation_file", "annotation", "header", "global", 
  "property", "optional_direction", "optional_default", 
  "user_defined_class", "members", "member", "procedure", 
  "procedure_declaration", "procedure_annotations", 
  "procedure_annotation", "structure_annotation", "pointer_rule_list", 
  "pointer_rule", "structures", "structure", "behavior_annotation", 
  "analyze_annotation", "rules", "rule", "condition", "test", "effects", 
  "effect", "binary_property_operator", "property_sensitivity", 
  "report_annotation", "report_element_list", "report_element", 
  "action_annotation", "pattern", "identifier_list", 
  "qualified_identifier", "c_compare_expression", "c_expression", 
  "primary.expression", "postfix.expression", "argument.expression.list", 
  "unary.expression", "unary.operator", "cast.expression", 
  "multiplicative.expression", "additive.expression", "shift.expression", 
  "relational.expression", "equality.expression", "AND.expression", 
  "exclusive.OR.expression", "inclusive.OR.expression", 
  "logical.AND.expression", "logical.OR.expression", 
  "conditional.expression", "assignment.expression", 
  "assignment.operator", "expression", "constant.expression", 
  "expression.opt", "declaration", "declaring.list", "@1", "@2", "@3", 
  "@4", "@5", "@6", "@7", "@8", "default.declaring.list", "@9", "@10", 
  "@11", "@12", "@13", "@14", "@15", "@16", "declaration.specifier", 
  "basic.declaration.specifier", "sue.declaration.specifier", 
  "typedef.declaration.specifier", "declaration.qualifier.list", 
  "declaration.qualifier", "type.specifier", "basic.type.specifier", 
  "sue.type.specifier", "typedef.type.specifier", "type.qualifier.list", 
  "pointer.type.qualifier.list", "elaborated.type.name", "declarator", 
  "paren.typedef.declarator", "paren.postfix.typedef.declarator", 
  "simple.paren.typedef.declarator", "parameter.typedef.declarator", 
  "clean.typedef.declarator", "clean.postfix.typedef.declarator", 
  "abstract.declarator", "unary.abstract.declarator", 
  "postfix.abstract.declarator", "postfixing.abstract.declarator", 
  "array.abstract.declarator", "identifier.declarator", 
  "unary.identifier.declarator", "postfix.identifier.declarator", 
  "paren.identifier.declarator", "old.function.declarator", 
  "postfix.old.function.declarator", "identifier.list", 
  "identifier.or.typedef.name", "type.name", "attributes.opt", 
  "attributes", "attribute", "attribute.list", "attrib", "any.word", 
  "initializer.opt", "initializer", "initializer.list", 
  "parameter.type.list", "parameter.list", "parameter.declaration", 
  "struct.or.union.specifier", "struct.or.union", 
  "struct.declaration.list", "struct.declaration", 
  "struct.default.declaring.list", "struct.declaring.list", 
  "struct.declarator", "struct.identifier.declarator", 
  "bit.field.size.opt", "bit.field.size", "enum.specifier", 
  "enumerator.list", "enumerator.value.opt", "comma.opt", "statement", 
  "labeled.statement", "compound.statement", "ctokLEFT", "ctokRIGHT", 
  "declaration.list", "statement.list", "expression.statement", 
  "selection.statement", "iteration.statement", "jump.statement", 
  "constant", "string.literal.list", "type.qualifier", 
  "pointer.type.qualifier", "storage.class", "basic.type.name", 
  "identifier", "typedefname", "expr_pattern", "stmt_pattern", 
  "type_pattern", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,    58,    45,    44,    59,    33,    47,
      94,   126,    40,    41,   123,   125,    42,    38,    37,    43,
     124,    46,    36,    64,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,    91,    93,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   199,   200,   200,   201,   201,   201,   201,   201,   201,
     201,   201,   202,   203,   203,   204,   204,   204,   204,   204,
     204,   205,   205,   205,   206,   206,   207,   208,   208,   209,
     209,   210,   210,   211,   211,   212,   212,   213,   213,   213,
     213,   213,   214,   214,   214,   214,   214,   215,   215,   216,
     216,   216,   216,   217,   217,   217,   217,   217,   217,   218,
     218,   218,   218,   219,   219,   219,   219,   220,   220,   221,
     221,   222,   222,   222,   222,   223,   223,   223,   223,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   226,   226,   226,   226,   226,   227,
     227,   227,   227,   228,   228,   228,   228,   228,   228,   228,
     229,   229,   229,   229,   230,   230,   231,   231,   231,   231,
     231,   231,   232,   232,   233,   234,   234,   235,   235,   236,
     236,   236,   236,   236,   236,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   241,   241,
     241,   241,   241,   241,   242,   242,   242,   242,   242,   242,
     243,   243,   244,   244,   244,   244,   245,   245,   245,   246,
     246,   246,   247,   247,   247,   247,   247,   248,   248,   248,
     249,   249,   250,   250,   251,   251,   252,   252,   253,   253,
     254,   254,   255,   255,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   257,   257,   258,   259,   259,
     260,   260,   260,   260,   262,   263,   261,   264,   265,   261,
     266,   267,   261,   268,   261,   269,   261,   261,   271,   272,
     270,   273,   274,   270,   275,   276,   270,   277,   270,   278,
     270,   270,   279,   279,   279,   280,   280,   280,   280,   281,
     281,   281,   282,   282,   282,   283,   283,   283,   284,   284,
     285,   285,   285,   285,   286,   286,   286,   286,   287,   287,
     287,   288,   288,   288,   289,   289,   290,   290,   291,   291,
     292,   292,   292,   292,   293,   293,   293,   293,   293,   294,
     294,   294,   295,   295,   296,   296,   296,   297,   297,   297,
     298,   298,   299,   299,   299,   300,   300,   300,   300,   301,
     301,   301,   301,   302,   302,   302,   303,   303,   303,   303,
     304,   304,   305,   305,   305,   306,   306,   306,   307,   307,
     308,   308,   308,   309,   309,   309,   310,   310,   311,   311,
     312,   312,   312,   312,   313,   313,   314,   314,   315,   316,
     316,   317,   317,   317,   318,   318,   318,   319,   319,   320,
     320,   320,   321,   321,   322,   322,   322,   323,   323,   323,
     323,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   325,   325,   325,   325,   325,
     326,   326,   327,   327,   328,   328,   329,   329,   330,   330,
     331,   331,   332,   332,   333,   333,   334,   335,   335,   335,
     336,   336,   337,   337,   338,   338,   339,   339,   339,   339,
     339,   339,   339,   339,   340,   340,   340,   340,   341,   341,
     341,   341,   342,   343,   344,   344,   345,   345,   346,   347,
     347,   347,   348,   348,   348,   348,   348,   348,   348,   349,
     349,   349,   349,   349,   350,   350,   350,   350,   350,   351,
     351,   352,   352,   352,   353,   353,   354,   354,   354,   354,
     354,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     356,   357,   358,   358,   359,   360
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     4,     2,     6,     6,     4,     4,     4,
       4,     0,     1,     1,     0,     2,     3,     1,     3,     1,
       2,     4,     3,     5,     4,     1,     2,     1,     1,     1,
       1,     1,     4,     4,     4,     3,     3,     1,     2,     7,
       5,     4,     2,     1,     2,     3,     2,     3,     4,     1,
       3,     4,     4,     4,     2,     4,     2,     5,     5,     1,
       2,     7,     5,     4,     2,     1,     3,     3,     2,     3,
       5,     3,     6,     4,     3,     3,     2,     1,     2,     3,
       3,     2,     1,     2,     3,     3,     3,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     7,     3,     7,     1,     3,     1,     4,     2,     2,
       3,     3,     6,     6,     1,     1,     3,     1,     2,     3,
       3,     3,     3,     3,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     1,
       1,     1,     3,     5,     6,     1,     1,     4,     3,     4,
       3,     3,     2,     2,     3,     3,     1,     3,     1,     2,
       2,     2,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     0,     1,
       2,     2,     2,     2,     0,     0,     6,     0,     0,     6,
       0,     0,     7,     0,     5,     0,     5,     3,     0,     0,
       6,     0,     0,     6,     0,     0,     7,     0,     5,     0,
       5,     3,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     5,     3,     3,
       4,     4,     1,     3,     1,     2,     1,     1,     2,     3,
       3,     4,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     4,     1,     2,     3,     2,     3,     4,     3,
       1,     1,     1,     2,     3,     2,     3,     4,     1,     3,
       1,     2,     3,     4,     3,     4,     1,     3,     1,     1,
       1,     2,     1,     2,     0,     1,     1,     2,     6,     1,
       3,     0,     1,     4,     1,     1,     1,     0,     2,     3,
       4,     1,     1,     3,     1,     3,     1,     1,     3,     3,
       3,     1,     2,     2,     2,     1,     2,     2,     1,     2,
       2,     2,     1,     2,     2,     4,     5,     2,     3,     4,
       1,     1,     1,     2,     2,     2,     2,     3,     2,     3,
       3,     2,     3,     2,     0,     1,     2,     5,     6,     2,
       2,     4,     0,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     4,     3,     3,     2,     3,
       3,     4,     1,     1,     1,     2,     1,     2,     2,     5,
       7,     5,     5,     7,     9,     9,     9,     9,     5,     3,
       2,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,   124,   489,
     495,   493,   410,   499,     0,   490,   486,   492,   487,   411,
     481,   494,   498,   497,   496,   491,   482,   488,   501,   483,
       0,     0,     2,    10,     4,     5,     8,     0,    11,     9,
     454,     0,     0,     0,   262,   263,   264,     0,     0,   280,
     281,   282,     0,   288,   298,     0,   299,    12,   294,   275,
     284,   291,   283,     6,     7,     0,     0,   127,     0,     0,
      14,    59,     0,     0,     0,   500,   429,   358,   359,     0,
       1,     3,     0,   230,     0,   231,     0,   243,     0,     0,
     234,   300,   304,   301,   316,   317,   302,   340,   342,   341,
     303,   350,   348,   314,   267,   278,   279,   268,   232,   271,
     274,   257,     0,     0,   277,   270,   248,   341,   266,   273,
     245,   237,   286,   265,   287,   233,   290,   269,   293,   272,
     259,   289,   251,   295,   276,   285,   292,     0,   407,   455,
      21,    21,     0,     0,   128,     0,     0,    53,     0,     0,
     432,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,     0,    35,    37,    38,    39,    40,    41,   247,
     240,   261,   254,   364,     0,   484,   485,     0,   305,   318,
     343,   351,   296,     0,     0,     0,     0,     0,     0,     0,
     312,   364,     0,     0,   345,   333,     0,   315,   364,     0,
       0,   364,   364,   364,   364,   364,   408,     0,   280,   281,
     282,     0,     0,   412,     0,     0,     0,    17,    19,    22,
      23,     0,    18,    20,     0,     0,    60,     0,   127,    56,
       0,     0,    13,    54,    34,     0,   125,     0,     0,     0,
       0,     0,    69,     0,    92,     0,   430,   435,     0,   434,
     505,    64,     0,    66,     0,     0,     0,   136,   135,     0,
     116,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     109,     0,     0,     0,    31,    36,   364,   364,     0,   377,
     365,   366,     0,     0,   308,   319,   344,   352,   297,     0,
     309,     0,   320,   346,   349,   354,   235,   334,   386,   391,
     263,   395,   398,   281,   402,     0,     0,   384,   387,   356,
     174,   175,   176,   177,   178,   179,     0,   336,     0,   479,
     474,   475,   476,   477,   478,     0,     0,   502,     0,   156,
     168,   180,     0,   182,   186,   189,   192,   197,   200,   202,
     204,   206,   208,   210,   227,     0,   150,   151,   149,   155,
       0,   377,   249,   377,   238,   377,   252,     0,   424,   418,
     364,   424,   416,   364,   405,   413,   415,     0,   414,     0,
     409,     0,     0,    24,    24,    61,    62,    57,     0,    55,
       0,    33,     0,     0,     0,     0,     0,    74,     0,    97,
      67,    70,    68,    93,   433,   432,   427,     0,     0,     0,
      45,     0,     0,     0,    46,     0,    47,     0,     0,     0,
     136,   146,   148,     0,   147,     0,   119,     0,     0,   110,
     105,   106,   103,   104,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,   118,     0,   112,   136,     0,     0,
       0,    75,    87,     0,   241,   255,     0,     0,   244,   367,
     306,     0,   313,   311,   310,   321,   347,   355,   377,   325,
       0,   394,   392,   322,   323,   324,   393,   325,     0,   396,
     397,   401,   399,   400,   403,   404,   353,     0,   335,     0,
       0,   452,   180,   212,   225,     0,   360,   362,     0,     0,
       0,   172,     0,   169,   170,     0,     0,     0,     0,     0,
     162,   163,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   337,   480,   339,     0,   258,   377,   246,   377,
     260,   377,   426,   364,   425,   421,   364,   423,   417,   419,
     406,    29,     0,    27,     0,    15,    16,    58,   126,    98,
      95,    96,    94,     0,     0,   431,   428,    63,    65,    42,
       0,    52,     0,    44,    48,    43,   109,     0,   137,   121,
     120,   115,   139,   144,   141,   143,   142,   145,   138,   140,
       0,     0,     0,    86,     0,     0,    91,    88,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   377,   371,     0,   381,   378,   307,   236,
     326,   327,     0,     0,     0,   326,   357,   390,   385,   388,
     389,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   152,     0,   325,     0,   361,   363,     0,
       0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,   456,   436,   437,     0,     0,
       0,   438,   439,   440,   441,   149,   291,     0,     0,     0,
     158,     0,   166,   160,   164,     0,   161,   165,   183,   185,
     184,   187,   188,   190,   191,   193,   194,   195,   196,   198,
     199,   201,   203,   205,   207,     0,   209,   338,   250,   239,
     253,   420,   422,    30,     0,    26,    25,    73,     0,     0,
       0,   117,     0,     0,    84,    85,    89,    90,   109,    99,
     100,   101,   102,    81,     0,    79,    77,    76,     0,     0,
     130,   129,   132,   134,   131,   133,   242,   256,   376,     0,
     369,   372,   374,   375,   382,     0,   328,   329,   330,   331,
     213,   226,   326,   181,   442,   471,     0,     0,     0,   470,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     453,   448,     0,     0,     0,   457,     0,     0,     0,   443,
     173,   503,   159,     0,   157,     0,    28,     0,    72,    51,
       0,     0,     0,     0,    83,   122,   123,     0,   371,     0,
     379,     0,   332,     0,     0,   472,     0,     0,   446,   469,
     473,     0,     0,     0,     0,   449,     0,   450,     0,   153,
     444,   447,   167,   211,     0,     0,    50,   111,   113,    80,
       0,   368,   370,     0,   380,   383,     0,   445,     0,   228,
       0,     0,     0,     0,   504,   451,   154,    71,     0,    82,
     373,   461,   468,     0,     0,     0,     0,   459,   462,    49,
     228,   228,     0,     0,     0,     0,     0,     0,     0,   463,
     460,     0,     0,     0,     0,   465,   467,   466,   464
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    31,    32,    33,    34,    35,   221,   545,   373,   542,
     543,    36,    37,   162,   163,   164,   405,   406,   146,   147,
     165,    38,   241,   242,   440,   441,   243,   244,   724,   434,
     167,   268,   269,   168,    39,   235,    71,   442,   443,   329,
     330,   671,   482,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   483,   484,   632,   653,   345,
     654,    40,    41,   191,   458,   203,   529,   276,   602,   173,
     202,    42,   201,   527,   205,   531,   277,   603,   198,   204,
      43,    44,    45,    46,    47,   109,    48,    49,    50,    51,
      52,   177,    53,   358,    91,    92,   185,    93,    94,    95,
     611,   463,   464,   465,   195,    96,    97,    98,   117,   100,
     101,   305,   150,   488,   279,   280,   281,   739,   740,   741,
     448,   607,   745,   306,   307,   308,    54,    55,   212,   213,
     214,   215,   359,   362,   533,   360,    56,   151,   246,   248,
     655,   656,   657,   658,   771,    57,   660,   661,   662,   663,
     664,   346,   347,    58,   182,    59,    60,   348,   765,   349,
     667,    62
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -616
static const short yypact[] =
{
    1591,    75,   105,   195,   223,   286,   227,   281,  -616,  -616,
    -616,  -616,  -616,  -616,   -60,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
     -16,   863,  -616,  -616,  -616,  -616,  -616,    96,  -616,  -616,
    -616,   471,   592,   540,  1496,   671,   772,  1532,   557,  1496,
     814,   772,  1633,  -616,  -616,   311,  -616,  2693,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,   245,   283,    23,   380,   476,
    -616,  -616,   386,   356,   -57,  -616,   354,  -616,  -616,   291,
    -616,  -616,  1725,  -616,   654,  -616,    91,  -616,  1153,    34,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,   198,
    -616,  -616,  -616,   248,  -616,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,   873,   325,  -616,  -616,  -616,   248,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  2536,   368,  -616,
     750,  1000,   464,   476,  -616,   183,    61,  -616,   358,   815,
     310,   396,   -57,   417,    15,   155,   447,   451,   889,  1381,
     463,  -616,  2118,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  -616,   391,    34,  -616,  -616,  1547,  -616,  -616,
    -616,  -616,  -616,    34,   485,   248,   526,   537,   582,   554,
    -616,   391,  2447,  1957,  -616,   551,  2500,  -616,   391,   873,
     648,   391,   391,   391,   391,   391,  -616,     1,  2255,   312,
     312,  2370,  2574,  -616,   736,   757,  2610,  -616,  -616,  -616,
    -616,   520,  -616,  -616,   520,   251,  -616,   250,  -616,  -616,
     183,   492,  -616,  -616,  -616,   341,  -616,   268,   131,   597,
     689,   277,  -616,   163,  -616,  2173,  -616,   -57,   605,   396,
    -616,  -616,   183,  -616,   183,   347,   811,   600,  -616,   645,
    -616,   370,   370,   370,   370,   728,   730,   738,   238,  -616,
    2239,   703,   313,   610,  -616,  -616,   391,   391,   691,   608,
     391,  -616,   669,    34,  -616,  -616,  -616,  -616,  -616,   745,
     248,   701,   248,   248,  -616,   248,  -616,  -616,  -616,   -40,
     772,  2334,   -40,   772,  2334,   178,   706,   704,   666,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  1698,  -616,  2186,  -616,
    -616,  -616,  -616,  -616,  -616,  2262,  2262,  -616,   781,  -616,
     803,  -616,  2173,  -616,   749,    53,   337,   147,   510,   802,
     817,   804,   746,   -47,  -616,   806,  -616,   771,  -616,  -616,
    1987,   608,  -616,   608,  -616,   608,  -616,  2173,   824,  -616,
     391,   824,  -616,   391,  -616,  -616,  -616,   428,  -616,     1,
    -616,  2648,   935,   933,   933,  -616,  -616,  -616,   183,  -616,
     183,  -616,   941,   944,   945,   370,    52,  -616,   610,  -616,
    -616,  -616,  -616,  -616,  -616,   310,  -616,   838,   318,   561,
    -616,   599,   382,   877,  -616,   372,  -616,   628,   948,   610,
    -616,   522,  -616,  1600,   522,   875,  -616,   768,   726,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,   370,   370,   370,   370,
     370,   370,   370,   370,  -616,   610,  -616,  2831,   610,   610,
     502,  -616,  -616,  1145,  -616,  -616,   869,  2063,  -616,  -616,
    -616,   779,  -616,  -616,  -616,  -616,  -616,  -616,   608,  1317,
    1773,  -616,  -616,  -616,  -616,  -616,  -616,   672,  1826,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,   820,  -616,  1134,
    2063,  -616,  1664,  -616,  -616,   399,   182,  2412,   878,  1048,
    1698,  -616,  2093,  -616,  -616,   788,  2143,   -57,  2173,   -57,
    -616,  -616,  -616,  2173,  2173,  2173,  2173,  2173,  2173,  2173,
    2173,  2173,  2173,  2173,  2173,  2173,  2173,  2173,  2173,  2173,
    2173,  2173,  -616,  -616,  -616,   872,  -616,   608,  -616,   608,
    -616,   608,  -616,   391,  -616,  -616,   391,  -616,  -616,  -616,
    -616,   520,   591,  -616,   980,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  1618,   220,   569,  -616,  -616,  -616,  -616,  -616,
     476,  -616,   610,  -616,  -616,  -616,  1089,   576,  -616,  -616,
    -616,  -616,   522,  -616,   942,  -616,   844,  -616,   522,  1412,
     661,   992,   993,  -616,  1001,  1003,  -616,  -616,  1007,  1109,
    -616,   678,  1037,   610,   610,   692,   370,   370,   370,   370,
     370,   370,   608,   608,   535,  2063,  -616,  -616,  -616,  -616,
    1317,  -616,   901,   920,   926,   672,  -616,  -616,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  2173,  -616,  2173,   899,  1879,  -616,  -616,  2173,
     932,   934,   939,  2173,  2173,   937,   943,   978,   -57,  1276,
     949,   950,   971,   982,   986,  -616,  -616,  -616,   940,  1048,
    1197,  -616,  -616,  -616,  -616,   981,   988,  1038,  1006,  1036,
    -616,   524,  -616,  -616,  -616,    22,  -616,  -616,  -616,  -616,
    -616,   749,   749,    53,    53,   337,   337,   337,   337,   147,
     147,   510,   802,   817,   804,   799,   746,  -616,  -616,  -616,
    -616,  -616,  -616,  -616,   935,  -616,  -616,  -616,   207,   884,
     681,  -616,   726,   726,  -616,  -616,  -616,  -616,  1089,  -616,
    -616,  -616,  -616,  -616,  1108,  -616,  -616,  -616,  1276,  1045,
    1618,  1618,  1618,  1618,  1618,  1618,  -616,  -616,  -616,   611,
    -616,  1023,  -616,  -616,  -616,   712,  -616,   248,  -616,  -616,
    -616,  -616,   899,  -616,  -616,  -616,  2173,  1025,  1043,  -616,
    1382,  1276,  1046,  1047,  1012,   988,  2173,  2173,   959,  -616,
    -616,  -616,   940,  1197,  1197,  -616,  1054,  1276,  1276,  -616,
    -616,  -616,  -616,  2173,  -616,  2173,  -616,    52,  -616,  -616,
     403,   366,   553,  1126,  -616,  -616,  -616,  1055,   535,  2173,
    -616,  1936,  -616,   659,  1276,  -616,   825,  1053,  -616,  -616,
    -616,  1058,   693,   715,  1092,  -616,  1197,  -616,  1090,  -616,
    -616,  -616,  -616,  -616,   236,   476,  -616,  -616,  -616,  -616,
    1194,  -616,  -616,   734,  -616,  -616,  1276,  -616,  1276,  2173,
    1403,  2173,  1276,  1276,  -616,  -616,  -616,  -616,   896,  -616,
    -616,  -616,  -616,  1094,  1095,  1102,   777,  1096,  -616,  -616,
    2173,  2173,  1424,  1111,  1276,  1115,  1116,  1117,  1118,  -616,
    -616,  1276,  1276,  1276,  1276,  -616,  -616,  -616,  -616
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -616,  -616,  1195,  -616,  -616,  -616,  1086,   854,  -198,  -616,
     525,  -616,  -616,  -616,  1068,  -616,  -616,   826,  -139,    -3,
    -616,   -34,  -616,   991,   186,  -616,  -374,  -235,   443,  -416,
    -616,  -149,   819,  -616,  -616,   196,   307,  -616,   139,  -616,
    -616,  -616,  -177,  -616,  -315,   360,   374,   503,   375,   722,
     727,   732,   741,   724,  -616,  -186,  -438,  -616,  -286,  -231,
    -255,   -56,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
    -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,  -616,
    -160,  -616,   -63,  -616,   -50,   -24,  -109,   -51,    40,    66,
     180,  -101,   -28,   545,   494,  -616,  -143,   -82,   -52,  -616,
    -220,  -415,  -395,   -32,  -616,     2,   -74,  -616,   549,   521,
    -616,  -616,    21,   761,   342,  -616,   973,  -616,   468,  -616,
    -104,  -433,  -616,  -616,  -616,   791,  -616,  -616,  1051,  -187,
    -616,  -616,   902,   905,   913,  -159,  -616,  1123,   883,  1030,
      60,  -616,  -616,  -265,  -598,  -476,  -615,  -616,  -616,  -616,
    -616,  -616,  -616,   442,  -172,   465,   167,   -14,    63,  -616,
    -616,  -616
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -229
static const short yytable[] =
{
      77,   139,    70,   387,   227,   288,   179,   344,   393,   606,
     272,   199,   553,   659,   394,   187,   331,   502,   251,   115,
     104,   589,   110,   114,   131,   365,   374,   288,   207,   102,
     485,   282,   299,   102,   102,    76,   299,   186,   102,   187,
     289,    77,   606,   773,   774,   612,    74,   620,   166,   116,
     459,   489,   363,   612,   132,   237,    79,   520,   672,   344,
      77,   460,   776,    61,    67,   613,   142,   194,   331,   193,
     102,   197,   102,   613,   102,   102,   138,    78,    63,   462,
     240,   469,   472,   302,   474,   194,   208,   302,   172,   252,
     180,    88,   171,    75,    61,   285,    75,   143,   102,   102,
     187,    28,    89,   207,    28,   230,   103,   207,    64,   187,
     119,   103,   357,    75,   180,   136,   401,   407,    78,   525,
      61,    28,   186,    68,    88,   521,   532,   231,   166,   300,
     634,   186,   784,   300,   237,   183,   232,    78,    77,   226,
     451,   491,   301,   233,   506,   507,   301,   103,   493,   494,
     711,   103,   190,   291,    75,   331,   194,   816,   253,   240,
     102,   208,    28,   102,   344,   208,   237,   606,   194,   102,
      82,   344,   744,   331,   815,   817,   818,   209,   309,   286,
     331,   112,   772,   131,   365,   102,   228,    75,   678,   679,
     680,   240,   113,   102,   750,    28,   751,   102,    65,   534,
      61,   286,   534,   210,   485,   386,   485,   486,   363,   187,
     237,   107,   675,   361,   118,    78,   124,   461,   845,   135,
     471,   612,   375,   237,   233,   489,    66,   489,   379,   254,
      72,   186,   303,    77,   695,   240,   303,   190,   392,   237,
     103,   613,   510,   511,    75,    68,   190,   526,   240,   528,
     291,   530,   209,    67,    67,    61,   209,   291,   453,    61,
     455,   456,   207,   457,   240,   208,   637,   638,   395,   102,
     103,   382,   635,   115,   136,    61,   131,   114,   210,    61,
     476,   787,   210,   636,    73,   102,   477,   102,   102,    67,
     102,   193,   238,   239,   230,   707,   418,   270,   270,   192,
     299,   466,   793,   470,   473,   419,   475,   193,   299,   140,
      78,   847,    68,    68,   512,   513,   231,   211,   393,   299,
     208,   383,   384,   385,   753,   376,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   703,   331,   822,   190,   141,    68,   196,
      67,   302,   390,   102,   609,   102,   209,   193,   610,   302,
      69,   228,   103,   606,   119,   103,   615,   136,   835,   361,
     302,   418,   304,   410,   258,   124,   304,   179,   135,    61,
     436,   486,   210,   144,   380,    67,   187,   402,   403,   758,
     746,   145,   211,   557,   187,   746,   211,   300,   233,   561,
     411,   412,   413,   414,   233,   300,    67,   380,   186,    68,
     301,   209,   757,   824,   381,   112,   300,   137,   301,   291,
      68,   709,   400,   698,   418,   699,   113,   700,   614,   301,
     149,   234,   103,   827,    61,   261,   614,   210,   288,   208,
      20,   262,   263,   288,    68,   102,   102,   563,   398,   264,
     399,    26,   229,   102,   102,   236,   560,   344,   148,   131,
     152,   180,   331,   616,    75,    68,   331,    67,   118,   180,
     803,   135,    28,   788,   216,   665,   299,   825,    75,    67,
     812,   813,   153,   673,   245,   676,   105,   105,   105,   105,
     250,   122,   126,   128,   133,    67,   487,    29,   736,   737,
     303,   633,   508,   509,   247,   807,   225,   634,   303,   106,
     106,   106,   106,   833,   123,   127,   129,   134,   112,   303,
     145,   255,   103,    61,   552,   256,    68,   302,   285,   113,
     209,    61,   746,   296,   752,   273,   378,   377,    68,   357,
     351,    87,    61,   352,   353,   354,   355,   356,   593,   594,
     136,   211,   666,    61,    68,   856,   210,   270,   120,   236,
     674,   236,   677,   791,   792,   572,   573,   574,   575,   576,
     577,   578,   579,   300,   554,   595,    83,   278,   592,    84,
     288,    75,   178,   184,   853,   855,   301,   290,    90,   393,
     742,   427,    99,   121,   372,   567,   102,    99,   429,   823,
     431,   102,    67,   139,   614,   865,   866,   868,   331,   181,
     189,   418,   286,   437,   258,   593,   594,   286,   444,   445,
     828,   580,   593,   594,   590,   591,   782,   380,   292,   170,
      88,    67,   783,    99,   762,   665,   558,    99,   188,   293,
     304,    89,   708,   230,   665,   665,   665,    88,   304,   712,
     122,   126,   128,   133,   135,   169,   295,   704,    89,   304,
     350,    68,   200,   738,   408,   231,   705,   743,   184,   388,
     487,   284,   230,   103,   559,   261,   303,   184,   438,   514,
     515,   262,   439,   192,   294,   547,   848,   548,    75,   264,
      68,   193,   389,    75,   231,   189,    28,    85,   287,    61,
      86,    28,   535,   565,   189,   537,   233,   593,   594,   764,
      75,   763,   396,   797,   665,   802,   139,   409,    28,   798,
     775,   666,   666,   188,   593,   594,    99,   593,   594,   257,
     258,   415,   188,   416,   713,   730,   731,   732,   733,   734,
     735,   417,   105,   105,    88,   126,   133,   665,   710,   196,
     294,   725,   728,   729,   790,    89,    99,   193,   665,   665,
     665,   836,   467,   665,   665,   106,   106,   634,   127,   134,
     196,   450,   217,   468,   218,   435,   108,   184,   193,   726,
     727,   193,   447,     9,   742,   219,   220,   826,   795,   260,
     665,   261,   446,    15,    16,   842,    18,   262,   263,    20,
     175,   634,   665,   454,   189,   264,   265,    75,   478,   266,
      26,   176,   479,    27,    67,    28,   304,   843,   237,   800,
     801,   808,   665,   634,   665,    75,   402,   403,   665,   665,
     238,   239,   188,   775,   775,   666,   850,   820,   821,   503,
     480,   366,   634,   240,   367,   233,   196,   452,   504,   505,
     665,   270,   270,   495,   193,   145,    29,   665,   665,   665,
     665,   743,   368,    80,   837,   369,   681,   682,     1,     2,
       3,     4,     5,    68,     6,   701,   775,     7,   702,   863,
     196,   608,   683,   684,     9,   634,   404,    67,   193,   689,
     690,   516,   257,   258,    15,    16,   851,    18,   852,    67,
      20,   518,   857,   858,   496,   259,   497,   634,     8,   426,
     785,    26,   498,   427,    27,   517,   522,   519,    99,   125,
     429,   267,   431,   432,   870,   523,     9,   838,   230,   133,
     839,   875,   876,   877,   878,   357,    15,    16,   541,    18,
     230,   640,    20,   544,   549,   556,    68,   550,   551,   562,
     231,   566,   260,    26,   261,   569,    27,    29,    68,   789,
     262,   263,   231,   112,   570,   499,   500,   501,   264,   265,
     604,   859,   266,    75,   113,     9,    10,    11,    12,   669,
     639,    13,   697,   706,    14,    15,    16,    17,    18,   635,
      19,    20,    21,    22,    23,   714,   715,    24,    25,    29,
     636,   175,    26,   747,   716,    27,   717,   426,   193,   200,
     718,   427,   176,   685,   686,   687,   688,   200,   429,   430,
     431,   432,   748,   222,    28,   223,    75,   175,   749,   310,
     311,   312,   313,   314,   315,   219,   220,   754,   176,   755,
     756,   316,   759,   768,   760,  -228,   481,   770,    29,   640,
     766,   767,     9,    10,    11,    12,   641,    30,    13,   642,
     643,    14,    15,    16,    17,    18,   644,    19,    20,    21,
      22,    23,   645,   646,    24,    25,   647,   648,   318,    26,
     649,   650,    27,   651,   267,   596,   597,   598,   599,   761,
     634,   769,   777,    75,   319,   320,   321,   322,   323,   778,
     324,    28,   426,   325,   326,   779,   427,   428,   780,   781,
     568,   794,   796,   429,   430,   431,   432,   433,   420,   421,
     422,   423,   424,   425,   799,    29,   719,   720,   721,   722,
     723,   327,   328,   652,    30,   617,   804,   310,   311,   312,
     313,   314,   315,   719,   720,   721,   722,   829,   805,   316,
     814,   809,   810,  -228,   481,   811,   819,   831,   840,   841,
       9,    10,    11,    12,   641,   844,    13,   642,   643,    14,
      15,    16,    17,    18,   644,    19,    20,    21,    22,    23,
     645,   646,    24,    25,   647,   648,   318,    26,   649,   650,
      27,   651,   846,   596,   597,   598,   599,   849,   640,   860,
     861,    75,   319,   320,   321,   322,   323,   862,   324,    28,
     426,   325,   326,   864,   427,   428,   869,   871,   872,   873,
     874,   429,   430,   431,   432,   433,    81,   224,   546,   786,
     275,   564,   391,    29,   600,   601,   830,   571,   691,   327,
     328,   652,    30,    88,   692,   696,     9,    10,    11,    12,
     693,   668,    13,   449,   174,    14,    15,    16,    17,    18,
     694,    19,    20,    21,    22,    23,   832,   371,    24,    25,
     619,   539,   538,    26,   536,   249,    27,   640,   555,   397,
       0,   175,     0,     0,     0,     0,   310,   311,   312,   313,
     314,   315,   176,     0,     0,    28,     0,     0,   316,     0,
       0,     0,  -228,   481,   770,     0,    75,   618,     0,     0,
       0,     0,     0,   641,    28,     0,   642,   643,     0,    29,
       0,     0,     0,   644,     0,     0,     0,     0,    30,   645,
     646,     0,     0,   647,   648,   318,     0,   649,   650,     0,
     651,     0,   600,   601,     0,     0,     0,     0,     0,     0,
      75,   319,   320,   321,   322,   323,     0,   324,    28,     0,
     325,   326,     0,     0,     0,   310,   311,   312,   313,   314,
     315,     0,     0,     0,     0,     0,     0,   316,     0,     0,
       0,  -228,   481,   806,   257,   258,     0,     0,   327,   328,
     652,     0,   641,     0,     0,   642,   643,   271,     0,     0,
       0,     0,   644,     0,   854,     0,     0,   459,   645,   646,
       0,     0,   647,   648,   318,     0,   649,   650,   460,   651,
       0,     0,     0,     0,     0,   867,   193,     0,     0,    75,
     319,   320,   321,   322,   323,     0,   324,    28,     0,   325,
     326,     0,     0,     0,   260,   175,   261,     0,     0,     0,
       0,     0,   262,   263,     0,     0,   176,     0,     0,     0,
     264,   265,     0,     0,   266,     0,     0,   327,   328,   652,
      75,   310,   311,   312,   313,   314,   315,   426,    28,     0,
       0,   427,   428,   316,     0,     0,     0,  -228,   429,   430,
     431,   432,   310,   311,   312,   313,   314,   315,     0,     0,
       0,     0,     0,     0,   316,     0,     0,     0,  -228,     0,
       0,     0,     0,   310,   311,   312,   313,   314,   315,     0,
     318,     0,     0,     0,     0,   316,  -228,     0,     0,     0,
       0,     0,     0,   111,     0,    75,   319,   320,   321,   322,
     323,   318,   324,     0,     0,   325,   326,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   319,   320,   321,
     322,   323,   318,   324,     0,     0,   325,   326,     0,     0,
       0,     0,     0,   327,   328,     0,   267,    75,   319,   320,
     321,   322,   323,     0,   324,     0,     0,   325,   326,     0,
       0,     0,     0,     0,   327,   328,     1,     2,     3,     4,
       5,     0,     6,     0,     0,     7,     0,     0,     9,    10,
      11,     0,     0,     0,    13,   327,   328,     0,    15,    16,
      17,    18,   112,     0,    20,    21,    22,    23,     0,     0,
      24,    25,     0,   113,   130,    26,     8,    88,    27,     0,
       0,     0,     0,     0,     9,    10,    11,    12,   283,     0,
      13,     0,     0,    14,    15,    16,    17,    18,     0,    19,
      20,    21,    22,    23,     0,   426,    24,    25,     0,   427,
     428,    26,     0,   568,    27,   175,   429,   430,   431,   432,
     433,    29,     0,   426,     0,    75,   176,   427,   428,     0,
       0,     0,     0,    28,   429,   430,   431,   432,   433,     0,
      75,     0,     0,     9,    10,    11,    12,     0,    28,    13,
       0,     0,    14,    15,    16,    17,    18,    29,    19,    20,
      21,    22,    23,   112,     0,    24,    25,     0,     0,     0,
      26,     0,     0,    27,   113,     0,     0,   154,   155,     7,
       0,     0,     0,     0,     0,     9,    10,    11,    12,     0,
       0,    13,    28,     0,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,   156,   157,    24,    25,     0,
       0,     0,    26,     0,     0,    27,    29,     0,     0,     0,
       0,   158,   159,     0,   160,    30,    75,   310,   311,   312,
     313,   314,   315,     0,    28,     0,     0,     0,     0,   316,
     161,     0,     0,     0,   481,     0,     0,     0,     0,     0,
       0,    10,    11,    12,     0,     0,    13,     0,    29,    14,
       0,     0,    17,     0,     0,    19,    20,    21,    22,    23,
       0,     0,    24,    25,     0,     0,   318,    26,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,     0,
       0,    75,   319,   320,   321,   322,   323,     0,   324,    28,
       0,   325,   326,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   460,   297,     0,     0,     0,     0,
       0,     0,   193,    29,     0,     9,    10,    11,    12,   327,
     328,    13,    30,     0,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,     0,     0,    24,    25,     0,
       0,     0,    26,     0,     0,    27,   467,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   468,   297,     0,
       0,     0,     0,     0,    28,   193,     0,     0,     9,    10,
      11,    12,     0,     0,    13,     0,   298,    14,    15,    16,
      17,    18,     0,    19,    20,    21,    22,    23,    29,     0,
      24,    25,     0,     0,     0,    26,     0,    30,    27,   635,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
     636,   297,     0,     0,     0,     0,     0,    28,   193,     0,
       0,     9,    10,    11,    12,     0,     0,    13,     0,   298,
      14,    15,    16,    17,    18,     0,    19,    20,    21,    22,
      23,    29,     0,    24,    25,     0,     0,     0,    26,     0,
      30,    27,     0,     0,     0,   310,   311,   312,   313,   314,
     315,     0,     0,     0,     0,     0,     0,   316,     0,     0,
      28,     0,   605,   834,     0,     0,   310,   311,   312,   313,
     314,   315,   298,     0,     0,     0,     0,     0,   316,     0,
       0,     0,     0,     0,    29,     0,     0,   317,     0,     0,
       0,     0,     0,    30,   318,     0,   310,   311,   312,   313,
     314,   315,     0,     0,     0,     0,     0,     0,   316,    75,
     319,   320,   321,   322,   323,   318,   324,   524,     0,   325,
     326,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,   319,   320,   321,   322,   323,     0,   324,     0,     0,
     325,   326,     0,     0,     0,   318,     0,   327,   328,     0,
     154,   155,     7,     0,     0,     0,     0,     0,     0,     0,
      75,   319,   320,   321,   322,   323,     0,   324,   327,   328,
     325,   326,   310,   311,   312,   313,   314,   315,   156,   157,
       0,     0,     0,     0,   316,     0,     0,     0,     0,   605,
       0,     0,     0,     0,   158,   159,     0,   160,   327,   328,
       0,     0,   310,   311,   312,   313,   314,   315,     0,     0,
       0,     0,     0,   274,   316,     0,     0,     0,     0,   481,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   319,   320,   321,
     322,   323,     0,   324,     0,     0,   325,   326,     0,     0,
       0,   318,   310,   311,   312,   313,   314,   315,     0,     0,
       0,     0,     0,     0,   316,   670,    75,   319,   320,   321,
     322,   323,     0,   324,   327,   328,   325,   326,     0,     0,
       0,     0,   310,   311,   312,   313,   314,   315,   420,   421,
     422,   423,   424,   425,   316,   310,   311,   312,   313,   314,
     315,   318,     0,     0,   327,   328,     0,   490,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   319,   320,   321,
     322,   323,     0,   324,   426,     0,   325,   326,   427,   428,
       0,   318,     0,     0,     0,   429,   430,   431,   432,   433,
       0,     0,     0,     0,   318,     0,    75,   319,   320,   321,
     322,   323,     0,   324,   327,   328,   325,   326,     0,    75,
     319,   320,   321,   322,   323,     0,   324,     0,     0,   325,
     326,   310,   311,   312,   313,   314,   315,     0,     0,     0,
       0,     0,     0,   492,   327,   328,     0,     0,    10,    11,
       0,     0,     0,    13,     0,     0,     0,   327,   328,    17,
       0,     0,     0,    20,    21,    22,    23,     0,     0,    24,
      25,     0,     0,     0,    26,     0,     0,     0,     0,     0,
     318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,   319,   320,   321,   322,
     323,     0,   324,     0,   467,   325,   326,     0,     0,     0,
       0,     0,     0,     0,     0,   468,     0,     0,     0,     0,
      29,     0,     0,   193,     0,     0,     9,    10,    11,    12,
       0,     0,    13,   327,   328,    14,    15,    16,    17,    18,
     112,    19,    20,    21,    22,    23,     0,     0,    24,    25,
       0,   113,     0,    26,     0,     0,    27,     0,     0,     0,
       0,   357,     0,    10,    11,    12,     0,    75,    13,     0,
       0,    14,     0,     0,    17,    28,     0,    19,    20,    21,
      22,    23,   635,     0,    24,    25,     0,     0,     0,    26,
       0,     0,     0,   636,     0,     0,     0,     0,     0,    29,
       0,   193,     0,    75,     0,    10,    11,    12,     0,     0,
      13,    28,     0,    14,     0,     0,    17,     0,     0,    19,
      20,    21,    22,    23,     0,     0,    24,    25,     0,   297,
       0,    26,     0,     0,     0,    29,     0,     0,     0,     9,
      10,    11,    12,     0,     0,    13,     0,     0,    14,    15,
      16,    17,    18,    28,    19,    20,    21,    22,    23,     0,
       0,    24,    25,     0,     0,     0,    26,     0,     0,    27,
       0,     0,     0,     0,     0,     0,     0,    29,     0,     0,
      75,     0,   297,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     9,    10,    11,    12,     0,     0,    13,     0,
     298,    14,    15,    16,    17,    18,     0,    19,    20,    21,
      22,    23,    29,     0,    24,    25,     0,     0,     0,    26,
       0,    30,    27,   206,     0,     0,     0,     0,     0,    10,
      11,    12,     0,     0,    13,     0,     0,    14,     0,     0,
      17,    28,     0,    19,    20,    21,    22,    23,     0,     0,
      24,    25,     0,   298,     0,    26,     0,     0,     0,     0,
       0,   364,     0,     0,     0,    29,     0,    10,    11,    12,
       0,     0,    13,     0,    30,    14,     0,    28,    17,     0,
       0,    19,    20,    21,    22,    23,     0,     0,    24,    25,
       0,     0,     0,    26,     0,     0,     0,   370,     0,     0,
       0,    29,     0,    10,    11,    12,     0,     0,    13,     0,
      30,    14,     0,     0,    17,    28,     0,    19,    20,    21,
      22,    23,     0,     0,    24,    25,     0,     0,     0,    26,
       0,     0,     0,     0,     0,   540,     0,     0,     0,    29,
       0,    10,    11,    12,     0,     0,    13,     0,    30,    14,
       0,    28,    17,     0,     0,    19,    20,    21,    22,    23,
       0,     0,    24,    25,     0,     0,     0,    26,     0,     0,
       0,     0,     0,     0,     0,    29,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     9,    10,    11,    12,    28,
       0,    13,     0,     0,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,     0,     0,    24,    25,     0,
       0,     0,    26,    29,   581,    27,     0,     0,     0,     0,
       0,     0,    30,     0,     0,     0,     0,     0,  -109,  -109,
    -109,  -109,  -109,     0,    28,     0,     0,   582,   583,     0,
     420,   421,   422,   423,   424,   425,     0,     0,   584,   585,
     586,     0,     0,     0,     0,     0,     0,     0,    29,     0,
       0,     0,     0,   587,     0,     0,     0,    30,     0,     0,
       0,     0,     0,     0,     0,   588
};

static const short yycheck[] =
{
      14,    57,     5,   238,   143,   177,    88,   193,   243,   447,
     159,   112,   386,   489,   245,    89,   193,   332,     3,    47,
      44,   437,    46,    47,    52,   212,   224,   199,   137,    43,
     316,   174,   192,    47,    48,    14,   196,    89,    52,   113,
     183,    55,   480,   658,   659,   460,   106,   480,    82,    47,
      90,   316,   211,   468,    52,     3,    72,   104,   496,   245,
      74,   101,   660,     0,     3,   460,    43,    99,   245,   109,
      84,   103,    86,   468,    88,    89,    55,    14,     3,   299,
      28,   301,   302,   192,   304,   117,   137,   196,    86,    74,
      88,    90,     1,   153,    31,   177,   153,    74,   112,   113,
     174,   161,   101,   212,   161,    44,    43,   216,     3,   183,
      47,    48,   111,   153,   112,    52,   255,   256,    55,   350,
      57,   161,   174,    62,    90,   172,   357,    66,   162,   192,
     108,   183,   110,   196,     3,   101,    75,    74,   152,   142,
     283,   318,   192,   146,    91,    92,   196,    84,   325,   326,
     566,    88,    89,   185,   153,   332,   188,   772,     3,    28,
     174,   212,   161,   177,   350,   216,     3,   605,   200,   183,
      74,   357,   605,   350,   772,   773,   774,   137,   192,   177,
     357,    90,   658,   211,   371,   199,     3,   153,   503,   504,
     505,    28,   101,   207,   632,   161,   634,   211,     3,   358,
     137,   199,   361,   137,   490,    74,   492,   316,   367,   283,
       3,    44,   498,   211,    47,   152,    49,   299,   816,    52,
     302,   636,   225,     3,   227,   490,     3,   492,   231,    74,
       3,   283,   192,   247,   520,    28,   196,   174,    75,     3,
     177,   636,    95,    96,   153,    62,   183,   351,    28,   353,
     282,   355,   212,     3,     3,   192,   216,   289,   290,   196,
     292,   293,   371,   295,    28,   316,   486,   487,   247,   283,
     207,     3,    90,   301,   211,   212,   304,   301,   212,   216,
     102,    74,   216,   101,     3,   299,   108,   301,   302,     3,
     304,   109,    15,    16,    44,    75,    58,   158,   159,   101,
     460,   299,   718,   301,   302,    67,   304,   109,   468,    64,
     247,    75,    62,    62,   167,   168,    66,   137,   553,   479,
     371,    53,    54,    55,   639,    75,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   541,   521,   783,   283,    64,    62,   101,
       3,   460,    75,   367,   458,   369,   316,   109,   459,   468,
      74,     3,   299,   801,   301,   302,   467,   304,   801,   367,
     479,    58,   192,     3,     4,   208,   196,   459,   211,   316,
      67,   490,   316,     3,    66,     3,   460,    15,    16,   644,
     610,    44,   212,    75,   468,   615,   216,   460,   401,   402,
     261,   262,   263,   264,   407,   468,     3,    66,   460,    62,
     460,   371,   643,   787,    73,    90,   479,   106,   468,   451,
      62,   560,    75,   527,    58,   529,   101,   531,   460,   479,
      74,    73,   369,    67,   371,    65,   468,   371,   610,   490,
     128,    71,    72,   615,    62,   459,   460,    75,   252,    79,
     254,   139,   145,   467,   468,   148,    74,   643,    72,   487,
     106,   459,   639,   477,   153,    62,   643,     3,   301,   467,
     756,   304,   161,   708,   106,   489,   636,    74,   153,     3,
     766,   767,   191,   497,   174,   499,    44,    45,    46,    47,
      73,    49,    50,    51,    52,     3,   316,   185,   602,   603,
     460,   102,   165,   166,   108,   760,    42,   108,   468,    44,
      45,    46,    47,   799,    49,    50,    51,    52,    90,   479,
      44,    74,   459,   460,   385,    74,    62,   636,   610,   101,
     490,   468,   752,   191,   635,    72,    44,   230,    62,   111,
     198,     1,   479,   201,   202,   203,   204,   205,    46,    47,
     487,   371,   489,   490,    62,   841,   490,   418,     1,   252,
     497,   254,   499,   712,   713,   426,   427,   428,   429,   430,
     431,   432,   433,   636,   388,    73,   105,   186,   439,   108,
     752,   153,    88,    89,   839,   840,   636,   102,    43,   824,
     604,    69,    43,    48,    74,   409,   610,    48,    76,   785,
      78,   615,     3,   659,   636,   860,   861,   862,   785,    88,
      89,    58,   610,     3,     4,    46,    47,   615,   276,   277,
      67,   435,    46,    47,   438,   439,   102,    66,   102,    84,
      90,     3,   108,    84,   648,   649,    75,    88,    89,   102,
     460,   101,    73,    44,   658,   659,   660,    90,   468,    73,
     208,   209,   210,   211,   487,     1,   102,    66,   101,   479,
     109,    62,   113,   128,    64,    66,    75,   604,   174,    72,
     490,   177,    44,   610,    75,    65,   636,   183,    68,   169,
     170,    71,    72,   101,   102,   378,   825,   380,   153,    79,
      62,   109,     3,   153,    66,   174,   161,   105,   177,   636,
     108,   161,   360,    75,   183,   363,   709,    46,    47,   649,
     153,   648,   107,   102,   728,   747,   772,    72,   161,   108,
     660,   658,   659,   174,    46,    47,   177,    46,    47,     3,
       4,     3,   183,     3,    73,   596,   597,   598,   599,   600,
     601,     3,   300,   301,    90,   303,   304,   761,   562,   101,
     102,    73,    60,    61,    73,   101,   207,   109,   772,   773,
     774,   102,    90,   777,   778,   300,   301,   108,   303,   304,
     101,   102,    22,   101,    24,    72,   105,   283,   109,   593,
     594,   109,   174,   112,   798,    35,    36,   790,   728,    63,
     804,    65,   101,   122,   123,   102,   125,    71,    72,   128,
     128,   108,   816,   102,   283,    79,    80,   153,   102,    83,
     139,   139,   108,   142,     3,   161,   636,   102,     3,   107,
     108,   761,   836,   108,   838,   153,    15,    16,   842,   843,
      15,    16,   283,   773,   774,   772,   102,   777,   778,    90,
     174,   105,   108,    28,   108,   848,   101,   102,    99,   100,
     864,   712,   713,    72,   109,    44,   185,   871,   872,   873,
     874,   798,   105,     0,   804,   108,   506,   507,     5,     6,
       7,     8,     9,    62,    11,   533,   816,    14,   536,   102,
     101,   102,   508,   509,   112,   108,    75,     3,   109,   514,
     515,    89,     3,     4,   122,   123,   836,   125,   838,     3,
     128,    97,   842,   843,   101,    16,   103,   108,    45,    65,
     111,   139,   109,    69,   142,    98,   110,   171,   369,   105,
      76,   195,    78,    79,   864,   154,   112,   102,    44,   487,
     105,   871,   872,   873,   874,   111,   122,   123,     3,   125,
      44,     1,   128,    10,     3,   107,    62,     3,     3,    72,
      66,     3,    63,   139,    65,    80,   142,   185,    62,    75,
      71,    72,    66,    90,   196,   162,   163,   164,    79,    80,
     101,    75,    83,   153,   101,   112,   113,   114,   115,   191,
     102,   118,   110,     3,   121,   122,   123,   124,   125,    90,
     127,   128,   129,   130,   131,     3,     3,   134,   135,   185,
     101,   128,   139,   102,     3,   142,     3,    65,   109,   460,
       3,    69,   139,   510,   511,   512,   513,   468,    76,    77,
      78,    79,   102,    23,   161,    25,   153,   128,   102,    89,
      90,    91,    92,    93,    94,    35,    36,   105,   139,   105,
     101,   101,   105,    72,   101,   105,   106,   107,   185,     1,
     101,   101,   112,   113,   114,   115,   116,   194,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   195,    48,    49,    50,    51,   111,
     108,   105,   111,   153,   154,   155,   156,   157,   158,   111,
     160,   161,    65,   163,   164,    67,    69,    70,   102,    73,
      73,     3,    67,    76,    77,    78,    79,    80,    29,    30,
      31,    32,    33,    34,   101,   185,    17,    18,    19,    20,
      21,   191,   192,   193,   194,     1,   111,    89,    90,    91,
      92,    93,    94,    17,    18,    19,    20,    21,   105,   101,
     191,   105,   105,   105,   106,   143,   102,   102,   105,   101,
     112,   113,   114,   115,   116,    73,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   102,    48,    49,    50,    51,     3,     1,   105,
     105,   153,   154,   155,   156,   157,   158,   105,   160,   161,
      65,   163,   164,   117,    69,    70,   105,   102,   102,   102,
     102,    76,    77,    78,    79,    80,    31,   141,   374,   704,
     162,   405,   241,   185,   197,   198,   793,   418,   516,   191,
     192,   193,   194,    90,   517,   521,   112,   113,   114,   115,
     518,   490,   118,   280,   101,   121,   122,   123,   124,   125,
     519,   127,   128,   129,   130,   131,   798,   216,   134,   135,
     479,   369,   367,   139,   361,   152,   142,     1,   395,   249,
      -1,   128,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,   139,    -1,    -1,   161,    -1,    -1,   101,    -1,
      -1,    -1,   105,   106,   107,    -1,   153,   173,    -1,    -1,
      -1,    -1,    -1,   116,   161,    -1,   119,   120,    -1,   185,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,   194,   132,
     133,    -1,    -1,   136,   137,   138,    -1,   140,   141,    -1,
     143,    -1,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,    -1,   160,   161,    -1,
     163,   164,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,   105,   106,     1,     3,     4,    -1,    -1,   191,   192,
     193,    -1,   116,    -1,    -1,   119,   120,    16,    -1,    -1,
      -1,    -1,   126,    -1,     1,    -1,    -1,    90,   132,   133,
      -1,    -1,   136,   137,   138,    -1,   140,   141,   101,   143,
      -1,    -1,    -1,    -1,    -1,     1,   109,    -1,    -1,   153,
     154,   155,   156,   157,   158,    -1,   160,   161,    -1,   163,
     164,    -1,    -1,    -1,    63,   128,    65,    -1,    -1,    -1,
      -1,    -1,    71,    72,    -1,    -1,   139,    -1,    -1,    -1,
      79,    80,    -1,    -1,    83,    -1,    -1,   191,   192,   193,
     153,    89,    90,    91,    92,    93,    94,    65,   161,    -1,
      -1,    69,    70,   101,    -1,    -1,    -1,   105,    76,    77,
      78,    79,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
     138,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,   153,   154,   155,   156,   157,
     158,   138,   160,    -1,    -1,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,   138,   160,    -1,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,   191,   192,    -1,   195,   153,   154,   155,
     156,   157,   158,    -1,   160,    -1,    -1,   163,   164,    -1,
      -1,    -1,    -1,    -1,   191,   192,     5,     6,     7,     8,
       9,    -1,    11,    -1,    -1,    14,    -1,    -1,   112,   113,
     114,    -1,    -1,    -1,   118,   191,   192,    -1,   122,   123,
     124,   125,    90,    -1,   128,   129,   130,   131,    -1,    -1,
     134,   135,    -1,   101,     1,   139,    45,    90,   142,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   101,    -1,
     118,    -1,    -1,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,    -1,    65,   134,   135,    -1,    69,
      70,   139,    -1,    73,   142,   128,    76,    77,    78,    79,
      80,   185,    -1,    65,    -1,   153,   139,    69,    70,    -1,
      -1,    -1,    -1,   161,    76,    77,    78,    79,    80,    -1,
     153,    -1,    -1,   112,   113,   114,   115,    -1,   161,   118,
      -1,    -1,   121,   122,   123,   124,   125,   185,   127,   128,
     129,   130,   131,    90,    -1,   134,   135,    -1,    -1,    -1,
     139,    -1,    -1,   142,   101,    -1,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,
      -1,   118,   161,    -1,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    40,    41,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,   142,   185,    -1,    -1,    -1,
      -1,    56,    57,    -1,    59,   194,   153,    89,    90,    91,
      92,    93,    94,    -1,   161,    -1,    -1,    -1,    -1,   101,
      75,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,    -1,    -1,   118,    -1,   185,   121,
      -1,    -1,   124,    -1,    -1,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,    -1,    -1,   138,   139,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,    -1,
      -1,   153,   154,   155,   156,   157,   158,    -1,   160,   161,
      -1,   163,   164,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   185,    -1,   112,   113,   114,   115,   191,
     192,   118,   194,    -1,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,    -1,
      -1,    -1,   139,    -1,    -1,   142,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   101,   102,    -1,
      -1,    -1,    -1,    -1,   161,   109,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   173,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   185,    -1,
     134,   135,    -1,    -1,    -1,   139,    -1,   194,   142,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
     101,   102,    -1,    -1,    -1,    -1,    -1,   161,   109,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,   173,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   185,    -1,   134,   135,    -1,    -1,    -1,   139,    -1,
     194,   142,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
     161,    -1,   106,   107,    -1,    -1,    89,    90,    91,    92,
      93,    94,   173,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,   185,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,   194,   138,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,   101,   153,
     154,   155,   156,   157,   158,   138,   160,   110,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,    -1,
     163,   164,    -1,    -1,    -1,   138,    -1,   191,   192,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,    -1,   160,   191,   192,
     163,   164,    89,    90,    91,    92,    93,    94,    40,    41,
      -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    56,    57,    -1,    59,   191,   192,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    75,   101,    -1,    -1,    -1,    -1,   106,
      -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,    -1,   160,    -1,    -1,   163,   164,    -1,    -1,
      -1,   138,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,   101,   102,   153,   154,   155,   156,
     157,   158,    -1,   160,   191,   192,   163,   164,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    29,    30,
      31,    32,    33,    34,   101,    89,    90,    91,    92,    93,
      94,   138,    -1,    -1,   191,   192,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   154,   155,   156,
     157,   158,    -1,   160,    65,    -1,   163,   164,    69,    70,
      -1,   138,    -1,    -1,    -1,    76,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,   138,    -1,   153,   154,   155,   156,
     157,   158,    -1,   160,   191,   192,   163,   164,    -1,   153,
     154,   155,   156,   157,   158,    -1,   160,    -1,    -1,   163,
     164,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,   101,   191,   192,    -1,    -1,   113,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   191,   192,   124,
      -1,    -1,    -1,   128,   129,   130,   131,    -1,    -1,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,   154,   155,   156,   157,
     158,    -1,   160,    -1,    90,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
     185,    -1,    -1,   109,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,   118,   191,   192,   121,   122,   123,   124,   125,
      90,   127,   128,   129,   130,   131,    -1,    -1,   134,   135,
      -1,   101,    -1,   139,    -1,    -1,   142,    -1,    -1,    -1,
      -1,   111,    -1,   113,   114,   115,    -1,   153,   118,    -1,
      -1,   121,    -1,    -1,   124,   161,    -1,   127,   128,   129,
     130,   131,    90,    -1,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,   185,
      -1,   109,    -1,   153,    -1,   113,   114,   115,    -1,    -1,
     118,   161,    -1,   121,    -1,    -1,   124,    -1,    -1,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,    -1,   102,
      -1,   139,    -1,    -1,    -1,   185,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,   118,    -1,    -1,   121,   122,
     123,   124,   125,   161,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,
     153,    -1,   102,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     173,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   185,    -1,   134,   135,    -1,    -1,    -1,   139,
      -1,   194,   142,   107,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,
     124,   161,    -1,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,    -1,   173,    -1,   139,    -1,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,   185,    -1,   113,   114,   115,
      -1,    -1,   118,    -1,   194,   121,    -1,   161,   124,    -1,
      -1,   127,   128,   129,   130,   131,    -1,    -1,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,   107,    -1,    -1,
      -1,   185,    -1,   113,   114,   115,    -1,    -1,   118,    -1,
     194,   121,    -1,    -1,   124,   161,    -1,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,   185,
      -1,   113,   114,   115,    -1,    -1,   118,    -1,   194,   121,
      -1,   161,   124,    -1,    -1,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   112,   113,   114,   115,   161,
      -1,   118,    -1,    -1,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,    -1,
      -1,    -1,   139,   185,     3,   142,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    -1,   161,    -1,    -1,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     5,     6,     7,     8,     9,    11,    14,    45,   112,
     113,   114,   115,   118,   121,   122,   123,   124,   125,   127,
     128,   129,   130,   131,   134,   135,   139,   142,   161,   185,
     194,   200,   201,   202,   203,   204,   210,   211,   220,   233,
     260,   261,   270,   279,   280,   281,   282,   283,   285,   286,
     287,   288,   289,   291,   325,   326,   335,   344,   352,   354,
     355,   357,   360,     3,     3,     3,     3,     3,    62,    74,
     218,   235,     3,     3,   106,   153,   311,   356,   357,    72,
       0,   201,    74,   105,   108,   105,   108,     1,    90,   101,
     292,   293,   294,   296,   297,   298,   304,   305,   306,   307,
     308,   309,   356,   357,   284,   352,   354,   355,   105,   284,
     284,     1,    90,   101,   284,   291,   304,   307,   355,   357,
       1,   292,   352,   354,   355,   105,   352,   354,   352,   354,
       1,   291,   304,   352,   354,   355,   357,   106,   311,   260,
      64,    64,    43,    74,     3,    44,   217,   218,    72,    74,
     311,   336,   106,   191,    12,    13,    40,    41,    56,    57,
      59,    75,   212,   213,   214,   219,   220,   229,   232,     1,
     292,     1,   304,   268,   101,   128,   139,   290,   293,   296,
     304,   308,   353,   101,   293,   295,   297,   305,   307,   308,
     357,   262,   101,   109,   302,   303,   101,   302,   277,   290,
     307,   271,   269,   264,   278,   273,   107,   285,   286,   287,
     288,   289,   327,   328,   329,   330,   106,    22,    24,    35,
      36,   205,    23,    25,   205,    42,   218,   217,     3,   235,
      44,    66,    75,   218,    73,   234,   235,     3,    15,    16,
      28,   221,   222,   225,   226,   174,   337,   108,   338,   336,
      73,     3,    74,     3,    74,    74,    74,     3,     4,    16,
      63,    65,    71,    72,    79,    80,    83,   195,   230,   231,
     237,    16,   230,    72,    75,   213,   266,   275,   186,   313,
     314,   315,   295,   101,   293,   296,   304,   308,   353,   295,
     102,   302,   102,   102,   102,   102,   313,   102,   173,   279,
     281,   283,   285,   287,   289,   310,   322,   323,   324,   356,
      89,    90,    91,    92,    93,    94,   101,   110,   138,   154,
     155,   156,   157,   158,   160,   163,   164,   191,   192,   238,
     239,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   258,   350,   351,   356,   358,
     109,   313,   313,   313,   313,   313,   313,   111,   292,   331,
     334,   304,   332,   334,   107,   328,   105,   108,   105,   108,
     107,   327,    74,   207,   207,   218,    75,   235,    44,   218,
      66,    73,     3,    53,    54,    55,    74,   226,    72,     3,
      75,   222,    75,   226,   258,   311,   107,   338,   234,   234,
      75,   217,    15,    16,    75,   215,   216,   217,    64,    72,
       3,   237,   237,   237,   237,     3,     3,     3,    58,    67,
      29,    30,    31,    32,    33,    34,    65,    69,    70,    76,
      77,    78,    79,    80,   228,    72,    67,     3,    68,    72,
     223,   224,   236,   237,   313,   313,   101,   174,   319,   315,
     102,   295,   102,   302,   102,   302,   302,   302,   263,    90,
     101,   296,   299,   300,   301,   302,   304,    90,   101,   299,
     304,   296,   299,   304,   299,   304,   102,   108,   102,   108,
     174,   106,   241,   254,   255,   257,   285,   289,   312,   342,
     101,   241,   101,   241,   241,    72,   101,   103,   109,   162,
     163,   164,   243,    90,    99,   100,    91,    92,   165,   166,
      95,    96,   167,   168,   169,   170,    89,    98,    97,   171,
     104,   172,   110,   154,   110,   258,   319,   272,   319,   265,
     319,   274,   258,   333,   334,   313,   333,   313,   332,   331,
     107,     3,   208,   209,    10,   206,   206,   235,   235,     3,
       3,     3,   237,   225,   223,   337,   107,    75,    75,    75,
      74,   218,    72,    75,   216,    75,     3,   223,    73,    80,
     196,   231,   237,   237,   237,   237,   237,   237,   237,   237,
     223,     3,    26,    27,    37,    38,    39,    52,    64,   228,
     223,   223,   237,    46,    47,    73,    48,    49,    50,    51,
     197,   198,   267,   276,   101,   106,   255,   320,   102,   319,
     290,   299,   300,   301,   302,   290,   356,     1,   173,   324,
     320,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   256,   102,   108,    90,   101,   299,   299,   102,
       1,   116,   119,   120,   126,   132,   133,   136,   137,   140,
     141,   143,   193,   257,   259,   339,   340,   341,   342,   344,
     345,   346,   347,   348,   349,   356,   357,   359,   312,   191,
     102,   240,   255,   356,   357,   257,   356,   357,   243,   243,
     243,   244,   244,   245,   245,   246,   246,   246,   246,   247,
     247,   248,   249,   250,   251,   257,   252,   110,   319,   319,
     319,   313,   313,   207,    66,    75,     3,    75,    73,   217,
     223,   228,    73,    73,     3,     3,     3,     3,     3,    17,
      18,    19,    20,    21,   227,    73,   223,   223,    60,    61,
     237,   237,   237,   237,   237,   237,   319,   319,   128,   316,
     317,   318,   356,   357,   320,   321,   299,   102,   102,   102,
     255,   255,   290,   243,   105,   105,   101,   258,   259,   105,
     101,   111,   356,   357,   339,   357,   101,   101,    72,   105,
     107,   343,   344,   345,   345,   339,   343,   111,   111,    67,
     102,    73,   102,   108,   110,   111,   209,    74,   226,    75,
      73,   230,   230,   228,     3,   339,    67,   102,   108,   101,
     107,   108,   302,   257,   111,   105,     1,   259,   339,   105,
     105,   143,   257,   257,   191,   343,   345,   343,   343,   102,
     339,   339,   255,   254,   225,    74,   218,    67,    67,    21,
     227,   102,   317,   257,   107,   320,   102,   339,   102,   105,
     105,   101,   102,   102,    73,   343,   102,    75,   217,     3,
     102,   339,   339,   259,     1,   259,   257,   339,   339,    75,
     105,   105,   105,   102,   117,   259,   259,     1,   259,   105,
     339,   102,   102,   102,   102,   339,   339,   339,   339
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#define YYLEX	yylex ()

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;


int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 6:
#line 496 "language.y"
    { Annotations::Enabled_properties.push_back(yyvsp[0].t_id->name()); }
    break;

  case 7:
#line 498 "language.y"
    { Annotations::Disabled_properties.push_back(yyvsp[0].t_id->name()); }
    break;

  case 11:
#line 503 "language.y"
    {
        Annotations::Current->init()->add_analysis(yyvsp[0].t_analyze);
      }
    break;

  case 12:
#line 510 "language.y"
    { Annotations::Current->store_header(yyvsp[0].declL); }
    break;

  case 13:
#line 517 "language.y"
    {
        Annotations::Current->add_globals( yyvsp[-1].t_structures );
        delete yyvsp[-1].t_structures;
      }
    break;

  case 14:
#line 523 "language.y"
    {
        structuretree_list * temp = new structuretree_list();
	temp->push_back( yyvsp[0].t_structure);
        Annotations::Current->add_globals( temp );
      }
    break;

  case 15:
#line 534 "language.y"
    {
        Annotations::Current->add_enum_property(new enumPropertyAnn(yyvsp[-4].t_id, yyvsp[-2].t_direction, true, yyvsp[0].t_id, yyvsp[-1].t_enumvalue_list));
	delete yyvsp[-4].t_id;
	delete yyvsp[-1].t_enumvalue_list;
	delete yyvsp[0].t_id;
      }
    break;

  case 16:
#line 542 "language.y"
    {
        Annotations::Current->add_enum_property(new enumPropertyAnn(yyvsp[-4].t_id, yyvsp[-2].t_direction, false, yyvsp[0].t_id, yyvsp[-1].t_enumvalue_list));
	delete yyvsp[-4].t_id;
	delete yyvsp[-1].t_enumvalue_list;
	delete yyvsp[0].t_id;
      }
    break;

  case 17:
#line 550 "language.y"
    {
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Union));
	delete yyvsp[-2].t_id;
      }
    break;

  case 18:
#line 558 "language.y"
    {
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Intersect));
	delete yyvsp[-2].t_id;
      }
    break;

  case 19:
#line 566 "language.y"
    {
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Union));
	delete yyvsp[-2].t_id;
      }
    break;

  case 20:
#line 574 "language.y"
    {
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Intersect));
	delete yyvsp[-2].t_id;
      }
    break;

  case 21:
#line 584 "language.y"
    {
      yyval.t_direction = Forward;
    }
    break;

  case 22:
#line 589 "language.y"
    {
      yyval.t_direction = Forward;
    }
    break;

  case 23:
#line 594 "language.y"
    {
      yyval.t_direction = Backward;
    }
    break;

  case 24:
#line 601 "language.y"
    {
        yyval.t_id = (parserID *)0;
      }
    break;

  case 25:
#line 606 "language.y"
    {
	yyval.t_id = yyvsp[0].t_id;
      }
    break;

  case 26:
#line 613 "language.y"
    {
	yyval.t_enumvalue_list = yyvsp[-1].t_enumvalue_list;
      }
    break;

  case 27:
#line 620 "language.y"
    {
        yyval.t_enumvalue_list = new enumvalue_list();
	yyval.t_enumvalue_list->push_back(yyvsp[0].t_enumvalue);
      }
    break;

  case 28:
#line 626 "language.y"
    {
	yyval.t_enumvalue_list = yyvsp[-2].t_enumvalue_list;
	yyval.t_enumvalue_list->push_back(yyvsp[0].t_enumvalue);
      }
    break;

  case 29:
#line 634 "language.y"
    {
        yyval.t_enumvalue = new enumValueAnn(yyvsp[0].t_id, (enumvalue_list *) 0);
	delete yyvsp[0].t_id;
      }
    break;

  case 30:
#line 640 "language.y"
    {
	yyval.t_enumvalue = new enumValueAnn(yyvsp[-1].t_id, yyvsp[0].t_enumvalue_list);
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_enumvalue_list;
      }
    break;

  case 31:
#line 651 "language.y"
    {
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      }
    break;

  case 32:
#line 657 "language.y"
    {
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      }
    break;

  case 33:
#line 665 "language.y"
    {
        procedureAnn::Current = new procedureAnn(yyvsp[-3].t_id, yyvsp[-1].t_ids, Annotations::Current, yyvsp[-4].t_tok);
        delete yyvsp[-3].t_id;
      }
    break;

  case 34:
#line 671 "language.y"
    {
        procedureAnn::Current = new procedureAnn(yyvsp[-2].t_id, (parserid_list *)0, Annotations::Current, yyvsp[-3].t_tok);
        delete yyvsp[-2].t_id;
      }
    break;

  case 37:
#line 684 "language.y"
    {
      }
    break;

  case 38:
#line 688 "language.y"
    {
      }
    break;

  case 39:
#line 692 "language.y"
    {
	procedureAnn::Current->add_analysis(yyvsp[0].t_analyze);
      }
    break;

  case 40:
#line 697 "language.y"
    {
      }
    break;

  case 41:
#line 701 "language.y"
    {
      }
    break;

  case 42:
#line 709 "language.y"
    {
        procedureAnn::Current->add_on_entry(yyvsp[-1].t_structures);
      }
    break;

  case 43:
#line 714 "language.y"
    {
        procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, yyvsp[-1].t_structures, yyvsp[-3].t_tok));
      }
    break;

  case 44:
#line 719 "language.y"
    {
      }
    break;

  case 45:
#line 723 "language.y"
    {
      }
    break;

  case 46:
#line 727 "language.y"
    {
      }
    break;

  case 49:
#line 738 "language.y"
    {
	procedureAnn::Current->add_on_exit(new pointerRuleAnn(yyvsp[-4].t_expr, yyvsp[-1].t_structures, yyvsp[-6].t_tok));
      }
    break;

  case 50:
#line 743 "language.y"
    {
	structuretree_list * temp = new structuretree_list();
	temp->push_back(yyvsp[0].t_structure);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn(yyvsp[-2].t_expr, temp, yyvsp[-4].t_tok));
      }
    break;

  case 51:
#line 750 "language.y"
    {
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, yyvsp[-1].t_structures, yyvsp[-3].t_tok));
      }
    break;

  case 52:
#line 755 "language.y"
    {
	structuretree_list * temp = new structuretree_list();
	temp->push_back(yyvsp[0].t_structure);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, temp, yyvsp[-1].t_tok));
      }
    break;

  case 53:
#line 765 "language.y"
    {
        yyval.t_structures = new structuretree_list();
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      }
    break;

  case 54:
#line 771 "language.y"
    {
	yyval.t_structures = yyvsp[-1].t_structures;
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      }
    break;

  case 55:
#line 777 "language.y"
    {
	yyval.t_structures = yyvsp[-2].t_structures;
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      }
    break;

  case 56:
#line 783 "language.y"
    { 
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;

        /* "delete" doesn't actually add anything to the structure tree, so
           we just pass an empty list back. Similarly for the next two
           productions. */

        yyval.t_structures = new structuretree_list();
      }
    break;

  case 57:
#line 795 "language.y"
    {
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
	yyval.t_structures = yyvsp[-2].t_structures;
      }
    break;

  case 58:
#line 802 "language.y"
    {
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
	yyval.t_structures = yyvsp[-3].t_structures;
      }
    break;

  case 59:
#line 811 "language.y"
    {
        yyval.t_structure = new structureTreeAnn(yyvsp[0].t_id, (structuretree_list *)0,
                                  structureTreeAnn::None, false);
	delete yyvsp[0].t_id;
      }
    break;

  case 60:
#line 818 "language.y"
    {
        yyval.t_structure = new structureTreeAnn(yyvsp[-2].t_id, yyvsp[0].t_structure, structureTreeAnn::Arrow, false);
	delete yyvsp[-2].t_id;
      }
    break;

  case 61:
#line 824 "language.y"
    {
        yyval.t_structure = new structureTreeAnn(yyvsp[-3].t_id, yyvsp[0].t_structure, structureTreeAnn::Arrow, true);
	delete yyvsp[-3].t_id;
      }
    break;

  case 62:
#line 830 "language.y"
    {
        yyval.t_structure = new structureTreeAnn(yyvsp[-3].t_id, yyvsp[-1].t_structures, structureTreeAnn::Dot, false);
	delete yyvsp[-3].t_id;
      }
    break;

  case 63:
#line 841 "language.y"
    {
        procedureAnn::Current->add_uses( yyvsp[-1].t_ids );
	delete yyvsp[-1].t_ids;
      }
    break;

  case 64:
#line 847 "language.y"
    {
        parserid_list temp;
	temp.push_back( * yyvsp[0].t_id);
        procedureAnn::Current->add_uses( & temp );
	delete yyvsp[0].t_id;
      }
    break;

  case 65:
#line 855 "language.y"
    {
        procedureAnn::Current->add_defs( yyvsp[-1].t_ids );
	delete yyvsp[-1].t_ids;
      }
    break;

  case 66:
#line 861 "language.y"
    {
        parserid_list temp;
	temp.push_back( * yyvsp[0].t_id);
        procedureAnn::Current->add_defs( & temp );
	delete yyvsp[0].t_id;
      }
    break;

  case 67:
#line 874 "language.y"
    {
	yyval.t_analyze = new analyzeAnn(Annotations::Current, yyvsp[-3].t_id, yyvsp[-1].t_rules, yyvsp[-4].t_tok);
	delete yyvsp[-3].t_id;
      }
    break;

  case 68:
#line 880 "language.y"
    {
	rule_list * temp_rules = new rule_list();
	temp_rules->push_back(new ruleAnn((exprAnn *)0, yyvsp[-1].t_exprs, yyvsp[-2].t_tok));
	yyval.t_analyze = new analyzeAnn(Annotations::Current, yyvsp[-3].t_id, temp_rules, yyvsp[-4].t_tok);
	delete yyvsp[-3].t_id;
      }
    break;

  case 69:
#line 890 "language.y"
    {
	yyval.t_rules = new rule_list();
	yyval.t_rules->push_back(yyvsp[0].t_rule);
      }
    break;

  case 70:
#line 896 "language.y"
    {
	yyval.t_rules = yyvsp[-1].t_rules;
	yyval.t_rules->push_back(yyvsp[0].t_rule);
      }
    break;

  case 71:
#line 904 "language.y"
    {
	yyval.t_rule = new ruleAnn(yyvsp[-4].t_expr, yyvsp[-1].t_exprs, yyvsp[-6].t_tok);
      }
    break;

  case 72:
#line 909 "language.y"
    {
	exprann_list * temp = new exprann_list();
	temp->push_back(yyvsp[0].t_expr);
	yyval.t_rule = new ruleAnn(yyvsp[-2].t_expr, temp, yyvsp[-4].t_tok);
      }
    break;

  case 73:
#line 916 "language.y"
    {
	yyval.t_rule = new ruleAnn((exprAnn *)0, yyvsp[-1].t_exprs, yyvsp[-3].t_tok);
      }
    break;

  case 74:
#line 921 "language.y"
    {
	exprann_list * temp = new exprann_list();
	temp->push_back(yyvsp[0].t_expr);
	yyval.t_rule = new ruleAnn((exprAnn *)0, temp, yyvsp[-1].t_tok);
      }
    break;

  case 75:
#line 931 "language.y"
    {
        yyval.t_expr = yyvsp[0].t_expr;
      }
    break;

  case 76:
#line 936 "language.y"
    {
        yyval.t_expr = new connectiveExprAnn(Broadway::Or, yyvsp[-2].t_expr, yyvsp[0].t_expr, yyvsp[-1].t_tok);
      }
    break;

  case 77:
#line 941 "language.y"
    {
        yyval.t_expr = new connectiveExprAnn(Broadway::And, yyvsp[-2].t_expr, yyvsp[0].t_expr, yyvsp[-1].t_tok);
      }
    break;

  case 78:
#line 946 "language.y"
    {
        yyval.t_expr = new connectiveExprAnn(Broadway::Not, yyvsp[0].t_expr, (exprAnn *)0, yyvsp[-1].t_tok);
      }
    break;

  case 79:
#line 951 "language.y"
    {
        yyval.t_expr = yyvsp[-1].t_expr;
      }
    break;

  case 80:
#line 960 "language.y"
    {
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-1].t_property_sensitivity.fs, yyvsp[-4].t_id, yyvsp[-2].t_id, Broadway::Is_Bottom,
				     (const parserID *)0, yyvsp[-1].t_property_sensitivity.line);
	delete yyvsp[-4].t_id;
	delete yyvsp[-2].t_id;
      }
    break;

  case 81:
#line 968 "language.y"
    {
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-1].t_property_sensitivity.fs, (const parserID *)0, yyvsp[-2].t_id, Broadway::Is_Bottom,
				     (const parserID *)0, yyvsp[-1].t_property_sensitivity.line);
	delete yyvsp[-2].t_id;
      }
    break;

  case 82:
#line 975 "language.y"
    {
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-2].t_property_sensitivity.fs, yyvsp[-5].t_id, yyvsp[-3].t_id, yyvsp[-1].t_property_operator.op, yyvsp[0].t_id, yyvsp[-1].t_property_operator.line);
	delete yyvsp[-5].t_id;
	delete yyvsp[-3].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 83:
#line 983 "language.y"
    {
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-2].t_property_sensitivity.fs, (const parserID *)0, yyvsp[-3].t_id, yyvsp[-1].t_property_operator.op, yyvsp[0].t_id, yyvsp[-1].t_property_operator.line);
	delete yyvsp[-3].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 84:
#line 992 "language.y"
    {
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[-1].t_id, yyvsp[-2].t_id, Broadway::Is_Equivalent, yyvsp[0].t_id, yyvsp[-2].t_id->line());
	delete yyvsp[-2].t_id;
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 85:
#line 1000 "language.y"
    {
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[0].t_id, yyvsp[-2].t_id, Broadway::Is_Element_Of,
				    (const parserID *)0, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 86:
#line 1008 "language.y"
    {
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[-1].t_id, (const parserID *)0,
                                    Broadway::Is_EmptySet, (const parserID *)0, yyvsp[0].t_tok);

	delete yyvsp[-1].t_id;
      }
    break;

  case 87:
#line 1018 "language.y"
    {
        yyval.t_expr = new numericExprAnn(yyvsp[0].t_c_expression, annlineno);
      }
    break;

  case 88:
#line 1023 "language.y"
    {
        yyval.t_expr = new numericExprAnn(yyvsp[-1].t_id, annlineno);
      }
    break;

  case 89:
#line 1030 "language.y"
    {
        yyval.t_expr = new pointerExprAnn(Broadway::Is_AliasOf, yyvsp[-2].t_id, yyvsp[0].t_id);
        delete yyvsp[-2].t_id;
        delete yyvsp[0].t_id;
      }
    break;

  case 90:
#line 1037 "language.y"
    {
        yyval.t_expr = new pointerExprAnn(Broadway::Is_SameAs, yyvsp[-2].t_id, yyvsp[0].t_id);
        delete yyvsp[-2].t_id;
        delete yyvsp[0].t_id;
      }
    break;

  case 91:
#line 1044 "language.y"
    {
        yyval.t_expr = new pointerExprAnn(Broadway::Is_Empty, yyvsp[-1].t_id, (const parserID *)0);
        delete yyvsp[-1].t_id;
      }
    break;

  case 92:
#line 1053 "language.y"
    {
	yyval.t_exprs = new exprann_list();
	yyval.t_exprs->push_back(yyvsp[0].t_expr);
      }
    break;

  case 93:
#line 1059 "language.y"
    {
	yyval.t_exprs = yyvsp[-1].t_exprs;
	yyval.t_exprs->push_back(yyvsp[0].t_expr);
      }
    break;

  case 94:
#line 1068 "language.y"
    {
	yyval.t_expr = new numericExprAnn(yyvsp[-2].t_id, yyvsp[0].t_c_expression, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
      }
    break;

  case 95:
#line 1074 "language.y"
    {
        yyval.t_expr = new enumPropertyExprAnn(Broadway::None,
				     (const parserID *)0, yyvsp[-2].t_id, Broadway::Assign,
				     yyvsp[0].t_id, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 96:
#line 1083 "language.y"
    {
        enumPropertyExprAnn * tmp = new enumPropertyExprAnn(Broadway::None,
							    (const parserID *)0, yyvsp[-2].t_id, Broadway::Assign,
							    yyvsp[0].t_id, yyvsp[-1].t_tok);
	tmp->set_weak();
	yyval.t_expr = tmp;
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 97:
#line 1094 "language.y"
    {
        yyval.t_expr = new setPropertyExprAnn(Broadway::None,
				    (const parserID *)0, yyvsp[0].t_id, Broadway::Add_Elements,
				    (const parserID *)0, yyvsp[-1].t_tok);
	delete yyvsp[0].t_id;
      }
    break;

  case 98:
#line 1102 "language.y"
    {
        yyval.t_expr = new setPropertyExprAnn(Broadway::None,
				    yyvsp[-1].t_id, yyvsp[-2].t_id, Broadway::Add_Equivalences, yyvsp[0].t_id, yyvsp[-2].t_id->line());
	delete yyvsp[-2].t_id;
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_id;
      }
    break;

  case 99:
#line 1114 "language.y"
    { yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_Exactly; }
    break;

  case 100:
#line 1118 "language.y"
    { yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_AtLeast; }
    break;

  case 101:
#line 1122 "language.y"
    { yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Could_Be; }
    break;

  case 102:
#line 1126 "language.y"
    { yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_AtMost; }
    break;

  case 103:
#line 1133 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Before; }
    break;

  case 104:
#line 1137 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::After; }
    break;

  case 105:
#line 1141 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Always; }
    break;

  case 106:
#line 1145 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Ever; }
    break;

  case 107:
#line 1149 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Trace; }
    break;

  case 108:
#line 1153 "language.y"
    { yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Confidence; }
    break;

  case 109:
#line 1157 "language.y"
    { yyval.t_property_sensitivity.line =  0;
         yyval.t_property_sensitivity.fs = Broadway::Before; }
    break;

  case 110:
#line 1166 "language.y"
    {
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, false, yyvsp[-1].t_report_elements, yyvsp[-2].t_tok));
       }
    break;

  case 111:
#line 1171 "language.y"
    {
         procedureAnn::Current->add_report(new reportAnn(yyvsp[-3].t_expr, false, yyvsp[-1].t_report_elements, yyvsp[-6].t_tok));
       }
    break;

  case 112:
#line 1175 "language.y"
    {
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, true, yyvsp[-1].t_report_elements, yyvsp[-2].t_tok));
       }
    break;

  case 113:
#line 1180 "language.y"
    {
         procedureAnn::Current->add_report(new reportAnn(yyvsp[-3].t_expr, true, yyvsp[-1].t_report_elements, yyvsp[-6].t_tok));
       }
    break;

  case 114:
#line 1188 "language.y"
    {
         yyval.t_report_elements = new report_element_list();
         yyval.t_report_elements->push_back( yyvsp[0].t_report_element );
       }
    break;

  case 115:
#line 1194 "language.y"
    {
         yyval.t_report_elements = yyvsp[-2].t_report_elements;
         yyval.t_report_elements->push_back(yyvsp[0].t_report_element);
       }
    break;

  case 116:
#line 1204 "language.y"
    {
         yyval.t_report_element = new literalReportElementAnn(yyvsp[0].t_id);
         delete yyvsp[0].t_id;
       }
    break;

  case 117:
#line 1210 "language.y"
    {
        yyval.t_report_element = new expressionReportElementAnn(yyvsp[0].t_property_sensitivity.fs,
	                                    new enumPropertyExprAnn(yyvsp[0].t_property_sensitivity.fs, yyvsp[-3].t_id, yyvsp[-1].t_id, Broadway::Report,
                                                                (const parserID *)0, yyvsp[-3].t_id->line()),
                                            annlineno);
	delete yyvsp[-3].t_id;
	delete yyvsp[-1].t_id;
      }
    break;

  case 118:
#line 1220 "language.y"
    {
        yyval.t_report_element = new expressionReportElementAnn(yyvsp[0].t_property_sensitivity.fs,
                                            new numericExprAnn(yyvsp[-1].t_c_expression, yyvsp[0].t_property_sensitivity.line),
                                            annlineno);
      }
    break;

  case 119:
#line 1227 "language.y"
    {
        yyval.t_report_element = new locationReportElementAnn(yyvsp[0].t_id);
      }
    break;

  case 120:
#line 1232 "language.y"
    {
        yyval.t_report_element = new bindingReportElementAnn(yyvsp[-1].t_id, false);
      }
    break;

  case 121:
#line 1237 "language.y"
    {
        yyval.t_report_element = new bindingReportElementAnn(yyvsp[-1].t_id, true);
      }
    break;

  case 122:
#line 1246 "language.y"
    {
        procedureAnn::Current->add_action(new actionAnn(yyvsp[-3].t_expr, yyvsp[0].stmtN, yyvsp[-5].t_tok));
      }
    break;

  case 123:
#line 1251 "language.y"
    {
        procedureAnn::Current->add_action(new actionAnn(yyvsp[-3].t_expr, yyvsp[-5].t_tok));
      }
    break;

  case 124:
#line 1260 "language.y"
    {
      }
    break;

  case 125:
#line 1268 "language.y"
    { yyval.t_ids = new parserid_list();
        yyval.t_ids->push_back( * yyvsp[0].t_id );
	delete yyvsp[0].t_id;
      }
    break;

  case 126:
#line 1274 "language.y"
    { yyval.t_ids = yyvsp[-2].t_ids;
        yyval.t_ids->push_back( * yyvsp[0].t_id);
        delete yyvsp[0].t_id;
      }
    break;

  case 127:
#line 1282 "language.y"
    { yyval.t_id = yyvsp[0].t_id; }
    break;

  case 128:
#line 1285 "language.y"
    { yyval.t_id = yyvsp[0].t_id;
        yyval.t_id->set_io();
      }
    break;

  case 129:
#line 1295 "language.y"
    { yyval.t_c_expression = new binaryNode(Operator::EQ, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 130:
#line 1298 "language.y"
    { yyval.t_c_expression = new binaryNode(Operator::NE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 131:
#line 1301 "language.y"
    { yyval.t_c_expression = new binaryNode('<', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 132:
#line 1304 "language.y"
    { yyval.t_c_expression = new binaryNode(Operator::LE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 133:
#line 1307 "language.y"
    { yyval.t_c_expression = new binaryNode('>', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 134:
#line 1310 "language.y"
    { yyval.t_c_expression = new binaryNode(Operator::GE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 135:
#line 1316 "language.y"
    { yyval.t_c_expression = yyvsp[0].t_c_expression; }
    break;

  case 136:
#line 1319 "language.y"
    { yyval.t_c_expression = new idNodeAnn(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
      }
    break;

  case 137:
#line 1324 "language.y"
    { yyval.t_c_expression = yyvsp[-1].t_c_expression; yyval.t_c_expression->parenthesized(true); }
    break;

  case 138:
#line 1327 "language.y"
    { yyval.t_c_expression = new binaryNode('+', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 139:
#line 1330 "language.y"
    { yyval.t_c_expression = new binaryNode('-', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 140:
#line 1333 "language.y"
    { yyval.t_c_expression = new binaryNode('|', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 141:
#line 1336 "language.y"
    { yyval.t_c_expression = new binaryNode('^', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 142:
#line 1339 "language.y"
    { yyval.t_c_expression = new binaryNode('&', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 143:
#line 1342 "language.y"
    { yyval.t_c_expression = new binaryNode('*', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 144:
#line 1345 "language.y"
    { yyval.t_c_expression = new binaryNode('/', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 145:
#line 1348 "language.y"
    { yyval.t_c_expression = new binaryNode('%', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); }
    break;

  case 146:
#line 1351 "language.y"
    { yyval.t_c_expression = new unaryNode('-', yyvsp[0].t_c_expression); }
    break;

  case 147:
#line 1354 "language.y"
    { yyval.t_c_expression = new unaryNode('+', yyvsp[0].t_c_expression); }
    break;

  case 148:
#line 1357 "language.y"
    { yyval.t_c_expression = new unaryNode('~', yyvsp[0].t_c_expression); }
    break;

  case 149:
#line 1375 "language.y"
    { yyval.exprN = yyvsp[0].idN; }
    break;

  case 151:
#line 1381 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 152:
#line 1383 "language.y"
    { if (yyvsp[-1].exprN->typ() == Comma) yyvsp[-1].exprN->coord(Coord(yyvsp[-2].tok));
                                  yyvsp[-1].exprN->parenthesized(true);
                                  yyval.exprN = yyvsp[-1].exprN; }
    break;

  case 153:
#line 1390 "language.y"
    { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
               yyval.exprN = (exprNode *) /* This is BAD */
		 new blockNode((decl_list *)0, yyvsp[-2].stmtL, Coord(yyvsp[-4].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 154:
#line 1396 "language.y"
    { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
              yyval.exprN =  (exprNode *) /* This is BAD */
		new blockNode(yyvsp[-3].declL, yyvsp[-2].stmtL, Coord(yyvsp[-5].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 155:
#line 1401 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 156:
#line 1406 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 157:
#line 1409 "language.y"
    { /* I don't like this (SZG)...
		 if (->typ() == Index)
	        { indexNode * atmp = (indexNode *);
		  atmp->add_dim();
		  $$ = atmp; }
	      else */
	      { /* Only allow one dimension arg per indexNode */
		yyval.exprN = new binaryNode(Operator::Index, yyvsp[-3].exprN, yyvsp[-1].exprN, Coord(yyvsp[-2].tok));
	      }
	    }
    break;

  case 158:
#line 1421 "language.y"
    { yyval.exprN = new callNode(yyvsp[-2].exprN, (expr_list *)0, Coord(yyvsp[-1].tok)); }
    break;

  case 159:
#line 1424 "language.y"
    { yyval.exprN = new callNode(yyvsp[-3].exprN, yyvsp[-1].exprL, Coord(yyvsp[-2].tok)); }
    break;

  case 160:
#line 1427 "language.y"
    { yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 161:
#line 1430 "language.y"
    { yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 162:
#line 1433 "language.y"
    { yyval.exprN = new unaryNode( Operator::POSTINC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); }
    break;

  case 163:
#line 1436 "language.y"
    { yyval.exprN = new unaryNode( Operator::POSTDEC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); }
    break;

  case 164:
#line 1440 "language.y"
    { yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 165:
#line 1443 "language.y"
    { yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 166:
#line 1448 "language.y"
    { yyval.exprL = new expr_list(); yyval.exprL->push_back(yyvsp[0].exprN); }
    break;

  case 167:
#line 1451 "language.y"
    { yyval.exprL = yyvsp[-2].exprL; yyval.exprL->push_back(yyvsp[0].exprN); }
    break;

  case 168:
#line 1456 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 169:
#line 1459 "language.y"
    { yyval.exprN = new unaryNode( Operator::PREINC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 170:
#line 1462 "language.y"
    { yyval.exprN = new unaryNode( Operator::PREDEC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 171:
#line 1465 "language.y"
    { yyvsp[-1].unaryN->expr(yyvsp[0].exprN);
              yyval.exprN = yyvsp[-1].unaryN; }
    break;

  case 172:
#line 1469 "language.y"
    { yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 173:
#line 1472 "language.y"
    { yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[-1].typeN.ty, Coord(yyvsp[-3].tok)); }
    break;

  case 174:
#line 1476 "language.y"
    { yyval.unaryN = new unaryNode('&', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 175:
#line 1477 "language.y"
    { yyval.unaryN = new unaryNode('*', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 176:
#line 1478 "language.y"
    { yyval.unaryN = new unaryNode('+', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 177:
#line 1479 "language.y"
    { yyval.unaryN = new unaryNode('-', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 178:
#line 1480 "language.y"
    { yyval.unaryN = new unaryNode('~', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 179:
#line 1481 "language.y"
    { yyval.unaryN = new unaryNode('!', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 181:
#line 1487 "language.y"
    { yyval.exprN = new castNode(yyvsp[-2].typeN.ty, yyvsp[0].exprN, false, Coord(yyvsp[-3].tok)); }
    break;

  case 183:
#line 1493 "language.y"
    { yyval.exprN = new binaryNode('*', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 184:
#line 1496 "language.y"
    { yyval.exprN = new binaryNode('/', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 185:
#line 1499 "language.y"
    { yyval.exprN = new binaryNode('%', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 187:
#line 1505 "language.y"
    { yyval.exprN = new binaryNode('+', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 188:
#line 1508 "language.y"
    { yyval.exprN = new binaryNode('-', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 190:
#line 1514 "language.y"
    { yyval.exprN = new binaryNode( Operator::LS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 191:
#line 1517 "language.y"
    { yyval.exprN = new binaryNode( Operator::RS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 193:
#line 1523 "language.y"
    { yyval.exprN = new binaryNode('<', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 194:
#line 1526 "language.y"
    { yyval.exprN = new binaryNode('>', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 195:
#line 1529 "language.y"
    { yyval.exprN = new binaryNode( Operator::LE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 196:
#line 1532 "language.y"
    { yyval.exprN = new binaryNode( Operator::GE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 198:
#line 1538 "language.y"
    { yyval.exprN = new binaryNode( Operator::EQ, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 199:
#line 1541 "language.y"
    { yyval.exprN = new binaryNode( Operator::NE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 201:
#line 1547 "language.y"
    { yyval.exprN = new binaryNode('&', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 203:
#line 1553 "language.y"
    { CBZ::WarnAboutPrecedence('^', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('^', yyvsp[0].exprN);
	      yyval.exprN = new binaryNode('^', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 205:
#line 1561 "language.y"
    { CBZ::WarnAboutPrecedence('|', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('|', yyvsp[0].exprN);
              yyval.exprN = new binaryNode('|', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 207:
#line 1569 "language.y"
    { yyval.exprN = new binaryNode( Operator::ANDAND, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 209:
#line 1575 "language.y"
    { CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[0].exprN);
              yyval.exprN = new binaryNode( Operator::OROR, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 211:
#line 1583 "language.y"
    { yyval.exprN = new ternaryNode(yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-3].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 213:
#line 1589 "language.y"
    { yyvsp[-1].binaryN->left(yyvsp[-2].exprN);
	      yyvsp[-1].binaryN->right(yyvsp[0].exprN);
	      yyval.exprN = yyvsp[-1].binaryN; }
    break;

  case 214:
#line 1596 "language.y"
    { yyval.binaryN = new binaryNode('=', (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 215:
#line 1599 "language.y"
    { yyval.binaryN = new binaryNode( Operator::MULTassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  }
    break;

  case 216:
#line 1602 "language.y"
    { yyval.binaryN = new binaryNode( Operator::DIVassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 217:
#line 1605 "language.y"
    { yyval.binaryN = new binaryNode( Operator::MODassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 218:
#line 1608 "language.y"
    { yyval.binaryN = new binaryNode( Operator::PLUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  }
    break;

  case 219:
#line 1611 "language.y"
    { yyval.binaryN = new binaryNode( Operator::MINUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 220:
#line 1614 "language.y"
    { yyval.binaryN = new binaryNode( Operator::LSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 221:
#line 1617 "language.y"
    { yyval.binaryN = new binaryNode( Operator::RSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 222:
#line 1620 "language.y"
    { yyval.binaryN = new binaryNode( Operator::ANDassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 223:
#line 1623 "language.y"
    { yyval.binaryN = new binaryNode( Operator::ERassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 224:
#line 1626 "language.y"
    { yyval.binaryN = new binaryNode( Operator::ORassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 225:
#line 1631 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 226:
#line 1633 "language.y"
    {  
              if (yyvsp[-2].exprN->typ() == Comma) 
                { commaNode * ctmp = (commaNode *)yyvsp[-2].exprN;
		  ctmp->exprs().push_back(yyvsp[0].exprN);
		  yyval.exprN = yyvsp[-2].exprN; }
              else
                { expr_list * e = new expr_list();
		  e->push_back(yyvsp[-2].exprN);
		  e->push_back(yyvsp[0].exprN);
		  yyval.exprN = new commaNode(e, yyvsp[-2].exprN->coord()); }
	    }
    break;

  case 227:
#line 1647 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 228:
#line 1651 "language.y"
    { yyval.exprN = (exprNode *) 0; }
    break;

  case 229:
#line 1652 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 230:
#line 1689 "language.y"
    { yyval.declL = yyvsp[-1].declL; }
    break;

  case 231:
#line 1692 "language.y"
    { yyval.declL = yyvsp[-1].declL; }
    break;

  case 232:
#line 1696 "language.y"
    { yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); }
    break;

  case 233:
#line 1702 "language.y"
    { yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); }
    break;

  case 234:
#line 1710 "language.y"
    { yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); }
    break;

  case 235:
#line 1712 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 236:
#line 1714 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN));
	    }
    break;

  case 237:
#line 1719 "language.y"
    { yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); }
    break;

  case 238:
#line 1721 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 239:
#line 1723 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 240:
#line 1727 "language.y"
    { yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); }
    break;

  case 241:
#line 1729 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 242:
#line 1731 "language.y"
    { yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); }
    break;

  case 243:
#line 1737 "language.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 244:
#line 1740 "language.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 245:
#line 1744 "language.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 246:
#line 1747 "language.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 248:
#line 1756 "language.y"
    { yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); }
    break;

  case 249:
#line 1758 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 250:
#line 1760 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 251:
#line 1764 "language.y"
    { yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); }
    break;

  case 252:
#line 1766 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 253:
#line 1768 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 254:
#line 1772 "language.y"
    { yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); }
    break;

  case 255:
#line 1774 "language.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 256:
#line 1776 "language.y"
    { yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); }
    break;

  case 257:
#line 1782 "language.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 258:
#line 1785 "language.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 259:
#line 1789 "language.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 260:
#line 1792 "language.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 262:
#line 1800 "language.y"
    { primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and(); 
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 263:
#line 1805 "language.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 264:
#line 1808 "language.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 265:
#line 1815 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); }
    break;

  case 266:
#line 1819 "language.y"
    { yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; }
    break;

  case 267:
#line 1824 "language.y"
    { yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 268:
#line 1828 "language.y"
    { yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN); 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 269:
#line 1836 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); }
    break;

  case 270:
#line 1840 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 271:
#line 1845 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 272:
#line 1853 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 273:
#line 1857 "language.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; }
    break;

  case 274:
#line 1862 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 276:
#line 1871 "language.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 277:
#line 1874 "language.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 280:
#line 1886 "language.y"
    { primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc;}
    break;

  case 281:
#line 1891 "language.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 283:
#line 1896 "language.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 284:
#line 1902 "language.y"
    { yyval.typeN.ty = yyvsp[0].primN;  
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 285:
#line 1906 "language.y"
    { yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 286:
#line 1911 "language.y"
    { yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 287:
#line 1915 "language.y"
    { yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN);
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 288:
#line 1922 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty; 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 289:
#line 1926 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord));
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 290:
#line 1931 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 291:
#line 1939 "language.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, typeNode::NONE, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 292:
#line 1943 "language.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 293:
#line 1948 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 295:
#line 1956 "language.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 297:
#line 1962 "language.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 298:
#line 1968 "language.y"
    { yyval.typeN.ty = yyvsp[0].sueN; 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 299:
#line 1972 "language.y"
    { yyval.typeN.ty = yyvsp[0].enumN;  
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 303:
#line 1982 "language.y"
    {
	      CBZ::Warning(2, string("function prototype parameters must have types"));
              /* SZG Don't need this? $$ = AddDefaultParameterTypes(); */
            }
    break;

  case 305:
#line 1994 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); }
    break;

  case 306:
#line 1997 "language.y"
    { yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-3].tok)), declNode::NONE, Redecl); }
    break;

  case 307:
#line 2000 "language.y"
    { yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(yyvsp[-3].tq.tq,(typeNode *)0, Coord(yyvsp[-4].tok)), yyvsp[-3].tq.sc, Redecl); }
    break;

  case 308:
#line 2003 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq,(typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); }
    break;

  case 309:
#line 2010 "language.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 310:
#line 2013 "language.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[-1].typeN.ty); }
    break;

  case 311:
#line 2016 "language.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 312:
#line 2022 "language.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 313:
#line 2026 "language.y"
    { yyval.declN = yyvsp[-1].declN;  }
    break;

  case 314:
#line 2032 "language.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 315:
#line 2036 "language.y"
    { yyval.declN = new declNode(yyvsp[-1].idN, declNode::NONE, yyvsp[0].typeN.ty, (exprNode *)0, (exprNode *)0); }
    break;

  case 318:
#line 2050 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); }
    break;

  case 319:
#line 2053 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); }
    break;

  case 320:
#line 2059 "language.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 321:
#line 2062 "language.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 325:
#line 2075 "language.y"
    { yyval.typeN.ty = new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[0].tok)); 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 326:
#line 2079 "language.y"
    { yyval.typeN.ty = new ptrNode(yyvsp[0].tq.tq, (typeNode *)0, Coord(yyvsp[-1].tok));  
	      yyval.typeN.sc = yyvsp[0].tq.sc; }
    break;

  case 327:
#line 2083 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)));   
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 328:
#line 2087 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)));   
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 329:
#line 2094 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 330:
#line 2098 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 331:
#line 2102 "language.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 332:
#line 2106 "language.y"
    { yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(yyvsp[0].typeN.ty); 
	      yyval.typeN.sc = merge_sc(yyvsp[-2].typeN.sc, yyvsp[0].typeN.sc, yyvsp[-2].typeN.ty->coord()); }
    break;

  case 333:
#line 2113 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty;
	       yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 334:
#line 2117 "language.y"
    { yyval.typeN.ty = new funcNode(typeNode::NONE, (decl_list *)0, (typeNode *)0, Coord(yyvsp[-1].tok)); 
	       yyval.typeN.sc = declNode::NONE; }
    break;

  case 335:
#line 2121 "language.y"
    { yyval.typeN.ty = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok)); 
	       yyval.typeN.sc = declNode::NONE; }
    break;

  case 336:
#line 2128 "language.y"
    { yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, (exprNode *)0, Coord(yyvsp[-1].tok));
              yyval.typeN.sc = declNode::NONE; }
    break;

  case 337:
#line 2132 "language.y"
    { yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok)); 
              yyval.typeN.sc = declNode::NONE; }
    break;

  case 338:
#line 2136 "language.y"
    { yyval.typeN.ty = yyvsp[-3].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok))); 
              yyval.typeN.sc = yyvsp[-3].typeN.sc; }
    break;

  case 339:
#line 2142 "language.y"
    { 
              CBZ::SyntaxError(string("array declaration with illegal empty dimension"));
              yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0,
							     (exprNode *) new constNode(constant((int)1)),
							     Coord(yyvsp[-1].tok)));
	      yyval.typeN.sc = declNode::NONE;
            }
    break;

  case 343:
#line 2162 "language.y"
    { yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok))); }
    break;

  case 344:
#line 2165 "language.y"
    { yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok))); }
    break;

  case 345:
#line 2171 "language.y"
    { yyval.declN = yyvsp[-1].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 346:
#line 2174 "language.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 347:
#line 2177 "language.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 348:
#line 2183 "language.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 349:
#line 2186 "language.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 350:
#line 2192 "language.y"
    { 
/*              OldStyleFunctionDefinition = TRUE; */
              yyval.declN = yyvsp[0].declN; 
            }
    break;

  case 351:
#line 2198 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, SU); }
    break;

  case 352:
#line 2201 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), declNode::NONE, SU); }
    break;

  case 353:
#line 2207 "language.y"
    { funcNode *f = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok));
	      f->is_knr (true);
	      yyval.declN = yyvsp[-3].declN->modify_type_and(f); }
    break;

  case 354:
#line 2212 "language.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 355:
#line 2215 "language.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 356:
#line 2230 "language.y"
    { yyval.declL = new decl_list();
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    }
    break;

  case 357:
#line 2239 "language.y"
    { yyval.declL = yyvsp[-2].declL;
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    }
    break;

  case 360:
#line 2257 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 361:
#line 2261 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty)->finish_and(); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord()); }
    break;

  case 362:
#line 2265 "language.y"
    { yyval.typeN.ty = new primNode(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = yyvsp[0].tq.sc; }
    break;

  case 363:
#line 2269 "language.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 364:
#line 2278 "language.y"
    { yyval.attribL = (attrib_list *)0; }
    break;

  case 365:
#line 2280 "language.y"
    { yyval.attribL = yyvsp[0].attribL; }
    break;

  case 366:
#line 2285 "language.y"
    { yyval.attribL = yyvsp[0].attribL; }
    break;

  case 367:
#line 2287 "language.y"
    { yyval.attribL = yyvsp[-1].attribL; 
		  yyvsp[-1].attribL->splice(yyvsp[-1].attribL->end(), * yyvsp[0].attribL); 
		  delete yyvsp[0].attribL; }
    break;

  case 368:
#line 2294 "language.y"
    { if (CBZ::ANSIOnly)
	            CBZ::SyntaxError(string("__attribute__ not allowed with -ansi switch"));
                  yyval.attribL = yyvsp[-2].attribL; }
    break;

  case 369:
#line 2301 "language.y"
    { yyval.attribL = new attrib_list(); yyval.attribL->push_back(yyvsp[0].attribN); }
    break;

  case 370:
#line 2303 "language.y"
    { yyval.attribL = yyvsp[-2].attribL; yyval.attribL->push_back(yyvsp[0].attribN); }
    break;

  case 371:
#line 2308 "language.y"
    { yyval.attribN = 0; }
    break;

  case 372:
#line 2310 "language.y"
    { yyval.attribN = new attribNode(yyvsp[0].idN, (exprNode *)0); }
    break;

  case 373:
#line 2312 "language.y"
    { yyval.attribN = new attribNode(yyvsp[-3].idN, yyvsp[-1].exprN); }
    break;

  case 376:
#line 2319 "language.y"
    { yyval.idN = new idNode("const", Coord(yyvsp[0].tok)); }
    break;

  case 377:
#line 2324 "language.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 378:
#line 2325 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 379:
#line 2330 "language.y"
    { yyval.exprN = yyvsp[-1].initializerN; yyval.exprN->coord(yyvsp[-2].tok); }
    break;

  case 380:
#line 2331 "language.y"
    { yyval.exprN = yyvsp[-2].initializerN; yyval.exprN->coord(yyvsp[-3].tok); }
    break;

  case 381:
#line 2332 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 382:
#line 2338 "language.y"
    { yyval.initializerN = new initializerNode(new expr_list(), yyvsp[0].exprN->coord());
	      yyval.initializerN->add_expr(yyvsp[0].exprN); }
    break;

  case 383:
#line 2342 "language.y"
    { yyval.initializerN = yyvsp[-2].initializerN;
	      yyval.initializerN->add_expr(yyvsp[0].exprN);
            }
    break;

  case 385:
#line 2352 "language.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(new declNode((typeNode *) new primNode(basic_type::Ellipsis), declNode::NONE)); }
    break;

  case 386:
#line 2357 "language.y"
    { /* primNode * n = new primNode(qualifier(), basic_type::Void); */
	      CBZ::SyntaxError(Coord(yyvsp[0].tok), string("First argument cannot be `...'"));
              yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(new primNode(typeNode::NONE, basic_type::Void), declNode::NONE));
            }
    break;

  case 387:
#line 2367 "language.y"
    { yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[0].declN); }
    break;

  case 388:
#line 2371 "language.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN); }
    break;

  case 389:
#line 2376 "language.y"
    { 
	      CBZ::SyntaxError(yyvsp[-2].declN->coord(), string("formals cannot have initializers"));
	      yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[-2].declN);
            }
    break;

  case 390:
#line 2383 "language.y"
    { yyval.declL = yyvsp[-2].declL; }
    break;

  case 391:
#line 2389 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); }
    break;

  case 392:
#line 2392 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); }
    break;

  case 393:
#line 2396 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 394:
#line 2399 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 395:
#line 2402 "language.y"
    { yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); }
    break;

  case 396:
#line 2405 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); }
    break;

  case 397:
#line 2409 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); }
    break;

  case 398:
#line 2412 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); }
    break;

  case 399:
#line 2415 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); }
    break;

  case 400:
#line 2419 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 401:
#line 2422 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 402:
#line 2425 "language.y"
    { yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); }
    break;

  case 403:
#line 2428 "language.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); }
    break;

  case 404:
#line 2432 "language.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); }
    break;

  case 405:
#line 2445 "language.y"
    { yyval.sueN = yyvsp[-3].sueN->set_name_fields_and((idNode *)0, yyvsp[-1].declL, yyvsp[-2].tok, yyvsp[0].tok); }
    break;

  case 406:
#line 2449 "language.y"
    { yyval.sueN = yyvsp[-4].sueN->set_name_fields_and(yyvsp[-3].idN, yyvsp[-1].declL, yyvsp[-2].tok, yyvsp[0].tok); }
    break;

  case 407:
#line 2452 "language.y"
    { yyval.sueN = yyvsp[-1].sueN->set_name_and(yyvsp[0].idN, yyvsp[-1].sueN->coord()); }
    break;

  case 408:
#line 2456 "language.y"
    { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-2].sueN->set_name_fields_and((idNode *)0, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].tok);
	    }
    break;

  case 409:
#line 2463 "language.y"
    { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-3].sueN->set_name_fields_and(yyvsp[-2].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].tok);
	    }
    break;

  case 410:
#line 2472 "language.y"
    { yyval.sueN = new structNode(Coord(yyvsp[0].tok)); }
    break;

  case 411:
#line 2473 "language.y"
    { yyval.sueN = new unionNode(Coord(yyvsp[0].tok)); }
    break;

  case 413:
#line 2480 "language.y"
    { yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyval.declL->end(), * yyvsp[0].declL);
	      delete yyvsp[0].declL; }
    break;

  case 416:
#line 2495 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, SU)); }
    break;

  case 417:
#line 2499 "language.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); }
    break;

  case 418:
#line 2506 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, SU)); }
    break;

  case 419:
#line 2510 "language.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); }
    break;

  case 420:
#line 2518 "language.y"
    { yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 421:
#line 2523 "language.y"
    { yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 422:
#line 2531 "language.y"
    { yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 423:
#line 2536 "language.y"
    { yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 424:
#line 2543 "language.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 426:
#line 2549 "language.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 427:
#line 2555 "language.y"
    { yyval.enumN = new enumNode((idNode *)0, yyvsp[-2].declL, yyvsp[-4].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); }
    break;

  case 428:
#line 2558 "language.y"
    { yyval.enumN = new enumNode(yyvsp[-4].idN, yyvsp[-2].declL, yyvsp[-5].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok));   }
    break;

  case 429:
#line 2561 "language.y"
    { yyval.enumN = new enumNode(yyvsp[0].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].idN->coord(), yyvsp[0].idN->coord()); }
    break;

  case 430:
#line 2567 "language.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); }
    break;

  case 431:
#line 2571 "language.y"
    { yyval.declL = yyvsp[-3].declL; 
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); }
    break;

  case 432:
#line 2577 "language.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 433:
#line 2578 "language.y"
    { yyval.exprN = yyvsp[0].exprN;   }
    break;

  case 434:
#line 2582 "language.y"
    { }
    break;

  case 435:
#line 2583 "language.y"
    { }
    break;

  case 436:
#line 2594 "language.y"
    { yyval.stmtN = yyvsp[0].targetN; }
    break;

  case 437:
#line 2597 "language.y"
    { yyval.stmtN = yyvsp[0].blockN; }
    break;

  case 438:
#line 2600 "language.y"
    { yyval.stmtN = yyvsp[0].exprstmtN; }
    break;

  case 439:
#line 2603 "language.y"
    { yyval.stmtN = yyvsp[0].stmtN; }
    break;

  case 440:
#line 2606 "language.y"
    { yyval.stmtN = yyvsp[0].loopN; }
    break;

  case 441:
#line 2609 "language.y"
    { yyval.stmtN = yyvsp[0].jumpN; }
    break;

  case 442:
#line 2620 "language.y"
    {  yyval.stmtN = (stmtNode *) 0; }
    break;

  case 443:
#line 2622 "language.y"
    { yyval.stmtN = yyvsp[-1].stmtN; }
    break;

  case 444:
#line 2628 "language.y"
    { yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); }
    break;

  case 445:
#line 2631 "language.y"
    { yyval.targetN = new caseNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-3].tok)); }
    break;

  case 446:
#line 2634 "language.y"
    { yyval.targetN = new caseNode(0, yyvsp[0].stmtN, Coord(yyvsp[-2].tok)); }
    break;

  case 447:
#line 2638 "language.y"
    { yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); }
    break;

  case 448:
#line 2644 "language.y"
    { yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); }
    break;

  case 449:
#line 2647 "language.y"
    { yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 450:
#line 2650 "language.y"
    { yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 451:
#line 2653 "language.y"
    { yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); }
    break;

  case 452:
#line 2657 "language.y"
    { yyval.tok = yyvsp[0].tok; CBZ::current_unit->enter_scope(); }
    break;

  case 453:
#line 2661 "language.y"
    { yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_scope(); }
    break;

  case 454:
#line 2685 "language.y"
    { yyval.declL = yyvsp[0].declL; }
    break;

  case 455:
#line 2688 "language.y"
    { yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyvsp[-1].declL->end(), * yyvsp[0].declL); 
	      delete yyvsp[0].declL;
	    }
    break;

  case 456:
#line 2697 "language.y"
    { yyval.stmtL = new stmt_list();
              yyval.stmtL->push_back(yyvsp[0].stmtN); }
    break;

  case 457:
#line 2701 "language.y"
    { yyval.stmtL = yyvsp[-1].stmtL;
              yyval.stmtL->push_back(yyvsp[0].stmtN); }
    break;

  case 458:
#line 2711 "language.y"
    { yyval.exprstmtN = new exprstmtNode(yyvsp[-1].exprN); }
    break;

  case 459:
#line 2716 "language.y"
    { yyval.stmtN = new ifNode(yyvsp[-2].exprN, yyvsp[0].stmtN, 0, Coord(yyvsp[-4].tok)); }
    break;

  case 460:
#line 2719 "language.y"
    { yyval.stmtN = new ifNode (yyvsp[-4].exprN, yyvsp[-2].stmtN, yyvsp[0].stmtN, Coord(yyvsp[-6].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 461:
#line 2722 "language.y"
    { yyval.stmtN = new switchNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 462:
#line 2727 "language.y"
    { yyval.loopN = new whileNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 463:
#line 2730 "language.y"
    { yyval.loopN = new doNode(yyvsp[-5].stmtN, yyvsp[-2].exprN, Coord(yyvsp[-6].tok), Coord(yyvsp[-4].tok)); }
    break;

  case 464:
#line 2733 "language.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 465:
#line 2738 "language.y"
    { yyval.loopN = new forNode((exprNode *)0, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 466:
#line 2741 "language.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 467:
#line 2744 "language.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, (exprNode *)0, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 468:
#line 2747 "language.y"
    { yyval.loopN = new forNode((exprNode *)0, (exprNode *)0, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 469:
#line 2752 "language.y"
    { yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); }
    break;

  case 470:
#line 2755 "language.y"
    { yyval.jumpN = new continueNode(Coord(yyvsp[-1].tok)); }
    break;

  case 471:
#line 2758 "language.y"
    { yyval.jumpN = new breakNode(Coord(yyvsp[-1].tok)); }
    break;

  case 472:
#line 2761 "language.y"
    { yyval.jumpN = new returnNode(yyvsp[-1].exprN, procNode::current(), Coord(yyvsp[-2].tok)); }
    break;

  case 473:
#line 2765 "language.y"
    { yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); }
    break;

  case 474:
#line 2923 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 475:
#line 2924 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 476:
#line 2925 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 477:
#line 2926 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 478:
#line 2927 "language.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 479:
#line 2932 "language.y"
    { yyval.constN = yyvsp[0].constN; }
    break;

  case 480:
#line 2934 "language.y"
    {
	      string s3 = string(yyvsp[-1].constN->value().Str()) + string(yyvsp[0].constN->value().Str());
	      yyval.constN = new constNode(constant(s3.c_str()), s3.c_str(), yyvsp[-1].constN->coord());
	      delete yyvsp[-1].constN;
	      delete yyvsp[0].constN;
	    }
    break;

  case 481:
#line 2943 "language.y"
    { yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 482:
#line 2944 "language.y"
    { yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 483:
#line 2945 "language.y"
    { yyval.tq.tq = typeNode::INLINE;   yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 484:
#line 2949 "language.y"
    { yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 485:
#line 2950 "language.y"
    { yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 486:
#line 2954 "language.y"
    { yyval.tq.sc = declNode::TYPEDEF;  yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 487:
#line 2955 "language.y"
    { yyval.tq.sc = declNode::EXTERN;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 488:
#line 2956 "language.y"
    { yyval.tq.sc = declNode::STATIC;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 489:
#line 2957 "language.y"
    { yyval.tq.sc = declNode::AUTO;     yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 490:
#line 2958 "language.y"
    { yyval.tq.sc = declNode::REGISTER; yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 491:
#line 2962 "language.y"
    { yyval.primN = new primNode(basic_type::Void, Coord(yyvsp[0].tok));    }
    break;

  case 492:
#line 2963 "language.y"
    { yyval.primN = new primNode(basic_type::Char, Coord(yyvsp[0].tok));    }
    break;

  case 493:
#line 2964 "language.y"
    { yyval.primN = new primNode(basic_type::Int, Coord(yyvsp[0].tok));     }
    break;

  case 494:
#line 2965 "language.y"
    { yyval.primN = new primNode(basic_type::Float, Coord(yyvsp[0].tok));   }
    break;

  case 495:
#line 2966 "language.y"
    { yyval.primN = new primNode(basic_type::Double, Coord(yyvsp[0].tok));  }
    break;

  case 496:
#line 2968 "language.y"
    { yyval.primN = new primNode(basic_type::Signed, Coord(yyvsp[0].tok));  }
    break;

  case 497:
#line 2969 "language.y"
    { yyval.primN = new primNode(basic_type::Unsigned, Coord(yyvsp[0].tok));}
    break;

  case 498:
#line 2971 "language.y"
    { yyval.primN = new primNode(basic_type::Short, Coord(yyvsp[0].tok));   }
    break;

  case 499:
#line 2972 "language.y"
    { yyval.primN = new primNode(basic_type::Long, Coord(yyvsp[0].tok));    }
    break;

  case 502:
#line 2993 "language.y"
    {
	yyval.exprN = new exprPatternNode(yyvsp[0].t_id);
	delete yyvsp[0].t_id;
      }
    break;

  case 503:
#line 2999 "language.y"
    {
	yyval.exprN = new exprPatternNode(yyvsp[-1].t_id);
	delete yyvsp[-1].t_id;
      }
    break;

  case 504:
#line 3008 "language.y"
    {
	yyval.stmtN = new stmtPatternNode(yyvsp[-1].t_id);
	delete yyvsp[-1].t_id;
      }
    break;

  case 505:
#line 3017 "language.y"
    {
	yyval.typeN.ty = new typePatternNode(yyvsp[-1].t_id);
	yyval.typeN.sc = declNode::NONE;
	delete yyvsp[-1].t_id;
      }
    break;


    }

/* Line 1016 of /lusr/gnu/share/bison/yacc.c.  */
#line 5490 "language.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--yyssp;


#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 3024 "language.y"


