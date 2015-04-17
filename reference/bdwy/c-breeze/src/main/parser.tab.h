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

#ifndef BISON_PARSER_TAB_HH
# define BISON_PARSER_TAB_HH

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
/* Line 1281 of /lusr/gnu/share/bison/yacc.c.  */
#line 284 "parser.tab.hh"
# define YYSTYPE yystype
#endif

extern YYSTYPE cblval;


#endif /* not BISON_PARSER_TAB_HH */

