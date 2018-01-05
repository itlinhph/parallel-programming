#define  m       50
#define  n       50
#define  T       2
#define  dt      0.01
#define  dx      0.1
#define  D       0.1

float ** init(){
    float ** a;
    int i, j, xcenter, ycenter;
    a = (float **) malloc(m * sizeof(float *));
    for(i = 0; i < m; i++){
        a[i] = (float *) malloc(n * sizeof(float));
    }
    xcenter = m/2;
    ycenter = n/2;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            a[i][j] = 25.0;
        }
    }
    for(i = xcenter - 5; i <= xcenter + 4; i++){
        for(j = ycenter - 5; j <= ycenter + 4; j++){
            a[i][j] = 80.0;
        }
    }
    return a;
}

void write2file(float ** a, char *name){
    FILE *f;
    f = fopen(name, "a+");
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%f ", a[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void openmp_heat_equation_2d(){
    FILE *f;
    f = fopen("res.txt", "w+");
    fclose(f);
    float ** C = init(), ** dC = init(), c, u, d, l ,r;
    int i, j, count, id, sid, eid;
    omp_set_num_threads(5);
    for(count = 0; count < T/dt; count++){
        write2file(C, "res.txt");
#pragma omp parallel private(i, j, id, sid, eid, c, u, d, l ,r)
        {
            id = omp_get_thread_num();
            sid = id*m/5;
            eid = (id+1)*m/5;
#pragma omp barrier
            for(i = sid; i < eid; i++) {
                for (j = 0; j < n; j++) {
                    c = C[i][j];
                    u = (i == 0) ? 25 : C[i - 1][j];
                    d = (i == m - 1) ? 25 : C[i + 1][j];
                    l = (j == 0) ? 25 : C[i][j - 1];
                    r = (j == m - 1) ? 25 : C[i][j + 1];
                    dC[i][j] = (1 / (dx * dx)) * (u + d + l + r - 4 * c);
                }
            }
#pragma omp barrier
            for(i = sid; i < eid; i++){
                for(j = 0;j < n; j++){
                    C[i][j] += dt * D * dC[i][j];
                }
            }
        }
    }
}