/* A Bison parser, made from parser.y, by GNU bison 1.75.  */

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
#define yyparse cbparse
#define yylex   cblex
#define yyerror cberror
#define yylval  cblval
#define yychar  cbchar
#define yydebug cbdebug
#define yynerrs cbnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ctokAND = 258,
     ctokSTAR = 259,
     ctokPLUS = 260,
     ctokMINUS = 261,
     ctokTILDE = 262,
     ctokNOT = 263,
     ctokLESS = 264,
     ctokGREATER = 265,
     ctokOR = 266,
     ctokHAT = 267,
     ctokPERCENT = 268,
     ctokDIVIDE = 269,
     ctokLEFTPAREN = 270,
     ctokRIGHTPAREN = 271,
     ctokDOT = 272,
     ctokQUESTION = 273,
     ctokSEMICOLON = 274,
     ctokLEFTBRACE = 275,
     ctokRIGHTBRACE = 276,
     ctokCOMMA = 277,
     ctokLEFTBRACKET = 278,
     ctokRIGHTBRACKET = 279,
     ctokCOLON = 280,
     ctokAUTO = 281,
     ctokDOUBLE = 282,
     ctokINT = 283,
     ctokSTRUCT = 284,
     ctokBREAK = 285,
     ctokELSE = 286,
     ctokLONG = 287,
     ctokSWITCH = 288,
     ctokCASE = 289,
     ctokENUM = 290,
     ctokREGISTER = 291,
     ctokTYPEDEF = 292,
     ctokCHAR = 293,
     ctokEXTERN = 294,
     ctokRETURN = 295,
     ctokUNION = 296,
     ctokCONST = 297,
     ctokFLOAT = 298,
     ctokSHORT = 299,
     ctokUNSIGNED = 300,
     ctokCONTINUE = 301,
     ctokFOR = 302,
     ctokSIGNED = 303,
     ctokVOID = 304,
     ctokDEFAULT = 305,
     ctokGOTO = 306,
     ctokSIZEOF = 307,
     ctokVOLATILE = 308,
     ctokDO = 309,
     ctokIF = 310,
     ctokSTATIC = 311,
     ctokWHILE = 312,
     ctokUPLUS = 313,
     ctokUMINUS = 314,
     ctokINDIR = 315,
     ctokADDRESS = 316,
     ctokPOSTINC = 317,
     ctokPOSTDEC = 318,
     ctokPREINC = 319,
     ctokPREDEC = 320,
     ctokBOGUS = 321,
     IDENTIFIER = 322,
     STRINGliteral = 323,
     FLOATINGconstant = 324,
     INTEGERconstant = 325,
     OCTALconstant = 326,
     HEXconstant = 327,
     WIDECHARconstant = 328,
     CHARACTERconstant = 329,
     TYPEDEFname = 330,
     ctokARROW = 331,
     ctokICR = 332,
     ctokDECR = 333,
     ctokLS = 334,
     ctokRS = 335,
     ctokLE = 336,
     ctokGE = 337,
     ctokEQ = 338,
     ctokNE = 339,
     ctokANDAND = 340,
     ctokOROR = 341,
     ctokELLIPSIS = 342,
     ctokEQUALS = 343,
     ctokMULTassign = 344,
     ctokDIVassign = 345,
     ctokMODassign = 346,
     ctokPLUSassign = 347,
     ctokMINUSassign = 348,
     ctokLSassign = 349,
     ctokRSassign = 350,
     ctokANDassign = 351,
     ctokERassign = 352,
     ctokORassign = 353,
     ctokINLINE = 354,
     ctokATTRIBUTE = 355,
     ctokMETA_TYPE_EXPR = 356,
     ctokMETA_TYPE_STMT = 357,
     ctokMETA_EXPR = 358,
     ctokMETA_STMT = 359
   };
#endif
#define ctokAND 258
#define ctokSTAR 259
#define ctokPLUS 260
#define ctokMINUS 261
#define ctokTILDE 262
#define ctokNOT 263
#define ctokLESS 264
#define ctokGREATER 265
#define ctokOR 266
#define ctokHAT 267
#define ctokPERCENT 268
#define ctokDIVIDE 269
#define ctokLEFTPAREN 270
#define ctokRIGHTPAREN 271
#define ctokDOT 272
#define ctokQUESTION 273
#define ctokSEMICOLON 274
#define ctokLEFTBRACE 275
#define ctokRIGHTBRACE 276
#define ctokCOMMA 277
#define ctokLEFTBRACKET 278
#define ctokRIGHTBRACKET 279
#define ctokCOLON 280
#define ctokAUTO 281
#define ctokDOUBLE 282
#define ctokINT 283
#define ctokSTRUCT 284
#define ctokBREAK 285
#define ctokELSE 286
#define ctokLONG 287
#define ctokSWITCH 288
#define ctokCASE 289
#define ctokENUM 290
#define ctokREGISTER 291
#define ctokTYPEDEF 292
#define ctokCHAR 293
#define ctokEXTERN 294
#define ctokRETURN 295
#define ctokUNION 296
#define ctokCONST 297
#define ctokFLOAT 298
#define ctokSHORT 299
#define ctokUNSIGNED 300
#define ctokCONTINUE 301
#define ctokFOR 302
#define ctokSIGNED 303
#define ctokVOID 304
#define ctokDEFAULT 305
#define ctokGOTO 306
#define ctokSIZEOF 307
#define ctokVOLATILE 308
#define ctokDO 309
#define ctokIF 310
#define ctokSTATIC 311
#define ctokWHILE 312
#define ctokUPLUS 313
#define ctokUMINUS 314
#define ctokINDIR 315
#define ctokADDRESS 316
#define ctokPOSTINC 317
#define ctokPOSTDEC 318
#define ctokPREINC 319
#define ctokPREDEC 320
#define ctokBOGUS 321
#define IDENTIFIER 322
#define STRINGliteral 323
#define FLOATINGconstant 324
#define INTEGERconstant 325
#define OCTALconstant 326
#define HEXconstant 327
#define WIDECHARconstant 328
#define CHARACTERconstant 329
#define TYPEDEFname 330
#define ctokARROW 331
#define ctokICR 332
#define ctokDECR 333
#define ctokLS 334
#define ctokRS 335
#define ctokLE 336
#define ctokGE 337
#define ctokEQ 338
#define ctokNE 339
#define ctokANDAND 340
#define ctokOROR 341
#define ctokELLIPSIS 342
#define ctokEQUALS 343
#define ctokMULTassign 344
#define ctokDIVassign 345
#define ctokMODassign 346
#define ctokPLUSassign 347
#define ctokMINUSassign 348
#define ctokLSassign 349
#define ctokRSassign 350
#define ctokANDassign 351
#define ctokERassign 352
#define ctokORassign 353
#define ctokINLINE 354
#define ctokATTRIBUTE 355
#define ctokMETA_TYPE_EXPR 356
#define ctokMETA_TYPE_STMT 357
#define ctokMETA_EXPR 358
#define ctokMETA_STMT 359




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

    /* Copyright (C) 1989,1990 James A. Roskind, All rights reserved.
    This grammar was developed  and  written  by  James  A.  Roskind. 
    Copying  of  this  grammar  description, as a whole, is permitted 
    providing this notice is intact and applicable  in  all  complete 
    copies.   Translations as a whole to other parser generator input 
    languages  (or  grammar  description  languages)   is   permitted 
    provided  that  this  notice is intact and applicable in all such 
    copies,  along  with  a  disclaimer  that  the  contents  are   a 
    translation.   The reproduction of derived text, such as modified 
    versions of this grammar, or the output of parser generators,  is 
    permitted,  provided  the  resulting  work includes the copyright 
    notice "Portions Copyright (c)  1989,  1990  James  A.  Roskind". 
    Derived products, such as compilers, translators, browsers, etc., 
    that  use  this  grammar,  must also provide the notice "Portions 
    Copyright  (c)  1989,  1990  James  A.  Roskind"  in   a   manner 
    appropriate  to  the  utility,  and in keeping with copyright law 
    (e.g.: EITHER displayed when first invoked/executed; OR displayed 
    continuously on display terminal; OR via placement in the  object 
    code  in  form  readable in a printout, with or near the title of 
    the work, or at the end of the file).  No royalties, licenses  or 
    commissions  of  any  kind are required to copy this grammar, its 
    translations, or derivative products, when the copies are made in 
    compliance with this notice. Persons or corporations that do make 
    copies in compliance with this notice may charge  whatever  price 
    is  agreeable  to  a  buyer, for such copies or derivative works. 
    THIS GRAMMAR IS PROVIDED ``AS IS'' AND  WITHOUT  ANY  EXPRESS  OR 
    IMPLIED  WARRANTIES,  INCLUDING,  WITHOUT LIMITATION, THE IMPLIED 
    WARRANTIES  OF  MERCHANTABILITY  AND  FITNESS  FOR  A  PARTICULAR 
    PURPOSE.

    James A. Roskind
    Independent Consultant
    516 Latania Palm Drive
    Indialantic FL, 32903
    (407)729-4348
    jar@ileaf.com
    or ...!uunet!leafusa!jar


    ---end of copyright notice---


This file is a companion file to a C++ grammar description file.

*/


#include "c_breeze.h"
#include "meta.h"

// -- Utility functions -- defined at the end of this file...

declNode::Storage_class merge_sc(declNode::Storage_class sc1,
				 declNode::Storage_class sc2,
				 const Coord c = Coord::Unknown);

struct _TQ merge_tq(struct _TQ ts1,
		    struct _TQ ts2);

#ifndef YYDEBUG
int yydebug=0;
#endif

extern int yylex(void);

void yyerror(const char *msg)
{
  CBZ::SyntaxError(string(msg));
}

/* End of create code (EAB) */



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
#line 119 "parser.y"
typedef union {
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
#line 401 "parser.tab.cc"
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
#line 422 "parser.tab.cc"

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
#define YYFINAL  70
#define YYLAST   3140

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  105
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  144
/* YYNRULES -- Number of rules. */
#define YYNRULES  395
/* YYNRULES -- Number of states. */
#define YYNSTATES  644

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   359

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    21,    28,
      30,    35,    39,    44,    48,    52,    55,    58,    62,    66,
      68,    72,    74,    77,    80,    83,    86,    91,    93,    95,
      97,    99,   101,   103,   105,   110,   112,   116,   120,   124,
     126,   130,   134,   136,   140,   144,   146,   150,   154,   158,
     162,   164,   168,   172,   174,   178,   180,   184,   186,   190,
     192,   196,   198,   202,   204,   210,   212,   216,   218,   220,
     222,   224,   226,   228,   230,   232,   234,   236,   238,   240,
     244,   246,   248,   249,   251,   254,   257,   260,   263,   264,
     265,   272,   273,   274,   281,   282,   283,   291,   292,   298,
     299,   305,   309,   310,   311,   318,   319,   320,   327,   328,
     329,   337,   338,   344,   345,   351,   355,   357,   359,   361,
     364,   367,   370,   373,   376,   379,   382,   385,   388,   391,
     393,   396,   399,   401,   403,   405,   407,   409,   411,   414,
     417,   420,   422,   425,   428,   430,   433,   436,   438,   441,
     443,   446,   448,   450,   452,   454,   456,   458,   460,   463,
     468,   474,   478,   482,   487,   492,   494,   498,   500,   503,
     505,   507,   510,   514,   518,   523,   525,   527,   529,   531,
     534,   537,   541,   545,   549,   553,   558,   560,   563,   567,
     570,   574,   579,   583,   585,   587,   589,   592,   596,   599,
     603,   608,   610,   614,   616,   619,   623,   628,   632,   637,
     639,   643,   645,   647,   649,   652,   654,   657,   658,   660,
     662,   665,   672,   674,   678,   679,   681,   686,   688,   690,
     692,   693,   696,   700,   705,   707,   709,   713,   715,   719,
     721,   723,   727,   731,   735,   737,   740,   743,   746,   748,
     751,   754,   756,   759,   762,   765,   767,   770,   773,   778,
     784,   787,   791,   796,   798,   800,   802,   805,   808,   811,
     814,   818,   821,   825,   829,   832,   836,   839,   840,   842,
     845,   851,   858,   861,   864,   869,   870,   873,   874,   876,
     878,   880,   882,   884,   886,   888,   891,   893,   897,   902,
     906,   910,   913,   917,   921,   926,   928,   930,   933,   937,
     941,   946,   948,   951,   953,   956,   959,   965,   973,   979,
     985,   993,  1003,  1013,  1023,  1033,  1039,  1043,  1046,  1049,
    1053,  1057,  1059,  1062,  1064,  1066,  1067,  1071,  1074,  1075,
    1080,  1081,  1086,  1087,  1088,  1089,  1099,  1100,  1104,  1105,
    1110,  1111,  1116,  1117,  1122,  1123,  1128,  1129,  1134,  1135,
    1141,  1142,  1148,  1149,  1155,  1156,  1162,  1163,  1166,  1168,
    1170,  1172,  1174,  1176,  1178,  1181,  1183,  1185,  1187,  1189,
    1191,  1193,  1195,  1197,  1199,  1201,  1203,  1205,  1207,  1209,
    1211,  1213,  1215,  1217,  1219,  1221
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     106,     0,    -1,   220,    -1,   247,    -1,   241,    -1,   242,
      -1,    15,   126,    16,    -1,    15,   211,   215,   212,    16,
      -1,    15,   211,   214,   215,   212,    16,    -1,   107,    -1,
     108,    23,   126,    24,    -1,   108,    15,    16,    -1,   108,
      15,   109,    16,    -1,   108,    17,   247,    -1,   108,    76,
     247,    -1,   108,    77,    -1,   108,    78,    -1,   108,    17,
     248,    -1,   108,    76,   248,    -1,   124,    -1,   109,    22,
     124,    -1,   108,    -1,    77,   110,    -1,    78,   110,    -1,
     111,   112,    -1,    52,   110,    -1,    52,    15,   181,    16,
      -1,     3,    -1,     4,    -1,     5,    -1,     6,    -1,     7,
      -1,     8,    -1,   110,    -1,    15,   181,    16,   112,    -1,
     112,    -1,   113,     4,   112,    -1,   113,    14,   112,    -1,
     113,    13,   112,    -1,   113,    -1,   114,     5,   113,    -1,
     114,     6,   113,    -1,   114,    -1,   115,    79,   114,    -1,
     115,    80,   114,    -1,   115,    -1,   116,     9,   115,    -1,
     116,    10,   115,    -1,   116,    81,   115,    -1,   116,    82,
     115,    -1,   116,    -1,   117,    83,   116,    -1,   117,    84,
     116,    -1,   117,    -1,   118,     3,   117,    -1,   118,    -1,
     119,    12,   118,    -1,   119,    -1,   120,    11,   119,    -1,
     120,    -1,   121,    85,   120,    -1,   121,    -1,   122,    86,
     121,    -1,   122,    -1,   122,    18,   126,    25,   123,    -1,
     123,    -1,   110,   125,   124,    -1,    88,    -1,    89,    -1,
      90,    -1,    91,    -1,    92,    -1,    93,    -1,    94,    -1,
      95,    -1,    96,    -1,    97,    -1,    98,    -1,   124,    -1,
     126,    22,   124,    -1,   103,    -1,   123,    -1,    -1,   126,
      -1,   130,    19,    -1,   139,    19,    -1,   150,    19,    -1,
     156,    19,    -1,    -1,    -1,   148,   161,   131,   182,   132,
     188,    -1,    -1,    -1,   154,   161,   133,   182,   134,   188,
      -1,    -1,    -1,   130,    22,   161,   135,   182,   136,   188,
      -1,    -1,   148,     1,   137,   182,   188,    -1,    -1,   154,
       1,   138,   182,   188,    -1,   130,    22,     1,    -1,    -1,
      -1,   152,   173,   140,   182,   141,   188,    -1,    -1,    -1,
     158,   173,   142,   182,   143,   188,    -1,    -1,    -1,   139,
      22,   173,   144,   182,   145,   188,    -1,    -1,   152,     1,
     146,   182,   188,    -1,    -1,   158,     1,   147,   182,   188,
      -1,   139,    22,     1,    -1,   149,    -1,   150,    -1,   151,
      -1,   155,   245,    -1,   152,   246,    -1,   149,   153,    -1,
     149,   246,    -1,   156,   245,    -1,   152,   160,    -1,   150,
     153,    -1,   157,   245,    -1,   152,   248,    -1,   151,   153,
      -1,   245,    -1,   158,   245,    -1,   152,   153,    -1,   243,
      -1,   245,    -1,   155,    -1,   156,    -1,   157,    -1,   246,
      -1,   158,   246,    -1,   155,   243,    -1,   155,   246,    -1,
     160,    -1,   158,   160,    -1,   156,   243,    -1,   248,    -1,
     158,   248,    -1,   157,   243,    -1,   243,    -1,   158,   243,
      -1,   244,    -1,   159,   244,    -1,   194,    -1,   204,    -1,
     162,    -1,   165,    -1,   173,    -1,   177,    -1,   163,    -1,
       4,   162,    -1,     4,    15,   164,    16,    -1,     4,   159,
      15,   164,    16,    -1,     4,   159,   162,    -1,    15,   162,
      16,    -1,    15,   164,   171,    16,    -1,    15,   162,    16,
     171,    -1,   248,    -1,    15,   164,    16,    -1,   248,    -1,
     248,   171,    -1,   166,    -1,   167,    -1,     4,   165,    -1,
       4,   159,   165,    -1,    15,   166,    16,    -1,    15,   166,
      16,   171,    -1,   169,    -1,   170,    -1,   171,    -1,     4,
      -1,     4,   159,    -1,     4,   168,    -1,     4,   159,   168,
      -1,    15,   169,    16,    -1,    15,   170,    16,    -1,    15,
     171,    16,    -1,    15,   169,    16,   171,    -1,   172,    -1,
      15,    16,    -1,    15,   191,    16,    -1,    23,    24,    -1,
      23,   127,    24,    -1,   172,    23,   127,    24,    -1,   172,
      23,    24,    -1,   174,    -1,   176,    -1,   175,    -1,     4,
     173,    -1,     4,   159,   173,    -1,   176,   171,    -1,    15,
     174,    16,    -1,    15,   174,    16,   171,    -1,   247,    -1,
      15,   176,    16,    -1,   178,    -1,     4,   177,    -1,     4,
     159,   177,    -1,   176,    15,   179,    16,    -1,    15,   177,
      16,    -1,    15,   177,    16,   171,    -1,   247,    -1,   179,
      22,   247,    -1,   247,    -1,   248,    -1,   154,    -1,   154,
     168,    -1,   158,    -1,   158,   168,    -1,    -1,   183,    -1,
     184,    -1,   183,   184,    -1,   100,    15,    15,   185,    16,
      16,    -1,   186,    -1,   185,    22,   186,    -1,    -1,   187,
      -1,   187,    15,   126,    16,    -1,   247,    -1,   248,    -1,
      42,    -1,    -1,    88,   189,    -1,    20,   190,    21,    -1,
      20,   190,    22,    21,    -1,   124,    -1,   189,    -1,   190,
      22,   189,    -1,   192,    -1,   192,    22,    87,    -1,    87,
      -1,   193,    -1,   192,    22,   193,    -1,   193,    88,   189,
      -1,   192,    22,     1,    -1,   148,    -1,   148,   168,    -1,
     148,   173,    -1,   148,   165,    -1,   152,    -1,   152,   168,
      -1,   152,   173,    -1,   154,    -1,   154,   168,    -1,   154,
     173,    -1,   154,   165,    -1,   158,    -1,   158,   168,    -1,
     158,   173,    -1,   195,    20,   196,    21,    -1,   195,   180,
      20,   196,    21,    -1,   195,   180,    -1,   195,    20,    21,
      -1,   195,   180,    20,    21,    -1,    29,    -1,    41,    -1,
     197,    -1,   196,   197,    -1,   199,    19,    -1,   198,    19,
      -1,   158,   201,    -1,   198,    22,   201,    -1,   154,   200,
      -1,   199,    22,   200,    -1,   161,   202,   182,    -1,   203,
     182,    -1,   173,   202,   182,    -1,   203,   182,    -1,    -1,
     203,    -1,    25,   127,    -1,    35,    20,   205,   207,    21,
      -1,    35,   180,    20,   205,   207,    21,    -1,    35,   180,
      -1,   180,   206,    -1,   205,    22,   180,   206,    -1,    -1,
      88,   127,    -1,    -1,    22,    -1,   209,    -1,   210,    -1,
     216,    -1,   217,    -1,   218,    -1,   219,    -1,     1,    19,
      -1,   104,    -1,   247,    25,   208,    -1,    34,   127,    25,
     208,    -1,    50,    25,   208,    -1,   248,    25,   208,    -1,
     211,   212,    -1,   211,   214,   212,    -1,   211,   215,   212,
      -1,   211,   214,   215,   212,    -1,    20,    -1,    21,    -1,
      20,    21,    -1,    20,   214,    21,    -1,    20,   215,    21,
      -1,    20,   214,   215,    21,    -1,   129,    -1,   214,   129,
      -1,   208,    -1,   215,   208,    -1,   128,    19,    -1,    55,
      15,   126,    16,   208,    -1,    55,    15,   126,    16,   208,
      31,   208,    -1,    33,    15,   126,    16,   208,    -1,    57,
      15,   126,    16,   208,    -1,    54,   208,    57,    15,   126,
      16,    19,    -1,    47,    15,   128,    19,   128,    19,   128,
      16,   208,    -1,    47,    15,     1,    19,   128,    19,   128,
      16,   208,    -1,    47,    15,   128,    19,   128,    19,     1,
      16,   208,    -1,    47,    15,   128,    19,     1,    19,   128,
      16,   208,    -1,    47,    15,     1,    16,   208,    -1,    51,
     247,    19,    -1,    46,    19,    -1,    30,    19,    -1,    40,
     128,    19,    -1,    51,   248,    19,    -1,   221,    -1,   220,
     221,    -1,   129,    -1,   222,    -1,    -1,   173,   223,   213,
      -1,   173,    66,    -1,    -1,   148,   173,   224,   213,    -1,
      -1,   154,   173,   225,   213,    -1,    -1,    -1,    -1,   152,
     173,   226,   213,   227,   158,   173,   228,   213,    -1,    -1,
     177,   229,   213,    -1,    -1,   148,   177,   230,   213,    -1,
      -1,   154,   177,   231,   213,    -1,    -1,   152,   177,   232,
     213,    -1,    -1,   158,   177,   233,   213,    -1,    -1,   177,
     239,   234,   213,    -1,    -1,   148,   177,   239,   235,   213,
      -1,    -1,   154,   177,   239,   236,   213,    -1,    -1,   152,
     177,   239,   237,   213,    -1,    -1,   158,   177,   239,   238,
     213,    -1,    -1,   240,   214,    -1,    69,    -1,    70,    -1,
      71,    -1,    72,    -1,    74,    -1,    68,    -1,   242,    68,
      -1,    42,    -1,    53,    -1,    99,    -1,    42,    -1,    53,
      -1,    37,    -1,    39,    -1,    56,    -1,    26,    -1,    36,
      -1,    49,    -1,    38,    -1,    28,    -1,    43,    -1,    27,
      -1,    48,    -1,    45,    -1,    44,    -1,    32,    -1,    67,
      -1,    75,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   376,   376,   386,   393,   394,   396,   402,   408,   415,
     419,   431,   434,   437,   440,   443,   446,   450,   453,   457,
     461,   465,   469,   472,   475,   479,   482,   486,   488,   489,
     490,   491,   492,   495,   497,   501,   503,   506,   509,   513,
     515,   518,   522,   524,   527,   531,   533,   536,   539,   542,
     546,   548,   551,   555,   557,   561,   563,   569,   571,   577,
     579,   583,   585,   591,   593,   597,   599,   605,   609,   612,
     615,   618,   621,   624,   627,   630,   633,   636,   640,   643,
     655,   658,   662,   664,   699,   703,   707,   713,   722,   724,
     720,   731,   733,   730,   739,   741,   738,   749,   747,   756,
     754,   761,   768,   770,   766,   776,   778,   775,   784,   786,
     783,   794,   792,   801,   799,   806,   810,   816,   819,   825,
     830,   835,   839,   846,   851,   856,   863,   868,   873,   880,
     882,   885,   890,   892,   896,   902,   905,   909,   914,   919,
     923,   929,   934,   939,   946,   951,   956,   962,   964,   968,
     970,   975,   980,   986,   988,   989,   990,   999,  1002,  1005,
    1008,  1011,  1017,  1021,  1024,  1029,  1034,  1039,  1044,  1047,
    1055,  1058,  1061,  1066,  1070,  1075,  1077,  1078,  1082,  1087,
    1091,  1095,  1101,  1106,  1110,  1114,  1120,  1125,  1129,  1135,
    1140,  1144,  1149,  1161,  1163,  1167,  1170,  1173,  1178,  1182,
    1185,  1190,  1194,  1199,  1206,  1209,  1214,  1220,  1223,  1237,
    1247,  1258,  1260,  1264,  1269,  1273,  1277,  1285,  1288,  1292,
    1295,  1301,  1308,  1311,  1315,  1318,  1320,  1325,  1327,  1328,
    1332,  1334,  1338,  1340,  1341,  1345,  1350,  1357,  1360,  1365,
    1374,  1379,  1384,  1391,  1396,  1400,  1404,  1407,  1410,  1413,
    1417,  1420,  1423,  1427,  1430,  1433,  1436,  1440,  1452,  1456,
    1460,  1464,  1471,  1480,  1482,  1486,  1488,  1495,  1497,  1502,
    1507,  1513,  1518,  1525,  1531,  1538,  1544,  1551,  1553,  1557,
    1562,  1566,  1569,  1574,  1579,  1585,  1587,  1590,  1592,  1601,
    1605,  1608,  1611,  1614,  1617,  1628,  1630,  1637,  1641,  1644,
    1648,  1653,  1657,  1660,  1663,  1667,  1668,  1674,  1678,  1681,
    1684,  1689,  1693,  1701,  1706,  1715,  1720,  1724,  1727,  1731,
    1735,  1738,  1743,  1746,  1749,  1752,  1756,  1760,  1763,  1766,
    1770,  1784,  1788,  1792,  1809,  1825,  1822,  1829,  1836,  1835,
    1843,  1840,  1848,  1850,  1852,  1847,  1858,  1856,  1863,  1862,
    1870,  1869,  1877,  1876,  1885,  1884,  1892,  1891,  1899,  1898,
    1906,  1905,  1913,  1912,  1920,  1919,  1928,  1927,  1944,  1946,
    1947,  1948,  1949,  1953,  1955,  1964,  1966,  1967,  1970,  1972,
    1975,  1977,  1978,  1979,  1980,  1983,  1985,  1986,  1987,  1988,
    1990,  1991,  1993,  1994,  1997,  2001
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ctokAND", "ctokSTAR", "ctokPLUS", 
  "ctokMINUS", "ctokTILDE", "ctokNOT", "ctokLESS", "ctokGREATER", 
  "ctokOR", "ctokHAT", "ctokPERCENT", "ctokDIVIDE", "ctokLEFTPAREN", 
  "ctokRIGHTPAREN", "ctokDOT", "ctokQUESTION", "ctokSEMICOLON", 
  "ctokLEFTBRACE", "ctokRIGHTBRACE", "ctokCOMMA", "ctokLEFTBRACKET", 
  "ctokRIGHTBRACKET", "ctokCOLON", "ctokAUTO", "ctokDOUBLE", "ctokINT", 
  "ctokSTRUCT", "ctokBREAK", "ctokELSE", "ctokLONG", "ctokSWITCH", 
  "ctokCASE", "ctokENUM", "ctokREGISTER", "ctokTYPEDEF", "ctokCHAR", 
  "ctokEXTERN", "ctokRETURN", "ctokUNION", "ctokCONST", "ctokFLOAT", 
  "ctokSHORT", "ctokUNSIGNED", "ctokCONTINUE", "ctokFOR", "ctokSIGNED", 
  "ctokVOID", "ctokDEFAULT", "ctokGOTO", "ctokSIZEOF", "ctokVOLATILE", 
  "ctokDO", "ctokIF", "ctokSTATIC", "ctokWHILE", "ctokUPLUS", 
  "ctokUMINUS", "ctokINDIR", "ctokADDRESS", "ctokPOSTINC", "ctokPOSTDEC", 
  "ctokPREINC", "ctokPREDEC", "ctokBOGUS", "IDENTIFIER", "STRINGliteral", 
  "FLOATINGconstant", "INTEGERconstant", "OCTALconstant", "HEXconstant", 
  "WIDECHARconstant", "CHARACTERconstant", "TYPEDEFname", "ctokARROW", 
  "ctokICR", "ctokDECR", "ctokLS", "ctokRS", "ctokLE", "ctokGE", "ctokEQ", 
  "ctokNE", "ctokANDAND", "ctokOROR", "ctokELLIPSIS", "ctokEQUALS", 
  "ctokMULTassign", "ctokDIVassign", "ctokMODassign", "ctokPLUSassign", 
  "ctokMINUSassign", "ctokLSassign", "ctokRSassign", "ctokANDassign", 
  "ctokERassign", "ctokORassign", "ctokINLINE", "ctokATTRIBUTE", 
  "ctokMETA_TYPE_EXPR", "ctokMETA_TYPE_STMT", "ctokMETA_EXPR", 
  "ctokMETA_STMT", "$accept", "prog.start", "primary.expression", 
  "postfix.expression", "argument.expression.list", "unary.expression", 
  "unary.operator", "cast.expression", "multiplicative.expression", 
  "additive.expression", "shift.expression", "relational.expression", 
  "equality.expression", "AND.expression", "exclusive.OR.expression", 
  "inclusive.OR.expression", "logical.AND.expression", 
  "logical.OR.expression", "conditional.expression", 
  "assignment.expression", "assignment.operator", "expression", 
  "constant.expression", "expression.opt", "declaration", 
  "declaring.list", "@1", "@2", "@3", "@4", "@5", "@6", "@7", "@8", 
  "default.declaring.list", "@9", "@10", "@11", "@12", "@13", "@14", 
  "@15", "@16", "declaration.specifier", "basic.declaration.specifier", 
  "sue.declaration.specifier", "typedef.declaration.specifier", 
  "declaration.qualifier.list", "declaration.qualifier", "type.specifier", 
  "basic.type.specifier", "sue.type.specifier", "typedef.type.specifier", 
  "type.qualifier.list", "pointer.type.qualifier.list", 
  "elaborated.type.name", "declarator", "paren.typedef.declarator", 
  "paren.postfix.typedef.declarator", "simple.paren.typedef.declarator", 
  "parameter.typedef.declarator", "clean.typedef.declarator", 
  "clean.postfix.typedef.declarator", "abstract.declarator", 
  "unary.abstract.declarator", "postfix.abstract.declarator", 
  "postfixing.abstract.declarator", "array.abstract.declarator", 
  "identifier.declarator", "unary.identifier.declarator", 
  "postfix.identifier.declarator", "paren.identifier.declarator", 
  "old.function.declarator", "postfix.old.function.declarator", 
  "identifier.list", "identifier.or.typedef.name", "type.name", 
  "attributes.opt", "attributes", "attribute", "attribute.list", "attrib", 
  "any.word", "initializer.opt", "initializer", "initializer.list", 
  "parameter.type.list", "parameter.list", "parameter.declaration", 
  "struct.or.union.specifier", "struct.or.union", 
  "struct.declaration.list", "struct.declaration", 
  "struct.default.declaring.list", "struct.declaring.list", 
  "struct.declarator", "struct.identifier.declarator", 
  "bit.field.size.opt", "bit.field.size", "enum.specifier", 
  "enumerator.list", "enumerator.value.opt", "comma.opt", "statement", 
  "labeled.statement", "compound.statement", "ctokLEFT", "ctokRIGHT", 
  "compound.statement.no.new.scope", "declaration.list", "statement.list", 
  "expression.statement", "selection.statement", "iteration.statement", 
  "jump.statement", "translation.unit", "external.definition", 
  "function.definition", "@17", "@18", "@19", "@20", "@21", "@22", "@23", 
  "@24", "@25", "@26", "@27", "@28", "@29", "@30", "@31", "@32", 
  "old.function.declaration.list", "@33", "constant", 
  "string.literal.list", "type.qualifier", "pointer.type.qualifier", 
  "storage.class", "basic.type.name", "identifier", "typedefname", 0
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   105,   106,   107,   107,   107,   107,   107,   107,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   109,
     109,   110,   110,   110,   110,   110,   110,   111,   111,   111,
     111,   111,   111,   112,   112,   113,   113,   113,   113,   114,
     114,   114,   115,   115,   115,   116,   116,   116,   116,   116,
     117,   117,   117,   118,   118,   119,   119,   120,   120,   121,
     121,   122,   122,   123,   123,   124,   124,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   126,   126,
     126,   127,   128,   128,   129,   129,   129,   129,   131,   132,
     130,   133,   134,   130,   135,   136,   130,   137,   130,   138,
     130,   130,   140,   141,   139,   142,   143,   139,   144,   145,
     139,   146,   139,   147,   139,   139,   148,   148,   148,   149,
     149,   149,   149,   150,   150,   150,   151,   151,   151,   152,
     152,   152,   153,   153,   154,   154,   154,   155,   155,   155,
     155,   156,   156,   156,   157,   157,   157,   158,   158,   159,
     159,   160,   160,   161,   161,   161,   161,   162,   162,   162,
     162,   162,   163,   163,   163,   164,   164,   165,   165,   165,
     166,   166,   166,   167,   167,   168,   168,   168,   169,   169,
     169,   169,   170,   170,   170,   170,   171,   171,   171,   172,
     172,   172,   172,   173,   173,   174,   174,   174,   175,   175,
     175,   176,   176,   177,   177,   177,   178,   178,   178,   179,
     179,   180,   180,   181,   181,   181,   181,   182,   182,   183,
     183,   184,   185,   185,   186,   186,   186,   187,   187,   187,
     188,   188,   189,   189,   189,   190,   190,   191,   191,   191,
     192,   192,   192,   192,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   194,   194,
     194,   194,   194,   195,   195,   196,   196,   197,   197,   198,
     198,   199,   199,   200,   200,   201,   201,   202,   202,   203,
     204,   204,   204,   205,   205,   206,   206,   207,   207,   208,
     208,   208,   208,   208,   208,   208,   208,   209,   209,   209,
     209,   210,   210,   210,   210,   211,   212,   213,   213,   213,
     213,   214,   214,   215,   215,   216,   217,   217,   217,   218,
     218,   218,   218,   218,   218,   218,   219,   219,   219,   219,
     219,   220,   220,   221,   221,   223,   222,   222,   224,   222,
     225,   222,   226,   227,   228,   222,   229,   222,   230,   222,
     231,   222,   232,   222,   233,   222,   234,   222,   235,   222,
     236,   222,   237,   222,   238,   222,   240,   239,   241,   241,
     241,   241,   241,   242,   242,   243,   243,   243,   244,   244,
     245,   245,   245,   245,   245,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   247,   248
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     5,     6,     1,
       4,     3,     4,     3,     3,     2,     2,     3,     3,     1,
       3,     1,     2,     2,     2,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     5,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     0,     1,     2,     2,     2,     2,     0,     0,
       6,     0,     0,     6,     0,     0,     7,     0,     5,     0,
       5,     3,     0,     0,     6,     0,     0,     6,     0,     0,
       7,     0,     5,     0,     5,     3,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       5,     3,     3,     4,     4,     1,     3,     1,     2,     1,
       1,     2,     3,     3,     4,     1,     1,     1,     1,     2,
       2,     3,     3,     3,     3,     4,     1,     2,     3,     2,
       3,     4,     3,     1,     1,     1,     2,     3,     2,     3,
       4,     1,     3,     1,     2,     3,     4,     3,     4,     1,
       3,     1,     1,     1,     2,     1,     2,     0,     1,     1,
       2,     6,     1,     3,     0,     1,     4,     1,     1,     1,
       0,     2,     3,     4,     1,     1,     3,     1,     3,     1,
       1,     3,     3,     3,     1,     2,     2,     2,     1,     2,
       2,     1,     2,     2,     2,     1,     2,     2,     4,     5,
       2,     3,     4,     1,     1,     1,     2,     2,     2,     2,
       3,     2,     3,     3,     2,     3,     2,     0,     1,     2,
       5,     6,     2,     2,     4,     0,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     4,     3,
       3,     2,     3,     3,     4,     1,     1,     2,     3,     3,
       4,     1,     2,     1,     2,     2,     5,     7,     5,     5,
       7,     9,     9,     9,     9,     5,     3,     2,     2,     3,
       3,     1,     2,     1,     1,     0,     3,     2,     0,     4,
       0,     4,     0,     0,     0,     9,     0,     3,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,     0,   383,   389,   387,   263,   393,     0,   384,
     380,   386,   381,   264,   375,   388,   392,   391,   390,   385,
     376,   382,   394,   395,   377,     0,   333,     0,     0,     0,
     116,   117,   118,     0,     0,   134,   135,   136,     0,   141,
     335,   193,   195,   194,   366,   203,   151,     0,   152,     2,
     331,   334,   147,   129,   137,   201,   144,   378,   379,     0,
     196,   204,   149,     0,     0,     0,     0,   282,   211,   212,
       1,    84,     0,    85,     0,    97,     0,     0,    88,   153,
     157,   154,   169,   170,   155,   366,   167,   121,   132,   133,
     122,    86,   125,   128,   111,   131,   124,   102,   366,   120,
     127,    99,    91,   155,   366,   139,   119,   140,    87,   143,
     123,   146,   126,   113,   142,   105,   366,   148,   130,   138,
     145,   337,     0,     0,     0,   198,   186,     0,   356,     0,
       0,   260,   332,   197,   205,   150,   199,   202,   207,   285,
     287,     0,   101,    94,   155,   156,   115,     0,     0,   108,
     194,   217,     0,     0,   158,   171,     0,     0,     0,     0,
     165,   217,     0,     0,   358,     0,   168,   217,   217,     0,
       0,   362,   217,   217,     0,     0,   360,   217,   217,     0,
     364,     0,   336,   187,   239,   244,   117,   248,   251,   135,
     255,     0,     0,   237,   240,   209,    27,    28,    29,    30,
      31,    32,     0,   189,     0,   373,   368,   369,   370,   371,
     372,     0,     0,     9,    21,    33,     0,    35,    39,    42,
      45,    50,    53,    55,    57,    59,    61,    63,    81,     0,
       4,     5,     3,     0,   347,     0,   311,     0,     0,     0,
       0,   367,   261,     0,   134,   135,   136,     0,     0,   265,
       0,     0,     0,   200,   208,     0,   283,   288,     0,   287,
     217,     0,     0,   217,     0,   230,   218,   219,     0,     0,
     161,   172,     0,   162,     0,   173,    89,   339,   349,     0,
     230,   103,   343,   353,     0,   230,    92,   341,   351,     0,
     230,   106,   355,     0,     0,   305,   307,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,    80,   296,
      33,    65,    78,    83,     0,   313,   289,   290,     0,     0,
       0,   291,   292,   293,   294,     3,   144,   178,     0,   247,
     245,   175,   176,   177,   246,   178,     0,   249,   250,   254,
     252,   253,   256,   257,   206,     0,   188,     0,     0,     0,
     213,   215,     0,     0,     0,    25,     0,    22,    23,     0,
       0,     0,     0,    15,    16,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,   374,   192,     0,   357,
     102,   312,     0,   277,   271,   217,   277,   269,   217,   258,
     266,   268,     0,   267,     0,   262,     0,   286,   285,   280,
       0,    95,   109,     0,     0,    98,   220,   159,     0,   166,
     164,   163,   174,   230,   359,   112,   230,     0,   363,   100,
     230,   361,   114,   230,   365,   295,   328,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
       0,     0,   315,   306,   301,     0,     0,   308,     0,   309,
     314,     0,     0,   179,   180,     0,     0,     0,   179,   210,
     243,   238,   241,     0,   234,   242,     6,   178,     0,   214,
     216,     0,     0,     0,     0,    11,     0,    19,    13,    17,
       0,    14,    18,    36,    38,    37,    40,    41,    43,    44,
      46,    47,    48,    49,    51,    52,    54,    56,    58,    60,
       0,    62,   191,   279,   217,   278,   274,   217,   276,   270,
     272,   259,   284,   281,   230,   230,   224,   231,   160,    90,
     104,     0,    93,   107,     0,     0,   329,     0,     0,   299,
     326,   330,     0,     0,     0,    66,    79,   302,     0,   303,
     310,   297,   300,   181,   182,   183,   184,   235,     0,   179,
      34,     0,     0,    26,    12,     0,    10,     0,   273,   275,
      96,   110,   229,     0,   222,   225,   227,   228,   344,     0,
     298,     0,    82,     0,     0,     0,     0,   304,   185,   232,
       0,     0,     7,    20,    64,     0,   224,     0,     0,   318,
     325,     0,     0,     0,     0,   316,   319,   233,   236,     8,
     221,   223,     0,   345,    82,    82,     0,     0,     0,   226,
       0,     0,     0,     0,   320,   317,     0,     0,     0,     0,
     322,   324,   323,   321
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    25,   213,   214,   496,   310,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   311,   312,
     460,   313,   229,   314,   236,    27,   161,   423,   173,   430,
     260,   534,   151,   172,    28,   168,   426,   178,   433,   263,
     535,   167,   177,   237,    30,    31,    32,   238,    95,   239,
      35,    36,    37,   240,    59,    39,    78,    79,    80,   158,
      81,    82,    83,   474,   331,   332,   333,   126,    60,    41,
      42,   150,    65,    45,   191,   139,   352,   265,   266,   267,
     583,   584,   585,   415,   485,   568,   192,   193,   194,    46,
      47,   248,   249,   250,   251,   394,   397,   524,   395,    48,
     140,   256,   258,   315,   316,   317,   318,   464,   182,   241,
     320,   321,   322,   323,   324,    49,    50,    51,   122,   162,
     174,   169,   427,   608,   127,   163,   175,   170,   179,   235,
     279,   289,   284,   293,   128,   129,   230,   231,    52,    62,
      53,    54,   232,   446
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -421
static const short yypact[] =
{
    2451,   997,   240,  -421,  -421,  -421,  -421,  -421,    68,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,    16,  -421,    38,    90,   369,
    3041,   578,  1383,  1974,   555,  3041,   846,  1383,  2027,  -421,
     -59,  -421,  -421,   152,    58,  -421,  -421,   246,  -421,  2451,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,   997,
    -421,  -421,  -421,    95,   293,   109,   222,    71,  -421,  -421,
    -421,  -421,   620,  -421,   106,  -421,   882,    75,  -421,  -421,
    -421,  -421,  -421,  -421,   110,    49,   302,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,   137,   159,  -421,
    -421,  -421,  -421,   181,   252,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,   193,  -421,  -421,  -421,
    -421,  -421,   216,  2564,  2799,  -421,   226,   216,  -421,  3007,
    2653,   239,  -421,  -421,  -421,  -421,   302,  -421,   302,   180,
     271,   222,  -421,  -421,  -421,  -421,  -421,   998,   376,  -421,
     302,   211,    75,   889,  -421,  -421,    75,   299,   302,   310,
    -421,   211,   216,   216,  -421,  2617,  -421,   211,   211,   216,
     216,  -421,   211,   211,   216,   216,  -421,   211,   211,   216,
    -421,  1053,  -421,  -421,  -421,   460,  1383,  2398,   460,  1383,
    2398,    73,   318,   317,   260,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  1793,  -421,  2927,  -421,  -421,  -421,  -421,  -421,
    -421,  2940,  2940,  -421,  1389,  -421,  2954,  -421,    51,   140,
     138,    32,   348,   347,   353,   374,   304,    30,  -421,   363,
    -421,   359,  -421,  2823,  -421,   216,  -421,   369,  2080,   555,
    2133,  3007,  -421,   636,  1880,     9,     9,  2487,  2682,  -421,
     184,   336,  2718,  -421,  -421,  2954,  -421,   222,   388,   271,
     211,   998,   328,   211,   414,   352,   211,  -421,   331,    75,
    -421,  -421,   356,   302,   454,   302,  -421,  -421,  -421,   216,
     352,  -421,  -421,  -421,   216,   352,  -421,  -421,  -421,   216,
     352,  -421,  -421,   216,   444,  -421,  -421,   479,   467,  2954,
    1896,   480,   472,   469,   222,  1611,   492,   495,  -421,  -421,
    1654,  -421,  -421,   489,   501,  -421,  -421,  -421,  1134,  1215,
    1377,  -421,  -421,  -421,  -421,   499,   500,   754,  2239,  -421,
    -421,  -421,  -421,  -421,  -421,   885,  2292,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,   462,  -421,  2186,  2837,   148,
     111,  2529,   516,  1296,  1793,  -421,  1869,  -421,  -421,  2913,
     222,  1896,   222,  -421,  -421,  -421,  2954,  2954,  2954,  2954,
    2954,  2954,  2954,  2954,  2954,  2954,  2954,  2954,  2954,  2954,
    2954,  2954,  2954,  1896,  2954,  -421,  -421,  -421,   509,  -421,
    -421,  -421,  2954,   513,  -421,   211,   513,  -421,   211,  -421,
    -421,  -421,   179,  -421,   636,  -421,  2747,  -421,   180,  -421,
     522,  -421,  -421,   532,  2837,  -421,  -421,  -421,   377,  -421,
    -421,  -421,  -421,   352,  -421,  -421,   352,     9,  -421,  -421,
     352,  -421,  -421,   352,  -421,  -421,  -421,  1896,   526,   534,
    -421,   913,  1611,   535,   536,   503,   500,  1896,  1896,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    2954,  2954,  -421,  -421,  -421,  1134,  1455,  -421,  1533,  -421,
    -421,  1611,  1611,   754,  -421,   545,   551,   552,   885,  -421,
    -421,  -421,  -421,  2837,  -421,  -421,  -421,   558,  2345,  -421,
    -421,  2954,  1296,  1455,   559,  -421,   308,  -421,  -421,  -421,
     253,  -421,  -421,  -421,  -421,  -421,    51,    51,   140,   140,
     138,   138,   138,   138,    32,    32,   348,   347,   353,   374,
       1,   304,  -421,  -421,   211,  -421,  -421,   211,  -421,  -421,
    -421,  -421,  -421,  -421,   352,   352,    57,  -421,  -421,  -421,
    -421,   447,  -421,  -421,   381,  1611,  -421,   380,   557,  -421,
    -421,  -421,   562,   425,   458,  -421,  -421,  -421,  1455,  -421,
    -421,  -421,  -421,  -421,   302,  -421,  -421,  -421,   210,   558,
    -421,  1455,   566,  -421,  -421,  2954,  -421,  2954,  -421,  -421,
    -421,  -421,  -421,   463,  -421,   570,  -421,  -421,  -421,  1611,
    -421,  1611,  1896,  1689,  1896,  1611,  1611,  -421,  -421,  -421,
     955,   571,  -421,  -421,  -421,   574,    57,  1896,   216,  -421,
    -421,   567,   575,   579,   468,   543,  -421,  -421,  -421,  -421,
    -421,  -421,   475,  -421,  1896,  1896,  1716,   584,  1611,  -421,
     592,   597,   607,   610,  -421,  -421,  1611,  1611,  1611,  1611,
    -421,  -421,  -421,  -421
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -421,  -421,  -421,  -421,  -421,   464,  -421,  -115,    42,    77,
     190,    81,   250,   261,   251,   263,   258,  -421,  -100,  -219,
    -421,   196,  -194,    74,     4,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,    21,  -421,  -113,  -421,    31,     5,   407,
    -121,   523,   -74,    54,   -65,   248,    24,     7,  -421,   -89,
     -49,    10,  -421,   -68,  -306,  -296,   390,  -421,   176,    41,
    -421,   672,   707,  -421,  -421,    -3,   294,   123,  -421,   391,
    -421,    50,  -421,   259,  -389,  -421,  -421,  -421,   313,  -421,
    -421,   412,  -245,  -421,  -421,   266,   276,   283,  -172,  -421,
     540,   275,   427,   408,  -421,  -421,   -96,  -420,   -77,  -153,
     -31,  -421,  -421,  -421,  -421,  -421,   635,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,   341,  -421,  -421,  -421,   -18,   -53,
     831,   572,     0,   265
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -355
static const short yytable[] =
{
      55,    55,    55,   400,    26,    67,   135,   121,    68,   244,
     186,   153,    88,    88,    88,    88,    70,   105,   109,   111,
     117,    29,   475,   461,   228,   537,   577,   155,   319,    55,
     475,    33,   476,    55,    55,    87,    92,    93,    55,   388,
     476,   373,   374,    63,   131,   557,   559,    68,   383,    55,
     234,    14,   186,    26,    38,   366,   246,    71,   102,    55,
      72,   407,    20,   268,   367,   368,    68,   272,  -156,  -348,
      29,  -156,    55,   572,    55,   398,    55,    55,  -346,    76,
      33,   244,   261,   154,   157,   277,   278,   159,    66,   344,
     156,   141,   282,   283,   567,   345,   143,   287,   288,   582,
     135,   365,   292,    38,   271,   438,   353,   146,    24,    73,
     147,   136,    74,   375,   376,   487,   384,   330,    63,   337,
     340,   148,   342,   195,    22,   138,   488,   244,   246,   484,
    -338,   244,    23,   228,   124,    22,   329,  -156,   597,   339,
     497,    68,    22,    23,   185,   369,   370,    55,    55,  -156,
      23,   601,    55,    55,   187,   228,    55,  -342,   389,   157,
     270,   400,   159,   157,   486,   465,   159,   123,    88,    88,
     461,   109,   117,    22,   246,   124,    40,   190,   246,  -352,
     418,   325,   475,   147,   247,    55,   185,    55,    55,    63,
      55,    92,   476,    63,   148,   484,   187,    63,   523,   228,
     492,  -340,   424,   401,   392,    84,   402,   428,   135,    97,
     103,   618,   431,  -354,   115,   186,   434,   371,   372,   190,
      88,   525,   117,   186,   525,    40,   105,   109,   111,   117,
     398,   599,   600,   244,   186,   133,   181,    55,    55,    55,
      55,   555,   556,    55,     1,   391,    22,    55,   144,   233,
     149,   503,   504,   505,   408,     2,   351,    68,   353,   252,
     353,    55,   473,   102,   484,    56,   130,   393,   255,    55,
     478,  -156,  -350,    69,  -156,   461,   157,   576,   155,   159,
     246,    96,   489,   490,   276,   244,   114,   466,   468,    22,
     280,   281,   228,   257,    86,   285,   286,    23,   100,    86,
     290,   291,   247,   120,   443,   325,   247,    22,   123,   137,
      63,   264,    69,    22,    56,   273,   124,   165,   325,   325,
     325,    23,   493,   391,   574,   124,   275,    55,    55,   133,
     575,    69,   246,   117,   346,    55,    55,    86,   159,   347,
    -156,    86,   160,   165,   137,   479,   165,   417,   348,   185,
     379,   124,  -156,   325,   124,   403,   603,   185,   404,   187,
     498,   334,   501,   338,   341,   380,   343,   187,   185,    63,
      75,   165,   419,    76,   439,   186,   570,    63,   187,   124,
     147,   484,   190,   411,    77,   381,   412,   385,    56,   382,
     190,   148,   165,   538,    56,    56,   591,   589,   349,   592,
     124,   190,    55,   461,    55,   563,    69,    34,   351,   409,
     563,   506,   507,   144,   390,   144,   115,   160,    86,   144,
     135,   160,   569,   396,   271,   135,   164,   386,   393,   413,
      56,   377,   378,   125,   558,    96,    22,   133,   114,   171,
     414,   595,   325,    22,    23,   176,   326,   461,   508,   509,
      86,   147,   100,    86,   125,   120,    34,   180,   514,   515,
     247,   571,   148,   435,   327,   325,   325,    56,   325,   391,
     421,   325,   325,    55,   596,   328,   166,   604,    55,   605,
     461,   541,   437,   124,   627,   606,    96,   441,   114,    14,
     461,   629,   325,   325,   442,   114,   391,   461,   436,   440,
      20,   563,    86,   100,    86,   120,    56,   447,    86,   185,
     448,   461,   120,    56,    22,   548,   135,    56,   526,   187,
     462,   528,    69,   117,   471,   472,   253,    22,   254,    22,
     188,   623,   491,   522,   160,    23,   586,   243,   392,   425,
     125,    55,   190,   533,   429,   325,    24,   536,   274,   432,
     349,   545,   349,   546,   550,   551,   101,   500,   325,    76,
     552,   564,   487,   510,   511,   512,   513,   565,   566,   444,
      77,   325,   188,   488,   628,   573,   593,   594,   396,   520,
     144,   124,   602,   326,   326,   607,   624,   619,   215,   325,
     620,   325,    86,    56,   625,   325,   325,    91,   626,   114,
      57,    56,    90,   634,     3,    99,   586,   107,   636,   350,
     119,    58,    56,   637,     9,    10,   120,    12,   326,    56,
      14,   142,    22,   638,    76,   499,   639,   502,   325,   516,
      23,    20,   518,   544,    21,    77,   325,   325,   325,   325,
      76,   517,   521,   553,   554,   519,   189,   578,   494,   133,
     579,    77,   125,   245,   133,   243,   621,   416,   274,   243,
     482,   392,   274,   420,   406,   422,   611,   613,   355,    86,
     530,    56,    43,    43,    64,   357,   358,    24,   529,   527,
     215,   259,   539,   532,   132,   540,   410,    22,   189,   542,
       0,     0,   543,     0,     0,    23,     0,   215,   630,   631,
     633,    43,     0,    22,     0,    43,    43,    44,    61,     0,
      43,    23,     0,   445,     0,     0,     0,   588,   477,   215,
       0,    43,     0,     0,     0,   245,   477,     0,   470,     0,
     326,    43,     0,     0,     0,   188,    85,     0,    86,     0,
      98,   104,     0,   188,    43,   116,     0,     0,    43,    64,
       0,     0,     0,    56,   188,     0,    44,   326,   327,    99,
       0,   350,   119,   215,     0,     0,   134,     0,     0,   328,
       0,   245,     0,     0,     0,   245,     0,   124,     0,   145,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
     614,     0,     0,   580,   581,     0,    57,     0,     0,     0,
       0,   587,     0,   622,     0,     0,     0,    58,   274,     0,
      99,     0,   119,   243,     0,     0,   107,     0,     0,   119,
     262,    22,     0,     0,    64,    43,     0,     0,    64,    23,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,     0,   215,     0,
     549,   189,     0,     0,     0,     0,   215,     0,     0,   189,
     134,    89,    89,    89,    89,   108,   106,   110,   112,   118,
     189,   587,     3,     0,   470,     0,   470,   245,   477,   561,
     562,     0,     9,    10,     0,    12,    76,     0,    14,   335,
       0,     0,     0,    76,     0,   188,     0,   152,     0,    20,
     336,   470,    21,     0,   269,     0,     0,     0,   124,    43,
       0,    43,     0,     0,   547,    43,   196,   197,   198,   199,
     200,   201,     0,   119,    57,     0,     0,    57,   202,   245,
       0,    57,   -82,     0,     0,    58,     0,     0,    58,     0,
       0,    64,    58,     0,   145,    24,   145,     0,     0,    22,
     145,     0,    22,   590,   598,   215,    22,    23,   196,   197,
     198,   199,   200,   201,    23,   204,   470,     0,     0,     0,
     202,     0,     0,     0,     0,   483,   617,     0,     0,   470,
      22,   205,   206,   207,   208,   209,     0,   210,     0,     0,
     211,   212,     0,     0,     0,     0,     0,   609,     0,   610,
     262,     1,   147,   615,   616,     0,     0,   204,   262,     0,
       0,   189,     2,   148,     0,     0,   308,    89,    89,     0,
     110,   118,    22,   205,   206,   207,   208,   209,     0,   210,
       0,     0,   211,   212,     0,     0,   635,     0,     0,    57,
      57,   215,     0,     0,   640,   641,   642,   643,     0,     0,
      58,    58,     0,     0,   294,     0,   196,   197,   198,   199,
     200,   201,     0,     0,    22,    22,     0,     0,   202,    89,
       0,   118,   -82,   295,   296,     0,    43,     0,     0,     3,
       4,     5,     6,   297,     0,     7,   298,   299,     8,     9,
      10,    11,    12,   300,    13,    14,    15,    16,    17,   301,
     302,    18,    19,   303,   304,   204,    20,   305,   306,    21,
     307,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   205,   206,   207,   208,   209,     0,   210,    23,     0,
     211,   212,     0,     0,     0,   294,     0,   196,   197,   198,
     199,   200,   201,     0,     0,     0,     0,     0,     0,   202,
       0,     0,    24,   -82,   295,   463,   308,   309,     0,     0,
       3,     4,     5,     6,   297,     0,     7,   298,   299,     8,
       9,    10,    11,    12,   300,    13,    14,    15,    16,    17,
     301,   302,    18,    19,   303,   304,   204,    20,   305,   306,
      21,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,   205,   206,   207,   208,   209,     0,   210,    23,
       0,   211,   212,     0,     0,     0,   294,     0,   196,   197,
     198,   199,   200,   201,     0,     0,     0,     0,     0,     0,
     202,     0,     0,    24,   -82,   295,   467,   308,   309,     0,
       0,     3,     4,     5,     6,   297,     0,     7,   298,   299,
       8,     9,    10,    11,    12,   300,    13,    14,    15,    16,
      17,   301,   302,    18,    19,   303,   304,   204,    20,   305,
     306,    21,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,     0,   210,
      23,     0,   211,   212,     0,     0,     0,   294,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
       0,   202,     0,     0,    24,   -82,   295,     0,   308,   309,
       0,     0,     3,     4,     5,     6,   297,     0,     7,   298,
     299,     8,     9,    10,    11,    12,   300,    13,    14,    15,
      16,    17,   301,   302,    18,    19,   303,   304,   204,    20,
     305,   306,    21,   307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   205,   206,   207,   208,   209,     0,
     210,    23,     0,   211,   212,     0,     0,     0,   294,     0,
     196,   197,   198,   199,   200,   201,     0,     0,     0,     0,
       0,     0,   202,     0,     0,    24,   -82,   295,   469,   308,
     309,     0,     0,     0,   359,     0,   360,   297,     0,     3,
     298,   299,   361,     0,     0,     0,     0,   300,     0,     9,
      10,     0,    12,   301,   302,    14,     0,   303,   304,   204,
       0,   305,   306,     0,   307,     0,    20,     0,     0,    21,
       0,     0,     0,     0,    22,   205,   206,   207,   208,   209,
       0,   210,    23,     0,   211,   212,   294,     0,   196,   197,
     198,   199,   200,   201,     0,   362,   363,   364,     0,     0,
     202,     0,     0,     0,   -82,   295,   463,     0,     0,     0,
     308,   309,    24,     0,     0,   297,     0,     0,   298,   299,
       0,     0,     0,     0,     0,   300,     0,     0,     0,     0,
       0,   301,   302,     0,     0,   303,   304,   204,     0,   305,
     306,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,     0,   210,
      23,     0,   211,   212,   294,     0,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,     0,   202,     0,
       0,     0,   -82,   295,   560,     0,     0,     0,   308,   309,
       0,     0,     0,   297,     0,     0,   298,   299,     0,     0,
       0,     0,     0,   300,     0,     0,     0,     0,     0,   301,
     302,     0,     0,   303,   304,   204,     0,   305,   306,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   205,   206,   207,   208,   209,     0,   210,    23,     0,
     211,   212,   294,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,     0,   202,     0,     0,     0,
     -82,   295,     0,     0,     0,     0,   308,   309,     0,     0,
       0,   297,     0,     0,   298,   299,     0,     0,     0,     0,
       0,   300,     0,     0,     0,     0,     0,   301,   302,     0,
       0,   303,   304,   204,     0,   305,   306,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,   205,
     206,   207,   208,   209,     0,   210,    23,     0,   211,   212,
     612,     0,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,   202,     0,     0,     0,   -82,     0,
       0,     0,     0,     0,   308,   309,     0,   632,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
       0,   202,   -82,     0,     0,     0,     0,     0,     0,     0,
       0,   204,   449,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,     0,     0,     0,    22,   205,   206,   207,
     208,   209,     0,   210,     0,     0,   211,   212,   204,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   205,   206,   207,   208,   209,     0,
     210,     0,   308,   211,   212,     0,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,     0,   202,     0,
       0,     0,     0,   295,     0,     0,     0,     0,     0,   308,
       4,     5,     6,     0,     0,     7,     0,     0,     8,     0,
       0,    11,     0,     0,    13,    14,    15,    16,    17,     0,
       0,    18,    19,     0,     0,   204,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,   205,   206,   207,   208,   209,     0,   210,    23,     0,
     211,   212,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,   202,     0,     0,     0,     0,   295,
       0,     0,    24,     0,     0,     0,   308,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     4,     5,     0,
       0,   202,     7,     0,     0,     0,     0,     0,    11,     0,
       0,   204,    14,    15,    16,    17,     0,     0,    18,    19,
       0,     0,     0,    20,     0,     0,    22,   205,   206,   207,
     208,   209,     0,   210,     0,     0,   211,   212,   204,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   205,   206,   207,   208,   209,     0,
     210,     0,   308,   211,   212,    94,     0,     0,     1,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
       3,     4,     5,     6,     0,     0,     7,     0,     0,     8,
       9,    10,    11,    12,     0,    13,    14,    15,    16,    17,
       0,     0,    18,    19,     0,     0,     0,    20,   113,     0,
      21,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     2,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     3,     4,     5,     6,     0,     0,     7,
       0,     0,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,    17,    24,     0,    18,    19,     0,     0,     0,
      20,    94,     0,    21,   147,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,   148,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     3,     4,     5,     6,
       0,     0,     7,     0,     0,     8,     9,    10,    11,    12,
       0,    13,    14,    15,    16,    17,    24,     0,    18,    19,
       0,     0,     0,    20,   113,     0,    21,   147,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,   148,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     3,
       4,     5,     6,     0,     0,     7,     0,     0,     8,     9,
      10,    11,    12,     0,    13,    14,    15,    16,    17,    24,
       0,    18,    19,     0,     0,     0,    20,   480,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     3,     4,     5,     6,     0,     0,     7,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    24,     0,    18,    19,     0,     0,     0,    20,
       0,     0,    21,   327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   183,     0,     0,     0,     0,
       0,    23,   124,     0,     0,     3,     4,     5,     6,     0,
       0,     7,     0,   481,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    24,     0,    18,    19,     0,
       0,     0,    20,     0,     0,    21,   335,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,   336,   183,     0,
       0,     0,     0,     0,    23,   124,     0,     0,     3,     4,
       5,     6,     0,     0,     7,     0,   184,     8,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    24,     0,
      18,    19,     0,     0,     0,    20,     0,     0,    21,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
     488,   183,     0,     0,     0,     0,     0,    23,   124,     0,
       0,     3,     4,     5,     6,     0,     0,     7,     0,   184,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    24,     0,    18,    19,     0,     0,     0,    20,     0,
       0,    21,   335,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   336,     0,     0,     0,     0,     0,     0,
      23,   124,     0,     0,     3,     4,     5,     6,     0,     0,
       7,     0,   184,     8,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    24,     0,    18,    19,     0,     0,
       0,    20,     0,     0,    21,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     2,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     3,     4,     5,
       6,     0,     0,     7,     0,     0,     8,     9,    10,    11,
      12,   147,    13,    14,    15,    16,    17,    24,     0,    18,
      19,     0,   148,     0,    20,     0,     0,    21,     0,     0,
       0,     0,   392,     0,     4,     5,     6,     0,    22,     7,
       0,     0,     8,     0,     0,    11,    23,     0,    13,    14,
      15,    16,    17,   487,     0,    18,    19,     0,     0,     0,
      20,     0,     0,     0,   488,     0,     0,     0,     0,     0,
      24,     0,   124,     0,    22,     0,     4,     5,     6,     0,
       0,     7,    23,     0,     8,     0,     0,    11,     0,     0,
      13,    14,    15,    16,    17,     0,     0,    18,    19,     0,
     183,     0,    20,     0,     0,     0,    24,     0,     0,     0,
       3,     4,     5,     6,     0,     0,     7,     0,     0,     8,
       9,    10,    11,    12,    23,    13,    14,    15,    16,    17,
       0,     0,    18,    19,     0,     0,     0,    20,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,    22,     0,   183,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     3,     4,     5,     6,     0,     0,     7,
       0,   184,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,    17,    24,     0,    18,    19,     0,     0,     0,
      20,     0,     0,    21,   242,     0,     0,     0,     0,     0,
       4,     5,     6,     0,     0,     7,     0,     0,     8,     0,
       0,    11,    23,     0,    13,    14,    15,    16,    17,     0,
       0,    18,    19,   399,   184,     0,    20,     0,     0,     4,
       5,     6,     0,     0,     7,     0,    24,     8,     0,     0,
      11,     0,     0,    13,    14,    15,    16,    17,    23,     0,
      18,    19,     0,     0,     0,    20,     0,     0,     0,   405,
       0,     0,     0,     0,     0,     4,     5,     6,     0,     0,
       7,     0,    24,     8,     0,     0,    11,    23,     0,    13,
      14,    15,    16,    17,     0,     0,    18,    19,   531,     0,
       0,    20,     0,     0,     4,     5,     6,     0,     0,     7,
       0,    24,     8,     0,     0,    11,     0,     0,    13,    14,
      15,    16,    17,    23,     0,    18,    19,     0,     0,     0,
      20,     0,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,   202,     0,     0,    24,     0,     0,
       0,     0,    23,   203,     0,     0,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,     0,   202,     0,
     196,   197,   198,   199,   200,   201,    24,   387,     0,     0,
       0,   204,   202,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,    22,   205,   206,   207,
     208,   209,     0,   210,     0,   204,   211,   212,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   204,
      22,   205,   206,   207,   208,   209,     0,   210,     0,     0,
     211,   212,     0,     0,    22,   205,   206,   207,   208,   209,
       0,   210,     0,     0,   211,   212,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,     0,   202,   495,
     196,   197,   198,   199,   200,   201,     0,     0,     0,     0,
       0,     0,   354,   196,   197,   198,   199,   200,   201,     0,
       0,     0,     0,     0,     0,   356,     0,   196,   197,   198,
     199,   200,   201,     0,     0,   204,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   204,
      22,   205,   206,   207,   208,   209,     0,   210,     0,     0,
     211,   212,   204,     0,    22,   205,   206,   207,   208,   209,
       0,   210,     0,     0,   211,   212,   204,    22,   205,   206,
     207,   208,   209,     0,   210,     0,     0,   211,   212,     0,
       0,    22,   205,   206,   207,   208,   209,     0,   210,     0,
       0,   211,   212,     3,     4,     5,     6,     0,     0,     7,
       0,     0,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,    17,     0,     0,    18,    19,     0,     0,     0,
      20,     0,     0,    21,     0,     0,     0,     3,     4,     5,
       0,     0,     0,     7,     0,     0,     0,     9,    10,    11,
      12,     0,    23,    14,    15,    16,    17,     0,     0,    18,
      19,     0,     0,     0,    20,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24
};

static const short yycheck[] =
{
       0,     1,     2,   248,     0,     8,    59,    66,     8,   130,
     123,    76,    30,    31,    32,    33,     0,    35,    36,    37,
      38,     0,   328,    22,   124,   414,    25,    76,   181,    29,
     336,     0,   328,    33,    34,    30,    31,    32,    38,   233,
     336,     9,    10,     2,    47,   465,   466,    47,    18,    49,
     127,    42,   165,    49,     0,     4,   130,    19,    34,    59,
      22,   255,    53,   152,    13,    14,    66,   156,    19,    20,
      49,    22,    72,   493,    74,   247,    76,    77,    20,     4,
      49,   202,   147,    76,    77,   162,   163,    77,    20,    16,
      15,    20,   169,   170,   483,    22,    72,   174,   175,    42,
     153,   216,   179,    49,   153,   299,   202,     1,    99,    19,
       4,    16,    22,    81,    82,     4,    86,   185,    77,   187,
     188,    15,   190,   123,    67,    16,    15,   248,   202,   348,
      20,   252,    75,   233,    23,    67,   185,    88,   558,   188,
     359,   141,    67,    75,   123,     5,     6,   147,   148,   100,
      75,   571,   152,   153,   123,   255,   156,    20,   235,   152,
     153,   406,   152,   156,    16,   318,   156,    15,   186,   187,
      22,   189,   190,    67,   248,    23,     0,   123,   252,    20,
     269,   181,   488,     4,   130,   185,   165,   187,   188,   148,
     190,   186,   488,   152,    15,   414,   165,   156,   392,   299,
     353,    20,   279,    19,    25,    29,    22,   284,   261,    33,
      34,   600,   289,    20,    38,   328,   293,    79,    80,   165,
     238,   393,   240,   336,   396,    49,   244,   245,   246,   247,
     402,    21,    22,   354,   347,    59,    20,   237,   238,   239,
     240,   460,   461,   243,     4,   241,    67,   247,    72,    23,
      74,   366,   367,   368,   257,    15,   202,   257,   354,    20,
     356,   261,   327,   239,   483,     0,    20,   243,    88,   269,
     335,    19,    20,     8,    22,    22,   269,    24,   327,   269,
     354,    33,   350,   351,   161,   406,    38,   318,   319,    67,
     167,   168,   392,    22,    29,   172,   173,    75,    33,    34,
     177,   178,   248,    38,   304,   305,   252,    67,    15,    16,
     269,   100,    47,    67,    49,    16,    23,    15,   318,   319,
     320,    75,   353,   319,    16,    23,    16,   327,   328,   153,
      22,    66,   406,   351,    16,   335,   336,    72,   328,    22,
      88,    76,    77,    15,    16,   345,    15,    16,    88,   328,
       3,    23,   100,   353,    23,    19,   575,   336,    22,   328,
     360,   185,   362,   187,   188,    12,   190,   336,   347,   328,
       1,    15,    16,     4,   300,   488,   491,   336,   347,    23,
       4,   600,   328,   260,    15,    11,   263,    24,   123,    85,
     336,    15,    15,    16,   129,   130,    16,    16,   202,    19,
      23,   347,   402,    22,   404,   473,   141,     0,   354,    21,
     478,   369,   370,   237,   238,   239,   240,   152,   153,   243,
     473,   156,   487,   247,   473,   478,    85,    68,   404,    15,
     165,    83,    84,    43,   465,   187,    67,   261,   190,    98,
      88,    16,   442,    67,    75,   104,   181,    22,   371,   372,
     185,     4,   187,   188,    64,   190,    49,   116,   377,   378,
     406,   492,    15,    19,     4,   465,   466,   202,   468,   465,
      16,   471,   472,   473,    16,    15,    86,   577,   478,    16,
      22,   427,    15,    23,    16,    22,   238,    15,   240,    42,
      22,    16,   492,   493,    25,   247,   492,    22,    19,    19,
      53,   569,   237,   238,   239,   240,   241,    15,   243,   488,
      15,    22,   247,   248,    67,   441,   569,   252,   395,   488,
      19,   398,   257,   541,    25,    25,   136,    67,   138,    67,
     123,   608,    16,    24,   269,    75,   536,   130,    25,   280,
     150,   541,   488,    21,   285,   545,    99,    15,   158,   290,
     354,    25,   356,    19,    19,    19,     1,   361,   558,     4,
      57,    16,     4,   373,   374,   375,   376,    16,    16,   304,
      15,   571,   165,    15,    31,    16,    19,    15,   402,   383,
     404,    23,    16,   318,   319,    15,    19,    16,   124,   589,
      16,   591,   327,   328,    19,   595,   596,    19,    19,   351,
      42,   336,    30,    19,    26,    33,   606,    35,    16,   202,
      38,    53,   347,    16,    36,    37,   351,    39,   353,   354,
      42,     1,    67,    16,     4,   360,    16,   362,   628,   379,
      75,    53,   381,   437,    56,    15,   636,   637,   638,   639,
       4,   380,   384,   447,   448,   382,   123,   524,   354,   473,
     527,    15,   262,   130,   478,   248,   606,   266,   268,   252,
     347,    25,   272,   273,   252,   275,   592,   593,   204,   404,
     404,   406,     0,     1,     2,   211,   212,    99,   402,   396,
     216,   141,   423,   408,    49,   426,   259,    67,   165,   430,
      -1,    -1,   433,    -1,    -1,    75,    -1,   233,   624,   625,
     626,    29,    -1,    67,    -1,    33,    34,     0,     1,    -1,
      38,    75,    -1,   305,    -1,    -1,    -1,   541,   328,   255,
      -1,    49,    -1,    -1,    -1,   202,   336,    -1,   320,    -1,
     465,    59,    -1,    -1,    -1,   328,    29,    -1,   473,    -1,
      33,    34,    -1,   336,    72,    38,    -1,    -1,    76,    77,
      -1,    -1,    -1,   488,   347,    -1,    49,   492,     4,   187,
      -1,   354,   190,   299,    -1,    -1,    59,    -1,    -1,    15,
      -1,   248,    -1,    -1,    -1,   252,    -1,    23,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
     594,    -1,    -1,   534,   535,    -1,    42,    -1,    -1,    -1,
      -1,   536,    -1,   607,    -1,    -1,    -1,    53,   418,    -1,
     238,    -1,   240,   406,    -1,    -1,   244,    -1,    -1,   247,
     148,    67,    -1,    -1,   152,   153,    -1,    -1,   156,    75,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,    -1,   384,    -1,
     442,   328,    -1,    -1,    -1,    -1,   392,    -1,    -1,   336,
     153,    30,    31,    32,    33,    19,    35,    36,    37,    38,
     347,   606,    26,    -1,   466,    -1,   468,   354,   488,   471,
     472,    -1,    36,    37,    -1,    39,     4,    -1,    42,     4,
      -1,    -1,    -1,     4,    -1,   488,    -1,    15,    -1,    53,
      15,   493,    56,    -1,    15,    -1,    -1,    -1,    23,   237,
      -1,   239,    -1,    -1,     1,   243,     3,     4,     5,     6,
       7,     8,    -1,   351,    42,    -1,    -1,    42,    15,   406,
      -1,    42,    19,    -1,    -1,    53,    -1,    -1,    53,    -1,
      -1,   269,    53,    -1,   237,    99,   239,    -1,    -1,    67,
     243,    -1,    67,   545,   564,   491,    67,    75,     3,     4,
       5,     6,     7,     8,    75,    52,   558,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    20,    21,    -1,    -1,   571,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    -1,    -1,    -1,    -1,    -1,   589,    -1,   591,
     328,     4,     4,   595,   596,    -1,    -1,    52,   336,    -1,
      -1,   488,    15,    15,    -1,    -1,   103,   186,   187,    -1,
     189,   190,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    -1,    -1,   628,    -1,    -1,    42,
      42,   577,    -1,    -1,   636,   637,   638,   639,    -1,    -1,
      53,    53,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    67,    67,    -1,    -1,    15,   238,
      -1,   240,    19,    20,    21,    -1,   404,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,   404,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    -1,
      77,    78,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    99,    19,    20,    21,   103,   104,    -1,    -1,
      26,    27,    28,    29,    30,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    75,
      -1,    77,    78,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    99,    19,    20,    21,   103,   104,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    -1,    77,    78,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    99,    19,    20,    -1,   103,   104,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    -1,    77,    78,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    99,    19,    20,    21,   103,
     104,    -1,    -1,    -1,    15,    -1,    17,    30,    -1,    26,
      33,    34,    23,    -1,    -1,    -1,    -1,    40,    -1,    36,
      37,    -1,    39,    46,    47,    42,    -1,    50,    51,    52,
      -1,    54,    55,    -1,    57,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    75,    -1,    77,    78,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    76,    77,    78,    -1,    -1,
      15,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
     103,   104,    99,    -1,    -1,    30,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    50,    51,    52,    -1,    54,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      75,    -1,    77,    78,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,   103,   104,
      -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    50,    51,    52,    -1,    54,    55,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    -1,
      77,    78,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    20,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    -1,    77,    78,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,   103,    77,    78,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,   103,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,    -1,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    75,    -1,
      77,    78,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    27,    28,    -1,
      -1,    15,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    52,    42,    43,    44,    45,    -1,    -1,    48,    49,
      -1,    -1,    -1,    53,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,   103,    77,    78,     1,    -1,    -1,     4,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    39,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    48,    49,    -1,    -1,    -1,    53,     1,    -1,
      56,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    15,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,
      53,     1,    -1,    56,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    26,    27,    28,    29,
      -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,    39,
      -1,    41,    42,    43,    44,    45,    99,    -1,    48,    49,
      -1,    -1,    -1,    53,     1,    -1,    56,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    99,
      -1,    48,    49,    -1,    -1,    -1,    53,     1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    56,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    75,    23,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    87,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    99,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    15,    16,    -1,
      -1,    -1,    -1,    -1,    75,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    -1,    32,    -1,    87,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    99,    -1,
      48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      15,    16,    -1,    -1,    -1,    -1,    -1,    75,    23,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,    87,
      35,    36,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    99,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    87,    35,    36,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    99,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    56,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    -1,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,
      39,     4,    41,    42,    43,    44,    45,    99,    -1,    48,
      49,    -1,    15,    -1,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    25,    -1,    27,    28,    29,    -1,    67,    32,
      -1,    -1,    35,    -1,    -1,    38,    75,    -1,    41,    42,
      43,    44,    45,     4,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      99,    -1,    23,    -1,    67,    -1,    27,    28,    29,    -1,
      -1,    32,    75,    -1,    35,    -1,    -1,    38,    -1,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      16,    -1,    53,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,
      36,    37,    38,    39,    75,    41,    42,    43,    44,    45,
      -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
      -1,    67,    -1,    16,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    87,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    21,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,    75,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    21,    87,    -1,    53,    -1,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    99,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    75,    -1,
      48,    49,    -1,    -1,    -1,    53,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      32,    -1,    99,    35,    -1,    -1,    38,    75,    -1,    41,
      42,    43,    44,    45,    -1,    -1,    48,    49,    21,    -1,
      -1,    53,    -1,    -1,    27,    28,    29,    -1,    -1,    32,
      -1,    99,    35,    -1,    -1,    38,    -1,    -1,    41,    42,
      43,    44,    45,    75,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    75,    24,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
       3,     4,     5,     6,     7,     8,    99,    24,    -1,    -1,
      -1,    52,    15,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    52,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    -1,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    15,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    52,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      67,    68,    69,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    52,    -1,    67,    68,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    52,    67,    68,    69,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    -1,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    26,    27,    28,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    36,    37,    38,
      39,    -1,    75,    42,    43,    44,    45,    -1,    -1,    48,
      49,    -1,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     4,    15,    26,    27,    28,    29,    32,    35,    36,
      37,    38,    39,    41,    42,    43,    44,    45,    48,    49,
      53,    56,    67,    75,    99,   106,   129,   130,   139,   148,
     149,   150,   151,   152,   154,   155,   156,   157,   158,   160,
     173,   174,   175,   176,   177,   178,   194,   195,   204,   220,
     221,   222,   243,   245,   246,   247,   248,    42,    53,   159,
     173,   177,   244,   174,   176,   177,    20,   180,   247,   248,
       0,    19,    22,    19,    22,     1,     4,    15,   161,   162,
     163,   165,   166,   167,   173,   177,   248,   153,   243,   245,
     246,    19,   153,   153,     1,   153,   160,   173,   177,   246,
     248,     1,   161,   173,   177,   243,   245,   246,    19,   243,
     245,   243,   245,     1,   160,   173,   177,   243,   245,   246,
     248,    66,   223,    15,    23,   171,   172,   229,   239,   240,
      20,   180,   221,   173,   177,   244,    16,    16,    16,   180,
     205,    20,     1,   161,   173,   177,     1,     4,    15,   173,
     176,   137,    15,   159,   162,   165,    15,   162,   164,   166,
     248,   131,   224,   230,   239,    15,   171,   146,   140,   226,
     232,   239,   138,   133,   225,   231,   239,   147,   142,   233,
     239,    20,   213,    16,    87,   148,   150,   152,   154,   156,
     158,   179,   191,   192,   193,   247,     3,     4,     5,     6,
       7,     8,    15,    24,    52,    68,    69,    70,    71,    72,
      74,    77,    78,   107,   108,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   127,
     241,   242,   247,    23,   213,   234,   129,   148,   152,   154,
     158,   214,    21,   154,   155,   156,   157,   158,   196,   197,
     198,   199,    20,   171,   171,    88,   206,    22,   207,   205,
     135,   159,   176,   144,   100,   182,   183,   184,   164,    15,
     162,   165,   164,    16,   171,    16,   182,   213,   213,   235,
     182,   182,   213,   213,   237,   182,   182,   213,   213,   236,
     182,   182,   213,   238,     1,    20,    21,    30,    33,    34,
      40,    46,    47,    50,    51,    54,    55,    57,   103,   104,
     110,   123,   124,   126,   128,   208,   209,   210,   211,   214,
     215,   216,   217,   218,   219,   247,   248,     4,    15,   165,
     168,   169,   170,   171,   173,     4,    15,   168,   173,   165,
     168,   173,   168,   173,    16,    22,    16,    22,    88,   126,
     154,   158,   181,   211,    15,   110,    15,   110,   110,    15,
      17,    23,    76,    77,    78,   112,     4,    13,    14,     5,
       6,    79,    80,     9,    10,    81,    82,    83,    84,     3,
      12,    11,    85,    18,    86,    24,    68,    24,   127,   213,
     173,   129,    25,   161,   200,   203,   173,   201,   203,    21,
     197,    19,    22,    19,    22,    21,   196,   127,   180,    21,
     207,   182,   182,    15,    88,   188,   184,    16,   164,    16,
     171,    16,   171,   132,   213,   188,   141,   227,   213,   188,
     134,   213,   188,   143,   213,    19,    19,    15,   127,   128,
      19,    15,    25,   247,   248,   208,   248,    15,    15,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     125,    22,    19,    21,   212,   214,   215,    21,   215,    21,
     208,    25,    25,   159,   168,   169,   170,   171,   159,   247,
       1,    87,   193,    20,   124,   189,    16,     4,    15,   168,
     168,    16,   214,   215,   181,    16,   109,   124,   247,   248,
     126,   247,   248,   112,   112,   112,   113,   113,   114,   114,
     115,   115,   115,   115,   116,   116,   117,   118,   119,   120,
     126,   121,    24,   127,   202,   203,   182,   202,   182,   201,
     200,    21,   206,    21,   136,   145,    15,   189,    16,   188,
     188,   158,   188,   188,   126,    25,    19,     1,   128,   208,
      19,    19,    57,   126,   126,   124,   124,   212,   215,   212,
      21,   208,   208,   168,    16,    16,    16,   189,   190,   159,
     112,   215,   212,    16,    16,    22,    24,    25,   182,   182,
     188,   188,    42,   185,   186,   187,   247,   248,   173,    16,
     208,    16,    19,    19,    15,    16,    16,   212,   171,    21,
      22,   212,    16,   124,   123,    16,    22,    15,   228,   208,
     208,   128,     1,   128,   126,   208,   208,    21,   189,    16,
      16,   186,   126,   213,    19,    19,    19,    16,    31,    16,
     128,   128,     1,   128,    19,   208,    16,    16,    16,    16,
     208,   208,   208,   208
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
        case 3:
#line 388 "parser.y"
    { yyval.exprN = yyvsp[0].idN; }
    break;

  case 5:
#line 394 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 6:
#line 396 "parser.y"
    { if (yyvsp[-1].exprN->typ() == Comma) yyvsp[-1].exprN->coord(Coord(yyvsp[-2].tok));
                                  yyvsp[-1].exprN->parenthesized(true);
                                  yyval.exprN = yyvsp[-1].exprN; }
    break;

  case 7:
#line 403 "parser.y"
    { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
               yyval.exprN = (exprNode *) /* This is BAD */
		 new blockNode((decl_list *)0, yyvsp[-2].stmtL, Coord(yyvsp[-4].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 8:
#line 409 "parser.y"
    { if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
              yyval.exprN =  (exprNode *) /* This is BAD */
		new blockNode(yyvsp[-3].declL, yyvsp[-2].stmtL, Coord(yyvsp[-5].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 9:
#line 417 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 10:
#line 420 "parser.y"
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

  case 11:
#line 432 "parser.y"
    { yyval.exprN = new callNode(yyvsp[-2].exprN, (expr_list *)0, Coord(yyvsp[-1].tok)); }
    break;

  case 12:
#line 435 "parser.y"
    { yyval.exprN = new callNode(yyvsp[-3].exprN, yyvsp[-1].exprL, Coord(yyvsp[-2].tok)); }
    break;

  case 13:
#line 438 "parser.y"
    { yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 14:
#line 441 "parser.y"
    { yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 15:
#line 444 "parser.y"
    { yyval.exprN = new unaryNode( Operator::POSTINC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); }
    break;

  case 16:
#line 447 "parser.y"
    { yyval.exprN = new unaryNode( Operator::POSTDEC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); }
    break;

  case 17:
#line 451 "parser.y"
    { yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 18:
#line 454 "parser.y"
    { yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); }
    break;

  case 19:
#line 459 "parser.y"
    { yyval.exprL = new expr_list(); yyval.exprL->push_back(yyvsp[0].exprN); }
    break;

  case 20:
#line 462 "parser.y"
    { yyval.exprL = yyvsp[-2].exprL; yyval.exprL->push_back(yyvsp[0].exprN); }
    break;

  case 21:
#line 467 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 22:
#line 470 "parser.y"
    { yyval.exprN = new unaryNode( Operator::PREINC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 23:
#line 473 "parser.y"
    { yyval.exprN = new unaryNode( Operator::PREDEC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 24:
#line 476 "parser.y"
    { yyvsp[-1].unaryN->expr(yyvsp[0].exprN);
              yyval.exprN = yyvsp[-1].unaryN; }
    break;

  case 25:
#line 480 "parser.y"
    { yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 26:
#line 483 "parser.y"
    { yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[-1].typeN.ty, Coord(yyvsp[-3].tok)); }
    break;

  case 27:
#line 487 "parser.y"
    { yyval.unaryN = new unaryNode('&', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 28:
#line 488 "parser.y"
    { yyval.unaryN = new unaryNode('*', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 29:
#line 489 "parser.y"
    { yyval.unaryN = new unaryNode('+', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 30:
#line 490 "parser.y"
    { yyval.unaryN = new unaryNode('-', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 31:
#line 491 "parser.y"
    { yyval.unaryN = new unaryNode('~', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 32:
#line 492 "parser.y"
    { yyval.unaryN = new unaryNode('!', (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 34:
#line 498 "parser.y"
    { yyval.exprN = new castNode(yyvsp[-2].typeN.ty, yyvsp[0].exprN, false, Coord(yyvsp[-3].tok)); }
    break;

  case 36:
#line 504 "parser.y"
    { yyval.exprN = new binaryNode('*', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 37:
#line 507 "parser.y"
    { yyval.exprN = new binaryNode('/', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 38:
#line 510 "parser.y"
    { yyval.exprN = new binaryNode('%', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 40:
#line 516 "parser.y"
    { yyval.exprN = new binaryNode('+', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 41:
#line 519 "parser.y"
    { yyval.exprN = new binaryNode('-', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 43:
#line 525 "parser.y"
    { yyval.exprN = new binaryNode( Operator::LS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 44:
#line 528 "parser.y"
    { yyval.exprN = new binaryNode( Operator::RS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 46:
#line 534 "parser.y"
    { yyval.exprN = new binaryNode('<', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 47:
#line 537 "parser.y"
    { yyval.exprN = new binaryNode('>', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 48:
#line 540 "parser.y"
    { yyval.exprN = new binaryNode( Operator::LE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 49:
#line 543 "parser.y"
    { yyval.exprN = new binaryNode( Operator::GE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 51:
#line 549 "parser.y"
    { yyval.exprN = new binaryNode( Operator::EQ, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 52:
#line 552 "parser.y"
    { yyval.exprN = new binaryNode( Operator::NE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 54:
#line 558 "parser.y"
    { yyval.exprN = new binaryNode('&', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 56:
#line 564 "parser.y"
    { CBZ::WarnAboutPrecedence('^', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('^', yyvsp[0].exprN);
	      yyval.exprN = new binaryNode('^', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 58:
#line 572 "parser.y"
    { CBZ::WarnAboutPrecedence('|', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('|', yyvsp[0].exprN);
              yyval.exprN = new binaryNode('|', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 60:
#line 580 "parser.y"
    { yyval.exprN = new binaryNode( Operator::ANDAND, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 62:
#line 586 "parser.y"
    { CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[0].exprN);
              yyval.exprN = new binaryNode( Operator::OROR, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); }
    break;

  case 64:
#line 594 "parser.y"
    { yyval.exprN = new ternaryNode(yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-3].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 66:
#line 600 "parser.y"
    { yyvsp[-1].binaryN->left(yyvsp[-2].exprN);
	      yyvsp[-1].binaryN->right(yyvsp[0].exprN);
	      yyval.exprN = yyvsp[-1].binaryN; }
    break;

  case 67:
#line 607 "parser.y"
    { yyval.binaryN = new binaryNode('=', (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 68:
#line 610 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::MULTassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  }
    break;

  case 69:
#line 613 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::DIVassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 70:
#line 616 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::MODassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 71:
#line 619 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::PLUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  }
    break;

  case 72:
#line 622 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::MINUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); }
    break;

  case 73:
#line 625 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::LSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 74:
#line 628 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::RSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 75:
#line 631 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::ANDassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   }
    break;

  case 76:
#line 634 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::ERassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 77:
#line 637 "parser.y"
    { yyval.binaryN = new binaryNode( Operator::ORassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    }
    break;

  case 78:
#line 642 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 79:
#line 644 "parser.y"
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

  case 80:
#line 655 "parser.y"
    { yyval.exprN = new metaexprNode(yyvsp[0].idN); }
    break;

  case 81:
#line 659 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 82:
#line 663 "parser.y"
    { yyval.exprN = (exprNode *) 0; }
    break;

  case 83:
#line 664 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 84:
#line 701 "parser.y"
    { yyval.declL = yyvsp[-1].declL; }
    break;

  case 85:
#line 704 "parser.y"
    { yyval.declL = yyvsp[-1].declL; }
    break;

  case 86:
#line 708 "parser.y"
    { yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); }
    break;

  case 87:
#line 714 "parser.y"
    { yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); }
    break;

  case 88:
#line 722 "parser.y"
    { yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); }
    break;

  case 89:
#line 724 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 90:
#line 726 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN));
	    }
    break;

  case 91:
#line 731 "parser.y"
    { yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); }
    break;

  case 92:
#line 733 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 93:
#line 735 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 94:
#line 739 "parser.y"
    { yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); }
    break;

  case 95:
#line 741 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 96:
#line 743 "parser.y"
    { yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); }
    break;

  case 97:
#line 749 "parser.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 98:
#line 752 "parser.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 99:
#line 756 "parser.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 100:
#line 759 "parser.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 102:
#line 768 "parser.y"
    { yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); }
    break;

  case 103:
#line 770 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 104:
#line 772 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 105:
#line 776 "parser.y"
    { yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); }
    break;

  case 106:
#line 778 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 107:
#line 780 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); }
    break;

  case 108:
#line 784 "parser.y"
    { yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); }
    break;

  case 109:
#line 786 "parser.y"
    { yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 110:
#line 788 "parser.y"
    { yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); }
    break;

  case 111:
#line 794 "parser.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 112:
#line 797 "parser.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 113:
#line 801 "parser.y"
    { CBZ::SyntaxError(string("declaration without a variable")); }
    break;

  case 114:
#line 804 "parser.y"
    { yyval.declL = (decl_list *)0; /* empty list */ }
    break;

  case 116:
#line 812 "parser.y"
    { primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and(); 
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 117:
#line 817 "parser.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 118:
#line 820 "parser.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 119:
#line 827 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); }
    break;

  case 120:
#line 831 "parser.y"
    { yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; }
    break;

  case 121:
#line 836 "parser.y"
    { yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 122:
#line 840 "parser.y"
    { yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN); 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 123:
#line 848 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); }
    break;

  case 124:
#line 852 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 125:
#line 857 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 126:
#line 865 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 127:
#line 869 "parser.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; }
    break;

  case 128:
#line 874 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 130:
#line 883 "parser.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 131:
#line 886 "parser.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 134:
#line 898 "parser.y"
    { primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc;}
    break;

  case 135:
#line 903 "parser.y"
    { yyval.typeN = yyvsp[0].typeN; }
    break;

  case 137:
#line 911 "parser.y"
    { yyval.typeN.ty = yyvsp[0].primN;  
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 138:
#line 915 "parser.y"
    { yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 139:
#line 920 "parser.y"
    { yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 140:
#line 924 "parser.y"
    { yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN);
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 141:
#line 931 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty; 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 142:
#line 935 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord));
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 143:
#line 940 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 144:
#line 948 "parser.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, typeNode::NONE, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 145:
#line 952 "parser.y"
    { yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); }
    break;

  case 146:
#line 957 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); }
    break;

  case 148:
#line 965 "parser.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 150:
#line 971 "parser.y"
    { yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); }
    break;

  case 151:
#line 977 "parser.y"
    { yyval.typeN.ty = yyvsp[0].sueN; 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 152:
#line 981 "parser.y"
    { yyval.typeN.ty = yyvsp[0].enumN;  
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 156:
#line 991 "parser.y"
    {
	      CBZ::Warning(2, string("function prototype parameters must have types"));
              /* SZG Don't need this? $$ = AddDefaultParameterTypes(); */
            }
    break;

  case 158:
#line 1003 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); }
    break;

  case 159:
#line 1006 "parser.y"
    { yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-3].tok)), declNode::NONE, Redecl); }
    break;

  case 160:
#line 1009 "parser.y"
    { yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(yyvsp[-3].tq.tq,(typeNode *)0, Coord(yyvsp[-4].tok)), yyvsp[-3].tq.sc, Redecl); }
    break;

  case 161:
#line 1012 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq,(typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); }
    break;

  case 162:
#line 1019 "parser.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 163:
#line 1022 "parser.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[-1].typeN.ty); }
    break;

  case 164:
#line 1025 "parser.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 165:
#line 1031 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 166:
#line 1035 "parser.y"
    { yyval.declN = yyvsp[-1].declN;  }
    break;

  case 167:
#line 1041 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 168:
#line 1045 "parser.y"
    { yyval.declN = new declNode(yyvsp[-1].idN, declNode::NONE, yyvsp[0].typeN.ty, (exprNode *)0, (exprNode *)0); }
    break;

  case 171:
#line 1059 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); }
    break;

  case 172:
#line 1062 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); }
    break;

  case 173:
#line 1068 "parser.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 174:
#line 1071 "parser.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 178:
#line 1084 "parser.y"
    { yyval.typeN.ty = new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[0].tok)); 
	      yyval.typeN.sc = declNode::NONE; }
    break;

  case 179:
#line 1088 "parser.y"
    { yyval.typeN.ty = new ptrNode(yyvsp[0].tq.tq, (typeNode *)0, Coord(yyvsp[-1].tok));  
	      yyval.typeN.sc = yyvsp[0].tq.sc; }
    break;

  case 180:
#line 1092 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)));   
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 181:
#line 1096 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)));   
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 182:
#line 1103 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 183:
#line 1107 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 184:
#line 1111 "parser.y"
    { yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; }
    break;

  case 185:
#line 1115 "parser.y"
    { yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(yyvsp[0].typeN.ty); 
	      yyval.typeN.sc = merge_sc(yyvsp[-2].typeN.sc, yyvsp[0].typeN.sc, yyvsp[-2].typeN.ty->coord()); }
    break;

  case 186:
#line 1122 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty;
	       yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 187:
#line 1126 "parser.y"
    { yyval.typeN.ty = new funcNode(typeNode::NONE, (decl_list *)0, (typeNode *)0, Coord(yyvsp[-1].tok)); 
	       yyval.typeN.sc = declNode::NONE; }
    break;

  case 188:
#line 1130 "parser.y"
    { yyval.typeN.ty = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok)); 
	       yyval.typeN.sc = declNode::NONE; }
    break;

  case 189:
#line 1137 "parser.y"
    { yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, (exprNode *)0, Coord(yyvsp[-1].tok));
              yyval.typeN.sc = declNode::NONE; }
    break;

  case 190:
#line 1141 "parser.y"
    { yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok)); 
              yyval.typeN.sc = declNode::NONE; }
    break;

  case 191:
#line 1145 "parser.y"
    { yyval.typeN.ty = yyvsp[-3].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok))); 
              yyval.typeN.sc = yyvsp[-3].typeN.sc; }
    break;

  case 192:
#line 1151 "parser.y"
    { 
              CBZ::SyntaxError(string("array declaration with illegal empty dimension"));
              yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0,
							     (exprNode *) new constNode(constant((int)1)),
							     Coord(yyvsp[-1].tok)));
	      yyval.typeN.sc = declNode::NONE;
            }
    break;

  case 196:
#line 1171 "parser.y"
    { yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok))); }
    break;

  case 197:
#line 1174 "parser.y"
    { yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok))); }
    break;

  case 198:
#line 1180 "parser.y"
    { yyval.declN = yyvsp[-1].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 199:
#line 1183 "parser.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 200:
#line 1186 "parser.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 201:
#line 1192 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); }
    break;

  case 202:
#line 1195 "parser.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 203:
#line 1201 "parser.y"
    { 
/*              OldStyleFunctionDefinition = TRUE; */
              yyval.declN = yyvsp[0].declN; 
            }
    break;

  case 204:
#line 1207 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, SU); }
    break;

  case 205:
#line 1210 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), declNode::NONE, SU); }
    break;

  case 206:
#line 1216 "parser.y"
    { funcNode *f = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok));
	      f->is_knr (true);
	      yyval.declN = yyvsp[-3].declN->modify_type_and(f); }
    break;

  case 207:
#line 1221 "parser.y"
    { yyval.declN = yyvsp[-1].declN; }
    break;

  case 208:
#line 1224 "parser.y"
    { yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); }
    break;

  case 209:
#line 1239 "parser.y"
    { yyval.declL = new decl_list();
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    }
    break;

  case 210:
#line 1248 "parser.y"
    { yyval.declL = yyvsp[-2].declL;
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    }
    break;

  case 213:
#line 1266 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc; }
    break;

  case 214:
#line 1270 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty)->finish_and(); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord()); }
    break;

  case 215:
#line 1274 "parser.y"
    { yyval.typeN.ty = new primNode(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = yyvsp[0].tq.sc; }
    break;

  case 216:
#line 1278 "parser.y"
    { yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); }
    break;

  case 217:
#line 1287 "parser.y"
    { yyval.attribL = (attrib_list *)0; }
    break;

  case 218:
#line 1289 "parser.y"
    { yyval.attribL = yyvsp[0].attribL; }
    break;

  case 219:
#line 1294 "parser.y"
    { yyval.attribL = yyvsp[0].attribL; }
    break;

  case 220:
#line 1296 "parser.y"
    { yyval.attribL = yyvsp[-1].attribL; 
		  yyvsp[-1].attribL->splice(yyvsp[-1].attribL->end(), * yyvsp[0].attribL); 
		  delete yyvsp[0].attribL; }
    break;

  case 221:
#line 1303 "parser.y"
    { if (CBZ::ANSIOnly)
	            CBZ::SyntaxError(string("__attribute__ not allowed with -ansi switch"));
                  yyval.attribL = yyvsp[-2].attribL; }
    break;

  case 222:
#line 1310 "parser.y"
    { yyval.attribL = new attrib_list(); yyval.attribL->push_back(yyvsp[0].attribN); }
    break;

  case 223:
#line 1312 "parser.y"
    { yyval.attribL = yyvsp[-2].attribL; yyval.attribL->push_back(yyvsp[0].attribN); }
    break;

  case 224:
#line 1317 "parser.y"
    { yyval.attribN = 0; }
    break;

  case 225:
#line 1319 "parser.y"
    { yyval.attribN = new attribNode(yyvsp[0].idN, (exprNode *)0); }
    break;

  case 226:
#line 1321 "parser.y"
    { yyval.attribN = new attribNode(yyvsp[-3].idN, yyvsp[-1].exprN); }
    break;

  case 229:
#line 1328 "parser.y"
    { yyval.idN = new idNode("const", Coord(yyvsp[0].tok)); }
    break;

  case 230:
#line 1333 "parser.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 231:
#line 1334 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 232:
#line 1339 "parser.y"
    { yyval.exprN = yyvsp[-1].initializerN; yyval.exprN->coord(yyvsp[-2].tok); }
    break;

  case 233:
#line 1340 "parser.y"
    { yyval.exprN = yyvsp[-2].initializerN; yyval.exprN->coord(yyvsp[-3].tok); }
    break;

  case 234:
#line 1341 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 235:
#line 1347 "parser.y"
    { yyval.initializerN = new initializerNode(new expr_list(), yyvsp[0].exprN->coord());
	      yyval.initializerN->add_expr(yyvsp[0].exprN); }
    break;

  case 236:
#line 1351 "parser.y"
    { yyval.initializerN = yyvsp[-2].initializerN;
	      yyval.initializerN->add_expr(yyvsp[0].exprN);
            }
    break;

  case 238:
#line 1361 "parser.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(new declNode((typeNode *) new primNode(basic_type::Ellipsis), declNode::NONE)); }
    break;

  case 239:
#line 1366 "parser.y"
    { /* primNode * n = new primNode(qualifier(), basic_type::Void); */
	      CBZ::SyntaxError(Coord(yyvsp[0].tok), string("First argument cannot be `...'"));
              yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(new primNode(typeNode::NONE, basic_type::Void), declNode::NONE));
            }
    break;

  case 240:
#line 1376 "parser.y"
    { yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[0].declN); }
    break;

  case 241:
#line 1380 "parser.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN); }
    break;

  case 242:
#line 1385 "parser.y"
    { 
	      CBZ::SyntaxError(yyvsp[-2].declN->coord(), string("formals cannot have initializers"));
	      yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[-2].declN);
            }
    break;

  case 243:
#line 1392 "parser.y"
    { yyval.declL = yyvsp[-2].declL; }
    break;

  case 244:
#line 1398 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); }
    break;

  case 245:
#line 1401 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); }
    break;

  case 246:
#line 1405 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 247:
#line 1408 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 248:
#line 1411 "parser.y"
    { yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); }
    break;

  case 249:
#line 1414 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); }
    break;

  case 250:
#line 1418 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); }
    break;

  case 251:
#line 1421 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); }
    break;

  case 252:
#line 1424 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); }
    break;

  case 253:
#line 1428 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 254:
#line 1431 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); }
    break;

  case 255:
#line 1434 "parser.y"
    { yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); }
    break;

  case 256:
#line 1437 "parser.y"
    { yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); }
    break;

  case 257:
#line 1441 "parser.y"
    { yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); }
    break;

  case 258:
#line 1454 "parser.y"
    { yyval.sueN = yyvsp[-3].sueN->set_name_fields_and((idNode *)0, yyvsp[-1].declL, yyvsp[-2].tok, yyvsp[0].tok); }
    break;

  case 259:
#line 1458 "parser.y"
    { yyval.sueN = yyvsp[-4].sueN->set_name_fields_and(yyvsp[-3].idN, yyvsp[-1].declL, yyvsp[-2].tok, yyvsp[0].tok); }
    break;

  case 260:
#line 1461 "parser.y"
    { yyval.sueN = yyvsp[-1].sueN->set_name_and(yyvsp[0].idN, yyvsp[-1].sueN->coord()); }
    break;

  case 261:
#line 1465 "parser.y"
    { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-2].sueN->set_name_fields_and((idNode *)0, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].tok);
	    }
    break;

  case 262:
#line 1472 "parser.y"
    { 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-3].sueN->set_name_fields_and(yyvsp[-2].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].tok);
	    }
    break;

  case 263:
#line 1481 "parser.y"
    { yyval.sueN = new structNode(Coord(yyvsp[0].tok)); }
    break;

  case 264:
#line 1482 "parser.y"
    { yyval.sueN = new unionNode(Coord(yyvsp[0].tok)); }
    break;

  case 266:
#line 1489 "parser.y"
    { yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyval.declL->end(), * yyvsp[0].declL);
	      delete yyvsp[0].declL; }
    break;

  case 269:
#line 1504 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, SU)); }
    break;

  case 270:
#line 1508 "parser.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); }
    break;

  case 271:
#line 1515 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, SU)); }
    break;

  case 272:
#line 1519 "parser.y"
    { yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); }
    break;

  case 273:
#line 1527 "parser.y"
    { yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 274:
#line 1532 "parser.y"
    { yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 275:
#line 1540 "parser.y"
    { yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 276:
#line 1545 "parser.y"
    { yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); }
    break;

  case 277:
#line 1552 "parser.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 279:
#line 1558 "parser.y"
    { yyval.exprN = yyvsp[0].exprN; }
    break;

  case 280:
#line 1564 "parser.y"
    { yyval.enumN = new enumNode((idNode *)0, yyvsp[-2].declL, yyvsp[-4].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); }
    break;

  case 281:
#line 1567 "parser.y"
    { yyval.enumN = new enumNode(yyvsp[-4].idN, yyvsp[-2].declL, yyvsp[-5].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok));   }
    break;

  case 282:
#line 1570 "parser.y"
    { yyval.enumN = new enumNode(yyvsp[0].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].idN->coord(), yyvsp[0].idN->coord()); }
    break;

  case 283:
#line 1576 "parser.y"
    { yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); }
    break;

  case 284:
#line 1580 "parser.y"
    { yyval.declL = yyvsp[-3].declL; 
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); }
    break;

  case 285:
#line 1586 "parser.y"
    { yyval.exprN = (exprNode *)0; }
    break;

  case 286:
#line 1587 "parser.y"
    { yyval.exprN = yyvsp[0].exprN;   }
    break;

  case 287:
#line 1591 "parser.y"
    { }
    break;

  case 288:
#line 1592 "parser.y"
    { }
    break;

  case 289:
#line 1603 "parser.y"
    { yyval.stmtN = yyvsp[0].targetN; }
    break;

  case 290:
#line 1606 "parser.y"
    { yyval.stmtN = yyvsp[0].blockN; }
    break;

  case 291:
#line 1609 "parser.y"
    { yyval.stmtN = yyvsp[0].exprstmtN; }
    break;

  case 292:
#line 1612 "parser.y"
    { yyval.stmtN = yyvsp[0].stmtN; }
    break;

  case 293:
#line 1615 "parser.y"
    { yyval.stmtN = yyvsp[0].loopN; }
    break;

  case 294:
#line 1618 "parser.y"
    { yyval.stmtN = yyvsp[0].jumpN; }
    break;

  case 295:
#line 1629 "parser.y"
    {  yyval.stmtN = (stmtNode *) 0; }
    break;

  case 296:
#line 1630 "parser.y"
    { yyval.stmtN = new metastmtNode(yyvsp[0].idN); }
    break;

  case 297:
#line 1639 "parser.y"
    { yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); }
    break;

  case 298:
#line 1642 "parser.y"
    { yyval.targetN = new caseNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-3].tok)); }
    break;

  case 299:
#line 1645 "parser.y"
    { yyval.targetN = new caseNode(0, yyvsp[0].stmtN, Coord(yyvsp[-2].tok)); }
    break;

  case 300:
#line 1649 "parser.y"
    { yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); }
    break;

  case 301:
#line 1655 "parser.y"
    { yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); }
    break;

  case 302:
#line 1658 "parser.y"
    { yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 303:
#line 1661 "parser.y"
    { yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 304:
#line 1664 "parser.y"
    { yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); }
    break;

  case 305:
#line 1667 "parser.y"
    { yyval.tok = yyvsp[0].tok; CBZ::current_unit->enter_scope(); }
    break;

  case 306:
#line 1668 "parser.y"
    { yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_scope(); }
    break;

  case 307:
#line 1676 "parser.y"
    { yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); }
    break;

  case 308:
#line 1679 "parser.y"
    { yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 309:
#line 1682 "parser.y"
    { yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); }
    break;

  case 310:
#line 1685 "parser.y"
    { yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); }
    break;

  case 311:
#line 1691 "parser.y"
    { yyval.declL = yyvsp[0].declL; }
    break;

  case 312:
#line 1694 "parser.y"
    { yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyvsp[-1].declL->end(), * yyvsp[0].declL); 
	      delete yyvsp[0].declL;
	    }
    break;

  case 313:
#line 1703 "parser.y"
    { yyval.stmtL = new stmt_list();
              yyval.stmtL->push_back(yyvsp[0].stmtN); }
    break;

  case 314:
#line 1707 "parser.y"
    { yyval.stmtL = yyvsp[-1].stmtL;
              yyval.stmtL->push_back(yyvsp[0].stmtN); }
    break;

  case 315:
#line 1717 "parser.y"
    { yyval.exprstmtN = new exprstmtNode(yyvsp[-1].exprN); }
    break;

  case 316:
#line 1722 "parser.y"
    { yyval.stmtN = new ifNode(yyvsp[-2].exprN, yyvsp[0].stmtN, 0, Coord(yyvsp[-4].tok)); }
    break;

  case 317:
#line 1725 "parser.y"
    { yyval.stmtN = new ifNode (yyvsp[-4].exprN, yyvsp[-2].stmtN, yyvsp[0].stmtN, Coord(yyvsp[-6].tok), Coord(yyvsp[-1].tok)); }
    break;

  case 318:
#line 1728 "parser.y"
    { yyval.stmtN = new switchNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 319:
#line 1733 "parser.y"
    { yyval.loopN = new whileNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 320:
#line 1736 "parser.y"
    { yyval.loopN = new doNode(yyvsp[-5].stmtN, yyvsp[-2].exprN, Coord(yyvsp[-6].tok), Coord(yyvsp[-4].tok)); }
    break;

  case 321:
#line 1739 "parser.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 322:
#line 1744 "parser.y"
    { yyval.loopN = new forNode((exprNode *)0, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 323:
#line 1747 "parser.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 324:
#line 1750 "parser.y"
    { yyval.loopN = new forNode(yyvsp[-6].exprN, (exprNode *)0, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); }
    break;

  case 325:
#line 1753 "parser.y"
    { yyval.loopN = new forNode((exprNode *)0, (exprNode *)0, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); }
    break;

  case 326:
#line 1758 "parser.y"
    { yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); }
    break;

  case 327:
#line 1761 "parser.y"
    { yyval.jumpN = new continueNode(Coord(yyvsp[-1].tok)); }
    break;

  case 328:
#line 1764 "parser.y"
    { yyval.jumpN = new breakNode(Coord(yyvsp[-1].tok)); }
    break;

  case 329:
#line 1767 "parser.y"
    { yyval.jumpN = new returnNode(yyvsp[-1].exprN, procNode::current(), Coord(yyvsp[-2].tok)); }
    break;

  case 330:
#line 1771 "parser.y"
    { yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); }
    break;

  case 331:
#line 1786 "parser.y"
    { if (yyvsp[0].defL != 0) CBZ::current_unit->merge_in(yyvsp[0].defL); }
    break;

  case 332:
#line 1789 "parser.y"
    { CBZ::current_unit->merge_in(yyvsp[0].defL); }
    break;

  case 333:
#line 1794 "parser.y"
    {
              if (yydebug)
                {
                  /* printf("external.definition # declaration\n");
                  PrintNode(stdout, FirstItem(), 0); 
                  printf("\n\n\n"); */
		}
              yyval.defL = new def_list();
	      while (! yyvsp[0].declL->empty()) {
		defNode * d = yyvsp[0].declL->front();
		yyvsp[0].declL->pop_front();
		yyval.defL->push_front(d);
	      }
	      delete yyvsp[0].declL;
            }
    break;

  case 334:
#line 1810 "parser.y"
    { 
              if (yydebug)
                {
                  printf("external.definition # function.definition\n");
                  // PrintNode(stdout, , 0); 
                  printf("\n\n\n");
                }
              yyval.defL = new def_list();
	      yyval.defL->push_back(yyvsp[0].procN);
            }
    break;

  case 335:
#line 1825 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); }
    break;

  case 336:
#line 1827 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 337:
#line 1833 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[-1].declN); }
    break;

  case 338:
#line 1836 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); }
    break;

  case 339:
#line 1838 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 340:
#line 1843 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); }
    break;

  case 341:
#line 1845 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 342:
#line 1848 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)); }
    break;

  case 343:
#line 1850 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 344:
#line 1852 "parser.y"
    { yyval.procN = new procNode(false, yyvsp[-5].declN->set_type_and(new primNode(yyvsp[-6].tq.tq), yyvsp[-6].tq.sc, Redecl)); }
    break;

  case 345:
#line 1854 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[-5].blockN); }
    break;

  case 346:
#line 1858 "parser.y"
    { yyval.procN = new procNode(true, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); }
    break;

  case 347:
#line 1860 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 348:
#line 1863 "parser.y"
    {  yyval.procN = new procNode(true, 
				 yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				     add_parameter_types_and((decl_list *)0)); }
    break;

  case 349:
#line 1867 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 350:
#line 1870 "parser.y"
    { yyval.procN = new procNode(true, 
				yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); }
    break;

  case 351:
#line 1874 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 352:
#line 1877 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0));
            }
    break;

  case 353:
#line 1882 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 354:
#line 1885 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); }
    break;

  case 355:
#line 1889 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 356:
#line 1892 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-1].declN->coord()), declNode::NONE, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); }
    break;

  case 357:
#line 1896 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 358:
#line 1899 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); }
    break;

  case 359:
#line 1903 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 360:
#line 1906 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); }
    break;

  case 361:
#line 1910 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 362:
#line 1913 "parser.y"
    { yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); }
    break;

  case 363:
#line 1917 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 364:
#line 1920 "parser.y"
    { yyval.procN = new procNode(true, yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				          add_parameter_types_and(yyvsp[0].declL)); }
    break;

  case 365:
#line 1923 "parser.y"
    { yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); }
    break;

  case 366:
#line 1928 "parser.y"
    { CBZ::OldStyleFunctionDefinition = true; }
    break;

  case 367:
#line 1930 "parser.y"
    { CBZ::OldStyleFunctionDefinition = false; 
	       yyval.declL = yyvsp[0].declL; }
    break;

  case 368:
#line 1945 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 369:
#line 1946 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 370:
#line 1947 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 371:
#line 1948 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 372:
#line 1949 "parser.y"
    { yyval.exprN = yyvsp[0].constN; }
    break;

  case 373:
#line 1954 "parser.y"
    { yyval.constN = yyvsp[0].constN; }
    break;

  case 374:
#line 1956 "parser.y"
    {
	      string s3 = string(yyvsp[-1].constN->value().Str()) + string(yyvsp[0].constN->value().Str());
	      yyval.constN = new constNode(constant(s3.c_str()), s3.c_str(), yyvsp[-1].constN->coord());
	      delete yyvsp[-1].constN;
	      delete yyvsp[0].constN;
	    }
    break;

  case 375:
#line 1965 "parser.y"
    { yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 376:
#line 1966 "parser.y"
    { yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 377:
#line 1967 "parser.y"
    { yyval.tq.tq = typeNode::INLINE;   yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 378:
#line 1971 "parser.y"
    { yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 379:
#line 1972 "parser.y"
    { yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 380:
#line 1976 "parser.y"
    { yyval.tq.sc = declNode::TYPEDEF;  yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 381:
#line 1977 "parser.y"
    { yyval.tq.sc = declNode::EXTERN;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 382:
#line 1978 "parser.y"
    { yyval.tq.sc = declNode::STATIC;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 383:
#line 1979 "parser.y"
    { yyval.tq.sc = declNode::AUTO;     yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 384:
#line 1980 "parser.y"
    { yyval.tq.sc = declNode::REGISTER; yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; }
    break;

  case 385:
#line 1984 "parser.y"
    { yyval.primN = new primNode(basic_type::Void, Coord(yyvsp[0].tok));    }
    break;

  case 386:
#line 1985 "parser.y"
    { yyval.primN = new primNode(basic_type::Char, Coord(yyvsp[0].tok));    }
    break;

  case 387:
#line 1986 "parser.y"
    { yyval.primN = new primNode(basic_type::Int, Coord(yyvsp[0].tok));     }
    break;

  case 388:
#line 1987 "parser.y"
    { yyval.primN = new primNode(basic_type::Float, Coord(yyvsp[0].tok));   }
    break;

  case 389:
#line 1988 "parser.y"
    { yyval.primN = new primNode(basic_type::Double, Coord(yyvsp[0].tok));  }
    break;

  case 390:
#line 1990 "parser.y"
    { yyval.primN = new primNode(basic_type::Signed, Coord(yyvsp[0].tok));  }
    break;

  case 391:
#line 1991 "parser.y"
    { yyval.primN = new primNode(basic_type::Unsigned, Coord(yyvsp[0].tok));}
    break;

  case 392:
#line 1993 "parser.y"
    { yyval.primN = new primNode(basic_type::Short, Coord(yyvsp[0].tok));   }
    break;

  case 393:
#line 1994 "parser.y"
    { yyval.primN = new primNode(basic_type::Long, Coord(yyvsp[0].tok));    }
    break;


    }

/* Line 1016 of /lusr/gnu/share/bison/yacc.c.  */
#line 4396 "parser.tab.cc"

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


#line 2006 "parser.y"

/* ----end of grammar----*/

// ------------------------------------------------------------
// Utility functions
// ------------------------------------------------------------

// This function "merges" two storage classes. Since at most one
// storage class is allowed on a declaration, one of them must be
// NONE.

declNode::Storage_class merge_sc(declNode::Storage_class sc1,
				 declNode::Storage_class sc2,
				 const Coord c)
{
  declNode::Storage_class out;

  if ((sc1 != declNode::NONE) &&
      (sc2 != declNode::NONE)) {
    // If both are specified...
    if (sc1 == sc2) // scl == scr == single storage class
      CBZ::Warning(4, c, string("redundant storage class"));
    else
      CBZ::SyntaxError(c,
		       "conflicting storage classes `" +
		       declNode::storage_class_name(sc1) +
                       "' and `" +
		       declNode::storage_class_name(sc2) +
		       "'");
    out = sc1;
  }
  else
    // -- Use the one that IS specified..
    if (sc2 == declNode::NONE)
      out = sc1;
    else
      out = sc2;

  return out;
}

// This function merges two struct TQ objects, each of which contains
// a list of type qualifiers, and a storage class.

struct _TQ merge_tq(struct _TQ ts1,
		    struct _TQ ts2)
{
  struct _TQ out;

  if ((ts1.tq & ts2.tq) != 0)
    CBZ::Warning(4, Coord(ts2.coord), "redundant type qualifier");

  out.tq = typeNode::Type_qualifiers(ts1.tq | ts2.tq);
  out.sc = merge_sc(ts1.sc, ts2.sc);
  out.coord = ts2.coord;

  return out;
}

