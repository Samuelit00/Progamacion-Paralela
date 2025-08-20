#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 40

int A[SIZE][SIZE], B[SIZE][SIZE], SUM[SIZE][SIZE], DIFF[SIZE][SIZE], MULT[SIZE][SIZE];
pthread_t t1, t2;

void *sum_matrices(void *arg) {
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++)
            SUM[i][j] = A[i][j] + B[i][j];
    pthread_exit(NULL);
}

void *diff_matrices(void *arg) {
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++)
            DIFF[i][j] = A[i][j] - B[i][j];
    pthread_exit(NULL);
}

void multiply_results() {
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++) {
            MULT[i][j] = SUM[i][j] * DIFF[i][j];
        }
}

int main() {
    srand(time(NULL));
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++) {
            A[i][j]=rand()%2;
            B[i][j]=rand()%2;
        }

    clock_t start = clock();
    pthread_create(&t1,NULL,sum_matrices,NULL);
    pthread_create(&t2,NULL,diff_matrices,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    multiply_results();
    clock_t end = clock();

    printf("Tiempo con threads: %.5f seg\n", (double)(end-start)/CLOCKS_PER_SEC);
    return 0;
}
