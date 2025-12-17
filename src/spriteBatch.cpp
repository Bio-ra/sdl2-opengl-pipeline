#include "spriteBatch.h"
#include "quad.h"
#include <cassert>

SpriteBatch::SpriteBatch(int maxQuads)
    : m_maxQuads(maxQuads), m_totalQuads(0)
{
    // m_vertices.reserve((size_t)maxQuads * 4 * 4);
    m_vertices.reserve(maxQuads * 16); // 4 verts * 4 floats per vert
    m_groups.reserve(32);
}

void SpriteBatch::begin() {
    m_vertices.clear();
    m_groups.clear();
    m_totalQuads = 0;
}

void SpriteBatch::appendQuadVerts(float x, float y, float w, float h,
                                  float u0, float v0, float u1, float v1)
{

    // cleaner: build array and insert
    float verts[] = {
        x, y, u0, v0,
        x + w, y, u1, v0,
        x + w, y + h, u1, v1,
        x, y + h, u0, v1
    };
    m_vertices.insert(m_vertices.end(), verts, verts + 16);
}

void SpriteBatch::draw(Texture* texture,
                       float x, float y, float width, float height,
                       float u0, float v0, float u1, float v1,
                       // float r, float green, float b, float a)
                       float r, float g, float b, float a)
{
    assert(texture != nullptr);
    if (m_totalQuads >= m_maxQuads) return;

    if (m_groups.empty() ||
        m_groups.back().texture != texture ||
        m_groups.back().tint[0] != r ||
        // m_groups.back().tint[1] != green ||
        m_groups.back().tint[1] != g ||
        m_groups.back().tint[2] != b ||
        m_groups.back().tint[3] != a)
    {
        // Group grp;
        // grp.texture = texture;
        // grp.startQuad = m_totalQuads;
        // grp.quadCount = 0;
        // grp.tint[0] = r;
        // grp.tint[1] = green;
        // grp.tint[2] = b;
        // grp.tint[3] = a;
        // m_groups.push_back(grp);
        Group grp = {texture, m_totalQuads, 0, {r, g, b, a}};
        m_groups.push_back(grp);
    }

    appendQuadVerts(x, y, width, height, u0, v0, u1, v1);
    // m_groups.back().quadCount += 1;
    // m_totalQuads += 1;
    m_groups.back().quadCount++;
    m_totalQuads++;
}

void SpriteBatch::flush(GLint locModel, GLint locTint)
{
    if (m_totalQuads == 0) return;

    // float identityMat[9] = {1,0,0, 0,1,0, 0,0,1};
    float identity[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    
    // GLint curProg = 0;
    GLint prog;
    // glGetIntegerv(GL_CURRENT_PROGRAM, &curProg);
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    // GLint locUVScale = (curProg != 0) ? glGetUniformLocation(curProg, "uUVScale") : -1;
    // GLint locUVOffset = (curProg != 0) ? glGetUniformLocation(curProg, "uUVOffset") : -1;
    GLint locUVScale = (prog != 0) ? glGetUniformLocation(prog, "uUVScale") : -1;
    GLint locUVOffset = (prog != 0) ? glGetUniformLocation(prog, "uUVOffset") : -1;
    
    for (const Group& g : m_groups) {
        if (g.quadCount <= 0) continue;

        g.texture->bind();

        // if (locModel >= 0) glUniformMatrix3fv(locModel, 1, GL_FALSE, identityMat);
        if (locModel >= 0) glUniformMatrix3fv(locModel, 1, GL_FALSE, identity);
        if (locTint >= 0) glUniform4f(locTint, g.tint[0], g.tint[1], g.tint[2], g.tint[3]);
        
        if (locUVScale >= 0) glUniform2f(locUVScale, 1.0f, 1.0f);
        if (locUVOffset >= 0) glUniform2f(locUVOffset, 0.0f, 0.0f);

        // int startFloat = g.startQuad * 4 * 4;
        // const float* ptr = m_vertices.data() + startFloat;
        // UpdateQuadBuffer(ptr, g.quadCount);
        UpdateQuadBuffer(m_vertices.data() + g.startQuad * 16, g.quadCount);
        DrawQuads(g.quadCount);
    }

    begin();
}