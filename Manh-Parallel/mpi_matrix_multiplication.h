void multiply()
{
    int world_rank, world_size, i, j, k, m = 10, n = 4, p = 4, *a, *b, *c, *as;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0){
        a = (int *) malloc (m*n * sizeof (int));
        b = (int *) malloc (n*p * sizeof (int));
        c = (int *) malloc (m*p * sizeof (int));
        as = (int *) malloc (m/5*n * sizeof(int));
        //Khoi tao a
        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                a[i*n+j] = i+j;
            }
        }
        //Khoi tao b
        for(i = 0; i < n; i++){
            for(j = 0; j < p; j++){
                if(i == j){
                    b[i*p+j] = 1;
                } else {
                    b[i*p+j] = 0;
                }
            }
        }
        //Khoi tao c
        for(i = 0; i < m*p;i++){
            c[i] = 0;
        }
        //In ma tran a
        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                printf("%d ", a[i*n+j]);
            }
            printf("\n");
        }
        //In ma tran b
        printf("\n");
        for(i = 0; i < n; i++){
            for(j = 0; j < p; j++){
                printf("%d ", b[i*p+j]);
            }
            printf("\n");
        }
        printf("\n");

        k = 0;
        for(j = 0; j < m/5*n; j++){
            as[k] = a[j];
            k++;
        }

        for(i = 0; i < m/5; i++){
            for(j = 0; j < p; j++){
                for(k = 0; k < n; k++){
                    c[i*p+j] += as[i*n+k] * b[k*p+j];
                }
            }
        }

        for(i = 1; i < world_size; i++){
            k = 0;
            for(j = i*m/5*n; j < (i+1)*m/5*n; j++){
                as[k] = a[j];
                k++;
            }
            MPI_Send(as, m/5*n, MPI_INT, i, i+world_size, MPI_COMM_WORLD);
            MPI_Send(b, n*p, MPI_INT, i, i+world_size*2, MPI_COMM_WORLD);
        }

        for(i = 1; i < world_size; i++){
            MPI_Recv(c + i*m/5*p, m/5*p, MPI_INT, i, i+world_size*3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for(i = 0; i < m; i++){
            for(j = 0; j < p; j++){
                printf("%d ", c[i*p+j]);
            }
            printf("\n");
        }
    } else {
        b = (int *) malloc (n*p * sizeof(int *));
        as = (int *) malloc (m/5*n * sizeof(int *));
        c = (int *) malloc (m/5*p * sizeof(int *));
        for(i = 0; i < m/5*p; i++){
            c[i] = 0;
        }
        MPI_Recv(as, m/5*n, MPI_INT, 0, world_rank+world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b, n*p, MPI_INT, 0, world_rank+world_size*2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(i = 0; i < m/5; i++){
            for(j = 0; j < p; j++){
                for(k = 0; k < n; k++){
                    c[i*p+j] += as[i*n+k] * b[k*p+j];
                }
            }
        }
        MPI_Send(c, m/5*p, MPI_INT, 0, world_rank+world_size*3, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
