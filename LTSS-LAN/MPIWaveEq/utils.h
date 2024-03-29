#include <stdlib.h>
#include <stdio.h>
#include <math.h>

float * create_arr(int length) {
	float* arr = (float*) malloc (length * sizeof(float));
	for (int i=0; i<length; i++) {
		*(arr + i) = sin(2.0 * M_PI * (float)i / (float)length);
	}
	return arr;
}

float * zeros(int length) {
	float* arr = (float*) malloc (length * sizeof(float));
	for (int i=0; i<length; i++) {
		*(arr + i) = 0.0;
	}
	return arr;
}

void print_arr(const char* prefix, float * arr, int length) {
	printf("%s", prefix);
	for (int i=0; i<length; i++) {
			printf("%.6f ", *(arr + i));
	}
	printf("\n");	
}

void fd(float* C, float* dC, float D, float dx, int length, float LN, float RN) {
	for (int i=0; i<length; i++) {
		float c = C[i];
		// Get adjacent values unless boundary is reached
		float u = (i==0) ? LN : C[i-1]; 
		float d = (i==length-1) ? RN : C[i+1];

		// Update value
		dC[i] = (D / (dx * dx)) * (u + d - 2 * c);
	}
}
