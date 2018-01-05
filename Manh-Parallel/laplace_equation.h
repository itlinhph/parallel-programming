#define m 10
#define n 10
#define high_temp 200.0
#define nor_temp 25.0
#define ep 1e-6

float ** matInit(){
    float **a;
    int i, j;
    a = (float **) malloc (m * sizeof(float*));
    for(i = 0; i < m; i++){
        a[i] = (float*) malloc(n * sizeof(float));
    }
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            a[i][j] = nor_temp;
        }
    }
    return a;
}

float **change(float **a){
    float **b, u, d, l, r;
    b = matInit();
    int i, j;
    for(i = 0; i < m; i++){
        for(j = 0; j < m; j++){
            u = (i == 0)?high_temp:a[i-1][j];
            d = (i == m-1)?25:a[i+1][j];
            l = (j == 0)?25:a[i][j-1];
            r = (j == n-1)?200:a[i][j+1];
            b[i][j] = (u + d + l + r)/4;
        }
    }
    return b;
}

void write2file(float ** a){
    FILE *f;
    f = fopen("res3.txt", "a+");
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%f ", a[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int check(float **a, float **b){
    int i, j;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            if(fabs(a[i][j] - b[i][j]) > ep){
                return 1;
            }
        }
    }
    return 0;
}

void laplace_equation(){
    int count = 0;
    remove("res3.txt");
    float **mat1, **mat2;

    mat1 = matInit();
    mat2 = change(mat1);
    while(check(mat1, mat2) != 0){
        write2file(mat1);
        mat1 = mat2;
        mat2 = change(mat2);
        count ++;
    }
    printf("%d\n", count);
}