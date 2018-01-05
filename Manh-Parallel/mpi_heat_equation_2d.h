#define m 50
#define n 50
#define T 2
#define dt 0.01f
#define dx 0.1f
#define D 0.1f
#define CENTER_TEMP 80.0f
#define NOR_TEMP 25.0f

float *matInit()
{
    float *a;
    int i, j, xcenter = m / 2, ycenter = n / 2;
    a = (float *)malloc(m * n * sizeof(float));
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if ((i < xcenter - 5) || (i > xcenter + 4) || (j < ycenter - 5) || (j > ycenter + 4))
            {
                a[i * n + j] = NOR_TEMP;
            }
            else
            {
                a[i * n + j] = CENTER_TEMP;
            }
        }
    }
    return a;
}

void write2file(float *a)
{
    FILE *f;
    f = fopen("res2.txt", "a+");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(f, "%f ", a[i * n + j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void FD(float *C, float *dC, float *up, float *down, int size)
{
    int i, j;
    float c, u, d, l, r;
    for (i = 0; i < m / size; i++)
    {
        for (j = 0; j < n; j++)
        {
            c = C[i * n + j];
            u = (i == 0) ? up[j] : C[(i - 1) * n + j];
            d = (i == m / size - 1) ? down[j] : C[(i + 1) * n + j];
            l = (j == 0) ? 25 : C[i * n + j - 1];
            r = (j == n - 1) ? 25 : C[i * n + j + 1];
            dC[i * n + j] = (D / (dx * dx)) * (u + d + l + r - 4 * c);
        }
    }
}

void mpi_heat_equation_2d()
{
    remove("res2.txt");
    int i, j, k, count, rank, size;
    float *C, *dC, *up, *down, *gather;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    gather = (float *)malloc(m * n * sizeof(float));
    int m2 = m / size;
    up = (float *)calloc(n, sizeof(float));
    down = (float *)calloc(n, sizeof(float));
    for (i = 0; i < n; i++)
    {
        up[i] = 25.0;
        down[i] = 25.0;
    }

    //Khoi tao ma tran lon
    float *oriMat = matInit();

    //Khoi tao ma tran nho
    C = (float *)malloc(m2 * n * sizeof(float));
    dC = (float *)malloc(m2 * n * sizeof(float));
    k = 0;
    for (i = rank * m2; i < (rank + 1) * m2; i++)
    {
        for (j = 0; j < n; j++)
        {
            C[k] = oriMat[i * n + j];
            k++;
        }
    }

    //Vong lap
    for (count = 0; count < T / dt; count++)
    {
        //Gui xuong duoi
        if (rank != size - 1)
        {
            MPI_Send(C + (m2 - 1) * n, n, MPI_FLOAT, rank + 1, rank + size, MPI_COMM_WORLD);
        }

        //Gui len tren
        if (rank != 0)
        {
            MPI_Send(C, n, MPI_FLOAT, rank - 1, rank + 2 * size, MPI_COMM_WORLD);
        }

        //Nhan tu tren xuong
        if (rank != 0)
        {
            MPI_Recv(up, n, MPI_FLOAT, rank - 1, rank - 1 + size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //Nhan tu duoi len
        if (rank != size - 1)
        {
            MPI_Recv(down, n, MPI_FLOAT, rank + 1, rank + 1 + 2 * size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        //Tinh dC theo C
        FD(C, dC, up, down, size);

        //Tinh C theo dC
        for (i = 0; i < m2; i++)
        {
            for (j = 0; j < n; j++)
            {
                C[i * n + j] += dt * dC[i * n + j];
            }
        }

        //Gop ma tran va ghi ra file
        MPI_Gather(C, m2 * n, MPI_FLOAT, gather, m2 * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
        if (rank == 0)
        {
            write2file(gather);
        }
    }
    MPI_Finalize();
}