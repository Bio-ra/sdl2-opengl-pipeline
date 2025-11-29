#pragma once

#include <GL/glew.h>

// Global OpenGL objects for the quad
extern GLuint quadVAO;
extern GLuint quadVBO;
extern GLuint quadEBO;

// Function declarations
void CreateQuadGeometry();
void UpdateQuadBuffer(const float* vertices, int quadCount);
void DrawQuad();
void DrawQuads(int quadCount);
void DeleteQuadGeometry();