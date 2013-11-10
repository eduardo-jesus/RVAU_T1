#ifndef _OBJECT_
#define _OBJECT_

#include <vector>
#include <map>

#include "Material.hpp"
#include "Math3D.hpp"
#include "Pattern.h"
#include "CollisionBox.h"

class Object {
public:
    Object();
    ~Object();
    
    void load(std::string filename);
    void toVBOs();
    void render();

    void addMaterial(std::string name, Material material);
    void addMaterial(Material material);
    Material getMaterial(std::string name);
    void applyMaterial(std::string name);

    virtual void draw() = 0;
    virtual void update(double elapsed_time);
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
    bool isCollidingWith(CollisionBox box);

    void setVisible(bool visible);
    bool isVisible();
protected:
    std::map<std::string, Material> materials_;
    std::string last_used_material_;
    
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