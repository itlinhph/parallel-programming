#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int main() {
    srand(time(NULL));
    int row, column, **mat1, **mat2, **mat3, num_threads, i, j;
    printf("Nhap so dong va cot: ");
    scanf("%d %d", &row, &column);
    mat1 = (int **) malloc(row * sizeof(int));
    mat2 = (int **) malloc(row * sizeof(int));
    mat3 = (int **) malloc(row * sizeof(int));
    for(i = 0; i < row; i++){
        *(mat1 + i) = (int *) malloc(column * sizeof(int));
        *(mat2 + i) = (int *) malloc(column * sizeof(int));
        *(mat3 + i) = (int *) malloc(column * sizeof(int));
    }

    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            mat1[i][j] = rand()%101 + 10;
            mat2[i][j] = rand()%101 + 10;
            printf("%d %d", mat1[i][j], mat2[i][j]);
        }
    }
    printf("Nhap so luong: ");
    scanf("%d", &num_threads);
    omp_set_num_threads(num_threads);
#pragma omp parallel private(i, j)
    {
        int id = omp_get_thread_num();
        int sid = i/num_threads * id;
        int eid = sid + i/num_threads;

        for(i = sid; i < eid; i++){
            for(j = 0; j < column; j++){
                mat3[i][j] = mat1[i][j] + mat2[i][j];
            }
        }
    }
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("a[%d][%d] = %3d   ", i, j, *(*(mat3+i) + j));
        }
        printf("\n");
    }
}
