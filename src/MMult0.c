/*Create macros so thah the matrices are stored in row-major order*/
#define A(i, j) a[(i) * lda + j]
#define B(i, j) b[(i) * ldb + j]
#define C(i, j) c[(i) * ldc + j]

/*Routing for computing C=A*B+C*/
void my_MMult(int m, int n, int k, double* a, int lda,
                double* b, int ldb, double* c, int ldc)
{
    int i, j, p;
    for (i = 0; i < m; i++) /*Loop over the rows of C*/
    {
          for (j = 0; j < n; j++) /*Loop over the columns of C*/
        {
            for (p = 0; p < k; p++) /*Updata C(i,j) with the inner product of the
                                    ith row of A and the jth column of B*/
            {
                C(i, j) = C(i, j) + A(i, p) * B(p, j);
            }
        }            
    }
}