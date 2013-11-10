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

    n_deaths_ = 0;

    width_ = 3;//10;
    height_ = 4.8;//10;

    left_arm_.instantiate(Vector3(0,2,6.8), 20, 0, true);
    left_leg_.instantiate(Vector3(0,2,3.5), 0, 0, false);
    right_arm_.instantiate(Vector3(0,-2,6.8), -20, 0, false);
    right_leg_.instantiate(Vector3(0,-2,3.5), 0, 0, true);

}


Player::~Player() {

}

void Player::loadPlayerModels(std::string body, std::string arm, std::string leg) {
    load(body);
    left_arm_.load(arm);
    right_arm_.load(arm);
    left_leg_.load(leg);
    right_leg_.load(leg);
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

    glTranslated(position_.x, position_.y, 0);
    //glScaled(10,10,20);
    //glutSolidCube(1);
    glScaled(4,4,4);
    //left_arm_.draw();
    //left_leg_.draw();
    //right_arm_.draw();
    //right_leg_.draw();
    render();

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

void Player::update(double elapsed_time) {
    /*updatePlayerAnimation(elapsed_time);
    left_arm_.update(elapsed_time);
    left_leg_.update(elapsed_time);
    right_arm_.update(elapsed_time);
    right_leg_.update(elapsed_time);*/
}

void Player::respawn(double x, double y) {
    position_.x = x;
    position_.y = y;
    moving_up_ = moving_down_ = false;
    alive_ = true;
}

CollisionBox Player::getCollisionBox() {
    double left = position_.x - 5;
    double right = left + width_;
    double top = position_.y + 5;
    double bottom = top - height_;

    return CollisionBox(left, right, top, bottom);
}

void Player::setNDeaths(unsigned int n_deaths) {
    n_deaths_ = n_deaths;
}

unsigned int Player::getNDeaths() {
    return n_deaths_;
}

void Player::incNDeaths() {
    n_deaths_++;
}

void Player::kill() {
    setAlive(false);
    incNDeaths();
}