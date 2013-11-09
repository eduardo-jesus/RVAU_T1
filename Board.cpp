#include "Board.h"

#include <cmath>


Board::Board(void) {
    width_ = MIN_WIDTH;//250.0;
    height_ = MIN_HEIGHT;//-100.0;

    visible_ = false;
}


Board::~Board(void) {
}

/*
void Board::setDimensions(double width, double heigth) {
    if(abs(width_ - width) > 10) {
        width_ = width;
    }

    if(abs(height_ - heigth) > 10) {
        height_ = heigth;
    }
}*/

void Board::setDimensions(double right_bottom_x, double right_bottom_y) {
    if(right_bottom_x < 0 || right_bottom_y > 0) {
        return;
    }
    
    if(right_bottom_x > MIN_WIDTH) {
        width_ = right_bottom_x;
    }

    right_bottom_y *= -1;
    if(right_bottom_y > MIN_HEIGHT) {
        height_ = right_bottom_y;
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
    return !(x < 0 || x > width_ || y > 0 || y < -height_);
}

bool Board::isOnBoard(Object* o) {
    CollisionBox box = o->getCollisionBox();

    return box.top < 0 && box.left > 0 && box.bottom > -height_ && box.right < width_;
}

bool Board::hasPlayerFinished(Player player) {
    return player.getPosition().x + player.getWidth() / 2.0 >= width_;
}