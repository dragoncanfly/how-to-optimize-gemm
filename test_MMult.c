#include<stdio.h>
#include<stdlib.h>

#include "parameters.h"

void REF_MMult(int, int, int, double*, int, double*, int, double*, int);
void my_MMult(int, int, int, double*, int, double*, int, double*, int);
void copy_matrix(int, int, double*, int, double*, int);
void random_matrix(int, int, double*, int);
double compare_matrices(int, int, double*, int, double*, int);

double dclock();

int main()
{
    int p,
    m, n, k,
    lda, ldb, ldc,
    rep;

    double
    dtime, dtime_best,
    gflops,
    diff;

    double
    *a, *b, *c, *cref, *cold;

    printf("my_MMult = [\n");
    for (p = PFIRST; p <= PLAST; p += PINC)
    {
        m = (M == -1 ? p : M);
        n = (N == -1 ? p : N);
        k = (K == -1 ? p : K);

        gflops = 2.0 * m * n * k * 1.0e-09;

        lda = (LDA == -1 ? m : LDA);
        ldb = (LDB == -1 ? k : LDB);
        ldc = (LDC == -1 ? m : LDC);

        /*Allocate space for the matrices*/
        /*Note: Creatge an extra column in A to make sure that prefetching
        beyond the matrix does not cause a segfault*/
        a = (double*)malloc(lda * (k + 1) * sizeof(double));
        b = (double*)malloc(ldb * n * sizeof(double));
        c = (double*)malloc(ldc * n * sizeof(double));
        cold = (double*)malloc(ldc * n * sizeof(double));
        cref = (double*)malloc(ldc * n * sizeof(double));

        /*Generate random matrices A, B, Cold*/
        random_matrix(m, k, a, lda);
        random_matrix(k, n, b, ldb);
        random_matrix(m, n, cold, ldc);
#if 1
    memset(cold, 0, ldc * n * sizeof(double));
#endif
        copy_matrix(m, n, cold, ldc, cref, ldc);

        /*Run the reference inplementation so the answers can be compared*/
        REF_MMult(m, n, k, a, lda, b, ldb, cref, ldc);
        /*for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%f ", cref[i * n + j]);
            }
            printf("\n");
        }
        printf("\n\n");*/
        /*Time the "optimized" implementation*/
        for (rep = 0; rep < NREPEATS; rep++)
        {
            copy_matrix(m, n, cold, ldc, c, ldc);
            
            /*Time your implementation*/
            dtime = dclock();

            my_MMult(m, n, k, a, lda, b, ldb, c, ldc);

            dtime = dclock() - dtime;

            if (rep == 0)
                dtime_best = dtime;
            else
            {
                dtime_best = (dtime < dtime_best ? dtime : dtime_best);
            }
            /*for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    printf("%f ", c[i * n + j]);
                }
                printf("\n");
            }
            printf("\n\n");*/
        }

        diff = compare_matrices(m, n, c, ldc, cref, ldc);
        //printf("\n\n");
        //printf("%f \n", diff);
        //diff should always be 0
        if (diff > 0.5f)
        {
            printf("The diff value cannot be a number of not 0!!!\n");
            exit(0);
        }
        printf("%d %le %le \n", p, gflops / dtime_best, diff);
        fflush(stdout);

        free(a);
        free(b);
        free(c);
        free(cold);
        free(cref);
    }
    printf("];\n");
    exit(0);
}