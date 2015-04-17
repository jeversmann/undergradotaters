#!/bin/bash

LLVM_RELEASE_TYPE=Release

echo Building CBZ depdencencies
echo -------------------------------
cd AnnotationPass/cbz
mkdir -p lib
make headers
make
cd ..
#make -f langmake

echo Building pointer engine
echo -------------------------------
cd ..
mkdir -p $LLVM_RELEASE_TYPE/AnnotationPass
mkdir -p $LLVM_RELEASE_TYPE/AnnotationPass/bdwy
mkdir -p $LLVM_RELEASE_TYPE/model
make
