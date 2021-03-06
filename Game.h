#pragma once

#include <map>
#include <vector>
#include <ctime>
#include <string>

#include "Pattern.h"
#include "Board.h"
#include "Cannon.h"
#include "Player.h"
#include "Bullet.h"
#include "Hole.h"
#include "Spikes.h"
#include "RotateButton.h"
#include "Arrow.h"
#include "ResizeMarker.h"
#include "SpawnMarker.h"

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

    int window_width_;
    int window_height_;

    //ARToolkit needed variables
    ARUint8 *data_ptr_;
    ARUint8 *data_ptr_backup_;
    ARMarkerInfo *marker_info_;
    int thresh_;

    Board board_;
    Cannon cannon_;
    Player player_;
    Bullet bullet_;
    Hole hole_;
    Spikes spikes_;
    RotateButton rotate_;
    Arrow up_button_;
    Arrow down_button_;
    ResizeMarker resize_;
    SpawnMarker spawn_;

    //animation
    int anim_millis_;
    clock_t previous_clock_;

    bool finished_;

    Material floor_material_;

#define TEX_UNIT_TO_UNIT 50.0;
public:
    Game(int window_width, int window_height);
    ~Game();

    void init();
    void loadPatterns();
    void loadModels();
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
    
    void drawControls();
    void drawBoard();
    void drawText();
    void writeText(std::string text); 

    void drawRect(double cx, double cy, double width, double height);
    void drawRect(double cx, double cy, double width, double height, double start_u, double start_v);

    void resetVisiblePatterns();

    void setFinished(bool finished);
    bool isFinished();
};

