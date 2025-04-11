#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <memory>

#include "EdgeHandler.h"
#include "Image.h"
#include "Kernel.h"


class ImageProcessing {
public:
    explicit ImageProcessing(EdgeHandler& edgeHandler);
    ~ImageProcessing();

    std::unique_ptr<Image> convolution(const Image& image, const Kernel& kernel) const;

private:
    EdgeHandler& edgeHandler;
};



#endif //IMAGEPROCESSING_H
