#include <stdio.h>
#include <malloc.h>
#include <omp.h>

void find_max_matrix(){
    int **a, *b, i, j, n;
    printf("Nhap kich thuoc mang 2 chieu: ");
    scanf("%d", &n);
    a = (int **) malloc(n*sizeof(int));
    b = (int *) malloc(n*sizeof(int));
    for(i = 0; i < n; i++){
        a[i] = (int *) malloc(n * sizeof(int));
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("a[%d][%d] = ", i, j);
            scanf("%d", *(a+i)+j);
        }
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("a[%d][%d] = %d      ", i, j, *(*(a+i) + j));
        }
        printf("\n");
    }

    omp_set_num_threads(n);
#pragma omp parallel
    {
        int id;
        id = omp_get_thread_num();
        *(b + id) = find_max(*(a + id), n);
    }
    printf("%d", find_max(b, n));
}

