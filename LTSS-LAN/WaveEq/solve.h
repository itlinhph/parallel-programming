#include <omp.h>
#include <stdlib.h>

void spaceDis(float* dW, float* W, int nParts) {
	int i;
	for (i=0; i<nParts; i++) {
		float c = W[i];
		float u = (i==0) ? W[1] : W[i-1];
		float v = (i==nParts-1) ? W[nParts-2] : W[i+1];
		dW[i] = u - 2 * c + v;
	}
} 

void solve_single(float* W, float L, float dx, float T, float dt, float C) {
	int nTimes = (int)(T / dt); 
	int nParts = (int)(L / dx) + 1;
	int i, j;
	float r = (C * dt / dx) * (C * dt / dx);
	
	float * dW = (float*) malloc(sizeof(float) * nParts);	
	float* prevW = (float*) malloc(sizeof(float) * nParts);

	spaceDis(dW, W, nParts);
	for (i=0; i<nParts; i++) {
		prevW[i] = W[i];
		// W[i] += 0.5 * r * dW[i];
	}

	for (i=1; i<nTimes; i++) {
		spaceDis(dW, W, nParts);
		for (j=0; j<nParts; j++) {
			float prevJ = W[j];
			W[j] = 2 * W[j] - prevW[j] + r * dW[j];
			prevW[j] = prevJ; 
		}
	}
}

void spaceDisPartial(float* dW, float* W, int nParts, int start, int end) {
	int i;
	for (i=start; i<end; i++) {
		float c = W[i];
		float u = (i==0) ? W[1] : W[i-1];
		float v = (i==nParts-1) ? W[nParts-2] : W[i+1];
		dW[i] = u - 2 * c + v;
	}
}

void solve_parallel(float* W, float L, float dx, float T, float dt, float C, int threads) {
	int nTimes = (int)(T / dt); 
	int nParts = (int)(L / dx) + 1;
	int i, j, chunk_size = nParts / threads;
	float r = (C * dt / dx) * (C * dt / dx);
	float prevJ;
	float * dW = (float*) malloc(sizeof(float) * nParts);
	
	float* prevW = (float*) malloc(sizeof(float) * nParts);

	omp_set_num_threads(threads);
	# pragma omp parallel private(i, j, prevJ)
	{
		int id = omp_get_thread_num();
		int start = id * chunk_size;
		int end = start + chunk_size;

		spaceDisPartial(dW, W, nParts, start, end);
		#pragma omp barrier
		for (i=start; i<end; i++) {
			prevW[i] = W[i];
			// W[i] += 0.5 * r * dW[i];
		}

		for (i=1; i<nTimes; i++) {
			#pragma omp barrier
			spaceDisPartial(dW, W, nParts, start, end);
			#pragma omp barrier
			for (j=start; j<end; j++) {
				prevJ = W[j];
				W[j] = - 2 * W[j] + prevW[j] + r * dW[j];
				prevW[j] = prevJ; 
			}
		}
	}
}
