#include "quad.h"
#include <vector>

GLuint quadVAO = 0;
GLuint quadVBO = 0;
GLuint quadEBO = 0;

static const int MAX_QUADS = 1024;
static const int VERTICES_PER_QUAD = 4;
static const int INDICES_PER_QUAD = 6;

// #################################################
// Create Quad Geometry (VBO, VAO, EBO) - dynamic VBO
// #################################################
void CreateQuadGeometry() {
    std::vector<unsigned int> indices;
    indices.reserve(MAX_QUADS * INDICES_PER_QUAD); // simplified
    for (int i = 0; i < MAX_QUADS; ++i) {
        unsigned int base = i * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 3);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_QUADS * VERTICES_PER_QUAD * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

// #################################################
// Update dynamic vertex buffer
// #################################################
void UpdateQuadBuffer(const float* vertices, int quadCount) {
    if (quadCount <= 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, quadCount * VERTICES_PER_QUAD * 4 * sizeof(float), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// #################################################
// Draw the Quad(s)
// #################################################
void DrawQuad() {
    DrawQuads(1);
}

void DrawQuads(int quadCount) {
    if (quadCount <= 0) return;

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, quadCount * INDICES_PER_QUAD, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// #################################################
// Delete Quad Geometry
// #################################################
void DeleteQuadGeometry() {
    if (quadVAO) {
        glDeleteVertexArrays(1, &quadVAO);
        quadVAO = 0;
    }
    if (quadVBO) {
        glDeleteBuffers(1, &quadVBO);
        quadVBO = 0;
    }
    if (quadEBO) {
        glDeleteBuffers(1, &quadEBO);
        quadEBO = 0;
    }
}