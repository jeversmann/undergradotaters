# Common configuration and flags used for building C-Breeze components.
# This file should NOT contain any rules, just definitions for variables.

SHELL=/bin/sh

# Configuration

# Build-dependent flags

DEBUG=1
PROF=0

# Utilities

CXX     = g++ -fPIC 
LD      = $(CXX)
YACC    = bison
LEX     = flex
AR	    = ar
RANLIB	= ranlib
MAKEDEP = gccmakedep

# System flags

LIBCBZ = ../../lib/libc-breeze.a
INCLUDES= -I. -I../../include

# Optimization flags

ifeq ($(OPT), 1)
CFLAGS = -O4 -D_GNU_SOURCE
endif

ifeq ($(OPT), 2)
CFLAGS = -O4 -fomit-frame-pointer -funroll-loops -D_GNU_SOURCE 
endif

ifeq ($(DEBUG), 1)
CFLAGS = -fPIC -g
endif

# Debug flags

ifeq ($(DEBUG), 2)
CFLAGS = -g -DCBZ_HEAPLAYERS -D_GNU_SOURCE 
endif

ifeq ($(PROF), 1)
CFLAGS = -O0 -fno-default-inline -fno-inline -pg -D_GNU_SOURCE 
endif

ifeq ($(PROF), 2)
CFLAGS = -O4 -pg -D_GNU_SOURCE 
endif

ifeq ($(PROF), 3)
CFLAGS = -O2 -pg -DOLD_DOMINATOR_TEST -D_GNU_SOURCE 
endif

