mpicc -o reduce_avg.o reduce_avg.c &&
echo "NP\tData\tProcess Time\tComm Time" >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 1000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 10000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 100000 >> reduce_avg.result
