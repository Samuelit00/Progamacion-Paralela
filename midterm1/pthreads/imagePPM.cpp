#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <algorithm>  // Para min y max

using namespace std;

// ==========================================================
//                PPMImage - COLOR
// ==========================================================
// Constructor por defecto
PPMImage::PPMImage() {
    pixels = nullptr;
}

// Destructor para PPMImage
PPMImage::~PPMImage() {
    if (pixels) {
        for (int i = 0; i < height; ++i)
            delete[] pixels[i];
        delete[] pixels;
        pixels = nullptr;
    }
}

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
            if (r < 0) r = 0; if (r > maxVal) r = maxVal;
            if (g < 0) g = 0; if (g > maxVal) g = maxVal;
            if (b < 0) b = 0; if (b > maxVal) b = maxVal;

            pixels[i][j].r = r;
            pixels[i][j].g = g;
            pixels[i][j].b = b;
        }
    }
}


// Escribir imagen PPM en stdout
void PPMImage::writeStdout() {
    cout << "P3\n";
    cout << width << " " << height << "\n";
    cout << maxVal << "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            cout << pixels[i][j].r << " "
                 << pixels[i][j].g << " "
                 << pixels[i][j].b << " ";
        cout << "\n";
    }
}

// -------------------- args y helper (PPM) --------------------
struct PPMArgs {
    PPMImage* img;
    Rgb** src;
    int start_row, end_row;
    int start_col, end_col;
};

static Rgb** clonePPMSource(PPMImage* img) {
    int h = img->getHeight();
    int w = img->getWidth();
    Rgb** src = new Rgb*[h];
    for (int i = 0; i < h; ++i) {
        src[i] = new Rgb[w];
        for (int j = 0; j < w; ++j) src[i][j] = img->pixels[i][j];
    }
    return src;
}

// -------------------- blur PPM thread --------------------
void* ppm_blur_thread(void* arg) {
    PPMArgs* a = (PPMArgs*)arg;
    PPMImage* img = a->img;
    Rgb** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();

    int kernel[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
    int factor = 9;

    int rstart = max(1, a->start_row);
    int rend = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend = min(w-1, a->end_col);

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sr=0, sg=0, sb=0;
            for (int ky=-1; ky<=1; ++ky)
                for (int kx=-1; kx<=1; ++kx) {
                    sr += src[y+ky][x+kx].r * kernel[ky+1][kx+1];
                    sg += src[y+ky][x+kx].g * kernel[ky+1][kx+1];
                    sb += src[y+ky][x+kx].b * kernel[ky+1][kx+1];
                }
            img->pixels[y][x].r = sr / factor;
            img->pixels[y][x].g = sg / factor;
            img->pixels[y][x].b = sb / factor;
        }
    }
    return nullptr;
}

// -------------------- laplace PPM thread --------------------
void* ppm_laplace_thread(void* arg) {
    PPMArgs* a = (PPMArgs*)arg;
    PPMImage* img = a->img;
    Rgb** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();
    int maxV = img->getMaxVal();

    int rstart = max(1, a->start_row);
    int rend = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend = min(w-1, a->end_col);

    int kernel[3][3] = { {0,-1,0},{-1,4,-1},{0,-1,0} };

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sr=0, sg=0, sb=0;
            for (int ky=-1; ky<=1; ++ky)
                for (int kx=-1; kx<=1; ++kx) {
                    sr += src[y+ky][x+kx].r * kernel[ky+1][kx+1];
                    sg += src[y+ky][x+kx].g * kernel[ky+1][kx+1];
                    sb += src[y+ky][x+kx].b * kernel[ky+1][kx+1];
                }
            sr = max(0, min(maxV, sr));
            sg = max(0, min(maxV, sg));
            sb = max(0, min(maxV, sb));
            img->pixels[y][x].r = sr;
            img->pixels[y][x].g = sg;
            img->pixels[y][x].b = sb;
        }
    }
    return nullptr;
}

// -------------------- sharpening PPM thread --------------------
void* ppm_sharp_thread(void* arg) {
    PPMArgs* a = (PPMArgs*)arg;
    PPMImage* img = a->img;
    Rgb** src = a->src;
    int h = img->getHeight();
    int w = img->getWidth();
    int maxV = img->getMaxVal();

    int rstart = max(1, a->start_row);
    int rend = min(h-1, a->end_row);
    int cstart = max(1, a->start_col);
    int cend = min(w-1, a->end_col);

    int kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };

    for (int y = rstart; y < rend; ++y) {
        for (int x = cstart; x < cend; ++x) {
            int sr=0, sg=0, sb=0;
            for (int ky=-1; ky<=1; ++ky)
                for (int kx=-1; kx<=1; ++kx) {
                    sr += src[y+ky][x+kx].r * kernel[ky+1][kx+1];
                    sg += src[y+ky][x+kx].g * kernel[ky+1][kx+1];
                    sb += src[y+ky][x+kx].b * kernel[ky+1][kx+1];
                }
            sr = max(0, min(maxV, sr));
            sg = max(0, min(maxV, sg));
            sb = max(0, min(maxV, sb));
            img->pixels[y][x].r = sr;
            img->pixels[y][x].g = sg;
            img->pixels[y][x].b = sb;
        }
    }
    return nullptr;
}

// -------------------- funciones paralelas públicas PPM --------------------
void PPMImage::blurParallel(int thread_count) {
    int parts = 4;
    pthread_t threads[4];
    PPMArgs args[4];

    Rgb** src = clonePPMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };
    args[1] = { this, src, 0, mid_row, mid_col, width };
    args[2] = { this, src, mid_row, height, 0, mid_col };
    args[3] = { this, src, mid_row, height, mid_col, width };

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, ppm_blur_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}

void PPMImage::laplaceParallel(int thread_count) {
    int parts = 4;
    pthread_t threads[4];
    PPMArgs args[4];

    Rgb** src = clonePPMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };
    args[1] = { this, src, 0, mid_row, mid_col, width };
    args[2] = { this, src, mid_row, height, 0, mid_col };
    args[3] = { this, src, mid_row, height, mid_col, width };

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, ppm_laplace_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}

void PPMImage::sharpeningParallel(int thread_count) {
    int parts = 4;
    pthread_t threads[4];
    PPMArgs args[4];

    Rgb** src = clonePPMSource(this);

    int mid_row = height / 2;
    int mid_col = width / 2;

    args[0] = { this, src, 0, mid_row, 0, mid_col };
    args[1] = { this, src, 0, mid_row, mid_col, width };
    args[2] = { this, src, mid_row, height, 0, mid_col };
    args[3] = { this, src, mid_row, height, mid_col, width };

    for (int i = 0; i < parts; ++i) pthread_create(&threads[i], nullptr, ppm_sharp_thread, &args[i]);
    for (int i = 0; i < parts; ++i) pthread_join(threads[i], nullptr);

    for (int i = 0; i < height; ++i) delete[] src[i];
    delete[] src;
}