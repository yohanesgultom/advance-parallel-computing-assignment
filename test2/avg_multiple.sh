mpicc -o avg.o avg.c &&
echo "NP\tData\tProcess Time\tProcess Time to Avg\tComm Time" > avg.result &&
mpiexec -np 2 ./avg.o 1200 >> avg.result &&
mpiexec -np 2 ./avg.o 12000 >> avg.result &&
mpiexec -np 2 ./avg.o 120000 >> avg.result  &&
mpiexec -np 2 ./avg.o 1200000 >> avg.result &&
mpiexec -np 2 ./avg.o 12000000 >> avg.result &&
mpiexec -np 2 ./avg.o 120000000 >> avg.result &&
mpiexec -np 3 ./avg.o 800 >> avg.result &&
mpiexec -np 3 ./avg.o 8000 >> avg.result &&
mpiexec -np 3 ./avg.o 80000 >> avg.result &&
mpiexec -np 3 ./avg.o 800000 >> avg.result &&
mpiexec -np 3 ./avg.o 8000000 >> avg.result &&
mpiexec -np 3 ./avg.o 80000000 >> avg.result &&
mpiexec -np 4 ./avg.o 600 >> avg.result &&
mpiexec -np 4 ./avg.o 6000 >> avg.result &&
mpiexec -np 4 ./avg.o 60000 >> avg.result &&
mpiexec -np 4 ./avg.o 600000 >> avg.result &&
mpiexec -np 4 ./avg.o 6000000 >> avg.result &&
mpiexec -np 4 ./avg.o 60000000 >> avg.result
