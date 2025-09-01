#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid == 0) { // Child
        close(fd[0]); // Close read end
        int start = 1, end = 10, sum = 0;
        for (int i = start; i <= end; i++) sum += i;
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]);
        exit(0);
    } else { // Parent
        close(fd[1]); // Close write end
        int result;
        wait(NULL);
        read(fd[0], &result, sizeof(result));
        close(fd[0]);
        printf("Sum: %d\n", result);
    }
    return 0;
}
