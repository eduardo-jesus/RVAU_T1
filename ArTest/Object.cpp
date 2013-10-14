#include "Object.h"

#include <GL/gl.h>

Object::Object() {
}


Object::~Object() {
}

Object::draw() {
    glPushMatrix();
    
    glEnd();
    glPopMatrix();
}

Object::update() {

}
