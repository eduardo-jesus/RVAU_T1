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