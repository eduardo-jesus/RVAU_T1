#include "Object.h"

#include <iostream>
#include <vector>

#include <gl/glew.h>
//#include <gl/glut.h>

#include "tiny_obj_loader.hpp"

Object::Object() {
}

Object::~Object() {
}

void Object::load(std::string filename) {
    std::string dir = "";
    std::string::size_type last_slash = filename.find_last_of("/");
    if (last_slash != std::string::npos) {
        dir = filename.substr(0, last_slash + 1);
    }

    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, filename.c_str(), dir.c_str());
    
    if (!err.empty()) {
        std::cerr << err << std::endl;
        exit(-1);
    }

    std::cout << "# of shapes: " << shapes.size() << std::endl;

    std::vector<Triangle> triangles;

    for (size_t i = 0; i < shapes.size(); ++i) {
        std::vector<float> positions = shapes[i].mesh.positions;
        std::vector<float> normals = shapes[i].mesh.normals;
        std::vector<float> tex_coords = shapes[i].mesh.texcoords;
        std::vector<unsigned int> indices = shapes[i].mesh.indices;

        size_t num_normals = normals.size() / 3;
        size_t num_faces = indices.size() / 3;
        size_t num_tex_coords = tex_coords.size() / 2;

        for (size_t j = 0; j < num_faces; ++j) {
            Triangle triangle;

            for (size_t k = 0; k < 3; ++k) {
                size_t index = indices[j * 3 + k];
                
                if (index < num_normals) {
                    triangle.normals[k].x = normals[index * 3 + 0];
                    triangle.normals[k].y = normals[index * 3 + 1];
                    triangle.normals[k].z = normals[index * 3 + 2];
                } else {
                    triangle.normals[k] = Vector3(0.0, 0.0, 1.0);
                }

                if (index < num_tex_coords) {
                    triangle.uvws[k].x = tex_coords[index * 2 + 0];
                    triangle.uvws[k].y = tex_coords[index * 2 + 1];
                } else {
                    triangle.uvws[k] = Vector2(0.0, 0.0);
                }

                triangle.vertices[k].x = positions[index * 3 + 0];
                triangle.vertices[k].y = positions[index * 3 + 1];
                triangle.vertices[k].z = positions[index * 3 + 2];
            }

            triangles.push_back(triangle);
        }
    }

    triangles_ = triangles;
}

void Object::toVBO() {
    VBO vbo;
    vbo.vertices = 0;
    
    // TODO: add material field
    //vbo.material = triangles_[0].material; 
    //std::string last_material = vbo.material;
    for (size_t i = 0; i < triangles_.size(); ++i) {
        // If current triangle belongs to other shape than previous one,
        // upload current vertex data and generate new VBO.
        /*if (triangles_[i].material != last_material) {
            glGenBuffers(1, &vbo.id);
            glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
            glBufferData(GL_ARRAY_BUFFER, vbo.data.size() * sizeof(double), &vbo.data[0], GL_STATIC_DRAW);
            vbos_.push_back(vbo);

            vbo = VBO();
            vbo.vertices = 0;
            vbo.material = triangles_[i].material;
            last_material = vbo.material;
        }*/
        for (int j = 0; j < 3; ++j) {
            vbo.data.push_back(triangles_[i].vertices[j].x);
            vbo.data.push_back(triangles_[i].vertices[j].y);
            vbo.data.push_back(triangles_[i].vertices[j].z);

            vbo.data.push_back(triangles_[i].normals[j].x);
            vbo.data.push_back(triangles_[i].normals[j].y);
            vbo.data.push_back(triangles_[i].normals[j].z);

            vbo.data.push_back(triangles_[i].uvws[j].x);
            vbo.data.push_back(triangles_[i].uvws[j].y);

            ++vbo.vertices;
        }
    }
    glGenBuffers(1, &vbo.id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
    glBufferData(GL_ARRAY_BUFFER, vbo.data.size() * sizeof(double), &vbo.data[0], GL_STATIC_DRAW);

    vbos_.push_back(vbo);
}

void Object::render() {
    for (size_t i = 0; i < triangles_.size(); ++i) {
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < 3; ++j) {
            glTexCoord2f(triangles_[i].uvws[j].x, triangles_[i].uvws[j].y);
            glNormal3f(triangles_[i].normals[j].x, triangles_[i].normals[j].y, triangles_[i].normals[j].z);
            glVertex3f(triangles_[i].vertices[j].x, triangles_[i].vertices[j].y, triangles_[i].vertices[j].z);
        }
        glEnd();
    }
}


void Object::draw() {

}

void Object::update() {

}

Vector3 Object::getPosition() {
    return position_;
}

double Object::getX() {
    return position_.x;
}

void Object::setX(double x) {
    position_.x = x;
}

double Object::getY() {
    return position_.y;
}

void Object::setY(double y) {
    position_.y = y;
}

double Object::getZ() {
    return position_.z;
}

void Object::setZ(double z) {
    position_.z = z;
}

void Object::setPattern(Pattern *pattern) {
    pattern_ = pattern;
}

Pattern *Object::getPattern() {
    return pattern_;
}

void Object::setWidth(double width) {
    width_ = width;
}

double Object::getWidth() {
    return width_;
}

void Object::setHeight(double height) {
    height_ = height;
}

double Object::getHeight() {
    return height_;
}

CollisionBox Object::getCollisionBox() {
    return CollisionBox();
}

bool Object::isCollidingWith(Object* obj) {
    // http://lazyfoo.net/SDL_tutorials/lesson17/

    CollisionBox a = getCollisionBox();
    CollisionBox b = obj->getCollisionBox();

    //The sides of the rectangles
    /*double left_A, left_B;
    double right_A, right_B;
    double top_A, top_B;
    double bottom_A, bottom_B;

    //Calculate the sides of rect A
    left_A = position_.x;
    right_A = left_A + width_;
    top_A = position_.y;
    bottom_A = top_A - height_;
        
    //Calculate the sides of rect B
    left_B = obj->getX();
    right_B = left_B + obj->getWidth();
    top_B = obj->getY();
    bottom_B = top_B - obj->getHeight;*/

    //If any of the sides from A are outside of B
    if(a.bottom >= b.top)
    {
        return false;
    }
    
    if(a.top <= b.bottom)
    {
        return false;
    }
    
    if(a.right <= b.left)
    {
        return false;
    }
    
    if(a.left >= b.right)
    {
        return false;
    }
   
    //If none of the sides from A are outside B
    return true;
}

void Object::setVisible(bool visible) {
    visible_ = visible;
}

bool Object::isVisible() {
    return visible_;
}