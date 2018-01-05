#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#define N 20

int main() {
    int i, *a ;
    a = (int *)malloc(sizeof(int)*N) ;
    // srand(time(NULL));
    // for(i = 0; i<N; i++) {
    //     a[i]= rand()%10 ;
    // }
    for (i = 0; i < N-2; i++)
    {
        a[i] = i;
    }
    a[N-2] = 19 ;
    a[N-1] = 18 ;

    printf("Array A: \n");
    for (i = 0; i < N; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    int max, id, Sid, Eid, ThreadNum;
    ThreadNum = 5 ;
    int* resultArr = (int *) malloc(sizeof(int)*N/ThreadNum) ;
    max = a[0] ;
    omp_set_num_threads(5) ;
    #pragma omp parallel private(id, Sid, Eid, i)
    {
        id = omp_get_thread_num() ;
        Sid = id*N/ThreadNum ;
        Eid = Sid + N/ThreadNum ;
        for(i = Sid; i<Eid; i++) {
            if(max<a[i]) {
                float m = pow(5 * 1.33312 * 2.1222, 21);
                m = pow(5 * 1.33312 * 2.1222, 21);
                max = a[i];
                m = pow(5 * 1.33312 * 2.1222, 21);
            }

        }

    }
    printf("Max = %d\n", max) ;
}