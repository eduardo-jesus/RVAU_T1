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

void Hole::setVisible(bool visible) {
    visible_ = visible;
}

bool Hole::isVisible() {
    return visible_;
}

void Hole::draw() {
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
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , -depth_);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , -depth_);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , -depth_);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , -depth_);
    glEnd();

    //UP SIDE
    glNormal3d(0, -1, 0);
    glBegin(GL_POLYGON);
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , 0);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , 0);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , -depth_);
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , -depth_);
    glEnd();

    //DOWN SIDE
    glNormal3d(0, 1, 0);
    glBegin(GL_POLYGON);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , 0);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , 0);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , -depth_);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , -depth_);
    glEnd();
    
    //LEFT SIDE
    glNormal3d(1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , -depth_);
    glVertex3d(position_.x - width_/2.0, position_.y + height_/2.0 , 0);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , 0);
    glVertex3d(position_.x - width_/2.0, position_.y - height_/2.0 , -depth_);
    glEnd();

    //LEFT SIDE
    glNormal3d(-1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , 0);
    glVertex3d(position_.x + width_/2.0, position_.y + height_/2.0 , -depth_);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , -depth_);
    glVertex3d(position_.x + width_/2.0, position_.y - height_/2.0 , 0);
    glEnd();

    glPopMatrix();
}

CollisionBox Hole::getCollisionBox() {
    double left = position_.x - width_/2.0;
    double right = position_.x + width_/2.0;
    double top = position_.y + height_/2.0;
    double bottom = position_.y - width_/2.0;

    return CollisionBox(left, right, top, bottom);
}