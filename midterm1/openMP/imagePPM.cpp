#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <algorithm>  // Para min y max
#include <fstream>
#include <cstring>    // Por si usas memcpy

using namespace std;

// ==========================================================
//                PPMImage - COLOR
// ==========================================================

// Destructor para PPMImage
PPMImage::~PPMImage() {
    if (pixels) {
        for (int i = 0; i < height; ++i)
            delete[] pixels[i];
        delete[] pixels;
        pixels = nullptr;
    }
}

// Leer imagen PPM desde stdin
void PPMImage::readStdin() {
    string magic;
    cin >> magic;

    // Ignorar comentarios y leer ancho, alto
    char c;
    c = cin.peek();
    while (c == '#') {
        string dummy;
        getline(cin, dummy);
        c = cin.peek();
    }

    cin >> width >> height;

    // Ignorar comentarios antes de maxVal
    c = cin.peek();
    while (c == '#') {
        string dummy;
        getline(cin, dummy);
        c = cin.peek();
    }

    cin >> maxVal;

    // Reservar memoria para los píxeles
    pixels = new Rgb*[height];
    for (int i = 0; i < height; i++)
        pixels[i] = new Rgb[width];

    // Leer píxeles
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r, g, b;
            cin >> r >> g >> b;

            // Validar valores
            r = max(0, min(maxVal, r));
            g = max(0, min(maxVal, g));
            b = max(0, min(maxVal, b));

            pixels[i][j].r = r;
            pixels[i][j].g = g;
            pixels[i][j].b = b;
        }
    }
}

// Escribir imagen PPM en archivo
void PPMImage::writeToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    out << "P3\n";
    out << width << " " << height << "\n";
    out << maxVal << "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            out << pixels[i][j].r << " "
                << pixels[i][j].g << " "
                << pixels[i][j].b << " ";
        out << "\n";
    }

    out.close();
}

// Filtro Blur PPM
void PPMImage::blur() {
    Rgb** result = new Rgb*[height];
    for (int i = 0; i < height; i++)
        result[i] = new Rgb[width];

    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    int factor = 9;

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++) {
                    sumR += pixels[y + ky][x + kx].r * kernel[ky + 1][kx + 1];
                    sumG += pixels[y + ky][x + kx].g * kernel[ky + 1][kx + 1];
                    sumB += pixels[y + ky][x + kx].b * kernel[ky + 1][kx + 1];
                }

            result[y][x].r = sumR / factor;
            result[y][x].g = sumG / factor;
            result[y][x].b = sumB / factor;
        }
    }

    for (int i = 1; i < height - 1; i++)
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(Rgb));

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}

// Filtro Laplace PPM
void PPMImage::laplace() {
    Rgb** result = new Rgb*[height];
    for (int i = 0; i < height; i++)
        result[i] = new Rgb[width];

    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++) {
                    sumR += pixels[y + ky][x + kx].r * kernel[ky + 1][kx + 1];
                    sumG += pixels[y + ky][x + kx].g * kernel[ky + 1][kx + 1];
                    sumB += pixels[y + ky][x + kx].b * kernel[ky + 1][kx + 1];
                }

            result[y][x].r = max(0, min(maxVal, sumR));
            result[y][x].g = max(0, min(maxVal, sumG));
            result[y][x].b = max(0, min(maxVal, sumB));
        }
    }

    for (int i = 1; i < height - 1; i++)
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(Rgb));

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}

// Filtro Sharpening PPM
void PPMImage::sharpening() {
    Rgb** result = new Rgb*[height];
    for (int i = 0; i < height; i++)
        result[i] = new Rgb[width];

    int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int ky = -1; ky <= 1; ky++)
                for (int kx = -1; kx <= 1; kx++) {
                    sumR += pixels[y + ky][x + kx].r * kernel[ky + 1][kx + 1];
                    sumG += pixels[y + ky][x + kx].g * kernel[ky + 1][kx + 1];
                    sumB += pixels[y + ky][x + kx].b * kernel[ky + 1][kx + 1];
                }

            result[y][x].r = max(0, min(maxVal, sumR));
            result[y][x].g = max(0, min(maxVal, sumG));
            result[y][x].b = max(0, min(maxVal, sumB));
        }
    }

    for (int i = 1; i < height - 1; i++)
        memcpy(pixels[i] + 1, result[i] + 1, (width - 2) * sizeof(Rgb));

    for (int i = 0; i < height; i++)
        delete[] result[i];
    delete[] result;
}
