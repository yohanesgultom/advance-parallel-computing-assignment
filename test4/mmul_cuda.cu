// Source adapted from: http://www.programmingsimplified.com/c-program-multiply-matrices
#include <stdio.h>
#include <time.h>

// create random matrix row-major-format
float* create_flat_matrix(int row, int col, int max)
{
    float* m = (float*)malloc(row*col*sizeof(float));
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            float val = (max > 0) ? (float)(rand() % max) : 0.0f;
            m[col * i + j] = val;
        }
    }
    return m;
}

// print matrix row-major-format
void print_flat_matrix(float *m, int row, int col)
{
    int i, j = 0;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            printf("%.2f ", m[col * i + j]);
        }
        printf("\n");
    }
}

void mmul(float *first, int m, int p, float *second, int q, float *multiply)
{
    int c, d, k = 0;
    float sum = .0f;
    for (c = 0; c < m; c++) {
        for (d = 0; d < q; d++) {
            for (k = 0; k < p; k++) {
                sum = sum + first[c*m+k] * second[k*q+d];
            }
            multiply[c*q+d] = sum;
            sum = 0;
        }
    }
}

__global__ void mmul_d(float *first, int m, int p, float *second, int q, float *multiply)
{
    int c, d, k = 0;
    float sum = .0f;
    for (c = 0; c < m; c++) {
        for (d = 0; d < q; d++) {
            for (k = 0; k < p; k++) {
                sum = sum + first[c*m+k] * second[k*q+d];
            }
            multiply[c*q+d] = sum;
            sum = 0;
        }
    }
}

__global__ void mmul_d_thread(float *first, int m, int p, float *second, int q, float *multiply)
{
    int c = blockIdx.x * blockDim.x + threadIdx.x;
    int d, k = 0;
    float sum = .0f;
    for (d = 0; d < q; d++) {
        for (k = 0; k < p; k++) {
            sum = sum + first[c*m+k] * second[k*q+d];
        }
        multiply[c*q+d] = sum;
        sum = 0;
    }
}


int main(int argc, char** argv)
{
    if (argc < 6) {
        printf("insufficient args. for A x B = C, required args: [row num A] [col num A OR row num B] [col num B] [cuda block size] [reps]\n");
        return EXIT_FAILURE;
    }

    int m, n, p, q, max = 0;
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    p = n;
    q = atoi(argv[3]);
    int blockSize = atoi(argv[4]);
    int nBlocks = (m * n) / blockSize + ((m * n) % blockSize == 0 ? 0 : 1);
    int reps = atoi(argv[5]);
    max = 10;

    float *first_d, *second_d, *multiply_d;
    float *first, *second, *multiply;

    int i = 0;
    double total_time = 0.0f;
    for (i = 0; i < reps; i++) {
        double exec_time = ((double) clock()) * -1;
        first = create_flat_matrix(m, n, max);
        second = create_flat_matrix(p, q, max);
        multiply = create_flat_matrix(m, q, 0);

        // printf("first:\n");
        // print_flat_matrix(first, m, n);
        //
        // printf("second:\n");
        // print_flat_matrix(second, p, q);

        cudaMalloc((void **) &first_d, m * n * sizeof(float));
        cudaMalloc((void **) &second_d, p * q * sizeof(float));
        cudaMalloc((void **) &multiply_d, m * q * sizeof(float));

        cudaMemcpy(first_d, first, m * n * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(second_d, second, p * q * sizeof(float), cudaMemcpyHostToDevice);

        if (blockSize <= 1) {
            mmul_d <<< 1, 1 >>> (first_d, m, n, second_d, q, multiply_d);
        } else {
            mmul_d_thread <<< nBlocks, blockSize >>> (first_d, m, n, second_d, q, multiply_d);
        }

        cudaMemcpy(multiply, multiply_d, m * q * sizeof(float), cudaMemcpyDeviceToHost);

        // mmul_h(first, m, n, second, q, multiply);
        // printf("multiply:\n");
        // print_flat_matrix(multiply, m, q);

        free(multiply); free(second); free(first);
        cudaFree(first_d); cudaFree(second_d); cudaFree(multiply_d);
        total_time = total_time + ((exec_time + ((double)clock())) / CLOCKS_PER_SEC);
        // printf("%d: %.6f\n", i, ((exec_time + ((double)clock())) / CLOCKS_PER_SEC));
    }
    printf("%d\t%d\t%d\t%d\t%d\t%.6fs\n", m, n, q, blockSize, reps, (total_time / reps));
    return EXIT_SUCCESS;
}
