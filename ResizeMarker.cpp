#include "ResizeMarker.h"


ResizeMarker::ResizeMarker(void)
{
}


ResizeMarker::~ResizeMarker(void)
{
}

double (& ResizeMarker::getTransMatrix()) [16] {
    return trans_matrix_;
}

void ResizeMarker::draw() {
    glPushMatrix();
    glLoadMatrixd(trans_matrix_);
    render();
    glPopMatrix();
}