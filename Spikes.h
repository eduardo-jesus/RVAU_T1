#pragma once

#include "Object.h"

class Spikes : public Object {
private:
    //bool visible_;
public:
    Spikes(void);
    Spikes(double x, double y, double width, double height);
    ~Spikes(void);

    //void setVisible(bool visible);
    //bool isVisible();

    void draw();
    CollisionBox getCollisionBox();
};

