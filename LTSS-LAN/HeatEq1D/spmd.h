#include <omp.h>

#ifndef INIT_VALUE
#define INIT_VALUE 25.0
#endif

#ifndef INIT_HEAD
#define INIT_HEAD 100.0
#endif

void partial_FD(float* C, float* dC, float D, float dx, int start, int end, int m) {
	int i;
	for (i=start; i<end; i++) {
		float c = C[i];
		// Get adjacent values unless boundary is reached
		float u = (i==0) ? INIT_HEAD : C[i-1]; 
		float d = (i==m-1) ? INIT_VALUE : C[i+1];

		// Update value
		dC[i] = (D / (dx * dx)) * (u + d - 2 * c);
	}
}

void solve_spmd(float* C, float* dC, float dx, int m, float T, float dt, float D, int threads) {
	float t;
	int i, start, end, id;
	int chunk_size = m / threads;

	omp_set_num_threads(threads);

	#pragma omp parallel private(i, id, start, end, t)
	{
		for (t=0.0; t<=T; t+=dt) {
			id = omp_get_thread_num();
			start = id * chunk_size;
			end = start + chunk_size;

			partial_FD(C, dC, D, dx, start, end, m);
			#pragma omp barrier
			for (i=start; i<end; i++) {
				C[i] = C[i] + dt * dC[i];
			}
			#pragma omp barrier
		}
	}
}
