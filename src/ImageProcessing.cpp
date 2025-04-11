#include "ImageProcessing.h"

#define MIN_VALUE 0
#define MAX_VALUE 255

uint8_t getChannelAsUint8(const float channel) {
    if (channel < MIN_VALUE)
        return MIN_VALUE;
    if (channel > MAX_VALUE)
        return MAX_VALUE;
    return static_cast<uint8_t>(channel);
}

ImageProcessing::ImageProcessing(EdgeHandler& edgeHandler) : edgeHandler(edgeHandler) {}

ImageProcessing::~ImageProcessing() = default;

std::unique_ptr<Image> ImageProcessing::convolution(const Image &image, const Kernel &kernel) const {
    const unsigned int order = kernel.getOrder();
    const auto kernelWeights = kernel.getWeights();

    const unsigned int padding = (order - 1) / 2;

    const auto preparedImage = edgeHandler.prepareImage(image, padding);
    const auto originalData = preparedImage->getData();
    const unsigned int outputHeight = preparedImage->getHeight() - 2 * padding;
    const unsigned int outputWidth = preparedImage->getWidth() - 2 * padding;

    std::vector pixels(outputHeight, std::vector<Pixel>(outputWidth));
    for (unsigned int y = 0; y < outputHeight; y++) {
        for (unsigned int x = 0; x < outputWidth; x++) {
            float channelRed = 0;
            float channelGreen = 0;
            float channelBlue = 0;

            for (unsigned int j = 0; j < order; j++) {
                for (unsigned int i = 0; i < order; i++) {
                    Pixel originalPixel = originalData[y + j][x + i];
                    const float kernelWeight = kernelWeights[j * order + i];
                    channelRed += static_cast<float>(originalPixel.getR()) * kernelWeight;
                    channelGreen += static_cast<float>(originalPixel.getG()) * kernelWeight;
                    channelBlue += static_cast<float>(originalPixel.getB()) * kernelWeight;
                }
            }
            pixels[y][x] = Pixel(getChannelAsUint8(channelRed),
                getChannelAsUint8(channelGreen), getChannelAsUint8(channelBlue));
        }
    }

    return std::make_unique<Image>(outputWidth, outputHeight, pixels);
}
