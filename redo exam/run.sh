# gcc -fopenmp bt1.c
# gcc -fopenmp bt2.c
# gcc -fopenmp sum1Array.c
# gcc -fopenmp maxInArray.c
# gcc -fopenmp sum2matrix.c
# gcc -fopenmp mulMatrix.c
# ./a.out

# mpicc MPIhello.c -o MPIrun
# mpicc MPIloop.c -o MPIrun
# mpicc MPImypi.c -o MPIrun
mpicc MPImulMatrix.c -o MPIrun
mpirun -np 5 ./MPIrun