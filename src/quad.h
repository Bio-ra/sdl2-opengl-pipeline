#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>

// Global OpenGL objects for the quad
extern GLuint quadVAO;
extern GLuint quadVBO;
extern GLuint quadEBO;

// Function declarations
void CreateQuadGeometry();
void DrawQuad();
void DeleteQuadGeometry();

#endif // QUAD_H