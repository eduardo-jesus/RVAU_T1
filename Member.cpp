#include "Member.h"


Member::Member(void) {}


Member::~Member(void) {}

void Member::setXAngle(double angle) {
    x_angle_ = angle;
}

double Member::getXAngle()  {
    return x_angle_;
}

void Member::setYAngle(double angle)  {
    y_angle_ = angle;
}

double Member::getYAngle() {
    return y_angle_;
}

void Member::setRotateFront(bool rotate_front) {
    rotate_front_ = rotate_front;
}

bool Member::getRotateFront() {
    return rotate_front_;
}

void Member::instantiate(Vector3 position, double x_angle, double y_angle, bool rotate_front) {
    setPosition(position);
    setXAngle(x_angle);
    setYAngle(y_angle);
    setRotateFront(rotate_front);
}

void Member::draw() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, position_.z);

    glPushMatrix();
    glRotated(x_angle_, 1, 0, 0);
    glRotated(y_angle_, 0, 1, 0);
    render();
    glPopMatrix();

    glPopMatrix();
}

void Member::update(double elapsed_time) {
    double delta_ang = elapsed_time * ROT_SPEED;

    if(rotate_front_) {
        y_angle_ -= delta_ang;
        if(y_angle_ < - MAX_ROT) {
            rotate_front_ = false;
        }
    } else {
        y_angle_ += delta_ang;
        
        if(y_angle_ > MAX_ROT) {
            rotate_front_ = true;
        }
    }
}