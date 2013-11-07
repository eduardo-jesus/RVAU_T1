#include "CollisionBox.h"


CollisionBox::CollisionBox(void)
{
}

CollisionBox::CollisionBox(double left, double right, double top, double bottom) {
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
}


CollisionBox::~CollisionBox(void)
{
}
