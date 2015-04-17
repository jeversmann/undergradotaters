SHELL=/bin/sh

# Configuration

CXX     = g++
LD      = $(CXX)
INCLUDES= -I. -I$(CBZHOME)/include
#CFLAGS  = -O
CFLAGS  = -g
YACC    = bison
LEX     = flex
AR	= ar
RANLIB	= ranlib
LIBCBZ = $(CBZHOME)/lib/libc-breeze.a

# Object code to build

OBJ = 	arch_info.o \
	arch_info_parser.o \
	asm_gen_walker.o \
	briggs_reg_alloc.o \
	dummy_reg_alloc.o \
	instruction.o \
	lir_flow_walker.o \
	lir_gen_walker.o \
	lir_var_type.o \
	LIR.o \
	match.o \
	register.o \
	storage_alloc.o \
	struct_info.o \

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
