#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>
#include <GL/glew.h>

std::string readShaderSource(const std::string& filePath);
GLuint compileShader(GLenum shaderType, const std::string& source);
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

#endif