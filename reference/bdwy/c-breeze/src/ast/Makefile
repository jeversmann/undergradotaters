SHELL=/bin/sh

# Configuration

CXX     = g++
LD      = $(CXX)
INCLUDES= -I. -I$(CBZHOME)/include
#CFLAGS  = -O4
YACC    = bison
LEX     = flex
AR	= ar
RANLIB	= ranlib
LIBCBZ = $(CBZHOME)/lib/libc-breeze.a

ifeq ($(OPT), 1)
CFLAGS = -O4
endif

ifeq ($(OPT), 2)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops
endif

ifeq ($(DEBUG), 1)
CFLAGS = -g
endif

ifeq ($(PROF), 1)
CFLAGS = -O0 -fno-default-inline -fno-inline -pg
endif

ifeq ($(PROF), 2)
CFLAGS = -O4 -pg
endif

# Object code to build

OBJ = 	arraynode.o \
	ast.o \
	attribnode.o \
	basic_type.o \
	basicblocknode.o \
	binarynode.o \
	blocknode.o \
	breaknode.o \
	callnode.o \
	casenode.o \
	castnode.o \
	commanode.o \
	constant.o \
	constnode.o \
	continuenode.o \
	coord.o \
	declnode.o \
	defnode.o \
	donode.o \
	enumnode.o \
	exprnode.o \
	exprstmtnode.o \
	fornode.o \
	funcnode.o \
	gotonode.o \
	idnode.o \
	ifnode.o \
	initializernode.o \
	jumpnode.o \
	labelnode.o \
	loopnode.o \
	node.o \
	operators.o \
	primnode.o \
	procnode.o \
	ptrnode.o \
	returnnode.o \
	selectionnode.o \
	stmtnode.o \
	structnode.o \
	subdeclnode.o \
	suenode.o \
	suespecnode.o \
	switchnode.o \
	targetnode.o \
	tdefnode.o \
	ternarynode.o \
	textnode.o \
	typenode.o \
	unarynode.o \
	unionnode.o \
	unitnode.o \
	whilenode.o

# Make targets

update: $(LIBCBZ)

$(LIBCBZ): $(OBJ)
	$(AR) r $@ $?
	$(RANLIB) $@

rebuild: $(OBJ)
	$(AR) q $(LIBCBZ) $?

objects: $(OBJ)

clean:
	rm -f *.o *.a *~ *.tab.*

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
