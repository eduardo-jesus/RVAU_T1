#include "Board.h"

#include <cmath>


Board::Board(void) {
    width_ = 250.0;
    height_ = -100.0;

    visible_ = false;
}


Board::~Board(void)
{
}

void Board::setDimensions(double width, double heigth) {
    if(abs(width_ - width) > 10) {
        width_ = width;
    }

    if(abs(height_ - heigth) > 10) {
        height_ = heigth;
    }
}

double Board::getWidth() {
    return width_;
}

double Board::getHeight() {
    return height_;
}

void Board::setVisible(bool visible) {
    visible_ = visible;
}

bool Board::isVisible() {
    return visible_;
}

bool Board::isOnBoard(double x, double y) {
    return !(x < 0 || x > width_ || y > 0 || y < height_);
}

bool Board::isOnBoard(Object* o) {
    return isOnBoard(o->getPosition().x, o->getPosition().y);
}