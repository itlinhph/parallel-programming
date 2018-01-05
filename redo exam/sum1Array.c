#include <stdio.h>
#include <omp.h>
#include <malloc.h>
#include <math.h>
#define N 20

int main() {
    int sum=0;
    int *a, i ;
    a= (int *)malloc(sizeof(int)*N) ;
    for(i=0; i<N; i++) {
        a[i] =i ;
    }

    int Tid, Tnum, Sid, Eid;
    Tnum = 10;
    omp_set_num_threads(Tnum) ;

    #pragma omp parallel private(Sid, Eid,Tid, i)
    {
        Tid = omp_get_thread_num() ;
        // printf("\n");
        Sid = Tid * N/Tnum ;
        Eid = Sid + N/Tnum ;
        for(i=Sid; i<Eid; i++) {
            float m = pow(5 * 1.33312 * 2.1222, 21);
            m = pow(5 * 1.33312 * 2.1222, 21);
            m = pow(5 * 1.33312 * 2.1222, 21);
            sum+= a[i] ; 
            float m2 = pow(5 * 1.33312 * 2.1222, 21);
            m2 = pow(5 * 1.33312 * 2.1222, 21);
        }
    }
    printf("Array A: \n");
    for(i=0; i<N; i++) {
        printf("%d ", a[i]) ;
    }
    printf("\n");

    printf("Sum= %d\n", sum) ;

}