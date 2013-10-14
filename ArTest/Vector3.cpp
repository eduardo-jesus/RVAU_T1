#include <cmath>

#include "Vector3.h"

Vector3::Vector3() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vector3::Vector3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::~Vector3() {}

void Vector3::set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3::length() {
    return sqrt(x * x + y * y + z * z);
}

void Vector3::normalize() {
    double l = length();

    x /= l;
    y /= l;
    z /= l;
}

static float distance(const Vector3 &v1, const Vector3 &v2) {
    double dx = v1.x - v2.x;
    double dy = v1.y - v2.y;
    double dz = v1.z - v2.z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3 Vector3::operator + (const Vector3 &v) {
    Vector3 result = Vector3();

    result.x = x + v.x;
    result.y = y + v.y;
    result.z = z + v.z;

    return result;
}

Vector3 Vector3::operator - (const Vector3 &v) {
    Vector3 result = Vector3();

    result.x = x - v.x;
    result.y = y - v.y;
    result.z = z - v.z;

    return result;
}

Vector3 Vector3::operator * (const Vector3 &v) {
    Vector3 result = Vector3();

    result.x = x * v.x;
    result.y = y * v.y;
    result.z = z * v.z;

    return result;
}

Vector3 Vector3::operator / (const Vector3 &v) {
    Vector3 result = Vector3();

    result.x = x / v.x;
    result.y = y / v.y;
    result.z = z / v.z;

    return result;
}

