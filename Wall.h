#pragma once

#include "Object.h"

class Wall: public Object {
public:
    Wall(void);
    ~Wall(void);

    void draw();
};

