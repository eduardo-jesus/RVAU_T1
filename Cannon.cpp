#include "Cannon.h"

#include <GL/glut.h>
#include "AR\gsub.h"

#include <cstdio>
#include <ctime>

Cannon::Cannon(void) {
    angle_ = 0.0;
    can_shoot_ = false;
    shooting_ = false;
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
    glutSolidCone(20, 100, 20, 20);

    glPopMatrix();
}