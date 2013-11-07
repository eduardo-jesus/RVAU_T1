#pragma once

#include "Object.h"

class Player : public Object {
private:
    static const double X_SPEED;
    static const double Y_SPEED;

    bool moving_up_;
    bool moving_down_;
    bool alive_;
public:
    Player(void);
    Player(double, double);
    ~Player(void);

    void draw();

    void setPosition(double, double);
    void setMovingUp(bool);
    bool isMovingUp();
    void setMovingDown(bool);
    bool isMovingDown();
    void setAlive(bool);
    bool isAlive();

    void updatePlayerAnimation(double elapsed_time);
};
