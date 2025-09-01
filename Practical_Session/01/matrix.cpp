#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 40

int A[SIZE][SIZE], B[SIZE][SIZE], SUM[SIZE][SIZE], DIFF[SIZE][SIZE], MULT[SIZE][SIZE];

void* sum_matrix(void* arg) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            SUM[i][j] = A[i][j] + B[i][j];
    pthread_exit(NULL);
}

void* diff_matrix(void* arg) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            DIFF[i][j] = A[i][j] - B[i][j];
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 2;
            B[i][j] = rand() % 2;
        }

    pthread_t t1, t2;
    pthread_create(&t1, NULL, sum_matrix, NULL);
    pthread_create(&t2, NULL, diff_matrix, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            MULT[i][j] = SUM[i][j] * DIFF[i][j];
        }

    printf("Multiplication done.\n");
    return 0;
}
