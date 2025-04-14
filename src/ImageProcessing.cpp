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

std::unique_ptr<Image> ImageProcessing::convolution(const Image &image, const Kernel &kernel) {
    const unsigned int order = kernel.getOrder();
    const auto kernelWeights = kernel.getWeights();

    const unsigned int width = image.getWidth();
    const auto originalReds = image.getReds();
    const auto originalGreens = image.getGreens();
    const auto originalBlues = image.getBlues();

    const unsigned int outputHeight = image.getHeight() - (order - 1);
    const unsigned int outputWidth = width - (order - 1);

    std::vector<uint8_t> reds(outputWidth * outputHeight);
    std::vector<uint8_t> greens(outputWidth * outputHeight);
    std::vector<uint8_t> blues(outputWidth * outputHeight);
    for (unsigned int y = 0; y < outputHeight; y++) {
        for (unsigned int x = 0; x < outputWidth; x++) {
            float channelRed = 0;
            float channelGreen = 0;
            float channelBlue = 0;

            for (unsigned int j = 0; j < order; j++) {
                for (unsigned int i = 0; i < order; i++) {
                    const unsigned int pos = (y + j) * width + (x + i);
                    const float kernelWeight = kernelWeights[j * order + i];
                    channelRed += static_cast<float>(originalReds[pos]) * kernelWeight;
                    channelGreen += static_cast<float>(originalGreens[pos]) * kernelWeight;
                    channelBlue += static_cast<float>(originalBlues[pos]) * kernelWeight;
                }
            }
            reds[y * outputWidth + x] = getChannelAsUint8(channelRed);
            greens[y * outputWidth + x] = getChannelAsUint8(channelGreen);
            blues[y * outputWidth + x] = getChannelAsUint8(channelBlue);
        }
    }

    return std::make_unique<Image>(outputWidth, outputHeight, reds, greens, blues);
}


std::unique_ptr<Image> ImageProcessing::extendEdge(const Image &image, const unsigned int padding) {
    const auto originalReds = image.getReds();
    const auto originalGreens = image.getGreens();
    const auto originalBlues = image.getBlues();
    const unsigned int height = image.getHeight();
    const unsigned int width = image.getWidth();

    const unsigned int extendedHeight = height + 2 * padding;
    const unsigned int extendedWidth = width + 2 * padding;

    std::vector<uint8_t> reds(extendedWidth * extendedHeight);
    std::vector<uint8_t> greens(extendedWidth * extendedHeight);
    std::vector<uint8_t> blues(extendedWidth * extendedHeight);
    // copy image main data
    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < width; i++) {
            const unsigned int pos = (j + padding) * extendedWidth + (i + padding);
            const unsigned int idx = j * width + i;
            reds[pos] = originalReds[idx];
            greens[pos] = originalGreens[idx];
            blues[pos] = originalBlues[idx];
        }
    }

    // fill left internal new columns
    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < padding; i++) {
            const unsigned int pos = (j + padding) * extendedWidth + i;
            const unsigned int idx = j * width;
            reds[pos] = originalReds[idx];
            greens[pos] = originalGreens[idx];
            blues[pos] = originalBlues[idx];
        }
    }

    // fill right internal new columns
    for (unsigned int j = 0; j < height; j++) {
        for (unsigned int i = 0; i < padding; i++) {
            const unsigned int pos = (j + padding) * extendedWidth + (padding + width + i);
            const unsigned int idx = j * width + (width - 1);
            reds[pos] = originalReds[idx];
            greens[pos] = originalGreens[idx];
            blues[pos] = originalBlues[idx];
        }
    }

    // fill top internal new rows
    for (unsigned int j = 0; j < padding; j++) {
        for (unsigned int i = 0; i < width; i++) {
            const unsigned int pos = j * extendedWidth + (padding + i);
            const unsigned int idx = i;
            reds[pos] = originalReds[idx];
            greens[pos] = originalGreens[idx];
            blues[pos] = originalBlues[idx];
        }
    }

    // fill bottom internal new rows
    for (unsigned int j = 0; j < padding; j++) {
        for (unsigned int i = 0; i < width; i++) {
            const unsigned int pos = (padding + height + j) * extendedWidth + (padding + i);
            const unsigned int idx = (height - 1) * width + i;
            reds[pos] = originalReds[idx];
            greens[pos] = originalGreens[idx];
            blues[pos] = originalBlues[idx];
        }
    }

    // fill corners
    for (unsigned int j = 0; j < padding; j++) {
        for (unsigned int i = 0; i < padding; i++) {
            const unsigned int pos0 = j * extendedWidth + i;
            const unsigned int idx0 = 0;
            reds[pos0] = originalReds[idx0];
            greens[pos0] = originalGreens[idx0];
            blues[pos0] = originalBlues[idx0];

            // bottom-left
            const unsigned int pos1 = (extendedHeight - 1 - j) * extendedWidth + i;
            const unsigned int idx1 = (height - 1) * width;
            reds[pos1] = originalReds[idx1];
            greens[pos1] = originalGreens[idx1];
            blues[pos1] = originalBlues[idx1];

            // top-right
            const unsigned int pos2 = j * extendedWidth + (extendedWidth - 1 - i);
            const unsigned int idx2 = width - 1;
            reds[pos2] = originalReds[idx2];
            greens[pos2] = originalGreens[idx2];
            blues[pos2] = originalBlues[idx2];

            // bottom-right
            const unsigned int pos3 = (extendedHeight - 1 - j) * extendedWidth + (extendedWidth - 1 - i);
            const unsigned int idx3 = (height - 1) * width + (width - 1);
            reds[pos3] = originalReds[idx3];
            greens[pos3] = originalGreens[idx3];
            blues[pos3] = originalBlues[idx3];
        }
    }

    return std::make_unique<Image>(extendedWidth, extendedHeight, reds, greens, blues);
}