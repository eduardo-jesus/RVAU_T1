#pragma once

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

Game game;

void keyEvent(unsigned char key, int x, int y) {
}

void mainLoop(void) {
   game.mainLoop();
}

void updateAnimations(int dummy) {
    game.updateAnimations();
    glutTimerFunc(30, updateAnimations, 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    game.init();
    
    arVideoCapStart();
    glutTimerFunc(30, updateAnimations,0);
    argMainLoop(NULL, keyEvent, mainLoop);
    return (0);
}