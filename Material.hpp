#ifndef _MATERIAL_
#define _MATERIAL_

#include <string>

#include <gl/glew.h>

struct Texture {
    GLuint id;
    unsigned char* image;
    Texture();
    Texture(std::string filename);
};

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float transmittance[4];
    float emission[4];
    float shininess[1];
    
    Texture texture;
    bool has_texture;
    
    Material();
};

#endif /* _MATERIAL_ */

