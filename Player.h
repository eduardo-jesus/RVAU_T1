#pragma once
class Player
{
private:
    double x_;
    double y_;

    bool moving_up_;
    bool moving_down_;
    bool alive_;
public:
    Player(void);
    Player(double, double);
    ~Player(void);
    void setX(double);
    double getX();
    void setY(double);
    double getY();
    void setPosition(double, double);
    void setMovingUp(bool);
    bool isMovingUp();
    void setMovingDown(bool);
    bool isMovingDown();
    void setAlive(bool);
    bool isAlive();

    void drawPlayer();
};

