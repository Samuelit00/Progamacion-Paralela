#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 10
int buffer[SIZE] = {0};
int in=0, out=0;

sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    while(1) {
        int item = rand()%10+1;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in]=item;
        printf("Producer -> %d\n", item);
        in=(in+1)%SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *consumer(void *arg) {
    while(1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item=buffer[out];
        buffer[out]=0;
        printf("Consumer <- %d\n", item);
        out=(out+1)%SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}

int main() {
    pthread_t prod1, prod2, cons1, cons2;
    sem_init(&empty,0,SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&prod1,NULL,producer,NULL);
    pthread_create(&prod2,NULL,producer,NULL);
    pthread_create(&cons1,NULL,consumer,NULL);
    pthread_create(&cons2,NULL,consumer,NULL);

    pthread_join(prod1,NULL);
    pthread_join(prod2,NULL);
    pthread_join(cons1,NULL);
    pthread_join(cons2,NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
