/*Create macros so that the matrices are stored in row-major order*/

#define A(i, j) a[(i) * lda + (j)]
#define B(i, j) b[(i) * ldb + (j)]
#define C(i, j) c[(i) * ldc + (j)]

/*Routine for computing C = A*B + C*/

void AddDot1x4(int, double*, int, double*, int, double*, int);

void my_MMult(int m, int n, int k, double* a, int lda,
            double* b, int ldb, double* c, int ldc)
{
    int i,j;
    for (i = 0; i < m; i += 4) //Loop over the rows of C
    {
        for (j = 0; j < n; j++) //Loop over the columns of C, unrolled by 4
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
    
    In this version, we use pointer to track where in four Rows of A because of row-major of matrix
    */
    int p;
    register double
    /*hold contributions to
    C(0, 0), C(0, 1), C(0, 2), C(0, 3)*/
    c_00_reg, c_01_reg, c_02_reg, c_03_reg,
    /*holds A(0, p)*/
    b_0p_reg;
    double
    /*Pointer to the current elements in the four columns of B*/
    *ap0_pntr, *ap1_pntr, *ap2_pntr, *ap3_pntr;

    ap0_pntr = &A(0, 0);
    ap1_pntr = &A(1, 0);
    ap2_pntr = &A(2, 0);
    ap3_pntr = &A(3, 0);

    c_00_reg = 0.0;
    c_01_reg = 0.0;
    c_02_reg = 0.0;
    c_03_reg = 0.0;

    for (p = 0; p < k; p++)
    {
        b_0p_reg = B(p, 0);

        c_00_reg += b_0p_reg * (*(ap0_pntr++));
        c_01_reg += b_0p_reg * (*(ap1_pntr++));
        c_02_reg += b_0p_reg * (*(ap2_pntr++));
        c_03_reg += b_0p_reg * (*(ap3_pntr++));
    }
    C(0, 0) += c_00_reg;
    C(1, 0) += c_01_reg;
    C(2, 0) += c_02_reg;
    C(3, 0) += c_03_reg;
}

