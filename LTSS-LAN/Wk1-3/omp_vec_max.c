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

int single_max(int * v, int length) {
	int max = -9999, i;
	for (i=0; i<length; i++) {
		if (v[i] > max) max = v[i];
	}
	return max;
}

int para_max(int * v, int length, int threads) {
	int max = -9999, i;
	int * local_max = (int*) malloc(sizeof(int) * threads);
	int part_len = length/threads;

	omp_set_num_threads(threads);
	#pragma omp parallel private(i)
	{
		int id = omp_get_thread_num();
		int start = id * part_len;
		int end = start + part_len;
		local_max[id] = -9999;
		for (i=start; i<end; i++) {
			if (v[i] > local_max[id]) local_max[id] = v[i];
		}
	}

	for (i=0; i<threads; i++) {
		if (local_max[i] > max) max = local_max[i];
	}

	return max;
}

int para_binary_max(int * v, int length) {
	int i = 0;
	int threads = length / 2;
	int max = v[0];
	int * prev_max = v;
	int arr_len = length;

	while (threads > 1) {
		int * local_max = (int*) malloc (sizeof(int) * threads);
		// print_arr(prev_max, arr_len);
		omp_set_num_threads(threads);
		// printf("%d\n", threads);
		# pragma omp parallel private(i)
		{
			int id = omp_get_thread_num();
			int start = 2 * id;
			int end = start + 1;
			if (end > arr_len) {
				local_max[id] = prev_max[start];
			}
			else if (prev_max[start] > prev_max[end]) local_max[id] = prev_max[start];
			else local_max[id] = prev_max[end];
		}
		arr_len = threads;
		threads = (threads + 1) / 2;
		prev_max = local_max;
	}

	if (prev_max[0] > prev_max[1]) max = prev_max[0];
	else max = prev_max[1];

	return max;
}

int main() {
	int N = 100, M = 10;
	int * A = (int*) malloc(N * sizeof(int));

	init_arr(A, N);
	int C = single_max(A, N);
	printf("Single max result: %d\n", C);

	int C2 = para_binary_max(A, N);
	printf("Parallel binary max result: %d\n", C2);
}
