#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define THREADS 2

int A[N] = {9,7,3,1,8,2,5,4,6,0};
int B[N];
int thread_count = THREADS;

void* count_sort(void* rank) {
    long my_rank = (long) rank;
    int local_n = N / thread_count;
    int start = my_rank * local_n;
    int end = (my_rank + 1) * local_n;

    for (int i = start; i < end; i++) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (A[j] < A[i] || (A[j] == A[i] && j < i))
                count++;
        }
        B[count] = A[i];
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS];

    for (long t = 0; t < THREADS; t++)
        pthread_create(&threads[t], NULL, count_sort, (void*)t);

    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);

    printf("Arreglo ordenado:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", B[i]);
    printf("\n");
    return 0;
}
