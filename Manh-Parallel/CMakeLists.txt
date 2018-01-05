cmake_minimum_required(VERSION 3.8)
project(parallel_programming)

set(CMAKE_C_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(SOURCE_FILES main.c test.h sample.h find_max_array.h max_in_matrix.h multiple_matrix.h heat_equation_seq.h heat_equation_par.h wave_equation_seq.h mpi_matrix_multiplication2.h mpi_heat_equation.h)
add_executable(parallel_programming ${SOURCE_FILES})