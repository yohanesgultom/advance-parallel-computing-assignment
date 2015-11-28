#include <stdio.h>
#include <stdlib.h>

float** create_random_matrix(int row, int col, int max)
{
    int i, j = 0;
    float **m = (float**) malloc(row * sizeof(float*));
    float *n2 = (float*) malloc(row * col * sizeof(float));
    for (i=0;i<row;i++) {
        ((float**)m)[i] = &(n2[i * col]);
        for (j=0;j<col;j++) {
            m[i][j] = (float) (rand() % max);
        }
    }
    return m;
}

void print_matrix(float **m, int row, int col)
{
    int i, j = 0;
    for (i=0;i<row;i++) {
        for (j=0;j<col;j++) {
            printf("%.2f\t", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// http://www.concentric.net/~Ttwang/tech/inthash.htm
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}
