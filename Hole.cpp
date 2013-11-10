#include "Hole.h"

#include <GL\glut.h>


Hole::Hole(void)
{
}

Hole::Hole(double x, double y, double width, double height, double depth) {
    position_.x = x;
    position_.y = y;
    width_ = width;
    height_ = height;
    depth_ = depth;
}

Hole::~Hole(void)
{
}
/*
void Hole::setVisible(bool visible) {
    visible_ = visible;
}

bool Hole::isVisible() {
    return visible_;
}*/

void Hole::draw() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, 1);
    render();

    glPopMatrix();
}

CollisionBox Hole::getCollisionBox() {
    double left = position_.x - width_/2.0;
    double right = position_.x + width_/2.0;
    double top = position_.y + height_/2.0;
    double bottom = position_.y - width_/2.0;

    return CollisionBox(left, right, top, bottom);
}