#pragma once

#include <string>

#include "Object.h"
#include "Player.h"
#include "Wall.h"
#include "Tower.h"
#include "Fortress.h"

class Board: public Object {
private:
    //double width_;
    //double height_;

    bool visible_;

    Fortress fortress_;
    Wall wall_;
    Tower tower_;

#define MIN_WIDTH  200
#define MIN_HEIGHT 100

public:
    Board(void);
    ~Board(void);

    void setDimensions(double right_bottom_x, double right_bottom_y);
    /*double getWidth();
    double getHeight();

    void setVisible(bool);
    bool isVisible();*/

    bool isOnBoard(double x, double y);
    bool isOnBoard(Object* o);
    bool hasPlayerFinished(Player player);

    void loadBoardModels(std::string tower_model, std::string wall_model, std::string fortress_model);

    void draw();
    void drawWalls();
    void drawTowers();
};

