nvcc -o mmul_cublas.o -lcublas -lcurand mmul_cublas.cu &&
echo "A_row\tA_col/v_row\tv_col\treps\tT(s)" > mmul_cublas.result &&
./mmul_cublas.o 100 100 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 200 200 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 300 300 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 400 400 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 500 500 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 600 600 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 700 700 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 800 800 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 900 900 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 1000 1000 1 10 >> mmul_cublas.result &&
./mmul_cublas.o 100 100 100 10 >> mmul_cublas.result &&
./mmul_cublas.o 200 200 200 10 >> mmul_cublas.result &&
./mmul_cublas.o 300 300 300 10 >> mmul_cublas.result &&
./mmul_cublas.o 400 400 400 10 >> mmul_cublas.result &&
./mmul_cublas.o 500 500 500 10 >> mmul_cublas.result &&
./mmul_cublas.o 600 600 600 10 >> mmul_cublas.result &&
./mmul_cublas.o 700 700 700 10 >> mmul_cublas.result &&
./mmul_cublas.o 800 800 800 10 >> mmul_cublas.result &&
./mmul_cublas.o 900 900 900 10 >> mmul_cublas.result &&
./mmul_cublas.o 1000 1000 1000 10 >> mmul_cublas.result
