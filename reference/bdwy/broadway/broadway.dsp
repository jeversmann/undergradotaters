# Microsoft Developer Studio Project File - Name="broadway" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=broadway - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "broadway.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "broadway.mak" CFG="broadway - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "broadway - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "broadway - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "broadway - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "c-breeze\src\main" /I "c-breeze\src\ast" /I "c-breeze\src\optimizations" /I "c-breeze\src\helpers" /I "c-breeze\src\pointers" /I "c-breeze\src\contrib" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "broadway - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "c-breeze\src\main" /I "c-breeze\src\ast" /I "c-breeze\src\optimizations" /I "c-breeze\src\helpers" /I "c-breeze\src\pointers" /I "c-breeze\src\contrib" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "broadway - Win32 Release"
# Name "broadway - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\action.cpp
# End Source File
# Begin Source File

SOURCE=.\src\actionchanger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\analyze.cpp
# End Source File
# Begin Source File

SOURCE=.\src\analyzer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\annotations.cpp
# End Source File
# Begin Source File

SOURCE=.\src\annvariable.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\arraynode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\ast.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\attribnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\basic_type.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\basicblocknode.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\bdwy.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bdwy_liveness.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\binarynode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\blocknode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\breaknode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\c_breeze.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\callgraph.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\callgraph_walker.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\callingcontext.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\callnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\casenode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\castnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\cfg.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\commanode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\constant.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\constants.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\constnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\constprop.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\continuenode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\coord.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\cpattern.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dataflow.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dead.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\declnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\defnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\defuse.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\dfpreds.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle-expr.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle-flatten.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle-init.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle-loop.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle-selection.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dominancefrontiers.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dominators.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\donode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\enum_value_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\enumnode.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\expr.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\exprnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\exprstmtnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\fornode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\funcnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\goto_label_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\gotonode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\id_lookup_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\idnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\ifnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\init_flowproblem_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\initializernode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\inliner.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\jumpnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\labelnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\language-lexer.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\language-parser.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\lex.cb.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\linker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\live.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\liveness.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\localcopyprop.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\location.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\loopnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\loops.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\main.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memoryaccess.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memoryblock.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memorymodel.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\mergepoints.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\meta.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\name_mangle_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\node.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\operators.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\optimize.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\output_context.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\parser.tab.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\phase.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\pointers.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\pointervalue.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\primnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\print_tree_visitor.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\print_walker.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\procedure.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\proceduredb.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\procnode.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\property.cpp
# End Source File
# Begin Source File

SOURCE=.\src\propertyanalyzer.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\ptrnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\reaching.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\reaching_genkill.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\ref_clone_changer.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\report.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\returnnode.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\rule.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\scope_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\selectionnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\semcheck.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\set_container_walker.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\ssa.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\stmtnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\structnode.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\structure.cpp
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\subdeclnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\suenode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\suespecnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\switchnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\symbol.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\targetnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\tdefnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\ternarynode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\textnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\typenode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\unarynode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\unionnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\unitnode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\unreachable.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\vcg.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\vcgast.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\vcgCCG.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\whilenode.cpp"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\worklist.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\action.h
# End Source File
# Begin Source File

SOURCE=.\src\actionchanger.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\allocation.h"
# End Source File
# Begin Source File

SOURCE=.\src\analyze.h
# End Source File
# Begin Source File

SOURCE=.\src\analyzer.h
# End Source File
# Begin Source File

SOURCE=.\src\ann.h
# End Source File
# Begin Source File

SOURCE=.\src\annotations.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\annote.h"
# End Source File
# Begin Source File

SOURCE=.\src\annvariable.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\ast.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\basic_type.h"
# End Source File
# Begin Source File

SOURCE=.\src\bdwy_liveness.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\bits.h"
# End Source File
# Begin Source File

SOURCE=.\src\broadway.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\c_breeze.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\callgraph.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\callgraph_walker.h"
# End Source File
# Begin Source File

SOURCE=.\src\callingcontext.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\cfg.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\changer.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\clone_changer.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\constant.h"
# End Source File
# Begin Source File

SOURCE=.\src\constantprop.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\constants.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\constprop.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\coord.h"
# End Source File
# Begin Source File

SOURCE=.\src\cpattern.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dataflow.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dead.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\defuse.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\df_number_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\dfpreds.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\dismantle.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dominancefrontiers.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\dominators.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\enum_value_walker.h"
# End Source File
# Begin Source File

SOURCE=.\src\expr.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\gc_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\goto_label_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\handle.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\id_lookup_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\init_flowproblem_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\inliner.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\ipanalysis.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\ipconstants.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\ipconstants_changer.h"
# End Source File
# Begin Source File

SOURCE=.\src\language.tab.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\linker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\live.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\liveness.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\localcopyprop.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\location.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\loops.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memoryaccess.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memoryblock.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\memorymodel.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\mergepoints.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\meta.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\name_mangle_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\ast\operators.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\optimize.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\output_context.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\parser.tab.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\path.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\phase.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\pointers.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\pointers_common.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\pointervalue.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\print_tree_visitor.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\main\print_walker.h"
# End Source File
# Begin Source File

SOURCE=.\src\procedure.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\proceduredb.h"
# End Source File
# Begin Source File

SOURCE=.\src\property.h
# End Source File
# Begin Source File

SOURCE=.\src\propertyanalyzer.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\reaching.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\reaching_genkill.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\reaching_getdefs.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\ref_clone_changer.h"
# End Source File
# Begin Source File

SOURCE=.\src\report.h
# End Source File
# Begin Source File

SOURCE=.\src\rule.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\scope_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\semcheck.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\set_container_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\ssa.h"
# End Source File
# Begin Source File

SOURCE=.\src\structure.h
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\sue_complete_walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\symbol.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\tree_visitor.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\optimizations\unreachable.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\contrib\vcg.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\visitor.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\helpers\walker.h"
# End Source File
# Begin Source File

SOURCE=".\c-breeze\src\pointers\worklist.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
