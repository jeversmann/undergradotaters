#!/bin/bash

# To be clear, this will kill all .s files you happen to have in your current
# directory

# use to test flow-insensitivity.
# will run flow-sensitive and flow-insensitive versions on all passes
# then you can: 
#  cd logs
#  vimdiff passesx.log_fi passesx.log_fs



#opt="/home/dabreegster/ut/compilers/llvm_build/bin/opt -load /home/dabreegster/ut/compilers/llvm_build/lib/Broadway.so"
opt="opt --load ../../../Release/lib/Broadway.so"

echo "Using: $opt"
echo "If that wasn't what you're expecting, edit $0"
echo
echo
echo

rm -f *.s
clang -O0 -S -emit-llvm tests/*.c
for file in passes*.s ; do
  echo "Testing $file"
  $opt -split-at-call -main < $file >/dev/null 2>`echo $file | sed 's/\.s$/.log_fs/'`
  $opt -split-at-call -use-fi -main < $file >/dev/null 2>`echo $file | sed 's/\.s$/.log_fi/'`
done
rm *.s
mkdir -p logs
mv *.log_fi logs/
mv *.log_fs logs/
