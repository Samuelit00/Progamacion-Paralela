#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 40

int A[SIZE][SIZE], B[SIZE][SIZE], SUM[SIZE][SIZE], DIFF[SIZE][SIZE], MULT[SIZE][SIZE];

int main() {
    srand(time(NULL));
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++) {
            A[i][j]=rand()%2;
            B[i][j]=rand()%2;
        }

    clock_t start = clock();
    for(int i=0; i<SIZE; i++)
        for(int j=0; j<SIZE; j++) {
            SUM[i][j]=A[i][j]+B[i][j];
            DIFF[i][j]=A[i][j]-B[i][j];
            MULT[i][j]=SUM[i][j]*DIFF[i][j];
        }
    clock_t end = clock();

    printf("Tiempo sin threads: %.5f seg\n", (double)(end-start)/CLOCKS_PER_SEC);
    return 0;
}
