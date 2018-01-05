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

int** single_mul(int** mat1, int** mat2, int height1, int wh, int width2) {
	int ** result =  (int**) malloc(height1 * sizeof(int*));
	int i, j;
	for (i=0; i<height1; i++) {
		result[i] = (int*) malloc(width2 * sizeof(int));
	}

	for (i=0; i<height1; i++) {
		for (j=0; j<width2; j++) {
			result[i][j] = 0;
			int k;
			for (k=0; k<wh; k++) {
				result[i][j] += mat1[i][k] * mat2 [k][j];
 			}
		}
	}
	return result;
}

int** para_mul(int** mat1, int**mat2, int height1, int wh, int width2, int threads) {
	int ** result = (int**) malloc(height1 * sizeof(int*));
	int i, j;
	for (i=0; i<height1; i++) {
		result[i] = (int*) malloc(width2 * sizeof(int));
	}
	int chunk_size = wh / threads;
	omp_set_num_threads(threads);

	int k, buffer;
	for (i=0; i<height1; i++) {
		for (j=0; j<width2; j++) {
			result[i][j] = 0;
			# pragma omp parallel private(k, buffer)
			{
				int id = omp_get_thread_num();
				int start = id * chunk_size;
				int end = start + chunk_size;
				buffer = 0;
				for (k=start; k<end; k++) {
					buffer += mat1[i][k] * mat2 [k][j];
				}
				result[i][j] += buffer;
			}
		}
	}

	return result;
}

int** vert_para_mul(int** mat1, int**mat2, int height1, int wh, int width2, int threads) {
	int ** result =  (int**) malloc(height1 * sizeof(int*));
	int i, j;
	for (i=0; i<height1; i++) {
		result[i] = (int*) malloc(width2 * sizeof(int));
	}

	int chunk_size = width2 / threads;

	omp_set_num_threads(threads);
	# pragma omp parallel private(i, j)
	{
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;

		for (i=0; i<height1; i++) {
			for (j=start; j<end; j++) {
				result[i][j] = 0;
				int k;
				for (k=0; k<wh; k++) {
					result[i][j] += mat1[i][k] * mat2 [k][j];
				}
			}
		}
	}
	
	return result;
}

int** horiz_para_mul(int** mat1, int**mat2, int height1, int wh, int width2, int threads) {
	int ** result =  (int**) malloc(height1 * sizeof(int*));
	int i, j;
	for (i=0; i<height1; i++) {
		result[i] = (int*) malloc(width2 * sizeof(int));
	}

	int chunk_size = height1 / threads;

	omp_set_num_threads(threads);
	# pragma omp parallel private(i, j)
	{
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;

		for (i=start; i<end; i++) {
			for (j=0; j<width2; j++) {
				result[i][j] = 0;
				int k;
				for (k=0; k<wh; k++) {
					result[i][j] += mat1[i][k] * mat2 [k][j];
				}
			}
		}
	}
	
	return result;
}

int main() {
	int H1 = 5, WH = 5, W2 = 5, M = 5;
	int ** A = (int**) malloc(H1 * sizeof(int*));
	int** B = (int**) malloc(WH * sizeof(int*));
	int i;
	for (i=0; i<H1; i++) {
		A[i] = (int*) malloc(WH * sizeof(int));
	}
	for (i=0; i<WH; i++) {
		B[i] = (int*) malloc(W2 * sizeof(int));
	}

	init_mat(A, H1, WH);
	init_mat(B, WH, W2);
	int** C = single_mul(A, B, H1, WH, W2);
	printf("Single mulitplication result:\n");
	print_mat(C, H1, W2);

	int** C2 = horiz_para_mul(A, B, H1, WH, W2, M);
	printf("Horizontal parallel multiplication result:\n");
	print_mat(C2, H1, W2);

	int** C3 = vert_para_mul(A, B, H1, WH, W2, M);
	printf("Vertical parallel multiplication result:\n");
	print_mat(C3, H1, W2);
}
