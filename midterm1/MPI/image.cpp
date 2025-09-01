#include "image.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

using namespace std;

// ==========================
// Constructor por defecto
// ==========================
Image::Image() : width(0), height(0), maxVal(255) {}

// ==========================
// Constructor parametrizado
// ==========================
Image::Image(int w, int h, int maxVal) : width(w), height(h), maxVal(maxVal) {}

// ==========================
// Destructor base
// ==========================
Image::~Image() {}
