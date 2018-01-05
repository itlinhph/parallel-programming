#!/usr/bin/env bash
mpicc main.c
mpirun -np 5 ./a.out