#include <stdio.h>
#include <stdlib.h>

float** create_random_matrix(int row, int col, int max);
void print_matrix(float **m, int row, int col);
// seed for srand() http://www.concentric.net/~Ttwang/tech/inthash.htm
unsigned long mix(unsigned long a, unsigned long b, unsigned long c);
