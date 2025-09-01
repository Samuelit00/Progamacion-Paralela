#include "image.h"
#include <cstring>
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    // Iniciar cronómetro
    auto start = chrono::high_resolution_clock::now();

    // Redirigir entrada y salida a archivos
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    // Detectar formato de imagen (P2 o P3)
    char tipo[3];
    cin >> tipo;

    // Devolver los caracteres al flujo de entrada
    for (int i = (int)strlen(tipo) - 1; i >= 0; i--) {
        cin.putback(tipo[i]);
    }

    // Crear objeto según el tipo de imagen
    Image *img = nullptr;
    if (tipo[0] == 'P' && tipo[1] == '2') {
        img = new PGMImage();
    } else if (tipo[0] == 'P' && tipo[1] == '3') {
        img = new PPMImage();
    } else {
        cerr << "Error: Formato de imagen no soportado: " << tipo << "\n";
        return 1;
    }

    // Leer la imagen desde stdin (redirigido)
    img->readStdin();

    // Obtener bandera y filtro
    char *flag = argv[3];
    char *filter = argv[4];

    if (strcmp(flag, "--f") == 0) {
        if (strcmp(filter, "blur") == 0)
            img->blur();
        else if (strcmp(filter, "sharpening") == 0)
            img->sharpening();
        else if (strcmp(filter, "laplace") == 0)
            img->laplace();
        else {
            cerr << "Error: Filtro no reconocido: " << filter << "\n";
            delete img;
            return 1;
        }
    } else {
        cerr << "Error: Flag no reconocido: " << flag << "\n";
        delete img;
        return 1;
    }

    // Escribir imagen procesada en stdout (redirigido)
    img->writeStdout();

    // Liberar memoria
    delete img;

    // Detener cronómetro
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Mostrar tiempo de CPU (stderr para no interferir con stdout)
    cerr << "Tiempo de CPU: " << elapsed.count() << " segundos\n";

    return 0;
}
