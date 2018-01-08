#include <stdio.h>
#include <omp.h>
#include <malloc.h>

#define threads 5
#define M 5
#define N 10
#define P 5
// A[M][N] * B[N][P] = C[M][P]

void initMatrix(int a[][100], int m, int n) {
    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++) {
            a[i][j] = i+j ;
            if(m==10) {
                a[i][j] = 1 ;
            }
        }
}

void printMatrix(int a[][100], int m, int n) {
    printf("\nMATRIX: \n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%3d ", a[i][j]);
        }
        printf("\n");
    }
}
int main() {
    int a[100][100] ;
    int b[100][100] ;
    int c[100][100] ;
    int i, j, k ;
    initMatrix(a, M, N) ;
    initMatrix(b, N, P) ;
    printMatrix(a, M, N) ;
    printMatrix(b, N, P) ;

    omp_set_num_threads(threads) ;
    int id, sid, eid;
    #pragma omp parallel private(id, sid, eid, i, j, k)
    {
        id = omp_get_thread_num() ;
        sid = id*M/threads ;
        eid = sid + M/threads ;
        
        for(i= sid; i<eid; i++) {
            for(j=0; j<P; j++) {
                c[i][j] = 0 ;
                for(k=0; k<N; k++)
                    c[i][j] += a[i][k]*b[k][j] ;
            }
        }
        
    }

    int d[100][100] ;
    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            d[i][j] = 0;
            for (k = 0; k < N; k++)
                d[i][j] += a[i][k] * b[k][j];
        }
    }
    printMatrix(d, M, P) ;
    printMatrix(c, M, P) ;
    return 0;
} 