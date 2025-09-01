#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

using namespace std;

int N = 6;                 // tamaño de la matriz
int thread_count = 4;

vector<vector<int>> A;
vector<vector<int>> B;
vector<vector<int>> C;

void llenarMatriz(vector<vector<int>>& M) {
    for (int i = 0; i < (int)M.size(); ++i) {
        for (int j = 0; j < (int)M[i].size(); ++j) {
            M[i][j] = rand() % 1000;
        }
    }
}

void imprimirMatriz(const vector<vector<int>>& M) {
    for (int i = 0; i < (int)M.size(); ++i) {
        for (int j = 0; j < (int)M[i].size(); ++j) {
            cout << M[i][j] << " ";
        }
        cout << "\n";
    }
}

void* multiMatrices(void* arg) {
    long my_rank = (long) arg;
    int rows_per_thread = N / thread_count;
    int first_row = my_rank * rows_per_thread;
    int last_row = (my_rank == thread_count - 1) ? N : first_row + rows_per_thread;

    for (int x = first_row; x < last_row; ++x) {
        for (int i = 0; i < N; ++i) {
            long long acc = 0; 
            for (int j = 0; j < N; ++j) {
                acc += (long long)A[x][j] * (long long)B[j][i];
            }
            C[x][i] = acc;
        }
    }
    return nullptr;
}

int main() {
    srand((unsigned)time(nullptr));

    N = 6;
    thread_count = 4; // <= N de preferencia

    // Reserva e inicializa matrices
    A.assign(N, vector<int>(N));
    B.assign(N, vector<int>(N));
    C.assign(N, vector<int>(N, 0));

    llenarMatriz(A);
    llenarMatriz(B);

    auto start = chrono::high_resolution_clock::now();

    cout << "A:\n"; imprimirMatriz(A);
    cout << "B:\n"; imprimirMatriz(B);

    vector<pthread_t> threads(thread_count);
    for (intptr_t r = 0; r < thread_count; ++r) {
        pthread_create(&threads[r], nullptr, multiMatrices, (void*)r);
    }
    for (int t = 0; t < thread_count; ++t) {
        pthread_join(threads[t], nullptr);
    }

    cout << "C = A * B:\n";
    imprimirMatriz(C);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos\n";

    return 0;
}
