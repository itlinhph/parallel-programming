#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int * single_add(int* a, int* b, int length) {
	int* C = (int*) malloc(length * sizeof(int));
	int i;
	for (i=0; i<length; i++) {
		C[i] = a[i] + b[i];
	}
	return C;
}

int * para_add(int* a, int* b, int length, int threads) {
	int* C = (int*) malloc(length * sizeof(int));
	int i;
	int part_len = length/threads;

	omp_set_num_threads(threads);
	#pragma omp parallel private(i)
	{
		int start = omp_get_thread_num() * part_len;
		int end = start + part_len;
		for (i=start; i<end; i++) {
			C[i] = a[i] + b[i];
		}
	}

	return C;
}

void print_arr(int* arr, int length) {
	int i;
	for (i=0; i<length; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void init_arr(int* arr, int length) {
	int i;
	for (i=0; i<length; i++) {
		arr[i] = i + 2;
	}
}

int main() {
	int N = 100, M = 10;
	int * A = (int*) malloc(N * sizeof(int));
	int * B = (int*) malloc(N * sizeof(int));

	init_arr(A, N); init_arr(B, N);
	int * C = single_add(A, B, N);
	printf("Single add result:\n");
	print_arr(C, N);

	int * C2 = para_add(A, B, N, M);
	printf("Parallel add result:\n");
	print_arr(C2, N);
}
