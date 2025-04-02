#ifndef EDGEHANDLER_H
#define EDGEHANDLER_H
#include <memory>

#include "Image.h"


class EdgeHandler {
public:
    EdgeHandler();
    virtual ~EdgeHandler();

    virtual std::unique_ptr<Image> prepareImage(const Image& image, unsigned int padding) = 0;
};



#endif //EDGEHANDLER_H
