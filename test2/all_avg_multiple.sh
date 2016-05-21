mpicc -o all_avg.o all_avg.c &&
echo "NP\tData\tProcess Time\tComm Time" > all_avg.result &&
mpiexec -np 2 ./all_avg.o 1200 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 12000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 120000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 1200000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 12000000 >> all_avg.result &&
mpiexec -np 2 ./all_avg.o 120000000 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 800 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 8000 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 80000 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 800000 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 8000000 >> all_avg.result &&
mpiexec -np 3 ./all_avg.o 80000000 >> all_avg.result &&
mpiexec -np 4 ./all_avg.o 600 >> all_avg.result &&
mpiexec -np 4 ./all_avg.o 6000 >> all_avg.result &&
mpiexec -np 4 ./all_avg.o 60000 >> all_avg.result &&
mpiexec -np 4 ./all_avg.o 600000 >> all_avg.result &&
mpiexec -np 4 ./all_avg.o 6000000 >> all_avg.result  &&
mpiexec -np 4 ./all_avg.o 60000000 >> all_avg.result
