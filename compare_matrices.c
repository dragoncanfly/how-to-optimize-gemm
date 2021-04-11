#include<math.h>
#include<stdio.h>
#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

double compare_matrices(int m, int n, double* a, int lda, double* b, int ldb)
{
    int i, j;
    double max_diff = 0.0, diff = 0;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            //some problem, when abs(A(i,j)-B(i,j)) was used, the result is different with no abs func call
            diff = A(i, j) - B(i, j);
            max_diff = (diff > max_diff ? diff : max_diff);
            //printf("a(i,j): %lf, b(i, j): %lf, diff: %lf, maxdiff: %lf \n", A(i, j), B(i, j), diff, max_diff);
        }
    }
    return max_diff;
}