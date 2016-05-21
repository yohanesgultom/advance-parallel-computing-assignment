mpicc -o transpose.o transpose.c &&
echo "NP\tData\tProcess Time\tComm Time" > transpose.result &&
mpiexec -np 2 ./transpose.o >> transpose.result &&
mpiexec -np 3 ./transpose.o >> transpose.result &&
mpiexec -np 4 ./transpose.o >> transpose.result
