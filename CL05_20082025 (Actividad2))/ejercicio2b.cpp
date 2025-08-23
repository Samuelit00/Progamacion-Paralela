#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

const int n = 5000;
int A[n];
double sum = 0.0;

int calcularArreglo(){
    for (int i = 0; i < n; i++){
        int randomNumber = (rand() % 3000);
        A[i] = randomNumber;
    }
}

int main(){
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    calcularArreglo();

    #pragma omp parallel for reduction(+:sum);
    for (int i = 0; i < n; i++){
        sum += A[i];
    }

    cout << "Suma total " << sum << endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;
    return 0;
}