#include "Vector2.hpp"

Vector2::Vector2() {
    x = 0.0;
    y = 0.0;
}

Vector2::Vector2(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector2::~Vector2() {

}

void Vector2::set(double x, double y) {
    this->x = x;
    this->y = y;
}