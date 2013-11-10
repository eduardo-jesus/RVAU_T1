#include "SpawnMarker.h"


SpawnMarker::SpawnMarker(void)
{
}


SpawnMarker::~SpawnMarker(void)
{
}

void SpawnMarker::draw() {
    glPushMatrix();
    glTranslated(position_.x, position_.y, 0);
    render();
    glPopMatrix();
}
