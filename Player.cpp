#include "Player.h"

#include <GL/glut.h>

const double Player::X_SPEED = 25;
const double Player::Y_SPEED = 50;

Player::Player(void) {
    position_.x = 0;
    position_.y = 0;

    moving_up_ = false;
    moving_down_ = false;
    alive_ = false;
}

Player::Player(double x, double y) {
    position_.x = x;
    position_.y = y;

    moving_up_ = false;
    moving_down_ = false;
    alive_ = true;

    width_ = 10;
    height_ = 10;
}

Player::~Player(void) {

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

void Player::draw() {
    glPushMatrix();

    glTranslated(position_.x, position_.y, 10);
    glScaled(10,10,20);
    glutSolidCube(1);

    glPopMatrix();
}

void Player::updatePlayerAnimation(double elapsed_time) {
    position_.x += X_SPEED * elapsed_time;

    if (moving_up_ != moving_down_) {
        if(moving_up_) {
            position_.y += Y_SPEED * elapsed_time;
        } else if(moving_down_) {
            position_.y -= Y_SPEED * elapsed_time; 
        }
    }

    moving_up_ = moving_down_ = false;
}

CollisionBox Player::getCollisionBox() {
    double left = position_.x - 5;
    double right = left + width_;
    double top = position_.y + 5;
    double bottom = top - height_;

    return CollisionBox(left, right, top, bottom);
}