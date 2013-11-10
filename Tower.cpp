#include "Tower.h"


Tower::Tower(void)
{
}


Tower::~Tower(void)
{
}

void Tower::draw() {
    glPushMatrix();
    render();
    glPopMatrix();
}
