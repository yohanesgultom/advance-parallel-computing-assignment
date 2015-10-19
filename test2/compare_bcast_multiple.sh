mpicc -o compare_bcast.o compare_bcast.c &&
mpiexec -np 2 ./compare_bcast.o 1000 10 >> compare_bcast.out &&
mpiexec -np 2 ./compare_bcast.o 10000 10 >> compare_bcast.out &&
mpiexec -np 2 ./compare_bcast.o 100000 10 >> compare_bcast.out

