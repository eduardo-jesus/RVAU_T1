#pragma once

#include "Object.h"

class Hole : public Object {
private:
    double depth_;

    bool visible_;
public:
    Hole(void);
    Hole(double x, double y, double width, double height, double depth_);
    ~Hole(void);

    void setVisible(bool visible);
    bool isVisible();

    void draw();
    CollisionBox getCollisionBox();
};

