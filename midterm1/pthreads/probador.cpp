#include "image.h"
#include <cstring>
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 5) {
        cerr << "Uso: " << argv[0] << " <entrada> <salida> --f <filtro>\n";
        cerr << "Filtros: blur | laplace | sharpening\n";
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    char tipo[3];
    cin >> tipo;

    // devolver tipo al flujo
    for (int i = (int)strlen(tipo) - 1; i >= 0; --i) cin.putback(tipo[i]);

    Image* img = nullptr;
    if (tipo[0] == 'P' && tipo[1] == '2') img = new PGMImage();
    else if (tipo[0] == 'P' && tipo[1] == '3') img = new PPMImage();
    else {
        cerr << "Formato no soportado: " << tipo << "\n";
        return 1;
    }

    img->readStdin();

    char* flag = argv[3];
    char* filter = argv[4];

    if (strcmp(flag, "--f") != 0) {
        cerr << "Flag no reconocido: " << flag << "\n";
        delete img;
        return 1;
    }

    // número de hilos pedido (pero la implementación divide en 4 regiones)
    int threads = 4;
    // Si quieres permitir cambiar el número: parse argv[5] etc.

    if (strcmp(filter, "blur") == 0) {
        PGMImage* pgm = dynamic_cast<PGMImage*>(img);
        if (pgm) pgm->blurParallel(threads);
        else {
            PPMImage* ppm = dynamic_cast<PPMImage*>(img);
            if (ppm) ppm->blurParallel(threads);
            else { cerr << "Tipo desconocido\n"; }
        }
    }
    else if (strcmp(filter, "laplace") == 0) {
        PGMImage* pgm = dynamic_cast<PGMImage*>(img);
        if (pgm) pgm->laplaceParallel(threads);
        else {
            PPMImage* ppm = dynamic_cast<PPMImage*>(img);
            if (ppm) ppm->laplaceParallel(threads);
        }
    }
    else if (strcmp(filter, "sharpening") == 0) {
        PGMImage* pgm = dynamic_cast<PGMImage*>(img);
        if (pgm) pgm->sharpeningParallel(threads);
        else {
            PPMImage* ppm = dynamic_cast<PPMImage*>(img);
            if (ppm) ppm->sharpeningParallel(threads);
        }
    } else {
        cerr << "Filtro no reconocido: " << filter << "\n";
    }

    img->writeStdout();
    delete img;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cerr << "Tiempo de CPU: " << elapsed.count() << " segundos\n";
    return 0;
}
