#include <stdio.h> 
#include <mpi.h>

#define N 1010

int main()
{
    int i, rank, ntasks, count, start, stop, nloops, total_nloops;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    count =N/ntasks ;
    start = rank*count;
    stop= start +count;
    nloops = 0;
    for (i =start;i <stop; ++i) {
        ++nloops;
    }
    printf("Task %d performed %d iterations of the loop from %d to %d \n" ,rank, nloops, start, stop);
    if (rank != 0) {
        MPI_Send(&nloops, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        total_nloops = nloops;
        for (i = 1; i <ntasks; ++i) {
            MPI_Recv(&nloops, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_nloops +=nloops;
        }
        nloops = 0;
        for (i = total_nloops; i < N; ++i) {
            ++nloops;
        }
        printf("Task 0 performed the remaining %d iterations of the loop\n", nloops);
    }
    MPI_Finalize();
    return 0;
}