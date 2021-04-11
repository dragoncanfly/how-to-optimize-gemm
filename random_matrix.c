#include<stdio.h>

#define A(i, j) a[(i) * lda + (j)]

void random_matrix(int m, int n, double* a, int lda)
{
    double drand48();
    int i,j;
    for (i = 0; i < m; i++)
    {
        for (j = 0;j < n; j++)
        {
            //drand48()返回俄服从均匀分布的[0.0, 1.0)之间的double型随机数
            A(i, j) = 2.0 * drand48() - 1.0;
        }
    }
}