#include <stdio.h>
#include <omp.h>
#include <malloc.h>

#define M 10
#define N 15

void initMatrix (int **a) {
    int i, j;
    
    for (i = 0; i < M; i++) {
        a[i] = (int *)malloc(sizeof(int) * N);
    }
    for(i = 0; i<M; i++)
        for (j = 0; j<N; j++) {
            a[i][j] = i+j ;
        }
}

void printMatrix(int **a) {
    printf("\nMATRIX: \n") ;
    for(int i = 0; i<M; i++) {
        for(int j= 0; j<N; j++) {
            printf("%2d ", a[i][j]) ;
        }
        printf("\n") ;
    }
}
int main() {
    int i, j; 
    int **a, **b, **c ;
    a = (int **)malloc(sizeof(int *) * M);
    b = (int **)malloc(sizeof(int *) * M);
    initMatrix(a) ;
    initMatrix(b) ;
    printMatrix(a) ;
    
    c = (int **)malloc(sizeof(int *) * M);
    for (i = 0; i < M; i++) {
        c[i] = (int *)malloc(sizeof(int) * N);
    }

    int id, sid, eid;
    int threads = 5 ;
    omp_set_num_threads(threads) ;

    #pragma omp parallel private(id,sid, eid, i,j)
    {
        id = omp_get_thread_num();
        sid = id*M/threads ;
        eid = sid + M/threads ;
        for(i=sid; i<eid; i++)
            for (j = 0; j < N; j++)
                c[i][j] = a[i][j] + b[i][j];

    }
    printMatrix(c);
    return 0;
}