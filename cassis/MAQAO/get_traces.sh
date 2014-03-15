#!/bin/bash

a=3

if [ "$#" == "$a" ]
then echo "OK"

export OMP_NUM_THREADS=$3

echo "Program file"
echo $1

echo "Function to trace"
echo $2

echo "Number of threads"
echo $3

gcc -o test $1 -DKERNEL=$2 -fopenmp -g
./maqao instrumentation.lua test "$2.*" > /dev/null
./test_i_mtl 2> out

awk '/STORE|LOAD/ {print $0 > "trace"$3}' out

echo "Removing files"
rm test_i_mtl
rm test
rm trace.MTL.*
rm madras*
rm out

else echo "Usage: ./get_traces file.c name_function number_threads"
fi
