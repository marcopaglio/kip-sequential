#include <gtest/gtest.h>
#include <filesystem>
#include "../src/STBImageReader.h"

class STBImageReaderTest : public ::testing::Test {
protected:
    STBImageReader* imageReader = nullptr;

    void SetUp() override {
        imageReader = new STBImageReader();
    }

    void TearDown() override {
        delete imageReader;
        imageReader = nullptr;
    }
};


TEST_F(STBImageReaderTest, testLoadRGBImageWhenImageExists) {
    constexpr unsigned int width = 5;
    constexpr unsigned int height = 3;
    // row0
    const Pixel rgb00(120, 1, 131);
    const Pixel rgb01(22, 58, 136);
    const Pixel rgb02(45, 31, 22);
    const Pixel rgb03(123, 15, 15);
    const Pixel rgb04(1, 12, 68);
    // row1
    const Pixel rgb10(1, 17, 226);
    const Pixel rgb11(18, 88, 140);
    const Pixel rgb12(66, 12, 28);
    const Pixel rgb13(89, 137, 213);
    const Pixel rgb14(82, 4, 64);
    // row2
    const Pixel rgb20(42, 36, 106);
    const Pixel rgb21(100, 10, 1);
    const Pixel rgb22(215, 36, 118);
    const Pixel rgb23(10, 4, 64);
    const Pixel rgb24(90, 36, 217);
    const std::vector pixels = {rgb00, rgb01, rgb02, rgb03, rgb04,
                                rgb10, rgb11, rgb12, rgb13, rgb14,
                                rgb20, rgb21, rgb22, rgb23, rgb24};

    std::stringstream inputFilePathStream;
    inputFilePathStream << PROJECT_SOURCE_DIR << "/tests/imgs/input/testImage.jpg";
    const std::string inputFilePath = inputFilePathStream.str();

    const auto img = imageReader->loadRGBImage(inputFilePath);

    ASSERT_NE(img, nullptr);
    EXPECT_EQ(img->getHeight(), height);
    EXPECT_EQ(img->getWidth(), width);
    ASSERT_EQ(img->getData().size(), width * height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            EXPECT_EQ(img->getData()[y * width + x].getR(), pixels[y * width + x].getR());
            EXPECT_EQ(img->getData()[y * width + x].getG(), pixels[y * width + x].getG());
            EXPECT_EQ(img->getData()[y * width + x].getB(), pixels[y * width + x].getB());
        }
    }
}

TEST_F(STBImageReaderTest, testLoadRGBImageWhenImageDoesntExist) {
    const std::string inputFilePath = "this/path/doesnt/exist/testImage.jpg";
    std::filesystem::remove_all(inputFilePath);

    EXPECT_THROW(imageReader->loadRGBImage(inputFilePath), std::runtime_error);
}


TEST_F(STBImageReaderTest, testSaveJPGImageWhenPathExists) {
    constexpr unsigned int height = 3;
    constexpr unsigned int width = 5;
    const std::vector somePixels(width * height, Pixel());
    const Image testImage(width, height, somePixels);

    std::stringstream outputFilePathStream;
    outputFilePathStream << PROJECT_SOURCE_DIR << "/tests/imgs/output/testImage.jpg";
    const std::string outputFilePath = outputFilePathStream.str();

    remove(outputFilePath.c_str());
    ASSERT_FALSE(std::filesystem::exists(outputFilePath));

    imageReader->saveJPGImage(testImage, outputFilePath);

    EXPECT_TRUE(std::filesystem::exists(outputFilePath));
}

TEST_F(STBImageReaderTest, testSaveJPGImageWhenPathDoesntExist) {
    constexpr unsigned int height = 3;
    constexpr unsigned int width = 5;
    const std::vector somePixels(width * height, Pixel());
    const Image testImage(width, height, somePixels);

    const std::string outputFilePath = "this/path/doesnt/exist/testImage.jpg";
    std::filesystem::remove_all(outputFilePath);

    EXPECT_THROW(imageReader->saveJPGImage(testImage, outputFilePath), std::runtime_error);
    EXPECT_FALSE(std::filesystem::exists(outputFilePath));
}