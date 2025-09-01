#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int n = 10;
    for (int i = 0; i < n; i++) {
        int num = rand() % 100;
        printf("%d\n", num);
    }
    return 0;
}
