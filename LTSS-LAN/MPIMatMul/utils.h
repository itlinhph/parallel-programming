#include <stdlib.h>
#include <stdio.h>

float * createA(int height, int width) {
	float* mat = (float*) malloc (height * width * sizeof(float));
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			if (i == j) *(mat + i*width + j) = 1;
			else *(mat + i*width + j) = 0;
			// *(mat + i*width + j) = i + j;
		}
	}
	return mat;
}

float * createB(int height, int width) {
	float* mat = (float*) malloc (height * width * sizeof(float));
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			// *(mat + i*width + j) = i - j;
			*(mat + i*width + j) = i*width + j;
		}
	}
	return mat;
}

float * zeros(int height, int width) {
	float* mat = (float*) malloc (height * width * sizeof(float));
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			*(mat + i*width + j) = 0;
		}
	}
	return mat;
}

void print_mat(const char* prefix, float * mat, int height, int width) {
	printf("%s", prefix);
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			printf("%9.4f ", *(mat + i*width + j));
		}
		printf("\n");
	}
}

float * extractColumn(float* mat, int height, int width, int from, int to) {
	int n_width = to-from;
	float* cols = (float*) malloc (sizeof(float) * height * width);
	for (int i=from; i<to; i++) {
		for (int j=0; j<height; j++) {
			*(cols + j*n_width + (i-from)) = *(mat + j*width + i);
		}
	}
	return cols;
}

void applyColumn(float* mat, float* cols, int height, int width, int from, int to) {
	int n_width = to-from;	
	for (int i=from; i<to; i++) {
		for (int j=0; j<height; j++) {
			*(mat + j*width + i) = *(cols + j*n_width + (i-from));
		}
	}
}

float * matmul(float * a, float *b, int h1, int wh, int w2) {
	float * m = zeros(h1, w2);

	for (int i=0; i<h1; i++) {
		for (int j=0; j<w2; j++) {
			for (int k=0; k<wh; k++) {
				*(m + i*w2 + j) += *(a + i*wh + k) * *(b + k*w2 + j);
			}
		}
	}

	return m;
}
