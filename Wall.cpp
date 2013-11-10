#include "Wall.h"


Wall::Wall(void)
{
}


Wall::~Wall(void)
{
}

void Wall::draw() {
    glPushMatrix();
    render();
    glPopMatrix();
}