#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <memory>

#include "EdgeHandler.h"
#include "Image.h"
#include "Kernel.h"


/**
 * Provides functionality for basic image processing tasks, such as convolution, using a specified edge handling strategy.
 */
class ImageProcessing {
public:
    /**
     * Constructs an ImageProcessing object with the specified EdgeHandler.
     *
     * @param edgeHandler Reference to the EdgeHandler object responsible for managing edge operations.
     */
    explicit ImageProcessing(EdgeHandler& edgeHandler);

    /**
     * Default destructor.
     */
    ~ImageProcessing();


    /**
     * Applies a convolution operation on the given image using the specified kernel.
     *
     * Padding is managed internally to accommodate the kernel size using the @ref EdgeHandler strategy.
     *
     * @param image The input image to which the convolution operation will be performed.
     * @param kernel The kernel used for the convolution.
     * @return A unique pointer to a new Image object resulting from the convolution operation.
     */
    std::unique_ptr<Image> convolution(const Image& image, const Kernel& kernel) const;

private:
    /**
     * EdgeHandler instance used to manage edge-related operations during image processing tasks.
     */
    EdgeHandler& edgeHandler;
};



#endif //IMAGEPROCESSING_H
