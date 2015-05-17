# undergradotaters

These directories contain our Off-Broadway project and supporting files.

0. python-parser holds our implementation of a Broadway Annotation Language parser, which depends on grako, a python library. With it installed, run `make run < file >` to parse a file and save a json AST. It tries to give basic error messages where it fails to parse.
0. pass holds our llvm::FunctionPass, and many supporting classes and testing files. The Makefile in the directory will compile BroadwayPass, the primary opt pass for the project, and contains a few different commands to run it on test input in our directory.
