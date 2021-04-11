#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
//#define C(i, j) c[(i) * ldc + (j)]

void copy_matrix(int m, int n, double* a, int lda, double* b, int ldb)
{
    int i,j;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            B(i, j) = A(i, j);
        }
    }
}