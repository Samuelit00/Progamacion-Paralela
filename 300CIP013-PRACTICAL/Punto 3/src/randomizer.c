#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n;
    printf("Ingrese cantidad de números: ");
    scanf("%d", &n);
    srand(time(NULL));

    for(int i=0; i<n; i++) {
        printf("%d ", rand()%100);
    }
    printf("\n");
    return 0;
}
