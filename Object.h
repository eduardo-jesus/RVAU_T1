#pragma once

#include "Vector3.h"

class Object {
public:
    Object();
    ~Object();
    
    void draw();
    void update();

    void setPosition(Vector3 position);
    Vector3 getPosition();
    void setScale(float scale);
    Vector3 getScale();
protected:
    Vector3 position_;
    Vector3 scale_;
    //Matrix transform_;
};

