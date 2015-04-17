SHELL=/bin/sh

# Configuration

CXX     = g++
LD      = $(CXX)
INCLUDES= -I. -I$(CBZHOME)/include
#CFLAGS  = -O
#CFLAGS  = -g
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

OBJ = 	callgraph_walker.o \
	cfg.o \
	constprop.o \
	copy.o \
	copygenkill.o \
	dataflow.o \
	dead.o \
	defuse.o \
	dominancefrontiers.o \
	dominators.o \
	init_flowproblem_walker.o \
	live.o \
	localcopyprop.o \
	loops.o \
	optimize.o \
	reaching.o \
	reaching_genkill.o \
	ssa.o \
	unreachable.o

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
