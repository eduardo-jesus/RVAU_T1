#include "Pattern.h"

#include <algorithm>

Pattern::Pattern() {
}

Pattern::Pattern(std::string name, double width) {
    name_ = name;
    width_ = width;
    center_[0] = 0;
    center_[1] = 0;

    visible_ = false;
}

Pattern::Pattern(std::string name, double width, double center[]) {
    name_ = name;
    width_ = width;
    std::copy(center, center + 2, center_);

    visible_ = false;
}

Pattern::Pattern(std::string name, double width, double center[], ARMarkerInfo info) {
    name_ = name;
    width_ = width;
    std::copy(center, center + 2, center_);
    info_ = info;
    setTransMat();

    visible_ = false;
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

void Pattern::setId(int id) {
    id_ = id;
}

int Pattern::getId() {
    return id_;
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

double Pattern::angle(Pattern p1, Pattern p2) {
    double mat1[3][4], mat2[3][4];

    arUtilMatInv(p1.trans_, mat1);
    arUtilMatMul(mat1, p2.trans_, mat2);

    return - atan2(mat2[0][1],mat2[0][0]) * 180/3.14;
}

void Pattern::setVisible(bool) {
    visible_ = true;
}

bool Pattern::changeVisibility() {
    visible_ = !visible_;
    return visible_;
}

bool Pattern::isVisible() {
    return visible_;
}

int Pattern::loadPattern(Pattern& pattern) {
    int id = arLoadPatt(pattern.getName().c_str());
    if (id < 0) {
        printf("pattern %s load error\n", pattern.getName().c_str());
        getchar();
        exit(-1);
    }

    pattern.setId(id);

    return id;
}