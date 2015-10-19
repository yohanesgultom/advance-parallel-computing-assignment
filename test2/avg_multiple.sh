mpicc -o avg.o avg.c &&
echo "NP\tData\tProcess Time\tComm Time" >> avg.result &&
mpiexec -np 2 ./avg.o 1000 >> avg.result &&
mpiexec -np 2 ./avg.o 10000 >> avg.result &&
mpiexec -np 2 ./avg.o 100000 >> avg.result
