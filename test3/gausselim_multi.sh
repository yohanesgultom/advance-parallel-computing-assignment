mpicc -o gausselim.o gausselim.c -lm &&
echo "NP\tData\tProcess Time" > gausselim.result &&
mpiexec -np 1 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 200 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 1 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 300 >> gausselim.result &&
mpiexec -np 2 ./gausselim.o 300 >> gausselim.result
