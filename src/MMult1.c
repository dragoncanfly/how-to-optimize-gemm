/*Create macros so that the matrices are stored in row-major order*/
#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

/*Routine for computing C = A * B + C*/
void AddDot(int, double*, int, double*, double*);

void my_MMult(int m, int n, int k, double* a, int lda,
                double* b, int ldb, double* c, int ldc)
{
    int i, j;
    for (j = 0; j < n; j++) /*Loop over the columns of C*/
    {
        for (i = 0; i < m; i++) /*Loop over the rows of C*/
        {
            /*Update the C(i, j) with the inner product of the ith row of A and 
            jth column of B*/
            AddDot(k, &A(i, 0), lda, &B(0, j), &C(i, j));
        }
    }
}

/*Create macro to let X(i)equal the ith element of x*/
#define Y(i) y[(i) * incx]

void AddDot(int k, double* x, int incx, double* y, double* gamma)
{
    /*compute gamma := x'*y + gamma with vectors x and y of length n.
    Here x starts at location x with increment (stride) incx and y starts
    at location y  and has (implicit) stride of 1
    */
   int p;
   for (p = 0; p < k; p++)
   {
       *gamma += x[p] * Y(p);  //here row-major, so Y should has a stride of inxx
   }
}