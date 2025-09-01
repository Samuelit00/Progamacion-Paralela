#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <chrono>



using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now(); // Inicia el cronómetro
    long long N = 1000000000; 
    

    long long dentro_circulo = 0;
    srand(time(0));

    for (long long i = 0; i < N; i++) {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        
        if (x * x + y * y <= 1.0) {
            dentro_circulo++;
        }
    }

    double pi_aprox = 4.0 * dentro_circulo / N;

    cout.precision(10);
    cout << "Aproximación de pi = " << pi_aprox << endl;
    auto end = chrono::high_resolution_clock::now(); // Detiene el cronómetro
    chrono::duration<double> elapsed = end - start;
    std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << std::endl;
    return 0;
}
