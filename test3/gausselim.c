// Parallel Programming for Multicore and CLuster Systems, Thomas Rauber, Gudula Runger
// Section 7 Algorithm for Systems of Linear Equations

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double** create_matrix(int row, int col)
{
    int i = 0;
    double **m = (double**) malloc(row * sizeof(double*));
    double *n = (double*) malloc(row * col * sizeof(double));
    for (i=0;i<row;i++) m[i] = &(n[i * col]);
    return m;
}

void print_matrix(double **m, int row, int col)
{
    int i, j = 0;
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            printf("%f\t", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_vector(double *v, int row)
{
    int i;
    for (i=0;i<row;i++) {
        printf("%f\n", v[i]);
    }
    printf("\n");
}

int max_col(double **a, int k, int n)
{
    int i, i_max = k;
    double current, temp_max = a[k][i_max];
    for (i = k; i < n; i++) {
        current = fabs(a[i][k]); // iterate each row for given column k
        if (temp_max < current) {
            temp_max = current;
            i_max = i;
        }
    }
    return i_max;
}

void exchange_row(double **a, double *b, int r, int k, int n)
{
    //printf("==> exchange row %d with %d\n", r, k);
    int i;
    double *a_r_temp = (double*) malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        a_r_temp[i] = a[r][i];
        a[r][i] = a[k][i];
        a[k][i] = a_r_temp[i];
    }
    double b_r_temp = b[r];
    b[r] = b[k];
    b[k] = b_r_temp;
}

double *gauss_sequential (double **a, double*b, int n)
{
    double *x, sum, l[n * n];
    int i, j, k, r;

    x = (double *) malloc(n * sizeof(double));
    for (k = 0; k < n-1; k++) { /* Forward elimination */
        r = max_col(a, k, n);
        if (k != r) exchange_row(a, b, r, k, n);
        for (i = k+1; i < n; i++) {
            l[i] = a[i][k] / a[k][k];
            for (j = k+1; j < n; j++) {
                a[i][j] = a[i][j] - l[i] * a[k][j];
            }
            b[i] = b[i] -l[i] * b[k];
        }
    }

    for (k = n-1; k >= 0; k--) { /* Backward substition */
        sum = 0.0;
        for (j = k+1; j < n; j++) {
            sum = sum + a[k][j] * x[j];
        }
        x[k] = 1 / a[k][k] * (b[k] - sum);
    }
    return x;
}

double *gauss_cyclic(double **a, double *b, int n)
{
    double *x, l[n*n], *buf;
    int i, k, j, r, tag = 42;
    MPI_Status status;
    struct { double val; int node; } z, y;
    x = (double *) malloc(n * sizeof(double));
    buf = (double *) malloc((n+1) * sizeof(double));
}

void gauss_sequential_test()
{
    int n = 3;
    double **a = create_matrix(n, n);
    double *b = (double*) malloc(n * sizeof(double));
    a[0][0] = -3; a[0][1] = 2; a[0][2] = -6; b[0] = 6; // -3x + 2y -6z = 6
    a[1][0] = 5; a[1][1] = 7; a[1][2] = -5; b[1] = 6; // 5x + 7y -5z = 6
    a[2][0] = 1; a[2][1] = 4; a[2][2] = -2; b[2] = 8; // x + 4y - 2z = 8
    double* x = gauss_sequential(a, b, n);
    print_vector(x, n);
    printf("expected [-2, 3, 1]\n");
}

int main(int argc, char** argv)
{
    gauss_sequential_test();
    return 0;
}
