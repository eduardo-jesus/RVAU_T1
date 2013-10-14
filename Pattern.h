#pragma once

#include <string>

#include <AR/ar.h>

class Pattern {
public:
    Pattern();
    Pattern(std::string, double, double[]);
    ~Pattern();
private:
    int id_;
    std::string name_;
    double width_;
    double center_[2];
    double trans_[3][4];
    ARMarkerInfo info_;
};

