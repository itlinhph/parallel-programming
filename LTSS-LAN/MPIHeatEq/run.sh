cd ../HeatEq1D
./run.sh
cd ../MPIHeatEq
echo "=========="
mpicc main.c -o main && mpirun -np 5 ./main
