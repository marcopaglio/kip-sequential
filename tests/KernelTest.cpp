#include <gtest/gtest.h>
#include "../src/Kernel.h"


TEST(KernelTest, testConstructor) {
    const std::string name = "kernelTestConstructor";
    constexpr unsigned int order = 3;
    constexpr int weightValue = 5;
    constexpr int normFactor = 45;

    const Kernel kernel(name, order, std::vector(order * order, weightValue), normFactor);

    EXPECT_EQ(kernel.getName(), name);
    EXPECT_EQ(kernel.getOrder(), order);
    EXPECT_EQ(kernel.getNormalizationFactor(), normFactor);
    ASSERT_EQ(kernel.getWeights().size(), order * order);
    for (auto weight : kernel.getWeights())
        EXPECT_EQ(weight, weightValue);
}