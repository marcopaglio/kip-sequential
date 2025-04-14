#ifndef EDGEHANDLER_H
#define EDGEHANDLER_H
#include <memory>

#include "Image.h"


/**
 * Represents an interface for handling image edges to prepare them for image processing
 * operations such as convolutions.
 */
class EdgeHandler {
public:
    /**
     * Default constructor.
     */
    EdgeHandler();

    /**
     * Default destructor.
     *
     * It is declared as virtual to ensure that the proper destructor is called for derived classes.
     */
    virtual ~EdgeHandler();

    /**
     * Prepares an image by adding a border around it with a specified padding size.
     *
     * This method is typically used to handle edge cases when performing image processing operations
     * such as convolutions. The padding ensures that computations on the edges of the image can
     * be carried out correctly by extending the borders.
     *
     * @param image The input image to be processed, represented as an `Image` object.
     * @param padding The size of the border to be added around the image in pixels.
     * @return A unique pointer to the prepared `Image` object with the specified padding applied.
     */
    virtual std::unique_ptr<Image> prepareImage(const Image& image, unsigned int padding) = 0;
};



#endif //EDGEHANDLER_H
