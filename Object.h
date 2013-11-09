#ifndef _OBJECT_
#define _OBJECT_

#include <vector>

#include "Math3D.hpp"
#include "Pattern.h"
#include "CollisionBox.h"

class Object {
public:
    Object();
    ~Object();
    
    void load(std::string filename);
    void toVBO();
    void render();

    virtual void draw() = 0;
    void update();
    virtual CollisionBox getCollisionBox();

    void setPosition(Vector3 position);
    Vector3 getPosition();

    double getX();
    void setX(double x);
    double getY();
    void setY(double y);
    double getZ();
    void setZ(double z);

    void setScale(float scale);
    Vector3 getScale();

    void setPattern(Pattern *pattern);
    Pattern *getPattern();

    void setWidth(double width);
    double getWidth();
    void setHeight(double height);
    double getHeight();

    bool isCollidingWith(Object*);

    void setVisible(bool visible);
    bool isVisible();
protected:
    std::vector<Triangle> triangles_;
    std::vector<VBO> vbos_;

    Vector3 position_;
    Vector3 scale_;
    //Matrix transform_;

    double width_;
    double height_;

    bool visible_;

    Pattern *pattern_;
};

#endif