#pragma once
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

