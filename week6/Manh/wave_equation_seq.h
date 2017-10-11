#define L 1
#define dx 0.005
#define dt 0.005
#define c 1
#define Time 4
#define pi 3.14
#define f 5

//Tinh ham u(t, x)
//Boundary condition: u(t, 0) = u(t, L) = 0
void wave_equation_seq(){
    FILE *file1;
    int nx = L/dx + 1;
    int nt = Time/dt + 1;
    int i, j;
    float r = (c*dt/dx)*(c*dt/dx);
    float **u;
    u = (float **) malloc(nt * sizeof(float *));
    for(i = 0; i < nt; i++){
        *(u+i) = (float *) malloc(nx * sizeof(float));
    }
    //Initial
    for(i = 0; i < nx; i++){
        u[0][i] = sin(2*pi*i*dx/L);
        u[1][i] = sin(2*pi*i*dx/L);
    }
    //Loop
    for(i = 2; i < nt; i++){
        for(j = 1; j < nx-1; j++){
//            float a1 = j==0?-u[i-1][0]:u[i-1][j-1];
//            float a2 = u[i-1][j];
//            float a3 = j==nx-1?-u[i-1][nx-1]:u[i-1][j+1];
            float a1 = u[i-1][j-1];
            float a2 = u[i-1][j];
            float a3 = u[i-1][j+1];
            u[i][j] = 2*u[i-1][j] - u[i-2][j] + r*(a3 - 2*a2 + a1);
        }
    }
    file1 = fopen("/home/duymanh/Dropbox/CLionProjects/parallel_programming/wave_equation_seq/1.txt", "wt");
    for(i = 0; i < nt; i++){
        for(j = 0; j < nx; j++){
            fprintf(file1, "%f ", u[i][j]);
        }
        fprintf(file1, "\n");
    }
    fclose(file1);
}
