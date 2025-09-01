#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10
int buffer[SIZE];
pthread_mutex_t lock;
pthread_cond_t not_empty, not_full;

void* producer(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        int placed = 0;
        for (int i = 0; i < SIZE; i++) {
            if (buffer[i] == 0) {
                int val = rand() % 9 + 1;
                buffer[i] = val;
                placed = 1;
                break;
            }
        }
        if (placed) {
            pthread_cond_signal(&not_empty);
        }
        pthread_mutex_unlock(&lock);
        sleep(rand() % 3 + 1);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        int taken = 0;
        for (int i = 0; i < SIZE; i++) {
            if (buffer[i] != 0) {
                int val = buffer[i];
                buffer[i] = 0;
                taken = 1;
                pthread_mutex_unlock(&lock);
                sleep(val);
                pthread_mutex_lock(&lock);
                break;
            }
        }
        if (taken) {
            pthread_cond_signal(&not_full);
        }
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod1, cons1;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    for (int i = 0; i < SIZE; i++) buffer[i] = 0;

    pthread_create(&prod1, NULL, producer, NULL);
    pthread_create(&cons1, NULL, consumer, NULL);

    pthread_join(prod1, NULL);
    pthread_join(cons1, NULL);
    return 0;
}
