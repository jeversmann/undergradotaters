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

#ifndef BISON_LANGUAGE_TAB_H
# define BISON_LANGUAGE_TAB_H

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
/* Line 1281 of /lusr/gnu/share/bison/yacc.c.  */
#line 467 "language.tab.h"
# define YYSTYPE yystype
#endif

extern YYSTYPE annlval;


#endif /* not BISON_LANGUAGE_TAB_H */

