// Parallel Programming for Multicore and CLuster Systems, Thomas Rauber, Gudula Runger
// Section 7 Algorithm for Systems of Linear Equations

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

/***** globals ****/
int n;
int me, p;

/**** helpers ****/

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

int get_rand_sign()
{
    int sign;
    if (rand() % 10 > 4) {
        sign = 1;
    } else {
        sign = -1;
    }
    return sign;
}


/****** sequential gauss elimination ***********/

int max_col(double **a, int k)
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

void exchange_row(double **a, double *b, int r, int k)
{
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

double *gauss_sequential (double **a, double*b)
{
    double *x, sum, l[n * n];
    int i, j, k, r;

    x = (double *) malloc(n * sizeof(double));
    for (k = 0; k < n-1; k++) { /* Forward elimination */
        r = max_col(a, k);
        if (k != r) exchange_row(a, b, r, k);
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

/********** parallel gauss elimination ***********/

int max_col_loc(double **a, int k)
{
    int i_max = -1;
    double current, temp_max = -99;
    int i;
    for (i = k; i < n; i++) {
        // check for max if row i is owned by me
        if (i % p == me) {
            //printf("%d owns row %d for k=%d a[%d][%d]=%f\n", me, i, k, i, k, a[i][k]);
            // iterate each row for given column k
            current = fabs(a[i][k]);
            if (i_max == -1) {
                // first element
                temp_max = current;
                i_max = i;
            } else if (temp_max < current) {
                temp_max = current;
                i_max = i;
            }
        }
    }
    //printf("%d local max is %f on row %d for k=%d\n", me, temp_max, i_max, k);
    return i_max;
}

void copy_row(double **a, double *b, int k, double *buf)
{
    int i;
    for (i = 0; i < n; i++) {
        buf[i] = a[k][i];
    }
    buf[n] = b[k]; // last element
}

void copy_exchange_row(double **a, double *b, int r, double *buf, int k)
{
    int i;
    double *a_r_temp = (double*) malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        a_r_temp[i] = a[r][i];
        a[k][i] = a[r][i];
        a[r][i] = buf[i];
        buf[i] = a_r_temp[i];
    }
     // last element
    double b_r_temp = b[r];
    b[k] = b[r];
    b[r] = buf[n];
    buf[n] = b_r_temp;
}

void copy_back_row(double **a, double *b, double *buf, int k)
{
    int i;
    for (i = 0; i < n; i++) {
        a[k][i] = buf[i];
    }
    b[k] = buf[n]; // last element
}

double *gauss_cyclic(double **a, double *b)
{
    double *x, l[n*n], *buf;
    int i, k, j, r, tag = 42;
    MPI_Status status;
    struct { double val; int node; } z, y;
    x = (double *) malloc(n * sizeof(double));
    buf = (double *) malloc((n+1) * sizeof(double));

    for (k=0; k<n-1; k++) { /* Forward elimination */
        r = max_col_loc(a, k);
        z.node = me;
        if (r != -1) {
            z.val = fabs(a[r][k]);
        } else {
            z.val = 0.0;
        }
        // z = local max
        // y = global max
        MPI_Allreduce(&z, &y, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
        //printf("p%d says global max is own by p%d = %f\n", me, y.node, y.val);
        if (k % p == y.node) { /* Pivot row and row k are on the same processor */
            if (k % p == me) {
                if (a[k][k] != y.val) exchange_row(a, b, r, k);
                copy_row(a, b, k, buf);
            }
        } else { /* Pivot row and row k are owned by different processor */
            if (k % p == me) {
                copy_row(a, b, k, buf);
                MPI_Send(buf+k, n-k+1, MPI_DOUBLE, y.node, tag, MPI_COMM_WORLD);
            } else if (y.node == me) {
                MPI_Recv(buf+k, n-k+1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
                copy_exchange_row(a, b, r, buf, k);
            }
        }
        MPI_Bcast(buf+k, n-k+1, MPI_DOUBLE, y.node, MPI_COMM_WORLD);
        if ((k % p != y.node) && (k % p == me)) {
            copy_back_row(a, b, buf, k);
        }
        i = k+1; while (i % p != me) i++;
        for (; i<n; i+=p) {
            l[i] = a[i][k] / buf[k];
            for (j=k+1; j<n;j++) {
                a[i][j] = a[i][j] - l[i] * buf[j];
            }
            b[i] = b[i] - l[i] * buf[n];
        }
    }

    double sum;
    for (k=n-1; k>=0; k--) { /* Backward substition */
        if (k % p == me) {
            sum = 0.0;
            for (j=k+1; j<n; j++) {
                sum = sum + a[k][j] * x[j];
            }
            x[k] = 1/a[k][k] * (b[k] - sum);
        }
        MPI_Bcast(&x[k], 1, MPI_DOUBLE, k%p, MPI_COMM_WORLD);
    }
    free(buf);
    return x;
}


/***** Test ******/

// void gauss_sequential_test()
// {
//     double **a = create_matrix(n, n);
//     double *b = (double*) malloc(n * sizeof(double));
//     a[0][0] = -3; a[0][1] = 2; a[0][2] = -6; b[0] = 6; // -3x + 2y -6z = 6
//     a[1][0] = 5; a[1][1] = 7; a[1][2] = -5; b[1] = 6; // 5x + 7y -5z = 6
//     a[2][0] = 1; a[2][1] = 4; a[2][2] = -2; b[2] = 8; // x + 4y - 2z = 8
//     double* x = gauss_sequential(a, b);
//     print_vector(x, n);
//     printf("expected [-2, 3, 1]\n");
//     free(a[0]);
//     free(a);
//     free(b);
// }
//
// void gauss_cyclic_test()
// {
//     double **a = create_matrix(n, n);
//     double *b = (double*) malloc(n * sizeof(double));
//     a[0][0] = -3; a[0][1] = 2; a[0][2] = -6; b[0] = 6; // -3x + 2y -6z = 6
//     a[1][0] = 5; a[1][1] = 7; a[1][2] = -5; b[1] = 6; // 5x + 7y -5z = 6
//     a[2][0] = 1; a[2][1] = 4; a[2][2] = -2; b[2] = 8; // x + 4y - 2z = 8
//
//     MPI_Init(NULL, NULL);
//     MPI_Comm_rank(MPI_COMM_WORLD, &me);
//     MPI_Comm_size(MPI_COMM_WORLD, &p);
//
//     if (me == 0) {
//         print_matrix(a, n, n);
//         print_vector(b, n);
//     }
//     double* x = gauss_cyclic(a, b);
//     if (me == 0) {
//         print_vector(x, n);
//         printf("expected [-2, 3, 1]\n");
//     }
//     MPI_Finalize();
//     free(a[0]);
//     free(a);
//     free(b);
// }

// int main(int argc, char** argv)
// {
//     if (argc > 1 && strcmp(argv[1], "seq") == 0) {
//         gauss_sequential_test();
//     } else {
//         gauss_cyclic_test();
//     }
//     return 0;
// }

int main(int argc, char** argv)
{
    n = atoi(argv[1]); // number of variables for each problem
    int vars[n];
    int coef[n];
    int i, j, count, sum;
    double process_time = 0.0;

    double **a = create_matrix(n, n);
    double *b = (double*) malloc(n * sizeof(double));

    // generate random variables and coeficients
    for (i=0; i<n; i++) {
        vars[i] = rand() % 100 * get_rand_sign();
    }

    // generate random coeficients
    // calculate sum of linear equation
    for (i=0; i<n; i++) {
        sum = 0;
        for (j=0; j<n; j++) {
            coef[j] = rand() % 100 * get_rand_sign();
            a[i][j] = coef[j];
            sum += vars[j] * coef[j];
        }
        b[i] = sum;
    }

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (me == 0) {
        // print_matrix(a, n, n);
        // print_vector(b, n);
        process_time -= MPI_Wtime();
    }
    double* x = gauss_cyclic(a, b);
    if (me == 0) {
        process_time += MPI_Wtime();
        printf("%d\t%d\t%f\n", p, n, process_time);
        // print_vector(x, n);
    }
    MPI_Finalize();
    free(a[0]);
    free(a);
    free(b);

    return 0;
}
