#include "shaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readShaderSource(const std::string& filePath) {
    
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();

    return shaderStream.str();
}

GLuint compileShader(GLenum shaderType, const std::string& source) {
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = readShaderSource(vertexPath);
    if (vertexCode.empty()) {
        std::cerr << "Vertex shader is empty or failed to load: " << vertexPath << std::endl;
        return 0;
    }
    std::string fragmentCode = readShaderSource(fragmentPath);
    if (fragmentCode.empty()) {
        std::cerr << "Fragment shader is empty or failed to load: " << fragmentPath << std::endl;
        return 0;
    }
    
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    if (vertexShader == 0) {
        std::cerr << "Failed to compile vertex shader: " << vertexPath << std::endl;
        return 0;
    }
    
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
    if (fragmentShader == 0) {
        std::cerr << "Failed to compile fragment shader: " << fragmentPath << std::endl;
        glDeleteShader(vertexShader);
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}