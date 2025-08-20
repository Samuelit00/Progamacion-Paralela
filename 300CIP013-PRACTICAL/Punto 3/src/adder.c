#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int n;
    printf("Ingrese cantidad de números: ");
    scanf("%d", &n);
    int *arr = malloc(n * sizeof(int));

    printf("Ingrese los números: ");
    for(int i=0; i<n; i++) scanf("%d", &arr[i]);

    int num_process = n/3;
    int fd[num_process][2];
    for(int i=0; i<num_process; i++) pipe(fd[i]);

    for(int p=0; p<num_process; p++) {
        pid_t pid = fork();
        if(pid == 0) {
            close(fd[p][0]);
            int partial_sum=0;
            for(int j=p*3; j<(p+1)*3 && j<n; j++) partial_sum+=arr[j];
            write(fd[p][1], &partial_sum, sizeof(partial_sum));
            close(fd[p][1]);
            exit(0);
        }
    }

    int total=0;
    for(int p=0; p<num_process; p++) {
        wait(NULL);
        close(fd[p][1]);
        int partial;
        read(fd[p][0], &partial, sizeof(partial));
        total+=partial;
        close(fd[p][0]);
    }
    printf("La suma total es: %d\n", total);
    free(arr);
    return 0;
}
