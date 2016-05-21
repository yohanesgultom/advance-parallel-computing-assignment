// Parallel Programming for Multicore and CLuster Systems, Thomas Rauber, Gudula Runger
// Section 7 Algorithm for Systems of Linear Equations

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

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

int main(int argc, char** argv)
{
    n = atoi(argv[1]); // number of variables for each problem
    int vars[n];
    int coef[n];
    int i, j, count, sum;
    double process_time = 0.0;

    double **a = create_matrix(n, n);
    double *b = (double*) malloc(n * sizeof(double));
    
    clock_t start, end;

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

    // print_matrix(a, n, n);
    // print_vector(b, n);
    start = clock();
    double* x = gauss_sequential(a, b);
    end = clock();    
    process_time = ((double)(end - start)) / CLOCKS_PER_SEC; 
    //print_vector(x, n);
    printf("%d\t%f\n", n, process_time);

    free(a[0]);
    free(a);
    free(b);

    return 0;
}
