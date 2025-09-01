#include <iostream>
#include <random>
#include <pthread.h>
#include <chrono>

using namespace std;

int in_circle = 0;
pthread_mutex_t mutex;

struct threadData {
    int tosses;
};

void* monteCarlo(void* arg){
    threadData* data = (threadData*) arg;
    int count = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < data->tosses; ++i){
        double x = dist(gen);
        double y = dist(gen);
        if (x * x + y * y <= 1.0){
            count++;
        }
    }
    pthread_mutex_lock(&mutex);
    in_circle += count;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main(int argc, char* argv[]){
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    if (argc != 3){
        std::cerr << "Operation failed. Enter 2 arguments";
        return 1;
    }

    int num_tosses = std::atoi(argv[1]);
    int num_threads = std::atoi(argv[2]);
    pthread_t threads[num_threads];
    threadData thrdata[num_threads];
    pthread_mutex_init(&mutex, nullptr);
    int tosses_per_thread = num_tosses / num_threads;
    int remainder = num_tosses % num_threads;

    for (int i = 0; i < num_threads; ++i){
        thrdata[i].tosses = tosses_per_thread + (i < remainder ? 1: 0);
        pthread_create(&threads[i], nullptr, monteCarlo, &thrdata[i]);
    }

    for (int i = 0; i < num_threads; ++i){
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    double pi_estimate = 4.0 * static_cast<double>(in_circle)/num_tosses;

    std::cout << "Tosses: " << num_tosses << std::endl;
    std::cout << "Threads: " << num_threads << std::endl;
    std::cout << "Inside the circle: " << in_circle << std::endl;
    std::cout << "Estimated pi = " << pi_estimate << std::endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << std::endl;
    return 0;
}