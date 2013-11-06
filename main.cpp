#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/arMulti.h>

#include "Game.h"

Game game = Game();

void keyEvent(unsigned char key, int x, int y) {
}

/* main loop */
void mainLoop(void) {
   game.mainLoop();
}

void updateAnimations(int dummy) {
    game.updateAnimations();
    glutTimerFunc(30, updateAnimations,0);
}

int main(int argc, char **argv) {
    printf("New main.cpp");
    glutInit(&argc, argv);
    game.init();

    arVideoCapStart();
    glutTimerFunc(30, updateAnimations,0);
    argMainLoop(NULL, keyEvent, mainLoop);
    return (0);
}