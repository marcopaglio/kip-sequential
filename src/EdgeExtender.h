#ifndef EDGEEXTENDER_H
#define EDGEEXTENDER_H
#include <memory>

#include "Image.h"
#include "EdgeHandler.h"


/**
 * A final concrete implementation of the @ref EdgeHandler interface for preparing an image
 * by replicating the edge pixels outward to a specified padding width.
 */
class EdgeExtender final : public EdgeHandler {
public:
    /**
     * Default constructor.
     */
    EdgeExtender();

    /**
     * Default destructor.
     */
    ~EdgeExtender() override;

    /**
     * Extends the edges of the given image by padding a specified number of pixels around it.
     * The edge pixels are replicated outward to fill the padding region.
     *
     * @param image The original image to be padded.
     * @param padding The number of pixels to add around each edge of the input image.
     * @return A unique pointer to a new Image object with extended edges.
     */
    std::unique_ptr<Image> prepareImage(const Image& image, unsigned int padding) override;
};



#endif //EDGEEXTENDER_H
