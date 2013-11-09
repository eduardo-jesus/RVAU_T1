#pragma once

#include "Object.h"

class Player : public Object {
private:
    static const double X_SPEED;
    static const double Y_SPEED;

    bool moving_up_;
    bool moving_down_;
    bool alive_;
    unsigned int n_deaths_;
public:
    Player();
    ~Player();

    void draw();
    CollisionBox getCollisionBox();

    void setPosition(double, double);
    void setMovingUp(bool);
    bool isMovingUp();
    void setMovingDown(bool);
    bool isMovingDown();
    void setAlive(bool);
    bool isAlive();
    void setNDeaths(unsigned int n_deaths);
    unsigned int getNDeaths();
    void incNDeaths();

    void kill();
    void respawn(double x, double y);

    void updatePlayerAnimation(double elapsed_time);
};
