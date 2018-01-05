#include <stdio.h>
#include <math.h>
#include "solve.h"

#define L 3.2
#define dx 0.1
#define T 10.0
#define dt 0.1
#define C 0.1
#define THREADS 5

float* generate_arr(int m) {
	float * arr = (float*) malloc(m * sizeof(float));
	int i;
	for (i=0; i<m; i++) {
		arr[i] = sin(2.0 * M_PI * (float)i * dx / L);
	}
	return arr;
}

void print_arr(float* arr, int m) {
	int i;
	for (i=0; i<m; i++) {
		printf("%.5f ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	int M = (int)(L / dx) + 1;
	float * W1 = generate_arr(M);
	float * W2 = generate_arr(M);	

	printf("Initial value:\n");
	print_arr(W1, M);

	solve_single(W1, L, dx, T, dt, C);
	printf("Result (single thread):\n");
	print_arr(W1, M);

	solve_parallel(W2, L, dx, T, dt, C, THREADS);
	printf("Result (%d threads):\n", THREADS);
	print_arr(W2, M);

	return 0;
}
