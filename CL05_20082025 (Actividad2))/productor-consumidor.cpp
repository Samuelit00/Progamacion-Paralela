// productor_consumidor.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define PRODUCE_COUNT 20

int buffer[BUFFER_SIZE];
int count = 0;   // número de elementos en el buffer
int in = 0, out = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty, not_full;

void* productor(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) // buffer lleno
            pthread_cond_wait(&not_full, &mutex);

        buffer[in] = i; // producir un número
        printf("Productor produjo: %d\n", i);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) // buffer vacío
            pthread_cond_wait(&not_empty, &mutex);

        int item = buffer[out]; // consumir
        printf("Consumidor consumió: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    pthread_create(&prod, NULL, productor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    return 0;
}
