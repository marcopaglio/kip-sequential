#ifndef KERNEL_H
#define KERNEL_H
#include <string>
#include <vector>


class Kernel {
public:
    explicit Kernel(std::string name, unsigned int order, const std::vector<int>& weights, int normFactor);
    ~Kernel();

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] unsigned int getOrder() const;
    [[nodiscard]] std::vector<int> getWeights() const;
    [[nodiscard]] int getNormalizationFactor() const;

private:
    std::string name;
    unsigned int order;
    std::vector<int> weights;
    int normalizationFactor;

};



#endif //KERNEL_H
