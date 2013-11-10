#pragma once

#include "Object.h"
#include "CollisionBox.h"

class Tower: public Object {
public:
    Tower(void);
    ~Tower(void);

    void draw();
};

