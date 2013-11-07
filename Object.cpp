#include "Object.h"

#include <GL/glut.h>

Object::Object() {
}

Object::~Object() {
}

void Object::draw() {
    glPushMatrix();
    
    glEnd();
    glPopMatrix();
}

void Object::update() {

}


Vector3 Object::getPosition() {
    return position_;
}

double Object::getX() {
    return position_.x;
}

void Object::setX(double x) {
    position_.x = x;
}

double Object::getY() {
    return position_.y;
}

void Object::setY(double y) {
    position_.y = y;
}

double Object::getZ() {
    return position_.z;
}

void Object::setZ(double z) {
    position_.z = z;
}
