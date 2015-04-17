
include ../../Makefile.inc

# Object code to build

OBJ = 	c_breeze.o \
	main.o \
	output_context.o \
	phase.o \
	print_tree_visitor.o \
	print_walker.o \
	parser.tab.o \
	lexer.cb.o

# Make targets

update: $(LIBCBZ)

$(LIBCBZ): $(OBJ)
	$(AR) r $@ $?
	$(RANLIB) $@

rebuild: $(OBJ)
	$(AR) q $(LIBCBZ) $?

objects: $(OBJ)

#parser.tab.cc: parser.y
#	$(YACC) -d -pcb parser.y -o parser.tab.cc
#	-mv -f parser.tab.cc.h parser.tab.h	# for production CS network bison
#	-mv -f parser.tab.hh parser.tab.h	# for other CS network bison

#lexer.cb.cc: lexer.l parser.tab.cc
#	$(LEX) -Pcb -olexer.cb.cc lexer.l

clean:
	rm -f *.o *.a # *~ parser.tab.* lexer.cb.cc
	rm -f .depend

# Implicit rule for compiling all C++ code

.cc.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

# Additional file for dependencies
ifeq (.depend, $(wildcard .depend))
include .depend
endif

# rebuild the dependencies.  Note: Ignores standard includes on
# purpose, on the assumption that they won't change in between builds,
# and we want to only record dependencies for our files, so that this
# makefile will work on different platforms that store the standard
# headers in different places.  
depend:
	touch .depend
	$(MAKEDEP) -f .depend $(INCLUDES) -- $(CFLAGS) -- $(OBJ:%.o=%.cc)
