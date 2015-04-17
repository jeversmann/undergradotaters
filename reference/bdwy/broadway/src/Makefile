SHELL=/bin/sh

# Configuration

CXX     = g++
LD      = $(CXX)
INCLUDES= -I. -I$(CBZHOME)/src/pointers/heaplayers-0.2 -I$(CBZHOME)/include
YACC    = bison
LEX     = flex
AR	= ar
RANLIB	= ranlib
LIBCBZ = $(CBZHOME)/lib/libc-breeze.a
MALLOC = 

ifeq ($(OPT), 1)
CFLAGS = -O4
endif

ifeq ($(OPT), 3)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops -DCBZ_HEAPLAYERS
endif

ifeq ($(OPT), 2)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops
endif

ifeq ($(EXP), 1)
CFLAGS = -DOLD_DOMINATOR_TEST -g
endif

ifeq ($(DEBUG), 1)
CFLAGS = -g
endif

ifeq ($(DEBUG), 2)
CFLAGS = -g -DCBZ_HEAPLAYERS
endif

ifeq ($(PROF), 1)
CFLAGS = -O2 -pg
endif

ifeq ($(PROF), 2)
CFLAGS = -O4 -pg
endif

ifeq ($(PROF), 3)
CFLAGS = -O2 -pg -DOLD_DOMINATOR_TEST
endif

# Object code to build

OBJ = 	annotations.o \
	language-parser.o \
	language-lexer.o \
	annvariable.o \
	structure.o \
	procedure.o \
	enum_property.o \
	set_property.o \
	analyzer.o \
	expr.o \
	rule.o \
	pointerrule.o \
	analyze.o \
	propertyanalyzer.o \
	callingcontext.o \
	report.o \
	cpattern.o \
	action.o \
	actionchanger.o \
	bdwy_liveness.o

# Make targets

objects: $(OBJ)

malloc.o: malloc.c malloc.h

clean:
	rm -f *.o *.a *~ *.tab.* language-lexer.cc language-parser.cc

bdwy: $(OBJ) $(MALLOC) bdwy.cc $(LIBCBZ)
	$(CXX) $(INCLUDES) $(CFLAGS) bdwy.cc $(OBJ) $(MALLOC) -o bdwy -L$(CBZHOME)/lib -lc-breeze


language-lexer.cc: language.l
	flex language.l
	mv lex.ann.c language-lexer.cc

language-parser.cc: language.y
	bison -d -pann language.y
	mv language.tab.c language-parser.cc

# Implicit rule for compiling all C++ code

.cc.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

# rebuild the dependencies.  Note: Ignores standard includes on
# purpose, on the assumption that they won't change in between builds,
# and we want to only record dependencies for our files, so that this
# makefile will work on different platforms that store the standard
# headers in different places.  
depend:
	makedepend -Y -- $(CFLAGS) -- $(OBJ:%.o=%.cc)
# DO NOT DELETE

annotations.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
annotations.o: property.h structure.h rule.h report.h action.h analyze.h
annotations.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
annotations.o: cpattern.h actionchanger.h
annvariable.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
annvariable.o: property.h structure.h rule.h report.h action.h analyze.h
annvariable.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
annvariable.o: cpattern.h actionchanger.h
structure.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
structure.o: property.h structure.h rule.h report.h action.h analyze.h
structure.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
structure.o: cpattern.h actionchanger.h
procedure.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
procedure.o: property.h structure.h rule.h report.h action.h analyze.h
procedure.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
procedure.o: cpattern.h actionchanger.h
property.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
property.o: structure.h rule.h report.h action.h analyze.h procedure.h
property.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
property.o: actionchanger.h
analyzer.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
analyzer.o: structure.h rule.h report.h action.h analyze.h procedure.h
analyzer.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
analyzer.o: actionchanger.h
expr.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
expr.o: structure.h rule.h report.h action.h analyze.h procedure.h
expr.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
expr.o: actionchanger.h
rule.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
rule.o: structure.h rule.h report.h action.h analyze.h procedure.h
rule.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
rule.o: actionchanger.h
analyze.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
analyze.o: structure.h rule.h report.h action.h analyze.h procedure.h
analyze.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
analyze.o: actionchanger.h
propertyanalyzer.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
propertyanalyzer.o: property.h structure.h rule.h report.h action.h analyze.h
propertyanalyzer.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
propertyanalyzer.o: cpattern.h actionchanger.h
callingcontext.o: broadway.h ann.h annvariable.h callingcontext.h expr.h
callingcontext.o: property.h structure.h rule.h report.h action.h analyze.h
callingcontext.o: procedure.h annotations.h propertyanalyzer.h analyzer.h
callingcontext.o: cpattern.h actionchanger.h
report.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
report.o: structure.h rule.h report.h action.h analyze.h procedure.h
report.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
report.o: actionchanger.h
cpattern.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
cpattern.o: structure.h rule.h report.h action.h analyze.h procedure.h
cpattern.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
cpattern.o: actionchanger.h
action.o: broadway.h ann.h annvariable.h callingcontext.h expr.h property.h
action.o: structure.h rule.h report.h action.h analyze.h procedure.h
action.o: annotations.h propertyanalyzer.h analyzer.h cpattern.h
action.o: actionchanger.h
