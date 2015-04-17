#!/bin/bash

# To be clear, this will kill all .s files you happen to have in your current
# directory

# use to test flow-insensitivity.
# will run flow-sensitive and flow-insensitive versions on all passes
# then you can: 
#  cd logs
#  vimdiff passesx.log_fi passesx.log_fs


LLVMLIBDIR="Debug/lib"
TESTFOLDER="test"
opt="opt --load ../../../../$LLVMLIBDIR/Broadway.so"

echo "Using: $opt"
echo "If that wasn't what you're expecting, edit $0"
echo
echo
echo

COMMAND="$opt -main -pal"

# Compile test programs 
rm -f *.s
clang -O0 -S -emit-llvm test/*.c

# FILESTATE
echo "Performing filestate tests..."
$COMMAND $TESTFOLDER/filestate.pal < filestate-test1.s > /dev/null 2> filestate-test1.log 
$COMMAND $TESTFOLDER/filestate.pal < filestate-test2.s > /dev/null 2> filestate-test2.log 

# TAINTEDNESS
echo "Performing taintedness tests..."
$COMMAND $TESTFOLDER/taintedness.pal < taintedness-test1.s > /dev/null 2> taintedness-test1.log
$COMMAND $TESTFOLDER/taintedness.pal < taintedness-test2.s > /dev/null 2> taintedness-test2.log

# MATRIX
echo "Performing matrix tests..."
$COMMAND $TESTFOLDER/matrix.pal < matrix-test1.s > /dev/null 2> matrix-test1.log 
$COMMAND $TESTFOLDER/matrix.pal < matrix-test2.s > /dev/null 2> matrix-test2.log 

echo "See ./logs for results."
# Cleanup logs
rm *.s
mkdir -p logs
mv *.log logs/
chmod 755 logs -R
chmod 755 test/.results -R

# Check Results
for file in logs/*.log
do
  file=`basename ${file}`
  echo
  echo "Checking File: ${file}"
  if [ ! -f test/.results/${file} ] 
  then
	echo -e "\tDo not have checks for: ${file}"
	continue
  fi
  
  diff logs/${file} test/.results/${file} > /dev/null
  if [ $? -eq 0 ]; then
	echo -e "\tTest: PASSED"
  else
	echo -e "\tTest: FAILED"
  fi
done


