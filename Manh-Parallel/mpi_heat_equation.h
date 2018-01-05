#define M 20
#define Time 1.0
#define dt 0.01
#define dx 0.1
#define D 0.1

void mpi_heat_equation(){
    int rank, size, i, j, t, Ntime = Time/dt;
    float *dT, *T, l, c, r, *gather;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    dT = (float *) malloc(M/4 * sizeof(float));
    T = (float *) malloc(M/4 * sizeof(float));
    for(i = 0; i < M/4; i++){
        T[i] = 25.0;
    }
    for(t = 0; t <= Ntime; t++){
        //Gui sang trai
        if(rank != 0){
            MPI_Send(&T[0], 1, MPI_FLOAT, rank-1, rank+size, MPI_COMM_WORLD);
        }
        //Gui sang phai
        if(rank != size-1){
            MPI_Send(&T[M/4-1], 1, MPI_FLOAT, rank+1, rank+2*size, MPI_COMM_WORLD);
        }
        //Tinh dT theo T
        for(i = 0; i < M/4; i++){
            c = T[i];
            if(rank == 0){
                if(i == 0){
                    l = 100.0;
                } else {
                    l = T[i-1];
                }
                if(i == M/4-1){
                    MPI_Recv(&r, 1, MPI_FLOAT, rank+1, rank+1+size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    r = T[i+1];
                }
            } else if(rank == size-1){
                if(i == 0){
                    MPI_Recv(&l, 1, MPI_FLOAT, rank-1, rank-1+2*size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    l = T[i-1];
                }
                if(i == M/4-1){
                    r = 25.0;
                } else {
                    r = T[i+1];
                }
            } else {
                if(i == 0){
                    MPI_Recv(&l, 1, MPI_FLOAT, rank-1, rank-1+2*size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    l = T[i-1];
                }
                if(i == M/4-1){
                    MPI_Recv(&r, 1, MPI_FLOAT, rank+1, rank+1+size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                } else {
                    r = T[i+1];
                }
            }
            dT[i] = D * (l - 2 * c + r) / (dx * dx);
        }
        //Tinh T theo dT
        for(i = 0; i < M/4; i++){
            T[i] += dT[i] * dt;
        }
    }

    gather = (float *) malloc(M * sizeof(float));
    MPI_Gather(T, M/4, MPI_FLOAT, gather, M/4, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        for(j = 0; j < M; j++){
            printf("%f ", gather[j]);
        }
        printf("\n");
    }
    MPI_Finalize();
}
