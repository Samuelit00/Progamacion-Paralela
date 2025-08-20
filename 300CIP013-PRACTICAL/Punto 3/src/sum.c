#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int start, end;
    printf("Ingrese rango inicial: ");
    scanf("%d", &start);
    printf("Ingrese rango final: ");
    scanf("%d", &end);

    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if(pid == 0) {
        close(fd[0]);
        int sum = 0;
        for(int i = start; i <= end; i++) sum += i;
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]);
        exit(0);
    } else {
        wait(NULL);
        close(fd[1]);
        int result;
        read(fd[0], &result, sizeof(result));
        close(fd[0]);
        printf("La suma es: %d\n", result);
    }
    return 0;
}
