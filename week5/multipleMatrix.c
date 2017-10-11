#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <malloc.h>

#define m 8
#define n 8
#define p 4

// void displayMatrix(int **A, int row, int col) {
// 	int i, j;
// 	for(i=0; i<row; i++) {
// 		for(j=0; j<col; j++) {
// 			printf("%d\t", *(A+i*col+j));
			
// 		}
// 		printf("\n");
// 	}

// }

int main(int argc, char *argv[]) {
	int i, j , k;
	int id, nthreads, mc, starti ;

	// int **A, **B, **C ;
	// A = (int *) malloc((m*n)* sizeof(int)) ;
	// B = (int *) malloc((n*p)* sizeof(int)) ;
	// C = (int *) malloc((m*p)* sizeof(int)) ;
	int A[8][8], B[8][4],C[8][4];
	

	// KHoi tao du lieu A,B,C
	// MA TRAN A LA MA TRAN DON VI
	for(i=0; i<m; i++) {
		for(j=0; j<n; j++) {
			if(i==j)
				A[i][j] = 1.0 ;
			else 
				A[i][j] = 0.0 ;
		}
	}
	printf("\n\nMATRIX A: \n");
	int row = m,col = n;
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			printf("%d\t", A[i][j]); //*(A+i*col+j));
			
		}
		printf("\n");
	}


	// Ma tran B
	for(i=0; i<n; i++) {
		for(j=0; j<p; j++) {
		B[i][j]= 1.0 * (i*p+j) ;
		}	
	}
	row = n;col = p;
	printf("\n\nMatrix B: \n");
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			printf("%d\t",B[i][j]);// *(B+i*col+j));
			
		}
		printf("\n");
	}


	// NHan ma tran: A*B = C
	omp_set_num_threads(8) ;
#pragma omp parallel private(id,starti, i,j,k)
	{
		nthreads = omp_get_num_threads() ;
		id= omp_get_thread_num() ;
		mc= m/nthreads;
		starti = id*mc;
		for(i=starti ; i<starti + mc; i++) {
			for(j=0; j<p; j++) {
				C[i][j] = 0;
				for(k=0; k<n; k++) {
					//*(C+i*p +j) =*(C+i*p+j) + (*(A+i*n +k))*(*(B+k*p+j)) ;
					C[i][j] += A[i][k] * B[k][j] ;
				}
			}
		}
		#pragma omp barrier
	}
	#pragma omp barrier

	printf("\nMatran C:\n");
	row = n;col = p;
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			printf("%d\t",C[i][j]);// *(C+i*col+j));
			
		}
		printf("\n");
	}


	return 0;

}