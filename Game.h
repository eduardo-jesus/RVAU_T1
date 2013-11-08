#pragma once

#include <map>
#include <vector>
#include <ctime>

#include "Pattern.h"
#include "Board.h"
#include "Cannon.h"
#include "Player.h"
#include "Bullet.h"
#include "Hole.h"

class Game {
private:
    std::map<int, Pattern> patterns_;
    std::vector<Pattern*> visible_patterns_;

    // patterns ids
    int LEFT_TOP_CORNER;
    int RIGHT_BOTTOM_CORNER;
    int SPAWN;
    int CANNON;
    int ROTATE_CANNON;
    int SPIKES;
    int HOLE;
    int UP;
    int DOWN;

    //ARToolkit needed variables
    ARUint8 *data_ptr_;
    ARMarkerInfo *marker_info_;
    int thresh_;

    Board board_;
    Cannon cannon_;
    Player player_;
    Bullet bullet_;
    Hole hole_;

    //animation
    int anim_millis_;
    clock_t previous_clock_;
public:
    Game(void);
    ~Game(void);

    //ARToolkit related functions
    void init();
    void loadPatterns();
    bool grabVideoFrame();
    void cleanup();
    int detectMarkers();

    bool updateBoard();
    void updateCannon();
    void updatePlayer();
    void updateTraps();
    void updateControls();

    void mainLoop();

    void drawScene();
    void updateAnimations();
    
    void drawBoard();
    //remove later
    void drawRect(double x, double y, double gl_para[16]);
    void drawRect(double cx, double cy, double width, double height);
    void drawCone(double matrix[16], double angle);

    void resetVisiblePatterns();
};

