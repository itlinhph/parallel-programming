#include <mpi.h>

#include "utils.h"

#define M 20
#define N 20
#define T 2.0
#define dt 0.01
#define dx 0.1
#define D 0.1

#define UPTAG 100
#define DOWNTAG 101

int main(int argc, char* argv[]) {
	int rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	int chunk_size = M / size;
	float *C, *dC, *UN, *DN, *Cs;

	// Set UN & DN for bounds
	UN = (float*) malloc (sizeof(float) * N);
	DN = (float*) malloc (sizeof(float) * N);
	if (rank == 0) {
		for (int i=0; i<N; i++) {
			*(UN + i) = ROOM_TEMP;
		}
	} else if (rank == size-1) {
		for (int i=0; i<N; i++) {
			*(DN + i) = ROOM_TEMP;
		}
	}

	// Initialize and scatter
	if (rank == 0) {
		C = create_mat(M, N);
	}

	dC = zeros(M, N);
	Cs = (float*) malloc (sizeof(float) * chunk_size * N);
	MPI_Scatter(C, chunk_size*N, MPI_FLOAT, Cs, chunk_size*N, MPI_FLOAT, 0, MPI_COMM_WORLD);

	for (float t=0.0; t<T; t+=dt) {
		// Send upmost row
		if (rank != 0) {
			MPI_Send(Cs, N, MPI_FLOAT, rank-1, rank, MPI_COMM_WORLD);
		}

		// Receive upmost row as DN + Send bottom-most row
		if (rank != size-1) {
			DN = (float*) malloc (sizeof(float) * N);
			MPI_Recv(DN, N, MPI_FLOAT, rank+1, rank+1, MPI_COMM_WORLD, &status);
			MPI_Send(Cs + (chunk_size-1)*N, N, MPI_FLOAT, rank+1, rank + size, MPI_COMM_WORLD);
		}

		// Receive bottom-most row as UN
		if (rank != 0) {
			UN = (float*) malloc (sizeof(float) * N);			
			MPI_Recv(UN, N, MPI_FLOAT, rank-1, rank - 1 + size, MPI_COMM_WORLD, &status);
		}
		
		// Calculate & update value
		fd_mpi(Cs, dC, D, dx, chunk_size, N, UN, DN);
		for (int i=0; i<chunk_size; i++) {
			for (int j=0; j<N; j++) {
				*(Cs + i*N + j) += dt * *(dC + i*N + j);
			}
		}
	}

	MPI_Gather(Cs, chunk_size * N, MPI_FLOAT, C, chunk_size * N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		print_mat("Final state (MPI):\n", C, M, N);
	}

	MPI_Finalize();
	return 0;
}
