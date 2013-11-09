#include <stdio.h>
#include <stdlib.h>

#include <gl/glew.h>
#include <gl/glut.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/arMulti.h>

#include "Game.h"

int FRAME_RATE = 60;
double TICK_RATE = 1.0 / FRAME_RATE;
int DELTA_T = 1000.0 / FRAME_RATE;


Game game;

void mainLoop() {
   game.mainLoop();
}

void updateAnimations(int dummy) {
    game.updateAnimations();
    glutTimerFunc(DELTA_T, updateAnimations, 0);
}

void initialize(int argc, char **argv) {
    glutInit(&argc, argv);

}

int main(int argc, char **argv) {
    initialize(argc, argv);

    game.init();
    
    arVideoCapStart();
    glutTimerFunc(DELTA_T, updateAnimations, 0);
    argMainLoop(NULL, NULL, mainLoop);
    
    return 0;
}