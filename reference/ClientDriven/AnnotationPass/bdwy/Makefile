##===- lib/Transforms/Hello/Makefile -----------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../../..

LIBRARYNAME = AnnotationPass

LOADABLE_MODULE = 1

SOURCES = *.cc

include $(LEVEL)/Makefile.common

CBZHOME = $(HOME)/compilers-project/c-breeze

# INCLUDE += -I$(HOME)/compilers-project/broadway/src/ -I$(HOME)/compilers-project/c-breeze.20070515/include/
INCLUDE += -I$(HOME)/compilers-project/c-breeze.20070515/include/
# LIBDIR += -L$(HOME)/compilers-project/broadway/src/lib/libbdwy.a -L$(HOME)/compilers-project/c-breeze.20070515/lib/libc-breeze.a
LIBDIR += -L$(HOME)/compilers-project/c-breeze.20070515/lib/libc-breeze.a
# LIBS += $(HOME)/compilers-project/broadway/src/lib/libbdwy.a $(HOME)/compilers-project/c-breeze.20070515/lib/libc-breeze.a
LIBS += $(HOME)/compilers-project/c-breeze.20070515/lib/libc-breeze.a


CXXFLAGS += $(INCLUDE)
LDFLAGS += $(LIBDIR) $(LIBS)