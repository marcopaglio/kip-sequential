#include <iostream>
#include <sstream>

#include "HighResolutionTimer.h"
#include "Image.h"
#include "Kernel.h"
#include "STBImageReader.h"
#include "ImageProcessing.h"
#include "KernelFactory.h"
#include "SteadyTimer.h"
#include "Timer.h"

#define NUM_KERNELS 2
#define NUM_REPS 3

int main() {
    std::unique_ptr<Timer> timer;
    if constexpr (std::chrono::high_resolution_clock::is_steady)
        timer = std::make_unique<HighResolutionTimer>();
    else
        timer = std::make_unique<SteadyTimer>();

    try {
        constexpr unsigned int imageQuality = 4;
        constexpr unsigned int imageNum = 1;
        const std::string imageName = std::to_string(imageQuality) + "K-" + std::to_string(imageNum);
        std::stringstream fullPathStream;
        STBImageReader imageReader{};

        // load img
        fullPathStream << PROJECT_SOURCE_DIR << "/src/imgs/input/" << imageName << ".jpg";
        auto img = imageReader.loadRGBImage(fullPathStream.str());
        std::cout << "Image " << img->getWidth() << "x" << img->getHeight() <<
            " loaded from: " << fullPathStream.str() << std::endl;
        fullPathStream.str(std::string());

        // enlargement
        constexpr unsigned int order = 7;
        img = ImageProcessing::extendEdge(*img, (order - 1) / 2);
        std::cout << "Image enlarged to " << img->getWidth() << "x" << img->getHeight() << std::endl;

        for (unsigned int kernelType = 0; kernelType < NUM_KERNELS; kernelType++) {
            // create kernel
            std::unique_ptr<Kernel> kernel;
            switch (kernelType) {
                case 0:
                    kernel = KernelFactory::createBoxBlurKernel(order);
                    break;
                case 1:
                    kernel = KernelFactory::createEdgeDetectionKernel(order);
                    break;
                default:
                    throw std::invalid_argument("Invalid kernel type");
            }
            std::cout << "Kernel of type \"" << kernel->getName() << "\" and size " << kernel->getOrder() <<
                " created." << std::endl;

            // transform
            const std::chrono::duration<double> wall_clock_time_start = timer->now();
            std::unique_ptr<Image> outputImage;
            for (unsigned int rep = 0; rep < NUM_REPS; rep++)
                outputImage = ImageProcessing::convolution(*img, *kernel);
            const std::chrono::duration<double> wall_clock_time_end = timer->now();
            const std::chrono::duration<double> wall_clock_time_duration = wall_clock_time_end - wall_clock_time_start;
            std::cout << "Image processed " << NUM_REPS << " times in " << wall_clock_time_duration.count() << " seconds [Wall Clock]" <<
                " with an average of " << wall_clock_time_duration.count() / NUM_REPS << " seconds per repetition." << std::endl;

            // save
            fullPathStream << PROJECT_SOURCE_DIR << "/src/imgs/output/" << imageName <<
                "_" << kernel->getName() << kernel->getOrder() << ".jpg";
            imageReader.saveJPGImage(*outputImage, fullPathStream.str());
            std::cout << "Image " << outputImage->getWidth() << "x" << outputImage->getHeight() <<
                " saved at: " << fullPathStream.str() << std::endl << std::endl;
            fullPathStream.str(std::string());
        }

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}