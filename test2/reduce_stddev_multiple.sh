mpicc -o reduce_stddev.o reduce_stddev.c -lm &&
echo "NP\tData\tProcess Time\tComm Time" >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 1000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 10000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 100000 >> reduce_stddev.result
