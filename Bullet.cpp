#include "Bullet.h"

#include <GL\glut.h>
#include <cmath>
#include <cstdio>

#define PI 3.14159265

Bullet::Bullet(void){
}

Bullet::Bullet(double x, double y, double angle) {
    position_ = Vector3(x, y, 20);
    setAngle(angle);
    speed_ = 20;
    moving_ = true;

    radius_ = 25;
    length_ = 50;
}

Bullet::~Bullet(void)
{
}


void Bullet::setPosition(double x, double y, double z) {
    position_ = Vector3(x, y, z);
}
Vector3 Bullet::getPosition() {
    return position_;
}

void Bullet::incPosition(double delta_x, double delta_y) {
    position_.x += delta_x;
    position_.y += delta_y;
}

void Bullet::setSpeed(double speed) {
    speed_ = speed;
}

double Bullet::getSpeed() {
    return speed_;
}

void Bullet::setAngle(double angle) {
    angle_ = angle;
    direction_x_ = cos(angle * PI / 180.0);
    direction_y_ = sin(angle * PI / 180.0);
}

void Bullet::setDirectionX(double direction_x) {
    direction_x_ = direction_x;
}

double Bullet::getDirectionX() {
    return direction_x_;
}

void Bullet::setDirectionY(double direction_y) {
    direction_y_ = direction_y;
}

double Bullet::getDirectionY() {
    return direction_y_;
}

void Bullet::setMoving(bool moving) {
    moving_ = moving;
}

bool Bullet::isMoving() {
    return moving_;
}

void Bullet::draw() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, position_.z);
    glRotated(angle_, 0, 0, 1);
    glRotated(90.0, 0, 1, 0);
    
    //glutSolidCone(25, 50, 20, 20);
    glutSolidCone(radius_, length_, 20, 20);
    
    glPopMatrix();
}

void Bullet::updateBulletPosition(double elapsed_time) {
    double delta_x = direction_x_ * speed_ * elapsed_time;
    double delta_y = direction_y_ * speed_ * elapsed_time;
    incPosition(delta_x, delta_y);
}

CollisionBox Bullet::getCollisionBox() {
    // if we define dx=x2-x1 and dy=y2-y1, then the normals are (-dy, dx) and (dy, -dx).
    double v1_x = position_.x + radius_ * (- direction_y_);
    double v1_y = position_.y + radius_ * direction_x_;
    double v2_x = position_.x + radius_ * direction_y_;
    double v2_y = position_.y + radius_ * (- direction_x_);
    double v3_x = position_.x + length_ * direction_x_;
    double v3_y = position_.y + length_ * direction_y_;

    double min_x = v1_x;
    double max_x = v1_x;
    double min_y = v1_y;
    double max_y = v1_y;

    if(v2_x < min_x) {
        min_x = v2_x;
    }
    if(v3_x < min_x) {
        min_x = v3_x;
    }

    if(v2_x > max_x) {
        max_x = v2_x;
    }
    if(v3_x > max_x) {
        max_x = v3_x;
    }

    if(v2_y < min_y) {
        min_y = v2_y;
    }
    if(v3_y < min_y) {
        min_y = v3_y;
    }

    if(v2_y > max_y) {
        max_x = v2_x;
    }
    if(v3_y > max_y) {
        max_y = v3_y;
    }

    return CollisionBox(min_x, max_x, max_y, min_y);

}