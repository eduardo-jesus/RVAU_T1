#include "Fortress.h"


Fortress::Fortress(void)
{
}


Fortress::~Fortress(void)
{
}


void Fortress::draw() {
    glPushMatrix();
    render();
    glPopMatrix();
}