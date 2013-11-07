#pragma once

#include <string>

#include <AR/ar.h>
#include "Vector3.h"

class Pattern {
public:
    Pattern();
    Pattern(std::string name, double width);
    Pattern(std::string, double, double[]);
    Pattern(std::string, double, double[], ARMarkerInfo);
    ~Pattern();

    void setName(std::string);
    std::string getName();
    void setWidth(double);
    double getWidth();

    void setId(int);
    int getId();
    double (& getTrans()) [3][4];
    void setInfo(ARMarkerInfo);
    void setTransMat(); //Set trans_ using arGetTransMat. info_ must not be null 

    static int loadPattern(Pattern& pattern);

    static Vector3 distance(Pattern, Pattern);
    static double angle(Pattern, Pattern);

    void setVisible(bool);
    bool changeVisibility();
    bool isVisible();
        
private:
    int id_;
    std::string name_;
    double width_;
    double center_[2];
    double trans_[3][4];
    ARMarkerInfo info_;

    bool visible_;
};

