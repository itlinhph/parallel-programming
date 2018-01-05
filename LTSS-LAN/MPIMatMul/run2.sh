./run.sh
echo "-------"
mpicc main2.c -o main2 -lm && mpirun -np 4 ./main2 
