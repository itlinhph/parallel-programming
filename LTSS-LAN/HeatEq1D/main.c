#include <stdio.h>
#include <stdlib.h>

#include "single_thread.h"
#include "spmd.h"

#define INIT_VALUE 25.0 //Nhiệt độ phòng
#define INIT_HEAD 100.0 // Nhiệt độ truyền
#define M 20 // Số khoảng chia
#define dX 0.1 // dx
#define T 1.0 // Thời gian đo
#define dT 0.01 // Khoảng tgian
#define D 0.1 // Hệ số truyền nhiệt
#define THREADS 5 // Số luồng


float* generate_arr(float value, int m) {
	float * arr = (float*) malloc(m * sizeof(float));
	int i;
	for (i=0; i<m; i++) {
		arr[i] = value;
	}
	return arr;
}

void print_arr(float* arr, int m) {
	int i;
	for (i=0; i<m; i++) {
		printf("%.6f ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[]) {
	// Instantiate data
	float* dC = generate_arr(0.0, M); // Độ tăng nhiệt độ trên mỗi khoảng
	float* C1 = generate_arr(INIT_VALUE, M); // Nhiệt độ trên mỗi khoảng
	printf("Initial values:\n");
	print_arr(C1, M);

	printf("\nResult (single thread):\n");
	solve_single(C1, dC, dX, M, T, dT, D);
	print_arr(C1, M);

	float* C2 = generate_arr(INIT_VALUE, M); // Tương tự C1, dùng cho song song
	printf("\nResult (%d threads):\n", THREADS);
	solve_spmd(C2, dC, dX, M, T, dT, D, THREADS);
	print_arr(C2, M);

	return 0;
}
