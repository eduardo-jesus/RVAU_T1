#include <cmath>

#include <gl/glew.h>

#include "Bullet.h"
#include "Math3D.hpp"

Bullet::Bullet() {
    moving_ = false;
}

Bullet::~Bullet() {
}


void Bullet::newInstance(double x, double y, double angle) {
    angle += 180;
    direction_x_ = cos(angle * M_PI / 180.0);
    direction_y_ = sin(angle * M_PI / 180.0);

    x += 22.5 * direction_x_;
    y += 22.5 * direction_y_;

    position_ = Vector3(x, y, 8.0);

    speed_ = 20;
    moving_ = true;
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
    render();
    
    glPopMatrix();
}

void Bullet::updateBulletPosition(double elapsed_time) {
    double delta_x = direction_x_ * speed_ * elapsed_time;
    double delta_y = direction_y_ * speed_ * elapsed_time;
    incPosition(delta_x, delta_y);
}

CollisionBox Bullet::getCollisionBox() {
    double min_x = position_.x - 2.5;
    double min_y = position_.y - 2.5;
    double max_x = position_.x + 2.5;
    double max_y = position_.y + 2.5;

    return CollisionBox(min_x, max_x, max_y, min_y);

}