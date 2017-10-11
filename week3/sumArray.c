#include <stdio.h>
#include <omp.h>
#include <malloc.h>
#define N 105
int main() {
	int sum, id, i ;
	
	int *a ;
	a= (int *) malloc(sizeof(int) *N) ;
	for(i=0; i<N; i++) {
		a[i] = i ;
	}

	int Nid, Sid, Eid, T=10;
	omp_set_num_threads(T) ;
	#pragma omp parallel private(i, Sid, Eid)
	{
		Nid= omp_get_thread_num() ;
		Sid = Nid*N/T;
		Eid = Sid + N/T ;

		for(i=Sid; i<Eid; i++) {
			sum+= a[i] ;
		}

	}
	printf("Sum: %d\n",sum );
	return 0;
}