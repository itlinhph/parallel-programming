void multiply2(){
    int world_rank, world_size, *a, *b, *c, *bs, *cs, *ds, m = 10, n = 4, p = 8, i, j, k, count;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if(world_rank == 0){
        //Cap phat cac ma tran
        a = (int *) malloc(m*n*sizeof(int));
        b = (int *) malloc(n*p*sizeof(int));
        c = (int *) malloc(m*p*sizeof(int));
        bs = (int *) malloc(n*p/4*sizeof(int));
        cs = (int *) malloc(m*p/4*sizeof(int));
        ds = (int *) malloc(m*p/4*sizeof(int));

        //Khoi tao ma tran a, b, cs, ds
        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                a[i*n+j] = i*j;
            }
        }
        for(i = 0; i < n; i++){
            for(j = 0; j < p; j++){
                b[i*p+j] = i+j;
            }
        }
        for(i = 0; i < m*p/4; i++){
            cs[i] = 0;
        }
        for(i = 0; i < m*p/4; i++){
            ds[i] = 0;
        }

        //In ma tran a, b
        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                printf("%4d ", a[i*n+j]);
            }
            printf("\n");
        }
        printf("\n");
        for(i = 0; i < n; i++){
            for(j = 0; j < p; j++){
                printf("%4d", b[i*p+j]);
            }
            printf("\n");
        }
        printf("\n");

        //Tinh toan a*bs tren rank 0 va gan vao ma tran c
        k = 0;
        for(i = 0; i < n; i++){
            for(j = 0; j < p/4; j++){
                bs[k] = b[i*p+j];
                k++;
            }
        }
        for(i = 0; i < m; i++){
            for(j = 0; j < p/4; j++){
                for(k = 0; k < n; k++){
                    ds[i*p/4+j] += a[i*n+k] * bs[k*p/4+j];
                }
            }
        }
        k = 0;
        for(i = 0; i < m; i++){
            for(j = 0; j < p/4; j++){
                c[i*p+j] = ds[k];
                k++;
            }
        }

        //Khoi tao ma tran bs chua cac cot cua ma tran b, gui ma tran a va ma tran bs
        for(count = 1; count < world_size; count++){
            k = 0;
            for(i = 0; i < n; i++){
                for(j = count*p/4; j < (count+1)*p/4; j++){
                    bs[k] = b[i*p+j];                    
                    k++;
                }
            }
            MPI_Send(a, m*n, MPI_INT, count, count+world_size, MPI_COMM_WORLD);
            MPI_Send(bs, n*p/4, MPI_INT, count, count+2*world_size, MPI_COMM_WORLD);
        }

        //Nhan lai ma tran cs, gan vao ma tran c
        for(count = 1; count < world_size; count++){
            MPI_Recv(cs, m*p/4, MPI_INT, count, count+3*world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            k = 0;
            for(i = 0; i < m; i++){
                for(j = count*p/4; j < (count+1)*p/4; j++){
                    c[i*p+j] = cs[k];
                    k++;
                }
            }
        }        

        //In ma tran c
        for(i = 0; i < m; i++){
            for(j  = 0; j < p; j++){
                printf("%4d", c[i*p+j]);
            }
            printf("\n");
        }
        printf("\n");
    } else {
        //Cap phat, khoi tao cac ma tran
        a = (int *) malloc(m*n*sizeof(int));
        bs = (int *) malloc(n*p/4*sizeof(int));
        cs = (int *) malloc(m*p/4*sizeof(int));
        for(i = 0; i < m*p/4; i++){
            cs[i] = 0;
        }

        //Receive ma tran a va cac cot cua ma tran b
        MPI_Recv(a, m*n, MPI_INT, 0, world_rank+world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(bs, n*p/4, MPI_INT, 0, world_rank+2*world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //Tinh toan
        for(i = 0; i < m; i++){
            for(j = 0; j < p/4; j++){
                for(k = 0; k < n; k++){
                    cs[i*p/4+j] += a[i*n+k] * bs[k*p/4+j];
                }
            }
        }
        //Gui lai cac cot cua ma tran c
        MPI_Send(cs, m*p/4, MPI_INT, 0, world_rank+3*world_size, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}