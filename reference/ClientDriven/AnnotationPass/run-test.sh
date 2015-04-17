#!/bin/bash
clang -O0 -S -emit-llvm ${2}
opt -load ../../../../Debug/lib/Broadway.so -load ../../../../Debug/lib/AnnotationPass.so -annotation -pal ${1} < ${2}

