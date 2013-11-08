#include "Cannon.h"

#include <GL/glut.h>
#include "AR\gsub.h"

#include <cstdio>
#include <ctime>
#include <cmath>

Cannon::Cannon(void) {
    angle_ = 0.0;
    can_shoot_ = false;
    shooting_ = false;

    radius_ = 20;
    length_ = 100;
}

Cannon::~Cannon(void)
{
}

void Cannon::setAngle(double angle) {
    angle_ = angle;
}

double Cannon::getAngle() {
    return angle_;
}

void Cannon::setCanShoot(bool can) {
    can_shoot_ = can;
}

bool Cannon::canShoot() {
    return can_shoot_;
}

void Cannon::setShooting(bool shooting) {
    shooting_ = shooting;
}

bool Cannon::isShooting() {
    return shooting_;
}

void Cannon::shoot() {
    printf("SHOOT %d\n", time(0));
}

void Cannon::draw() {
    //double matrix[16];
    //argConvGlpara(pattern_->getTrans(), matrix);
    
    glPushMatrix();
    //glLoadMatrixd(matrix);

    glTranslated(position_.x, position_.y, 5);

    glRotated(angle_, 0, 0, 1);
    glRotated(90.0, 0, 1, 0);
    //glutSolidCone(20, 100, 20, 20);
    glutSolidCone(radius_, length_, 20, 20);

    glPopMatrix();
}

#define PI 3.14159265

CollisionBox Cannon::getCollisionBox() {
    double direction_x_ = cos(angle_ * PI / 180.0);
    double direction_y_ = sin(angle_ * PI / 180.0);
    
    double v1_x = position_.x + radius_ * (- direction_y_);
    double v1_y = position_.y + radius_ * direction_x_;
    double v2_x = position_.x + radius_ * direction_y_;
    double v2_y = position_.y + radius_ * (- direction_x_);
    double v3_x = position_.x + length_ * direction_x_;
    double v3_y = position_.y + length_ * direction_y_;

    double min_x = v1_x;
    double max_x = v1_x;
    double min_y = v1_y;
    double max_y = v1_y;

    if(v2_x < min_x) {
        min_x = v2_x;
    }
    if(v3_x < min_x) {
        min_x = v3_x;
    }

    if(v2_x > max_x) {
        max_x = v2_x;
    }
    if(v3_x > max_x) {
        max_x = v3_x;
    }

    if(v2_y < min_y) {
        min_y = v2_y;
    }
    if(v3_y < min_y) {
        min_y = v3_y;
    }

    if(v2_y > max_y) {
        max_y = v2_y;
    }
    if(v3_y > max_y) {
        max_y = v3_y;
    }

    return CollisionBox(min_x, max_x, max_y, min_y);
}