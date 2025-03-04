#include <iostream>
#include "Kernel.h"

Kernel::Kernel(std::string name, const unsigned int order, const std::vector<int>& weights, const int normFactor):
    name(std::move(name)), order(order), weights(weights), normalizationFactor(normFactor) {}

Kernel::~Kernel() = default;

std::string Kernel::getName() const {
    return name;
}

unsigned int Kernel::getOrder() const {
    return order;
}

std::vector<int> Kernel::getWeights() const {
    return weights;
}

int Kernel::getNormalizationFactor() const {
    return normalizationFactor;
}
