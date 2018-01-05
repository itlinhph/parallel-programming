#include <mpi.h>

#include "utils.h"

#define M 20 
#define dX 0.1
#define T 1.0
#define dT 0.01
#define D 0.1

int main(int argc, char argv[]) {
	int size, rank;
	float *Cs, *dC;
	float LN, RN;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	// Initialize on each thread
	int chunk_size = M / size;
	Cs = create_arr(chunk_size, DEFAULT_RIGHT);
	dC = zeros(chunk_size);

	for (float t=0.0; t<=T; t+=dT) {
		// Send left nodes 
		if (rank != 0) {
			MPI_Send(Cs, 1, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
		}

		// Receive right neighbors then send right nodes
		if (rank < size-1) {
			MPI_Recv(&RN, 1, MPI_FLOAT, rank + 1, rank + 1, MPI_COMM_WORLD, &status);
			MPI_Send(Cs + chunk_size - 1, 1, MPI_FLOAT, rank + 1, rank + size, MPI_COMM_WORLD);
		} else {
			RN = DEFAULT_RIGHT;
		}

		// Receive left neighbors
		if (rank != 0) {
			MPI_Recv(&LN, 1, MPI_FLOAT, rank - 1, rank - 1 + size, MPI_COMM_WORLD, &status);
		} else {
			LN = DEFAULT_LEFT;
		}

		// Update dC
		fd(Cs, dC, D, dX, chunk_size, LN, RN);
		for (int i=0; i<chunk_size; i++) {
			Cs[i] = Cs[i] + dT * dC[i];
		}
	}

	// Create full buffer at p0
	float * C;
	if (rank == 0) {
		C = (float*) malloc(sizeof(float) * M);
	}

	// Gather all buffer
	MPI_Gather(Cs, chunk_size, MPI_FLOAT, C, chunk_size,
			   MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		print_arr("Result (MPI):\n", C, M);

	MPI_Finalize();
	return 0;
}
