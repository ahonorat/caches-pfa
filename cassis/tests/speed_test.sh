#!/bin/bash

mkdir build
cd build
cmake ../../ -DLUA_LIBRARIES=-llua5.2 -DLUA_INCLUDE_DIR=/usr/include/lua5.2
make
cd ..
echo "Running some benchs"
echo "Time for the test  with x values in y ms."
for j in par falsesharing pipeline broadcast
do
echo "---------------------------------"
echo "Running "$j" test "
for i in 1000 5000 10000 50000 100000
do
echo -n -e $i "\t"
cd ../MAQAO
./get_traces.sh ../tests/speed_test/test.c $j 4 $i > /dev/null
cd ../tests/speed_test
/usr/bin/time ./bench.sh 2>&1 | awk '/WARNING|Err/ {print $0} /user/ {print 60000*substr($3,0,1)+1000*substr($3,3,2)+substr($3,6,2)}' > res_speed.txt
cd ..
done
done

rm -f ../MAQAO/trace*
rm -f ../MAQAO/instr
rm -rf build 
