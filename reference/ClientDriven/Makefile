LEVEL = ../../..
LLVMLIBS += LLVMSupport.a
LIBRARYNAME = Broadway
LOADABLE_MODULE = 1

BDWYSOURCES =	AnnotationPass/bdwy/annotations.cc \
	AnnotationPass/language-parser.cc \
	AnnotationPass/language-lexer.cc \
	AnnotationPass/bdwy/annvariable.cc \
	AnnotationPass/bdwy/structure.cc \
	AnnotationPass/bdwy/procedure.cc \
	AnnotationPass/bdwy/enum_property.cc \
	AnnotationPass/bdwy/set_property.cc \
	AnnotationPass/bdwy/analyzer.cc \
	AnnotationPass/bdwy/expr.cc \
	AnnotationPass/bdwy/rule.cc \
	AnnotationPass/bdwy/pointerrule.cc \
	AnnotationPass/bdwy/analyze.cc \
	AnnotationPass/bdwy/propertyanalyzer.cc \
	AnnotationPass/bdwy/report.cc \
	AnnotationPass/bdwy/cpattern.cc \
	AnnotationPass/bdwy/action.cc \
	AnnotationPass/bdwy/actionchanger.cc \
	AnnotationPass/bdwy/bdwy_liveness.cc \
	AnnotationPass/bdwy/unification_ann.cc \
	AnnotationPass/bdwy/adaptor_statistics.cc \
	AnnotationPass/bdwy/diagnostic.cc \


ANNSOURCES = AnnotationPass/AnnotationPass.cc $(BDWYSOURCES) AnnotationPass/ProcedureAnnotationInfo.cpp AnnotationPass/AnnotationAnalysis.cpp

# LLVM build system is, of course, a mess of hacks:
# http://comments.gmane.org/gmane.comp.compilers.llvm.devel/16984
SOURCES = Main_DFA_Algo.cpp model/Location.cpp model/Blocks.cpp model/ProcedureDB.cpp model/Mem_Table.cpp model/Uses.cpp model/Defs.cpp ConstantAnalysis.cpp SplitPass.cpp \
	$(ANNSOURCES)


include $(LEVEL)/Makefile.common

INCLUDE += -IAnnotationPass -IAnnotationPass/cbz/include -IAnnotationPass/bdwy

LIBS += AnnotationPass/cbz/lib/libc-breeze.a

CXXFLAGS += $(INCLUDE)
LDFLAGS += $(LIBDIR) $(LIBS)


# And of course this is only found from a mailing list; CPPFLAGS += ... gets
# ignored, despite what the docs say.
CompileCommonOpts += -Wno-unused -Wno-overloaded-virtual $(INCLUDE)
