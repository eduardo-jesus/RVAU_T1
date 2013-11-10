#include "RotateButton.h"


RotateButton::RotateButton(void) {}


RotateButton::~RotateButton(void) {}

double (& RotateButton::getTransMatrix()) [16] {
    return trans_matrix_;
}

void RotateButton::draw() {
    glPushMatrix();
    glLoadMatrixd(trans_matrix_);
    render();
    glPopMatrix();
}