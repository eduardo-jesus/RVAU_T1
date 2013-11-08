#ifndef _OBJECT_
#define _OBJECT_

#include <vector>

#include "Vector2.hpp"
#include "Vector3.h"
#include "Pattern.h"
#include "CollisionBox.h"

struct Triangle {
    Vector3 vertices[3];
    Vector3 normals[3];
    Vector2 uvws[3];
};

class Object {
public:
    Object();
    ~Object();
    
    void load(std::string filename);
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

    Vector3 position_;
    Vector3 scale_;
    //Matrix transform_;

    double width_;
    double height_;

    bool visible_;

    Pattern *pattern_;
};

#endif