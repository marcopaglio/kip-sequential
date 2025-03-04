#include "Pixel.h"

Pixel::Pixel(const int r, const int g, const int b): r(r), g(g), b(b) {}

Pixel::~Pixel() = default;

int Pixel::getR() const {
    return r;
}

int Pixel::getG() const {
    return g;
}

int Pixel::getB() const {
    return b;
}