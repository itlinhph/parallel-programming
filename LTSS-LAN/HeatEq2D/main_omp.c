#include <omp.h>

#include "utils.h"

#define M 20
#define N 20
#define T 2.0
#define dt 0.01
#define dx 0.1
#define D 0.1
#define THREADS 4

int main (int argc, char* argv[]) {
	float* C = create_mat(M, N);
	float* dC = zeros(M, N);

	print_mat("Initial state:\n", C, M, N);

	omp_set_num_threads(THREADS);
	int chunk_size = M / THREADS;

	int i, j, id, start, end;
	float t;
	#pragma omp parallel private(t, id, i, j, start, end)
	{
		id = omp_get_thread_num();
		start = id * chunk_size;
		end = start + chunk_size;

		for (t=0.0; t<=T; t+=dt) {
			# pragma omp barrier
			fd_omp(C, dC, D, dx, M, N, start, end);
			#pragma omp barrier
			for (i=start; i<end; i++) {
				for (j=0; j<N; j++) {
					*(C + i*N + j) += dt * *(dC + i*N + j);
				}
			}
		}
	}

	print_mat("Final state (OMP):\n", C, M, N);

	return 0;
}
