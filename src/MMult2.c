/*Create macros so that the matrices are stored in row-major order*/
#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

/*Routine for computing C=A*B + C*/

void AddDot(int, double*, int, double*, double*);

void my_MMult(int m, int n, int k, double* a, int lda,
            double* b, int ldb, double* c, int ldc)
{
    int i, j;
    for (i = 0; i < m; i++)  //Loop over the rows of C
    {
        for (j = 0; j < n; j += 4)  /*Loop over the columns of C, unrolled by 4*/
        {
            /*Update the C(i, j) with the inner product of the ith row of A and the 
            jth column of B*/
            AddDot(k, &A(i, 0), lda, &B(0, j), &C(i, j));
            
            /*Update the C(i, j+1) with the inner product of the ith row of A and the 
            (j+1)th column of B*/
            AddDot(k, &A(i, 0), lda, &B(0, j+1), &C(i, j+1));

            /*Update the C(i, j+2) with the inner product of the ith row of A and the 
            (j+2)th column of B*/
            AddDot(k, &A(i, 0), lda, &B(0, j+2), &C(i, j+2));

            /*Update the C(i, j+3) with the inner product of the ith row of A and the 
            (j+3)th column of B*/
            AddDot(k, &A(i, 0), lda, &B(0, j+3), &C(i, j+3));
        }
    }
}

/*Create macros to let X(i) equal the ith element of x*/
#define Y(i) y[(i) * incx]

void AddDot(int k, double* x, int incx, double* y, double* gamma)
{
    /*compute gamma := x'*y+gamma with vectors x and y of lenght n.
    Here y starts at location y with increment (stride) incx and x start at location y and has (implicit) stride of 1.
    */
   int p;
   for (p = 0; p < k; p++)
   {
       *gamma += x[p] * Y(p);
   }
}