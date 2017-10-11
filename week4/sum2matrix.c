#include <stdio.h>
#include <omp.h>

#define N 4 // hang
#define M 2 // cot

int main() 
{
	int a[M][N] ;
	int b[M][N] ;
	int c[M][N] ;
	int i,j ;

	for(i=0; i<M; i++) {
		for(j=0; j<N; j++) {
			a[i][j] = i+j ;
			b[i][j] = 2*i +2*j ;
		}
	}

	

	int id, Sid, Eid, Nid, T=2 ; // T: so luong
	omp_set_num_threads(T) ;
	#pragma omp parallel private(id, Sid, Eid, Nid, i, j)
	{
		// dung luong tinh theo hang
		id= omp_get_thread_num() ;
		Nid = M/T ; // SO HANG MA MOI LUONG CAN XU LY
		Sid = Nid*id ;
		Eid = Sid +Nid;
		for(i=Sid; i<Eid; i++) {
			for(j=0; j<M; j++) {
				c[i][j] = a[i][j] + b[i][j] ;
			}
		}
	}

	for(i=0; i<N; i++) {
		for(j=0; i<M; j++) {
			printf("\t %d",c[i][j] );

		}
		printf("\n");
	}



}

