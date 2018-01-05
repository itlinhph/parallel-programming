#include <omp.h>
#include <stdio.h>

int main() {
    int threads = 10;
    omp_set_num_threads(threads);

    #pragma omp parallel
    printf("Hello from %d/%d\n", omp_get_thread_num(), omp_get_num_threads());

    return 0;
}
