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

void Bullet::drawBullet() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, position_.z);
    glRotated(angle_, 0, 0, 1);
    glRotated(90.0, 0, 1, 0);
    
    glutSolidCone(25, 50, 20, 20);
    
    glPopMatrix();
}

void Bullet::updateBulletPosition(double elapsed_time) {
    double delta_x = direction_x_ * speed_ * elapsed_time;
    double delta_y = direction_y_ * speed_ * elapsed_time;
    incPosition(delta_x, delta_y);
}