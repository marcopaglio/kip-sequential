#include "gtest/gtest.h"
#include "../src/Image.h"


TEST(ImageTest, testConstructor) {
    constexpr unsigned int height = 3;
    constexpr unsigned int width = 5;
    // row0
    const Pixel rgb00(120, 0, 130);
    const Pixel rgb01(23, 58, 135);
    const Pixel rgb02(44, 30, 20);
    const Pixel rgb03(123, 15, 15);
    const Pixel rgb04(1, 12, 68);
    // row1
    const Pixel rgb10(1, 17, 225);
    const Pixel rgb11(19, 89, 139);
    const Pixel rgb12(67, 12, 29);
    const Pixel rgb13(88, 137, 213);
    const Pixel rgb14(81, 3, 64);
    // row2
    const Pixel rgb20(43, 38, 106);
    const Pixel rgb21(100, 10, 0);
    const Pixel rgb22(215, 35, 120);
    const Pixel rgb23(10, 4, 65);
    const Pixel rgb24(90, 36, 217);
    const std::vector pixels = {rgb00, rgb01, rgb02, rgb03, rgb04,
                                rgb10, rgb11, rgb12, rgb13, rgb14,
                                rgb20, rgb21, rgb22, rgb23, rgb24};

    Image image(width, height, pixels);

    EXPECT_EQ(image.getWidth(), width);
    EXPECT_EQ(image.getHeight(), height);
    ASSERT_EQ(image.getData().size(), width * height);
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            EXPECT_EQ(image.getData()[y * width + x].getR(), pixels[y * width + x].getR());
            EXPECT_EQ(image.getData()[y * width + x].getG(), pixels[y * width + x].getG());
            EXPECT_EQ(image.getData()[y * width + x].getB(), pixels[y * width + x].getB());
        }
    }
}