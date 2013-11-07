#pragma once

#include "Object.h"

class Board
{
private:
    double width_;
    double height_;

    bool visible_;
public:
    Board(void);
    ~Board(void);

    void setDimensions(double width, double height);
    double getWidth();
    double getHeight();

    void setVisible(bool);
    bool isVisible();

    bool isOnBoard(double x, double y);
    bool isOnBoard(Object* o);
};

