#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

using namespace std;
// ==========================================================
//                PGMImage - ESCALA DE GRISES
// ==========================================================

// Destructor para PGMImage
PGMImage::~PGMImage() {
    if (pixels) {
        for (int i = 0; i < height; ++i)
            delete[] pixels[i];
        delete[] pixels;
        pixels = nullptr;
    }
}

// Leer imagen PGM desde stdin
void PGMImage::readStdin() {
    string line;

    // Leer el número mágico
    getline(cin, line);
    // Ignorar comentarios
    getline(cin, line);
    while (line[0] == '#')
        getline(cin, line);

    // Leer ancho y alto
    stringstream ss(line);
    ss >> width >> height;

    // Leer valor máximo
    cin >> maxVal;

    // Crear memoria dinámica para los píxeles
    pixels = new int*[height];
    for (int i = 0; i < height; i++)
        pixels[i] = new int[width];

    // Leer valores de píxeles
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            cin >> pixels[i][j];
}

// Escribir imagen PGM en stdout
void PGMImage::writeStdout() {
    cout << "P2\n";
    cout << width << " " << height << "\n";
    cout << maxVal << "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            cout << pixels[i][j] << " ";
        cout << "\n";
    }
}

// Filtro Blur PGM
void PGMImage::blur() {
    int** result = new int*[height];
    for (int i = 0; i < height; i++)
        result[i] = new int[width];

    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    int factor = 9;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;

            // Convolución
            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++)
                    sum += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];

            result[y][x] = sum / factor;
        }
    }

    // Copiar resultados
    for (int i = 1; i < height - 1; i++)
        for (int j = 1; j < width - 1; j++)
            pixels[i][j] = result[i][j];

    // Liberar memoria
    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}

// Filtro Laplace PGM
void PGMImage::laplace() {
    int** result = new int*[height];
    for (int i = 0; i < height; i++)
        result[i] = new int[width];

    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;

            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++)
                    sum += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];

            result[y][x] = max(0, min(maxVal, sum));
        }
    }

    for (int i = 1; i < height - 1; i++)
        for (int j = 1; j < width - 1; j++)
            pixels[i][j] = result[i][j];

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}

// Filtro Sharpening PGM
void PGMImage::sharpening() {
    int** result = new int*[height];
    for (int i = 0; i < height; i++)
        result[i] = new int[width];

    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;

            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++)
                    sum += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];

            result[y][x] = max(0, min(maxVal, sum));
        }
    }

    for (int i = 1; i < height - 1; i++)
        for (int j = 1; j < width - 1; j++)
            pixels[i][j] = result[i][j];

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}