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

	int chunk_size = (int) ceil((float)H1 / (float)size);
	float * As;
	float * Cs;

	if (rank == 0) {
		A = createA(chunk_size * size, WH);
		print_mat("A =\n", A, H1, WH); 

		B = createB(WH, W2);
		print_mat("B =\n", B, WH, W2);

		C = zeros(chunk_size * size, W2);

		As = A;
		for (int id=1; id<size; id++) {
			MPI_Send(A + id*chunk_size*WH, chunk_size * WH, MPI_FLOAT, id, AS_TAG, MPI_COMM_WORLD);
			MPI_Send(B, WH * W2, MPI_FLOAT, id, B_TAG, MPI_COMM_WORLD);
		}
	} else {
		As = (float*) malloc (chunk_size * WH * sizeof(float));
		B = (float*) malloc (WH * W2 * sizeof(float));
		MPI_Recv(As, WH * chunk_size, MPI_FLOAT, 0, AS_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(B, WH * W2, MPI_FLOAT, 0, B_TAG, MPI_COMM_WORLD, &status);
	}

	Cs = matmul(As, B, chunk_size, WH, W2);

	if (rank == 0) {
		// Assign Cs(0) to C
		for (int i=0; i<chunk_size; i++) {
			for (int j=0; j<W2; j++) {
				*(C + i*W2 + j) = *(Cs + i*W2 + j);
			}
		}
		// Get Cs(id) and assign to C
		for (int id=1; id<size; id++) {
			MPI_Recv(C + id*W2*chunk_size, chunk_size * W2, MPI_FLOAT, id, CS_TAG, MPI_COMM_WORLD, &status);
		}

		print_mat("A*B =\n", C, H1, W2);
	} else {
		MPI_Send(Cs, chunk_size * W2, MPI_FLOAT, 0, CS_TAG, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
