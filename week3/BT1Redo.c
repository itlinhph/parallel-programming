#include <stdio.h>
#include <omp.h>

int main() {
	int id;
	omp_set_num_threads(10) ;

	#pragma omp parallel
	{
		id = omp_get_thread_num() ;
		printf("Hello from thread %d, nthreads 10\n", id );

	}
	return 0; 
}