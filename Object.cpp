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

void Object::setPattern(Pattern *pattern) {
    pattern_ = pattern;
}

Pattern *Object::getPattern() {
    return pattern_;
}

void Object::setWidth(double width) {
    width_ = width;
}

double Object::getWidth() {
    return width_;
}

void Object::setHeight(double height) {
    height_ = height;
}

double Object::getHeight() {
    return height_;
}

CollisionBox Object::getCollisionBox() {
    return CollisionBox();
}

bool Object::isCollidingWith(Object* obj) {
    // http://lazyfoo.net/SDL_tutorials/lesson17/

    CollisionBox a = getCollisionBox();
    CollisionBox b = obj->getCollisionBox();

    //The sides of the rectangles
    /*double left_A, left_B;
    double right_A, right_B;
    double top_A, top_B;
    double bottom_A, bottom_B;

    //Calculate the sides of rect A
    left_A = position_.x;
    right_A = left_A + width_;
    top_A = position_.y;
    bottom_A = top_A - height_;
        
    //Calculate the sides of rect B
    left_B = obj->getX();
    right_B = left_B + obj->getWidth();
    top_B = obj->getY();
    bottom_B = top_B - obj->getHeight;*/

    //If any of the sides from A are outside of B
    if(a.bottom >= b.top)
    {
        return false;
    }
    
    if(a.top <= b.bottom)
    {
        return false;
    }
    
    if(a.right <= b.left)
    {
        return false;
    }
    
    if(a.left >= b.right)
    {
        return false;
    }
   
    //If none of the sides from A are outside B
    return true;
}
