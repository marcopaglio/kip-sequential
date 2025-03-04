#ifndef PIXEL_H
#define PIXEL_H
#include <cstdint>


class Pixel final {
public:
    explicit Pixel(int r=0, int g=0, int b=0);
    ~Pixel();

    [[nodiscard]] int getR() const;
    [[nodiscard]] int getG() const;
    [[nodiscard]] int getB() const;

private:
    int r, g, b;
};



#endif //PIXEL_H
