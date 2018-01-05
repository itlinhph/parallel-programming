#include <omp.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>

//int main(){
//    int threads, id;
//    threads = 10;
//    id = 10;
//    omp_set_num_threads(10);
//#pragma omp parallel
//    printf("Hello from thread %d, nthreads %d\n",
//    omp_get_thread_num(),
//    omp_get_num_threads());
//}
int main(){
    int *a, *b, *c, i, id, sid, eid, max = -1;
    a = (int *) malloc(100 * sizeof(int));
    b = (int *) malloc(100 * sizeof(int));
    c = (int *) malloc(100 * sizeof(int));
    for(i = 0; i < 100; i++){
        *(a+i) = i;
        *(b+i) = i * i;
    }
    omp_set_num_threads(10);
#pragma omp parallel private(i, id, sid, eid)
    {
        id = omp_get_thread_num();
        sid = id * 10;
        eid = (id + 1) * 10;
        for(i = sid; i < eid; i++){
            if(*(a+i) > max){
                max = *(a+i);
            }
        }
    }
    printf("%d", max);
    printf("\n");
}

