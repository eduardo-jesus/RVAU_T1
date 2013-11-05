#include "Game.h"

#include <cstdio>

#include <GL/glut.h>
#include "AR\video.h"
#include "AR\gsub.h"

Game::Game(void)
{
    thresh_ = 100;
}

Game::~Game(void)
{
}

void Game::init() {
    char *vconf = "Data\\WDM_camera_flipV.xml";
    int xsize, ysize;

    char *cparam_name    = "Data/camera_para.dat";
    ARParam cparam;


    ARParam  wparam;

    /* open the video path */
    if (arVideoOpen(vconf) < 0) {
        printf("[Game::init] Error reading video configuration file");
        getchar();
        exit(0);
    }
    /* find the size of the window */
    if (arVideoInqSize(&xsize, &ysize) < 0) {
        printf("[Game::init] Error finding window size");
        getchar();
        exit(0);
    }
    printf("[Game::init] Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if (arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("[Game::init] Camera parameter load error !!\n");
        getchar();
        exit(0);
    }

    arParamChangeSize(&wparam, xsize, ysize, &cparam);
    arInitCparam(&cparam);
    printf("*** Camera Parameter ***\n");
    arParamDisp(&cparam);

    loadPatterns();

    /* open the graphics window */
    //argInit( &cparam, 1.0, 0, 2, 1, 0 );
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
    arFittingMode   = AR_FITTING_TO_IDEAL;
    arImageProcMode = AR_IMAGE_PROC_IN_HALF;
    argDrawMode     = AR_DRAW_BY_TEXTURE_MAPPING;
    argTexmapMode   = AR_DRAW_TEXTURE_HALF_IMAGE;
}

void Game::loadPatterns() {
    Pattern left_top_pattern = Pattern("Data/left_top_corner.pat", 40.0);
    Pattern right_bottom_pattern = Pattern("Data/right_bottom_corner.pat", 40.0);
    Pattern spawn_pattern = Pattern("Data/spawn.pat", 40.0);
    Pattern cannon_pattern = Pattern("Data/cannon.pat", 40.0);
    Pattern rotate_cannon_pattern = Pattern("Data/rotate.pat", 40.0);

    if ((LEFT_TOP_CORNER = arLoadPatt(left_top_pattern.getName().c_str())) < 0) {
        printf("pattern left_top load error !!\n");
        getchar();
        exit(0);
    }
    left_top_pattern.setId(LEFT_TOP_CORNER);
    patterns_[LEFT_TOP_CORNER] = left_top_pattern;

    if ((RIGHT_BOTTOM_CORNER = arLoadPatt(right_bottom_pattern.getName().c_str())) < 0) {
        printf("pattern rigth_bottom load error !!\n");
        getchar();
        exit(0);
    }
    right_bottom_pattern.setId(RIGHT_BOTTOM_CORNER);
    patterns_[RIGHT_BOTTOM_CORNER] = right_bottom_pattern;

    if ((SPAWN = arLoadPatt(spawn_pattern.getName().c_str())) < 0) {
        printf("pattern spawn load error !!\n");
        getchar();
        exit(0);
    }
    spawn_pattern.setId(SPAWN);
    patterns_[SPAWN] = spawn_pattern;

    if ((CANNON = arLoadPatt(cannon_pattern.getName().c_str())) < 0) {
        printf("pattern cannon load error !!\n");
        getchar();
        exit(0);
    }
    cannon_pattern.setId(CANNON);
    patterns_[CANNON] = cannon_pattern;

    if ((ROTATE_CANNON = arLoadPatt(rotate_cannon_pattern.getName().c_str())) < 0) {
        printf("pattern rotate load error !!\n");
        getchar();
        exit(0);
    }
    rotate_cannon_pattern.setId(ROTATE_CANNON);
    patterns_[ROTATE_CANNON] = rotate_cannon_pattern;
}

bool Game::grabVideoFrame() {
    if( (data_ptr_ = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return false;
    }
    return true;
}

void Game::cleanup() {
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

int Game::detectMarkers() {
    int marker_num;

    if (arDetectMarkerLite(data_ptr_, thresh_, &marker_info_, &marker_num) < 0) {
        printf("Error on arDetectMarkerLite");
        cleanup();
        getchar();
        exit(0);
    }

    for(int i = 0; i < marker_num; ++i) {
        int id = marker_info_[i].id;

        if(patterns_.find(id) != patterns_.end()) {
            Pattern& pattern = patterns_[id];

            pattern.setVisible(true);
            pattern.setInfo(marker_info_[i]);
            printf("[Game::detectMarkers] pattern %s detected\n", pattern.getName().c_str());
        }
    }

    return marker_num;
}

void Game::mainLoop() {
    if(!grabVideoFrame()) {
        arUtilSleep(2);
        return;
    }

    int marker_num = detectMarkers();

    argDrawMode2D();
    if (!arDebug) {
        argDispImage(data_ptr_, 0, 0);

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(6.0);
        for (int i = 0; i < marker_num; ++i) {
            argDrawSquare(marker_info_[i].vertex, 0, 0);
        }
        glLineWidth( 1.0 );

    } else {
        argDispImage(data_ptr_, 1, 1);
        if(arImageProcMode == AR_IMAGE_PROC_IN_HALF) {
            argDispHalfImage(arImage, 0, 0);
        } else {
            argDispImage(arImage, 0, 0);
        }

        glColor3f(1.0, 0.0, 0.0);
        glLineWidth(6.0);
        for(int i = 0; i < marker_num; ++i) {
            argDrawSquare(marker_info_[i].vertex, 0, 0);
        }
        glLineWidth( 1.0 );
    }

    arVideoCapNext();

    drawScene();

    argSwapBuffers();
}

void Game::drawScene() {
    argDrawMode3D();
    argDraw3dCamera(0, 0);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    Pattern& cannon = patterns_[CANNON];
    Pattern& rotate = patterns_[ROTATE_CANNON];
    Pattern& left_top = patterns_[LEFT_TOP_CORNER];
    Pattern& right_bottom = patterns_[RIGHT_BOTTOM_CORNER];

    if(cannon.isVisible()) {
        cannon.changeVisibility();
        cannon.setTransMat();

        double angle = 0;

        if(rotate.isVisible()) {
            rotate.changeVisibility();
            rotate.setTransMat();

            angle = Pattern::angle(cannon, rotate);
        }

        double matrix[16];
        argConvGlpara(cannon.getTrans(), matrix);
        drawCone(matrix,angle);
    }

    if(left_top.isVisible() && right_bottom.isVisible()) {
        left_top.changeVisibility();
        left_top.setTransMat();

        right_bottom.changeVisibility();
        right_bottom.setTransMat();

        Vector3 dist = Pattern::distance(left_top, right_bottom);

        double gl_param[16];

        argConvGlpara(left_top.getTrans(), gl_param);
        drawRect(dist.x, dist.y, gl_param);
    }
}

void Game::drawRect(double x, double y, double gl_para[16]) {
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
    glVertex3d(0,y,0);
    glVertex3d(x,y,0);
    glVertex3d(x,0,0);
    glEnd();

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

void Game::drawCone(double matrix[16], double angle) {
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
    glLoadMatrixd( matrix );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);

    glRotated(angle, 0,0,1);
    glRotated(90.0,1,0,0);
    glutSolidCone(20, 200, 20, 20);

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}
