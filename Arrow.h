#pragma once

#include "Object.h"

class Arrow: public Object {
private:
    double trans_matrix_[16];
    bool down_;
public:
    Arrow(void);
    Arrow(bool down);
    ~Arrow(void);

    void setDown(bool down);
    bool isDown();

    double (& getTransMatrix()) [16];

    void draw();
    void drawUp();
    void drawDown();
};

