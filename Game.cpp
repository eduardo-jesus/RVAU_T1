#include "Game.h"

#include <cstdio>

#include <GL/glut.h>
#include "AR\video.h"
#include "AR\gsub.h"

Game::Game(void) {
    thresh_ = 100;
    anim_millis_ = 30;
    previous_clock_ = clock();
    board_ = Board();
    player_ = Player();
}

Game::~Game(void) {
}

void Game::init() {
    char *vconf = "Data\\WDM_camera_flipV.xml";
    int xsize, ysize;

    char *cparam_name = "Data/camera_para.dat";
    //char *cparam_name = "Data/camera_para_busto.dat";
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

    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    //GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0}; // Must be defined when drawing the objects
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Must be defined when drawing the objects
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
}

void Game::loadPatterns() {
    Pattern left_top_pattern = Pattern("Data/patt.left_top_corner", 40.0);
    LEFT_TOP_CORNER = Pattern::loadPattern(left_top_pattern);
    patterns_[LEFT_TOP_CORNER] = left_top_pattern;

    Pattern right_bottom_pattern = Pattern("Data/patt.right_bottom_corner", 40.0);
    RIGHT_BOTTOM_CORNER = Pattern::loadPattern(right_bottom_pattern);
    patterns_[RIGHT_BOTTOM_CORNER] = right_bottom_pattern;

    Pattern spawn_pattern = Pattern("Data/patt.spawn", 40.0);
    SPAWN = Pattern::loadPattern(spawn_pattern);
    patterns_[SPAWN] = spawn_pattern;

    Pattern cannon_pattern = Pattern("Data/patt.cannon", 40.0);
    CANNON = Pattern::loadPattern(cannon_pattern);
    patterns_[CANNON] = cannon_pattern;

    cannon_.setPattern(&patterns_[CANNON]);

    Pattern rotate_cannon_pattern = Pattern("Data/patt.cannon_rotate", 40.0);
    ROTATE_CANNON = Pattern::loadPattern(rotate_cannon_pattern);
    patterns_[ROTATE_CANNON] = rotate_cannon_pattern;

    Pattern spikes_pattern = Pattern("Data/patt.spikes", 40.0);
    SPIKES = Pattern::loadPattern(spikes_pattern);
    patterns_[SPIKES] = spikes_pattern;

    Pattern hole_pattern = Pattern("Data/patt.hole", 40.0);
    HOLE = Pattern::loadPattern(hole_pattern);
    patterns_[HOLE] = hole_pattern;

    Pattern up_pattern = Pattern("Data/patt.up", 40.0);
    UP = Pattern::loadPattern(up_pattern);
    patterns_[UP] = up_pattern;

    Pattern down_pattern = Pattern("Data/patt.down", 40.0);
    DOWN = Pattern::loadPattern(down_pattern);
    patterns_[DOWN] = down_pattern;
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

    if (arDetectMarker(data_ptr_, thresh_, &marker_info_, &marker_num) < 0) {
        printf("Error on arDetectMarkerLite");
        cleanup();
        getchar();
        exit(0);
    }

    std::map<int, bool> detected;
    for(std::map<int, Pattern>::iterator it = patterns_.begin(); it != patterns_.end(); ++it) {
        detected[it->first] = false;
    }

    //printf("detected %d markers\n", marker_num);

    for(int i = 0; i < marker_num; ++i) {
        int id = marker_info_[i].id;

        if(patterns_.find(id) != patterns_.end()) {
            detected[id] = true;
            
            Pattern& pattern = patterns_[id];

            pattern.setVisible(true);
            visible_patterns_.push_back(&pattern);
            pattern.setInfo(marker_info_[i]);
            //printf("[Game::detectMarkers] pattern %s detected\n", pattern.getName().c_str());
        }
    }

    for(std::map<int, bool>::iterator it = detected.begin(); it != detected.end(); ++it) {
        if(!it->second) {
            patterns_[it->first].setContinuous(false);
        }
    }

    if (updateBoard()) {
        updateCannon();
        updatePlayer();
        updateTraps();
        updateControls();
    }

    return marker_num;
}

void Game::resetVisiblePatterns() {
    for(unsigned int i = 0; i < visible_patterns_.size(); ++i) {
        visible_patterns_[i]->changeVisibility();
    }

    visible_patterns_.clear();
}

bool Game::updateBoard() {
    if(patterns_[LEFT_TOP_CORNER].isVisible()) {
        board_.setVisible(true);

        if(patterns_[RIGHT_BOTTOM_CORNER].isVisible()) {
            Vector3 dist = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[RIGHT_BOTTOM_CORNER]);
            board_.setDimensions(dist.x, dist.y);
        }
    } else {
        board_.setVisible(false);
    }

    return board_.isVisible();
}

void Game::updateCannon() {
    if(patterns_[CANNON].isVisible()) {

        if(patterns_[ROTATE_CANNON].isVisible()) {
            double angle = Pattern::angle(patterns_[CANNON], patterns_[ROTATE_CANNON]);
            cannon_.setAngle(angle);

            if(!cannon_.isShooting()) {
                cannon_.setCanShoot(true);
            }
        } else if(cannon_.canShoot()) {
            //cannon_.setShooting(true);
            cannon_.setCanShoot(false);
            cannon_.shoot();

            Vector3 distance = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[CANNON]);
            bullet_ = Bullet(distance.x, distance.y, cannon_.getAngle());
        }
    }
}

void Game::updatePlayer() {
    if (!patterns_[LEFT_TOP_CORNER].isVisible()) {
        return;
    }

    if(patterns_[SPAWN].isVisible() && !player_.isAlive()) {
        Vector3 dist = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[SPAWN]);
        player_ = Player(dist.x, dist.y);
    }

    if (player_.isAlive()) {
        player_.setMovingUp(patterns_[UP].isVisible());
        player_.setMovingDown(patterns_[DOWN].isVisible());
    }
}

void Game::updateTraps() {
    if (patterns_[HOLE].isVisible()) {

    }

    if (patterns_[SPIKES].isVisible()) {

    }
}

void Game::updateControls() {

}

void Game::mainLoop() {
    if(!grabVideoFrame()) {
        arUtilSleep(2);
        return;
    }

    int marker_num = detectMarkers();

    argDrawMode2D();
    argDispImage(data_ptr_, 0, 0);

    arVideoCapNext();

    drawScene();

    resetVisiblePatterns();

    argSwapBuffers();
}

void Game::drawScene() {
    argDrawMode3D();
    argDraw3dCamera(0, 0);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    Pattern& cannon = patterns_[CANNON];
    Pattern& left_top = patterns_[LEFT_TOP_CORNER];
    Pattern& right_bottom = patterns_[RIGHT_BOTTOM_CORNER];

    if(cannon.isVisible()) {
        cannon_.draw();
    }

    if(board_.isVisible()) {
        double gl_param[16];
        argConvGlpara(left_top.getTrans(), gl_param);
        drawRect(board_.getWidth(), board_.getHeight(), gl_param);

        /*if(patterns_[SPAWN].isVisible()) {
            Vector3 dist = Pattern::distance(left_top, patterns_[SPAWN]);
            Player p = Player(dist.x, dist.y);
            p.drawPlayer();
        }*/

        if(player_.isAlive()) {
            player_.draw();
        }

        if(bullet_.isMoving()) {
            bullet_.draw();
        }
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void Game::updateAnimations() {
    //printf("anim\n");
    clock_t current_clock = clock();
    double elapsed_time = (current_clock - previous_clock_) / (double) CLOCKS_PER_SEC;

    if(player_.isAlive()) {
        if(!board_.isOnBoard(&player_)) {
            player_.setAlive(false);
        }
        else {
            player_.updatePlayerAnimation(elapsed_time);
        }
    }

    if(bullet_.isMoving()) {
        bullet_.updateBulletPosition(elapsed_time);
    }
    
    previous_clock_ = current_clock;
}

void Game::drawRect(double x, double y, double gl_para[16]) {
    glLoadMatrixd( gl_para );

    GLfloat light_position[]  = {100.0,-200.0,200.0,0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glVertex3d(0,0,0);
    glVertex3d(0,y,0);
    glVertex3d(x,y,0);
    glVertex3d(x,0,0);
    glEnd();
}

/*
void Game::drawCone(double matrix[16], double angle) {
    glLoadMatrixd( matrix );

    GLfloat light_position[]  = {100.0,-200.0,200.0,0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPushMatrix();
    glRotated(angle, 0,0,1);
    glRotated(90.0,0,1,0);
    glutSolidCone(20, 200, 20, 20);
    glPopMatrix();

    if(bullet_.isMoving()) {
        bullet_.draw();
    }
}
*/
