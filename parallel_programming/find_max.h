int find_max(int *a, int n){
    if (n==1){
        return *a;
    }
    else {
        int mid = n/2;
        int b = find_max(a, mid);
        int c = find_max(a + mid, n - mid);
        return b>c?b:c;
    }
}

int find_max_2(int *a, int n){
    if(n == 1){
        return *a;
    }
    else if(n % 2 == 0) {
        int *b = (int *) malloc(sizeof(int) * n/2);
        omp_set_num_threads(n/2);
#pragma omp parallel
        {
            int id = omp_get_thread_num();
            b[id] = a[id] > a[id+n/2]?a[id]:a[id+n/2];
        }
        return find_max_2(b, n/2);
    }
    else {
        int *b = (int *) malloc(sizeof(int) * (n/2+1));
        omp_set_num_threads(n/2);
#pragma omp parallel
        {
            int id = omp_get_thread_num();
            b[id] = a[id] > a[id+n/2]?a[id]:a[id+n/2];
            b[n/2] = a[n-1];
        }
        return find_max_2(b, n/2 + 1);
    }
}