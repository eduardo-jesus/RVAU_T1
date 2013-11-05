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

int main(int argc, char **argv) {
    printf("New main.cpp");
    glutInit(&argc, argv);
    game.init();

    arVideoCapStart();
    argMainLoop(NULL, keyEvent, mainLoop);
    return (0);
}