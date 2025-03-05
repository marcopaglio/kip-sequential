#ifndef IMAGE_H
#define IMAGE_H
#include <vector>


class Image {
public:
    Image(unsigned int w, unsigned int h, const std::vector<uint8_t>& reds,
        const std::vector<uint8_t>& greens, const std::vector<uint8_t>& blues);
    ~Image();

    [[nodiscard]] unsigned int getWidth() const;
    [[nodiscard]] unsigned int getHeight() const;
    [[nodiscard]] std::vector<uint8_t> getReds() const;
    [[nodiscard]] std::vector<uint8_t> getGreens() const;
    [[nodiscard]] std::vector<uint8_t> getBlues() const;

private:
    unsigned int width, height;
    std::vector<uint8_t> reds;
    std::vector<uint8_t> greens;
    std::vector<uint8_t> blues;

};



#endif //IMAGE_H
