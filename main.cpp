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

Game *game;

void mainLoop() {
   game->mainLoop();
}

void updateAnimations(int dummy) {
    game->updateAnimations();
    glutTimerFunc(DELTA_T, updateAnimations, 0);
}

void initialize(int argc, char **argv, int *window_width, int *window_height) {
    glutInit(&argc, argv);

    char *vconf = "Data\\WDM_camera_flipV.xml";
    char *cparam_name = "Data/camera_para.dat";

    ARParam cparam;
    ARParam  wparam;

    /* open the video path */
    if (arVideoOpen(vconf) < 0) {
        printf("[Game::init] Error reading video configuration file");
        getchar();
        exit(-1);
    }
    
    /* find the size of the window */
    if (arVideoInqSize(window_width, window_height) < 0) {
        printf("[Game::init] Error finding window size");
        getchar();
        exit(-1);
    }

    printf("[Game::init] Image size (x,y) = (%d,%d)\n", *window_width, *window_height);

    /* set the initial camera parameters */
    if (arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("[Game::init] Camera parameter load error !!\n");
        getchar();
        exit(0);
    }

    arParamChangeSize(&wparam, *window_width, *window_height, &cparam);
    arInitCparam(&cparam);
    printf("*** Camera Parameter ***\n");
    arParamDisp(&cparam);

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
    arFittingMode   = AR_FITTING_TO_IDEAL;
    arImageProcMode = AR_IMAGE_PROC_IN_HALF;
    argDrawMode     = AR_DRAW_BY_TEXTURE_MAPPING;
    argTexmapMode   = AR_DRAW_TEXTURE_HALF_IMAGE;

    GLenum res = glewInit();
    if (GLEW_OK != res)    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
    }

    // Establish array contains vertices, normals and texture coordinates
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};

    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

int main(int argc, char **argv) {
    int window_width, window_height;

    initialize(argc, argv, &window_width, &window_height);

    game = new Game(window_width, window_height);

    game->init();
    
    arVideoCapStart();
    glutTimerFunc(DELTA_T, updateAnimations, 0);
    argMainLoop(NULL, NULL, mainLoop);
    
    delete game;

    return 0;
}