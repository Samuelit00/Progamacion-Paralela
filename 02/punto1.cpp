#include <iostream>
#include <string>
#include <omp.h>
#include <chrono>

using namespace std;

int main(int argc, char** argv)
{
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    int ite = stoi(argv[1]);

    double suma = 0.0;
    #pragma omp parallel for reduction(+:suma)
    for(int n = 0; n < ite; ++n) {
        double ans = 1.0 / (2.0 * n + 1.0);
        if(n % 2 == 0)
            suma += ans;
        else
            suma -= ans;
    }

    double pi = 4.0 * suma;
    cout << "Con " << ite << " iteraciones la aproximacion es: " << pi << std::endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << endl;

    return 0;
}

