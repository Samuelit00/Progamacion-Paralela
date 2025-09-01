#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

using namespace std;
// ==========================================================
//                PGMImage - ESCALA DE GRISES
// ==========================================================
// Constructor por defecto
PGMImage::PGMImage() {
    pixels = nullptr;
}

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
// -------------------- argumentos para hilos PGM --------------------
struct PGMArgs {
    PGMImage* img;
    int** src; // fuente (copia)
    int start_row, end_row;
    int start_col, end_col;
};

// -------------------- función auxiliar: crear copia de fuente PGM ----
static int** clonePGMSource(PGMImage* img) {
    int h = img->getHeight();
    int w = img->getWidth();
    int** src = new int*[h];
    for (int i = 0; i < h; ++i) {
        src[i] = new int[w];
        for (int j = 0; j < w; ++j) src[i][j] = img->pixels[i][j];
    }
    return src;
}

// -------------------- kernel: blur (cada hilo procesa su cuadrante) ----
void* pgm_blur_thread(void* arg) {
    PGMArgs* a = (PGMArgs*)arg;
    PGMImage* img = a->img;
    int** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();

    int kernel[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
    int factor = 9;

    int rstart = max(1, a->start_row);
    int rend   = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend   = min(w-1, a->end_col);

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ++ky)
                for (int kx = -1; kx <= 1; ++kx)
                    sum += src[y+ky][x+kx] * kernel[ky+1][kx+1];
            img->pixels[y][x] = sum / factor;
        }
    }
    return nullptr;
}

// -------------------- kernel: laplace --------------------
void* pgm_laplace_thread(void* arg) {
    PGMArgs* a = (PGMArgs*)arg;
    PGMImage* img = a->img;
    int** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();
    int maxV = img->getMaxVal();

    int rstart = max(1, a->start_row);
    int rend   = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend   = min(w-1, a->end_col);

    int kernel[3][3] = { {0,-1,0},{-1,4,-1},{0,-1,0} };

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ++ky)
                for (int kx = -1; kx <= 1; ++kx)
                    sum += src[y+ky][x+kx] * kernel[ky+1][kx+1];
            if (sum < 0) sum = 0;
            if (sum > maxV) sum = maxV;
            img->pixels[y][x] = sum;
        }
    }
    return nullptr;
}

// -------------------- kernel: sharpening --------------------
void* pgm_sharp_thread(void* arg) {
    PGMArgs* a = (PGMArgs*)arg;
    PGMImage* img = a->img;
    int** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();
    int maxV = img->getMaxVal();

    int rstart = max(1, a->start_row);
    int rend   = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend   = min(w-1, a->end_col);

    int kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ++ky)
                for (int kx = -1; kx <= 1; ++kx)
                    sum += src[y+ky][x+kx] * kernel[ky+1][kx+1];
            if (sum < 0) sum = 0;
            if (sum > maxV) sum = maxV;
            img->pixels[y][x] = sum;
        }
    }
    return nullptr;
}

// -------------------- funciones paralelas públicas --------------------
void PGMImage::blurParallel(int thread_count) {
    // siempre dividimos en 4 regiones (cuadrantes)
    int parts = 4;
    pthread_t threads[4];
    PGMArgs args[4];

    int** src = clonePGMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };           // arriba-izq
    args[1] = { this, src, 0, mid_row, mid_col, width };       // arriba-der
    args[2] = { this, src, mid_row, height, 0, mid_col };      // abajo-izq
    args[3] = { this, src, mid_row, height, mid_col, width };  // abajo-der

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, pgm_blur_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    // liberar src
    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}

void PGMImage::laplaceParallel(int thread_count) {
    int parts = 4;
    pthread_t threads[4];
    PGMArgs args[4];

    int** src = clonePGMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };
    args[1] = { this, src, 0, mid_row, mid_col, width };
    args[2] = { this, src, mid_row, height, 0, mid_col };
    args[3] = { this, src, mid_row, height, mid_col, width };

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, pgm_laplace_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}

void PGMImage::sharpeningParallel(int thread_count) {
    int parts = 4;
    pthread_t threads[4];
    PGMArgs args[4];

    int** src = clonePGMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };
    args[1] = { this, src, 0, mid_row, mid_col, width };
    args[2] = { this, src, mid_row, height, 0, mid_col };
    args[3] = { this, src, mid_row, height, mid_col, width };

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, pgm_sharp_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}