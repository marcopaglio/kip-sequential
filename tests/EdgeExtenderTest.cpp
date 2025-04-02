#include <gtest/gtest.h>
#include "../src/EdgeExtender.h"
#include "../src/Image.h"


class EdgeExtenderTest : public ::testing::Test {
protected:
    const unsigned int height = 3;
    const unsigned int width = 5;
    Pixel rgb00, rgb01, rgb02, rgb03, rgb04;
    Pixel rgb10, rgb11, rgb12, rgb13, rgb14;
    Pixel rgb20, rgb21, rgb22, rgb23, rgb24;
    Image* imageToProcess = nullptr;
    EdgeExtender* edgeExtender = nullptr;

    void SetUp() override {
        rgb00 = Pixel(120, 0, 130);
        rgb01 = Pixel(23, 58, 135);
        rgb02 = Pixel(44, 30, 20);
        rgb03 = Pixel(123, 15, 15);
        rgb04 = Pixel(1, 12, 68);
        const std::vector row0 = {rgb00, rgb01, rgb02, rgb03, rgb04};
        rgb10 = Pixel(1, 17, 225);
        rgb11 = Pixel(19, 89, 139);
        rgb12 = Pixel(67, 12, 29);
        rgb13 = Pixel(88, 137, 213);
        rgb14 = Pixel(81, 3, 64);
        const std::vector row1 = {rgb10, rgb11, rgb12, rgb13, rgb14};
        rgb20 = Pixel(43, 38, 106);
        rgb21 = Pixel(100, 10, 0);
        rgb22 = Pixel(215, 35, 120);
        rgb23 = Pixel(10, 4, 65);
        rgb24 = Pixel(90, 36, 217);
        const std::vector row2 = {rgb20, rgb21, rgb22, rgb23, rgb24};

        imageToProcess = new Image(width, height, std::vector{row0, row1, row2});
        edgeExtender = new EdgeExtender();
    }

    void TearDown() override {
        delete imageToProcess;
        imageToProcess = nullptr;

        delete edgeExtender;
        edgeExtender = nullptr;
    }
};


TEST_F(EdgeExtenderTest, testPrepareImageWithZeroPadding) {
    constexpr unsigned int padding = 0;
    constexpr unsigned int heightExtended = 3;
    constexpr unsigned int widthExtended = 5;
    const std::vector row0 = {rgb00, rgb01, rgb02, rgb03, rgb04};
    const std::vector row1 = {rgb10, rgb11, rgb12, rgb13, rgb14};
    const std::vector row2 = {rgb20, rgb21, rgb22, rgb23, rgb24};
    const std::vector pixelsProcessed = {row0, row1, row2};

    const std::unique_ptr<Image> imageProcessed = edgeExtender->prepareImage(*imageToProcess, padding);

    EXPECT_EQ(imageProcessed->getHeight(), heightExtended);
    EXPECT_EQ(imageProcessed->getWidth(), widthExtended);
    ASSERT_EQ(imageProcessed->getData().size(), heightExtended);
    ASSERT_EQ(imageProcessed->getData()[0].size(), widthExtended);
    for (unsigned int j = 0; j < heightExtended; j++) {
        for (unsigned int i = 0; i < widthExtended; i++) {
            EXPECT_EQ(imageProcessed->getData()[j][i].getR(), pixelsProcessed[j][i].getR());
            EXPECT_EQ(imageProcessed->getData()[j][i].getG(), pixelsProcessed[j][i].getG());
            EXPECT_EQ(imageProcessed->getData()[j][i].getB(), pixelsProcessed[j][i].getB());
        }
    }
    EXPECT_NE(imageToProcess, imageProcessed.get());
}

TEST_F(EdgeExtenderTest, testPrepareImageWithPositivePadding) {
    constexpr unsigned int padding = 1;
    constexpr unsigned int heightExtended = 5;
    constexpr unsigned int widthExtended = 7;
    const std::vector row0 = {rgb00, rgb00, rgb01, rgb02, rgb03, rgb04, rgb04};
    const std::vector row1 = {rgb00, rgb00, rgb01, rgb02, rgb03, rgb04, rgb04};
    const std::vector row2 = {rgb10, rgb10, rgb11, rgb12, rgb13, rgb14, rgb14};
    const std::vector row3 = {rgb20, rgb20, rgb21, rgb22, rgb23, rgb24, rgb24};
    const std::vector row4 = {rgb20, rgb20, rgb21, rgb22, rgb23, rgb24, rgb24};
    const std::vector pixelsProcessed = {row0, row1, row2, row3, row4};

    const std::unique_ptr<Image> imageProcessed = edgeExtender->prepareImage(*imageToProcess, padding);

    EXPECT_EQ(imageProcessed->getHeight(), heightExtended);
    EXPECT_EQ(imageProcessed->getWidth(), widthExtended);
    ASSERT_EQ(imageProcessed->getData().size(), heightExtended);
    ASSERT_EQ(imageProcessed->getData()[0].size(), widthExtended);
    for (unsigned int j = 0; j < heightExtended; j++) {
        for (unsigned int i = 0; i < widthExtended; i++) {
            EXPECT_EQ(imageProcessed->getData()[j][i].getR(), pixelsProcessed[j][i].getR());
            EXPECT_EQ(imageProcessed->getData()[j][i].getG(), pixelsProcessed[j][i].getG());
            EXPECT_EQ(imageProcessed->getData()[j][i].getB(), pixelsProcessed[j][i].getB());
        }
    }
}