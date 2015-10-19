mpicc -o all_avg.o all_avg.c &&
echo "NP\tData\tProcess Time\tComm Time" >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 1000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 10000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 100000 >> all_avg.result
