#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
constexpr long long NUM_STEPS = 1000000000;

int main(){
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    double step = 1.0 / NUM_STEPS;
    double sum = 0.0;
        double local_sum = 0.0;
        for (long long i = 0; i < NUM_STEPS; i++){
            double x = (i + 0.5) * step;
            local_sum += 4.0/(1.0 + x*x);
        }

        sum += local_sum;
    double pi = step * sum;
    cout << "Valor de n estimado : " << pi << endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;
    return 0;
}