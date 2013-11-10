#pragma once

#include "Object.h"
#include "Math3D.hpp"

class Member: public Object
{
private:
    double x_angle_;
    double y_angle_;

    bool rotate_front_;

#define MAX_ROT 70.0
#define ROT_SPEED 20.0
public:
    Member(void);
    ~Member(void);

    void instantiate(Vector3 position, double x_angle, double y_angle, bool rotate_front);

    void setXAngle(double angle);
    double getXAngle();
    void setYAngle(double angle);
    double getYAngle();
    void setRotateFront(bool rotate_front);
    bool getRotateFront();

    void draw();
    void update(double elapsed_time);
};

