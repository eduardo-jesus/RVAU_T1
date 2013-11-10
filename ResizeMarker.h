#pragma once

#include "Object.h"

class ResizeMarker: public Object {
private:
    double trans_matrix_[16];
public:
    ResizeMarker(void);
    ~ResizeMarker(void);

    double (& getTransMatrix()) [16];
    void draw();
};

