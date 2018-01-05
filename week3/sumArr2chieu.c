#include <stdio.h>
#include <malloc.h>
#define N 1000

#include <omp.h>

int main() {
	int *a, Stt=0, Sss=0;
	int i;
	a= (int *)malloc(sizeof(int) *N);
	for(i=0; i<N; i++) {
		*(a+i) = i;
	}
	for (i = 0; i < N; i++) 
		Stt = Stt + *(a+i) ;
	printf("Tong tuan tu = %d\n", Stt) ;

	int Nid, id, Sid, Eid, T=10;
	omp_set_num_threads(T) ;
	#pragma omp parallel private(i,id,Sid,Eid)
	{
		id= omp_get_thread_num();
		Nid=N/T ;
		Sid= id*Nid ;
		Eid = Sid + Nid;
		for(i = Sid; i<Eid; i++) 
			Sss = Sss + *(a+i) ;
	}
	for(i=0; i<30; i++) {
		printf("%d \t", *(s+i));
	}
	printf("\n");
	return 0;
}