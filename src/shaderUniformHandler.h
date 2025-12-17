#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H

#include <GL/glew.h>
#include <string>

class Shader {
private:
    GLuint programID;

public:
    Shader(GLuint programID);
    ~Shader();
    
    void use();
    GLuint getID() const;
    
    // colors, positions
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    
    // texture binding
    void setTexture(const std::string& name, GLuint textureID, int unit);
};

#endif