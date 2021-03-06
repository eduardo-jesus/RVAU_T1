#include "Game.h"

#include <cstdio>
#include <sstream>

#include <gl/glew.h>
#include <gl/glut.h>
#include "AR\video.h"
#include "AR\gsub.h"

Game::Game(int window_width, int window_height) {
    window_width_ = window_width;
    window_height_ = window_height;
    finished_ = false;
    thresh_ = 100;
    anim_millis_ = 30;
    previous_clock_ = clock();
    hole_ = Hole(0,0,40,40,100);
    spikes_ = Spikes(0,0,40,40);
    down_button_.setDown(true);

    floor_material_.texture = Texture("Data/models/ground.jpg");
    floor_material_.has_texture = true;
}

Game::~Game() {
}

void Game::init() {
    loadPatterns();
    loadModels();
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

void Game::loadModels() {
    std::string base_path = "Data/models/"; 

    bullet_.load(base_path + "bullet.obj");
    cannon_.load(base_path + "cannon.obj");
    spikes_.load(base_path + "spikes.obj");
    hole_.load(base_path + "hole.obj");
    rotate_.load(base_path + "rotate.obj");
    up_button_.load(base_path + "arrow.obj");
    down_button_.load(base_path + "arrow.obj");
    board_.loadBoardModels(base_path + "tower.obj", base_path + "wall.obj", base_path + "fortress.obj");
    player_.loadPlayerModels(base_path + "player_body.obj", base_path + "player_arm.obj", base_path + "player_leg.obj");
    resize_.load(base_path + "resize.obj");
    spawn_.load(base_path + "spawn.obj");

}

bool Game::grabVideoFrame() {
    if ((data_ptr_ = (ARUint8 *)arVideoGetImage()) == NULL) {
        if(data_ptr_backup_ != NULL) {
            data_ptr_ = data_ptr_backup_; 
        } else {
            return false;
        }
    } else {
        data_ptr_backup_ = data_ptr_ ; 
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
    }
    updateControls();

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
        Vector3 distance = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[CANNON]);

        cannon_.setX(distance.x);
        cannon_.setY(distance.y);

        if(board_.isOnBoard(cannon_.getX(), cannon_.getY())) {
            cannon_.setVisible(true);
            if(patterns_[ROTATE_CANNON].isVisible()) {
                double angle = Pattern::angle(patterns_[CANNON], patterns_[ROTATE_CANNON]);
                cannon_.setAngle(angle);

                if(!cannon_.isShooting()) {
                    cannon_.setCanShoot(true);
                }
            } else if(cannon_.canShoot()) {
                cannon_.setShooting(true);
                cannon_.setCanShoot(false);

                bullet_.newInstance(distance.x, distance.y, cannon_.getAngle());
            }
        }
        else {
            cannon_.setVisible(false);
        }
    } else {
        cannon_.setVisible(false);
    }
}

void Game::updatePlayer() {
    if (!patterns_[LEFT_TOP_CORNER].isVisible()) {
        return;
    }

    if(patterns_[SPAWN].isVisible()) {
        Vector3 dist = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[SPAWN]);

        if(board_.isOnBoard(dist.x, dist.y)) {
            spawn_.setVisible(true);
            spawn_.setX(dist.x);
            spawn_.setY(dist.y);
            if(!player_.isAlive()) {
                player_.respawn(dist.x, dist.y);
            }
        } else {
            spawn_.setVisible(false);
        }
    } else {
        spawn_.setVisible(false);
    }

    if (player_.isAlive()) {
        player_.setMovingUp(patterns_[UP].isVisible());
        player_.setMovingDown(patterns_[DOWN].isVisible());
    }
}

void Game::updateTraps() {
    if (patterns_[HOLE].isVisible()) {
        Vector3 dist = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[HOLE]);
        hole_.setX(dist.x);
        hole_.setY(dist.y);
        if(board_.isOnBoard(&hole_)) {
            hole_.setVisible(true);
        }
        else {
            hole_.setVisible(false);
        }
    }
    else {
        hole_.setVisible(false);
    }

    if (patterns_[SPIKES].isVisible()) {
        Vector3 dist = Pattern::distance(patterns_[LEFT_TOP_CORNER], patterns_[SPIKES]);
        spikes_.setX(dist.x);
        spikes_.setY(dist.y);
        if(board_.isOnBoard(&spikes_)) {
            spikes_.setVisible(true);
        }
        else {
            spikes_.setVisible(false);
        }
    }
    else {
        spikes_.setVisible(false);
    }
}

void Game::updateControls() {
    if(patterns_[ROTATE_CANNON].isVisible()) {
        rotate_.setVisible(true);
        argConvGlpara(patterns_[ROTATE_CANNON].getTrans(), rotate_.getTransMatrix());
    } else {
        rotate_.setVisible(false);
    }

    if(patterns_[UP].isVisible()) {
        up_button_.setVisible(true);
        argConvGlpara(patterns_[UP].getTrans(), up_button_.getTransMatrix());
    } else {
        up_button_.setVisible(false);
    }

    if(patterns_[DOWN].isVisible()) {
        down_button_.setVisible(true);
        argConvGlpara(patterns_[DOWN].getTrans(), down_button_.getTransMatrix());
    } else {
        down_button_.setVisible(false);
    }

    if(patterns_[RIGHT_BOTTOM_CORNER].isVisible()) {
        resize_.setVisible(true);
        argConvGlpara(patterns_[RIGHT_BOTTOM_CORNER].getTrans(), resize_.getTransMatrix());
    } else {
        resize_.setVisible(false);
    }
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    if(board_.isVisible()) {
        double gl_param[16];
        argConvGlpara(patterns_[LEFT_TOP_CORNER].getTrans(), gl_param);

        GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
        GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
        GLfloat   mat_flash_shiny[] = {50.0};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

        glLoadMatrixd( gl_param );

        GLfloat light_position[]  = {100.0,-200.0,200.0,0.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        drawBoard();

        if(cannon_.isVisible()) {
            cannon_.draw();

            if(player_.isAlive() && cannon_.isCollidingWith(&player_)) {
                player_.kill();
            }
        }

        if(spawn_.isVisible()) {
            spawn_.draw();
        }

        if(player_.isAlive() ) {
            player_.draw();
        }

        if(bullet_.isMoving()) {
            bullet_.draw();
        }

        if(spikes_.isVisible()) {
            spikes_.draw();

            if(player_.isAlive()) {
                if(spikes_.isCollidingWith(&player_)) {
                    player_.kill();
                }
            }
        }

        if(bullet_.isMoving() && player_.isAlive()) {
            if(bullet_.isCollidingWith(&player_)) {
                player_.kill();
                bullet_.setMoving(false);
                cannon_.setShooting(false);
            }
        }

        if(hole_.isVisible() && player_.isAlive()) {
            if(hole_.isCollidingWith(&player_)) {
                player_.kill();
            }
        }
    }

    drawControls();
    drawText();

    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
}

void Game::drawControls() {

    if(rotate_.isVisible()) {
        rotate_.draw();
    }

    if(up_button_.isVisible()) {
        up_button_.draw();
    }

    if(down_button_.isVisible()) {
        down_button_.draw();
    }

    if(resize_.isVisible()) {
        resize_.draw();
    }
}

void Game::updateAnimations() {
    clock_t current_clock = clock();
    double elapsed_time = (current_clock - previous_clock_) / (double) CLOCKS_PER_SEC;

    if(player_.isAlive()) {
        if(!board_.isOnBoard(&player_)) {
            if(board_.hasPlayerFinished(player_)) {
                setFinished(true);
                player_.setAlive(false);
            }
            else {
                player_.kill();
            }
        }
        else {
            player_.update(elapsed_time);
        }
    }

    if(bullet_.isMoving()) {
        if(!board_.isOnBoard(&bullet_)) {
            bullet_.setMoving(false);
            cannon_.setShooting(false);
        }
        else {
            bullet_.updateBulletPosition(elapsed_time);
        }
    }

    previous_clock_ = current_clock;
}

void Game::drawRect(double cx, double cy, double width, double height) {
    glPushMatrix();

    glTranslated(cx, cy, 0);

    glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glVertex3d(width/2.0,height/2.0,0);
    glVertex3d(-width/2.0,height/2.0,0);
    glVertex3d(-width/2.0,-height/2.0,0);
    glVertex3d(width/2.0,-height/2.0,0);
    glEnd();
    glPopMatrix();
}

void Game::drawRect(double cx, double cy, double width, double height, double start_u, double start_v) {
    double end_u = start_u + width / TEX_UNIT_TO_UNIT;
    double end_v = start_v + height / TEX_UNIT_TO_UNIT;

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floor_material_.texture.id);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floor_material_.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floor_material_.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floor_material_.specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, floor_material_.shininess);

    glTranslated(cx, cy, 0);
    glBegin(GL_POLYGON);
    glNormal3d(0,0,1);
    glTexCoord2d(end_u, end_v);
    glVertex3d(width/2.0,height/2.0,0);
    glTexCoord2d(start_u, end_v);
    glVertex3d(-width/2.0,height/2.0,0);
    glTexCoord2d(start_u, start_v);
    glVertex3d(-width/2.0,-height/2.0,0);
    glTexCoord2d(end_u, start_v);
    glVertex3d(width/2.0,-height/2.0,0);
    glEnd();
    glPopMatrix();
}

void Game::drawBoard() {
    board_.draw();

    double width = board_.getWidth();
    double height = board_.getHeight();
    if(!hole_.isVisible()) {
        drawRect(width/2, -height/2, width, height,0,0);
    } else {
        double hole_width = hole_.getWidth();
        double hole_height = hole_.getHeight();

        double width_top_bottom = width;
        double width_left = hole_.getPosition().x - hole_width/2.0;
        double width_right = width - width_left - hole_width;
        double heigth_top = -hole_.getPosition().y - hole_height/2.0;
        double height_middle = hole_height;
        double height_down = height - height_middle -heigth_top;

        double top_cx = width/2.0;
        double top_cy = - heigth_top / 2.0;
        double bottom_cx = top_cx;
        double bottom_cy = -(heigth_top + height_middle + height_down/2.0) ;
        double left_cx = width_left /2.0;
        double left_cy = -(heigth_top + height_middle/2.0);
        double right_cx = width_left + hole_width + width_right/2.0;
        double right_cy = left_cy;

        double sides_v = height_down / TEX_UNIT_TO_UNIT;
        double right_u = (width_left + hole_width) / TEX_UNIT_TO_UNIT;
        double top_v = (height_down + height_middle) / TEX_UNIT_TO_UNIT;

        //top rect
        drawRect(top_cx, top_cy, width_top_bottom, heigth_top, 0, top_v);
        //bottom rect
        drawRect(bottom_cx, bottom_cy, width_top_bottom, height_down, 0, 0);
        //left rect
        drawRect(left_cx, left_cy, width_left, height_middle, 0, sides_v);
        //right rect
        drawRect(right_cx, right_cy, width_right, height_middle, right_u, sides_v);

        hole_.draw();
    }
}

void Game::writeText(std::string text) {
    for(size_t i = 0; i < text.size(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void Game::drawText(){
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,window_width_,0,window_height_);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0,1.0,1.0);
    glRasterPos2f(10,window_height_-30);

    std::stringstream ss;
    ss << "Number of Deaths: " << player_.getNDeaths();
    writeText(ss.str());

    if(isFinished()) {
        char *finished_message = "FINISH";
        unsigned char* in = reinterpret_cast<unsigned char*>(finished_message);
        int message_length = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, in);

        // 18pt -> 24 px
        glRasterPos2f(window_width_/2 - message_length / 2, window_height_ / 2 + 12 );

        ss.str("");
        ss << finished_message;

        writeText(ss.str());
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void Game::setFinished(bool finished) {
    finished_ = finished;
}

bool Game::isFinished() {
    return finished_;
}