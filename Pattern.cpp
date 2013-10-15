#include "Pattern.h"

#include <algorithm>

Pattern::Pattern() {
}

Pattern::Pattern(std::string name, double width, double center[]) {
    name_ = name;
    width_ = width;
    std::copy(center, center + 2, center_);
}

Pattern::Pattern(std::string name, double width, double center[], ARMarkerInfo info) {
    name_ = name;
    width_ = width;
    std::copy(center, center + 2, center_);
    info_ = info;
    setTransMat();
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

double (& Pattern::getTrans()) [3][4] {
    return trans_;
}

void Pattern::setInfo(ARMarkerInfo info) {
    info_ = info;
}

void Pattern::setTransMat() {
    arGetTransMat(&info_, center_, width_, trans_);
}

Vector3 Pattern::distance(Pattern p1, Pattern p2) {
    double mat1[3][4], mat2[3][4];

    arUtilMatInv(p1.trans_, mat1);
    arUtilMatMul(mat1, p2.trans_, mat2);

    return Vector3(mat2[0][3], mat2[1][3], mat2[2][3]);
}






