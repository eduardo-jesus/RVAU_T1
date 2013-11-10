#include <gl/glew.h>

#include "Spikes.h"

Spikes::Spikes()
{
}

Spikes::Spikes(double x, double y, double width, double height) {
    position_.x = x;
    position_.y = y;
    width_ = width;
    height_ = height;
}

Spikes::~Spikes()
{
}

void Spikes::draw() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, 1);
    render();
    glPopMatrix();

}

CollisionBox Spikes::getCollisionBox() {
    double left = position_.x - width_/2.0;
    double right = position_.x + width_/2.0;
    double top = position_.y + height_/2.0;
    double bottom = position_.y - width_/2.0;

    return CollisionBox(left, right, top, bottom);
}