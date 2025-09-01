#include "image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>
#include <cstring>
using namespace std;

// -------------------- Función para clonar imágenes PGM --------------------
PGMImage* clonePGM(PGMImage* original) {
    PGMImage* copy = new PGMImage();
    copy->setWidth(original->getWidth());
    copy->setHeight(original->getHeight());
    copy->setMaxVal(original->getMaxVal());

    int height = original->getHeight();
    int width = original->getWidth();

    copy->pixels = new int*[height];
    for (int i = 0; i < height; i++) {
        copy->pixels[i] = new int[width];
        memcpy(copy->pixels[i], original->pixels[i], width * sizeof(int));
    }

    return copy;
}

int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now();
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <imagen.pgm>" << endl;
        return 1;
    }

    // Abrimos la imagen original
    freopen(argv[1], "r", stdin);
    string tipo;
    cin >> tipo;
    cin.putback(tipo[1]);
    cin.putback(tipo[0]);

    PGMImage* original = new PGMImage();
    original->readStdin();

    // Creamos copias para cada filtro
    PGMImage* imgBlur = clonePGM(original);
    PGMImage* imgLaplace = clonePGM(original);
    PGMImage* imgSharpening = clonePGM(original);

    // Paralelizamos el procesamiento con OpenMP
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            imgBlur->blur();
            imgBlur->writeToFile("imagen_blur.pgm");
        }

        #pragma omp section
        {
            imgLaplace->laplace();
            imgLaplace->writeToFile("imagen_laplace.pgm");
        }

        #pragma omp section
        {
            imgSharpening->sharpening();
            imgSharpening->writeToFile("imagen_sharpening.pgm");
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cerr << "Tiempo de CPU: " << elapsed.count() << " segundos\n";

    delete original;
    delete imgBlur;
    delete imgLaplace;
    delete imgSharpening;

    return 0;
}