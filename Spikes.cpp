#include "Spikes.h"

#include <GL\glut.h>

Spikes::Spikes(void)
{
}


Spikes::Spikes(double x, double y, double width, double height) {
    position_.x = x;
    position_.y = y;
    width_ = width;
    height_ = height;
}

Spikes::~Spikes(void)
{
}

/*
void Spikes::setVisible(bool visible) {
    visible_ = visible;
}

bool Spikes::isVisible() {
    return visible_;
}*/

void Spikes::draw() {
    glPushMatrix();

    GLfloat   mat_ambient[]     = {0.0, 1.0, 0.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 1.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    //Bottom
    glNormal3d(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , 1);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , 1);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , 1);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , 1);
    glEnd();

    glPopMatrix();

}

CollisionBox Spikes::getCollisionBox() {
    double left = position_.x - width_/2.0;
    double right = position_.x + width_/2.0;
    double top = position_.y + height_/2.0;
    double bottom = position_.y - width_/2.0;

    return CollisionBox(left, right, top, bottom);
}