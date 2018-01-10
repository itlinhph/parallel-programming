#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

#define M 5
#define N 10
#define P 5

void initMatrix(int *a, int row, int col) {
    int i, j;
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++) {
            a[i*col + j] = i + j;
        }
}

void printMatrix(const char* s, int *a, int row, int col) {
    printf("\nMATRIX %s: \n",s);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%2d ", a[i*col+j]);
        }
        printf("\n");
    }
}

int main() {
    int *a, *b, *c, *As, *Cs ;
    int i, j, k ;
    a = (int *)malloc(sizeof(int) *M*N);
    b = (int *)malloc(sizeof(int) *N*P);
    c = (int *)malloc(sizeof(int) *M*P);

    int rank, ntasks ;
    MPI_Init(NULL, NULL) ;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    int chunk_size = M/ntasks ;
    As = (int *)malloc(sizeof(int)*chunk_size*N) ;
    Cs = (int *)malloc(sizeof(int)*chunk_size*P) ;
    if(rank==0) {

        initMatrix(a, M, N);
        initMatrix(b, N, P);
        printMatrix("A", a, M, N);
        printMatrix("B",b, N, P);

        
    }
    MPI_Bcast(b, N*P, MPI_INT, 0, MPI_COMM_WORLD) ;

    MPI_Scatter(a, chunk_size*N, MPI_INT, As, chunk_size*N, MPI_INT, 0, MPI_COMM_WORLD);

    for(i = 0; i<chunk_size; i++) {
        for(j = 0; j<P; j++) {
            Cs[i*P +j] = 0 ;
            for(k=0; k<N; k++) {
                Cs[i*P +j] += As[i*N+k]*b[k*P+j] ;
            }
        }
    }

    MPI_Gather(Cs, chunk_size*P, MPI_INT, c, chunk_size*P, MPI_INT, 0, MPI_COMM_WORLD) ;
    if(rank==0) {
        printMatrix("C ",c, M, P) ;
    }
    MPI_Finalize() ;
}