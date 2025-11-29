#pragma once
#include <vector>
#include <GL/glew.h>
#include "textureHandler.h"

class SpriteBatch {
public:
    explicit SpriteBatch(int maxQuads = 1024);

    void begin();

    void draw(Texture* texture,
              float x, float y, float width, float height,
              float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 1.0f,
              // float r = 1.0f, float green = 1.0f, float b = 1.0f, float a = 1.0f);
              float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

    void flush(GLint locModel, GLint locTint);

    void end(GLint locModel, GLint locTint) { flush(locModel, locTint); }

private:
    struct Group {
        Texture* texture;
        int startQuad;
        int quadCount;
        float tint[4];
    };

    int m_maxQuads;
    int m_totalQuads;
    std::vector<float> m_vertices;
    std::vector<Group> m_groups;

    void appendQuadVerts(float x, float y, float w, float h, float u0, float v0, float u1, float v1);
};