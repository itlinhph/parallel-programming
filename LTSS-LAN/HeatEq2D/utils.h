#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ROOM_TEMP 25.0
#define CENTER_TEMP 80.0

float * create_mat(int height, int width) {
	float* mat = (float*) malloc (height * width * sizeof(float));
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			if (i >= height/2-5 && i < height/2+5 && j >= width/2 -5 && j < width/2+5) {
				*(mat + i*width + j) = CENTER_TEMP;
			} else {
				*(mat + i*width + j) = ROOM_TEMP;				
			}
		}
	}
	return mat;
}

float * zeros(int height, int width) {
	float* mat = (float*) malloc (height * width * sizeof(float));
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++)
			*(mat + i*width + j) = 0.0;
	}
	return mat;
}

void print_mat(const char* prefix, float * mat, int height, int width) {
	printf("%s", prefix);
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++)
			printf("%.3f ", *(mat + i*width + j));
		printf("\n");
	}
	printf("\n");	
}

void fd_mpi(float* C, float* dC, float D, float dx, int height, 
	int width, float* UN, float* DN) {
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			float c = *(C + i*width + j);
			// Get adjacent values unless boundary is reached
			float l = (j==0) ? ROOM_TEMP : *(C + i*width + j - 1); 
			float r = (j==width-1) ? ROOM_TEMP : *(C + i*width + j + 1);
			float u = (i==0) ? *(UN + j) : *(C + (i-1)*width + j); 
			float d = (i==height-1) ? *(DN + j) : *(C + (i+1)*width + j);
	
			// Update value
			*(dC + i*width + j) = (D / (dx * dx)) * (u + d + l + r - 4 * c);
		}
	}
}

void fd_omp(float* C, float* dC, float D, float dx, int height, 
	int width, int start, int end) {
	for (int i=start; i<end; i++) {
		for (int j=0; j<width; j++) {
			float c = *(C + i*width + j);
			// Get adjacent values unless boundary is reached
			float l = (j==0) ? ROOM_TEMP : *(C + i*width + j - 1); 
			float r = (j==width-1) ? ROOM_TEMP : *(C + i*width + j + 1);
			float u = (i==0) ? ROOM_TEMP : *(C + (i-1)*width + j); 
			float d = (i==height-1) ? ROOM_TEMP : *(C + (i+1)*width + j);
	
			// Update value
			*(dC + i*width + j) = (D / (dx * dx)) * (u + d + l + r - 4 * c);
		}
	}
}
