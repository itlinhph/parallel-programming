#define m 20
#define n 20
#define high_temp 200.0
#define nor_temp 25.0
#define eps 1e-3

double *matInit(){
    double *a;
    int i, j;
    a = (double *) malloc(m*n*sizeof(double));
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            a[i*n+j] = nor_temp;
        }
    }
    return a;
}

void write2file(double *a){
    int i, j;
    FILE *f;
    f = fopen("laplace.txt", "a+");
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            fprintf(f, "%f ", a[i*n+j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

double *change(double *C, double *up, double *down, double m2){
    double *b;
    b = (double *) malloc(m2*n*sizeof(double));
    double u, d, l, r;
    int i, j;
    for(i = 0; i < m2; i++){
        for(j = 0; j < n; j++){
            u = (i == 0)?up[j]:C[(i-1)*n+j];
            d = (i == m2-1)?down[j]:C[(i+1)*n+j];
            l = (j == 0)?nor_temp:C[i*n+j-1];
            r = (j == n-1)?high_temp:C[i*n+j+1];
            b[i*n+j] = (u+d+l+r)/4;
        }
    }
    return b;
}

int sub_check(double *C, double *dC, int m2){
    int i, j;
    for(i = 0; i < m2; i++){
        for(j = 0; j < n; j++){
            if(fabs(C[i*n+j] - dC[i*n+j]) > eps){
                return 1;
            }
        }
    }
    return 0;
}

int total_check(int *a, int size){
    int i; 
    for(i = 0; i < size; i++){
        if(a[i] == 1){
            return 1;
        }
    }
    return 0;
}

void mpi_laplace_equation(){
    remove("laplace.txt");
    int i, j, m2, rank, size, sub_signal, *total_signal;
    double *oriMat;
    double *C, *dC, *up, *down;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    m2 = m/size;
    //Khoi tao ma tran goc va cac ma tran C, dC, total_signal
    total_signal = (int *) malloc(size * sizeof(int));
    C = (double *) malloc(m2*n*sizeof(double));
    dC = (double *) malloc(m2*n*sizeof(double));
    if(rank == 0){
        oriMat = matInit();
    }
    MPI_Scatter(oriMat, m2*n, MPI_DOUBLE, C, m2*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //Khoi tao ma tran up, down
    up = (double *) malloc(n *sizeof(double));
    down = (double *) malloc(n * sizeof(double));
    for(i = 0; i < n; i++){
        up[i] = high_temp;
        down[i] = nor_temp;
    }

    //Vong lap
    do{
        //Gui xuong duoi
        if(rank != size-1){
            MPI_Send(C + (m2-1)*n, n, MPI_DOUBLE, rank+1, rank+size, MPI_COMM_WORLD);
        }

        //Gui len tren
        if(rank != 0){
            MPI_Send(C, n, MPI_DOUBLE, rank-1, rank+2*size, MPI_COMM_WORLD);
        }

        //Nhan tu tren xuong
        if(rank != 0){
            MPI_Recv(up, n, MPI_DOUBLE, rank-1, rank-1+size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //Nhan tu duoi len
        if(rank != size-1){
            MPI_Recv(down, n, MPI_DOUBLE, rank+1, rank+1+2*size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //Bien doi ma tran (bien doi C, gan vao dC)
        dC = change(C, up, down, m2);

        //Kiem tra dieu kien trong moi CPU
        sub_signal = sub_check(C, dC, m2);
        
        //Tao ma tran tin hieu trong toan bo khong gian, kiem tra dieu kien
        MPI_Gather(&sub_signal, 1, MPI_INT, total_signal, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if(rank == 0){
            if(total_check(total_signal, size) == 1){
                for(i = 0; i < size; i++){
                    total_signal[i] = 1;
                }                
            }
        }
        MPI_Scatter(total_signal, 1, MPI_INT, &sub_signal, 1, MPI_INT, 0, MPI_COMM_WORLD);

        //Thay doi
        C = dC;

        //Gop cac ma tran nho va in ra file
        MPI_Gather(C, m2*n, MPI_DOUBLE, oriMat, m2*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if(rank == 0){
            write2file(oriMat);
        }
    } while (sub_signal != 0);
    MPI_Finalize();
}