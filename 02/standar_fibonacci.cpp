#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

long long fibonacci(int n) {
    if (n <= 1) return n;
    long long x, y;
    x = fibonacci(n-1);

    y = fibonacci(n-2);
    return x+y;
}

int main(){
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    int n = 20;
    cout << fibonacci(n) << endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;
    return 0;
}