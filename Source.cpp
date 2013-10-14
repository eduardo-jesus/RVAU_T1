#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/arMulti.h>

/* set up the video format globals */

#ifdef _WIN32
char *vconf = "Data\\WDM_camera_flipV.xml";
#else
char *vconf = "";
#endif

int xsize, ysize;
int thresh = 100;
int count = 0;

char *cparam_name    = "Data/camera_para.dat";
ARParam cparam;

char *config_name = "Data/multi/marker.dat";
ARMultiMarkerInfoT *config;

char           *patt_name      = "Data/patt.hiro";
int             patt_id;
double          patt_width     = 80.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

//A path
char *a_name = "Data/multi/patt.a";
int a_id;
double a_width = 49.0;
double a_center[2] = {0.0, 0.0};
double a_trans[3][4];

//F path
char *f_name = "Data/multi/patt.f";
int f_id;
double f_width = 49.0;
double f_center[2] = {0.0, 0.0};
double f_trans[3][4];

static void init(void);
static void cleanup(void);
static void keyEvent(unsigned char key, int x, int y);
static void mainLoop(void);
static void draw(double trans1[3][4], double trans2[3][4], int mode);
static void draw(int patt = 0);
static void drawRect(double x, double y, double z, double gl_para[16]);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    init();

    arVideoCapStart();
    argMainLoop(NULL, keyEvent, mainLoop);
    return (0);
}

static void keyEvent(unsigned char key, int x, int y) {
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }

    if( key == 't' ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        printf("Enter new threshold value (current = %d): ", thresh);
        scanf("%d",&thresh); while( getchar()!='\n' );
        printf("\n");
        count = 0;
    }

    /* turn on and off the debug mode with right mouse */
    if( key == 'd' ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        arDebug = 1 - arDebug;
        if( arDebug == 0 ) {
            glClearColor( 0.0, 0.0, 0.0, 0.0 );
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
        }
        count = 0;
    }
}

/* main loop */
static void mainLoop(void) {
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    double          err;
    int             i;

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if (count == 0) {
        arUtilTimerReset();
    }
    count++;

    /* detect the markers in the video frame */
    if (arDetectMarkerLite(dataPtr, thresh, &marker_info, &marker_num) < 0) {
        cleanup();
        exit(0);
    }
    /*if (marker_num == 0) {
    argDispImage(dataPtr, 0, 0);
    arVideoCapNext();
    argSwapBuffers();
    return;
    }*/

    argDrawMode2D();
    if (!arDebug) {
        argDispImage(dataPtr, 0, 0);

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(6.0);
        for (i = 0; i < marker_num; ++i) {
            argDrawSquare(marker_info[i].vertex, 0, 0);
        }
        glLineWidth( 1.0 );

    } else {
        argDispImage(dataPtr, 1, 1);
        if(arImageProcMode == AR_IMAGE_PROC_IN_HALF) {
            argDispHalfImage(arImage, 0, 0);
        } else {
            argDispImage(arImage, 0, 0);
        }

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(6.0);
        for( i = 0; i < marker_num; ++i) {
            argDrawSquare(marker_info[i].vertex, 0, 0);
        }
        glLineWidth( 1.0 );
    }

    arVideoCapNext();


    argDrawMode3D();
    argDraw3dCamera(0, 0);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    /*if ((err = arMultiGetTransMat(marker_info, marker_num, config)) >= 0 && err <= 100.0) {
    for(i = 0; i < config->marker_num; ++i) {
    if (config->marker[i].visible >= 0) {
    draw(config->trans, config->marker[i].trans, 0);
    } else {
    draw(config->trans, config->marker[i].trans, 1);
    }
    }
    }*/

    int a_index = -1;
    int f_index = -1;

    for (int j = 0; j < marker_num; ++j) {
        /*if (patt_id == marker_info[j].id) {
            arGetTransMat(&marker_info[j], patt_center, patt_width, patt_trans);
            draw();
        } else */if(a_id == marker_info[j].id){
            a_index = j;
            arGetTransMat(&marker_info[j], a_center, a_width, patt_trans);
            draw();
        } else if(f_id == marker_info[j].id){
            f_index = j;
            arGetTransMat(&marker_info[j], f_center, f_width, patt_trans);
            draw(1);
        } else {
            //printf("wrong marker: %d\n", marker_info[j].id);
        }
    }

    if(a_index != -1 && f_index != -1){
        arGetTransMat(&marker_info[a_index], a_center, a_width, a_trans);
        arGetTransMat(&marker_info[f_index], f_center, f_width, f_trans);

        double wmat1[3][4], wmat2[3][4];

        arUtilMatInv(a_trans, wmat1);
        arUtilMatMul(wmat1, f_trans, wmat2);

        double af_dist[3];
        af_dist[0] = wmat2[0][3];
        af_dist[1] = wmat2[1][3];
        af_dist[2] = wmat2[2][3];

        printf("distance A-F: %f %f %f\n",af_dist[0], af_dist[1], af_dist[2]);

        double gl_param[16];
        argConvGlpara(a_trans, gl_param);
        drawRect(af_dist[0], af_dist[1], af_dist[2], gl_param);
    }

    argSwapBuffers();
}

static void init(void) {
    ARParam  wparam;

    /* open the video path */
    if (arVideoOpen(vconf) < 0) {
        exit(0);
    }
    /* find the size of the window */
    if (arVideoInqSize(&xsize, &ysize) < 0) {
        exit(0);
    }
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if (arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize(&wparam, xsize, ysize, &cparam);
    arInitCparam(&cparam);
    printf("*** Camera Parameter ***\n");
    arParamDisp(&cparam);

    /* if ((config = arMultiReadConfigFile(config_name)) == NULL) {
    printf("config data load error !!\n");
    exit(0);
    }*/

    if ((patt_id = arLoadPatt(patt_name)) < 0) {
        printf("pattern load error !!\n");
        exit(0);
    }

    if ((a_id = arLoadPatt(a_name)) < 0) {
        printf("pattern a load error !!\n");
        exit(0);
    }

    if ((f_id = arLoadPatt(f_name)) < 0) {
        printf("pattern f load error !!\n");
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 2, 1, 0 );
    arFittingMode   = AR_FITTING_TO_IDEAL;
    arImageProcMode = AR_IMAGE_PROC_IN_HALF;
    argDrawMode     = AR_DRAW_BY_TEXTURE_MAPPING;
    argTexmapMode   = AR_DRAW_TEXTURE_HALF_IMAGE;
}

/* cleanup function called when program exits */
static void cleanup(void) {
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static void draw(double trans1[3][4], double trans2[3][4], int mode) {
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_ambient1[]    = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash1[]      = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   mat_flash_shiny1[]= {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* load the camera transformation matrix */
    glMatrixMode(GL_MODELVIEW);
    argConvGlpara(trans1, gl_para);
    glLoadMatrixd( gl_para );
    argConvGlpara(trans2, gl_para);
    glMultMatrixd( gl_para );

    if( mode == 0 ) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    }
    else {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash1);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny1);	
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
    }
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
    if( !arDebug ) glutSolidCube(50.0);
    else          glutWireCube(50.0);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

static void draw(int patt) {
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    if(patt != 0) {
        mat_ambient[1] = 1.0;
        mat_flash[1] = 1.0;
    }

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* load the camera transformation matrix */
    argConvGlpara(patt_trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 24.5 );
    glutSolidCube(49.0);
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

static void drawRect(double x, double y, double z, double gl_para[16]) {
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* load the camera transformation matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glVertex3d(0,0,0);
    glVertex3d(0,y,z);
    glVertex3d(x,y,z);
    glVertex3d(x,0,0);
    glEnd();
    
    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}