mpicc -o reduce_stddev.o reduce_stddev.c -lm &&
echo "NP\tData\tProcess Time\tComm Time" > reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 1200 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 12000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 120000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 1200000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 12000000 >> reduce_stddev.result &&
mpiexec -np 2 ./reduce_stddev.o 120000000 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 800 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 8000 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 80000 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 800000 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 8000000 >> reduce_stddev.result &&
mpiexec -np 3 ./reduce_stddev.o 80000000 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 600 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 6000 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 60000 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 600000 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 6000000 >> reduce_stddev.result &&
mpiexec -np 4 ./reduce_stddev.o 60000000 >> reduce_stddev.result
