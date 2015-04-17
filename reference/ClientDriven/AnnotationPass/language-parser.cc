/* A Bison parser, made from language.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse annparse
#define yylex annlex
#define yyerror annerror
#define yylval annlval
#define yychar annchar
#define yydebug anndebug
#define yynerrs annnerrs
# define	tokID	257
# define	tokCONSTANT	258
# define	tokENABLE	259
# define	tokDISABLE	260
# define	tokMAYPROPERTY	261
# define	tokMUSTPROPERTY	262
# define	tokGLOBAL	263
# define	tokINITIALLY	264
# define	tokDIAGNOSTIC	265
# define	tokPROCEDURE	266
# define	tokACCESS	267
# define	tokMODIFY	268
# define	tokANALYZE	269
# define	tokDEFAULT	270
# define	tokIF	271
# define	tokIS_EXACTLY	272
# define	tokIS_ATLEAST	273
# define	tokCOULD_BE	274
# define	tokIS_ATMOST	275
# define	tokIS_BOTTOM	276
# define	tokUNION_SET	277
# define	tokINTERSECT_SET	278
# define	tokUNION_EQUIV	279
# define	tokINTERSECT_EQUIV	280
# define	tokIS_ELEMENT_OF	281
# define	tokIS_EMPTYSET	282
# define	tokADD	283
# define	tokALWAYS	284
# define	tokEVER	285
# define	tokBEFORE	286
# define	tokAFTER	287
# define	tokTRACE	288
# define	tokCONFIDENCE	289
# define	tokFORWARD	290
# define	tokBACKWARD	291
# define	tokIS_ALIASOF	292
# define	tokIS_SAMEAS	293
# define	tokIS_EMPTY	294
# define	tokON_ENTRY	295
# define	tokON_EXIT	296
# define	tokNEW	297
# define	tokARROW	298
# define	tokDELETE	299
# define	tokPATTERN	300
# define	tokANDAND	301
# define	tokOROR	302
# define	tokNOT_EQUALS	303
# define	tokEQUALS_EQUALS	304
# define	tokLESS_EQUALS	305
# define	tokGREATER_EQUALS	306
# define	tokIS_CONSTANT	307
# define	tokASSIGN	308
# define	tokWEAKASSIGN	309
# define	tokEQUALS	310
# define	tokREPORT	311
# define	tokERROR	312
# define	tokAPPEND	313
# define	tokWHEN	314
# define	tokREPLACE	315
# define	tokINLINE	316
# define	tokIO	317
# define	tokSTRING	318
# define	WEAK_REDUCE	319
# define	LOWER_THAN_EXPRESSION	320
# define	LOWER_THAN_UNARY	321
# define	LOWER_THAN_POSTFIX	322
# define	PARENTHESIZED	323
# define	ctokAND	324
# define	ctokSTAR	325
# define	ctokPLUS	326
# define	ctokMINUS	327
# define	ctokTILDE	328
# define	ctokNOT	329
# define	ctokLESS	330
# define	ctokGREATER	331
# define	ctokOR	332
# define	ctokHAT	333
# define	ctokPERCENT	334
# define	ctokDIVIDE	335
# define	ctokLEFTPAREN	336
# define	ctokRIGHTPAREN	337
# define	ctokDOT	338
# define	ctokQUESTION	339
# define	ctokSEMICOLON	340
# define	ctokLEFTBRACE	341
# define	ctokRIGHTBRACE	342
# define	ctokCOMMA	343
# define	ctokLEFTBRACKET	344
# define	ctokRIGHTBRACKET	345
# define	ctokCOLON	346
# define	ctokAUTO	347
# define	ctokDOUBLE	348
# define	ctokINT	349
# define	ctokSTRUCT	350
# define	ctokBREAK	351
# define	ctokELSE	352
# define	ctokLONG	353
# define	ctokSWITCH	354
# define	ctokCASE	355
# define	ctokENUM	356
# define	ctokREGISTER	357
# define	ctokTYPEDEF	358
# define	ctokCHAR	359
# define	ctokEXTERN	360
# define	ctokRETURN	361
# define	ctokUNION	362
# define	ctokCONST	363
# define	ctokFLOAT	364
# define	ctokSHORT	365
# define	ctokUNSIGNED	366
# define	ctokCONTINUE	367
# define	ctokFOR	368
# define	ctokSIGNED	369
# define	ctokVOID	370
# define	ctokDEFAULT	371
# define	ctokGOTO	372
# define	ctokSIZEOF	373
# define	ctokVOLATILE	374
# define	ctokDO	375
# define	ctokIF	376
# define	ctokSTATIC	377
# define	ctokWHILE	378
# define	ctokUPLUS	379
# define	ctokUMINUS	380
# define	ctokINDIR	381
# define	ctokADDRESS	382
# define	ctokPOSTINC	383
# define	ctokPOSTDEC	384
# define	ctokPREINC	385
# define	ctokPREDEC	386
# define	ctokBOGUS	387
# define	IDENTIFIER	388
# define	STRINGliteral	389
# define	FLOATINGconstant	390
# define	INTEGERconstant	391
# define	OCTALconstant	392
# define	HEXconstant	393
# define	WIDECHARconstant	394
# define	CHARACTERconstant	395
# define	TYPEDEFname	396
# define	ctokARROW	397
# define	ctokICR	398
# define	ctokDECR	399
# define	ctokLS	400
# define	ctokRS	401
# define	ctokLE	402
# define	ctokGE	403
# define	ctokEQ	404
# define	ctokNE	405
# define	ctokANDAND	406
# define	ctokOROR	407
# define	ctokELLIPSIS	408
# define	ctokEQUALS	409
# define	ctokMULTassign	410
# define	ctokDIVassign	411
# define	ctokMODassign	412
# define	ctokPLUSassign	413
# define	ctokMINUSassign	414
# define	ctokLSassign	415
# define	ctokRSassign	416
# define	ctokANDassign	417
# define	ctokERassign	418
# define	ctokORassign	419
# define	ctokINLINE	420
# define	ctokATTRIBUTE	421
# define	ctokMETA_TYPE_EXPR	422
# define	ctokMETA_TYPE_STMT	423
# define	ctokMETA_EXPR	424
# define	ctokMETA_STMT	425
# define	metatokID	426
# define	metatokEXPR	427
# define	metatokSTMT	428
# define	metatokTYPE	429

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


#line 29 "language.y"
#ifndef YYSTYPE
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
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		887
#define	YYFLAG		-32768
#define	YYNTBASE	200

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 429 ? yytranslate[x] : 364)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    69,     2,     2,    83,    79,    78,     2,
      73,    74,    77,    80,    67,    66,    82,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    65,    68,
     198,     2,   199,     2,    84,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   196,     2,   197,    71,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    75,    81,    76,    72,     2,     2,     2,
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
      56,    57,    58,    59,    60,    61,    62,    63,    64,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    12,    15,    17,    19,
      21,    23,    25,    30,    33,    41,    49,    54,    59,    64,
      69,    70,    72,    74,    75,    78,    79,    84,    88,    90,
      94,    96,    99,   104,   108,   114,   119,   121,   124,   126,
     128,   130,   132,   134,   139,   144,   149,   153,   157,   159,
     162,   170,   176,   181,   184,   186,   189,   193,   196,   200,
     205,   207,   211,   216,   221,   226,   229,   234,   237,   243,
     249,   251,   254,   262,   268,   273,   276,   278,   282,   286,
     289,   293,   299,   303,   310,   315,   319,   323,   326,   328,
     331,   335,   339,   342,   344,   347,   351,   355,   359,   362,
     366,   368,   370,   372,   374,   376,   378,   380,   382,   384,
     386,   387,   391,   399,   403,   411,   413,   417,   419,   424,
     427,   430,   434,   438,   445,   452,   454,   456,   460,   462,
     465,   469,   473,   477,   481,   485,   489,   491,   493,   497,
     501,   505,   509,   513,   517,   521,   525,   529,   532,   535,
     538,   540,   542,   544,   548,   554,   561,   563,   565,   570,
     574,   579,   583,   587,   590,   593,   597,   601,   603,   607,
     609,   612,   615,   618,   621,   626,   628,   630,   632,   634,
     636,   638,   640,   645,   647,   651,   655,   659,   661,   665,
     669,   671,   675,   679,   681,   685,   689,   693,   697,   699,
     703,   707,   709,   713,   715,   719,   721,   725,   727,   731,
     733,   737,   739,   745,   747,   751,   753,   755,   757,   759,
     761,   763,   765,   767,   769,   771,   773,   775,   779,   781,
     782,   784,   787,   790,   793,   796,   797,   798,   805,   806,
     807,   814,   815,   816,   824,   825,   831,   832,   838,   842,
     843,   844,   851,   852,   853,   860,   861,   862,   870,   871,
     877,   878,   884,   888,   890,   892,   894,   897,   900,   903,
     906,   909,   912,   915,   918,   921,   924,   926,   929,   932,
     934,   936,   938,   940,   942,   944,   946,   949,   952,   955,
     957,   960,   963,   965,   968,   971,   973,   976,   978,   981,
     983,   985,   987,   989,   991,   993,   995,   998,  1003,  1009,
    1013,  1017,  1022,  1027,  1029,  1033,  1035,  1038,  1040,  1042,
    1045,  1049,  1053,  1058,  1060,  1062,  1064,  1066,  1069,  1072,
    1076,  1080,  1084,  1088,  1093,  1095,  1098,  1102,  1105,  1109,
    1114,  1118,  1120,  1122,  1124,  1127,  1131,  1134,  1138,  1143,
    1145,  1149,  1151,  1154,  1158,  1163,  1167,  1172,  1174,  1178,
    1180,  1182,  1184,  1187,  1189,  1192,  1193,  1195,  1197,  1200,
    1207,  1209,  1213,  1214,  1216,  1221,  1223,  1225,  1227,  1228,
    1231,  1235,  1240,  1242,  1244,  1248,  1250,  1254,  1256,  1258,
    1262,  1266,  1270,  1272,  1275,  1278,  1281,  1283,  1286,  1289,
    1291,  1294,  1297,  1300,  1302,  1305,  1308,  1313,  1319,  1322,
    1326,  1331,  1333,  1335,  1337,  1340,  1343,  1346,  1349,  1353,
    1356,  1360,  1364,  1367,  1371,  1374,  1375,  1377,  1380,  1386,
    1393,  1396,  1399,  1404,  1405,  1408,  1409,  1411,  1413,  1415,
    1417,  1419,  1421,  1423,  1426,  1429,  1433,  1438,  1442,  1446,
    1449,  1453,  1457,  1462,  1464,  1466,  1468,  1470,  1473,  1477,
    1481,  1486,  1488,  1491,  1493,  1496,  1499,  1505,  1513,  1519,
    1525,  1533,  1543,  1553,  1563,  1573,  1579,  1583,  1586,  1589,
    1593,  1597,  1599,  1602,  1604,  1606,  1607,  1611,  1614,  1615,
    1620,  1621,  1626,  1627,  1632,  1633,  1638,  1639,  1643,  1644,
    1649,  1650,  1655,  1656,  1661,  1662,  1667,  1668,  1673,  1674,
    1680,  1681,  1687,  1688,  1694,  1695,  1701,  1702,  1705,  1707,
    1709,  1711,  1713,  1715,  1717,  1720,  1722,  1724,  1726,  1728,
    1730,  1732,  1734,  1736,  1738,  1740,  1742,  1744,  1746,  1748,
    1750,  1752,  1754,  1756,  1758,  1760,  1762,  1764,  1769,  1774
};
static const short yyrhs[] =
{
     201,     0,   200,   201,     0,   203,     0,   204,     0,     5,
       3,     0,     6,     3,     0,   211,     0,   234,     0,   202,
       0,   221,     0,   347,     0,     9,    75,   218,    76,     0,
       9,   219,     0,     7,     3,    65,   205,   208,   207,   206,
       0,     8,     3,    65,   205,   208,   207,   206,     0,     7,
       3,    65,    23,     0,     8,     3,    65,    24,     0,     7,
       3,    65,    25,     0,     8,     3,    65,    26,     0,     0,
      36,     0,    37,     0,     0,    10,     3,     0,     0,    11,
      75,   235,    76,     0,    75,   209,    76,     0,   210,     0,
     209,    67,   210,     0,     3,     0,     3,   208,     0,   212,
      75,   213,    76,     0,   212,    75,    76,     0,    12,     3,
      73,   235,    74,     0,    12,     3,    73,    74,     0,   214,
       0,   213,   214,     0,   215,     0,   220,     0,   221,     0,
     230,     0,   233,     0,    41,    75,   218,    76,     0,    42,
      75,   218,    76,     0,    42,    75,   216,    76,     0,    41,
      75,    76,     0,    42,    75,    76,     0,   217,     0,   216,
     217,     0,    17,    73,   224,    74,    75,   218,    76,     0,
      17,    73,   224,    74,   219,     0,    16,    75,   218,    76,
       0,    16,   219,     0,   219,     0,   218,   219,     0,   218,
      67,   219,     0,    45,   236,     0,   218,    45,   236,     0,
     218,    67,    45,   236,     0,   236,     0,     3,    44,   219,
       0,     3,    44,    43,   219,     0,     3,    75,   218,    76,
       0,    13,    75,   235,    76,     0,    13,     3,     0,    14,
      75,   235,    76,     0,    14,     3,     0,    15,     3,    75,
     222,    76,     0,    15,     3,    75,   226,    76,     0,   223,
       0,   222,   223,     0,    17,    73,   224,    74,    75,   226,
      76,     0,    17,    73,   224,    74,   227,     0,    16,    75,
     226,    76,     0,    16,   227,     0,   225,     0,   224,    48,
     224,     0,   224,    47,   224,     0,    69,   224,     0,    73,
     224,    74,     0,     3,    65,     3,   229,    22,     0,     3,
     229,    22,     0,     3,    65,     3,   229,   228,     3,     0,
       3,   229,   228,     3,     0,     3,     3,     3,     0,     3,
      27,     3,     0,     3,    28,     0,   237,     0,     3,    53,
       0,     3,    38,     3,     0,     3,    39,     3,     0,     3,
      40,     0,   227,     0,   226,   227,     0,     3,    56,   238,
       0,     3,    54,     3,     0,     3,    55,     3,     0,    29,
       3,     0,     3,     3,     3,     0,    18,     0,    19,     0,
      20,     0,    21,     0,    32,     0,    33,     0,    30,     0,
      31,     0,    34,     0,    35,     0,     0,    57,   231,    68,
       0,    57,    17,    73,   224,    74,   231,    68,     0,    58,
     231,    68,     0,    58,    17,    73,   224,    74,   231,    68,
       0,   232,     0,   231,    59,   232,     0,    64,     0,     3,
      65,     3,   229,     0,   238,   229,     0,    84,     3,     0,
     196,     3,   197,     0,    81,     3,    81,     0,    60,    73,
     224,    74,    61,   340,     0,    60,    73,   224,    74,    62,
      68,     0,    46,     0,   236,     0,   235,    67,   236,     0,
       3,     0,    63,     3,     0,   238,    50,   238,     0,   238,
      49,   238,     0,   238,   198,   238,     0,   238,    51,   238,
       0,   238,   199,   238,     0,   238,    52,   238,     0,     4,
       0,     3,     0,    73,   238,    74,     0,   238,    80,   238,
       0,   238,    66,   238,     0,   238,    81,   238,     0,   238,
      71,   238,     0,   238,    78,   238,     0,   238,    77,   238,
       0,   238,    70,   238,     0,   238,    79,   238,     0,    66,
     238,     0,    80,   238,     0,    72,   238,     0,   359,     0,
     353,     0,   354,     0,   102,   258,   103,     0,   102,   343,
     348,   344,   103,     0,   102,   343,   347,   348,   344,   103,
       0,   361,     0,   239,     0,   240,   110,   258,   111,     0,
     240,   102,   103,     0,   240,   102,   241,   103,     0,   240,
     104,   359,     0,   240,   163,   359,     0,   240,   164,     0,
     240,   165,     0,   240,   104,   360,     0,   240,   163,   360,
       0,   256,     0,   241,   109,   256,     0,   240,     0,   164,
     242,     0,   165,   242,     0,   243,   244,     0,   139,   242,
       0,   139,   102,   313,   103,     0,    90,     0,    91,     0,
      92,     0,    93,     0,    94,     0,    95,     0,   242,     0,
     102,   313,   103,   244,     0,   244,     0,   245,    91,   244,
       0,   245,   101,   244,     0,   245,   100,   244,     0,   245,
       0,   246,    92,   245,     0,   246,    93,   245,     0,   246,
       0,   247,   166,   246,     0,   247,   167,   246,     0,   247,
       0,   248,    96,   247,     0,   248,    97,   247,     0,   248,
     168,   247,     0,   248,   169,   247,     0,   248,     0,   249,
     170,   248,     0,   249,   171,   248,     0,   249,     0,   250,
      90,   249,     0,   250,     0,   251,    99,   250,     0,   251,
       0,   252,    98,   251,     0,   252,     0,   253,   172,   252,
       0,   253,     0,   254,   173,   253,     0,   254,     0,   254,
     105,   258,   112,   255,     0,   255,     0,   242,   257,   256,
       0,   175,     0,   176,     0,   177,     0,   178,     0,   179,
       0,   180,     0,   181,     0,   182,     0,   183,     0,   184,
       0,   185,     0,   256,     0,   258,   109,   256,     0,   255,
       0,     0,   258,     0,   262,   106,     0,   271,   106,     0,
     282,   106,     0,   288,   106,     0,     0,     0,   280,   293,
     263,   314,   264,   320,     0,     0,     0,   286,   293,   265,
     314,   266,   320,     0,     0,     0,   262,   109,   293,   267,
     314,   268,   320,     0,     0,   280,     1,   269,   314,   320,
       0,     0,   286,     1,   270,   314,   320,     0,   262,   109,
       1,     0,     0,     0,   284,   305,   272,   314,   273,   320,
       0,     0,     0,   290,   305,   274,   314,   275,   320,     0,
       0,     0,   271,   109,   305,   276,   314,   277,   320,     0,
       0,   284,     1,   278,   314,   320,     0,     0,   290,     1,
     279,   314,   320,     0,   271,   109,     1,     0,   281,     0,
     282,     0,   283,     0,   287,   357,     0,   284,   358,     0,
     281,   285,     0,   281,   358,     0,   288,   357,     0,   284,
     292,     0,   282,   285,     0,   289,   357,     0,   284,   360,
       0,   283,   285,     0,   357,     0,   290,   357,     0,   284,
     285,     0,   355,     0,   357,     0,   287,     0,   288,     0,
     289,     0,   363,     0,   358,     0,   290,   358,     0,   287,
     355,     0,   287,   358,     0,   292,     0,   290,   292,     0,
     288,   355,     0,   360,     0,   290,   360,     0,   289,   355,
       0,   355,     0,   290,   355,     0,   356,     0,   291,   356,
       0,   326,     0,   336,     0,   294,     0,   297,     0,   305,
       0,   309,     0,   295,     0,    91,   294,     0,    91,   102,
     296,   103,     0,    91,   291,   102,   296,   103,     0,    91,
     291,   294,     0,   102,   294,   103,     0,   102,   296,   303,
     103,     0,   102,   294,   103,   303,     0,   360,     0,   102,
     296,   103,     0,   360,     0,   360,   303,     0,   298,     0,
     299,     0,    91,   297,     0,    91,   291,   297,     0,   102,
     298,   103,     0,   102,   298,   103,   303,     0,   301,     0,
     302,     0,   303,     0,    91,     0,    91,   291,     0,    91,
     300,     0,    91,   291,   300,     0,   102,   301,   103,     0,
     102,   302,   103,     0,   102,   303,   103,     0,   102,   301,
     103,   303,     0,   304,     0,   102,   103,     0,   102,   323,
     103,     0,   110,   111,     0,   110,   259,   111,     0,   304,
     110,   259,   111,     0,   304,   110,   111,     0,   306,     0,
     308,     0,   307,     0,    91,   305,     0,    91,   291,   305,
       0,   308,   303,     0,   102,   306,   103,     0,   102,   306,
     103,   303,     0,   359,     0,   102,   308,   103,     0,   310,
       0,    91,   309,     0,    91,   291,   309,     0,   308,   102,
     311,   103,     0,   102,   309,   103,     0,   102,   309,   103,
     303,     0,   359,     0,   311,   109,   359,     0,   359,     0,
     360,     0,   286,     0,   286,   300,     0,   290,     0,   290,
     300,     0,     0,   315,     0,   316,     0,   315,   316,     0,
     187,   102,   102,   317,   103,   103,     0,   318,     0,   317,
     109,   318,     0,     0,   319,     0,   319,   102,   258,   103,
       0,   359,     0,   360,     0,   129,     0,     0,   175,   321,
       0,   345,   322,   346,     0,   345,   322,   109,   346,     0,
     256,     0,   321,     0,   322,   109,   321,     0,   324,     0,
     324,   109,   174,     0,   174,     0,   325,     0,   324,   109,
     325,     0,   325,   175,   321,     0,   324,   109,     1,     0,
     280,     0,   280,   300,     0,   280,   305,     0,   280,   297,
       0,   284,     0,   284,   300,     0,   284,   305,     0,   286,
       0,   286,   300,     0,   286,   305,     0,   286,   297,     0,
     290,     0,   290,   300,     0,   290,   305,     0,   327,   345,
     328,   346,     0,   327,   312,   345,   328,   346,     0,   327,
     312,     0,   327,   345,   346,     0,   327,   312,   345,   346,
       0,   116,     0,   128,     0,   329,     0,   328,   329,     0,
     331,   106,     0,   330,   106,     0,   290,   333,     0,   330,
     109,   333,     0,   286,   332,     0,   331,   109,   332,     0,
     293,   334,   314,     0,   335,   314,     0,   305,   334,   314,
       0,   335,   314,     0,     0,   335,     0,   112,   259,     0,
     122,   345,   337,   339,   346,     0,   122,   312,   345,   337,
     339,   346,     0,   122,   312,     0,   312,   338,     0,   337,
     109,   312,   338,     0,     0,   175,   259,     0,     0,   109,
       0,   341,     0,   342,     0,   349,     0,   350,     0,   351,
       0,   352,     0,     1,   106,     0,   362,    68,     0,   359,
     112,   340,     0,   121,   259,   112,   340,     0,   137,   112,
     340,     0,   360,   112,   340,     0,   343,   344,     0,   343,
     347,   344,     0,   343,   348,   344,     0,   343,   347,   348,
     344,     0,   107,     0,   108,     0,   107,     0,   108,     0,
     107,   108,     0,   107,   347,   108,     0,   107,   348,   108,
       0,   107,   347,   348,   108,     0,   261,     0,   347,   261,
       0,   340,     0,   348,   340,     0,   260,   106,     0,   142,
     102,   258,   103,   340,     0,   142,   102,   258,   103,   340,
     118,   340,     0,   120,   102,   258,   103,   340,     0,   144,
     102,   258,   103,   340,     0,   141,   340,   144,   102,   258,
     103,   106,     0,   134,   102,   260,   106,   260,   106,   260,
     103,   340,     0,   134,   102,     1,   106,   260,   106,   260,
     103,   340,     0,   134,   102,   260,   106,   260,   106,     1,
     103,   340,     0,   134,   102,   260,   106,     1,   106,   260,
     103,   340,     0,   134,   102,     1,   103,   340,     0,   138,
     359,   106,     0,   133,   106,     0,   117,   106,     0,   127,
     260,   106,     0,   138,   360,   106,     0,   366,     0,   365,
     366,     0,   261,     0,   367,     0,     0,   305,   368,   364,
       0,   305,   153,     0,     0,   280,   305,   369,   364,     0,
       0,   286,   305,   370,   364,     0,     0,   284,   305,   371,
     364,     0,     0,   290,   305,   372,   364,     0,     0,   309,
     373,   364,     0,     0,   280,   309,   374,   364,     0,     0,
     286,   309,   375,   364,     0,     0,   284,   309,   376,   364,
       0,     0,   290,   309,   377,   364,     0,     0,   309,   383,
     378,   364,     0,     0,   280,   309,   383,   379,   364,     0,
       0,   286,   309,   383,   380,   364,     0,     0,   284,   309,
     383,   381,   364,     0,     0,   290,   309,   383,   382,   364,
       0,     0,   384,   347,     0,   156,     0,   157,     0,   158,
       0,   159,     0,   161,     0,   155,     0,   354,   155,     0,
     129,     0,   140,     0,   186,     0,   129,     0,   140,     0,
     124,     0,   126,     0,   143,     0,   113,     0,   123,     0,
     136,     0,   125,     0,   115,     0,   130,     0,   114,     0,
     135,     0,   132,     0,   131,     0,   119,     0,   154,     0,
     162,     0,   192,     0,   193,    73,   192,    74,     0,   194,
      73,   192,    74,     0,   195,    73,   192,    74,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   490,   492,   495,   497,   498,   500,   502,   503,   504,
     505,   511,   518,   525,   535,   544,   552,   560,   568,   576,
     586,   591,   596,   602,   608,   614,   616,   620,   627,   634,
     641,   648,   658,   665,   672,   679,   686,   688,   691,   696,
     700,   705,   709,   716,   722,   727,   731,   735,   740,   742,
     745,   751,   758,   763,   772,   779,   785,   791,   803,   810,
     818,   826,   832,   838,   847,   855,   863,   869,   881,   888,
     897,   904,   911,   917,   924,   929,   938,   944,   949,   954,
     959,   965,   976,   983,   991,  1000,  1008,  1016,  1026,  1031,
    1038,  1045,  1052,  1060,  1067,  1074,  1082,  1091,  1102,  1110,
    1120,  1126,  1130,  1134,  1139,  1145,  1149,  1153,  1157,  1161,
    1165,  1172,  1179,  1183,  1188,  1194,  1202,  1210,  1218,  1228,
    1235,  1240,  1245,  1253,  1259,  1267,  1275,  1282,  1289,  1293,
    1301,  1306,  1309,  1312,  1315,  1318,  1323,  1327,  1332,  1335,
    1338,  1341,  1344,  1347,  1350,  1353,  1356,  1359,  1362,  1365,
    1382,  1389,  1390,  1392,  1398,  1404,  1410,  1413,  1417,  1429,
    1432,  1435,  1438,  1441,  1444,  1448,  1451,  1455,  1459,  1463,
    1467,  1470,  1473,  1477,  1480,  1484,  1486,  1487,  1488,  1489,
    1490,  1493,  1495,  1499,  1501,  1504,  1507,  1511,  1513,  1516,
    1520,  1522,  1525,  1529,  1531,  1534,  1537,  1540,  1544,  1546,
    1549,  1553,  1555,  1559,  1561,  1567,  1569,  1575,  1577,  1581,
    1583,  1589,  1591,  1595,  1597,  1603,  1607,  1610,  1613,  1616,
    1619,  1622,  1625,  1628,  1631,  1634,  1638,  1641,  1655,  1659,
    1661,  1696,  1700,  1704,  1710,  1717,  1717,  1717,  1727,  1727,
    1727,  1735,  1735,  1735,  1744,  1744,  1751,  1751,  1758,  1763,
    1763,  1763,  1772,  1772,  1772,  1780,  1780,  1780,  1789,  1789,
    1796,  1796,  1803,  1807,  1813,  1816,  1822,  1827,  1832,  1836,
    1843,  1848,  1853,  1860,  1865,  1870,  1877,  1879,  1882,  1887,
    1889,  1893,  1899,  1902,  1904,  1909,  1914,  1919,  1923,  1929,
    1934,  1939,  1946,  1951,  1956,  1962,  1964,  1968,  1970,  1975,
    1980,  1986,  1988,  1989,  1990,  1999,  2002,  2005,  2008,  2011,
    2017,  2021,  2024,  2029,  2034,  2039,  2044,  2047,  2055,  2058,
    2061,  2066,  2070,  2075,  2077,  2078,  2082,  2087,  2091,  2095,
    2101,  2106,  2110,  2114,  2120,  2125,  2129,  2135,  2140,  2144,
    2149,  2161,  2163,  2167,  2170,  2173,  2178,  2182,  2185,  2190,
    2194,  2199,  2206,  2209,  2214,  2220,  2223,  2237,  2247,  2258,
    2260,  2264,  2269,  2273,  2277,  2285,  2288,  2292,  2295,  2301,
    2308,  2311,  2315,  2318,  2320,  2325,  2327,  2328,  2332,  2334,
    2338,  2340,  2341,  2345,  2350,  2357,  2360,  2365,  2374,  2379,
    2384,  2391,  2396,  2400,  2404,  2407,  2410,  2413,  2417,  2420,
    2423,  2427,  2430,  2433,  2436,  2440,  2452,  2456,  2460,  2464,
    2471,  2480,  2482,  2486,  2488,  2495,  2497,  2502,  2507,  2513,
    2518,  2525,  2531,  2538,  2544,  2551,  2553,  2557,  2562,  2566,
    2569,  2574,  2579,  2585,  2587,  2590,  2592,  2601,  2605,  2608,
    2611,  2614,  2617,  2628,  2631,  2635,  2639,  2642,  2646,  2651,
    2655,  2658,  2661,  2665,  2669,  2673,  2675,  2682,  2686,  2689,
    2692,  2697,  2701,  2709,  2714,  2723,  2728,  2732,  2735,  2739,
    2743,  2746,  2751,  2754,  2757,  2760,  2764,  2768,  2771,  2774,
    2778,  2788,  2792,  2796,  2808,  2816,  2816,  2823,  2829,  2829,
    2834,  2834,  2841,  2841,  2845,  2845,  2850,  2850,  2856,  2856,
    2863,  2863,  2870,  2870,  2878,  2878,  2885,  2885,  2892,  2892,
    2899,  2899,  2906,  2906,  2913,  2913,  2921,  2921,  2938,  2940,
    2941,  2942,  2943,  2947,  2949,  2961,  2963,  2964,  2967,  2969,
    2972,  2974,  2975,  2976,  2977,  2980,  2982,  2983,  2984,  2985,
    2987,  2988,  2990,  2991,  2994,  2998,  3009,  3017,  3024,  3033
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "tokID", "tokCONSTANT", "tokENABLE", 
  "tokDISABLE", "tokMAYPROPERTY", "tokMUSTPROPERTY", "tokGLOBAL", 
  "tokINITIALLY", "tokDIAGNOSTIC", "tokPROCEDURE", "tokACCESS", 
  "tokMODIFY", "tokANALYZE", "tokDEFAULT", "tokIF", "tokIS_EXACTLY", 
  "tokIS_ATLEAST", "tokCOULD_BE", "tokIS_ATMOST", "tokIS_BOTTOM", 
  "tokUNION_SET", "tokINTERSECT_SET", "tokUNION_EQUIV", 
  "tokINTERSECT_EQUIV", "tokIS_ELEMENT_OF", "tokIS_EMPTYSET", "tokADD", 
  "tokALWAYS", "tokEVER", "tokBEFORE", "tokAFTER", "tokTRACE", 
  "tokCONFIDENCE", "tokFORWARD", "tokBACKWARD", "tokIS_ALIASOF", 
  "tokIS_SAMEAS", "tokIS_EMPTY", "tokON_ENTRY", "tokON_EXIT", "tokNEW", 
  "tokARROW", "tokDELETE", "tokPATTERN", "tokANDAND", "tokOROR", 
  "tokNOT_EQUALS", "tokEQUALS_EQUALS", "tokLESS_EQUALS", 
  "tokGREATER_EQUALS", "tokIS_CONSTANT", "tokASSIGN", "tokWEAKASSIGN", 
  "tokEQUALS", "tokREPORT", "tokERROR", "tokAPPEND", "tokWHEN", 
  "tokREPLACE", "tokINLINE", "tokIO", "tokSTRING", "':'", "'-'", "','", 
  "';'", "'!'", "'/'", "'^'", "'~'", "'('", "')'", "'{'", "'}'", "'*'", 
  "'&'", "'%'", "'+'", "'|'", "'.'", "'$'", "'@'", "WEAK_REDUCE", 
  "LOWER_THAN_EXPRESSION", "LOWER_THAN_UNARY", "LOWER_THAN_POSTFIX", 
  "PARENTHESIZED", "ctokAND", "ctokSTAR", "ctokPLUS", "ctokMINUS", 
  "ctokTILDE", "ctokNOT", "ctokLESS", "ctokGREATER", "ctokOR", "ctokHAT", 
  "ctokPERCENT", "ctokDIVIDE", "ctokLEFTPAREN", "ctokRIGHTPAREN", 
  "ctokDOT", "ctokQUESTION", "ctokSEMICOLON", "ctokLEFTBRACE", 
  "ctokRIGHTBRACE", "ctokCOMMA", "ctokLEFTBRACKET", "ctokRIGHTBRACKET", 
  "ctokCOLON", "ctokAUTO", "ctokDOUBLE", "ctokINT", "ctokSTRUCT", 
  "ctokBREAK", "ctokELSE", "ctokLONG", "ctokSWITCH", "ctokCASE", 
  "ctokENUM", "ctokREGISTER", "ctokTYPEDEF", "ctokCHAR", "ctokEXTERN", 
  "ctokRETURN", "ctokUNION", "ctokCONST", "ctokFLOAT", "ctokSHORT", 
  "ctokUNSIGNED", "ctokCONTINUE", "ctokFOR", "ctokSIGNED", "ctokVOID", 
  "ctokDEFAULT", "ctokGOTO", "ctokSIZEOF", "ctokVOLATILE", "ctokDO", 
  "ctokIF", "ctokSTATIC", "ctokWHILE", "ctokUPLUS", "ctokUMINUS", 
  "ctokINDIR", "ctokADDRESS", "ctokPOSTINC", "ctokPOSTDEC", "ctokPREINC", 
  "ctokPREDEC", "ctokBOGUS", "IDENTIFIER", "STRINGliteral", 
  "FLOATINGconstant", "INTEGERconstant", "OCTALconstant", "HEXconstant", 
  "WIDECHARconstant", "CHARACTERconstant", "TYPEDEFname", "ctokARROW", 
  "ctokICR", "ctokDECR", "ctokLS", "ctokRS", "ctokLE", "ctokGE", "ctokEQ", 
  "ctokNE", "ctokANDAND", "ctokOROR", "ctokELLIPSIS", "ctokEQUALS", 
  "ctokMULTassign", "ctokDIVassign", "ctokMODassign", "ctokPLUSassign", 
  "ctokMINUSassign", "ctokLSassign", "ctokRSassign", "ctokANDassign", 
  "ctokERassign", "ctokORassign", "ctokINLINE", "ctokATTRIBUTE", 
  "ctokMETA_TYPE_EXPR", "ctokMETA_TYPE_STMT", "ctokMETA_EXPR", 
  "ctokMETA_STMT", "metatokID", "metatokEXPR", "metatokSTMT", 
  "metatokTYPE", "'['", "']'", "'<'", "'>'", "annotation_file", 
  "annotation", "header", "global", "property", "optional_direction", 
  "optional_default", "optional_diagnostic", "user_defined_class", 
  "members", "member", "procedure", "procedure_declaration", 
  "procedure_annotations", "procedure_annotation", "structure_annotation", 
  "pointer_rule_list", "pointer_rule", "structures", "structure", 
  "behavior_annotation", "analyze_annotation", "rules", "rule", 
  "condition", "test", "effects", "effect", "binary_property_operator", 
  "property_sensitivity", "report_annotation", "report_element_list", 
  "report_element", "action_annotation", "pattern", "identifier_list", 
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
  "left.enter.pragma", "right.exit.pragma", "declaration.list", 
  "statement.list", "expression.statement", "selection.statement", 
  "iteration.statement", "jump.statement", "constant", 
  "string.literal.list", "type.qualifier", "pointer.type.qualifier", 
  "storage.class", "basic.type.name", "identifier", "typedefname", 
  "expr_pattern", "stmt_pattern", "type_pattern", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   200,   200,   201,   201,   201,   201,   201,   201,   201,
     201,   202,   203,   203,   204,   204,   204,   204,   204,   204,
     205,   205,   205,   206,   206,   207,   207,   208,   209,   209,
     210,   210,   211,   211,   212,   212,   213,   213,   214,   214,
     214,   214,   214,   215,   215,   215,   215,   215,   216,   216,
     217,   217,   217,   217,   218,   218,   218,   218,   218,   218,
     219,   219,   219,   219,   220,   220,   220,   220,   221,   221,
     222,   222,   223,   223,   223,   223,   224,   224,   224,   224,
     224,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   226,   226,   227,   227,   227,   227,   227,
     228,   228,   228,   228,   229,   229,   229,   229,   229,   229,
     229,   230,   230,   230,   230,   231,   231,   232,   232,   232,
     232,   232,   232,   233,   233,   234,   235,   235,   236,   236,
     237,   237,   237,   237,   237,   237,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     239,   239,   239,   239,   239,   239,   239,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   241,   241,   242,
     242,   242,   242,   242,   242,   243,   243,   243,   243,   243,
     243,   244,   244,   245,   245,   245,   245,   246,   246,   246,
     247,   247,   247,   248,   248,   248,   248,   248,   249,   249,
     249,   250,   250,   251,   251,   252,   252,   253,   253,   254,
     254,   255,   255,   256,   256,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   258,   258,   259,   260,
     260,   261,   261,   261,   261,   263,   264,   262,   265,   266,
     262,   267,   268,   262,   269,   262,   270,   262,   262,   272,
     273,   271,   274,   275,   271,   276,   277,   271,   278,   271,
     279,   271,   271,   280,   280,   280,   281,   281,   281,   281,
     282,   282,   282,   283,   283,   283,   284,   284,   284,   285,
     285,   286,   286,   286,   286,   287,   287,   287,   287,   288,
     288,   288,   289,   289,   289,   290,   290,   291,   291,   292,
     292,   293,   293,   293,   293,   294,   294,   294,   294,   294,
     295,   295,   295,   296,   296,   297,   297,   297,   298,   298,
     298,   299,   299,   300,   300,   300,   301,   301,   301,   301,
     302,   302,   302,   302,   303,   303,   303,   304,   304,   304,
     304,   305,   305,   306,   306,   306,   307,   307,   307,   308,
     308,   309,   309,   309,   310,   310,   310,   311,   311,   312,
     312,   313,   313,   313,   313,   314,   314,   315,   315,   316,
     317,   317,   318,   318,   318,   319,   319,   319,   320,   320,
     321,   321,   321,   322,   322,   323,   323,   323,   324,   324,
     324,   324,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   326,   326,   326,   326,
     326,   327,   327,   328,   328,   329,   329,   330,   330,   331,
     331,   332,   332,   333,   333,   334,   334,   335,   336,   336,
     336,   337,   337,   338,   338,   339,   339,   340,   340,   340,
     340,   340,   340,   340,   340,   341,   341,   341,   341,   342,
     342,   342,   342,   343,   344,   345,   346,   364,   364,   364,
     364,   347,   347,   348,   348,   349,   350,   350,   350,   351,
     351,   351,   351,   351,   351,   351,   352,   352,   352,   352,
     352,   365,   365,   366,   366,   368,   367,   367,   369,   367,
     370,   367,   371,   367,   372,   367,   373,   367,   374,   367,
     375,   367,   376,   367,   377,   367,   378,   367,   379,   367,
     380,   367,   381,   367,   382,   367,   384,   383,   353,   353,
     353,   353,   353,   354,   354,   355,   355,   355,   356,   356,
     357,   357,   357,   357,   357,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   359,   360,   361,   361,   362,   363
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     4,     2,     7,     7,     4,     4,     4,     4,
       0,     1,     1,     0,     2,     0,     4,     3,     1,     3,
       1,     2,     4,     3,     5,     4,     1,     2,     1,     1,
       1,     1,     1,     4,     4,     4,     3,     3,     1,     2,
       7,     5,     4,     2,     1,     2,     3,     2,     3,     4,
       1,     3,     4,     4,     4,     2,     4,     2,     5,     5,
       1,     2,     7,     5,     4,     2,     1,     3,     3,     2,
       3,     5,     3,     6,     4,     3,     3,     2,     1,     2,
       3,     3,     2,     1,     2,     3,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     7,     3,     7,     1,     3,     1,     4,     2,
       2,     3,     3,     6,     6,     1,     1,     3,     1,     2,
       3,     3,     3,     3,     3,     3,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       1,     1,     1,     3,     5,     6,     1,     1,     4,     3,
       4,     3,     3,     2,     2,     3,     3,     1,     3,     1,
       2,     2,     2,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     0,
       1,     2,     2,     2,     2,     0,     0,     6,     0,     0,
       6,     0,     0,     7,     0,     5,     0,     5,     3,     0,
       0,     6,     0,     0,     6,     0,     0,     7,     0,     5,
       0,     5,     3,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     5,     3,
       3,     4,     4,     1,     3,     1,     2,     1,     1,     2,
       3,     3,     4,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     4,     1,     2,     3,     2,     3,     4,
       3,     1,     1,     1,     2,     3,     2,     3,     4,     1,
       3,     1,     2,     3,     4,     3,     4,     1,     3,     1,
       1,     1,     2,     1,     2,     0,     1,     1,     2,     6,
       1,     3,     0,     1,     4,     1,     1,     1,     0,     2,
       3,     4,     1,     1,     3,     1,     3,     1,     1,     3,
       3,     3,     1,     2,     2,     2,     1,     2,     2,     1,
       2,     2,     2,     1,     2,     2,     4,     5,     2,     3,
       4,     1,     1,     1,     2,     2,     2,     2,     3,     2,
       3,     3,     2,     3,     2,     0,     1,     2,     5,     6,
       2,     2,     4,     0,     2,     0,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     4,     3,     3,     2,
       3,     3,     4,     1,     1,     1,     1,     2,     3,     3,
       4,     1,     2,     1,     2,     2,     5,     7,     5,     5,
       7,     9,     9,     9,     9,     5,     3,     2,     2,     3,
       3,     1,     2,     1,     1,     0,     3,     2,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     3,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,   125,   533,
     539,   537,   411,   543,     0,   534,   530,   536,   531,   412,
     525,   538,   542,   541,   540,   535,   526,   532,   545,   527,
       0,     0,     1,     9,     3,     4,     7,     0,    10,     8,
     461,     0,     0,     0,   263,   264,   265,     0,     0,   281,
     282,   283,     0,   289,   299,     0,   300,    11,   295,   276,
     285,   292,   284,     5,     6,     0,     0,   128,     0,     0,
      13,    60,     0,     0,   455,   544,   430,     0,   359,   360,
       0,     2,     0,   231,     0,   232,     0,   244,     0,     0,
     235,   301,   305,   302,   317,   318,   303,   341,   343,   342,
     304,   351,   349,   315,   268,   279,   280,   269,   233,   272,
     275,   258,     0,     0,   278,   271,   249,   342,   267,   274,
     246,   238,   287,   266,   288,   234,   291,   270,   294,   273,
     260,   290,   252,   296,   277,   286,   293,   408,     0,   462,
      20,    20,     0,     0,   129,     0,     0,    54,     0,     0,
       0,   433,   435,     0,     0,     0,     0,     0,     0,     0,
       0,    33,     0,    36,    38,    39,    40,    41,    42,   248,
     241,   262,   255,   365,     0,   528,   529,     0,   306,   319,
     344,   352,   297,     0,     0,     0,     0,     0,     0,     0,
     313,   365,     0,     0,   346,   334,     0,   316,   365,     0,
       0,   365,   365,   365,   365,   365,     0,   456,     0,   281,
     282,   283,     0,     0,   413,     0,     0,   409,    16,    18,
      21,    22,     0,    17,    19,     0,     0,    61,     0,   128,
      57,     0,     0,    12,    55,    35,     0,   126,     0,     0,
       0,     0,     0,    70,     0,    93,   435,     0,   431,   436,
       0,   549,    65,     0,    67,     0,     0,     0,   137,   136,
       0,   117,     0,     0,     0,     0,     0,     0,     0,     0,
     115,   110,     0,     0,     0,    32,    37,   365,   365,     0,
     378,   366,   367,     0,     0,   309,   320,   345,   353,   298,
       0,   310,     0,   321,   347,   350,   355,   236,   335,   387,
     392,   264,   396,   399,   282,   403,     0,     0,   385,   388,
     357,   175,   176,   177,   178,   179,   180,     0,   337,     0,
     523,   518,   519,   520,   521,   522,     0,     0,   546,     0,
     157,   169,   181,     0,   183,   187,   190,   193,   198,   201,
     203,   205,   207,   209,   211,   228,     0,   151,   152,   150,
     156,     0,   378,   250,   378,   239,   378,   253,     0,   410,
       0,   425,   419,   365,   425,   417,   365,   414,   406,   416,
       0,   415,     0,     0,    25,    25,    62,    63,    58,     0,
      56,     0,    34,     0,     0,     0,     0,     0,    75,     0,
      98,    68,    71,    69,    94,     0,   434,   433,   428,     0,
       0,    46,     0,     0,     0,    47,     0,    48,     0,     0,
       0,   137,   147,   149,     0,   148,     0,   120,     0,     0,
     111,   106,   107,   104,   105,   108,   109,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,   113,   137,     0,
       0,     0,    76,    88,     0,   242,   256,     0,     0,   245,
     368,   307,     0,   314,   312,   311,   322,   348,   356,   378,
     326,     0,   395,   393,   323,   324,   325,   394,   326,     0,
     397,   398,   402,   400,   401,   404,   405,   354,     0,   336,
       0,     0,   453,   181,   213,   226,     0,   361,   363,     0,
       0,     0,   173,     0,   170,   171,     0,     0,     0,     0,
       0,   163,   164,   172,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   524,   340,     0,   259,   378,   247,
     378,   261,   378,   407,   427,   365,   426,   422,   365,   424,
     418,   420,    30,     0,    28,     0,    23,    23,    59,   127,
      99,    96,    97,    95,     0,     0,   429,   432,    64,    66,
      43,     0,    53,     0,    45,    49,    44,   110,     0,   138,
     122,   121,   116,   140,   145,   142,   144,   143,   146,   139,
     141,     0,     0,     0,    87,     0,     0,    92,    89,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   378,   378,   372,   382,   379,     0,   308,
     237,   327,   328,     0,     0,     0,   327,   358,   391,   386,
     389,   390,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,     0,   153,     0,   326,     0,   362,   364,
       0,     0,     0,     0,     0,   229,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,   463,   437,   438,     0,
       0,     0,   439,   440,   441,   442,   150,   292,     0,     0,
       0,   159,     0,   167,   161,   165,     0,   162,   166,   184,
     186,   185,   188,   189,   191,   192,   194,   195,   196,   197,
     199,   200,   202,   204,   206,   208,     0,   210,   339,   251,
     240,   254,   421,   423,    31,     0,    27,     0,     0,    14,
      15,    74,     0,     0,     0,   118,     0,     0,    85,    86,
      90,    91,   110,   100,   101,   102,   103,    82,     0,    80,
      78,    77,     0,     0,   131,   130,   133,   135,   132,   134,
     243,   257,   377,     0,   370,   373,   375,   376,   383,     0,
     329,   330,   331,   332,   214,   227,   327,   182,   443,   478,
       0,     0,     0,   477,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   454,   449,     0,     0,     0,   464,
       0,     0,     0,   444,   174,   547,   160,     0,   158,     0,
      29,     0,    24,     0,    73,    52,     0,     0,     0,     0,
      84,   123,   124,     0,   372,     0,     0,   380,   333,     0,
       0,   479,     0,     0,   447,   476,   480,     0,     0,     0,
       0,   450,     0,   451,     0,   154,   445,   448,   168,   212,
      26,     0,     0,    51,   112,   114,    81,     0,   369,   371,
       0,   384,   381,     0,   446,     0,   229,     0,     0,     0,
       0,   548,   452,   155,    72,     0,    83,   374,   468,   475,
       0,     0,     0,     0,   466,   469,    50,   229,   229,     0,
       0,     0,     0,     0,     0,     0,   470,   467,     0,     0,
       0,     0,   472,   474,   473,   471,     0,     0
};

static const short yydefgoto[] =
{
      31,    32,    33,    34,    35,   222,   709,   546,   374,   543,
     544,    36,    37,   162,   163,   164,   406,   407,   146,   147,
     165,    38,   242,   243,   441,   442,   244,   245,   728,   435,
     167,   269,   270,   168,    39,   236,    71,   443,   444,   330,
     331,   672,   483,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   484,   485,   633,   654,   346,
     655,    40,    41,   191,   459,   203,   530,   277,   603,   173,
     202,    42,   201,   528,   205,   532,   278,   604,   198,   204,
      43,    44,    45,    46,    47,   109,    48,    49,    50,    51,
      52,   177,    53,   361,    91,    92,   185,    93,    94,    95,
     612,   464,   465,   466,   195,    96,    97,    98,   117,   100,
     101,   306,   151,   489,   280,   281,   282,   743,   744,   745,
     449,   607,   749,   307,   308,   309,    54,    55,   213,   214,
     215,   216,   362,   365,   535,   363,    56,   152,   248,   250,
     656,   657,   658,   659,   775,   608,   217,    57,   661,   662,
     663,   664,   665,   347,   348,    58,   182,    59,    60,   349,
     769,   350,   668,    62
};

static const short yypact[] =
{
    1556,    65,    79,    93,   130,   143,   158,   180,-32768,-32768,
  -32768,-32768,-32768,-32768,   333,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     -43,   926,-32768,-32768,-32768,-32768,-32768,   120,-32768,-32768,
  -32768,   207,   301,   220,  2589,   587,   676,  1609,   358,  2589,
     648,   676,  1662,-32768,-32768,   333,-32768,  2555,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   148,   152,   193,   224,    49,
  -32768,-32768,   159,   239,-32768,-32768,   248,    27,-32768,-32768,
     168,-32768,  1303,-32768,   360,-32768,   389,-32768,   458,   155,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   399,
  -32768,-32768,-32768,   541,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   800,   210,-32768,-32768,-32768,   541,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   248,  2514,-32768,
    1077,  1250,   496,    49,-32768,   188,   462,-32768,   197,   685,
      27,   211,   299,   371,    73,    87,   381,   394,   731,   896,
     406,-32768,  1400,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   334,   155,-32768,-32768,   821,-32768,-32768,
  -32768,-32768,-32768,   155,   403,   541,   425,   447,   720,   472,
  -32768,   334,  2425,   854,-32768,   451,  2478,-32768,   334,   800,
     751,   334,   334,   334,   334,   334,  2514,-32768,   478,  1490,
     348,   348,  2348,  2514,-32768,   483,   674,-32768,-32768,-32768,
  -32768,-32768,   524,-32768,-32768,   524,   198,-32768,   829,-32768,
  -32768,   188,   181,-32768,-32768,-32768,    -3,-32768,   631,   200,
     565,   653,   311,-32768,    50,-32768,   299,  2194,-32768,    27,
     505,-32768,-32768,   188,-32768,   188,    32,   507,   603,-32768,
     602,-32768,   741,   741,   741,   741,   704,   714,   719,   367,
  -32768,  1563,   639,   368,   513,-32768,-32768,   334,   334,   622,
     554,   334,-32768,   864,   155,-32768,-32768,-32768,-32768,-32768,
     888,   541,   644,   541,   541,-32768,   541,-32768,-32768,-32768,
     340,   676,  2312,   340,   676,  2312,   319,   649,   656,   595,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1727,-32768,  2207,
  -32768,-32768,-32768,-32768,-32768,-32768,  2237,  2237,-32768,   702,
  -32768,   568,-32768,  2194,-32768,   294,   189,   488,   -11,   519,
     691,   739,   735,   634,   -22,-32768,   747,-32768,   695,-32768,
  -32768,  1986,   554,-32768,   554,-32768,   554,-32768,  2514,-32768,
    2194,   756,-32768,   334,   756,-32768,   334,-32768,-32768,-32768,
     453,-32768,   478,   857,   861,   861,-32768,-32768,-32768,   188,
  -32768,   188,-32768,   881,   885,   890,   741,   256,-32768,   513,
  -32768,-32768,-32768,-32768,-32768,   505,-32768,   211,-32768,   469,
     690,-32768,   918,   179,   842,-32768,   361,-32768,  1014,   921,
     513,-32768,   434,-32768,  1454,   434,   871,-32768,   758,   775,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   741,   741,   741,
     741,   741,   741,   741,   741,-32768,   513,-32768,  2724,   513,
     513,   547,-32768,-32768,  1186,-32768,-32768,   862,  2069,-32768,
  -32768,-32768,   893,-32768,-32768,-32768,-32768,-32768,-32768,   554,
    1167,  1802,-32768,-32768,-32768,-32768,-32768,-32768,   785,  1855,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   805,-32768,
    1169,  2069,-32768,  1693,-32768,-32768,   380,   707,  2390,   867,
    1086,  1727,-32768,  2090,-32768,-32768,   781,  2111,    27,  2194,
      27,-32768,-32768,-32768,  2194,  2194,  2194,  2194,  2194,  2194,
    2194,  2194,  2194,  2194,  2194,  2194,  2194,  2194,  2194,  2194,
    2194,  2194,  2194,-32768,-32768,-32768,   873,-32768,   554,-32768,
     554,-32768,   554,-32768,-32768,   334,-32768,-32768,   334,-32768,
  -32768,-32768,   524,   709,-32768,   903,   982,   982,-32768,-32768,
  -32768,-32768,-32768,  1429,    55,   672,-32768,-32768,-32768,-32768,
  -32768,    49,-32768,   513,-32768,-32768,-32768,  1002,   708,-32768,
  -32768,-32768,-32768,   434,-32768,  1006,-32768,  1105,-32768,   434,
    1627,   712,   994,  1011,-32768,  1019,  1020,-32768,-32768,  1035,
    1132,-32768,   797,   950,   513,   513,   635,   741,   741,   741,
     741,   741,   741,   554,   554,   487,-32768,-32768,  2069,-32768,
  -32768,  1167,-32768,   902,   957,   960,   785,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  2194,-32768,  2194,   540,  1908,-32768,-32768,
    2194,   947,   958,   963,  2194,  2194,   961,   966,   962,    27,
    1310,   968,   971,  1005,   973,   969,-32768,-32768,-32768,  1003,
    1086,  1232,-32768,-32768,-32768,-32768,   977,   979,  1012,   996,
    1027,-32768,   508,-32768,-32768,-32768,   -42,-32768,-32768,-32768,
  -32768,-32768,   294,   294,   189,   189,   488,   488,   488,   488,
     -11,   -11,   519,   691,   739,   735,   737,   634,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   857,-32768,   188,  1101,-32768,
  -32768,-32768,   456,  1265,   830,-32768,   775,   775,-32768,-32768,
  -32768,-32768,  1002,-32768,-32768,-32768,-32768,-32768,  1103,-32768,
  -32768,-32768,  1310,  1039,  1429,  1429,  1429,  1429,  1429,  1429,
  -32768,-32768,-32768,   530,-32768,  1013,-32768,-32768,-32768,   828,
  -32768,   541,-32768,-32768,-32768,-32768,   540,-32768,-32768,-32768,
    2194,  1043,  1050,-32768,  1391,  1310,  1057,  1060,   964,   979,
    2194,  2194,   980,-32768,-32768,-32768,  1003,  1232,  1232,-32768,
    1066,  1310,  1310,-32768,-32768,-32768,-32768,  2194,-32768,  2194,
  -32768,   734,-32768,   256,-32768,-32768,   235,   769,   784,  1388,
  -32768,-32768,-32768,  1070,   487,  2194,  1965,-32768,-32768,   570,
    1310,-32768,   795,  1068,-32768,-32768,-32768,  1081,   760,   772,
    1112,-32768,  1232,-32768,  1084,-32768,-32768,-32768,-32768,-32768,
  -32768,   216,    49,-32768,-32768,-32768,-32768,  1187,-32768,-32768,
     794,-32768,-32768,  1310,-32768,  1310,  2194,  1421,  2194,  1310,
    1310,-32768,-32768,-32768,-32768,  1270,-32768,-32768,-32768,-32768,
    1085,  1088,  1098,   819,  1113,-32768,-32768,  2194,  2194,  1497,
    1126,  1310,  1131,  1136,  1143,  1146,-32768,-32768,  1310,  1310,
    1310,  1310,-32768,-32768,-32768,-32768,  1253,-32768
};

static const short yypgoto[] =
{
  -32768,  1223,-32768,-32768,-32768,  1114,   723,   900,  -213,-32768,
     566,-32768,-32768,-32768,  1127,-32768,-32768,   884,  -140,    -4,
  -32768,   -35,-32768,  1061,  -269,-32768,  -378,  -231,   503,  -423,
  -32768,  -149,   887,-32768,-32768,  -235,  -131,-32768,   309,-32768,
  -32768,-32768,  -174,-32768,  -240,   435,   497,   748,   528,   791,
     796,   801,   793,   808,-32768,  -186,  -130,-32768,  -252,  -221,
    -163,   -55,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    -104,-32768,   -89,-32768,   -28,    -2,    71,   -34,   157,    67,
     183,  -106,   -15,    63,    13,-32768,   -96,   -48,   -64,-32768,
    -168,  -445,  -438,   -60,-32768,     3,    -8,-32768,   610,   119,
  -32768,-32768,     7,   844,   215,-32768,  1038,-32768,   532,-32768,
      78,  -459,-32768,-32768,-32768,   868,-32768,-32768,  1141,  -185,
  -32768,-32768,   970,   981,   986,    41,-32768,  1204,   959,  1111,
     108,-32768,-32768,  -281,  -280,   338,  -202,  -434,  -600,-32768,
  -32768,-32768,-32768,-32768,-32768,   615,  -172,   578,     2,   -14,
      66,-32768,-32768,-32768
};


#define	YYLAST		2789


static const short yytable[] =
{
      78,    70,   139,   228,   359,   289,   199,   345,   388,   554,
     273,   368,   375,   394,   230,   590,   613,   237,   399,   332,
     400,    76,   621,   614,   613,   186,   396,   289,   367,   102,
      80,   614,   115,   102,   102,    67,   490,   131,   102,   194,
     179,    78,   104,   197,   110,   114,   107,   166,   398,   118,
     116,   124,    67,   238,   135,   132,   660,   194,   238,   777,
     778,   345,   137,    78,   381,   486,    61,   635,    63,   788,
     102,   382,   102,   332,   102,   102,   252,   145,   283,   241,
      79,   187,    64,   521,   241,   511,   512,   290,   300,   172,
     254,   180,   300,   503,   145,    68,    65,    61,   102,   102,
     378,   178,   184,   301,   209,   187,    90,   301,   401,   103,
     186,   121,    68,   119,   103,   180,   402,   408,   136,   186,
     555,    79,   237,    61,   237,   292,   393,   166,   194,   286,
     526,   711,   463,    66,   470,   473,    78,   475,   227,   534,
     194,   568,   234,    79,   715,   492,    67,   170,   253,   748,
     103,   522,   494,   495,   103,   190,   533,   513,   514,   332,
     102,    72,   255,   102,   302,   345,   187,   581,   302,   102,
     591,   592,   209,   367,   345,   187,   822,   332,   310,   209,
     287,    75,    67,    73,    67,   102,   332,   184,   452,    28,
     285,   229,   613,   556,   102,    82,   184,   131,   102,   614,
     229,    67,   287,   238,    61,   211,    68,   181,   189,   208,
     490,   124,   490,   140,   135,   364,    79,   141,    69,   238,
     186,    87,   376,   292,   234,   776,   379,   144,   380,   241,
     292,   454,   148,   456,   457,    78,   458,   142,    67,   486,
     190,   486,    68,   103,    68,   241,    88,   676,   548,   190,
     549,    68,   462,   366,   561,   472,   397,   183,    61,   238,
      68,    68,    61,   303,   679,   680,   681,   303,   143,   696,
     102,   235,    61,   211,   103,   387,   187,   208,   136,    61,
     211,   507,   508,   209,   208,   241,   102,   115,   102,   102,
     131,   102,   854,   189,   714,   210,   288,   184,    68,   799,
     114,   112,   189,   467,   118,   471,   474,   135,   476,    75,
     832,    88,   113,    83,   149,    79,    84,    28,   606,   638,
     639,   212,    89,   394,   209,   730,   731,   239,   240,   704,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   841,   332,   304,
     190,   606,    77,   304,   611,    74,   102,   300,   102,   120,
     153,   169,   616,   210,    75,   300,   103,   673,   119,   103,
     210,   136,   301,   364,    75,   305,   300,   403,   404,   305,
     301,   780,    28,    61,   211,   504,   247,   391,   487,   212,
     171,   301,   292,   138,   505,   506,   212,   186,   234,   562,
     757,   615,   536,   189,   234,   536,   297,    85,   249,   615,
      86,   366,   179,   352,   150,   831,   353,   354,   355,   356,
     357,   713,   477,   761,    61,   211,   419,   419,   478,   208,
     527,   460,   529,   302,   531,   420,   437,   564,   103,   289,
      74,   302,   461,   750,   289,   251,   102,   102,   750,    88,
     193,    88,   302,   187,   102,   102,   256,   209,   345,   238,
      89,   187,    89,   180,   617,    67,   332,   271,   271,   257,
     332,   180,   791,   131,   210,   206,   666,    20,   606,   274,
     112,   794,   762,   634,   674,   241,   677,    75,    26,   635,
     135,   113,   445,   446,    75,    28,   821,   823,   824,    67,
     488,   192,    28,   754,   428,   755,   291,   231,   809,   193,
      67,   430,    75,   432,    75,   210,   438,   259,   818,   819,
      28,   279,    28,   403,   404,    68,   103,    61,   293,   232,
     756,   793,   303,   300,    29,    61,   381,   610,   233,   226,
     303,   212,   852,    75,   112,   558,    61,   807,   301,    88,
     294,   303,   145,   840,   136,   113,   667,    61,   211,    68,
     174,   351,   487,   286,   675,   360,   678,   797,   798,    88,
      68,   412,   413,   414,   415,   296,   237,   615,   537,   262,
      89,   539,   439,   405,   289,   263,   440,   175,   750,   369,
     360,   746,   370,   265,   594,   595,   863,   102,   176,   373,
     394,   813,   102,   829,   842,   139,   699,    75,   700,   302,
     701,   786,    75,   207,   287,   332,   742,   787,   304,   287,
      28,   596,   106,   106,   106,   106,   304,   123,   127,   129,
     134,   636,    75,   803,   383,   766,   666,   304,   389,   804,
      28,    75,   637,   196,   305,   666,   666,   666,   210,    28,
     193,   193,   305,    99,   509,   510,   390,   828,    99,   105,
     105,   105,   105,   305,   122,   126,   128,   133,   409,   175,
     497,   747,   498,   843,   488,   410,   606,   103,   499,   635,
     176,   740,   741,   860,   862,   384,   385,   386,   238,   515,
     516,   808,   855,   108,    99,   553,   732,   733,    99,   188,
       9,   239,   240,    61,   872,   873,   875,   416,   303,   234,
      15,    16,   436,    18,   241,   767,    20,   417,   666,   594,
     595,   139,   418,   200,   447,   667,   667,    26,   271,   448,
      27,   500,   501,   502,   258,   259,   573,   574,   575,   576,
     577,   578,   579,   580,   411,   259,   712,   455,   260,   593,
     702,   666,   479,   703,   125,   594,   595,   381,   768,   594,
     595,     9,   666,   666,   666,   480,   559,   666,   666,   779,
     481,    15,    16,    29,    18,   496,   705,    20,   258,   259,
     371,   517,   716,   372,   188,   706,   717,    99,    26,     9,
     746,    27,   833,   188,   304,   261,   666,   262,   636,    15,
      16,   381,    18,   263,   264,    20,   520,   262,   666,   637,
     830,   265,   266,   263,   264,   267,    26,   193,    99,    27,
     305,   265,   192,   295,   122,   126,   128,   133,   419,   666,
     193,   666,    67,   519,    29,   666,   666,   834,   518,   261,
     801,   262,   667,   419,   594,   595,   635,   263,   264,   789,
     524,   234,   835,   196,   295,   265,   266,   666,   523,   267,
     542,   193,    29,   849,   666,   666,   666,   666,   360,   635,
     747,   729,   545,   814,   231,   850,   468,   594,   595,   106,
     106,   635,   127,   134,   550,   779,   779,   469,   551,   826,
     827,   112,    68,   552,   188,   193,   232,   857,   845,   258,
     259,   846,   113,   635,   796,   377,   734,   735,   736,   737,
     738,   739,    88,   272,   175,   563,   105,   105,   844,   126,
     133,    67,   870,   284,   567,   176,   886,   268,   635,   175,
     779,     1,     2,     3,     4,     5,   207,   806,     6,    75,
     176,     7,   682,   683,   311,   312,   313,   314,   315,   316,
     175,   858,   570,   859,    75,   571,   317,   864,   865,    75,
     261,   176,   262,   231,   605,   318,   196,   451,   263,   264,
     640,   268,     8,   670,   193,    75,   265,   266,   707,   877,
     267,    68,    99,    28,   698,   232,   882,   883,   884,   885,
     196,   453,   708,   319,   560,   196,   609,   718,   193,   597,
     598,   599,   600,   193,   641,   751,   684,   685,    75,   320,
     321,   322,   323,   324,   719,   325,   427,    67,   326,   327,
     428,   429,   720,   721,   569,   271,   271,   430,   431,   432,
     433,   434,   421,   422,   423,   424,   425,   426,   722,     9,
      10,    11,    12,   690,   691,    13,   328,   329,    14,    15,
      16,    17,    18,   758,    19,    20,    21,    22,    23,   231,
     752,    24,    25,   753,   759,   760,    26,   763,   764,    27,
     770,   200,   427,   771,   765,   773,   428,    68,   772,   200,
     783,   232,   635,   430,   431,   432,   433,   641,    28,   781,
     566,   782,   268,   311,   312,   313,   314,   315,   316,   784,
     218,   785,   219,   133,   792,   317,   800,   802,   817,  -229,
     482,   774,    29,   220,   221,   805,     9,    10,    11,    12,
     642,    30,    13,   643,   644,    14,    15,    16,    17,    18,
     645,    19,    20,    21,    22,    23,   646,   647,    24,    25,
     648,   649,   319,    26,   650,   651,    27,   652,   601,   602,
     723,   724,   725,   726,   727,   810,   811,    75,   320,   321,
     322,   323,   324,   815,   325,    28,   816,   326,   327,   825,
     618,   427,   820,   838,   847,   428,   311,   312,   313,   314,
     315,   316,   430,   848,   432,   433,   851,   853,   317,    29,
     856,   867,  -229,   482,   868,   328,   329,   653,    30,     9,
      10,    11,    12,   642,   869,    13,   643,   644,    14,    15,
      16,    17,    18,   645,    19,    20,    21,    22,    23,   646,
     647,    24,    25,   648,   649,   319,    26,   650,   651,    27,
     652,   871,   876,   641,   878,   597,   598,   599,   600,   879,
      75,   320,   321,   322,   323,   324,   880,   325,    28,   881,
     326,   327,   427,   887,    81,   225,   428,   429,   460,   686,
     687,   688,   689,   430,   431,   432,   433,   434,    67,   461,
     710,   790,    29,    67,   223,   547,   224,   193,   328,   329,
     653,    30,     9,    10,    11,    12,   220,   221,    13,   276,
     565,    14,    15,    16,    17,    18,   175,    19,    20,    21,
      22,    23,   837,   392,    24,    25,   572,   176,   692,    26,
     231,   641,    27,   695,   693,   231,   154,   155,     7,   450,
     694,    75,   311,   312,   313,   314,   315,   316,    68,    28,
     697,    28,   232,    68,   317,   669,   839,   232,  -229,   482,
     774,   795,   541,   619,   156,   157,   866,   358,   620,   642,
     538,   540,   643,   644,   246,    29,   557,   395,     0,   645,
     158,   159,     0,   160,    30,   646,   647,     0,     0,   648,
     649,   319,     0,   650,   651,     0,   652,     0,     0,   161,
       0,     0,     0,     0,   601,   602,    75,   320,   321,   322,
     323,   324,   812,   325,    28,     0,   326,   327,     0,     0,
     311,   312,   313,   314,   315,   316,   723,   724,   725,   726,
     836,     0,   317,   154,   155,     7,  -229,   482,     0,     0,
       0,     0,   861,     0,   328,   329,   653,   642,     0,     0,
     643,   644,     0,     0,     0,     0,     0,   645,     0,     0,
       0,   156,   157,   646,   647,     0,     0,   648,   649,   319,
       0,   650,   651,     0,   652,     0,     0,   158,   159,     0,
     160,     0,     0,     0,    75,   320,   321,   322,   323,   324,
       0,   325,    28,     0,   326,   327,   275,     0,     0,     0,
       0,   311,   312,   313,   314,   315,   316,     0,     0,     0,
       0,     0,     0,   317,     0,   427,     0,  -229,   874,   428,
     429,     0,   328,   329,   653,     0,   430,   431,   432,   433,
     434,   311,   312,   313,   314,   315,   316,     0,     0,     0,
     427,     0,     0,   317,   428,   429,     0,  -229,   569,     0,
     319,   430,   431,   432,   433,   434,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,   320,   321,   322,   323,
     324,     0,   325,     0,     0,   326,   327,     0,     0,     0,
     319,     1,     2,     3,     4,     5,     0,     0,     6,     0,
       0,     7,     0,     0,     0,    75,   320,   321,   322,   323,
     324,     0,   325,   328,   329,   326,   327,   311,   312,   313,
     314,   315,   316,   421,   422,   423,   424,   425,   426,   317,
    -229,     0,     8,     0,    10,    11,     0,     0,     0,    13,
     111,     0,     0,   328,   329,    17,     0,     0,     0,    20,
      21,    22,    23,     0,     0,    24,    25,     0,     0,   427,
      26,     0,     0,   428,   429,     0,   319,     0,     0,     0,
     430,   431,   432,   433,   434,     0,     0,     0,     0,     0,
       0,    75,   320,   321,   322,   323,   324,     0,   325,     0,
       0,   326,   327,   130,     0,     0,     0,     0,     0,     9,
      10,    11,    12,     0,     0,    13,    29,     0,    14,    15,
      16,    17,    18,     0,    19,    20,    21,    22,    23,   328,
     329,    24,    25,   427,     0,     0,    26,   428,   429,    27,
     112,     0,     0,     0,   430,   431,   432,   433,     0,     0,
       0,   113,     0,     0,     0,     0,     0,     0,    28,     0,
       0,     0,     9,    10,    11,    12,     0,     0,    13,     0,
       0,    14,    15,    16,    17,    18,     0,    19,    20,    21,
      22,    23,    29,     0,    24,    25,     0,     0,     0,    26,
       0,    30,    27,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,   113,     0,     0,     0,     0,     0,
       0,    28,     0,     0,     0,     9,    10,    11,    12,     0,
       0,    13,     0,     0,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,    29,     0,    24,    25,     0,
       0,     0,    26,     0,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   311,   312,   313,
     314,   315,   316,     0,    28,     0,     0,     0,     0,   317,
       0,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,    10,    11,    12,     0,     0,    13,     0,    29,    14,
       0,     0,    17,     0,     0,    19,    20,    21,    22,    23,
       0,     0,    24,    25,     0,     0,   319,    26,   622,   623,
     624,   625,   626,   627,   628,   629,   630,   631,   632,     0,
       0,    75,   320,   321,   322,   323,   324,     0,   325,    28,
       0,   326,   327,   460,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   298,     0,     0,     0,     0,
       0,     0,   193,    29,     0,     9,    10,    11,    12,   328,
     329,    13,    30,     0,    14,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,     0,     0,    24,    25,     0,
       0,     0,    26,     0,     0,    27,   468,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,   469,   298,     0,
       0,     0,     0,     0,    28,   193,     0,     0,     9,    10,
      11,    12,     0,     0,    13,     0,   299,    14,    15,    16,
      17,    18,     0,    19,    20,    21,    22,    23,    29,     0,
      24,    25,     0,     0,     0,    26,     0,    30,    27,   636,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
     637,   298,     0,     0,     0,     0,     0,    28,   193,     0,
       0,     9,    10,    11,    12,     0,     0,    13,     0,   299,
      14,    15,    16,    17,    18,     0,    19,    20,    21,    22,
      23,    29,     0,    24,    25,     0,     0,     0,    26,     0,
      30,    27,     0,     0,     0,   311,   312,   313,   314,   315,
     316,     0,     0,     0,     0,     0,     0,   317,     0,     0,
      28,     0,    74,   207,     0,     0,   311,   312,   313,   314,
     315,   316,   299,     0,     0,     0,     0,     0,   317,     0,
       0,     0,     0,     0,    29,     0,     0,   525,     0,     0,
       0,     0,     0,    30,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
     320,   321,   322,   323,   324,   319,   325,     0,     0,   326,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,   320,   321,   322,   323,   324,     0,   325,     0,     0,
     326,   327,     0,     0,     0,     0,     0,   328,   329,   311,
     312,   313,   314,   315,   316,     0,     0,     0,     0,     0,
       0,   317,     0,     0,     0,     0,    74,     0,   328,   329,
     311,   312,   313,   314,   315,   316,     0,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,   482,     0,     0,
       0,   311,   312,   313,   314,   315,   316,     0,   319,     0,
       0,     0,     0,   317,   671,     0,     0,     0,     0,     0,
       0,     0,     0,    75,   320,   321,   322,   323,   324,   319,
     325,     0,     0,   326,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,   320,   321,   322,   323,   324,
     319,   325,     0,     0,   326,   327,     0,     0,     0,     0,
       0,   328,   329,     0,     0,    75,   320,   321,   322,   323,
     324,     0,   325,     0,     0,   326,   327,     0,     0,     0,
       0,     0,   328,   329,   311,   312,   313,   314,   315,   316,
       0,     0,     0,     0,     0,     0,   317,   311,   312,   313,
     314,   315,   316,   328,   329,     0,     0,     0,     0,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,   312,   313,
     314,   315,   316,   319,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,   319,     0,    75,   320,
     321,   322,   323,   324,     0,   325,     0,     0,   326,   327,
       0,    75,   320,   321,   322,   323,   324,     0,   325,     0,
       0,   326,   327,     0,     0,     0,   319,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   328,   329,     0,     0,
       0,    75,   320,   321,   322,   323,   324,     0,   325,   328,
     329,   326,   327,   468,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   469,     0,     0,     0,     0,     0,
       0,     0,   193,     0,     0,     9,    10,    11,    12,   328,
     329,    13,     0,     0,    14,    15,    16,    17,    18,   112,
      19,    20,    21,    22,    23,     0,     0,    24,    25,     0,
     113,     0,    26,     0,     0,    27,     0,     0,     0,     0,
     360,     0,    10,    11,    12,     0,    75,    13,     0,     0,
      14,     0,     0,    17,    28,     0,    19,    20,    21,    22,
      23,   636,     0,    24,    25,     0,     0,     0,    26,     0,
       0,     0,   637,     0,     0,     0,     0,     0,    29,     0,
     193,     0,    75,     0,    10,    11,    12,     0,     0,    13,
      28,     0,    14,     0,     0,    17,     0,     0,    19,    20,
      21,    22,    23,     0,     0,    24,    25,     0,   298,     0,
      26,     0,     0,     0,    29,     0,     0,     0,     9,    10,
      11,    12,     0,     0,    13,     0,     0,    14,    15,    16,
      17,    18,    28,    19,    20,    21,    22,    23,     0,     0,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
       0,     0,     0,     0,     0,     0,    29,     0,     0,    75,
       0,   298,     0,     0,     0,     0,     0,    28,     0,     0,
       0,     9,    10,    11,    12,     0,     0,    13,     0,   299,
      14,    15,    16,    17,    18,     0,    19,    20,    21,    22,
      23,    29,     0,    24,    25,     0,     0,     0,    26,     0,
      30,    27,   207,     0,     0,     0,     0,     0,    10,    11,
      12,     0,     0,    13,     0,     0,    14,     0,     0,    17,
      28,     0,    19,    20,    21,    22,    23,     0,     0,    24,
      25,     0,   299,     0,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     9,    10,
      11,    12,     0,    30,    13,     0,    28,    14,    15,    16,
      17,    18,     0,    19,    20,    21,    22,    23,     0,     0,
      24,    25,     0,     0,     0,    26,     0,     0,    27,     0,
      29,     0,     9,    10,    11,     0,     0,     0,    13,    30,
       0,     0,    15,    16,    17,    18,     0,    28,    20,    21,
      22,    23,     0,     0,    24,    25,     0,   582,     0,    26,
       0,     0,    27,     0,     0,     0,     0,     0,     0,     0,
       0,    29,  -110,  -110,  -110,  -110,  -110,     0,     0,     0,
      30,   583,   584,     0,   421,   422,   423,   424,   425,   426,
       0,     0,   585,   586,   587,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   588,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   589
};

static const short yycheck[] =
{
      14,     5,    57,   143,   206,   177,   112,   193,   239,   387,
     159,   213,   225,   244,   145,   438,   461,   148,   253,   193,
     255,    14,   481,   461,   469,    89,   247,   199,   213,    43,
      73,   469,    47,    47,    48,     3,   317,    52,    52,    99,
      88,    55,    44,   103,    46,    47,    44,    82,   250,    47,
      47,    49,     3,     3,    52,    52,   490,   117,     3,   659,
     660,   247,    55,    77,    67,   317,     0,   109,     3,   111,
      84,    74,    86,   247,    88,    89,     3,    45,   174,    29,
      14,    89,     3,   105,    29,    96,    97,   183,   192,    86,
       3,    88,   196,   333,    45,    63,     3,    31,   112,   113,
     231,    88,    89,   192,   138,   113,    43,   196,    76,    43,
     174,    48,    63,    47,    48,   112,   256,   257,    52,   183,
     389,    55,   253,    57,   255,   185,    76,   162,   188,   177,
     351,    76,   300,     3,   302,   303,   150,   305,   142,   360,
     200,   410,   146,    77,   567,   319,     3,    84,    75,   608,
      84,   173,   326,   327,    88,    89,   358,   168,   169,   333,
     174,     3,    75,   177,   192,   351,   174,   436,   196,   183,
     439,   440,   206,   358,   360,   183,   776,   351,   192,   213,
     177,   154,     3,     3,     3,   199,   360,   174,   284,   162,
     177,     3,   637,   395,   208,    75,   183,   212,   212,   637,
       3,     3,   199,     3,   138,   138,    63,    88,    89,   138,
     491,   209,   493,    65,   212,   212,   150,    65,    75,     3,
     284,     1,   226,   283,   228,   659,    45,     3,   232,    29,
     290,   291,    73,   293,   294,   249,   296,    44,     3,   491,
     174,   493,    63,   177,    63,    29,    91,   499,   379,   183,
     381,    63,   300,   212,    75,   303,   249,   102,   192,     3,
      63,    63,   196,   192,   504,   505,   506,   196,    75,   521,
     284,    74,   206,   206,   208,    75,   284,   206,   212,   213,
     213,    92,    93,   317,   213,    29,   300,   302,   302,   303,
     305,   305,    76,   174,   563,   138,   177,   284,    63,   722,
     302,    91,   183,   300,   302,   302,   303,   305,   305,   154,
      75,    91,   102,   106,    75,   249,   109,   162,   448,   487,
     488,   138,   102,   554,   358,   594,   595,    16,    17,   542,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   806,   522,   192,
     284,   481,    14,   196,   460,   107,   370,   461,   372,     1,
     192,     1,   468,   206,   154,   469,   300,   497,   302,   303,
     213,   305,   461,   370,   154,   192,   480,    16,    17,   196,
     469,   661,   162,   317,   317,    91,   175,    76,   317,   206,
       1,   480,   452,    55,   100,   101,   213,   461,   402,   403,
     640,   461,   361,   284,   408,   364,   191,   106,   109,   469,
     109,   370,   460,   198,    76,   793,   201,   202,   203,   204,
     205,   561,   103,   644,   358,   358,    59,    59,   109,   358,
     352,    91,   354,   461,   356,    68,    68,    76,   372,   611,
     107,   469,   102,   611,   616,    74,   460,   461,   616,    91,
     110,    91,   480,   461,   468,   469,    75,   491,   644,     3,
     102,   469,   102,   460,   478,     3,   640,   158,   159,    75,
     644,   468,   707,   488,   317,   137,   490,   129,   608,    73,
      91,   712,   645,   103,   498,    29,   500,   154,   140,   109,
     488,   102,   277,   278,   154,   162,   776,   777,   778,     3,
     317,   102,   162,   633,    70,   635,   103,    45,   760,   110,
       3,    77,   154,    79,   154,   358,     3,     4,   770,   771,
     162,   187,   162,    16,    17,    63,   460,   461,   103,    67,
     636,    75,   461,   637,   186,   469,    67,   459,    76,    43,
     469,   358,   822,   154,    91,    76,   480,   749,   637,    91,
     103,   480,    45,   805,   488,   102,   490,   491,   491,    63,
     102,   110,   491,   611,   498,   112,   500,   716,   717,    91,
      63,   262,   263,   264,   265,   103,   707,   637,   363,    66,
     102,   366,    69,    76,   756,    72,    73,   129,   756,   106,
     112,   605,   109,    80,    47,    48,   848,   611,   140,    75,
     831,   764,   616,   789,   806,   660,   528,   154,   530,   637,
     532,   103,   154,   108,   611,   789,   129,   109,   461,   616,
     162,    74,    44,    45,    46,    47,   469,    49,    50,    51,
      52,    91,   154,   103,     3,   649,   650,   480,    73,   109,
     162,   154,   102,   102,   461,   659,   660,   661,   491,   162,
     110,   110,   469,    43,   166,   167,     3,   787,    48,    44,
      45,    46,    47,   480,    49,    50,    51,    52,    65,   129,
     102,   605,   104,   103,   491,    73,   806,   611,   110,   109,
     140,   603,   604,   846,   847,    54,    55,    56,     3,   170,
     171,   751,   832,   106,    84,   386,    61,    62,    88,    89,
     113,    16,    17,   637,   867,   868,   869,     3,   637,   713,
     123,   124,    73,   126,    29,   649,   129,     3,   732,    47,
      48,   776,     3,   113,   102,   659,   660,   140,   419,   175,
     143,   163,   164,   165,     3,     4,   427,   428,   429,   430,
     431,   432,   433,   434,     3,     4,    74,   103,    17,   440,
     535,   765,   103,   538,   106,    47,    48,    67,   650,    47,
      48,   113,   776,   777,   778,   109,    76,   781,   782,   661,
     175,   123,   124,   186,   126,    73,    67,   129,     3,     4,
     106,    90,    74,   109,   174,    76,    74,   177,   140,   113,
     804,   143,   796,   183,   637,    64,   810,    66,    91,   123,
     124,    67,   126,    72,    73,   129,   172,    66,   822,   102,
      76,    80,    81,    72,    73,    84,   140,   110,   208,   143,
     637,    80,   102,   103,   209,   210,   211,   212,    59,   843,
     110,   845,     3,    98,   186,   849,   850,    68,    99,    64,
     732,    66,   776,    59,    47,    48,   109,    72,    73,   112,
     155,   855,    68,   102,   103,    80,    81,   871,   111,    84,
       3,   110,   186,   103,   878,   879,   880,   881,   112,   109,
     804,    74,    11,   765,    45,   103,    91,    47,    48,   301,
     302,   109,   304,   305,     3,   777,   778,   102,     3,   781,
     782,    91,    63,     3,   284,   110,    67,   103,   103,     3,
       4,   106,   102,   109,    74,    76,   597,   598,   599,   600,
     601,   602,    91,    17,   129,    73,   301,   302,   810,   304,
     305,     3,   103,   102,     3,   140,     0,   196,   109,   129,
     822,     5,     6,     7,     8,     9,   108,   109,    12,   154,
     140,    15,   507,   508,    90,    91,    92,    93,    94,    95,
     129,   843,    81,   845,   154,   197,   102,   849,   850,   154,
      64,   140,    66,    45,   102,   111,   102,   103,    72,    73,
     103,   196,    46,   192,   110,   154,    80,    81,    75,   871,
      84,    63,   372,   162,   111,    67,   878,   879,   880,   881,
     102,   103,    10,   139,    76,   102,   103,     3,   110,    49,
      50,    51,    52,   110,     1,   103,   509,   510,   154,   155,
     156,   157,   158,   159,     3,   161,    66,     3,   164,   165,
      70,    71,     3,     3,    74,   716,   717,    77,    78,    79,
      80,    81,    30,    31,    32,    33,    34,    35,     3,   113,
     114,   115,   116,   515,   516,   119,   192,   193,   122,   123,
     124,   125,   126,   106,   128,   129,   130,   131,   132,    45,
     103,   135,   136,   103,   106,   102,   140,   106,   102,   143,
     102,   461,    66,   102,   112,   106,    70,    63,    73,   469,
      68,    67,   109,    77,    78,    79,    80,     1,   162,   112,
      76,   112,   196,    90,    91,    92,    93,    94,    95,   103,
      23,    74,    25,   488,     3,   102,     3,    68,   144,   106,
     107,   108,   186,    36,    37,   102,   113,   114,   115,   116,
     117,   195,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   198,   199,
      18,    19,    20,    21,    22,   112,   106,   154,   155,   156,
     157,   158,   159,   106,   161,   162,   106,   164,   165,   103,
       1,    66,   192,   103,   106,    70,    90,    91,    92,    93,
      94,    95,    77,   102,    79,    80,    74,   103,   102,   186,
       3,   106,   106,   107,   106,   192,   193,   194,   195,   113,
     114,   115,   116,   117,   106,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   118,   106,     1,   103,    49,    50,    51,    52,   103,
     154,   155,   156,   157,   158,   159,   103,   161,   162,   103,
     164,   165,    66,     0,    31,   141,    70,    71,    91,   511,
     512,   513,   514,    77,    78,    79,    80,    81,     3,   102,
     547,   705,   186,     3,    24,   375,    26,   110,   192,   193,
     194,   195,   113,   114,   115,   116,    36,    37,   119,   162,
     406,   122,   123,   124,   125,   126,   129,   128,   129,   130,
     131,   132,   799,   242,   135,   136,   419,   140,   517,   140,
      45,     1,   143,   520,   518,    45,    13,    14,    15,   281,
     519,   154,    90,    91,    92,    93,    94,    95,    63,   162,
     522,   162,    67,    63,   102,   491,   804,    67,   106,   107,
     108,    76,   372,   174,    41,    42,    76,   206,   480,   117,
     364,   370,   120,   121,   150,   186,   397,   246,    -1,   127,
      57,    58,    -1,    60,   195,   133,   134,    -1,    -1,   137,
     138,   139,    -1,   141,   142,    -1,   144,    -1,    -1,    76,
      -1,    -1,    -1,    -1,   198,   199,   154,   155,   156,   157,
     158,   159,     1,   161,   162,    -1,   164,   165,    -1,    -1,
      90,    91,    92,    93,    94,    95,    18,    19,    20,    21,
      22,    -1,   102,    13,    14,    15,   106,   107,    -1,    -1,
      -1,    -1,     1,    -1,   192,   193,   194,   117,    -1,    -1,
     120,   121,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    41,    42,   133,   134,    -1,    -1,   137,   138,   139,
      -1,   141,   142,    -1,   144,    -1,    -1,    57,    58,    -1,
      60,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
      -1,   161,   162,    -1,   164,   165,    76,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    66,    -1,   106,     1,    70,
      71,    -1,   192,   193,   194,    -1,    77,    78,    79,    80,
      81,    90,    91,    92,    93,    94,    95,    -1,    -1,    -1,
      66,    -1,    -1,   102,    70,    71,    -1,   106,    74,    -1,
     139,    77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,    -1,    -1,    -1,
     139,     5,     6,     7,     8,     9,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,   154,   155,   156,   157,   158,
     159,    -1,   161,   192,   193,   164,   165,    90,    91,    92,
      93,    94,    95,    30,    31,    32,    33,    34,    35,   102,
     103,    -1,    46,    -1,   114,   115,    -1,    -1,    -1,   119,
       1,    -1,    -1,   192,   193,   125,    -1,    -1,    -1,   129,
     130,   131,   132,    -1,    -1,   135,   136,    -1,    -1,    66,
     140,    -1,    -1,    70,    71,    -1,   139,    -1,    -1,    -1,
      77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,   154,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,     1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,    -1,    -1,   119,   186,    -1,   122,   123,
     124,   125,   126,    -1,   128,   129,   130,   131,   132,   192,
     193,   135,   136,    66,    -1,    -1,   140,    70,    71,   143,
      91,    -1,    -1,    -1,    77,    78,    79,    80,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
      -1,    -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,
      -1,   122,   123,   124,   125,   126,    -1,   128,   129,   130,
     131,   132,   186,    -1,   135,   136,    -1,    -1,    -1,   140,
      -1,   195,   143,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,    -1,   113,   114,   115,   116,    -1,
      -1,   119,    -1,    -1,   122,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,   186,    -1,   135,   136,    -1,
      -1,    -1,   140,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    90,    91,    92,
      93,    94,    95,    -1,   162,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,    -1,    -1,   119,    -1,   186,   122,
      -1,    -1,   125,    -1,    -1,   128,   129,   130,   131,   132,
      -1,    -1,   135,   136,    -1,    -1,   139,   140,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,    -1,
      -1,   154,   155,   156,   157,   158,   159,    -1,   161,   162,
      -1,   164,   165,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   186,    -1,   113,   114,   115,   116,   192,
     193,   119,   195,    -1,   122,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,    -1,
      -1,    -1,   140,    -1,    -1,   143,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   102,   103,    -1,
      -1,    -1,    -1,    -1,   162,   110,    -1,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,   174,   122,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,   186,    -1,
     135,   136,    -1,    -1,    -1,   140,    -1,   195,   143,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     102,   103,    -1,    -1,    -1,    -1,    -1,   162,   110,    -1,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   174,
     122,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,   186,    -1,   135,   136,    -1,    -1,    -1,   140,    -1,
     195,   143,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
     162,    -1,   107,   108,    -1,    -1,    90,    91,    92,    93,
      94,    95,   174,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,   195,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,   156,   157,   158,   159,   139,   161,    -1,    -1,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,   156,   157,   158,   159,    -1,   161,    -1,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,   192,   193,    90,
      91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,   107,    -1,   192,   193,
      90,    91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,   139,    -1,
      -1,    -1,    -1,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,   156,   157,   158,   159,   139,
     161,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     139,   161,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,   192,   193,    -1,    -1,   154,   155,   156,   157,   158,
     159,    -1,   161,    -1,    -1,   164,   165,    -1,    -1,    -1,
      -1,    -1,   192,   193,    90,    91,    92,    93,    94,    95,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    90,    91,    92,
      93,    94,    95,   192,   193,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,   139,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   154,   155,
     156,   157,   158,   159,    -1,   161,    -1,    -1,   164,   165,
      -1,   154,   155,   156,   157,   158,   159,    -1,   161,    -1,
      -1,   164,   165,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,   193,    -1,    -1,
      -1,   154,   155,   156,   157,   158,   159,    -1,   161,   192,
     193,   164,   165,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,   113,   114,   115,   116,   192,
     193,   119,    -1,    -1,   122,   123,   124,   125,   126,    91,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,    -1,
     102,    -1,   140,    -1,    -1,   143,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   154,   119,    -1,    -1,
     122,    -1,    -1,   125,   162,    -1,   128,   129,   130,   131,
     132,    91,    -1,   135,   136,    -1,    -1,    -1,   140,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,   186,    -1,
     110,    -1,   154,    -1,   114,   115,   116,    -1,    -1,   119,
     162,    -1,   122,    -1,    -1,   125,    -1,    -1,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,    -1,   103,    -1,
     140,    -1,    -1,    -1,   186,    -1,    -1,    -1,   113,   114,
     115,   116,    -1,    -1,   119,    -1,    -1,   122,   123,   124,
     125,   126,   162,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,   154,
      -1,   103,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
      -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,   174,
     122,   123,   124,   125,   126,    -1,   128,   129,   130,   131,
     132,   186,    -1,   135,   136,    -1,    -1,    -1,   140,    -1,
     195,   143,   108,    -1,    -1,    -1,    -1,    -1,   114,   115,
     116,    -1,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,
     162,    -1,   128,   129,   130,   131,   132,    -1,    -1,   135,
     136,    -1,   174,    -1,   140,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,   113,   114,
     115,   116,    -1,   195,   119,    -1,   162,   122,   123,   124,
     125,   126,    -1,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,    -1,    -1,    -1,   140,    -1,    -1,   143,    -1,
     186,    -1,   113,   114,   115,    -1,    -1,    -1,   119,   195,
      -1,    -1,   123,   124,   125,   126,    -1,   162,   129,   130,
     131,   132,    -1,    -1,   135,   136,    -1,     3,    -1,   140,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    18,    19,    20,    21,    22,    -1,    -1,    -1,
     195,    27,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65
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

case 5:
#line 499 "language.y"
{ Annotations::Enabled_properties.push_back(yyvsp[0].t_id->name()); ;
    break;}
case 6:
#line 501 "language.y"
{ Annotations::Disabled_properties.push_back(yyvsp[0].t_id->name()); ;
    break;}
case 10:
#line 506 "language.y"
{
        Annotations::Current->init()->add_analysis(yyvsp[0].t_analyze);
      ;
    break;}
case 11:
#line 513 "language.y"
{ Annotations::Current->store_header(yyvsp[0].declL); ;
    break;}
case 12:
#line 520 "language.y"
{
        Annotations::Current->add_globals( yyvsp[-1].t_structures );
        delete yyvsp[-1].t_structures;
      ;
    break;}
case 13:
#line 526 "language.y"
{
        structuretree_list * temp = new structuretree_list();
	temp->push_back( yyvsp[0].t_structure);
        Annotations::Current->add_globals( temp );
      ;
    break;}
case 14:
#line 537 "language.y"
{
        Annotations::Current->add_enum_property(new enumPropertyAnn(yyvsp[-5].t_id, yyvsp[-3].t_direction, true, yyvsp[0].t_id, yyvsp[-2].t_enumvalue_list, yyvsp[-1].t_ids));
	delete yyvsp[-5].t_id;
	delete yyvsp[-2].t_enumvalue_list;
	delete yyvsp[-1].t_ids;
      ;
    break;}
case 15:
#line 545 "language.y"
{
        Annotations::Current->add_enum_property(new enumPropertyAnn(yyvsp[-5].t_id, yyvsp[-3].t_direction, false, yyvsp[0].t_id, yyvsp[-2].t_enumvalue_list, yyvsp[-1].t_ids));
	delete yyvsp[-5].t_id;
	delete yyvsp[-2].t_enumvalue_list;
	delete yyvsp[-1].t_ids;
      ;
    break;}
case 16:
#line 553 "language.y"
{
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Union));
	delete yyvsp[-2].t_id;
      ;
    break;}
case 17:
#line 561 "language.y"
{
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Set,
								  setPropertyAnn::Intersect));
	delete yyvsp[-2].t_id;
      ;
    break;}
case 18:
#line 569 "language.y"
{
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Union));
	delete yyvsp[-2].t_id;
      ;
    break;}
case 19:
#line 577 "language.y"
{
        Annotations::Current->add_set_property(new setPropertyAnn(yyvsp[-2].t_id, Forward, 
								  setPropertyAnn::Equivalence,
								  setPropertyAnn::Intersect));
	delete yyvsp[-2].t_id;
      ;
    break;}
case 20:
#line 587 "language.y"
{
      yyval.t_direction = Forward;
    ;
    break;}
case 21:
#line 592 "language.y"
{
      yyval.t_direction = Forward;
    ;
    break;}
case 22:
#line 597 "language.y"
{
      yyval.t_direction = Backward;
    ;
    break;}
case 23:
#line 604 "language.y"
{
        yyval.t_id = (parserID *)0;
      ;
    break;}
case 24:
#line 609 "language.y"
{
	yyval.t_id = yyvsp[0].t_id;
      ;
    break;}
case 25:
#line 615 "language.y"
{ yyval.t_ids = NULL; ;
    break;}
case 26:
#line 617 "language.y"
{ yyval.t_ids = yyvsp[-1].t_ids; ;
    break;}
case 27:
#line 622 "language.y"
{
	yyval.t_enumvalue_list = yyvsp[-1].t_enumvalue_list;
      ;
    break;}
case 28:
#line 629 "language.y"
{
        yyval.t_enumvalue_list = new enumvalue_list();
	yyval.t_enumvalue_list->push_back(yyvsp[0].t_enumvalue);
      ;
    break;}
case 29:
#line 635 "language.y"
{
	yyval.t_enumvalue_list = yyvsp[-2].t_enumvalue_list;
	yyval.t_enumvalue_list->push_back(yyvsp[0].t_enumvalue);
      ;
    break;}
case 30:
#line 643 "language.y"
{
        yyval.t_enumvalue = new enumValueAnn(yyvsp[0].t_id, (enumvalue_list *) 0);
	delete yyvsp[0].t_id;
      ;
    break;}
case 31:
#line 649 "language.y"
{
	yyval.t_enumvalue = new enumValueAnn(yyvsp[-1].t_id, yyvsp[0].t_enumvalue_list);
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_enumvalue_list;
      ;
    break;}
case 32:
#line 660 "language.y"
{
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      ;
    break;}
case 33:
#line 666 "language.y"
{
        Annotations::Current->add_procedure(procedureAnn::Current);
        procedureAnn::Current = 0;
      ;
    break;}
case 34:
#line 674 "language.y"
{
        procedureAnn::Current = new procedureAnn(yyvsp[-3].t_id, yyvsp[-1].t_ids, Annotations::Current, yyvsp[-4].t_tok);
        delete yyvsp[-3].t_id;
      ;
    break;}
case 35:
#line 680 "language.y"
{
        procedureAnn::Current = new procedureAnn(yyvsp[-2].t_id, (parserid_list *)0, Annotations::Current, yyvsp[-3].t_tok);
        delete yyvsp[-2].t_id;
      ;
    break;}
case 38:
#line 693 "language.y"
{
      ;
    break;}
case 39:
#line 697 "language.y"
{
      ;
    break;}
case 40:
#line 701 "language.y"
{
	procedureAnn::Current->add_analysis(yyvsp[0].t_analyze);
      ;
    break;}
case 41:
#line 706 "language.y"
{
      ;
    break;}
case 42:
#line 710 "language.y"
{
      ;
    break;}
case 43:
#line 718 "language.y"
{
        procedureAnn::Current->add_on_entry(yyvsp[-1].t_structures);
      ;
    break;}
case 44:
#line 723 "language.y"
{
        procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, yyvsp[-1].t_structures, yyvsp[-3].t_tok));
      ;
    break;}
case 45:
#line 728 "language.y"
{
      ;
    break;}
case 46:
#line 732 "language.y"
{
      ;
    break;}
case 47:
#line 736 "language.y"
{
      ;
    break;}
case 50:
#line 747 "language.y"
{
	procedureAnn::Current->add_on_exit(new pointerRuleAnn(yyvsp[-4].t_expr, yyvsp[-1].t_structures, yyvsp[-6].t_tok));
      ;
    break;}
case 51:
#line 752 "language.y"
{
	structuretree_list * temp = new structuretree_list();
	temp->push_back(yyvsp[0].t_structure);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn(yyvsp[-2].t_expr, temp, yyvsp[-4].t_tok));
      ;
    break;}
case 52:
#line 759 "language.y"
{
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, yyvsp[-1].t_structures, yyvsp[-3].t_tok));
      ;
    break;}
case 53:
#line 764 "language.y"
{
	structuretree_list * temp = new structuretree_list();
	temp->push_back(yyvsp[0].t_structure);
	procedureAnn::Current->add_on_exit(new pointerRuleAnn((exprAnn *)0, temp, yyvsp[-1].t_tok));
      ;
    break;}
case 54:
#line 774 "language.y"
{
        yyval.t_structures = new structuretree_list();
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      ;
    break;}
case 55:
#line 780 "language.y"
{
	yyval.t_structures = yyvsp[-1].t_structures;
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      ;
    break;}
case 56:
#line 786 "language.y"
{
	yyval.t_structures = yyvsp[-2].t_structures;
	yyval.t_structures->push_back(yyvsp[0].t_structure);
      ;
    break;}
case 57:
#line 792 "language.y"
{ 
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;

        /* "delete" doesn't actually add anything to the structure tree, so
           we just pass an empty list back. Similarly for the next two
           productions. */

        yyval.t_structures = new structuretree_list();
      ;
    break;}
case 58:
#line 804 "language.y"
{
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
	yyval.t_structures = yyvsp[-2].t_structures;
      ;
    break;}
case 59:
#line 811 "language.y"
{
        procedureAnn::Current->add_delete(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
	yyval.t_structures = yyvsp[-3].t_structures;
      ;
    break;}
case 60:
#line 820 "language.y"
{
        yyval.t_structure = new structureTreeAnn(yyvsp[0].t_id, (structuretree_list *)0,
                                  structureTreeAnn::None, false);
	delete yyvsp[0].t_id;
      ;
    break;}
case 61:
#line 827 "language.y"
{
        yyval.t_structure = new structureTreeAnn(yyvsp[-2].t_id, yyvsp[0].t_structure, structureTreeAnn::Arrow, false);
	delete yyvsp[-2].t_id;
      ;
    break;}
case 62:
#line 833 "language.y"
{
        yyval.t_structure = new structureTreeAnn(yyvsp[-3].t_id, yyvsp[0].t_structure, structureTreeAnn::Arrow, true);
	delete yyvsp[-3].t_id;
      ;
    break;}
case 63:
#line 839 "language.y"
{
        yyval.t_structure = new structureTreeAnn(yyvsp[-3].t_id, yyvsp[-1].t_structures, structureTreeAnn::Dot, false);
	delete yyvsp[-3].t_id;
      ;
    break;}
case 64:
#line 850 "language.y"
{
        procedureAnn::Current->add_uses( yyvsp[-1].t_ids );
	delete yyvsp[-1].t_ids;
      ;
    break;}
case 65:
#line 856 "language.y"
{
        parserid_list temp;
	temp.push_back( * yyvsp[0].t_id);
        procedureAnn::Current->add_uses( & temp );
	delete yyvsp[0].t_id;
      ;
    break;}
case 66:
#line 864 "language.y"
{
        procedureAnn::Current->add_defs( yyvsp[-1].t_ids );
	delete yyvsp[-1].t_ids;
      ;
    break;}
case 67:
#line 870 "language.y"
{
        parserid_list temp;
	temp.push_back( * yyvsp[0].t_id);
        procedureAnn::Current->add_defs( & temp );
	delete yyvsp[0].t_id;
      ;
    break;}
case 68:
#line 883 "language.y"
{
	yyval.t_analyze = new analyzeAnn(Annotations::Current, yyvsp[-3].t_id, yyvsp[-1].t_rules, yyvsp[-4].t_tok);
	delete yyvsp[-3].t_id;
      ;
    break;}
case 69:
#line 889 "language.y"
{
	rule_list * temp_rules = new rule_list();
	temp_rules->push_back(new ruleAnn((exprAnn *)0, yyvsp[-1].t_exprs, yyvsp[-2].t_tok));
	yyval.t_analyze = new analyzeAnn(Annotations::Current, yyvsp[-3].t_id, temp_rules, yyvsp[-4].t_tok);
	delete yyvsp[-3].t_id;
      ;
    break;}
case 70:
#line 899 "language.y"
{
	yyval.t_rules = new rule_list();
	yyval.t_rules->push_back(yyvsp[0].t_rule);
      ;
    break;}
case 71:
#line 905 "language.y"
{
	yyval.t_rules = yyvsp[-1].t_rules;
	yyval.t_rules->push_back(yyvsp[0].t_rule);
      ;
    break;}
case 72:
#line 913 "language.y"
{
	yyval.t_rule = new ruleAnn(yyvsp[-4].t_expr, yyvsp[-1].t_exprs, yyvsp[-6].t_tok);
      ;
    break;}
case 73:
#line 918 "language.y"
{
	exprann_list * temp = new exprann_list();
	temp->push_back(yyvsp[0].t_expr);
	yyval.t_rule = new ruleAnn(yyvsp[-2].t_expr, temp, yyvsp[-4].t_tok);
      ;
    break;}
case 74:
#line 925 "language.y"
{
	yyval.t_rule = new ruleAnn((exprAnn *)0, yyvsp[-1].t_exprs, yyvsp[-3].t_tok);
      ;
    break;}
case 75:
#line 930 "language.y"
{
	exprann_list * temp = new exprann_list();
	temp->push_back(yyvsp[0].t_expr);
	yyval.t_rule = new ruleAnn((exprAnn *)0, temp, yyvsp[-1].t_tok);
      ;
    break;}
case 76:
#line 940 "language.y"
{
        yyval.t_expr = yyvsp[0].t_expr;
      ;
    break;}
case 77:
#line 945 "language.y"
{
        yyval.t_expr = new connectiveExprAnn(Broadway::Or, yyvsp[-2].t_expr, yyvsp[0].t_expr, yyvsp[-1].t_tok);
      ;
    break;}
case 78:
#line 950 "language.y"
{
        yyval.t_expr = new connectiveExprAnn(Broadway::And, yyvsp[-2].t_expr, yyvsp[0].t_expr, yyvsp[-1].t_tok);
      ;
    break;}
case 79:
#line 955 "language.y"
{
        yyval.t_expr = new connectiveExprAnn(Broadway::Not, yyvsp[0].t_expr, (exprAnn *)0, yyvsp[-1].t_tok);
      ;
    break;}
case 80:
#line 960 "language.y"
{
        yyval.t_expr = yyvsp[-1].t_expr;
      ;
    break;}
case 81:
#line 969 "language.y"
{
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-1].t_property_sensitivity.fs, yyvsp[-4].t_id, yyvsp[-2].t_id, Broadway::Is_Bottom,
				     (const parserID *)0, yyvsp[-1].t_property_sensitivity.line);
	delete yyvsp[-4].t_id;
	delete yyvsp[-2].t_id;
      ;
    break;}
case 82:
#line 977 "language.y"
{
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-1].t_property_sensitivity.fs, (const parserID *)0, yyvsp[-2].t_id, Broadway::Is_Bottom,
				     (const parserID *)0, yyvsp[-1].t_property_sensitivity.line);
	delete yyvsp[-2].t_id;
      ;
    break;}
case 83:
#line 984 "language.y"
{
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-2].t_property_sensitivity.fs, yyvsp[-5].t_id, yyvsp[-3].t_id, yyvsp[-1].t_property_operator.op, yyvsp[0].t_id, yyvsp[-1].t_property_operator.line);
	delete yyvsp[-5].t_id;
	delete yyvsp[-3].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 84:
#line 992 "language.y"
{
        yyval.t_expr = new enumPropertyExprAnn(yyvsp[-2].t_property_sensitivity.fs, (const parserID *)0, yyvsp[-3].t_id, yyvsp[-1].t_property_operator.op, yyvsp[0].t_id, yyvsp[-1].t_property_operator.line);
	delete yyvsp[-3].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 85:
#line 1001 "language.y"
{
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[-1].t_id, yyvsp[-2].t_id, Broadway::Is_Equivalent, yyvsp[0].t_id, yyvsp[-2].t_id->line());
	delete yyvsp[-2].t_id;
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 86:
#line 1009 "language.y"
{
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[0].t_id, yyvsp[-2].t_id, Broadway::Is_Element_Of,
				    (const parserID *)0, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 87:
#line 1017 "language.y"
{
        yyval.t_expr = new setPropertyExprAnn(Broadway::Before, yyvsp[-1].t_id, (const parserID *)0,
                                    Broadway::Is_EmptySet, (const parserID *)0, yyvsp[0].t_tok);

	delete yyvsp[-1].t_id;
      ;
    break;}
case 88:
#line 1027 "language.y"
{
        yyval.t_expr = new numericExprAnn(yyvsp[0].t_c_expression, annlineno);
      ;
    break;}
case 89:
#line 1032 "language.y"
{
        yyval.t_expr = new numericExprAnn(yyvsp[-1].t_id, annlineno);
      ;
    break;}
case 90:
#line 1039 "language.y"
{
        yyval.t_expr = new pointerExprAnn(Broadway::Is_AliasOf, yyvsp[-2].t_id, yyvsp[0].t_id);
        delete yyvsp[-2].t_id;
        delete yyvsp[0].t_id;
      ;
    break;}
case 91:
#line 1046 "language.y"
{
        yyval.t_expr = new pointerExprAnn(Broadway::Is_SameAs, yyvsp[-2].t_id, yyvsp[0].t_id);
        delete yyvsp[-2].t_id;
        delete yyvsp[0].t_id;
      ;
    break;}
case 92:
#line 1053 "language.y"
{
        yyval.t_expr = new pointerExprAnn(Broadway::Is_Empty, yyvsp[-1].t_id, (const parserID *)0);
        delete yyvsp[-1].t_id;
      ;
    break;}
case 93:
#line 1062 "language.y"
{
	yyval.t_exprs = new exprann_list();
	yyval.t_exprs->push_back(yyvsp[0].t_expr);
      ;
    break;}
case 94:
#line 1068 "language.y"
{
	yyval.t_exprs = yyvsp[-1].t_exprs;
	yyval.t_exprs->push_back(yyvsp[0].t_expr);
      ;
    break;}
case 95:
#line 1077 "language.y"
{
	yyval.t_expr = new numericExprAnn(yyvsp[-2].t_id, yyvsp[0].t_c_expression, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
      ;
    break;}
case 96:
#line 1083 "language.y"
{
        yyval.t_expr = new enumPropertyExprAnn(Broadway::None,
				     (const parserID *)0, yyvsp[-2].t_id, Broadway::Assign,
				     yyvsp[0].t_id, yyvsp[-1].t_tok);
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 97:
#line 1092 "language.y"
{
        enumPropertyExprAnn * tmp = new enumPropertyExprAnn(Broadway::None,
							    (const parserID *)0, yyvsp[-2].t_id, Broadway::Assign,
							    yyvsp[0].t_id, yyvsp[-1].t_tok);
	tmp->set_weak();
	yyval.t_expr = tmp;
	delete yyvsp[-2].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 98:
#line 1103 "language.y"
{
        yyval.t_expr = new setPropertyExprAnn(Broadway::None,
				    (const parserID *)0, yyvsp[0].t_id, Broadway::Add_Elements,
				    (const parserID *)0, yyvsp[-1].t_tok);
	delete yyvsp[0].t_id;
      ;
    break;}
case 99:
#line 1111 "language.y"
{
        yyval.t_expr = new setPropertyExprAnn(Broadway::None,
				    yyvsp[-1].t_id, yyvsp[-2].t_id, Broadway::Add_Equivalences, yyvsp[0].t_id, yyvsp[-2].t_id->line());
	delete yyvsp[-2].t_id;
	delete yyvsp[-1].t_id;
	delete yyvsp[0].t_id;
      ;
    break;}
case 100:
#line 1123 "language.y"
{ yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_Exactly; ;
    break;}
case 101:
#line 1127 "language.y"
{ yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_AtLeast; ;
    break;}
case 102:
#line 1131 "language.y"
{ yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Could_Be; ;
    break;}
case 103:
#line 1135 "language.y"
{ yyval.t_property_operator.line = yyvsp[0].t_tok;
        yyval.t_property_operator.op = Broadway::Is_AtMost; ;
    break;}
case 104:
#line 1142 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Before; ;
    break;}
case 105:
#line 1146 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::After; ;
    break;}
case 106:
#line 1150 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Always; ;
    break;}
case 107:
#line 1154 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Ever; ;
    break;}
case 108:
#line 1158 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Trace; ;
    break;}
case 109:
#line 1162 "language.y"
{ yyval.t_property_sensitivity.line = yyvsp[0].t_tok;
         yyval.t_property_sensitivity.fs = Broadway::Confidence; ;
    break;}
case 110:
#line 1166 "language.y"
{ yyval.t_property_sensitivity.line =  0;
         yyval.t_property_sensitivity.fs = Broadway::Before; ;
    break;}
case 111:
#line 1175 "language.y"
{
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, false, yyvsp[-1].t_report_elements, yyvsp[-2].t_tok));
       ;
    break;}
case 112:
#line 1180 "language.y"
{
         procedureAnn::Current->add_report(new reportAnn(yyvsp[-3].t_expr, false, yyvsp[-1].t_report_elements, yyvsp[-6].t_tok));
       ;
    break;}
case 113:
#line 1184 "language.y"
{
         procedureAnn::Current->add_report(new reportAnn((exprAnn *)0, true, yyvsp[-1].t_report_elements, yyvsp[-2].t_tok));
       ;
    break;}
case 114:
#line 1189 "language.y"
{
         procedureAnn::Current->add_report(new reportAnn(yyvsp[-3].t_expr, true, yyvsp[-1].t_report_elements, yyvsp[-6].t_tok));
       ;
    break;}
case 115:
#line 1197 "language.y"
{
         yyval.t_report_elements = new report_element_list();
         yyval.t_report_elements->push_back( yyvsp[0].t_report_element );
       ;
    break;}
case 116:
#line 1203 "language.y"
{
         yyval.t_report_elements = yyvsp[-2].t_report_elements;
         yyval.t_report_elements->push_back(yyvsp[0].t_report_element);
       ;
    break;}
case 117:
#line 1213 "language.y"
{
         yyval.t_report_element = new literalReportElementAnn(yyvsp[0].t_id);
         delete yyvsp[0].t_id;
       ;
    break;}
case 118:
#line 1219 "language.y"
{
        yyval.t_report_element = new expressionReportElementAnn(yyvsp[0].t_property_sensitivity.fs,
	                                    new enumPropertyExprAnn(yyvsp[0].t_property_sensitivity.fs, yyvsp[-3].t_id, yyvsp[-1].t_id, Broadway::Report,
                                                                (const parserID *)0, yyvsp[-3].t_id->line()),
                                            annlineno);
	delete yyvsp[-3].t_id;
	delete yyvsp[-1].t_id;
      ;
    break;}
case 119:
#line 1229 "language.y"
{
        yyval.t_report_element = new expressionReportElementAnn(yyvsp[0].t_property_sensitivity.fs,
                                            new numericExprAnn(yyvsp[-1].t_c_expression, yyvsp[0].t_property_sensitivity.line),
                                            annlineno);
      ;
    break;}
case 120:
#line 1236 "language.y"
{
        yyval.t_report_element = new locationReportElementAnn(yyvsp[0].t_id);
      ;
    break;}
case 121:
#line 1241 "language.y"
{
        yyval.t_report_element = new bindingReportElementAnn(yyvsp[-1].t_id, false);
      ;
    break;}
case 122:
#line 1246 "language.y"
{
        yyval.t_report_element = new bindingReportElementAnn(yyvsp[-1].t_id, true);
      ;
    break;}
case 123:
#line 1255 "language.y"
{
        procedureAnn::Current->add_action(new actionAnn(yyvsp[-3].t_expr, yyvsp[0].stmtN, yyvsp[-5].t_tok));
      ;
    break;}
case 124:
#line 1260 "language.y"
{
        procedureAnn::Current->add_action(new actionAnn(yyvsp[-3].t_expr, yyvsp[-5].t_tok));
      ;
    break;}
case 125:
#line 1269 "language.y"
{
      ;
    break;}
case 126:
#line 1277 "language.y"
{ yyval.t_ids = new parserid_list();
        yyval.t_ids->push_back( * yyvsp[0].t_id );
	delete yyvsp[0].t_id;
      ;
    break;}
case 127:
#line 1283 "language.y"
{ yyval.t_ids = yyvsp[-2].t_ids;
        yyval.t_ids->push_back( * yyvsp[0].t_id);
        delete yyvsp[0].t_id;
      ;
    break;}
case 128:
#line 1291 "language.y"
{ yyval.t_id = yyvsp[0].t_id; ;
    break;}
case 129:
#line 1294 "language.y"
{ yyval.t_id = yyvsp[0].t_id;
        yyval.t_id->set_io();
      ;
    break;}
case 130:
#line 1304 "language.y"
{ yyval.t_c_expression = new binaryNode(Operator::EQ, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 131:
#line 1307 "language.y"
{ yyval.t_c_expression = new binaryNode(Operator::NE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 132:
#line 1310 "language.y"
{ yyval.t_c_expression = new binaryNode('<', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 133:
#line 1313 "language.y"
{ yyval.t_c_expression = new binaryNode(Operator::LE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 134:
#line 1316 "language.y"
{ yyval.t_c_expression = new binaryNode('>', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 135:
#line 1319 "language.y"
{ yyval.t_c_expression = new binaryNode(Operator::GE, yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 136:
#line 1325 "language.y"
{ yyval.t_c_expression = yyvsp[0].t_c_expression; ;
    break;}
case 137:
#line 1328 "language.y"
{ yyval.t_c_expression = new idNodeAnn(yyvsp[0].t_id);
        delete yyvsp[0].t_id;
      ;
    break;}
case 138:
#line 1333 "language.y"
{ yyval.t_c_expression = yyvsp[-1].t_c_expression; yyval.t_c_expression->parenthesized(true); ;
    break;}
case 139:
#line 1336 "language.y"
{ yyval.t_c_expression = new binaryNode('+', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 140:
#line 1339 "language.y"
{ yyval.t_c_expression = new binaryNode('-', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 141:
#line 1342 "language.y"
{ yyval.t_c_expression = new binaryNode('|', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 142:
#line 1345 "language.y"
{ yyval.t_c_expression = new binaryNode('^', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 143:
#line 1348 "language.y"
{ yyval.t_c_expression = new binaryNode('&', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 144:
#line 1351 "language.y"
{ yyval.t_c_expression = new binaryNode('*', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 145:
#line 1354 "language.y"
{ yyval.t_c_expression = new binaryNode('/', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 146:
#line 1357 "language.y"
{ yyval.t_c_expression = new binaryNode('%', yyvsp[-2].t_c_expression, yyvsp[0].t_c_expression); ;
    break;}
case 147:
#line 1360 "language.y"
{ yyval.t_c_expression = new unaryNode('-', yyvsp[0].t_c_expression); ;
    break;}
case 148:
#line 1363 "language.y"
{ yyval.t_c_expression = new unaryNode('+', yyvsp[0].t_c_expression); ;
    break;}
case 149:
#line 1366 "language.y"
{ yyval.t_c_expression = new unaryNode('~', yyvsp[0].t_c_expression); ;
    break;}
case 150:
#line 1384 "language.y"
{ yyval.exprN = yyvsp[0].idN; ;
    break;}
case 152:
#line 1390 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 153:
#line 1392 "language.y"
{ if (yyvsp[-1].exprN->typ() == Comma) yyvsp[-1].exprN->coord(Coord(yyvsp[-2].tok));
                                  yyvsp[-1].exprN->parenthesized(true);
                                  yyval.exprN = yyvsp[-1].exprN; ;
    break;}
case 154:
#line 1399 "language.y"
{ if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
               yyval.exprN = (exprNode *) /* This is BAD */
		 new blockNode((decl_list *)0, yyvsp[-2].stmtL, Coord(yyvsp[-4].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 155:
#line 1405 "language.y"
{ if (CBZ::ANSIOnly)
	         CBZ::SyntaxError(string("statement expressions not allowed with -ansi switch"));
              yyval.exprN =  (exprNode *) /* This is BAD */
		new blockNode(yyvsp[-3].declL, yyvsp[-2].stmtL, Coord(yyvsp[-5].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 156:
#line 1410 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 157:
#line 1415 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 158:
#line 1418 "language.y"
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
case 159:
#line 1430 "language.y"
{ yyval.exprN = new callNode(yyvsp[-2].exprN, (expr_list *)0, Coord(yyvsp[-1].tok)); ;
    break;}
case 160:
#line 1433 "language.y"
{ yyval.exprN = new callNode(yyvsp[-3].exprN, yyvsp[-1].exprL, Coord(yyvsp[-2].tok)); ;
    break;}
case 161:
#line 1436 "language.y"
{ yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 162:
#line 1439 "language.y"
{ yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 163:
#line 1442 "language.y"
{ yyval.exprN = new unaryNode( Operator::POSTINC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); ;
    break;}
case 164:
#line 1445 "language.y"
{ yyval.exprN = new unaryNode( Operator::POSTDEC, yyvsp[-1].exprN, Coord(yyvsp[0].tok)); ;
    break;}
case 165:
#line 1449 "language.y"
{ yyval.exprN = new binaryNode('.', yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 166:
#line 1452 "language.y"
{ yyval.exprN = new binaryNode( Operator::ARROW, yyvsp[-2].exprN, yyvsp[0].idN, Coord(yyvsp[-1].tok)); ;
    break;}
case 167:
#line 1457 "language.y"
{ yyval.exprL = new expr_list(); yyval.exprL->push_back(yyvsp[0].exprN); ;
    break;}
case 168:
#line 1460 "language.y"
{ yyval.exprL = yyvsp[-2].exprL; yyval.exprL->push_back(yyvsp[0].exprN); ;
    break;}
case 169:
#line 1465 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 170:
#line 1468 "language.y"
{ yyval.exprN = new unaryNode( Operator::PREINC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 171:
#line 1471 "language.y"
{ yyval.exprN = new unaryNode( Operator::PREDEC, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 172:
#line 1474 "language.y"
{ yyvsp[-1].unaryN->expr(yyvsp[0].exprN);
              yyval.exprN = yyvsp[-1].unaryN; ;
    break;}
case 173:
#line 1478 "language.y"
{ yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 174:
#line 1481 "language.y"
{ yyval.exprN = new unaryNode( Operator::SIZEOF, yyvsp[-1].typeN.ty, Coord(yyvsp[-3].tok)); ;
    break;}
case 175:
#line 1485 "language.y"
{ yyval.unaryN = new unaryNode('&', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 176:
#line 1486 "language.y"
{ yyval.unaryN = new unaryNode('*', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 177:
#line 1487 "language.y"
{ yyval.unaryN = new unaryNode('+', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 178:
#line 1488 "language.y"
{ yyval.unaryN = new unaryNode('-', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 179:
#line 1489 "language.y"
{ yyval.unaryN = new unaryNode('~', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 180:
#line 1490 "language.y"
{ yyval.unaryN = new unaryNode('!', (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 182:
#line 1496 "language.y"
{ yyval.exprN = new castNode(yyvsp[-2].typeN.ty, yyvsp[0].exprN, false, Coord(yyvsp[-3].tok)); ;
    break;}
case 184:
#line 1502 "language.y"
{ yyval.exprN = new binaryNode('*', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 185:
#line 1505 "language.y"
{ yyval.exprN = new binaryNode('/', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 186:
#line 1508 "language.y"
{ yyval.exprN = new binaryNode('%', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 188:
#line 1514 "language.y"
{ yyval.exprN = new binaryNode('+', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 189:
#line 1517 "language.y"
{ yyval.exprN = new binaryNode('-', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 191:
#line 1523 "language.y"
{ yyval.exprN = new binaryNode( Operator::LS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 192:
#line 1526 "language.y"
{ yyval.exprN = new binaryNode( Operator::RS, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 194:
#line 1532 "language.y"
{ yyval.exprN = new binaryNode('<', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 195:
#line 1535 "language.y"
{ yyval.exprN = new binaryNode('>', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 196:
#line 1538 "language.y"
{ yyval.exprN = new binaryNode( Operator::LE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 197:
#line 1541 "language.y"
{ yyval.exprN = new binaryNode( Operator::GE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 199:
#line 1547 "language.y"
{ yyval.exprN = new binaryNode( Operator::EQ, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 200:
#line 1550 "language.y"
{ yyval.exprN = new binaryNode( Operator::NE, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 202:
#line 1556 "language.y"
{ yyval.exprN = new binaryNode('&', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 204:
#line 1562 "language.y"
{ CBZ::WarnAboutPrecedence('^', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('^', yyvsp[0].exprN);
	      yyval.exprN = new binaryNode('^', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 206:
#line 1570 "language.y"
{ CBZ::WarnAboutPrecedence('|', yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence('|', yyvsp[0].exprN);
              yyval.exprN = new binaryNode('|', yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 208:
#line 1578 "language.y"
{ yyval.exprN = new binaryNode( Operator::ANDAND, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 210:
#line 1584 "language.y"
{ CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[-2].exprN);
              CBZ::WarnAboutPrecedence(ctokOROR, yyvsp[0].exprN);
              yyval.exprN = new binaryNode( Operator::OROR, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-1].tok)); ;
    break;}
case 212:
#line 1592 "language.y"
{ yyval.exprN = new ternaryNode(yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].exprN, Coord(yyvsp[-3].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 214:
#line 1598 "language.y"
{ yyvsp[-1].binaryN->left(yyvsp[-2].exprN);
	      yyvsp[-1].binaryN->right(yyvsp[0].exprN);
	      yyval.exprN = yyvsp[-1].binaryN; ;
    break;}
case 215:
#line 1605 "language.y"
{ yyval.binaryN = new binaryNode('=', (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 216:
#line 1608 "language.y"
{ yyval.binaryN = new binaryNode( Operator::MULTassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  ;
    break;}
case 217:
#line 1611 "language.y"
{ yyval.binaryN = new binaryNode( Operator::DIVassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 218:
#line 1614 "language.y"
{ yyval.binaryN = new binaryNode( Operator::MODassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 219:
#line 1617 "language.y"
{ yyval.binaryN = new binaryNode( Operator::PLUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));  ;
    break;}
case 220:
#line 1620 "language.y"
{ yyval.binaryN = new binaryNode( Operator::MINUSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok)); ;
    break;}
case 221:
#line 1623 "language.y"
{ yyval.binaryN = new binaryNode( Operator::LSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 222:
#line 1626 "language.y"
{ yyval.binaryN = new binaryNode( Operator::RSassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 223:
#line 1629 "language.y"
{ yyval.binaryN = new binaryNode( Operator::ANDassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));   ;
    break;}
case 224:
#line 1632 "language.y"
{ yyval.binaryN = new binaryNode( Operator::ERassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 225:
#line 1635 "language.y"
{ yyval.binaryN = new binaryNode( Operator::ORassign, (exprNode *)0, (exprNode *)0, Coord(yyvsp[0].tok));    ;
    break;}
case 226:
#line 1640 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 227:
#line 1642 "language.y"
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
case 228:
#line 1656 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 229:
#line 1660 "language.y"
{ yyval.exprN = (exprNode *) 0; ;
    break;}
case 230:
#line 1661 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 231:
#line 1698 "language.y"
{ yyval.declL = yyvsp[-1].declL; ;
    break;}
case 232:
#line 1701 "language.y"
{ yyval.declL = yyvsp[-1].declL; ;
    break;}
case 233:
#line 1705 "language.y"
{ yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); ;
    break;}
case 234:
#line 1711 "language.y"
{ yyval.declL = new decl_list();
	      ((sueNode *)yyvsp[-1].typeN.ty)->force_new_and(Coord(yyvsp[0].tok));
	      yyval.declL->push_back(new declNode(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc)); ;
    break;}
case 235:
#line 1719 "language.y"
{ yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); ;
    break;}
case 236:
#line 1721 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 237:
#line 1723 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN));
	    ;
    break;}
case 238:
#line 1728 "language.y"
{ yyvsp[0].declN->set_type(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl); ;
    break;}
case 239:
#line 1730 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 240:
#line 1732 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 241:
#line 1736 "language.y"
{ yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); ;
    break;}
case 242:
#line 1738 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 243:
#line 1740 "language.y"
{ yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); ;
    break;}
case 244:
#line 1746 "language.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 245:
#line 1749 "language.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 246:
#line 1753 "language.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 247:
#line 1756 "language.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 249:
#line 1765 "language.y"
{ yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); ;
    break;}
case 250:
#line 1767 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 251:
#line 1769 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 252:
#line 1773 "language.y"
{ yyvsp[0].declN->set_type(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, NoRedecl); ;
    break;}
case 253:
#line 1775 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 254:
#line 1777 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[-4].declN->set_init_and(yyvsp[0].exprN)); ;
    break;}
case 255:
#line 1781 "language.y"
{ yyvsp[-2].declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL,NoRedecl)); ;
    break;}
case 256:
#line 1783 "language.y"
{ yyvsp[-2].declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 257:
#line 1785 "language.y"
{ yyval.declL = yyvsp[-6].declL;
	      yyvsp[-4].declN->set_init(yyvsp[0].exprN); ;
    break;}
case 258:
#line 1791 "language.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 259:
#line 1794 "language.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 260:
#line 1798 "language.y"
{ CBZ::SyntaxError(string("declaration without a variable")); ;
    break;}
case 261:
#line 1801 "language.y"
{ yyval.declL = (decl_list *)0; /* empty list */ ;
    break;}
case 263:
#line 1809 "language.y"
{ primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and(); 
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 264:
#line 1814 "language.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 265:
#line 1817 "language.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 266:
#line 1824 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); ;
    break;}
case 267:
#line 1828 "language.y"
{ yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; ;
    break;}
case 268:
#line 1833 "language.y"
{ yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 269:
#line 1837 "language.y"
{ yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN); 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 270:
#line 1845 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty;
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, yyvsp[-1].typeN.ty->coord()); ;
    break;}
case 271:
#line 1849 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 272:
#line 1854 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 273:
#line 1862 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 274:
#line 1866 "language.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); 
	      yyval.typeN.sc = yyvsp[-1].tq.sc; ;
    break;}
case 275:
#line 1871 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 277:
#line 1880 "language.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 278:
#line 1883 "language.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 281:
#line 1895 "language.y"
{ primNode * p = (primNode *) yyvsp[0].typeN.ty;
	      yyval.typeN.ty = p->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc;;
    break;}
case 282:
#line 1900 "language.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 284:
#line 1905 "language.y"
{ yyval.typeN = yyvsp[0].typeN; ;
    break;}
case 285:
#line 1911 "language.y"
{ yyval.typeN.ty = yyvsp[0].primN;  
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 286:
#line 1915 "language.y"
{ yyval.typeN.ty = (primNode *) yyvsp[0].primN->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 287:
#line 1920 "language.y"
{ yyval.typeN.ty = (primNode *) yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 288:
#line 1924 "language.y"
{ yyval.typeN.ty = ((primNode *)yyvsp[-1].typeN.ty)->merge_in_and(yyvsp[0].primN);
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 289:
#line 1931 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty; 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 290:
#line 1935 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->add_type_qualifiers_and(yyvsp[-1].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord));
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 291:
#line 1940 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 292:
#line 1948 "language.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, typeNode::NONE, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 293:
#line 1952 "language.y"
{ yyval.typeN.ty = new tdefNode(yyvsp[0].idN, yyvsp[-1].tq.tq, yyvsp[0].idN->decl()->type());
	      yyval.typeN.sc = yyvsp[-1].tq.sc;
	      yyval.typeN.ty->coord(Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 294:
#line 1957 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty->add_type_qualifiers_and(yyvsp[0].tq.tq);
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].tq.sc, Coord(yyvsp[0].tq.coord)); ;
    break;}
case 296:
#line 1965 "language.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 298:
#line 1971 "language.y"
{ yyval.tq = merge_tq(yyvsp[-1].tq, yyvsp[0].tq); ;
    break;}
case 299:
#line 1977 "language.y"
{ yyval.typeN.ty = yyvsp[0].sueN; 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 300:
#line 1981 "language.y"
{ yyval.typeN.ty = yyvsp[0].enumN;  
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 304:
#line 1991 "language.y"
{
	      CBZ::Warning(2, string("function prototype parameters must have types"));
              /* SZG Don't need this? $$ = AddDefaultParameterTypes($1); */
            ;
    break;}
case 306:
#line 2003 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); ;
    break;}
case 307:
#line 2006 "language.y"
{ yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(typeNode::NONE,(typeNode *)0, Coord(yyvsp[-3].tok)), declNode::NONE, Redecl); ;
    break;}
case 308:
#line 2009 "language.y"
{ yyval.declN = yyvsp[-1].declN->set_type_and(new ptrNode(yyvsp[-3].tq.tq,(typeNode *)0, Coord(yyvsp[-4].tok)), yyvsp[-3].tq.sc, Redecl); ;
    break;}
case 309:
#line 2012 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq,(typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); ;
    break;}
case 310:
#line 2019 "language.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 311:
#line 2022 "language.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[-1].typeN.ty); ;
    break;}
case 312:
#line 2025 "language.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 313:
#line 2031 "language.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 314:
#line 2035 "language.y"
{ yyval.declN = yyvsp[-1].declN;  ;
    break;}
case 315:
#line 2041 "language.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 316:
#line 2045 "language.y"
{ yyval.declN = new declNode(yyvsp[-1].idN, declNode::NONE, yyvsp[0].typeN.ty, (exprNode *)0, (exprNode *)0); ;
    break;}
case 319:
#line 2059 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, Redecl); ;
    break;}
case 320:
#line 2062 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), yyvsp[-1].tq.sc, Redecl); ;
    break;}
case 321:
#line 2068 "language.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 322:
#line 2071 "language.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 326:
#line 2084 "language.y"
{ yyval.typeN.ty = new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[0].tok)); 
	      yyval.typeN.sc = declNode::NONE; ;
    break;}
case 327:
#line 2088 "language.y"
{ yyval.typeN.ty = new ptrNode(yyvsp[0].tq.tq, (typeNode *)0, Coord(yyvsp[-1].tok));  
	      yyval.typeN.sc = yyvsp[0].tq.sc; ;
    break;}
case 328:
#line 2092 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)));   
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 329:
#line 2096 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)));   
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 330:
#line 2103 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 331:
#line 2107 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 332:
#line 2111 "language.y"
{ yyval.typeN.ty = yyvsp[-1].typeN.ty; 
	      yyval.typeN.sc = yyvsp[-1].typeN.sc; ;
    break;}
case 333:
#line 2115 "language.y"
{ yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(yyvsp[0].typeN.ty); 
	      yyval.typeN.sc = merge_sc(yyvsp[-2].typeN.sc, yyvsp[0].typeN.sc, yyvsp[-2].typeN.ty->coord()); ;
    break;}
case 334:
#line 2122 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty;
	       yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 335:
#line 2126 "language.y"
{ yyval.typeN.ty = new funcNode(typeNode::NONE, (decl_list *)0, (typeNode *)0, Coord(yyvsp[-1].tok)); 
	       yyval.typeN.sc = declNode::NONE; ;
    break;}
case 336:
#line 2130 "language.y"
{ yyval.typeN.ty = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok)); 
	       yyval.typeN.sc = declNode::NONE; ;
    break;}
case 337:
#line 2137 "language.y"
{ yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, (exprNode *)0, Coord(yyvsp[-1].tok));
              yyval.typeN.sc = declNode::NONE; ;
    break;}
case 338:
#line 2141 "language.y"
{ yyval.typeN.ty = new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok)); 
              yyval.typeN.sc = declNode::NONE; ;
    break;}
case 339:
#line 2145 "language.y"
{ yyval.typeN.ty = yyvsp[-3].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0, yyvsp[-1].exprN, Coord(yyvsp[-2].tok))); 
              yyval.typeN.sc = yyvsp[-3].typeN.sc; ;
    break;}
case 340:
#line 2151 "language.y"
{ 
              CBZ::SyntaxError(string("array declaration with illegal empty dimension"));
              yyval.typeN.ty = yyvsp[-2].typeN.ty->set_base_type_and(new arrayNode(typeNode::NONE, (typeNode *)0,
							     (exprNode *) new constNode(constant((int)1)),
							     Coord(yyvsp[-1].tok)));
	      yyval.typeN.sc = declNode::NONE;
            ;
    break;}
case 344:
#line 2171 "language.y"
{ yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok))); ;
    break;}
case 345:
#line 2174 "language.y"
{ yyval.declN = yyvsp[0].declN->modify_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok))); ;
    break;}
case 346:
#line 2180 "language.y"
{ yyval.declN = yyvsp[-1].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 347:
#line 2183 "language.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 348:
#line 2186 "language.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 349:
#line 2192 "language.y"
{ yyval.declN = new declNode(yyvsp[0].idN, declNode::NONE, (typeNode *)0, 
				(exprNode *)0, (exprNode *)0); ;
    break;}
case 350:
#line 2195 "language.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 351:
#line 2201 "language.y"
{ 
/*              OldStyleFunctionDefinition = TRUE; */
              yyval.declN = yyvsp[0].declN; 
            ;
    break;}
case 352:
#line 2207 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(typeNode::NONE, (typeNode *)0, Coord(yyvsp[-1].tok)), declNode::NONE, SU); ;
    break;}
case 353:
#line 2210 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new ptrNode(yyvsp[-1].tq.tq, (typeNode *)0, Coord(yyvsp[-2].tok)), declNode::NONE, SU); ;
    break;}
case 354:
#line 2216 "language.y"
{ funcNode *f = new funcNode(typeNode::NONE, yyvsp[-1].declL, (typeNode *)0, Coord(yyvsp[-2].tok));
	      f->is_knr (true);
	      yyval.declN = yyvsp[-3].declN->modify_type_and(f); ;
    break;}
case 355:
#line 2221 "language.y"
{ yyval.declN = yyvsp[-1].declN; ;
    break;}
case 356:
#line 2224 "language.y"
{ yyval.declN = yyvsp[-2].declN->modify_type_and(yyvsp[0].typeN.ty); ;
    break;}
case 357:
#line 2239 "language.y"
{ yyval.declL = new decl_list();
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    ;
    break;}
case 358:
#line 2248 "language.y"
{ yyval.declL = yyvsp[-2].declL;
	      declNode * d = new declNode(yyvsp[0].idN, declNode::NONE,
					 (typeNode *) 0,
					 (exprNode *) 0, (exprNode *) 0);
	      d->decl_location(declNode::FORMAL);
	      yyval.declL->push_back(d);
	    ;
    break;}
case 361:
#line 2266 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->finish_and();
	      yyval.typeN.sc = yyvsp[0].typeN.sc; ;
    break;}
case 362:
#line 2270 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty)->finish_and(); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord()); ;
    break;}
case 363:
#line 2274 "language.y"
{ yyval.typeN.ty = new primNode(yyvsp[0].tq.tq); 
	      yyval.typeN.sc = yyvsp[0].tq.sc; ;
    break;}
case 364:
#line 2278 "language.y"
{ yyval.typeN.ty = yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)); 
	      yyval.typeN.sc = merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord)); ;
    break;}
case 365:
#line 2287 "language.y"
{ yyval.attribL = (attrib_list *)0; ;
    break;}
case 366:
#line 2289 "language.y"
{ yyval.attribL = yyvsp[0].attribL; ;
    break;}
case 367:
#line 2294 "language.y"
{ yyval.attribL = yyvsp[0].attribL; ;
    break;}
case 368:
#line 2296 "language.y"
{ yyval.attribL = yyvsp[-1].attribL; 
		  yyvsp[-1].attribL->splice(yyvsp[-1].attribL->end(), * yyvsp[0].attribL); 
		  delete yyvsp[0].attribL; ;
    break;}
case 369:
#line 2303 "language.y"
{ if (CBZ::ANSIOnly)
	            CBZ::SyntaxError(string("__attribute__ not allowed with -ansi switch"));
                  yyval.attribL = yyvsp[-2].attribL; ;
    break;}
case 370:
#line 2310 "language.y"
{ yyval.attribL = new attrib_list(); yyval.attribL->push_back(yyvsp[0].attribN); ;
    break;}
case 371:
#line 2312 "language.y"
{ yyval.attribL = yyvsp[-2].attribL; yyval.attribL->push_back(yyvsp[0].attribN); ;
    break;}
case 372:
#line 2317 "language.y"
{ yyval.attribN = 0; ;
    break;}
case 373:
#line 2319 "language.y"
{ yyval.attribN = new attribNode(yyvsp[0].idN, (exprNode *)0); ;
    break;}
case 374:
#line 2321 "language.y"
{ yyval.attribN = new attribNode(yyvsp[-3].idN, yyvsp[-1].exprN); ;
    break;}
case 377:
#line 2328 "language.y"
{ yyval.idN = new idNode("const", Coord(yyvsp[0].tok)); ;
    break;}
case 378:
#line 2333 "language.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 379:
#line 2334 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 380:
#line 2339 "language.y"
{ yyval.exprN = yyvsp[-1].initializerN; yyval.exprN->coord(yyvsp[-2].tok); ;
    break;}
case 381:
#line 2340 "language.y"
{ yyval.exprN = yyvsp[-2].initializerN; yyval.exprN->coord(yyvsp[-3].tok); ;
    break;}
case 382:
#line 2341 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 383:
#line 2347 "language.y"
{ yyval.initializerN = new initializerNode(new expr_list(), yyvsp[0].exprN->coord());
	      yyval.initializerN->add_expr(yyvsp[0].exprN); ;
    break;}
case 384:
#line 2351 "language.y"
{ yyval.initializerN = yyvsp[-2].initializerN;
	      yyval.initializerN->add_expr(yyvsp[0].exprN);
            ;
    break;}
case 386:
#line 2361 "language.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(new declNode((typeNode *) new primNode(basic_type::Ellipsis), declNode::NONE)); ;
    break;}
case 387:
#line 2366 "language.y"
{ /* primNode * n = new primNode(qualifier($1), basic_type::Void); */
	      CBZ::SyntaxError(Coord(yyvsp[0].tok), string("First argument cannot be `...'"));
              yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(new primNode(typeNode::NONE, basic_type::Void), declNode::NONE));
            ;
    break;}
case 388:
#line 2376 "language.y"
{ yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[0].declN); ;
    break;}
case 389:
#line 2380 "language.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN); ;
    break;}
case 390:
#line 2385 "language.y"
{ 
	      CBZ::SyntaxError(yyvsp[-2].declN->coord(), string("formals cannot have initializers"));
	      yyval.declL = new decl_list(); 
	      yyval.declL->push_back(yyvsp[-2].declN);
            ;
    break;}
case 391:
#line 2392 "language.y"
{ yyval.declL = yyvsp[-2].declL; ;
    break;}
case 392:
#line 2398 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); ;
    break;}
case 393:
#line 2401 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); ;
    break;}
case 394:
#line 2405 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 395:
#line 2408 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 396:
#line 2411 "language.y"
{ yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); ;
    break;}
case 397:
#line 2414 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); ;
    break;}
case 398:
#line 2418 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); ;
    break;}
case 399:
#line 2421 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty, yyvsp[0].typeN.sc); ;
    break;}
case 400:
#line 2424 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(yyvsp[-1].typeN.ty),
				merge_sc(yyvsp[-1].typeN.sc, yyvsp[0].typeN.sc, yyvsp[0].typeN.ty->coord())); ;
    break;}
case 401:
#line 2428 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 402:
#line 2431 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Formal); ;
    break;}
case 403:
#line 2434 "language.y"
{ yyval.declN = new declNode(new primNode(yyvsp[0].tq.tq), yyvsp[0].tq.sc); ;
    break;}
case 404:
#line 2437 "language.y"
{ yyval.declN = new declNode(yyvsp[0].typeN.ty->set_base_type_and(new primNode(yyvsp[-1].tq.tq)),
				merge_sc(yyvsp[-1].tq.sc, yyvsp[0].typeN.sc, Coord(yyvsp[-1].tq.coord))); ;
    break;}
case 405:
#line 2441 "language.y"
{ yyval.declN = yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Formal); ;
    break;}
case 406:
#line 2454 "language.y"
{ yyval.sueN = yyvsp[-3].sueN->set_name_fields_and((idNode *)0, yyvsp[-1].declL, true, yyvsp[-2].tok, yyvsp[0].tok); ;
    break;}
case 407:
#line 2458 "language.y"
{ yyval.sueN = yyvsp[-4].sueN->set_name_fields_and(yyvsp[-3].idN, yyvsp[-1].declL, true, yyvsp[-2].tok, yyvsp[0].tok); ;
    break;}
case 408:
#line 2461 "language.y"
{ yyval.sueN = yyvsp[-1].sueN->set_name_and(yyvsp[0].idN, yyvsp[-1].sueN->coord()); ;
    break;}
case 409:
#line 2465 "language.y"
{ 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-2].sueN->set_name_fields_and((idNode *)0, (decl_list *)0, true, yyvsp[-1].tok, yyvsp[0].tok);
	    ;
    break;}
case 410:
#line 2472 "language.y"
{ 
              if (CBZ::ANSIOnly)
                 CBZ::Warning(1, string("empty structure declaration"));
	      yyval.sueN = yyvsp[-3].sueN->set_name_fields_and(yyvsp[-2].idN, (decl_list *)0, true, yyvsp[-1].tok, yyvsp[0].tok);
	    ;
    break;}
case 411:
#line 2481 "language.y"
{ yyval.sueN = new structNode(Coord(yyvsp[0].tok)); ;
    break;}
case 412:
#line 2482 "language.y"
{ yyval.sueN = new unionNode(Coord(yyvsp[0].tok)); ;
    break;}
case 414:
#line 2489 "language.y"
{ yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyval.declL->end(), * yyvsp[0].declL);
	      delete yyvsp[0].declL; ;
    break;}
case 417:
#line 2504 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, SU)); ;
    break;}
case 418:
#line 2508 "language.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); ;
    break;}
case 419:
#line 2515 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, SU)); ;
    break;}
case 420:
#line 2519 "language.y"
{ yyval.declL = yyvsp[-2].declL;
	      yyval.declL->push_back(yyvsp[0].declN->inherit_type_and(yyvsp[-2].declL, SU)); ;
    break;}
case 421:
#line 2527 "language.y"
{ yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 422:
#line 2532 "language.y"
{ yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 423:
#line 2540 "language.y"
{ yyval.declN = yyvsp[-2].declN;
	      yyval.declN->bitsize(yyvsp[-1].exprN);
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 424:
#line 2545 "language.y"
{ yyval.declN = new declNode("", declNode::NONE, 
				(typeNode *)0, (exprNode *)0, yyvsp[-1].exprN, yyvsp[-1].exprN->coord());
              yyval.declN->merge_attribs(yyvsp[0].attribL); ;
    break;}
case 425:
#line 2552 "language.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 427:
#line 2558 "language.y"
{ yyval.exprN = yyvsp[0].exprN; ;
    break;}
case 428:
#line 2564 "language.y"
{ yyval.enumN = new enumNode((idNode *)0, yyvsp[-2].declL, yyvsp[-4].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 429:
#line 2567 "language.y"
{ yyval.enumN = new enumNode(yyvsp[-4].idN, yyvsp[-2].declL, yyvsp[-5].tok, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok));   ;
    break;}
case 430:
#line 2570 "language.y"
{ yyval.enumN = new enumNode(yyvsp[0].idN, (decl_list *)0, yyvsp[-1].tok, yyvsp[0].idN->coord(), yyvsp[0].idN->coord()); ;
    break;}
case 431:
#line 2576 "language.y"
{ yyval.declL = new decl_list();
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); ;
    break;}
case 432:
#line 2580 "language.y"
{ yyval.declL = yyvsp[-3].declL; 
	      yyval.declL->push_back(new declNode(yyvsp[-1].idN, yyvsp[0].exprN)); ;
    break;}
case 433:
#line 2586 "language.y"
{ yyval.exprN = (exprNode *)0; ;
    break;}
case 434:
#line 2587 "language.y"
{ yyval.exprN = yyvsp[0].exprN;   ;
    break;}
case 435:
#line 2591 "language.y"
{ ;
    break;}
case 436:
#line 2592 "language.y"
{ ;
    break;}
case 437:
#line 2603 "language.y"
{ yyval.stmtN = yyvsp[0].targetN; ;
    break;}
case 438:
#line 2606 "language.y"
{ yyval.stmtN = yyvsp[0].blockN; ;
    break;}
case 439:
#line 2609 "language.y"
{ yyval.stmtN = yyvsp[0].exprstmtN; ;
    break;}
case 440:
#line 2612 "language.y"
{ yyval.stmtN = yyvsp[0].stmtN; ;
    break;}
case 441:
#line 2615 "language.y"
{ yyval.stmtN = yyvsp[0].loopN; ;
    break;}
case 442:
#line 2618 "language.y"
{ yyval.stmtN = yyvsp[0].jumpN; ;
    break;}
case 443:
#line 2629 "language.y"
{  yyval.stmtN = (stmtNode *) 0; ;
    break;}
case 444:
#line 2631 "language.y"
{ yyval.stmtN = yyvsp[-1].stmtN; ;
    break;}
case 445:
#line 2637 "language.y"
{ yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); ;
    break;}
case 446:
#line 2640 "language.y"
{ yyval.targetN = new caseNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-3].tok)); ;
    break;}
case 447:
#line 2643 "language.y"
{ yyval.targetN = new caseNode(0, yyvsp[0].stmtN, Coord(yyvsp[-2].tok)); ;
    break;}
case 448:
#line 2647 "language.y"
{ yyval.targetN = new labelNode(yyvsp[-2].idN, yyvsp[0].stmtN); ;
    break;}
case 449:
#line 2653 "language.y"
{ yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 450:
#line 2656 "language.y"
{ yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 451:
#line 2659 "language.y"
{ yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 452:
#line 2662 "language.y"
{ yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 453:
#line 2666 "language.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->enter_scope(); ;
    break;}
case 454:
#line 2670 "language.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_scope(); ;
    break;}
case 455:
#line 2674 "language.y"
{ yyval.tok =yyvsp[0].tok; CBZ::current_unit->enter_pragmas(); ;
    break;}
case 456:
#line 2676 "language.y"
{ yyval.tok = yyvsp[0].tok; CBZ::current_unit->exit_pragmas(); ;
    break;}
case 457:
#line 2684 "language.y"
{ yyval.blockN = new blockNode((decl_list *)0, (stmt_list *)0, Coord(yyvsp[-1].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 458:
#line 2687 "language.y"
{ yyval.blockN = new blockNode(yyvsp[-1].declL, (stmt_list *)0, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 459:
#line 2690 "language.y"
{ yyval.blockN = new blockNode((decl_list *)0, yyvsp[-1].stmtL, Coord(yyvsp[-2].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 460:
#line 2693 "language.y"
{ yyval.blockN = new blockNode(yyvsp[-2].declL, yyvsp[-1].stmtL, Coord(yyvsp[-3].tok), Coord(yyvsp[0].tok)); ;
    break;}
case 461:
#line 2699 "language.y"
{ yyval.declL = yyvsp[0].declL; ;
    break;}
case 462:
#line 2702 "language.y"
{ yyval.declL = yyvsp[-1].declL;
	      yyval.declL->splice(yyvsp[-1].declL->end(), * yyvsp[0].declL); 
	      delete yyvsp[0].declL;
	    ;
    break;}
case 463:
#line 2711 "language.y"
{ yyval.stmtL = new stmt_list();
              yyval.stmtL->push_back(yyvsp[0].stmtN); ;
    break;}
case 464:
#line 2715 "language.y"
{ yyval.stmtL = yyvsp[-1].stmtL;
              yyval.stmtL->push_back(yyvsp[0].stmtN); ;
    break;}
case 465:
#line 2725 "language.y"
{ yyval.exprstmtN = new exprstmtNode(yyvsp[-1].exprN); ;
    break;}
case 466:
#line 2730 "language.y"
{ yyval.stmtN = new ifNode(yyvsp[-2].exprN, yyvsp[0].stmtN, 0, Coord(yyvsp[-4].tok)); ;
    break;}
case 467:
#line 2733 "language.y"
{ yyval.stmtN = new ifNode (yyvsp[-4].exprN, yyvsp[-2].stmtN, yyvsp[0].stmtN, Coord(yyvsp[-6].tok), Coord(yyvsp[-1].tok)); ;
    break;}
case 468:
#line 2736 "language.y"
{ yyval.stmtN = new switchNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 469:
#line 2741 "language.y"
{ yyval.loopN = new whileNode(yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 470:
#line 2744 "language.y"
{ yyval.loopN = new doNode(yyvsp[-5].stmtN, yyvsp[-2].exprN, Coord(yyvsp[-6].tok), Coord(yyvsp[-4].tok)); ;
    break;}
case 471:
#line 2747 "language.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 472:
#line 2752 "language.y"
{ yyval.loopN = new forNode((exprNode *)0, yyvsp[-4].exprN, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 473:
#line 2755 "language.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, yyvsp[-4].exprN, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 474:
#line 2758 "language.y"
{ yyval.loopN = new forNode(yyvsp[-6].exprN, (exprNode *)0, yyvsp[-2].exprN, yyvsp[0].stmtN, Coord(yyvsp[-8].tok)); ;
    break;}
case 475:
#line 2761 "language.y"
{ yyval.loopN = new forNode((exprNode *)0, (exprNode *)0, (exprNode *)0, yyvsp[0].stmtN, Coord(yyvsp[-4].tok)); ;
    break;}
case 476:
#line 2766 "language.y"
{ yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); ;
    break;}
case 477:
#line 2769 "language.y"
{ yyval.jumpN = new continueNode(Coord(yyvsp[-1].tok)); ;
    break;}
case 478:
#line 2772 "language.y"
{ yyval.jumpN = new breakNode(Coord(yyvsp[-1].tok)); ;
    break;}
case 479:
#line 2775 "language.y"
{ yyval.jumpN = new returnNode(yyvsp[-1].exprN, procNode::current(), Coord(yyvsp[-2].tok)); ;
    break;}
case 480:
#line 2779 "language.y"
{ yyval.jumpN = new gotoNode(yyvsp[-1].idN, Coord(yyvsp[-2].tok)); ;
    break;}
case 481:
#line 2790 "language.y"
{ if (yyvsp[0].defL != 0) CBZ::current_unit->merge_in(yyvsp[0].defL); ;
    break;}
case 482:
#line 2793 "language.y"
{ CBZ::current_unit->merge_in(yyvsp[0].defL); ;
    break;}
case 483:
#line 2798 "language.y"
{
              yyval.defL = new def_list();
	      while (! yyvsp[0].declL->empty()) {
		defNode * d = yyvsp[0].declL->front();
                d->grab_pragmas();
		yyvsp[0].declL->pop_front();
		yyval.defL->push_front(d);
	      }
	      delete yyvsp[0].declL;
            ;
    break;}
case 484:
#line 2809 "language.y"
{ 
              yyvsp[0].procN->grab_pragmas();
              yyval.defL = new def_list();
	      yyval.defL->push_back(yyvsp[0].procN);
            ;
    break;}
case 485:
#line 2819 "language.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); ;
    break;}
case 486:
#line 2821 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 487:
#line 2827 "language.y"
{ yyval.procN = new procNode(false, yyvsp[-1].declN); ;
    break;}
case 488:
#line 2830 "language.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); ;
    break;}
case 489:
#line 2832 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 490:
#line 2837 "language.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)); ;
    break;}
case 491:
#line 2839 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 492:
#line 2842 "language.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)); ;
    break;}
case 493:
#line 2844 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 494:
#line 2846 "language.y"
{ yyval.procN = new procNode(false, yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)); ;
    break;}
case 495:
#line 2848 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 496:
#line 2852 "language.y"
{ yyval.procN = new procNode(true, yyvsp[0].declN->set_type_and(new primNode(yyvsp[0].declN->coord()), yyvsp[0].declN->storage_class(), Redecl)); ;
    break;}
case 497:
#line 2854 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 498:
#line 2857 "language.y"
{  yyval.procN = new procNode(true, 
				 yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				     add_parameter_types_and((decl_list *)0)); ;
    break;}
case 499:
#line 2861 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 500:
#line 2864 "language.y"
{ yyval.procN = new procNode(true, 
				yyvsp[0].declN->set_type_and(yyvsp[-1].typeN.ty, yyvsp[-1].typeN.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); ;
    break;}
case 501:
#line 2868 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 502:
#line 2871 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0));
            ;
    break;}
case 503:
#line 2876 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 504:
#line 2879 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[0].declN->set_type_and(new primNode(yyvsp[-1].tq.tq), yyvsp[-1].tq.sc, Redecl)->
				    add_parameter_types_and((decl_list *)0)); ;
    break;}
case 505:
#line 2883 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 506:
#line 2886 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-1].declN->coord()), declNode::NONE, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 507:
#line 2890 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 508:
#line 2893 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 509:
#line 2897 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 510:
#line 2900 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(yyvsp[-2].typeN.ty, yyvsp[-2].typeN.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 511:
#line 2904 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 512:
#line 2907 "language.y"
{ yyval.procN = new procNode(true,
				yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				    add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 513:
#line 2911 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 514:
#line 2914 "language.y"
{ yyval.procN = new procNode(true, yyvsp[-1].declN->set_type_and(new primNode(yyvsp[-2].tq.tq), yyvsp[-2].tq.sc, Redecl)->
				          add_parameter_types_and(yyvsp[0].declL)); ;
    break;}
case 515:
#line 2917 "language.y"
{ yyval.procN = procNode::current()->define_and(yyvsp[0].blockN); ;
    break;}
case 516:
#line 2922 "language.y"
{ CBZ::OldStyleFunctionDefinition = true; ;
    break;}
case 517:
#line 2924 "language.y"
{ CBZ::OldStyleFunctionDefinition = false; 
	       yyval.declL = yyvsp[0].declL; ;
    break;}
case 518:
#line 2939 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 519:
#line 2940 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 520:
#line 2941 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 521:
#line 2942 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 522:
#line 2943 "language.y"
{ yyval.exprN = yyvsp[0].constN; ;
    break;}
case 523:
#line 2948 "language.y"
{ yyval.constN = yyvsp[0].constN; ;
    break;}
case 524:
#line 2950 "language.y"
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
case 525:
#line 2962 "language.y"
{ yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 526:
#line 2963 "language.y"
{ yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 527:
#line 2964 "language.y"
{ yyval.tq.tq = typeNode::INLINE;   yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 528:
#line 2968 "language.y"
{ yyval.tq.tq = typeNode::CONST;    yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 529:
#line 2969 "language.y"
{ yyval.tq.tq = typeNode::VOLATILE; yyval.tq.sc = declNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 530:
#line 2973 "language.y"
{ yyval.tq.sc = declNode::TYPEDEF;  yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 531:
#line 2974 "language.y"
{ yyval.tq.sc = declNode::EXTERN;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 532:
#line 2975 "language.y"
{ yyval.tq.sc = declNode::STATIC;   yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 533:
#line 2976 "language.y"
{ yyval.tq.sc = declNode::AUTO;     yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 534:
#line 2977 "language.y"
{ yyval.tq.sc = declNode::REGISTER; yyval.tq.tq = typeNode::NONE; yyval.tq.coord = yyvsp[0].tok; ;
    break;}
case 535:
#line 2981 "language.y"
{ yyval.primN = new primNode(basic_type::Void, Coord(yyvsp[0].tok));    ;
    break;}
case 536:
#line 2982 "language.y"
{ yyval.primN = new primNode(basic_type::Char, Coord(yyvsp[0].tok));    ;
    break;}
case 537:
#line 2983 "language.y"
{ yyval.primN = new primNode(basic_type::Int, Coord(yyvsp[0].tok));     ;
    break;}
case 538:
#line 2984 "language.y"
{ yyval.primN = new primNode(basic_type::Float, Coord(yyvsp[0].tok));   ;
    break;}
case 539:
#line 2985 "language.y"
{ yyval.primN = new primNode(basic_type::Double, Coord(yyvsp[0].tok));  ;
    break;}
case 540:
#line 2987 "language.y"
{ yyval.primN = new primNode(basic_type::Signed, Coord(yyvsp[0].tok));  ;
    break;}
case 541:
#line 2988 "language.y"
{ yyval.primN = new primNode(basic_type::Unsigned, Coord(yyvsp[0].tok));;
    break;}
case 542:
#line 2990 "language.y"
{ yyval.primN = new primNode(basic_type::Short, Coord(yyvsp[0].tok));   ;
    break;}
case 543:
#line 2991 "language.y"
{ yyval.primN = new primNode(basic_type::Long, Coord(yyvsp[0].tok));    ;
    break;}
case 546:
#line 3012 "language.y"
{
	yyval.exprN = new exprPatternNode(yyvsp[0].t_id);
	delete yyvsp[0].t_id;
      ;
    break;}
case 547:
#line 3018 "language.y"
{
	yyval.exprN = new exprPatternNode(yyvsp[-1].t_id);
	delete yyvsp[-1].t_id;
      ;
    break;}
case 548:
#line 3027 "language.y"
{
	yyval.stmtN = new stmtPatternNode(yyvsp[-1].t_id);
	delete yyvsp[-1].t_id;
      ;
    break;}
case 549:
#line 3036 "language.y"
{
	yyval.typeN.ty = new typePatternNode(yyvsp[-1].t_id);
	yyval.typeN.sc = declNode::NONE;
	delete yyvsp[-1].t_id;
      ;
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
#line 3043 "language.y"

