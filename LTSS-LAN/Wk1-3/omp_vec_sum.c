#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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
		arr[i] = i;
	}
}

int single_sum(int * v, int length) {
	int sum = 0, i;
	for (i=0; i<length; i++) {
		sum += i;
	}
	return sum;
}

int para_sum(int * v, int length, int threads) {
	int sum = 0, i;
	int part_len = length/threads;

	omp_set_num_threads(threads);
	#pragma omp parallel private(i)
	{
		int start = omp_get_thread_num() * part_len;
		int end = start + part_len;
		for (i=start; i<end; i++) {
			sum += v[i];
		}
	}
	return sum;
}

int main() {
	int N = 100, M = 10;
	int * A = (int*) malloc(N * sizeof(int));

	init_arr(A, N);
	int C = single_sum(A, N);
	printf("Single sum result: %d\n", C);

	int C2 = para_sum(A, N, M);
	printf("Parallel sum result: %d\n", C2);
}
