#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


void init_mat(int** mat, int height, int width) {
	int i, j;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			mat[i][j] = i + j;
		}
	}
}

int single_sum(int** mat, int height, int width) {
	int sum = 0;
	int i, j;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			sum += mat[i][j];
		}
	}
	return sum;
}

int horiz_para_sum(int** mat, int height, int width, int threads) {
	int sum = 0;
	int i, j;
	int chunk_size = height/threads;
	omp_set_num_threads(threads);
	# pragma omp parallel private(i,j)
	{
		int tsum = 0;
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;
		for (i=start; i<end; i++) {
			for (j=0; j<width; j++) {
				tsum += mat[i][j];
			}
		}
		sum += tsum;
	}
	return sum;
}

int vert_para_sum(int** mat, int height, int width, int threads) {
	int sum = 0;
	int i, j;
	int chunk_size = width/threads;
	omp_set_num_threads(threads);
	# pragma omp parallel private(i,j)
	{
		int tsum = 0;
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;
		for (i=start; i<end; i++) {
			for (j=0; j<height; j++) {
				tsum += mat[j][i];
			}
		}
		sum += tsum;
	}
	return sum;
}

int main() {
	int NH = 100, NW = 100, M = 10;
	int ** A = (int**) malloc(NH * sizeof(int*));
	int i;
	for (i=0; i<NH; i++) {
		A[i] = (int*) malloc(NW * sizeof(int));
	}

	init_mat(A, NH, NW);
	int C = single_sum(A, NH, NW);
	printf("Single sum result: %d\n", C);

	int C2 = horiz_para_sum(A, NH, NW, M);
	printf("Horizontal parallel sum result: %d\n", C2);

	int C3 = vert_para_sum(A, NH, NW, M);
	printf("Vertical parallel sum result: %d\n", C3);
}
