/* A Bison parser, made from parser.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse cbparse
#define yylex cblex
#define yyerror cberror
#define yylval cblval
#define yychar cbchar
#define yydebug cbdebug
#define yynerrs cbnerrs
# define	ctokAND	257
# define	ctokSTAR	258
# define	ctokPLUS	259
# define	ctokMINUS	260
# define	ctokTILDE	261
# define	ctokNOT	262
# define	ctokLESS	263
# define	ctokGREATER	264
# define	ctokOR	265
# define	ctokHAT	266
# define	ctokPERCENT	267
# define	ctokDIVIDE	268
# define	ctokLEFTPAREN	269
# define	ctokRIGHTPAREN	270
# define	ctokDOT	271
# define	ctokQUESTION	272
# define	ctokSEMICOLON	273
# define	ctokLEFTBRACE	274
# define	ctokRIGHTBRACE	275
# define	ctokCOMMA	276
# define	ctokLEFTBRACKET	277
# define	ctokRIGHTBRACKET	278
# define	ctokCOLON	279
# define	ctokAUTO	280
# define	ctokDOUBLE	281
# define	ctokINT	282
# define	ctokSTRUCT	283
# define	ctokBREAK	284
# define	ctokELSE	285
# define	ctokLONG	286
# define	ctokSWITCH	287
# define	ctokCASE	288
# define	ctokENUM	289
# define	ctokREGISTER	290
# define	ctokTYPEDEF	291
# define	ctokCHAR	292
# define	ctokEXTERN	293
# define	ctokRETURN	294
# define	ctokUNION	295
# define	ctokCONST	296
# define	ctokFLOAT	297
# define	ctokSHORT	298
# define	ctokUNSIGNED	299
# define	ctokCONTINUE	300
# define	ctokFOR	301
# define	ctokSIGNED	302
# define	ctokVOID	303
# define	ctokDEFAULT	304
# define	ctokGOTO	305
# define	ctokSIZEOF	306
# define	ctokVOLATILE	307
# define	ctokDO	308
# define	ctokIF	309
# define	ctokSTATIC	310
# define	ctokWHILE	311
# define	ctokUPLUS	312
# define	ctokUMINUS	313
# define	ctokINDIR	314
# define	ctokADDRESS	315
# define	ctokPOSTINC	316
# define	ctokPOSTDEC	317
# define	ctokPREINC	318
# define	ctokPREDEC	319
# define	ctokBOGUS	320
# define	IDENTIFIER	321
# define	STRINGliteral	322
# define	FLOATINGconstant	323
# define	INTEGERconstant	324
# define	OCTALconstant	325
# define	HEXconstant	326
# define	WIDECHARconstant	327
# define	CHARACTERconstant	328
# define	TYPEDEFname	329
# define	ctokARROW	330
# define	ctokICR	331
# define	ctokDECR	332
# define	ctokLS	333
# define	ctokRS	334
# define	ctokLE	335
# define	ctokGE	336
# define	ctokEQ	337
# define	ctokNE	338
# define	ctokANDAND	339
# define	ctokOROR	340
# define	ctokELLIPSIS	341
# define	ctokEQUALS	342
# define	ctokMULTassign	343
# define	ctokDIVassign	344
# define	ctokMODassign	345
# define	ctokPLUSassign	346
# define	ctokMINUSassign	347
# define	ctokLSassign	348
# define	ctokRSassign	349
# define	ctokANDassign	350
# define	ctokERassign	351
# define	ctokORassign	352
# define	ctokINLINE	353
# define	ctokATTRIBUTE	354
# define	ctokMETA_TYPE_EXPR	355
# define	ctokMETA_TYPE_STMT	356
# define	ctokMETA_EXPR	357
# define	ctokMETA_STMT	358

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

#ifdef DEBUG
#  define YYERROR_VERBOSE
#endif /* DEBUG */

extern int yylex(void);

void yyerror(const char *msg)
{
  CBZ::SyntaxError(string(msg));
}

#define YYLTYPE_IS_TRIVIAL 1  /* TB */

/* End of create code (EAB) */


#line 125 "parser.y"
#ifndef YYSTYPE
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
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		644
#define	YYFLAG		-32768
#define	YYNTBASE	105

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 358 ? yytranslate[x] : 249)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     4,     6,     8,    12,    18,    25,    27,
      32,    36,    41,    45,    49,    52,    55,    59,    63,    65,
      69,    71,    74,    77,    80,    83,    88,    90,    92,    94,
      96,    98,   100,   102,   107,   109,   113,   117,   121,   123,
     127,   131,   133,   137,   141,   143,   147,   151,   155,   159,
     161,   165,   169,   171,   175,   177,   181,   183,   187,   189,
     193,   195,   199,   201,   207,   209,   213,   215,   217,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   241,
     243,   245,   246,   248,   251,   254,   257,   260,   261,   262,
     269,   270,   271,   278,   279,   280,   288,   289,   295,   296,
     302,   306,   307,   308,   315,   316,   317,   324,   325,   326,
     334,   335,   341,   342,   348,   352,   354,   356,   358,   361,
     364,   367,   370,   373,   376,   379,   382,   385,   388,   390,
     393,   396,   398,   400,   402,   404,   406,   408,   411,   414,
     417,   419,   422,   425,   427,   430,   433,   435,   438,   440,
     443,   445,   447,   449,   451,   453,   455,   457,   460,   465,
     471,   475,   479,   484,   489,   491,   495,   497,   500,   502,
     504,   507,   511,   515,   520,   522,   524,   526,   528,   531,
     534,   538,   542,   546,   550,   555,   557,   560,   564,   567,
     571,   576,   580,   582,   584,   586,   589,   593,   596,   600,
     605,   607,   611,   613,   616,   620,   625,   629,   634,   636,
     640,   642,   644,   646,   649,   651,   654,   655,   657,   659,
     662,   669,   671,   675,   676,   678,   683,   685,   687,   689,
     690,   693,   697,   702,   704,   706,   710,   712,   716,   718,
     720,   724,   728,   732,   734,   737,   740,   743,   745,   748,
     751,   753,   756,   759,   762,   764,   767,   770,   775,   781,
     784,   788,   793,   795,   797,   799,   802,   805,   808,   811,
     815,   818,   822,   826,   829,   833,   836,   837,   839,   842,
     848,   855,   858,   861,   866,   867,   870,   871,   873,   875,
     877,   879,   881,   883,   885,   888,   890,   894,   899,   903,
     907,   910,   914,   918,   923,   925,   927,   929,   931,   934,
     938,   942,   947,   949,   952,   954,   957,   960,   966,   974,
     980,   986,   994,  1004,  1014,  1024,  1034,  1040,  1044,  1047,
    1050,  1054,  1058,  1060,  1063,  1065,  1067,  1068,  1072,  1075,
    1076,  1081,  1082,  1087,  1088,  1093,  1094,  1099,  1100,  1104,
    1105,  1110,  1111,  1116,  1117,  1122,  1123,  1128,  1129,  1134,
    1135,  1141,  1142,  1148,  1149,  1155,  1156,  1162,  1163,  1166,
    1168,  1170,  1172,  1174,  1176,  1178,  1181,  1183,  1185,  1187,
    1189,  1191,  1193,  1195,  1197,  1199,  1201,  1203,  1205,  1207,
    1209,  1211,  1213,  1215,  1217,  1219,  1221
};
static const short yyrhs[] =
{
     221,     0,   247,     0,   241,     0,   242,     0,    15,   125,
      16,     0,    15,   210,   216,   211,    16,     0,    15,   210,
     215,   216,   211,    16,     0,   106,     0,   107,    23,   125,
      24,     0,   107,    15,    16,     0,   107,    15,   108,    16,
       0,   107,    17,   247,     0,   107,    76,   247,     0,   107,
      77,     0,   107,    78,     0,   107,    17,   248,     0,   107,
      76,   248,     0,   123,     0,   108,    22,   123,     0,   107,
       0,    77,   109,     0,    78,   109,     0,   110,   111,     0,
      52,   109,     0,    52,    15,   180,    16,     0,     3,     0,
       4,     0,     5,     0,     6,     0,     7,     0,     8,     0,
     109,     0,    15,   180,    16,   111,     0,   111,     0,   112,
       4,   111,     0,   112,    14,   111,     0,   112,    13,   111,
       0,   112,     0,   113,     5,   112,     0,   113,     6,   112,
       0,   113,     0,   114,    79,   113,     0,   114,    80,   113,
       0,   114,     0,   115,     9,   114,     0,   115,    10,   114,
       0,   115,    81,   114,     0,   115,    82,   114,     0,   115,
       0,   116,    83,   115,     0,   116,    84,   115,     0,   116,
       0,   117,     3,   116,     0,   117,     0,   118,    12,   117,
       0,   118,     0,   119,    11,   118,     0,   119,     0,   120,
      85,   119,     0,   120,     0,   121,    86,   120,     0,   121,
       0,   121,    18,   125,    25,   122,     0,   122,     0,   109,
     124,   123,     0,    88,     0,    89,     0,    90,     0,    91,
       0,    92,     0,    93,     0,    94,     0,    95,     0,    96,
       0,    97,     0,    98,     0,   123,     0,   125,    22,   123,
       0,   103,     0,   122,     0,     0,   125,     0,   129,    19,
       0,   138,    19,     0,   149,    19,     0,   155,    19,     0,
       0,     0,   147,   160,   130,   181,   131,   187,     0,     0,
       0,   153,   160,   132,   181,   133,   187,     0,     0,     0,
     129,    22,   160,   134,   181,   135,   187,     0,     0,   147,
       1,   136,   181,   187,     0,     0,   153,     1,   137,   181,
     187,     0,   129,    22,     1,     0,     0,     0,   151,   172,
     139,   181,   140,   187,     0,     0,     0,   157,   172,   141,
     181,   142,   187,     0,     0,     0,   138,    22,   172,   143,
     181,   144,   187,     0,     0,   151,     1,   145,   181,   187,
       0,     0,   157,     1,   146,   181,   187,     0,   138,    22,
       1,     0,   148,     0,   149,     0,   150,     0,   154,   245,
       0,   151,   246,     0,   148,   152,     0,   148,   246,     0,
     155,   245,     0,   151,   159,     0,   149,   152,     0,   156,
     245,     0,   151,   248,     0,   150,   152,     0,   245,     0,
     157,   245,     0,   151,   152,     0,   243,     0,   245,     0,
     154,     0,   155,     0,   156,     0,   246,     0,   157,   246,
       0,   154,   243,     0,   154,   246,     0,   159,     0,   157,
     159,     0,   155,   243,     0,   248,     0,   157,   248,     0,
     156,   243,     0,   243,     0,   157,   243,     0,   244,     0,
     158,   244,     0,   193,     0,   203,     0,   161,     0,   164,
       0,   172,     0,   176,     0,   162,     0,     4,   161,     0,
       4,    15,   163,    16,     0,     4,   158,    15,   163,    16,
       0,     4,   158,   161,     0,    15,   161,    16,     0,    15,
     163,   170,    16,     0,    15,   161,    16,   170,     0,   248,
       0,    15,   163,    16,     0,   248,     0,   248,   170,     0,
     165,     0,   166,     0,     4,   164,     0,     4,   158,   164,
       0,    15,   165,    16,     0,    15,   165,    16,   170,     0,
     168,     0,   169,     0,   170,     0,     4,     0,     4,   158,
       0,     4,   167,     0,     4,   158,   167,     0,    15,   168,
      16,     0,    15,   169,    16,     0,    15,   170,    16,     0,
      15,   168,    16,   170,     0,   171,     0,    15,    16,     0,
      15,   190,    16,     0,    23,    24,     0,    23,   126,    24,
       0,   171,    23,   126,    24,     0,   171,    23,    24,     0,
     173,     0,   175,     0,   174,     0,     4,   172,     0,     4,
     158,   172,     0,   175,   170,     0,    15,   173,    16,     0,
      15,   173,    16,   170,     0,   247,     0,    15,   175,    16,
       0,   177,     0,     4,   176,     0,     4,   158,   176,     0,
     175,    15,   178,    16,     0,    15,   176,    16,     0,    15,
     176,    16,   170,     0,   247,     0,   178,    22,   247,     0,
     247,     0,   248,     0,   153,     0,   153,   167,     0,   157,
       0,   157,   167,     0,     0,   182,     0,   183,     0,   182,
     183,     0,   100,    15,    15,   184,    16,    16,     0,   185,
       0,   184,    22,   185,     0,     0,   186,     0,   186,    15,
     125,    16,     0,   247,     0,   248,     0,    42,     0,     0,
      88,   188,     0,   212,   189,   213,     0,   212,   189,    22,
     213,     0,   123,     0,   188,     0,   189,    22,   188,     0,
     191,     0,   191,    22,    87,     0,    87,     0,   192,     0,
     191,    22,   192,     0,   192,    88,   188,     0,   191,    22,
       1,     0,   147,     0,   147,   167,     0,   147,   172,     0,
     147,   164,     0,   151,     0,   151,   167,     0,   151,   172,
       0,   153,     0,   153,   167,     0,   153,   172,     0,   153,
     164,     0,   157,     0,   157,   167,     0,   157,   172,     0,
     194,   212,   195,   213,     0,   194,   179,   212,   195,   213,
       0,   194,   179,     0,   194,   212,   213,     0,   194,   179,
     212,   213,     0,    29,     0,    41,     0,   196,     0,   195,
     196,     0,   198,    19,     0,   197,    19,     0,   157,   200,
       0,   197,    22,   200,     0,   153,   199,     0,   198,    22,
     199,     0,   160,   201,   181,     0,   202,   181,     0,   172,
     201,   181,     0,   202,   181,     0,     0,   202,     0,    25,
     126,     0,    35,   212,   204,   206,   213,     0,    35,   179,
     212,   204,   206,   213,     0,    35,   179,     0,   179,   205,
       0,   204,    22,   179,   205,     0,     0,    88,   126,     0,
       0,    22,     0,   208,     0,   209,     0,   217,     0,   218,
       0,   219,     0,   220,     0,     1,    19,     0,   104,     0,
     247,    25,   207,     0,    34,   126,    25,   207,     0,    50,
      25,   207,     0,   248,    25,   207,     0,   210,   211,     0,
     210,   215,   211,     0,   210,   216,   211,     0,   210,   215,
     216,   211,     0,    20,     0,    21,     0,    20,     0,    21,
       0,    20,    21,     0,    20,   215,    21,     0,    20,   216,
      21,     0,    20,   215,   216,    21,     0,   128,     0,   215,
     128,     0,   207,     0,   216,   207,     0,   127,    19,     0,
      55,    15,   125,    16,   207,     0,    55,    15,   125,    16,
     207,    31,   207,     0,    33,    15,   125,    16,   207,     0,
      57,    15,   125,    16,   207,     0,    54,   207,    57,    15,
     125,    16,    19,     0,    47,    15,   127,    19,   127,    19,
     127,    16,   207,     0,    47,    15,     1,    19,   127,    19,
     127,    16,   207,     0,    47,    15,   127,    19,   127,    19,
       1,    16,   207,     0,    47,    15,   127,    19,     1,    19,
     127,    16,   207,     0,    47,    15,     1,    16,   207,     0,
      51,   247,    19,     0,    46,    19,     0,    30,    19,     0,
      40,   127,    19,     0,    51,   248,    19,     0,   222,     0,
     221,   222,     0,   128,     0,   223,     0,     0,   172,   224,
     214,     0,   172,    66,     0,     0,   147,   172,   225,   214,
       0,     0,   153,   172,   226,   214,     0,     0,   151,   172,
     227,   214,     0,     0,   157,   172,   228,   214,     0,     0,
     176,   229,   214,     0,     0,   147,   176,   230,   214,     0,
       0,   153,   176,   231,   214,     0,     0,   151,   176,   232,
     214,     0,     0,   157,   176,   233,   214,     0,     0,   176,
     239,   234,   214,     0,     0,   147,   176,   239,   235,   214,
       0,     0,   153,   176,   239,   236,   214,     0,     0,   151,
     176,   239,   237,   214,     0,     0,   157,   176,   239,   238,
     214,     0,     0,   240,   215,     0,    69,     0,    70,     0,
      71,     0,    72,     0,    74,     0,    68,     0,   242,    68,
       0,    42,     0,    53,     0,    99,     0,    42,     0,    53,
       0,    37,     0,    39,     0,    56,     0,    26,     0,    36,
       0,    49,     0,    38,     0,    28,     0,    43,     0,    27,
       0,    48,     0,    45,     0,    44,     0,    32,     0,    67,
       0,    75,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   384,   394,   401,   402,   404,   410,   416,   423,   427,
     439,   442,   445,   448,   451,   454,   458,   461,   465,   469,
     473,   477,   480,   483,   487,   490,   494,   496,   497,   498,
     499,   500,   503,   505,   509,   511,   514,   517,   521,   523,
     526,   530,   532,   535,   539,   541,   544,   547,   550,   554,
     556,   559,   563,   565,   569,   571,   577,   579,   585,   587,
     591,   593,   599,   601,   605,   607,   613,   617,   620,   623,
     626,   629,   632,   635,   638,   641,   644,   648,   651,   663,
     666,   670,   672,   707,   711,   715,   721,   728,   728,   728,
     738,   738,   738,   746,   746,   746,   755,   755,   762,   762,
     769,   774,   774,   774,   783,   783,   783,   791,   791,   791,
     800,   800,   807,   807,   814,   818,   824,   827,   833,   838,
     843,   847,   854,   859,   864,   871,   876,   881,   888,   890,
     893,   898,   900,   904,   910,   913,   917,   922,   927,   931,
     937,   942,   947,   954,   959,   964,   970,   972,   976,   978,
     983,   988,   994,   996,   997,   998,  1007,  1010,  1013,  1016,
    1019,  1025,  1029,  1032,  1037,  1042,  1047,  1052,  1055,  1063,
    1066,  1069,  1074,  1078,  1083,  1085,  1086,  1090,  1095,  1099,
    1103,  1109,  1114,  1118,  1122,  1128,  1133,  1137,  1143,  1148,
    1152,  1157,  1169,  1171,  1175,  1178,  1181,  1186,  1190,  1193,
    1198,  1202,  1207,  1214,  1217,  1222,  1228,  1231,  1245,  1255,
    1266,  1268,  1272,  1277,  1281,  1285,  1293,  1296,  1300,  1303,
    1309,  1316,  1319,  1323,  1326,  1328,  1333,  1335,  1336,  1340,
    1342,  1346,  1349,  1351,  1355,  1360,  1367,  1370,  1375,  1384,
    1389,  1394,  1401,  1406,  1410,  1414,  1417,  1420,  1423,  1427,
    1430,  1433,  1437,  1440,  1443,  1446,  1450,  1462,  1466,  1470,
    1474,  1481,  1490,  1492,  1496,  1498,  1505,  1507,  1512,  1517,
    1523,  1528,  1535,  1541,  1548,  1554,  1561,  1563,  1567,  1572,
    1576,  1579,  1584,  1589,  1595,  1597,  1600,  1602,  1611,  1615,
    1618,  1621,  1624,  1627,  1638,  1640,  1643,  1647,  1650,  1654,
    1659,  1663,  1666,  1669,  1673,  1674,  1676,  1678,  1685,  1689,
    1692,  1695,  1700,  1704,  1712,  1717,  1726,  1731,  1735,  1738,
    1742,  1746,  1749,  1754,  1757,  1760,  1763,  1767,  1771,  1774,
    1777,  1781,  1793,  1797,  1801,  1819,  1833,  1833,  1840,  1846,
    1846,  1851,  1851,  1858,  1858,  1862,  1862,  1867,  1867,  1873,
    1873,  1880,  1880,  1887,  1887,  1895,  1895,  1902,  1902,  1909,
    1909,  1916,  1916,  1923,  1923,  1930,  1930,  1938,  1938,  1955,
    1957,  1958,  1959,  1960,  1964,  1966,  1978,  1980,  1981,  1984,
    1986,  1989,  1991,  1992,  1993,  1994,  1997,  1999,  2000,  2001,
    2002,  2004,  2005,  2007,  2008,  2011,  2015
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "ctokAND", "ctokSTAR", "ctokPLUS", 
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
  "ctokMETA_STMT", "prog.start", "primary.expression", 
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
  "left.enter.pragma", "right.exit.pragma", 
  "compound.statement.no.new.scope", "declaration.list", "statement.list", 
  "expression.statement", "selection.statement", "iteration.statement", 
  "jump.statement", "translation.unit", "external.definition", 
  "function.definition", "@17", "@18", "@19", "@20", "@21", "@22", "@23", 
  "@24", "@25", "@26", "@27", "@28", "@29", "@30", "@31", 
  "old.function.declaration.list", "@32", "constant", 
  "string.literal.list", "type.qualifier", "pointer.type.qualifier", 
  "storage.class", "basic.type.name", "identifier", "typedefname", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   105,   106,   106,   106,   106,   106,   106,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   108,   108,
     109,   109,   109,   109,   109,   109,   110,   110,   110,   110,
     110,   110,   111,   111,   112,   112,   112,   112,   113,   113,
     113,   114,   114,   114,   115,   115,   115,   115,   115,   116,
     116,   116,   117,   117,   118,   118,   119,   119,   120,   120,
     121,   121,   122,   122,   123,   123,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   125,   125,   125,
     126,   127,   127,   128,   128,   128,   128,   130,   131,   129,
     132,   133,   129,   134,   135,   129,   136,   129,   137,   129,
     129,   139,   140,   138,   141,   142,   138,   143,   144,   138,
     145,   138,   146,   138,   138,   147,   147,   147,   148,   148,
     148,   148,   149,   149,   149,   150,   150,   150,   151,   151,
     151,   152,   152,   153,   153,   153,   154,   154,   154,   154,
     155,   155,   155,   156,   156,   156,   157,   157,   158,   158,
     159,   159,   160,   160,   160,   160,   161,   161,   161,   161,
     161,   162,   162,   162,   163,   163,   164,   164,   164,   165,
     165,   165,   166,   166,   167,   167,   167,   168,   168,   168,
     168,   169,   169,   169,   169,   170,   170,   170,   171,   171,
     171,   171,   172,   172,   173,   173,   173,   174,   174,   174,
     175,   175,   176,   176,   176,   177,   177,   177,   178,   178,
     179,   179,   180,   180,   180,   180,   181,   181,   182,   182,
     183,   184,   184,   185,   185,   185,   186,   186,   186,   187,
     187,   188,   188,   188,   189,   189,   190,   190,   190,   191,
     191,   191,   191,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   193,   193,   193,
     193,   193,   194,   194,   195,   195,   196,   196,   197,   197,
     198,   198,   199,   199,   200,   200,   201,   201,   202,   203,
     203,   203,   204,   204,   205,   205,   206,   206,   207,   207,
     207,   207,   207,   207,   207,   207,   208,   208,   208,   208,
     209,   209,   209,   209,   210,   211,   212,   213,   214,   214,
     214,   214,   215,   215,   216,   216,   217,   218,   218,   218,
     219,   219,   219,   219,   219,   219,   219,   220,   220,   220,
     220,   220,   221,   221,   222,   222,   224,   223,   223,   225,
     223,   226,   223,   227,   223,   228,   223,   229,   223,   230,
     223,   231,   223,   232,   223,   233,   223,   234,   223,   235,
     223,   236,   223,   237,   223,   238,   223,   240,   239,   241,
     241,   241,   241,   241,   242,   242,   243,   243,   243,   244,
     244,   245,   245,   245,   245,   245,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   247,   248
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     1,     1,     1,     3,     5,     6,     1,     4,
       3,     4,     3,     3,     2,     2,     3,     3,     1,     3,
       1,     2,     2,     2,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     5,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     0,     1,     2,     2,     2,     2,     0,     0,     6,
       0,     0,     6,     0,     0,     7,     0,     5,     0,     5,
       3,     0,     0,     6,     0,     0,     6,     0,     0,     7,
       0,     5,     0,     5,     3,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     5,
       3,     3,     4,     4,     1,     3,     1,     2,     1,     1,
       2,     3,     3,     4,     1,     1,     1,     1,     2,     2,
       3,     3,     3,     3,     4,     1,     2,     3,     2,     3,
       4,     3,     1,     1,     1,     2,     3,     2,     3,     4,
       1,     3,     1,     2,     3,     4,     3,     4,     1,     3,
       1,     1,     1,     2,     1,     2,     0,     1,     1,     2,
       6,     1,     3,     0,     1,     4,     1,     1,     1,     0,
       2,     3,     4,     1,     1,     3,     1,     3,     1,     1,
       3,     3,     3,     1,     2,     2,     2,     1,     2,     2,
       1,     2,     2,     2,     1,     2,     2,     4,     5,     2,
       3,     4,     1,     1,     1,     2,     2,     2,     2,     3,
       2,     3,     3,     2,     3,     2,     0,     1,     2,     5,
       6,     2,     2,     4,     0,     2,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     3,     4,     3,     3,
       2,     3,     3,     4,     1,     1,     1,     1,     2,     3,
       3,     4,     1,     2,     1,     2,     2,     5,     7,     5,
       5,     7,     9,     9,     9,     9,     5,     3,     2,     2,
       3,     3,     1,     2,     1,     1,     0,     3,     2,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     3,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       5,     0,     5,     0,     5,     0,     5,     0,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,     0,   384,   390,   388,   262,   394,     0,   385,
     381,   387,   382,   263,   376,   389,   393,   392,   391,   386,
     377,   383,   395,   396,   378,   334,     0,     0,     0,   115,
     116,   117,     0,     0,   133,   134,   135,     0,   140,   336,
     192,   194,   193,   367,   202,   150,     0,   151,     1,   332,
     335,   146,   128,   136,   200,   143,   379,   380,     0,   195,
     203,   148,     0,     0,     0,   306,   281,     0,   210,   211,
      83,     0,    84,     0,    96,     0,     0,    87,   152,   156,
     153,   168,   169,   154,   367,   166,   120,   131,   132,   121,
      85,   124,   127,   110,   130,   123,   101,   367,   119,   126,
      98,    90,   154,   367,   138,   118,   139,    86,   142,   122,
     145,   125,   112,   141,   104,   367,   147,   129,   137,   144,
     338,     0,     0,     0,   197,   185,     0,   357,     0,   259,
       0,   333,   196,   204,   149,   198,   201,   206,     0,   284,
     286,   100,    93,   154,   155,   114,     0,     0,   107,   193,
     216,     0,     0,   157,   170,     0,     0,     0,     0,   164,
     216,     0,     0,   359,     0,   167,   216,   216,     0,     0,
     363,   216,   216,     0,     0,   361,   216,   216,     0,     0,
     365,     0,   337,   186,   238,   243,   116,   247,   250,   134,
     254,     0,     0,   236,   239,   208,    26,    27,    28,    29,
      30,    31,     0,   188,     0,   374,   369,   370,   371,   372,
     373,     0,     0,     8,    20,    32,     0,    34,    38,    41,
      44,    49,    52,    54,    56,    58,    60,    62,    80,     0,
       3,     4,     2,     0,   348,     0,   312,     0,     0,     0,
       0,   368,     0,   307,     0,   133,   134,   135,     0,     0,
     264,     0,     0,   260,   199,   207,   286,     0,   282,   287,
       0,   216,     0,     0,   216,     0,   229,   217,   218,     0,
       0,   160,   171,     0,   161,     0,   172,    88,   340,   350,
       0,   229,   102,   344,   354,     0,   229,    91,   342,   352,
       0,   229,   105,   346,   356,     0,     0,   304,   308,     0,
       0,     0,    81,     0,     0,     0,     0,     0,     0,     0,
      79,   295,    32,    64,    77,    82,     0,   314,   288,   289,
       0,     0,     0,   290,   291,   292,   293,     2,   143,   177,
       0,   246,   244,   174,   175,   176,   245,   177,     0,   248,
     249,   253,   251,   252,   255,   256,   205,     0,   187,     0,
       0,     0,   212,   214,     0,     0,     0,    24,     0,    21,
      22,     0,     0,     0,     0,    14,    15,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   375,   191,
       0,   358,   101,   104,   313,     0,   261,     0,   276,   270,
     216,   276,   268,   216,   265,   257,   267,     0,   266,     0,
       0,   285,   284,   279,    94,   108,     0,     0,    97,   219,
     158,     0,   165,   163,   162,   173,   229,   360,   111,   229,
     364,    99,   229,   362,   113,   229,   366,   294,   329,     0,
       0,     0,   328,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,     0,     0,   316,   305,   300,     0,     0,   309,
       0,   310,   315,     0,     0,   178,   179,     0,     0,     0,
     178,   209,   242,   237,   240,   233,   241,     0,     5,   177,
       0,   213,   215,     0,     0,     0,     0,    10,     0,    18,
      12,    16,     0,    13,    17,    35,    37,    36,    39,    40,
      42,    43,    45,    46,    47,    48,    50,    51,    53,    55,
      57,    59,     0,    61,   190,   258,   278,   216,   277,   273,
     216,   275,   269,   271,   280,   283,   229,   229,   223,   230,
     159,    89,   103,    92,   106,     0,     0,   330,     0,     0,
     298,   327,   331,     0,     0,     0,    65,    78,   301,     0,
     302,   311,   296,   299,   180,   181,   182,   183,   234,     0,
     178,    33,     0,     0,    25,    11,     0,     9,     0,   272,
     274,    95,   109,   228,     0,   221,   224,   226,   227,     0,
     297,     0,    81,     0,     0,     0,     0,   303,   184,     0,
     231,     0,     6,    19,    63,     0,   223,     0,   319,   326,
       0,     0,     0,     0,   317,   320,   235,   232,     7,   220,
     222,     0,    81,    81,     0,     0,     0,   225,     0,     0,
       0,     0,   321,   318,     0,     0,     0,     0,   323,   325,
     324,   322,     0,     0,     0
};

static const short yydefgoto[] =
{
     642,   213,   214,   498,   312,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   313,   314,   462,
     315,   229,   316,   236,    26,   160,   426,   172,   432,   261,
     536,   150,   171,    27,   167,   429,   177,   435,   264,   537,
     166,   176,   237,    29,    30,    31,   238,    94,   239,    34,
      35,    36,   240,    58,    38,    77,    78,    79,   157,    80,
      81,    82,   476,   333,   334,   335,   125,    59,    40,    41,
      42,    64,    44,   191,   139,   354,   266,   267,   268,   584,
     585,   586,   418,   486,   569,   192,   193,   194,    45,    46,
     249,   250,   251,   252,   399,   402,   527,   400,    47,   140,
     258,   260,   317,   318,   319,   320,   466,   487,   253,   182,
     241,   322,   323,   324,   325,   326,    48,    49,    50,   121,
     161,   173,   168,   178,   126,   162,   174,   169,   179,   235,
     280,   290,   285,   295,   127,   128,   230,   231,    51,    61,
      52,    53,   232,   448
};

static const short yypact[] =
{
    2510,   375,   159,-32768,-32768,-32768,-32768,-32768,    40,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    28,   107,    30,  3038,
     481,   712,  2033,   362,  3038,   635,   712,  2086,-32768,   -40,
  -32768,-32768,    42,    23,-32768,-32768,    40,-32768,  2510,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   375,-32768,
  -32768,-32768,    22,   234,    43,-32768,    59,   -46,-32768,-32768,
  -32768,   461,-32768,   327,-32768,   746,   126,-32768,-32768,-32768,
  -32768,-32768,-32768,    69,   210,    76,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    73,    83,-32768,-32768,
  -32768,-32768,   124,   361,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   174,   182,-32768,-32768,-32768,-32768,
  -32768,   186,  2623,  2786,-32768,   136,   186,-32768,  2969,    59,
    2712,-32768,-32768,-32768,-32768,    76,-32768,    76,   -46,    24,
     236,-32768,-32768,-32768,-32768,-32768,   836,   220,-32768,    76,
     111,   126,   813,-32768,-32768,   126,   163,    76,   212,-32768,
     111,   186,   186,-32768,  2676,-32768,   111,   111,   186,   186,
  -32768,   111,   111,   186,   186,-32768,   111,   111,   186,   186,
  -32768,  1088,-32768,-32768,-32768,   396,   712,  2457,   396,   712,
    2457,   142,   240,   264,   213,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1852,-32768,  2896,-32768,-32768,-32768,-32768,-32768,
  -32768,  2909,  2909,-32768,   534,-32768,  2985,-32768,    96,    64,
     139,    68,   401,   291,   297,   292,   228,    -2,-32768,   299,
  -32768,   257,-32768,  2799,-32768,   186,-32768,    30,  2139,   362,
    2192,  2969,  2712,-32768,   454,  1939,   200,   200,  2546,  2712,
  -32768,   117,   296,-32768,-32768,-32768,   236,  2985,-32768,   -46,
     314,   111,   836,   437,   111,   328,   273,   111,-32768,   490,
     126,-32768,-32768,   537,    76,   380,    76,-32768,-32768,-32768,
     186,   273,-32768,-32768,-32768,   186,   273,-32768,-32768,-32768,
     186,   273,-32768,-32768,-32768,   186,   359,-32768,-32768,   411,
     400,  2985,  1955,   414,   423,   439,   -46,  1646,   433,   462,
  -32768,-32768,   907,-32768,-32768,   464,   470,-32768,-32768,-32768,
    1169,  1250,  1412,-32768,-32768,-32768,-32768,   472,   476,   777,
    2298,-32768,-32768,-32768,-32768,-32768,-32768,   634,  2351,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   436,-32768,  2245,
    2875,   239,   293,  2588,   506,  1331,  1852,-32768,  1928,-32768,
  -32768,  1005,   -46,  1955,   -46,-32768,-32768,-32768,  2985,  2985,
    2985,  2985,  2985,  2985,  2985,  2985,  2985,  2985,  2985,  2985,
    2985,  2985,  2985,  2985,  2985,  1955,  2985,-32768,-32768,-32768,
     502,-32768,-32768,-32768,-32768,  2712,-32768,  2985,   508,-32768,
     111,   508,-32768,   111,-32768,-32768,-32768,   287,-32768,   454,
     314,-32768,    24,-32768,-32768,-32768,   520,  2875,-32768,-32768,
  -32768,   563,-32768,-32768,-32768,-32768,   273,-32768,-32768,   273,
  -32768,-32768,   273,-32768,-32768,   273,-32768,-32768,-32768,  1955,
     517,   525,-32768,  1724,  1646,   542,   547,   524,   476,  1955,
    1955,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2985,  2985,-32768,-32768,-32768,  1169,  1490,-32768,
    1568,-32768,-32768,  1646,  1646,   777,-32768,   572,   574,   578,
     634,-32768,-32768,-32768,-32768,-32768,-32768,  2875,-32768,   644,
    2404,-32768,-32768,  2985,  1331,  1490,   587,-32768,   329,-32768,
  -32768,-32768,   419,-32768,-32768,-32768,-32768,-32768,    96,    96,
      64,    64,   139,   139,   139,   139,    68,    68,   401,   291,
     297,   292,    92,   228,-32768,-32768,-32768,   111,-32768,-32768,
     111,-32768,-32768,-32768,-32768,-32768,   273,   273,   209,-32768,
  -32768,-32768,-32768,-32768,-32768,   354,  1646,-32768,   406,   586,
  -32768,-32768,-32768,   592,   369,   383,-32768,-32768,-32768,  1490,
  -32768,-32768,-32768,-32768,-32768,    76,-32768,-32768,-32768,   251,
     644,-32768,  1490,   598,-32768,-32768,  2985,-32768,  2985,-32768,
  -32768,-32768,-32768,-32768,   394,-32768,   600,-32768,-32768,  1646,
  -32768,  1646,  1955,  1751,  1955,  1646,  1646,-32768,-32768,  2763,
  -32768,   605,-32768,-32768,-32768,   608,   209,  1955,-32768,-32768,
     597,   606,   612,   465,   596,-32768,-32768,-32768,-32768,-32768,
  -32768,   561,  1955,  1955,  1774,   621,  1646,-32768,   625,   626,
     628,   629,-32768,-32768,  1646,  1646,  1646,  1646,-32768,-32768,
  -32768,-32768,   647,   655,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,   541,-32768,  -197,   183,   157,   275,
     185,   281,   278,   280,   286,   289,-32768,  -112,   206,-32768,
    -160,  -170,  -221,     5,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    20,-32768,  -115,-32768,    35,    89,     3,   -42,
     290,    62,    18,   -66,    -7,   -27,    10,-32768,   -61,   -51,
     -49,-32768,  -175,  -276,  -274,    19,-32768,   585,   189,-32768,
     556,    95,-32768,-32768,     9,   309,   279,-32768,   416,-32768,
      67,-32768,   301,  -395,-32768,-32768,-32768,   335,-32768,-32768,
     450,  -226,-32768,-32768,   269,   298,   295,  -124,-32768,   562,
     282,   443,   388,-32768,-32768,  -161,   -81,     6,    -6,   771,
    -145,  -281,-32768,-32768,-32768,-32768,-32768,   656,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   399,-32768,-32768,-32768,   539,   -54,
     840,    79,     0,   360
};


#define	YYLAST		3137


static const short yytable[] =
{
      54,    54,    54,    33,   134,    25,   101,   186,    68,   152,
     332,   228,   339,   342,    67,   344,   385,    66,    37,   367,
      28,    22,   539,   404,   154,    95,   120,   158,    54,    23,
     113,    74,    54,    54,    75,    32,   321,    54,   135,   468,
     470,   355,   351,  -347,   142,    76,    68,    70,    54,   186,
      71,    33,   130,    25,   477,   129,   478,   122,    54,   137,
      65,   124,   477,   390,   478,   123,    37,    68,    28,   371,
     372,    54,   138,    54,   495,    54,    54,   375,   376,    65,
     262,   441,   124,    32,   386,   153,   156,   411,   245,  -339,
     269,   164,   568,  -343,   273,    43,    60,    22,   134,   123,
     368,   272,   158,  -353,   165,    23,   158,    22,    89,   369,
     370,    98,   257,   106,   463,    23,   118,   578,    86,    91,
      92,   228,   195,    84,   403,   188,    72,    97,   103,    73,
      75,   440,   115,   244,   331,   242,   406,   341,    68,   407,
     190,   155,   185,    43,  -341,   228,    54,    54,   248,   377,
     378,    54,    54,   133,   254,    54,   255,   187,   346,   233,
     245,   156,   271,     1,   347,   156,   144,   188,   124,   404,
      60,   505,   506,   507,     2,   467,   275,   491,   492,   274,
      95,   327,   190,   113,   185,    54,   559,    54,    54,   228,
      54,    62,   247,    22,  -345,   355,   351,   355,   351,   187,
     245,    23,  -355,   502,   616,   352,   181,   245,   134,   421,
     494,   265,   101,   572,   477,   186,   478,   398,   373,   374,
     353,   158,   549,   186,   146,   522,    22,   526,   276,  -155,
    -349,    95,  -155,   113,   186,   147,   396,    54,    54,    54,
      54,   113,    14,   405,    54,   244,   394,   133,    54,   122,
     136,   583,   244,    20,   413,   488,   348,   123,   259,    68,
     248,   463,    54,   475,   247,    62,    98,   248,   412,   118,
      54,   480,   243,   599,   528,    91,    22,   528,   154,   545,
     156,   158,   124,   403,    23,   228,   349,    22,   275,   554,
     555,   146,   275,   423,   381,   425,   571,   489,  -155,    24,
     564,   350,   147,   383,   247,   564,   445,   327,   490,   382,
    -155,   247,   397,   384,   245,   408,   123,    98,   409,   118,
     327,   327,   327,   387,   106,   388,   394,   118,   145,    54,
      54,   146,   144,   188,   144,   243,    62,    54,    54,   144,
      62,   188,   147,   416,    62,   575,   113,   481,   190,   479,
     185,   576,   188,   245,    22,   327,   190,   479,   185,   352,
      55,   417,   500,   100,   503,   187,    75,   190,    69,   185,
     589,   610,   612,   187,   353,   186,   463,    76,   437,     1,
    -155,  -351,   398,  -155,   187,   595,   558,   560,    85,   525,
       2,   463,    99,    85,    22,   564,   424,   119,   244,   596,
     329,   628,   629,   631,   534,   463,    69,    54,    55,    54,
     605,   330,   189,   248,   573,   439,   606,    56,   247,   123,
     246,   134,   591,   570,   272,   592,   134,    69,    57,    22,
     438,    85,   118,   442,   613,    85,   159,    23,   443,   277,
     275,   463,    22,   577,   327,   281,   282,   621,   449,  -155,
     286,   287,   164,   136,   189,   291,   292,   247,    75,    62,
     123,  -155,   141,    22,   444,    75,   604,   327,   327,    76,
     327,    23,   394,   327,   327,    54,    76,   450,   597,   397,
      54,   625,    55,   163,   379,   380,   463,   463,    55,   464,
      55,   601,   246,   188,   327,   327,   170,   473,    69,   394,
      90,   474,   175,    22,   144,   164,   420,     3,   190,   479,
     185,   159,    85,   123,   180,   159,   134,     9,    10,    62,
      12,    22,   493,    14,    55,   187,   524,    62,    22,    23,
     510,   511,   246,   397,    20,   538,    23,    21,   587,   246,
     414,   328,   546,   415,   547,    85,   327,    99,    85,   361,
     119,   362,   164,   422,   508,   509,   485,   363,    63,   327,
     123,   551,    55,   600,   516,   517,   552,   499,    87,    87,
      87,    87,   327,   104,   108,   110,   116,   627,   164,   540,
      24,   553,   428,   463,   598,    39,   123,   431,   565,   327,
     566,   327,   434,   617,   567,   327,   327,    85,    99,    85,
     119,    55,    55,   574,    85,   593,   587,   594,   119,    55,
     364,   365,   366,    83,   602,   607,   622,    96,   102,    69,
     189,   618,   114,   485,   619,   623,   327,   626,   189,   149,
     159,   624,    63,    39,   327,   327,   327,   327,   337,   189,
     632,   634,   635,   132,   636,   637,   246,   643,   489,   338,
     512,   513,   514,   515,   107,   644,   143,   123,   148,   490,
     519,     3,   518,   520,   215,   496,   446,   123,   556,   557,
     521,     9,    10,   620,    12,   523,    56,    14,   533,   529,
     328,   328,   531,   419,   484,   246,    56,    57,    20,    85,
      55,    21,   395,   485,   535,   447,   530,    57,    55,   410,
     256,    22,   149,   263,   131,   532,     0,    63,     0,    55,
     472,    63,     0,   119,     0,   328,    55,     0,     0,     0,
       0,     0,   501,     0,   504,    87,    87,   541,   108,   116,
     542,     0,     0,   543,    24,     0,   544,   132,     3,     0,
       0,   149,     0,   149,   149,   357,   149,     0,     9,    10,
      75,    12,   359,   360,    14,    55,     0,   215,     0,     0,
       0,   151,     0,     0,     0,    20,     0,     0,    21,    85,
     336,     0,   340,   343,   215,   345,     0,    87,     0,   116,
     189,   329,   603,     0,   104,   108,   110,   116,    56,     0,
       0,     0,   330,     0,   149,     0,   149,     0,   215,    57,
     123,     0,     0,     0,   149,   485,   579,     0,     0,   580,
       0,    24,     0,    22,     0,     0,     0,    75,   149,    56,
       0,    23,   143,   392,   143,   393,    63,   328,   270,   143,
      57,     0,   550,   401,     0,    85,     0,   581,   582,     0,
     146,     0,   215,     0,    22,     0,     0,   132,     0,     0,
      55,   147,    23,     0,   328,    56,   472,     0,   472,     0,
       0,   562,   563,     0,     0,     0,    57,     0,     0,    88,
      88,    88,    88,     0,   105,   109,   111,   117,    56,     0,
      22,     0,     0,   472,     0,   149,   263,     0,    23,    57,
       0,     0,   116,   149,   263,     0,     0,   234,   588,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,     0,   215,     0,     0,
       0,     0,   278,   279,   590,     0,     0,     0,   215,   283,
     284,     0,     0,     0,   288,   289,     0,   472,     0,   293,
     294,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,   149,     0,     0,   588,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   608,     0,   609,
       0,     0,     0,   614,   615,     0,     0,     0,     0,     0,
       0,     0,   401,     0,   143,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   391,     0,   196,   197,
     198,   199,   200,   201,   633,     0,     0,     0,     0,     0,
     202,   497,   638,   639,   640,   641,    88,    88,     0,   109,
     117,   149,     0,     0,   215,     0,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,   430,   204,     0,     0,
     132,   433,     0,     0,     0,   132,   436,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,    88,   210,
     117,     0,   211,   212,     0,     0,     0,     0,     0,   296,
       0,   196,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,     0,   202,     0,     0,     0,   -81,   297,   298,
       0,     0,     0,     0,     3,     4,     5,     6,   299,   215,
       7,   300,   301,     8,     9,    10,    11,    12,   302,    13,
      14,    15,    16,    17,   303,   304,    18,    19,   305,   306,
     204,    20,   307,   308,    21,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,   205,   206,   207,   208,
     209,     0,   210,    23,     0,   211,   212,     0,     0,     0,
     296,     0,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,   202,     0,     0,    24,   -81,   297,
     465,   310,   311,     0,     0,     3,     4,     5,     6,   299,
       0,     7,   300,   301,     8,     9,    10,    11,    12,   302,
      13,    14,    15,    16,    17,   303,   304,    18,    19,   305,
     306,   204,    20,   307,   308,    21,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,   205,   206,   207,
     208,   209,     0,   210,    23,     0,   211,   212,     0,     0,
       0,   296,     0,   196,   197,   198,   199,   200,   201,     0,
       0,     0,     0,     0,     0,   202,     0,     0,    24,   -81,
     297,   469,   310,   311,     0,     0,     3,     4,     5,     6,
     299,     0,     7,   300,   301,     8,     9,    10,    11,    12,
     302,    13,    14,    15,    16,    17,   303,   304,    18,    19,
     305,   306,   204,    20,   307,   308,    21,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,   205,   206,
     207,   208,   209,     0,   210,    23,     0,   211,   212,     0,
       0,     0,   296,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,     0,   202,     0,     0,    24,
     -81,   297,     0,   310,   311,     0,     0,     3,     4,     5,
       6,   299,     0,     7,   300,   301,     8,     9,    10,    11,
      12,   302,    13,    14,    15,    16,    17,   303,   304,    18,
      19,   305,   306,   204,    20,   307,   308,    21,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,   205,
     206,   207,   208,   209,     0,   210,    23,     0,   211,   212,
       0,     0,     0,   296,     0,   196,   197,   198,   199,   200,
     201,     0,     0,     0,     0,     0,     0,   202,     0,     0,
      24,   -81,   297,   471,   310,   311,     0,     0,     0,     0,
       0,     0,   299,     0,     0,   300,   301,     0,     0,     0,
       0,     0,   302,     0,     0,     0,     0,     0,   303,   304,
       0,     0,   305,   306,   204,     0,   307,   308,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
     205,   206,   207,   208,   209,     0,   210,    23,     0,   211,
     212,   296,     0,   196,   197,   198,   199,   200,   201,     0,
       0,     0,     0,     0,     0,   202,     0,     0,     0,   -81,
     297,   465,     0,     0,     0,   310,   311,     0,     0,     0,
     299,     0,     0,   300,   301,     0,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,   303,   304,     0,     0,
     305,   306,   204,     0,   307,   308,     0,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,   205,   206,
     207,   208,   209,     0,   210,    23,     0,   211,   212,   296,
       0,   196,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,     0,   202,     0,     0,     0,   -81,   297,   561,
       0,     0,     0,   310,   311,     0,     0,     0,   299,     0,
       0,   300,   301,     0,     0,     0,     0,     0,   302,     0,
       0,     0,     0,     0,   303,   304,     0,     0,   305,   306,
     204,     0,   307,   308,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,   205,   206,   207,   208,
     209,     0,   210,    23,     0,   211,   212,   296,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
       0,   202,     0,     0,     0,   -81,   297,     0,     0,     0,
       0,   310,   311,     0,     0,     0,   299,     0,     0,   300,
     301,     0,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,   303,   304,     0,     0,   305,   306,   204,     0,
     307,   308,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   205,   206,   207,   208,   209,     0,
     210,    23,     0,   211,   212,   548,     0,   196,   197,   198,
     199,   200,   201,     0,     0,     0,     0,     0,     0,   202,
       0,     0,     0,   -81,     0,     0,     0,     0,     0,   310,
     311,     0,   611,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,     0,   202,     0,     0,     0,
     -81,     0,     0,     0,     0,   630,   204,   196,   197,   198,
     199,   200,   201,     0,     0,     0,     0,     0,     0,   202,
     -81,    22,   205,   206,   207,   208,   209,     0,   210,     0,
       0,   211,   212,   204,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,   205,
     206,   207,   208,   209,     0,   210,   204,   310,   211,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,   205,   206,   207,   208,   209,     0,   210,     0,
       0,   211,   212,     0,   310,   196,   197,   198,   199,   200,
     201,     0,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,   297,     0,     0,     0,     0,   310,     0,     4,
       5,     6,     0,     0,     7,     0,     0,     8,     0,     0,
      11,     0,     0,    13,    14,    15,    16,    17,     0,     0,
      18,    19,     0,     0,   204,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
     205,   206,   207,   208,   209,     0,   210,    23,     0,   211,
     212,   196,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,     0,   202,     0,     0,     0,     0,   297,     0,
       0,    24,     0,     0,     0,   310,     0,     0,   196,   197,
     198,   199,   200,   201,     0,     0,     4,     5,     0,     0,
     202,     7,     0,     0,     0,     0,     0,    11,     0,     0,
     204,    14,    15,    16,    17,     0,     0,    18,    19,     0,
       0,     0,    20,     0,     0,    22,   205,   206,   207,   208,
     209,     0,   210,     0,     0,   211,   212,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,     0,   210,
       0,   310,   211,   212,    93,     0,     0,     1,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,     3,
       4,     5,     6,     0,     0,     7,     0,     0,     8,     9,
      10,    11,    12,     0,    13,    14,    15,    16,    17,     0,
       0,    18,    19,     0,     0,     0,    20,   112,     0,    21,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     2,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     3,     4,     5,     6,     0,     0,     7,     0,
       0,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    24,     0,    18,    19,     0,     0,     0,    20,
      93,     0,    21,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,   147,     0,     0,     0,     0,     0,
       0,    23,     0,     0,     0,     3,     4,     5,     6,     0,
       0,     7,     0,     0,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,    24,     0,    18,    19,     0,
       0,     0,    20,   112,     0,    21,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,   147,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     3,     4,
       5,     6,     0,     0,     7,     0,     0,     8,     9,    10,
      11,    12,     0,    13,    14,    15,    16,    17,    24,     0,
      18,    19,     0,     0,     0,    20,   482,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     3,     4,     5,     6,     0,     0,     7,     0,     0,
       8,     9,    10,    11,    12,     0,    13,    14,    15,    16,
      17,    24,     0,    18,    19,     0,     0,     0,    20,     0,
       0,    21,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,   183,     0,     0,     0,     0,     0,
      23,   123,     0,     0,     3,     4,     5,     6,     0,     0,
       7,     0,   483,     8,     9,    10,    11,    12,     0,    13,
      14,    15,    16,    17,    24,     0,    18,    19,     0,     0,
       0,    20,     0,     0,    21,   337,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,   338,   183,     0,     0,
       0,     0,     0,    23,   123,     0,     0,     3,     4,     5,
       6,     0,     0,     7,     0,   184,     8,     9,    10,    11,
      12,     0,    13,    14,    15,    16,    17,    24,     0,    18,
      19,     0,     0,     0,    20,     0,     0,    21,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,   490,
     183,     0,     0,     0,     0,     0,    23,   123,     0,     0,
       3,     4,     5,     6,     0,     0,     7,     0,   184,     8,
       9,    10,    11,    12,     0,    13,    14,    15,    16,    17,
      24,     0,    18,    19,     0,     0,     0,    20,     0,     0,
      21,   337,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   338,     0,     0,     0,     0,     0,     0,    23,
     123,     0,     0,     3,     4,     5,     6,     0,     0,     7,
       0,   184,     8,     9,    10,    11,    12,     0,    13,    14,
      15,    16,    17,    24,     0,    18,    19,     0,     0,     0,
      20,     0,     0,    21,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     2,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     3,     4,     5,     6,
       0,     0,     7,     0,     0,     8,     9,    10,    11,    12,
     146,    13,    14,    15,    16,    17,    24,     0,    18,    19,
       0,   147,     0,    20,     0,     0,    21,     0,     0,     0,
       0,   397,     0,     4,     5,     6,     0,    22,     7,     0,
       0,     8,     0,     0,    11,    23,     0,    13,    14,    15,
      16,    17,   489,     0,    18,    19,     0,     0,     0,    20,
       0,     0,     0,   490,     0,     0,     0,     0,     0,    24,
       0,   123,     0,    22,     0,     4,     5,     6,     0,     0,
       7,    23,     0,     8,     0,     0,    11,     0,     0,    13,
      14,    15,    16,    17,     0,     0,    18,    19,     0,   183,
       0,    20,     0,     0,     0,    24,     0,     0,     0,     3,
       4,     5,     6,     0,     0,     7,     0,     0,     8,     9,
      10,    11,    12,    23,    13,    14,    15,    16,    17,     0,
       0,    18,    19,     0,     0,     0,    20,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    24,     0,     0,
      22,     0,   183,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     3,     4,     5,     6,     0,     0,     7,     0,
     184,     8,     9,    10,    11,    12,     0,    13,    14,    15,
      16,    17,    24,     0,    18,    19,     0,     0,     0,    20,
       0,     0,    21,   243,     0,     0,     0,     0,     0,     4,
       5,     6,     0,     0,     7,     0,     0,     8,     0,     0,
      11,    23,     0,    13,    14,    15,    16,    17,     0,     0,
      18,    19,     0,   184,     0,    20,   196,   197,   198,   199,
     200,   201,     0,     0,     0,    24,     0,     0,   202,     0,
       0,     0,     0,    65,   243,     0,     0,    23,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
       0,   202,   196,   197,   198,   199,   200,   201,     0,     0,
     203,    24,     0,     0,   202,   204,     0,     0,     0,     0,
       0,     0,     0,   389,     0,     0,     0,     0,     0,     0,
      22,   205,   206,   207,   208,   209,     0,   210,   204,     0,
     211,   212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   204,     0,    22,   205,   206,   207,   208,   209,     0,
     210,     0,     0,   211,   212,     0,    22,   205,   206,   207,
     208,   209,     0,   210,     0,     0,   211,   212,   196,   197,
     198,   199,   200,   201,     0,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,    65,     0,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
       0,   356,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,     0,   358,     0,     0,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,   204,   210,
       0,     0,   211,   212,     0,     0,     0,     0,     0,     0,
       0,   204,     0,    22,   205,   206,   207,   208,   209,     0,
     210,     0,     0,   211,   212,     0,    22,   205,   206,   207,
     208,   209,     0,   210,     0,     0,   211,   212,   196,   197,
     198,   199,   200,   201,     0,     3,     4,     5,     6,     0,
     202,     7,     0,     0,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    16,    17,     0,     0,    18,    19,     0,
       0,     0,    20,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   204,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
       0,     0,    22,   205,   206,   207,   208,   209,     0,   210,
       0,     0,   211,   212,     3,     4,     5,     0,    24,     0,
       7,     0,     0,     0,     9,    10,    11,    12,     0,     0,
      14,    15,    16,    17,     0,     0,    18,    19,     0,     0,
       0,    20,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24
};

static const short yycheck[] =
{
       0,     1,     2,     0,    58,     0,    33,   122,     8,    75,
     185,   123,   187,   188,     8,   190,    18,     8,     0,   216,
       0,    67,   417,   249,    75,    32,    66,    76,    28,    75,
      37,     1,    32,    33,     4,     0,   181,    37,    16,   320,
     321,   202,   202,    20,    71,    15,    46,    19,    48,   164,
      22,    48,    46,    48,   330,    46,   330,    15,    58,    16,
      20,    42,   338,   233,   338,    23,    48,    67,    48,     5,
       6,    71,    66,    73,   355,    75,    76,     9,    10,    20,
     146,   302,    63,    48,    86,    75,    76,   257,   130,    20,
     151,    15,   487,    20,   155,     0,     1,    67,   152,    23,
       4,   152,   151,    20,    85,    75,   155,    67,    29,    13,
      14,    32,    88,    34,    22,    75,    37,    25,    29,    30,
      31,   233,   122,    28,   248,   122,    19,    32,    33,    22,
       4,   301,    37,   130,   185,   129,    19,   188,   138,    22,
     122,    15,   122,    48,    20,   257,   146,   147,   130,    81,
      82,   151,   152,    58,   135,   155,   137,   122,    16,    23,
     202,   151,   152,     4,    22,   155,    71,   164,   149,   395,
      75,   368,   369,   370,    15,   320,   157,   352,   353,    16,
     187,   181,   164,   190,   164,   185,   467,   187,   188,   301,
     190,     2,   130,    67,    20,   356,   356,   358,   358,   164,
     242,    75,    20,   363,   599,   202,    20,   249,   262,   270,
     355,   100,   239,   494,   490,   330,   490,   244,    79,    80,
     202,   270,   443,   338,     4,   385,    67,   397,    16,    19,
      20,   238,    22,   240,   349,    15,   242,   237,   238,   239,
     240,   248,    42,   249,   244,   242,   241,   152,   248,    15,
      16,    42,   249,    53,   260,    16,    16,    23,    22,   259,
     242,    22,   262,   329,   202,    76,   187,   249,   259,   190,
     270,   337,    21,    22,   398,   186,    67,   401,   329,   439,
     270,   330,   263,   407,    75,   397,    22,    67,   269,   449,
     450,     4,   273,   274,     3,   276,   493,     4,    88,    99,
     475,    88,    15,    11,   242,   480,   306,   307,    15,    12,
     100,   249,    25,    85,   356,    19,    23,   238,    22,   240,
     320,   321,   322,    24,   245,    68,   321,   248,     1,   329,
     330,     4,   237,   330,   239,    21,   147,   337,   338,   244,
     151,   338,    15,    15,   155,    16,   353,   347,   330,   330,
     330,    22,   349,   395,    67,   355,   338,   338,   338,   356,
       0,    88,   362,     1,   364,   330,     4,   349,     8,   349,
      16,   592,   593,   338,   356,   490,    22,    15,    19,     4,
      19,    20,   409,    22,   349,    16,   467,   468,    28,   395,
      15,    22,    32,    33,    67,   570,    16,    37,   395,    16,
       4,   622,   623,   624,   410,    22,    46,   407,    48,   409,
      16,    15,   122,   395,   495,    15,    22,    42,   356,    23,
     130,   475,    16,   489,   475,    19,   480,    67,    53,    67,
      19,    71,   353,    19,   594,    75,    76,    75,    15,   160,
     421,    22,    67,    24,   444,   166,   167,   607,    15,    88,
     171,   172,    15,    16,   164,   176,   177,   395,     4,   270,
      23,   100,     1,    67,    25,     4,   578,   467,   468,    15,
     470,    75,   467,   473,   474,   475,    15,    15,   559,    25,
     480,    16,   122,    84,    83,    84,    22,    22,   128,    19,
     130,   572,   202,   490,   494,   495,    97,    25,   138,   494,
      19,    25,   103,    67,   409,    15,    16,    26,   490,   490,
     490,   151,   152,    23,   115,   155,   570,    36,    37,   330,
      39,    67,    16,    42,   164,   490,    24,   338,    67,    75,
     373,   374,   242,    25,    53,    15,    75,    56,   538,   249,
     261,   181,    25,   264,    19,   185,   546,   187,   188,    15,
     190,    17,    15,    16,   371,   372,   350,    23,     2,   559,
      23,    19,   202,   569,   379,   380,    19,   361,    29,    30,
      31,    32,   572,    34,    35,    36,    37,    16,    15,    16,
      99,    57,   281,    22,   565,     0,    23,   286,    16,   589,
      16,   591,   291,   599,    16,   595,   596,   237,   238,   239,
     240,   241,   242,    16,   244,    19,   606,    15,   248,   249,
      76,    77,    78,    28,    16,    15,    19,    32,    33,   259,
     330,    16,    37,   417,    16,    19,   626,    31,   338,    73,
     270,    19,    76,    48,   634,   635,   636,   637,     4,   349,
      19,    16,    16,    58,    16,    16,   356,     0,     4,    15,
     375,   376,   377,   378,    19,     0,    71,    23,    73,    15,
     382,    26,   381,   383,   123,   356,   306,    23,   462,   463,
     384,    36,    37,   606,    39,   386,    42,    42,   409,   400,
     320,   321,   403,   267,   349,   395,    42,    53,    53,   329,
     330,    56,   242,   487,   412,   307,   401,    53,   338,   256,
     138,    67,   146,   147,    48,   407,    -1,   151,    -1,   349,
     322,   155,    -1,   353,    -1,   355,   356,    -1,    -1,    -1,
      -1,    -1,   362,    -1,   364,   186,   187,   426,   189,   190,
     429,    -1,    -1,   432,    99,    -1,   435,   152,    26,    -1,
      -1,   185,    -1,   187,   188,   204,   190,    -1,    36,    37,
       4,    39,   211,   212,    42,   395,    -1,   216,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    53,    -1,    -1,    56,   409,
     185,    -1,   187,   188,   233,   190,    -1,   238,    -1,   240,
     490,     4,   576,    -1,   245,   246,   247,   248,    42,    -1,
      -1,    -1,    15,    -1,   238,    -1,   240,    -1,   257,    53,
      23,    -1,    -1,    -1,   248,   599,   527,    -1,    -1,   530,
      -1,    99,    -1,    67,    -1,    -1,    -1,     4,   262,    42,
      -1,    75,   237,   238,   239,   240,   270,   467,    15,   244,
      53,    -1,   444,   248,    -1,   475,    -1,   536,   537,    -1,
       4,    -1,   301,    -1,    67,    -1,    -1,   262,    -1,    -1,
     490,    15,    75,    -1,   494,    42,   468,    -1,   470,    -1,
      -1,   473,   474,    -1,    -1,    -1,    53,    -1,    -1,    29,
      30,    31,    32,    -1,    34,    35,    36,    37,    42,    -1,
      67,    -1,    -1,   495,    -1,   329,   330,    -1,    75,    53,
      -1,    -1,   353,   337,   338,    -1,    -1,   126,   538,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,    -1,   386,    -1,    -1,
      -1,    -1,   161,   162,   546,    -1,    -1,    -1,   397,   168,
     169,    -1,    -1,    -1,   173,   174,    -1,   559,    -1,   178,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     572,    -1,    -1,   407,    -1,    -1,   606,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   589,    -1,   591,
      -1,    -1,    -1,   595,   596,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   407,    -1,   409,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   235,    -1,     3,     4,
       5,     6,     7,     8,   626,    -1,    -1,    -1,    -1,    -1,
      15,    16,   634,   635,   636,   637,   186,   187,    -1,   189,
     190,   475,    -1,    -1,   493,    -1,   480,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   280,    -1,    -1,    -1,    -1,   285,    52,    -1,    -1,
     475,   290,    -1,    -1,    -1,   480,   295,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,   238,    74,
     240,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,   578,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    99,    19,    20,
      21,   103,   104,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    75,    -1,    77,    78,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    99,    19,
      20,    21,   103,   104,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    -1,    77,    78,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    99,
      19,    20,    -1,   103,   104,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    75,    -1,    77,    78,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      99,    19,    20,    21,   103,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    50,    51,    52,    -1,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    -1,    77,
      78,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,
      30,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      50,    51,    52,    -1,    54,    55,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    71,    72,    -1,    74,    75,    -1,    77,    78,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    30,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    50,    51,
      52,    -1,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    75,    -1,    77,    78,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,   103,   104,    -1,    -1,    -1,    30,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    50,    51,    52,    -1,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    75,    -1,    77,    78,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,     1,    52,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    71,    72,    -1,    74,    52,   103,    77,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    -1,   103,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,   103,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    -1,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    -1,    74,    75,    -1,    77,
      78,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    99,    -1,    -1,    -1,   103,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    27,    28,    -1,    -1,
      15,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      52,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    67,    68,    69,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,   103,    77,    78,     1,    -1,    -1,     4,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    -1,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    -1,    53,     1,    -1,    56,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    15,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,    53,
       1,    -1,    56,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    26,    27,    28,    29,    -1,
      -1,    32,    -1,    -1,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    99,    -1,    48,    49,    -1,
      -1,    -1,    53,     1,    -1,    56,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    26,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,    37,
      38,    39,    -1,    41,    42,    43,    44,    45,    99,    -1,
      48,    49,    -1,    -1,    -1,    53,     1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
      -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,    -1,
      35,    36,    37,    38,    39,    -1,    41,    42,    43,    44,
      45,    99,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,
      -1,    56,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      75,    23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,
      32,    -1,    87,    35,    36,    37,    38,    39,    -1,    41,
      42,    43,    44,    45,    99,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    56,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    15,    16,    -1,    -1,
      -1,    -1,    -1,    75,    23,    -1,    -1,    26,    27,    28,
      29,    -1,    -1,    32,    -1,    87,    35,    36,    37,    38,
      39,    -1,    41,    42,    43,    44,    45,    99,    -1,    48,
      49,    -1,    -1,    -1,    53,    -1,    -1,    56,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    15,
      16,    -1,    -1,    -1,    -1,    -1,    75,    23,    -1,    -1,
      26,    27,    28,    29,    -1,    -1,    32,    -1,    87,    35,
      36,    37,    38,    39,    -1,    41,    42,    43,    44,    45,
      99,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,
      56,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,
      -1,    87,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    -1,    56,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    26,    27,    28,    29,
      -1,    -1,    32,    -1,    -1,    35,    36,    37,    38,    39,
       4,    41,    42,    43,    44,    45,    99,    -1,    48,    49,
      -1,    15,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    25,    -1,    27,    28,    29,    -1,    67,    32,    -1,
      -1,    35,    -1,    -1,    38,    75,    -1,    41,    42,    43,
      44,    45,     4,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    99,
      -1,    23,    -1,    67,    -1,    27,    28,    29,    -1,    -1,
      32,    75,    -1,    35,    -1,    -1,    38,    -1,    -1,    41,
      42,    43,    44,    45,    -1,    -1,    48,    49,    -1,    16,
      -1,    53,    -1,    -1,    -1,    99,    -1,    -1,    -1,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    35,    36,
      37,    38,    39,    75,    41,    42,    43,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      67,    -1,    16,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      87,    35,    36,    37,    38,    39,    -1,    41,    42,    43,
      44,    45,    99,    -1,    48,    49,    -1,    -1,    -1,    53,
      -1,    -1,    56,    21,    -1,    -1,    -1,    -1,    -1,    27,
      28,    29,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      38,    75,    -1,    41,    42,    43,    44,    45,    -1,    -1,
      48,    49,    -1,    87,    -1,    53,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    99,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    20,    21,    -1,    -1,    75,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,     3,     4,     5,     6,     7,     8,    -1,    -1,
      24,    99,    -1,    -1,    15,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    71,    72,    -1,    74,    52,    -1,
      77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    15,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    52,    74,
      -1,    -1,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    67,    68,    69,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    -1,    67,    68,    69,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,     3,     4,
       5,     6,     7,     8,    -1,    26,    27,    28,    29,    -1,
      15,    32,    -1,    -1,    35,    36,    37,    38,    39,    -1,
      41,    42,    43,    44,    45,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    26,    27,    28,    -1,    99,    -1,
      32,    -1,    -1,    -1,    36,    37,    38,    39,    -1,    -1,
      42,    43,    44,    45,    -1,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

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
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

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
	    (To)[yyi] = (From)[yyi];		\
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
#define YYABORT 	goto yyabortlab
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
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
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

#ifdef YYERROR_VERBOSE

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
#endif

#line 315 "/usr/local/share/bison/bison.simple"


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

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

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

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
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
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
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
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

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
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 396 "parser.y"
{ yyval.exprN = yyvsp[0].idN; ;
    break;}
case 4:
#line 402 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 5:
#line 404 "parser.y"
{ if (yyvsp[-1].exprN->typ() == Comma) yyvsp[-1].exprN->coord(Coord(yyvsp[-2].tok));
                                  yyvsp[-1].exprN->parenthesized(true);
                                  yyval.exprN = yyvsp[-1].exprN; ;
    break;}
case 6:
#line 411 "parser.y"
{ if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
               yyval.exprN = (exprNode *) /* This is BAD */
		 new blockNode((decl_list *)0, yyvsp[-2].stmtL, Coord(yyvsp[-4].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 7:
#line 417 "parser.y"
{ if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
              yyval.exprN =  (exprNode *) /* This is BAD */
		new blockNode(yyvsp[-3].declL, yyvsp[-2].stmtL, Coord(yyvsp[-5].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 8:
#line 425 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 9:
#line 428 "parser.y"
{ /* I don't like this (SZG)...
		 if ($1->typ() == Index)
	        { indexNode * atmp = (indexNode *)$1;
		  atmp->add_dim($3);
		  $$ = atmp; }
	      else */
	      { /* Only allow one dimension arg per indexNode */
		yyval.exprN = new binaryNode(Operator::Index, yyvsp[-3].exprN, yyvsp[-1].exprN, Coord(yyvsp[-2].tok));
	      }
	    ;
    break;}
case 10:
#line 440 "parser.y"
{ yyval.exprN = new callNode(yyvsp[-2].exprN, (expr_list *)0, Coord(yyvsp[-1].tok)); ;
    break;}
case 11:
#line 443 "parser.y"
{ yyval.exprN = new callNode(yyvsp[-3].exprN, yyvsp[-1].exprL, Coord(yyvsp[-2].tok)); ;
    break;}
case 12:
#line 446 "parser.y"
{ yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 13:
#line 449 "parser.y"
{ yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 14:
#line 452 "parser.y"
{ yyval.exprN = new unaryNode( Operator::POSTINC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); ;
    break;}
case 15:
#line 455 "parser.y"
{ yyval.exprN = new unaryNode( Operator::POSTDEC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); ;
    break;}
case 16:
#line 459 "parser.y"
{ yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 17:
#line 462 "parser.y"
{ yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 18:
#line 467 "parser.y"
{ yyval.exprL = new expr_list(); yyval.exprL->push_back(yyvsp[0].exprN); ;
    break;}
case 19:
#line 470 "parser.y"
{ yyval.exprL = yyvsp[-2].exprL; yyval.exprL->push_back(yyvsp[0].exprN); ;
    break;}
case 20:
#line 475 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 21:
#line 478 "parser.y"
{ yyval.exprN = new unaryNode( Operator::PREINC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 22:
#line 481 "parser.y"
{ yyval.exprN = new unaryNode( Operator::PREDEC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 23:
#line 484 "parser.y"
{ yyvsp[-1].unaryN->expr(yyvsp[0].exprN);
              yyval.exprN = yyvsp[-1].unaryN; ;
    break;}
case 24:
#line 488 "parser.y"
{ yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 25:
#line 491 "parser.y"
{ yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[-1].typeN.ty, Coord(yyvsp[-3].tok)); ;
    break;}
case 26:
#line 495 "parser.y"
{ yyval.unaryN = new unaryNode('&', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 27:
#line 496 "parser.y"
{ yyval.unaryN = new unaryNode('*', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 28:
#line 497 "parser.y"
{ yyval.unaryN = new unaryNode('+', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 29:
#line 498 "parser.y"
{ yyval.unaryN = new unaryNode('-', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 30:
#line 499 "parser.y"
{ yyval.unaryN = new unaryNode('~', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 31:
#line 500 "parser.y"
{ yyval.unaryN = new unaryNode('!', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 33:
#line 506 "parser.y"
{ yyval.exprN = new castNode(yyvsp[-2].typeN.ty, yyvsp[0].exprN, false, Coord(yyvsp[-3].tok)); ;
    break;}
case 35:
#line 512 "parser.y"
{ yyval.exprN = new binaryNode('*', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 36:
#line 515 "parser.y"
{ yyval.exprN = new binaryNode('/', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 37:
#line 518 "parser.y"
{ yyval.exprN = new binaryNode('%', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 39:
#line 524 "parser.y"
{ yyval.exprN = new binaryNode('+', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 40:
#line 527 "parser.y"
{ yyval.exprN = new binaryNode('-', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 42:
#line 533 "parser.y"
{ yyval.exprN = new binaryNode( Operator::LS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 43:
#line 536 "parser.y"
{ yyval.exprN = new binaryNode( Operator::RS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 45:
#line 542 "parser.y"
{ yyval.exprN = new binaryNode('<', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 46:
#line 545 "parser.y"
{ yyval.exprN = new binaryNode('>', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 47:
#line 548 "parser.y"
{ yyval.exprN = new binaryNode( Operator::LE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 48:
#line 551 "parser.y"
{ yyval.exprN = new binaryNode( Operator::GE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 50:
#line 557 "parser.y"
{ yyval.exprN = new binaryNode( Operator::EQ, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 51:
#line 560 "parser.y"
{ yyval.exprN = new binaryNode( Operator::NE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 53:
#line 566 "parser.y"
{ yyval.exprN = new binaryNode('&', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 55:
#line 572 "parser.y"
{ CBZ::WarnAboutPrecedence('^', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('^', yyvsp[0].exprN);
	      yyval.exprN = new binaryNode('^', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 57:
#line 580 "parser.y"
{ CBZ::WarnAboutPrecedence('|', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('|', yyvsp[0].exprN);
              yyval.exprN = new binaryNode('|', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 59:
#line 588 "parser.y"
{ yyval.exprN = new binaryNode( Operator::ANDAND, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 61:
#line 594 "parser.y"
{ CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[0].exprN);
              yyval.exprN = new binaryNode( Operator::OROR, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 63:
#line 602 "parser.y"
{ yyval.exprN = new ternaryNode(yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-3].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 65:
#line 608 "parser.y"
{ yyvsp[-1].binaryN->left(yyvsp[-2].exprN);
	      yyvsp[-1].binaryN->right(yyvsp[0].exprN);
	      yyval.exprN = yyvsp[-1].binaryN; ;
    break;}
case 66:
#line 615 "parser.y"
{ yyval.binaryN = new binaryNode('=', (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 67:
#line 618 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::MULTassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  ;
    break;}
case 68:
#line 621 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::DIVassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 69:
#line 624 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::MODassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 70:
#line 627 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::PLUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  ;
    break;}
case 71:
#line 630 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::MINUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 72:
#line 633 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::LSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 73:
#line 636 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::RSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 74:
#line 639 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::ANDassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 75:
#line 642 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::ERassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 76:
#line 645 "parser.y"
{ yyval.binaryN = new binaryNode( Operator::ORassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 77:
#line 650 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 78:
#line 652 "parser.y"
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
	    ;
    break;}
case 79:
#line 663 "parser.y"
{ yyval.exprN = new metaexprNode(yyvsp[0].idN); ;
    break;}
case 80:
#line 667 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 81:
#line 671 "parser.y"
{ yyval.exprN = (exprNode *) 0; ;
    break;}
case 82:
#line 672 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 83:
#line 709 "parser.y"
{ yyval.declL = yyvsp[-1].declL; ;
    break;}
case 84:
#line 712 "parser.y"
{ yyval.declL = yyvsp[-1].declL; ;
    break;}
case 85:
#line 716 "parser.y"
{ yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); ;
    break;}
case 86:
#line 722 "parser.y"
{ yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); ;
    break;}
case 87:
#line 730 "parser.y"
{ yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); ;
    break;}
case 88:
#line 732 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 89:
#line 734 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN));
	    ;
    break;}
case 90:
#line 739 "parser.y"
{ yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); ;
    break;}
case 91:
#line 741 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 92:
#line 743 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 93:
#line 747 "parser.y"
{ yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); ;
    break;}
case 94:
#line 749 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 95:
#line 751 "parser.y"
{ yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); ;
    break;}
case 96:
#line 757 "parser.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 97:
#line 760 "parser.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 98:
#line 764 "parser.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 99:
#line 767 "parser.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 101:
#line 776 "parser.y"
{ yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); ;
    break;}
case 102:
#line 778 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 103:
#line 780 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 104:
#line 784 "parser.y"
{ yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); ;
    break;}
case 105:
#line 786 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 106:
#line 788 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 107:
#line 792 "parser.y"
{ yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); ;
    break;}
case 108:
#line 794 "parser.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 109:
#line 796 "parser.y"
{ yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); ;
    break;}
case 110:
#line 802 "parser.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 111:
#line 805 "parser.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 112:
#line 809 "parser.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 113:
#line 812 "parser.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 115:
#line 820 "parser.y"
{ primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and(); 
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 116:
#line 825 "parser.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 117:
#line 828 "parser.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 118:
#line 835 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); ;
    break;}
case 119:
#line 839 "parser.y"
{ yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; ;
    break;}
case 120:
#line 844 "parser.y"
{ yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 121:
#line 848 "parser.y"
{ yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN); 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 122:
#line 856 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); ;
    break;}
case 123:
#line 860 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 124:
#line 865 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 125:
#line 873 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 126:
#line 877 "parser.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; ;
    break;}
case 127:
#line 882 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 129:
#line 891 "parser.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 130:
#line 894 "parser.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 133:
#line 906 "parser.y"
{ primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc;;
    break;}
case 134:
#line 911 "parser.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 136:
#line 919 "parser.y"
{ yyval.typeN.ty = yyvsp[0].primN;  
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 137:
#line 923 "parser.y"
{ yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 138:
#line 928 "parser.y"
{ yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 139:
#line 932 "parser.y"
{ yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN);
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 140:
#line 939 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty; 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 141:
#line 943 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord));
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 142:
#line 948 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 143:
#line 956 "parser.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, typeNode::NONE, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 144:
#line 960 "parser.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 145:
#line 965 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 147:
#line 973 "parser.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 149:
#line 979 "parser.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 150:
#line 985 "parser.y"
{ yyval.typeN.ty = yyvsp[0].sueN; 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 151:
#line 989 "parser.y"
{ yyval.typeN.ty = yyvsp[0].enumN;  
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 155:
#line 999 "parser.y"
{
	      CBZ::Warning(2, string("function prototype parameters must have types"));
              /* SZG Don't need this? $$ = AddDefaultParameterTypes($1); */
            ;
    break;}
case 157:
#line 1011 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); ;
    break;}
case 158:
#line 1014 "parser.y"
{ yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-3].tok)), declNode::NONE, Redecl); ;
    break;}
case 159:
#line 1017 "parser.y"
{ yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(yyvsp[-3].tq.tq,(typeNode *)0, Coord(yyvsp[-4].tok)), yyvsp[-3].tq.sc, Redecl); ;
    break;}
case 160:
#line 1020 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq,(typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); ;
    break;}
case 161:
#line 1027 "parser.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 162:
#line 1030 "parser.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[-1].typeN.ty); ;
    break;}
case 163:
#line 1033 "parser.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 164:
#line 1039 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 165:
#line 1043 "parser.y"
{ yyval.declN = yyvsp[-1].declN;  ;
    break;}
case 166:
#line 1049 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 167:
#line 1053 "parser.y"
{ yyval.declN = new declNode(yyvsp[-1].idN, declNode::NONE, yyvsp[0].typeN.ty, (exprNode *)0, (exprNode *)0); ;
    break;}
case 170:
#line 1067 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); ;
    break;}
case 171:
#line 1070 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); ;
    break;}
case 172:
#line 1076 "parser.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 173:
#line 1079 "parser.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 177:
#line 1092 "parser.y"
{ yyval.typeN.ty = new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[0].tok)); 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 178:
#line 1096 "parser.y"
{ yyval.typeN.ty = new ptrNode(yyvsp[0].tq.tq, (typeNode *)0, Coord(yyvsp[-1].tok));  
	      yyval.typeN.sc = yyvsp[0].tq.sc; ;
    break;}
case 179:
#line 1100 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)));   
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 180:
#line 1104 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)));   
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 181:
#line 1111 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 182:
#line 1115 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 183:
#line 1119 "parser.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 184:
#line 1123 "parser.y"
{ yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(yyvsp[0].typeN.ty); 
	      yyval.typeN.sc = merge_sc(yyvsp[-2].typeN.sc, yyvsp[0].typeN.sc, yyvsp[-2].typeN.ty->coord()); ;
    break;}
case 185:
#line 1130 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty;
	       yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 186:
#line 1134 "parser.y"
{ yyval.typeN.ty = new funcNode(typeNode::NONE, (decl_list *)0, (typeNode *)0, Coord(yyvsp[-1].tok)); 
	       yyval.typeN.sc = declNode::NONE; ;
    break;}
case 187:
#line 1138 "parser.y"
{ yyval.typeN.ty = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok)); 
	       yyval.typeN.sc = declNode::NONE; ;
    break;}
case 188:
#line 1145 "parser.y"
{ yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, (exprNode *)0, Coord(yyvsp[-1].tok));
              yyval.typeN.sc = declNode::NONE; ;
    break;}
case 189:
#line 1149 "parser.y"
{ yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok)); 
              yyval.typeN.sc = declNode::NONE; ;
    break;}
case 190:
#line 1153 "parser.y"
{ yyval.typeN.ty = yyvsp[-3].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok))); 
              yyval.typeN.sc = yyvsp[-3].typeN.sc; ;
    break;}
case 191:
#line 1159 "parser.y"
{ 
              CBZ::SyntaxError(string("array declaration with illegal empty dimension"));
              yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0,
							     (exprNode *) new constNode(constant((int)1)),
							     Coord(yyvsp[-1].tok)));
	      yyval.typeN.sc = declNode::NONE;
            ;
    break;}
case 195:
#line 1179 "parser.y"
{ yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok))); ;
    break;}
case 196:
#line 1182 "parser.y"
{ yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok))); ;
    break;}
case 197:
#line 1188 "parser.y"
{ yyval.declN = yyvsp[-1].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 198:
#line 1191 "parser.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 199:
#line 1194 "parser.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 200:
#line 1200 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 201:
#line 1203 "parser.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 202:
#line 1209 "parser.y"
{ 
/*              OldStyleFunctionDefinition = TRUE; */
              yyval.declN = yyvsp[0].declN; 
            ;
    break;}
case 203:
#line 1215 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, SU); ;
    break;}
case 204:
#line 1218 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), declNode::NONE, SU); ;
    break;}
case 205:
#line 1224 "parser.y"
{ funcNode *f = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok));
	      f->is_knr (true);
	      yyval.declN = yyvsp[-3].declN->modify_type_and(f); ;
    break;}
case 206:
#line 1229 "parser.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 207:
#line 1232 "parser.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 208:
#line 1247 "parser.y"
{ yyval.declL = new decl_list();
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    ;
    break;}
case 209:
#line 1256 "parser.y"
{ yyval.declL = yyvsp[-2].declL;
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    ;
    break;}
case 212:
#line 1274 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 213:
#line 1278 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty)->finish_and(); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord()); ;
    break;}
case 214:
#line 1282 "parser.y"
{ yyval.typeN.ty = new primNode(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = yyvsp[0].tq.sc; ;
    break;}
case 215:
#line 1286 "parser.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 216:
#line 1295 "parser.y"
{ yyval.attribL = (attrib_list *)0; ;
    break;}
case 217:
#line 1297 "parser.y"
{ yyval.attribL = yyvsp[0].attribL; ;
    break;}
case 218:
#line 1302 "parser.y"
{ yyval.attribL = yyvsp[0].attribL; ;
    break;}
case 219:
#line 1304 "parser.y"
{ yyval.attribL = yyvsp[-1].attribL; 
		  yyvsp[-1].attribL->splice(yyvsp[-1].attribL->end(), * yyvsp[0].attribL); 
		  delete yyvsp[0].attribL; ;
    break;}
case 220:
#line 1311 "parser.y"
{ if (CBZ::ANSIOnly)
	            CBZ::SyntaxError(string("__attribute__ not allowed with -ansi switch"));
                  yyval.attribL = yyvsp[-2].attribL; ;
    break;}
case 221:
#line 1318 "parser.y"
{ yyval.attribL = new attrib_list(); yyval.attribL->push_back(yyvsp[0].attribN); ;
    break;}
case 222:
#line 1320 "parser.y"
{ yyval.attribL = yyvsp[-2].attribL; yyval.attribL->push_back(yyvsp[0].attribN); ;
    break;}
case 223:
#line 1325 "parser.y"
{ yyval.attribN = 0; ;
    break;}
case 224:
#line 1327 "parser.y"
{ yyval.attribN = new attribNode(yyvsp[0].idN, (exprNode *)0); ;
    break;}
case 225:
#line 1329 "parser.y"
{ yyval.attribN = new attribNode(yyvsp[-3].idN, yyvsp[-1].exprN); ;
    break;}
case 228:
#line 1336 "parser.y"
{ yyval.idN = new idNode("const", Coord(yyvsp[0].tok)); ;
    break;}
case 229:
#line 1341 "parser.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 230:
#line 1342 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 231:
#line 1348 "parser.y"
{ yyval.exprN = yyvsp[-1].initializerN; yyval.exprN->coord(yyvsp[-2].tok); ;
    break;}
case 232:
#line 1350 "parser.y"
{ yyval.exprN = yyvsp[-2].initializerN; yyval.exprN->coord(yyvsp[-3].tok); ;
    break;}
case 233:
#line 1351 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 234:
#line 1357 "parser.y"
{ yyval.initializerN = new initializerNode(new expr_list(), yyvsp[0].exprN->coord());
	      yyval.initializerN->add_expr(yyvsp[0].exprN); ;
    break;}
case 235:
#line 1361 "parser.y"
{ yyval.initializerN = yyvsp[-2].initializerN;
	      yyval.initializerN->add_expr(yyvsp[0].exprN);
            ;
    break;}
case 237:
#line 1371 "parser.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(new declNode((typeNode *) new primNode(basic_type::Ellipsis), declNode::NONE)); ;
    break;}
case 238:
#line 1376 "parser.y"
{ /* primNode * n = new primNode(qualifier($1), basic_type::Void); */
	      CBZ::SyntaxError(Coord(yyvsp[0].tok), string("First argument cannot be `...'"));
              yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(new primNode(typeNode::NONE, basic_type::Void), declNode::NONE));
            ;
    break;}
case 239:
#line 1386 "parser.y"
{ yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[0].declN); ;
    break;}
case 240:
#line 1390 "parser.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN); ;
    break;}
case 241:
#line 1395 "parser.y"
{ 
	      CBZ::SyntaxError(yyvsp[-2].declN->coord(), string("formals cannot have initializers"));
	      yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[-2].declN);
            ;
    break;}
case 242:
#line 1402 "parser.y"
{ yyval.declL = yyvsp[-2].declL; ;
    break;}
case 243:
#line 1408 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); ;
    break;}
case 244:
#line 1411 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); ;
    break;}
case 245:
#line 1415 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 246:
#line 1418 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 247:
#line 1421 "parser.y"
{ yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); ;
    break;}
case 248:
#line 1424 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); ;
    break;}
case 249:
#line 1428 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); ;
    break;}
case 250:
#line 1431 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); ;
    break;}
case 251:
#line 1434 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); ;
    break;}
case 252:
#line 1438 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 253:
#line 1441 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 254:
#line 1444 "parser.y"
{ yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); ;
    break;}
case 255:
#line 1447 "parser.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); ;
    break;}
case 256:
#line 1451 "parser.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); ;
    break;}
case 257:
#line 1464 "parser.y"
{ yyval.sueN = yyvsp[-3].sueN->set_name_fields_and((idNode *)0, yyvsp[-1].declL, true, yyvsp[-2].tok, yyvsp[0].tok); ;
    break;}
case 258:
#line 1468 "parser.y"
{ yyval.sueN = yyvsp[-4].sueN->set_name_fields_and(yyvsp[-3].idN, yyvsp[-1].declL, true, yyvsp[-2].tok, yyvsp[0].tok); ;
    break;}
case 259:
#line 1471 "parser.y"
{ yyval.sueN = yyvsp[-1].sueN->set_name_and(yyvsp[0].idN, yyvsp[-1].sueN->coord()); ;
    break;}
case 260:
#line 1475 "parser.y"
{ 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-2].sueN->set_name_fields_and((idNode *)0, (decl_list *)0, true, yyvsp[-1].tok, yyvsp[0].tok);
	    ;
    break;}
case 261:
#line 1482 "parser.y"
{ 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-3].sueN->set_name_fields_and(yyvsp[-2].idN, (decl_list *)0, true, yyvsp[-1].tok, yyvsp[0].tok);
	    ;
    break;}
case 262:
#line 1491 "parser.y"
{ yyval.sueN = new structNode(Coord(yyvsp[0].tok)); ;
    break;}
case 263:
#line 1492 "parser.y"
{ yyval.sueN = new unionNode(Coord(yyvsp[0].tok)); ;
    break;}
case 265:
#line 1499 "parser.y"
{ yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyval.declL->end(), * yyvsp[0].declL);
	      delete yyvsp[0].declL; ;
    break;}
case 268:
#line 1514 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, SU)); ;
    break;}
case 269:
#line 1518 "parser.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); ;
    break;}
case 270:
#line 1525 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, SU)); ;
    break;}
case 271:
#line 1529 "parser.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); ;
    break;}
case 272:
#line 1537 "parser.y"
{ yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 273:
#line 1542 "parser.y"
{ yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 274:
#line 1550 "parser.y"
{ yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 275:
#line 1555 "parser.y"
{ yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 276:
#line 1562 "parser.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 278:
#line 1568 "parser.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 279:
#line 1574 "parser.y"
{ yyval.enumN = new enumNode((idNode *)0, yyvsp[-2].declL, yyvsp[-4].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 280:
#line 1577 "parser.y"
{ yyval.enumN = new enumNode(yyvsp[-4].idN, yyvsp[-2].declL, yyvsp[-5].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok));   ;
    break;}
case 281:
#line 1580 "parser.y"
{ yyval.enumN = new enumNode(yyvsp[0].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].idN->coord(), yyvsp[0].idN->coord()); ;
    break;}
case 282:
#line 1586 "parser.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); ;
    break;}
case 283:
#line 1590 "parser.y"
{ yyval.declL = yyvsp[-3].declL; 
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); ;
    break;}
case 284:
#line 1596 "parser.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 285:
#line 1597 "parser.y"
{ yyval.exprN = yyvsp[0].exprN;   ;
    break;}
case 286:
#line 1601 "parser.y"
{ ;
    break;}
case 287:
#line 1602 "parser.y"
{ ;
    break;}
case 288:
#line 1613 "parser.y"
{ yyval.stmtN = yyvsp[0].targetN; ;
    break;}
case 289:
#line 1616 "parser.y"
{ yyval.stmtN = yyvsp[0].blockN; ;
    break;}
case 290:
#line 1619 "parser.y"
{ yyval.stmtN = yyvsp[0].exprstmtN; ;
    break;}
case 291:
#line 1622 "parser.y"
{ yyval.stmtN = yyvsp[0].stmtN; ;
    break;}
case 292:
#line 1625 "parser.y"
{ yyval.stmtN = yyvsp[0].loopN; ;
    break;}
case 293:
#line 1628 "parser.y"
{ yyval.stmtN = yyvsp[0].jumpN; ;
    break;}
case 294:
#line 1639 "parser.y"
{  yyval.stmtN = (stmtNode *) 0; ;
    break;}
case 295:
#line 1640 "parser.y"
{ yyval.stmtN = new metastmtNode(yyvsp[0].idN); ;
    break;}
case 296:
#line 1645 "parser.y"
{ yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); ;
    break;}
case 297:
#line 1648 "parser.y"
{ yyval.targetN = new caseNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-3].tok)); ;
    break;}
case 298:
#line 1651 "parser.y"
{ yyval.targetN = new caseNode(0, yyvsp[0].stmtN, Coord(yyvsp[-2].tok)); ;
    break;}
case 299:
#line 1655 "parser.y"
{ yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); ;
    break;}
case 300:
#line 1661 "parser.y"
{ yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 301:
#line 1664 "parser.y"
{ yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 302:
#line 1667 "parser.y"
{ yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 303:
#line 1670 "parser.y"
{ yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 304:
#line 1673 "parser.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->enter_scope(); ;
    break;}
case 305:
#line 1674 "parser.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_scope(); ;
    break;}
case 306:
#line 1677 "parser.y"
{ yyval.tok =yyvsp[0].tok; CBZ::current_unit->enter_pragmas(); ;
    break;}
case 307:
#line 1679 "parser.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_pragmas(); ;
    break;}
case 308:
#line 1687 "parser.y"
{ yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 309:
#line 1690 "parser.y"
{ yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 310:
#line 1693 "parser.y"
{ yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 311:
#line 1696 "parser.y"
{ yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 312:
#line 1702 "parser.y"
{ yyval.declL = yyvsp[0].declL; ;
    break;}
case 313:
#line 1705 "parser.y"
{ yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyvsp[-1].declL->end(), * yyvsp[0].declL); 
	      delete yyvsp[0].declL;
	    ;
    break;}
case 314:
#line 1714 "parser.y"
{ yyval.stmtL = new stmt_list();
              yyval.stmtL->push_back(yyvsp[0].stmtN); ;
    break;}
case 315:
#line 1718 "parser.y"
{ yyval.stmtL = yyvsp[-1].stmtL;
              yyval.stmtL->push_back(yyvsp[0].stmtN); ;
    break;}
case 316:
#line 1728 "parser.y"
{ yyval.exprstmtN = new exprstmtNode(yyvsp[-1].exprN); ;
    break;}
case 317:
#line 1733 "parser.y"
{ yyval.stmtN = new ifNode(yyvsp[-2].exprN, yyvsp[0].stmtN, 0, Coord(yyvsp[-4].tok)); ;
    break;}
case 318:
#line 1736 "parser.y"
{ yyval.stmtN = new ifNode (yyvsp[-4].exprN, yyvsp[-2].stmtN, yyvsp[0].stmtN, Coord(yyvsp[-6].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 319:
#line 1739 "parser.y"
{ yyval.stmtN = new switchNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 320:
#line 1744 "parser.y"
{ yyval.loopN = new whileNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 321:
#line 1747 "parser.y"
{ yyval.loopN = new doNode(yyvsp[-5].stmtN, yyvsp[-2].exprN, Coord(yyvsp[-6].tok), Coord(yyvsp[-4].tok)); ;
    break;}
case 322:
#line 1750 "parser.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 323:
#line 1755 "parser.y"
{ yyval.loopN = new forNode((exprNode *)0, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 324:
#line 1758 "parser.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 325:
#line 1761 "parser.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, (exprNode *)0, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 326:
#line 1764 "parser.y"
{ yyval.loopN = new forNode((exprNode *)0, (exprNode *)0, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 327:
#line 1769 "parser.y"
{ yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); ;
    break;}
case 328:
#line 1772 "parser.y"
{ yyval.jumpN = new continueNode(Coord(yyvsp[-1].tok)); ;
    break;}
case 329:
#line 1775 "parser.y"
{ yyval.jumpN = new breakNode(Coord(yyvsp[-1].tok)); ;
    break;}
case 330:
#line 1778 "parser.y"
{ yyval.jumpN = new returnNode(yyvsp[-1].exprN, procNode::current(), Coord(yyvsp[-2].tok)); ;
    break;}
case 331:
#line 1782 "parser.y"
{ yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); ;
    break;}
case 332:
#line 1795 "parser.y"
{ if (yyvsp[0].defL != 0) CBZ::current_unit->merge_in(yyvsp[0].defL); ;
    break;}
case 333:
#line 1798 "parser.y"
{ CBZ::current_unit->merge_in(yyvsp[0].defL); ;
    break;}
case 334:
#line 1803 "parser.y"
{
              if (yydebug)
                {
                  /* printf("external.definition # declaration\n");
                  PrintNode(stdout, FirstItem($1), 0); 
                  printf("\n\n\n"); */
		}
              yyval.defL = new def_list();
	      while (! yyvsp[0].declL->empty()) {
		defNode * d = yyvsp[0].declL->front();
		d->grab_pragmas();
		yyvsp[0].declL->pop_front();
		yyval.defL->push_back(d);
	      }
	      delete yyvsp[0].declL;
            ;
    break;}
case 335:
#line 1820 "parser.y"
{ 
              if (yydebug)
                {
                  printf("external.definition # function.definition\n");
                  // PrintNode(stdout, $1, 0); 
                  printf("\n\n\n");
                }
	      yyvsp[0].procN->grab_pragmas();
              yyval.defL = new def_list();
	      yyval.defL->push_back(yyvsp[0].procN);
            ;
    break;}
case 336:
#line 1836 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); ;
    break;}
case 337:
#line 1838 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 338:
#line 1844 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[-1].declN); ;
    break;}
case 339:
#line 1847 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); ;
    break;}
case 340:
#line 1849 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 341:
#line 1854 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); ;
    break;}
case 342:
#line 1856 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 343:
#line 1859 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)); ;
    break;}
case 344:
#line 1861 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 345:
#line 1863 "parser.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)); ;
    break;}
case 346:
#line 1865 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 347:
#line 1869 "parser.y"
{ yyval.procN = new procNode(true, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); ;
    break;}
case 348:
#line 1871 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 349:
#line 1874 "parser.y"
{  yyval.procN = new procNode(true, 
				 yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				     add_parameter_types_and((decl_list *)0)); ;
    break;}
case 350:
#line 1878 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 351:
#line 1881 "parser.y"
{ yyval.procN = new procNode(true, 
				yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); ;
    break;}
case 352:
#line 1885 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 353:
#line 1888 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0));
            ;
    break;}
case 354:
#line 1893 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 355:
#line 1896 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); ;
    break;}
case 356:
#line 1900 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 357:
#line 1903 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-1].declN->coord()), declNode::NONE, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 358:
#line 1907 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 359:
#line 1910 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 360:
#line 1914 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 361:
#line 1917 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 362:
#line 1921 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 363:
#line 1924 "parser.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 364:
#line 1928 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 365:
#line 1931 "parser.y"
{ yyval.procN = new procNode(true, yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				          add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 366:
#line 1934 "parser.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 367:
#line 1939 "parser.y"
{ CBZ::OldStyleFunctionDefinition = true; ;
    break;}
case 368:
#line 1941 "parser.y"
{ CBZ::OldStyleFunctionDefinition = false; 
	       yyval.declL = yyvsp[0].declL; ;
    break;}
case 369:
#line 1956 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 370:
#line 1957 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 371:
#line 1958 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 372:
#line 1959 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 373:
#line 1960 "parser.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 374:
#line 1965 "parser.y"
{ yyval.constN = yyvsp[0].constN; ;
    break;}
case 375:
#line 1967 "parser.y"
{
	      string s3 = string(yyvsp[-1].constN->value().Str()) + string(yyvsp[0].constN->value().Str());
              /* append text(), get rid of the quotes in between. */
              string t1 = yyvsp[-1].constN->text().substr(0,yyvsp[-1].constN->text().length()-1),
                     t2 = yyvsp[0].constN->text().substr(1,string::npos);
	      yyval.constN = new constNode(constant(s3.c_str()), (t1+t2).c_str(), yyvsp[-1].constN->coord());
	      delete yyvsp[-1].constN;
	      delete yyvsp[0].constN;
	    ;
    break;}
case 376:
#line 1979 "parser.y"
{ yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 377:
#line 1980 "parser.y"
{ yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 378:
#line 1981 "parser.y"
{ yyval.tq.tq = typeNode::INLINE;   yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 379:
#line 1985 "parser.y"
{ yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 380:
#line 1986 "parser.y"
{ yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 381:
#line 1990 "parser.y"
{ yyval.tq.sc = declNode::TYPEDEF;  yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 382:
#line 1991 "parser.y"
{ yyval.tq.sc = declNode::EXTERN;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 383:
#line 1992 "parser.y"
{ yyval.tq.sc = declNode::STATIC;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 384:
#line 1993 "parser.y"
{ yyval.tq.sc = declNode::AUTO;     yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 385:
#line 1994 "parser.y"
{ yyval.tq.sc = declNode::REGISTER; yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 386:
#line 1998 "parser.y"
{ yyval.primN = new primNode(basic_type::Void, Coord(yyvsp[0].tok));    ;
    break;}
case 387:
#line 1999 "parser.y"
{ yyval.primN = new primNode(basic_type::Char, Coord(yyvsp[0].tok));    ;
    break;}
case 388:
#line 2000 "parser.y"
{ yyval.primN = new primNode(basic_type::Int, Coord(yyvsp[0].tok));     ;
    break;}
case 389:
#line 2001 "parser.y"
{ yyval.primN = new primNode(basic_type::Float, Coord(yyvsp[0].tok));   ;
    break;}
case 390:
#line 2002 "parser.y"
{ yyval.primN = new primNode(basic_type::Double, Coord(yyvsp[0].tok));  ;
    break;}
case 391:
#line 2004 "parser.y"
{ yyval.primN = new primNode(basic_type::Signed, Coord(yyvsp[0].tok));  ;
    break;}
case 392:
#line 2005 "parser.y"
{ yyval.primN = new primNode(basic_type::Unsigned, Coord(yyvsp[0].tok));;
    break;}
case 393:
#line 2007 "parser.y"
{ yyval.primN = new primNode(basic_type::Short, Coord(yyvsp[0].tok));   ;
    break;}
case 394:
#line 2008 "parser.y"
{ yyval.primN = new primNode(basic_type::Long, Coord(yyvsp[0].tok));    ;
    break;}
}

#line 705 "/usr/local/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

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
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
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
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

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

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 2020 "parser.y"

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
