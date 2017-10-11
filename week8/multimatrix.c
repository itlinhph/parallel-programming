#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

#define m 8
#define n 4
#define p 8

int main() {
    int i, size, rank, mc;
    int *A, *B, *C ;
    MPI_Status thongbao ;
    MPI_Init(NULL, NULL) ;

    MPI_Comm_size(MPI_COMM_WORLD, &size) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;

    A= (int *) malloc(sizeof(int)*m*n) ;
    B= (int *) malloc(sizeof(int)*n*p) ;
    C= (int *) malloc(sizeof(int)*p*m) ;

    // domain decomposition
    int *Ac, *Bc, *Cc ;

    Ac= (int *) malloc(sizeof(int)*mc*n)
    Bc= (int *) malloc(sizeof(int)*n*p)
    Cc= (int *) malloc(sizeof(int)*mc*p)

    if(rank==0) {
        // khoi tao
        for(i=0; i<m; i++) 
            for(j=0; j<n; j++)
                if(i==j)
                    *(A+i*n +j)=1 ;
                else
                    *(A+i*n +j)=0 ;
        for(i=0; i<n; i++)
        
    }


}