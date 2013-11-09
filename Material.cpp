#include "Material.hpp"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

Material::Material() {
    ambient[0] = 0.7f;
    ambient[1] = 0.7f;
    ambient[2] = 0.7f;
    ambient[3] = 1.0f;

    diffuse[0] = 0.9f;
    diffuse[1] = 0.9f;
    diffuse[2] = 0.9f;
    diffuse[3] = 1.0f;

    specular[0] = 0.3f;
    specular[1] = 0.3f;
    specular[2] = 0.3f;
    specular[3] = 1.0f;

    shininess[0] = 0.5f;
    
    has_texture = false;
}

Texture::Texture() {
    id = 0;
}

Texture::Texture(std::string filename) {
    glEnable(GL_TEXTURE_2D);
    id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering
    int width, height, n_channels;
    image = stbi_load(filename.c_str(), &width, &height, &n_channels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, 4/*GL_RGBA*/, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

