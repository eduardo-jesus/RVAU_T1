#pragma once

#include "Object.h"

class Cannon : public Object {
private:
    double angle_;
    bool can_shoot_;
    bool shooting_;

    double radius_;
    double length_;
public:
    Cannon(void);
    ~Cannon(void);

    void setAngle(double);
    double getAngle();
    void setCanShoot(bool);
    bool canShoot();
    void setShooting(bool);
    bool isShooting();
    void draw();
    CollisionBox getCollisionBox();
};
