#pragma once

#include "Vector3.h"

class Object {
public:
    Object();
    ~Object();
    
    virtual void draw() = 0;
    void update();

    void setPosition(Vector3 position);
    Vector3 getPosition();

    double getX();
    void setX(double x);
    double getY();
    void setY(double y);
    double getZ();
    void setZ(double z);

    void setScale(float scale);
    Vector3 getScale();
protected:
    Vector3 position_;
    Vector3 scale_;
    //Matrix transform_;
};

