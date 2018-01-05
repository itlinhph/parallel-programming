#include <mpi.h>
#include <math.h>

#include "utils.h"

#define H1 8
#define WH 8
#define W2 4

#define AS_TAG 100
#define B_TAG 101
#define CS_TAG 102

int main(int argc, char argv[]) {
	int size, rank;
	float *A, *B, *C;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	int chunk_size = (int) ceil((float)W2 / (float)size);
	float * Bs;
	float * Cs;

	if (rank == 0) {
		A = createA(H1, WH);
		print_mat("A =\n", A, H1, WH); 

		B = createB(WH, chunk_size * size);
		print_mat("B =\n", B, WH, W2);

		C = zeros(H1, chunk_size * size);

		Bs = extractColumn(B, WH, W2, 0, chunk_size);
		float* Bs_;
		for (int id=1; id<size; id++) {
			Bs_ = extractColumn(B, WH, W2, id*chunk_size, id*chunk_size + chunk_size);
			MPI_Send(Bs_, WH * chunk_size, MPI_FLOAT, id, id, MPI_COMM_WORLD);
		}
	} else {
		Bs = (float*) malloc (chunk_size * WH * sizeof(float));
		A = (float*) malloc (H1 * WH * sizeof(float));
		MPI_Recv(Bs, WH * chunk_size, MPI_FLOAT, 0, rank, MPI_COMM_WORLD, &status);
		// print_mat("\nBs =\n", Bs, WH, chunk_size);		
	}

	MPI_Bcast(A, H1 * WH, MPI_FLOAT, 0, MPI_COMM_WORLD);
	Cs = matmul(A, Bs, H1, WH, chunk_size);

	if (rank != 0) {
		MPI_Send(Cs, chunk_size * H1, MPI_FLOAT, 0, rank, MPI_COMM_WORLD);		
	} else {
		// Assign Cs(0) to C
		applyColumn(C, Cs, H1, W2, 0, chunk_size);

		float *Cs_ = (float*) malloc (sizeof(float) * H1 * chunk_size);
		// Get Cs(id) and assign to C
		for (int id=1; id<size; id++) {
			MPI_Recv(Cs_, chunk_size * H1, MPI_FLOAT, id, id, MPI_COMM_WORLD, &status);
			applyColumn(C, Cs_, H1, W2, id*chunk_size, id*chunk_size + chunk_size);
		}

		print_mat("A*B =\n", C, H1, W2);
	}

	MPI_Finalize();
	return 0;
}


