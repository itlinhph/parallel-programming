#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

int main() {
    int i, size, rank;
    int *A, *B, *C; 


    MPI_Status thongbao ;
    MPI_Init(NULL, NULL) ;
    MPI_Comm_size(MPI_COMM_WORLD, &size) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;

    A= (int *) malloc (sizeof(int)*20) ;
    B= (int *) malloc (sizeof(int)*20) ;
    C= (int *) malloc (sizeof(int)*20) ;

    if(rank==0) {
        for(i=0; i<20; i++) 
            *(A+i) =i ;
        MPI_Send(A,20, MPI_INT, 1,100, MPI_COMM_WORLD) ;
    }
    else {
        MPI_Recv(B,20,MPI_INT, 0, 100, MPI_COMM_WORLD, &thongbao) ;
        for(i=0; i<20; i++)  
            *(C+i) = 2* *(B+i) ;
    }

    if(rank==1) 
        for(i=0; i<20; i++) 
            printf("%d \t", *(C+i)) ;
    printf("\n") ;
    MPI_Finalize();
    return 0;

}