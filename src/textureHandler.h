#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <GL/glew.h>
#include <string>

class Texture {
private:
    GLuint id;
    
public:
    Texture(const std::string& path);
    ~Texture();
    
    void bind(int unit = 0);
    GLuint getID() const;
};

#endif