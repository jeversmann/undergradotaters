#ifndef BISON_LANGUAGE_TAB_H
# define BISON_LANGUAGE_TAB_H

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


extern YYSTYPE annlval;

#endif /* not BISON_LANGUAGE_TAB_H */
