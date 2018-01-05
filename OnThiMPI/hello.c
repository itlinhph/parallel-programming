#include <stdio.h>
#include <mpi.h>

int main() {
    int size, rank;
    MPI_Init(NULL, NULL) ;
    MPI_Comm_size(MPI_COMM_WORLD, &size) ; // get current process id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ; // get number of process
    printf("Hello from %d 0f %d\n", rank, size) ;

    MPI_Finalize();
    return 0;
}