void multiple_matrix(){
    srand(time(NULL));
    int row1, column1, row2, column2, **mat1, **mat2, **mat3, num_threads, i, j, sid, eid;
    printf("Nhap so dong va cot ma tran 1: ");
    scanf("%d %d", &row1, &column1);
    printf("Nhap so dong va cot ma tran 2: ");
    scanf("%d %d", &row2, &column2);
    if(column1 != row2){
        printf("Khong the nhan 2 ma tran nay");
        return;
    }
    mat1 = (int **) malloc(row1 * sizeof(int));
    mat2 = (int **) malloc(row2 * sizeof(int));
    mat3 = (int **) malloc(row1 * sizeof(int));
    for(i = 0; i < row1; i++){
        *(mat1 + i) = (int *) malloc(column1 * sizeof(int));
        *(mat3 + i) = (int *) malloc(column2 * sizeof(int));
    }
    for(i = 0; i < row2; i++){
        *(mat2 + i) = (int *) malloc(column2 * sizeof(int));
    }

    for(i = 0; i < row1; i++){
        for(j = 0; j < column1; j++){
            mat1[i][j] = rand()%101 + 10;
        }
    }

    for(i = 0; i < row2; i++){
        for(j = 0; j < column2; j++){
            mat2[i][j] = rand()%101 + 10;
        }
    }

    printf("Nhap so luong: ");
    scanf("%d", &num_threads);
    omp_set_num_threads(num_threads);
#pragma omp parallel private(i, j, sid, eid)
    {
        int id = omp_get_thread_num();
        sid = row1/num_threads * id;
        eid = sid + row1/num_threads;
        int k;

        for(i = sid; i < eid; i++){
            for(j = 0; j < column2; j++){
                for(k = 0; k < column1; k++){
                    mat3[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
    }
    for(i = 0; i < row1; i++){
        for(j = 0; j < column1; j++){
            printf("%d  ", mat1[i][j]);
        }
        printf("\n");
    }
    for(i = 0; i < row2; i++){
        for(j = 0; j < column2; j++){
            printf("%d  ", mat2[i][j]);
        }
        printf("\n");
    }
    for(i = 0; i < row1; i++){
        for(j = 0; j < column2; j++){
            printf("%d  ", mat3[i][j]);
        }
        printf("\n");
    }
}
