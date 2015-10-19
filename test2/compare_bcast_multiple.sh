mpicc -o compare_bcast.o compare_bcast.c &&
echo "Algo\tNP\tData\tProcess Time" >> compare_bcast.result &&
mpiexec -np 2 ./compare_bcast.o 1000 10 >> compare_bcast.result &&
mpiexec -np 2 ./compare_bcast.o 10000 10 >> compare_bcast.result &&
mpiexec -np 2 ./compare_bcast.o 100000 10 >> compare_bcast.result
