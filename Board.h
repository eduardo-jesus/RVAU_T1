#pragma once

#include "Object.h"
#include "Player.h"

class Board {
private:
    double width_;
    double height_;

    bool visible_;

#define MIN_WIDTH  200
#define MIN_HEIGHT 100

public:
    Board(void);
    ~Board(void);

    //void setDimensions(double width, double height);
    void setDimensions(double right_bottom_x, double right_bottom_y);
    double getWidth();
    double getHeight();

    void setVisible(bool);
    bool isVisible();

    bool isOnBoard(double x, double y);
    bool isOnBoard(Object* o);
    bool hasPlayerFinished(Player player);
};

