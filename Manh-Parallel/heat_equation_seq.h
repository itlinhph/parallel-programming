#define M 20
#define Time 1.0
#define dt 0.01
#define dx 0.1
#define D 0.1

char *convert(char *c, int n){
    char a[10];
    sprintf(a, "%d", n);
    char b[10] = ".txt";
    strcpy(a + strlen(a), b);
    strcpy(c + 75, a);
    return c;
}


void DHB2(float *T, float *dT){
    int i;
    float c, l, r;
    for(i = 0; i < M; i++) {
        c = *(T + i);
        l = (i == 0) ? 100 : *(T + i - 1);
        r = (i == M - 1) ? 25 : *(T + i + 1);
        *(dT + i) = D * (l - 2 * c + r) / (dx * dx);
    }
}

void heat_equation_seq(){
    FILE *f;
    char A[255] = "/home/duymanh/Dropbox/CLionProjects/parallel_programming/heat_equation_seq/";
    int Ntime = Time/dt;
    int i, t;
    float *T, *dT;
    T = (float *) malloc(M * sizeof(float));
    for(i = 0; i < M; i++){
        *(T+i) = 25;
    }
    dT = (float *) malloc(M * sizeof(float));

    for(t = 0; t <= Ntime; t++) {
        DHB2(T, dT);
        for (i = 0; i < M; i++) {
            *(T + i) += *(dT + i) * dt;
        }
        f = fopen(convert(A, t), "wt");
        for (int k = 0; k < M; k++) {
            fprintf(f, "%f  ", *(T + k));
        }
        fclose(f);

        for(int k = 0; k < M; k++){
            printf("%f  ", *(T+k));
        }
        printf("\n");
    }
}
