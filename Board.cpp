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
    drawTowers();
    drawWalls();
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
    int horizontal_steps = width_ / 10;
    int vertical_steps = height_ / 10;

    double delta_x;
    double delta_y;

    double wall_bottom_y = -height_ - 5;
    double wall_right_x = width_ + 5;
     
    for(int i = 0; i < horizontal_steps; ++i) {
        delta_x = 10 * i + 5;
        
        glPushMatrix();
        glTranslated(delta_x, 5, 0);
        wall_.render();
        glPopMatrix();

        glPushMatrix();
        glTranslated(delta_x, wall_bottom_y, 0);
        wall_.render();
        glPopMatrix();
    }
    
    for(int i = 0; i < vertical_steps; ++i) {
        delta_y = -10 * i + 5;
        
        glPushMatrix();
        glTranslated(-5, delta_y, 0);
        wall_.render();
        glPopMatrix();
    }

    double right_vertical_steps = vertical_steps / 2 - 1;
    for(int i = 0; i < right_vertical_steps; ++i) {
        delta_y = -10 * i - 5;
        
        glPushMatrix();
        glTranslated(wall_right_x, delta_y, 0);
        wall_.render();
        glPopMatrix();
    }

    --right_vertical_steps;
    double right_second_segment_y = - right_vertical_steps * 10 - 40 - 5;
    for(int i = 0; i < right_vertical_steps; ++i) {
        delta_y = -10 * i + right_second_segment_y;
        
        glPushMatrix();
        glTranslated(wall_right_x, delta_y, 0);
        wall_.render();
        glPopMatrix();
    }

    double fortress_y = - floor(height_ / 2);
    glPushMatrix();
    glTranslated(width_, fortress_y, 0);
    fortress_.render();
    glPopMatrix();
}

void Board::loadBoardModels(std::string tower_model, std::string wall_model, std::string fortress_model) {
    tower_.load(tower_model);
    wall_.load(wall_model);
    fortress_.load(fortress_model);
}