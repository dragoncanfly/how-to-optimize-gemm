/*Create macros so that the matrices are stored in row-major order*/

#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

/*Routine for computing C = A*B + C*/

void AddDot(int, double*, int, double*, double*);
void AddDot1x4(int, double*, int, double*, int, double*, int);

void my_MMult(int m, int n, int k, double* a, int lda,
            double* b, int ldb, double* c, int ldc)
{
    int i,j;
    for (i = 0; i < m; i++) //Loop over the rows of C
    {
        for (j = 0; j < n; j += 4) //Loop over the columns of C, unrolled by 4
        {
            /*Update C(i, j), C(i, j+1), C(i, j+2), and C(i, j+3) in
            one routine (four inner products)
            */
           AddDot1x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
        }
    }
}

void AddDot1x4(int k, double*a, int lda, double* b, int ldb, double* c, int ldc)
{
    /*so, this routine computes four elements of C:
    C(0, 0), C(0, 1), C(0,2), C(0,3).
    Notice that this routine is called with c=C(i,j) in the previous routine, so these
    are actually the elements C(i,j), C(i, j+1), C(i, j+2), C(i, j+3) in the original matrix C
    */
    AddDot(k, &A(0, 0), lda, &B(0, 0), &C(0, 0));
    AddDot(k, &A(0, 0), lda, &B(0, 1), &C(0, 1));
    AddDot(k, &A(0, 0), lda, &B(0, 2), &C(0, 2));
    AddDot(k, &A(0, 0), lda, &B(0, 3), &C(0, 3));
}

/*Create macro to let Y(i) equal the ith element of y*/

#define Y(i) y[(i) * incy]

void AddDot(int k, double* x, int incy, double* y, double* gamma)
{
    /*compute gamma := x'*y+gamma with vectors x and y of length n
    Here y starts at location y with increment (stride) incy and x starts at 
    location x and has (implicit) stride of 1.
    */
   int p;
   for (p = 0; p < k; p++)
   {
       *gamma += x[p] * Y(p);
   }
}