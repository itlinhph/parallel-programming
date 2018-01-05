#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>


void init_mat(int** mat, int height, int width) {
	int i, j;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			mat[i][j] = i + j;
		}
	}
}

void print_mat(int** mat, int height, int width) {
	int i, j;
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

int** single_sum(int** mat1, int** mat2, int height, int width) {
	int ** result =  (int**) malloc(height * sizeof(int*));
	int i, j;
	for (i=0; i<height; i++) {
		result[i] = (int*) malloc(width * sizeof(int));
	}

	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			result[i][j] += mat1[i][j] + mat2[i][j];
		}
	}
	return result;
}

int** horiz_para_sum(int** mat1, int**mat2, int height, int width, int threads) {
	int ** result =  (int**) malloc(height * sizeof(int*));
	int i, j;
	for (i=0; i<height; i++) {
		result[i] = (int*) malloc(width * sizeof(int));
	}

	int chunk_size = height/threads;
	omp_set_num_threads(threads);
	# pragma omp parallel private(i,j)
	{
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;
		for (i=start; i<end; i++) {
			for (j=0; j<width; j++) {
				result[i][j] += mat1[i][j] + mat2[i][j];
			}
		}
	}
	return result;
}

int** vert_para_sum(int** mat1, int**mat2, int height, int width, int threads) {
	int ** result =  (int**) malloc(height * sizeof(int*));
	int i, j;
	for (i=0; i<height; i++) {
		result[i] = (int*) malloc(width * sizeof(int));
	}

	int chunk_size = width/threads;
	omp_set_num_threads(threads);
	# pragma omp parallel private(i,j)
	{
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;
		for (i=start; i<end; i++) {
			for (j=0; j<height; j++) {
				result[j][i] += mat1[j][i] + mat2[j][i];			}
		}
	}
	return result;
}

int** block_para_sum(int** mat1, int**mat2, int height, int width, int threads) {
	int ** result =  (int**) malloc(height * sizeof(int*));
	int i, j;
	for (i=0; i<height; i++) {
		result[i] = (int*) malloc(width * sizeof(int));
	}

	int chunk_size = sqrt((double)(width * height / threads));
	omp_set_num_threads(threads);
	# pragma omp parallel private(i,j)
	{
		int id = omp_get_thread_num();
		int start_row = (id * chunk_size) % height;
		int end_row = start + chunk_size;
		int start_col;
		int end_col;
		for (i=start_row; i<end_row; i++) {
			for (j=start_row; j<end_row; j++) {
				result[i][j] += mat1[i][j] + mat2[i][j];			}
		}
	}
	return result;
}

int main() {
	int NH = 100, NW = 100, M = 100;
	int ** A = (int**) malloc(NH * sizeof(int*));
	int** B = (int**) malloc(NH * sizeof(int*));
	int i;
	for (i=0; i<NH; i++) {
		A[i] = (int*) malloc(NW * sizeof(int));
		B[i] = (int*) malloc(NW * sizeof(int));
	}

	init_mat(A, NH, NW);
	init_mat(B, NH, NW);
	int** C = single_sum(A, B, NH, NW);
	printf("Single add result:\n");
	print_mat(C, NH, NW);

	int** C2 = horiz_para_sum(A, B, NH, NW, M);
	printf("Horizontal parallel add result:\n");
	print_mat(C2, NH, NW);

	int** C3 = vert_para_sum(A, B, NH, NW, M);
	printf("Vertical parallel add result:\n");
	print_mat(C3, NH, NW);

	int** C4 = block_para_sum(A, B, NH, NW, M);
	printf("Block parallel add result:\n");
	print_mat(C4, NH, NW);
}
