#include <gtest/gtest.h>
#include "../src/KernelFactory.h"

class BlurKernelFactoryTest : public ::testing::TestWithParam<unsigned int> {
protected:
    std::string boxBlurName = "boxBlur";
};

INSTANTIATE_TEST_SUITE_P(, BlurKernelFactoryTest,
                         testing::Values(3, 5, 7, 9),
                        [](const testing::TestParamInfo<BlurKernelFactoryTest::ParamType>& info) {
                            std::stringstream suffixStream;
                            suffixStream << "WhenOrderIs" << info.param;
                            return suffixStream.str();
                        });


TEST_P(BlurKernelFactoryTest, testCreateBlurKernel) {
    const unsigned int order = GetParam();
    const unsigned int numElements = order * order;
    const std::unique_ptr<Kernel> kernel = KernelFactory::createBoxBlurKernel(order);

    ASSERT_NE(kernel, nullptr);
    EXPECT_EQ(kernel->getName(), boxBlurName);
    EXPECT_EQ(kernel->getOrder(), order);
    EXPECT_EQ(kernel->getNormalizationFactor(), numElements);
    ASSERT_EQ(kernel->getWeights().size(), numElements);
    for (auto weight : kernel->getWeights())
        EXPECT_EQ(weight, 1);
}


TEST_F(BlurKernelFactoryTest, testCreateBlurKernelWithEvenOrder) {
    constexpr unsigned int order = 4;
    EXPECT_THROW(KernelFactory::createBoxBlurKernel(order), std::invalid_argument);
}



class EdgeDetectionKernelCreatorTest :
    public ::testing::TestWithParam<std::pair<unsigned int, std::vector<int>>> {
protected:
    std::string edgeDetectionName = "edgeDetection";
};

INSTANTIATE_TEST_SUITE_P(, EdgeDetectionKernelCreatorTest,
                         testing::Values(
                         std::make_pair(3, std::vector<int>{ -1, -1, -1,
                                                                       -1, 8, -1,
                                                                      -1, -1, -1   }),
                         std::make_pair(5, std::vector<int>{   -1, -1, -1, -1, -1,
                                                                        -1, -2, -2, -2, -1,
                                                                        -1, -2, 32, -2, -1,
                                                                        -1, -2, -2, -2, -1,
                                                                        -1, -1, -1, -1, -1  }),
                         std::make_pair(7, std::vector<int>{   -1, -1, -1, -1, -1, -1, -1,
                                                                        -1, -2, -2, -2, -2, -2, -1,
                                                                        -1, -2, -4, -4, -4, -2, -1,
                                                                        -1, -2, -4, 88, -4, -2, -1,
                                                                        -1, -2, -4, -4, -4, -2, -1,
                                                                        -1, -2, -2, -2, -2, -2, -1,
                                                                        -1, -1, -1, -1, -1, -1, -1  }),
                         std::make_pair(9, std::vector<int>{   -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                                                        -1, -2, -2, -2, -2, -2, -2, -2, -1,
                                                                        -1, -2, -4, -4, -4, -4, -4, -2, -1,
                                                                        -1, -2, -4, -8, -8, -8, -4, -2, -1,
                                                                       -1, -2, -4, -8, 208, -8, -4, -2, -1,
                                                                        -1, -2, -4, -8, -8, -8, -4, -2, -1,
                                                                        -1, -2, -4, -4, -4, -4, -4, -2, -1,
                                                                        -1, -2, -2, -2, -2, -2, -2, -2, -1,
                                                                        -1, -1, -1, -1, -1, -1, -1, -1, -1,  })),
                        [](const testing::TestParamInfo<EdgeDetectionKernelCreatorTest::ParamType>& info) {
                            std::stringstream suffixStream;
                            suffixStream << "WhenOrderIs" << info.param.first;
                            return suffixStream.str();
                        });


TEST_P(EdgeDetectionKernelCreatorTest, testCreateEdgeDetectionKernel) {
    const unsigned int order = GetParam().first;
    const std::vector<int> weights = GetParam().second;

    const std::unique_ptr<Kernel> kernel = KernelFactory::createEdgeDetectionKernel(order);

    ASSERT_NE(kernel, nullptr);
    EXPECT_EQ(kernel->getName(), edgeDetectionName);
    EXPECT_EQ(kernel->getOrder(), order);
    EXPECT_EQ(kernel->getNormalizationFactor(), 1);
    ASSERT_EQ(kernel->getWeights().size(), order * order);
    for (unsigned int i = 0; i < order * order; i++) {
        EXPECT_EQ(kernel->getWeights()[i], weights[i]);
    }
}


TEST_F(EdgeDetectionKernelCreatorTest, testCreateEdgeDetectionKernelWithEvenOrder) {
    constexpr unsigned int order = 4;
    EXPECT_THROW(KernelFactory::createEdgeDetectionKernel(order), std::invalid_argument);
}