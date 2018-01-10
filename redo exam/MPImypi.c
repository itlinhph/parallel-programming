#include <stdio.h>
#include <mpi.h>
// #define n 20
int main()
{
    int myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;

    int n = 10;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = 1.0 / (double)n;
    sum = 0.0;
    for(i = myid + 1; i <= n; i += numprocs) {
        x= h*((double)i -0.5) ;
        sum += (4.0 / (1.0 + x*x));
    }
    mypi =h*sum ;
    // printf("My pi= %.8f\n", mypi) ;
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myid ==0)
        printf(" Real PI is %.8f, Pi is %.8f, mypi in process %d is %.8f\n ", PI25DT, pi, myid, pi);
   
    MPI_Finalize();
    return 0;
}