#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
// #include <omp.h>
#include <mpi/mpi.h>
#include "mpi_laplace_equation.h"

void main()
{
    mpi_laplace_equation();
}