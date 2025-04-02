#ifndef EDGEEXTENDER_H
#define EDGEEXTENDER_H
#include <memory>

#include "Image.h"
#include "EdgeHandler.h"


class EdgeExtender final : public EdgeHandler {
public:
    EdgeExtender();
    ~EdgeExtender() override;

    std::unique_ptr<Image> prepareImage(const Image& image, unsigned int padding) override;
};



#endif //EDGEEXTENDER_H
