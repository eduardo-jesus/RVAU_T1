#include "Pattern.h"

#include <algorithm>

Pattern::Pattern() {
}

Pattern::Pattern(std::string name, double width, double center[]) {
    name_ = name;
    width_ = width;
    std::copy(center, center + 2, center_);
}

Pattern::~Pattern() {}

void Pattern::setName(std::string name) {
    name_ = name;
}

std::string Pattern::getName() {
    return name_;
}

void Pattern::setWidth(double width) {
    width_ = width;
}

double Pattern::getWidth() {
    return width_;
}






