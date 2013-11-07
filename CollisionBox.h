#pragma once
class CollisionBox
{
public:
    CollisionBox(void);
    CollisionBox(double left, double right, double top, double bottom);
    ~CollisionBox(void);

    double left;
    double right;
    double top;
    double bottom;
};

