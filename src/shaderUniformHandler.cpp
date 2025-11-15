#include "shaderUniformHandler.h"

Shader::Shader(GLuint programID) : programID(programID) {}

Shader::~Shader() {
        glDeleteProgram(programID);
}

void Shader::use() {
    glUseProgram(programID);
}

GLuint Shader::getID() const {
    return programID;
}

void Shader::setFloat(const std::string& name, float value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1f(loc, value);
}

void Shader::setInt(const std::string& name, int value) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    glUniform1i(loc, value);
}

void Shader::setTexture(const std::string& name, GLuint textureID, int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    setInt(name, unit);
}