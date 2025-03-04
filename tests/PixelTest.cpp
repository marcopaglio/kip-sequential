#include <gtest/gtest.h>
#include "../src/Pixel.h"


TEST(PixelTest, testConstructor) {
    constexpr int r = 13;
    constexpr int g  = 28;
    constexpr int b = 234;

    const Pixel pixel(r, g, b);

    EXPECT_EQ(pixel.getR(), r);
    EXPECT_EQ(pixel.getG(), g);
    EXPECT_EQ(pixel.getB(), b);
}

TEST(PixelTest, testDefaultConstructor) {
    const Pixel defaultPixel;

    EXPECT_EQ(defaultPixel.getR(), 0);
    EXPECT_EQ(defaultPixel.getG(), 0);
    EXPECT_EQ(defaultPixel.getB(), 0);
}