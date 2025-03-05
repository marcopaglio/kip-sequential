#include "Image.h"

Image::Image(const unsigned int w, const unsigned int h, const std::vector<Pixel>& data):
    width(w), height(h), data(data) {}

Image::~Image() = default;

unsigned int Image::getWidth() const {
    return width;
}

unsigned int Image::getHeight() const {
    return height;
}

std::vector<Pixel> Image::getData() const {
    return data;
}