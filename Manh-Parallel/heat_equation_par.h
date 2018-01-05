#include "heat_equation_seq.h"

void heat_equation_par(){
    FILE *f;
    int sid, eid, id;
    float c, l, r;
    char A[255] = "/home/duymanh/Dropbox/CLionProjects/parallel_programming/heat_equation_par/";
    int Ntime = Time/dt;
    int i, t;
    float *T, *dT;
    T = (float *) malloc(M * sizeof(float));
    for(i = 0; i < M; i++){
        *(T+i) = 25.0f;
    }
    dT = (float *) malloc(M * sizeof(float));
    for(i = 0; i < M; i++){
        printf("%f  ", T[i]);
    }
    printf("\n");
    omp_set_num_threads(4);
    for(t = 0; t <= Ntime; t++){
#pragma omp parallel private(i, id, sid, eid, t, c, l, r)
    {
    id = omp_get_thread_num();
    sid = id * M / 4;
    eid = sid + M / 4;
#pragma omp barrier
    for(i = sid; i < eid; i++) {
        printf("%d\n", i);
        c = *(T + i);
        l = (i == 0) ? 100 : *(T + i - 1);
        r = (i == M - 1) ? 25 : *(T + i + 1);
        *(dT + i) = D * (l - 2 * c + r) / (dx * dx);
    }
#pragma omp barrier
    for (i = sid; i < eid; i++) {
        *(T + i) += *(dT + i) * dt;
    }
#pragma omp barrier
    }
        for(int k = 0; k < M; k++){
            printf("%f  ", *(T+k));
        }
        printf("\n");


        f = fopen(convert(A, t), "wt");
        for(int k = 0; k < M; k++){
            fprintf(f, "%f  ", *(T+k));
        }
        fclose(f);
    }
}