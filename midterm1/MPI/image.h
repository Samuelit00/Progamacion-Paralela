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
    Image(int w, int h, int maxVal);
    virtual ~Image();

    int getWidth() const;
    int getHeight() const;
    int getMaxVal() const;

    // Métodos virtuales puros
    virtual void readStdin() = 0;
    virtual void writeStdout() = 0;
    virtual void blur() = 0;
    virtual void laplace() = 0;
    virtual void sharpening() = 0;
};

// Clase para imágenes PGM
class PGMImage : public Image {
public:
    int** pixels;
    PGMImage() : pixels(nullptr) {}
    ~PGMImage();
    void readStdin() override;
    void writeStdout() override;
    void blur() override;
    void laplace() override;
    void sharpening() override;
};

// Clase para imágenes PPM
class PPMImage : public Image {
public:
    Rgb** pixels;
    PPMImage() : pixels(nullptr) {}
    ~PPMImage();
    void readStdin() override;
    void writeStdout() override;
    void blur() override;
    void laplace() override;
    void sharpening() override;
};

#endif
