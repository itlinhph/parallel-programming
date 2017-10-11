#include <stdio.h>
#include <omp.h>
int main() {
	#pragma omp parallel
	printf("Hello From thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads() );
	// printf("Thread Num: %d\n", OMP_NUM_THREADS );
	return 0;	
}