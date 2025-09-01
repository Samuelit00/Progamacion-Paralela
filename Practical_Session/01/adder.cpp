#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n = 9;
    int numbers[] = {1,2,3,4,5,6,7,8,9};
    int chunk = n / 3;
    int fd[2];
    pipe(fd);

    for (int p = 0; p < 3; p++) {
        if (fork() == 0) {
            close(fd[0]);
            int sum = 0;
            for (int i = p * chunk; i < (p+1) * chunk; i++)
                sum += numbers[i];
            write(fd[1], &sum, sizeof(sum));
            close(fd[1]);
            exit(0);
        }
    }

    close(fd[1]);
    int total = 0, part;
    for (int p = 0; p < 3; p++) {
        wait(NULL);
        read(fd[0], &part, sizeof(part));
        total += part;
    }
    close(fd[0]);

    printf("Total sum: %d\n", total);
    return 0;
}
