mpicc -o reduce_avg.o reduce_avg.c &&
echo "NP\tData\tProcess Time\tComm Time" > reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 1200 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 12000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 120000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 1200000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 12000000 >> reduce_avg.result &&
mpiexec -np 2 ./reduce_avg.o 120000000 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 800 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 8000 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 80000 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 800000 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 8000000 >> reduce_avg.result &&
mpiexec -np 3 ./reduce_avg.o 80000000 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 600 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 6000 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 60000 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 600000 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 6000000 >> reduce_avg.result &&
mpiexec -np 4 ./reduce_avg.o 60000000 >> reduce_avg.result
