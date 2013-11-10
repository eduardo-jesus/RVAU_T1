#include "Board.h"

#include <cmath>


Board::Board(void) {
    width_ = MIN_WIDTH;//250.0;
    height_ = MIN_HEIGHT;//-100.0;

    visible_ = false;
}


Board::~Board(void) {
}

void Board::setDimensions(double right_bottom_x, double right_bottom_y) {
    if(right_bottom_x < 0 || right_bottom_y > 0) {
        return;
    }
    
    if(right_bottom_x > MIN_WIDTH && (int) right_bottom_x % 10 == 0) {
        width_ = right_bottom_x;
    }

    right_bottom_y *= -1;
    if(right_bottom_y > MIN_HEIGHT && (int) right_bottom_y % 10 == 0 ) {
        height_ = right_bottom_y;
    }
}
/*
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
}*/

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

void Board::draw() {
}

void Board::drawTowers() {
    glPushMatrix();
    tower_.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -height_, 0);
    tower_.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(width_, -height_, 0);
    tower_.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(width_, 0, 0);
    tower_.render();
    glPopMatrix();
}

void Board::drawWalls() {
    int width = (int) width_;
    int height = (int) height_;

    double delta_x;
    double delta_y;

    double wall_bottom_y = -height_ - .5;
    
    for(int i = 1; i <= width; ++i) {
        delta_x = 0.5 * i;
        
        glPushMatrix();
        glTranslated(0.5 * i, 0.5, 0);
        wall_.render();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0.5 * i, wall_bottom_y, 0);
        wall_.render();
        glPopMatrix();
    }

    for(int i = 1; i <= width; ++i) {
        delta_x = 0.5 * i;
        
        glPushMatrix();
        glTranslated(delta_x, 0.5, 0);
        wall_.render();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0.5 * i, wall_bottom_y, 0);
        wall_.render();
        glPopMatrix();
    }

    for(int i = 1; i <= height; ++i) {
        delta_y = -0.5 * i;
        
        glPushMatrix();
        glTranslated(-0.5, delta_y, 0);
        wall_.render();
        glPopMatrix();
    }
}