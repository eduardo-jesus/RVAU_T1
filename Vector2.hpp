#ifndef _VECTOR2_
#define _VECTOR2_

struct Vector2 {
    double x;
    double y;

    Vector2();
    Vector2(double x, double y);
    ~Vector2();

    void set(double x, double y);
};

#endif /* _VECTOR2_ */