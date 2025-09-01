#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

long long fibonacci(int n) {
    if (n <= 1) return n;
    long long x, y;
    #pragma omp task shared(x)
    x = fibonacci(n-1);

    #pragma omp task shared(y)
    y = fibonacci(n-2);
    #pragma omp taskwait
    return x+y;
}

int main(){
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    int n = 20;
    #pragma omp parallel
    #pragma omp single
    cout << fibonacci(n) << endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;
    return 0;
}