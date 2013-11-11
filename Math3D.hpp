#ifndef _MATH3D_
#define _MATH3D_

#include <vector>

#include <gl/glew.h>
#include <gl/glut.h>

#define M_PI 3.14159265358979323846

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Vector2 {
    double x;
    double y;

    Vector2();
    Vector2(double x, double y);
    ~Vector2();

    void set(double x, double y);
};

class Vector3 {
public:
    double x;
    double y;
    double z;

    Vector3();
    Vector3(double, double, double);
    ~Vector3();

    void set(double, double, double);
    double length();
    void normalize();

    static double distance(const Vector3 &v1, const Vector3 &v2);

    Vector3 operator + (const Vector3 &v);
    Vector3 operator - (const Vector3 &v);
    Vector3 operator * (const Vector3 &v);
    Vector3 operator / (const Vector3 &v);
};

struct Triangle {
    Vector3 vertices[3];
    Vector3 normals[3];
    Vector2 uvws[3];
    std::string material;
};

#endif /* _MATH3D_ */