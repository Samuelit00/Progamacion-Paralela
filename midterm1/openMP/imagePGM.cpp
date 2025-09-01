#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>  // Para memcpy

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

// Escribir imagen PGM en archivo
void PGMImage::writeToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    // Cabecera PGM
    out << "P2\n";
    out << width << " " << height << "\n";
    out << maxVal << "\n";

    // Escribir píxeles
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            out << pixels[i][j] << " ";
        out << "\n";
    }

    out.close();
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
            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++)
                    sum += pixels[y + ky][x + kx] * kernel[ky + 1][kx + 1];

            result[y][x] = sum / factor;
        }
    }

    // Copiar resultados
    for (int i = 1; i < height - 1; i++)
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(int));

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
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(int));

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
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(int));

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}
