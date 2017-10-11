#include <omp.h>
#include <stdio.h>
int main() {
	int threads, id;
	threads = 10;
	omp_set_num_threads(10) ;
	
	#pragma omp parallel
	{
		id= omp_get_thread_num() ;
		printf("Hello from thread %d, nthreads %d \n", id, threads);
	}
	
	return 0;
}