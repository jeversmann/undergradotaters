#ifndef BISON_PARSER_TAB_HH
# define BISON_PARSER_TAB_HH

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


extern YYSTYPE cblval;

#endif /* not BISON_PARSER_TAB_HH */
