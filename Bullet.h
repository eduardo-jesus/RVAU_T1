#pragma once

#include "Object.h"

class Bullet : private Object {
private:
    Vector3 position_;
    double speed_;
    double angle_;
    double direction_x_;
    double direction_y_;
   
    bool moving_;
public:
    Bullet(void);
    Bullet(double x, double y, double angle);
    ~Bullet(void);

    void setPosition(double x, double y, double z);
    Vector3 getPosition();
    void incPosition(double delta_x, double delta_y);
    void setSpeed(double speed);
    double getSpeed();
    void setAngle(double angle);
    double getAngle();
    void setDirectionX(double direction_x);
    double getDirectionX();
    void setDirectionY(double direction_y);
    double getDirectionY();
    void setMoving(bool moving);
    bool isMoving();

    void draw();
    void updateBulletPosition(double elapsed_time);
};

