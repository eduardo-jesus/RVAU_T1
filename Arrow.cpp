#include "Arrow.h"


Arrow::Arrow(void) {
    down_ = false;
}

Arrow::Arrow(bool down) {
    down_ = down;
}

Arrow::~Arrow(void)
{
}

void Arrow::setDown(bool down) {
    down_ = down;
}

bool Arrow::isDown() {
    return down_;
}

double (& Arrow::getTransMatrix()) [16] {
    return trans_matrix_;
}

void Arrow::draw() {
    down_ ? drawDown() : drawUp();
}

void Arrow::drawUp() {
    glPushMatrix();
    glLoadMatrixd(trans_matrix_);
    render();
    glPopMatrix();
}

void Arrow::drawDown() {
    glPushMatrix();
    glLoadMatrixd(trans_matrix_);
    glRotated(180, 0, 0, 1);
    render();
    glPopMatrix();
}