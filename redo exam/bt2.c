#include <stdio.h>
#include <omp.h>

int main() {
    int id, x;
    omp_set_num_threads(20) ;
    #pragma omp parallel private(id,x)
    {
        id= omp_get_thread_num();
        x= 10*id ;
        printf("\n") ;
        printf("Hello from thread %d, x= %d", id, x) ;
        printf("\n") ;
    }
    return 0;
}