// src/4_fork_wait.c
// Same as 3_fork_no_wait.c but parent waits, making child's output appear before parent's per iteration.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            printf("Sorry...cannot fork\n");
            exit(1);
        } else if (pid == 0) {
            printf("child %d\n", i);
            return 0; // Child exits so it isn't part of subsequent iterations
        } else {
            wait(NULL); // Parent waits for the child just created
            printf("parent %d\n", i);
        }
    }
    return 0;
}
