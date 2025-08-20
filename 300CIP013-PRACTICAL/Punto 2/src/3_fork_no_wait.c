
// Create multiple processes; scheduling may produce different output orders each run.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            printf("Sorry...cannot fork\n");
            exit(1);
        } else if (pid == 0) {
            printf("child %d\n", i);
        } else {
            printf("parent %d\n", i);
        }
    }
    return 0;
}
