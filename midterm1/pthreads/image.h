#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>

struct Rgb {
    int r;
    int g;
    int b;
};

class Image {
protected:
    int width;
    int height;
    int maxVal;

public:
    Image();
    Image(int w, int h, int m);
    virtual ~Image();

    int getWidth(){return width;}
    int getHeight(){return height;}
    int getMaxVal(){return maxVal;}
    virtual void readStdin() = 0;
    virtual void writeStdout() = 0;

    // Paralelo (pthreads) — todas las implementaciones deben proveerlo
    virtual void blurParallel(int thread_count) = 0;
    virtual void laplaceParallel(int thread_count) = 0;
    virtual void sharpeningParallel(int thread_count) = 0;
};

// PGM (P2)
class PGMImage : public Image {
public:
    int** pixels; // pixels[height][width]

    PGMImage();
    virtual ~PGMImage();

    void readStdin() override;
    void writeStdout() override;

    // Paralelo
    void blurParallel(int thread_count) override;
    void laplaceParallel(int thread_count) override;
    void sharpeningParallel(int thread_count) override;
};

// PPM (P3)
class PPMImage : public Image {
public:
    Rgb** pixels; // pixels[height][width]

    PPMImage();
    virtual ~PPMImage();

    void readStdin() override;
    void writeStdout() override;

    // Paralelo
    void blurParallel(int thread_count) override;
    void laplaceParallel(int thread_count) override;
    void sharpeningParallel(int thread_count) override;
};

#endif
