#include <omp.h>
#include <stdio.h>
int main() {
	int id,x;
	omp_set_num_threads(98) ;
	#pragma omp parallel private(id,x) // cac bien private se co nhung ban copy rieng

	{
		id= omp_get_thread_num();
		x=10*id;
		printf("\n");
		printf("hello from thread %d, x= %d", id, x);
		printf("\n");
	}
	return 0;
}
