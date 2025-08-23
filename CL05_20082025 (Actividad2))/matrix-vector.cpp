// matrix-vector.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 4   // filas
#define N 4   // columnas
#define THREADS 2

double A[M][N];
double x[N];
double y[M];
int thread_count = THREADS;

void* mat_vect(void* rank) {
    long my_rank = (long) rank;
    int local_m = M / thread_count;
    int first_row = my_rank * local_m;
    int last_row = (my_rank + 1) * local_m - 1;

    for (int i = first_row; i <= last_row; i++) {
        y[i] = 0.0;
        for (int j = 0; j < N; j++)
            y[i] += A[i][j] * x[j];
    }
    return NULL;
}

int main() {
    pthread_t threads[THREADS];

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = i + j;

    for (int i = 0; i < N; i++)
        x[i] = 1.0;

    for (long t = 0; t < THREADS; t++)
        pthread_create(&threads[t], NULL, mat_vect, (void*)t);

    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);

    printf("Resultado y = A * x :\n");
    for (int i = 0; i < M; i++)
        printf("%f\n", y[i]);

    return 0;
}
