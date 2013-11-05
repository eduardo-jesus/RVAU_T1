#pragma once
class Board
{
private:
    double width_;
    double height_;
public:
    Board(void);
    ~Board(void);

    void setDimensions(double width, double height);
    double getWidth();
    double getHeight();
};

