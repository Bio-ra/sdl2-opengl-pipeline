#include "quad.h"
#include <iostream>

// Global OpenGL objects for the quad
GLuint quadVAO = 0;
GLuint quadVBO = 0;
GLuint quadEBO = 0;

//#################################################
// Create Quad Geometry (VBO, VAO, EBO)
//#################################################
void CreateQuadGeometry() {
    // Define quad vertices (4 corners)
    // Format: position (x, y, z) + color (r, g, b)
    float vertices[] = {
        // positions         // colors
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top-left (red)
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // top-right (green)
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // bottom-right (blue)
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f   // bottom-left (yellow)
    };
    
    // Define indices for two triangles that make up the quad
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    
    // 1. GENERATE BUFFERS
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);
    
    // 2. BIND VAO FIRST
    glBindVertexArray(quadVAO);
    
    // 3. BIND AND SET VBO (vertex data)
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 4. BIND AND SET EBO (index data)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 5. CONFIGURE VERTEX ATTRIBUTES
    // Position attribute (location = 0 in shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute (location = 1 in shader)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // 6. UNBIND
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    std::cout << "Quad geometry created successfully" << std::endl;
}

//#################################################
// Draw the Quad
//#################################################
void DrawQuad() {
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//#################################################
// Delete Quad Geometry
//#################################################
void DeleteQuadGeometry() {
    if (quadVAO != 0) {
        glDeleteVertexArrays(1, &quadVAO);
        quadVAO = 0;
    }
    if (quadVBO != 0) {
        glDeleteBuffers(1, &quadVBO);
        quadVBO = 0;
    }
    if (quadEBO != 0) {
        glDeleteBuffers(1, &quadEBO);
        quadEBO = 0;
    }
}