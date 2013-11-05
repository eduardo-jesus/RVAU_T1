#pragma once

#include <map>

#include "Pattern.h"

class Game
{
private:
    std::map<int, Pattern> patterns_;

    // patterns ids
    int LEFT_TOP_CORNER;
    int RIGHT_BOTTOM_CORNER;
    int SPAWN;
    int CANNON;
    int ROTATE_CANNON;

    //ARToolkit needed variables
    ARUint8 *data_ptr_;
    ARMarkerInfo *marker_info_;
    int thresh_;

public:
    Game(void);
    ~Game(void);
    
    //ARToolkit related functions
    void init();
    void loadPatterns();
    bool grabVideoFrame();
    void cleanup();
    int detectMarkers();

    void mainLoop();


    void drawScene();
    //para remover depois
    void drawRect(double x, double y, double gl_para[16]);
    void drawCone(double matrix[16], double angle);
    
};

