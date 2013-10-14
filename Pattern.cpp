#include "Pattern.h"

#include <algorithm>

Pattern::Pattern() {
}

Pattern::Pattern(std::string name, double width, double center[]) {
    name_ = name;
    width_ = width;
    //std::copy(center, center_, sizeof(center));
}

Pattern::~Pattern() {}
