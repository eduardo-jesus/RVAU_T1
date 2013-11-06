#include "Player.h"

#include <GL/glut.h>

Player::Player(void) {
    x_ = 0;
    y_ = 0;

    x_speed_ = 25;
    y_speed_ = 10;

    moving_up_ = false;
    moving_down_ = false;
    alive_ = false;
}

Player::Player(double x, double y) {
    x_ = x;
    y_ = y;

    x_speed_ = 50;
    y_speed_ = 10;

    moving_up_ = false;
    moving_down_ = false;
    alive_ = true;
}


Player::~Player(void) {

}

void Player::setX(double x) {
    x_ = x;
}

double Player::getX() {
    return x_;
}

void Player::setY(double y) {
    y_ = y;
}

double Player::getY() {
    return y_;
}

void Player::setPosition(double x, double y) {
    setX(x);
    setY(y);
}

void Player::setMovingUp(bool moving_up) {
    moving_up_ = moving_up;
}

bool Player::isMovingUp() {
    return moving_up_;
}

void Player::setMovingDown(bool moving_down) {
    moving_down_ = moving_down;
}

bool Player::isMovingDown() {
    return moving_down_;
}

void Player::setAlive(bool alive) {
    alive_ = alive;
}

bool Player::isAlive() {
    return alive_;
}

void Player::drawPlayer() {
    glPushMatrix();
    
    glTranslated(x_, y_, 10);
    glScaled(10,10,20);
    glutSolidCube(1);

    glPopMatrix();
}

void Player::updatePlayerAnimation(double elapsed_time) {
    x_ += x_speed_ * elapsed_time;

    if(moving_up_ && !moving_down_) {
        y_ += y_speed_ * elapsed_time;
    }
    else if(moving_down_) {
        y_ -= y_speed_ * elapsed_time; 
    }

}