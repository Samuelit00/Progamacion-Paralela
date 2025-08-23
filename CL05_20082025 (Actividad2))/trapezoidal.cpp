// trapezoidal.cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define THREADS 2
long n = 1024; // número de trapecios
double a = 0.0, b = M_PI; // intervalo
double integral = 0.0;
pthread_mutex_t mutex;

double f(double x) {
    return sin(x); // función ejemplo
}

void* trapezoid(void* rank) {
    long my_rank = (long) rank;
    long local_n = n / THREADS;
    double h = (b - a) / n;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_int = (f(local_a) + f(local_b)) / 2.0;
    for (long i = 1; i < local_n; i++) {
        double x = local_a + i * h;
        local_int += f(x);
    }
    local_int = local_int * h;

    pthread_mutex_lock(&mutex);
    integral += local_int;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (long t = 0; t < THREADS; t++)
        pthread_create(&threads[t], NULL, trapezoid, (void*)t);

    for (int t = 0; t < THREADS; t++)
        pthread_join(threads[t], NULL);

    printf("Integral aproximada en [%f, %f] = %f\n", a, b, integral);

    pthread_mutex_destroy(&mutex);
    return 0;
}