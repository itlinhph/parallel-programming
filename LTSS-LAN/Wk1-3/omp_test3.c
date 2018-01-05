#include <omp.h>
#include <stdio.h>

int main() {
    int threads = 100, x, id;
    omp_set_num_threads(threads);

    #pragma omp parallel private(id, x)
    {
        id = omp_get_thread_num();
        x = id * 10;
        printf("\n");
        printf("Hello from %d/%d. x=%d", id, omp_get_num_threads(), x);
        printf("\n");
    }

    return 0;
}
