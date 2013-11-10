#pragma once

#include <string>

#include "Object.h"
#include "Member.h"

class Player : public Object {
private:
    static const double X_SPEED;
    static const double Y_SPEED;

    Member left_arm_;
    Member right_arm_;
    Member left_leg_;
    Member right_leg_;

    bool moving_up_;
    bool moving_down_;
    bool alive_;
    unsigned int n_deaths_;
public:
    Player();
    ~Player();

    void draw();
    void update(double elapsed_time);
    CollisionBox getCollisionBox();

    void loadPlayerModels(std::string body, std::string arm, std::string leg);

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
