#include "textureHandler.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path)
{
    stbi_set_flip_vertically_on_load(0);

    // int width = 0, height = 0, channels = 0;
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    // std::cout<<"image: "<<path<<" w:"<<width<<" h:"<<height<<" c:"<<channels<<std::endl;
    if (data) {
        // GLenum internalFormat = GL_RGB;
        // GLenum format = GL_RGB;
        // if (channels == 4) {
        //     internalFormat = GL_RGBA;
        //     format = GL_RGBA;
        // } else if (channels == 3) {
        //     internalFormat = GL_RGB;
        //     format = GL_RGB;
        // } else if (channels == 1) {
        //     internalFormat = GL_RED;
        //     format = GL_RED;
        // }
        GLenum format = (channels == 4) ? GL_RGBA : (channels == 3) ? GL_RGB : GL_RED;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // glTexImage2D(GL_TEXTURE_2D, 0, (GLint)internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // glGenerateMipmap(GL_TEXTURE_2D);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }

    stbi_image_free(data);
}

Texture::~Texture() {
    // if (id != 0) {
    if (id) {
        glDeleteTextures(1, &id);
    }
}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

GLuint Texture::getID() const {
    return id;
}