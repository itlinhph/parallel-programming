gcc -fopenmp main_omp.c -o main_omp && ./main_omp
mpicc main_mpi.c -o main_mpi && mpirun -np 4 ./main_mpi
