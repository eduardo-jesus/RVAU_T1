#pragma once

#include "Vector3.h"
#include "Pattern.h"
#include "CollisionBox.h"

class Object {
public:
    Object();
    ~Object();
    
    virtual void draw() = 0;
    void update();
    virtual CollisionBox getCollisionBox();

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

    void setPattern(Pattern *pattern);
    Pattern *getPattern();

    void setWidth(double width);
    double getWidth();
    void setHeight(double height);
    double getHeight();

    bool isCollidingWith(Object*);
protected:
    Vector3 position_;
    Vector3 scale_;
    //Matrix transform_;

    double width_;
    double height_;

    Pattern *pattern_;
};

