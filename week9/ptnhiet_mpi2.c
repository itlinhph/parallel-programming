#include <stdio.h>
#include <conio.h>

#include <malloc.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define C 1
#define m 20
#define T 5 
#define dt 0.01

void displayMatrix (float *A, int row, int col) {
    int i, j ;
    for(i=0; i<row; i++) {
        for(j=0; j<col; j++) 
            printf("  %f", *(A+i*col+j)) ;
        printf("\n");
    }
}

void writeToFile(float *A, int row, int col) {
    FILE *result =fopen("ViberStringMPI1.txt", "a") ;
    int i,j ;
    for(i=0; i<row; i++) {
        for(j=0; j<col; j++) {
            
        }
    }
}