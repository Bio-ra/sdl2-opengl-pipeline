#include "sprite.h"
#include "quad.h"
#include <GL/glew.h>

void Sprite::draw(GLint locModel, GLint locTint) const {
    // bind texture if present
    if (texture && texture->getID() != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getID());
    }

    // Set UV scale/offset uniforms if the current program defines them.
    // Query current program and set "uUVScale" and "uUVOffset" if present.
    GLint curProg = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &curProg);
    if (curProg != 0) {
        GLint locUVScale = glGetUniformLocation(curProg, "uUVScale");
        if (locUVScale >= 0) {
            glUniform2f(locUVScale, uvScaleX, uvScaleY);
        }
        GLint locUVOffset = glGetUniformLocation(curProg, "uUVOffset");
        if (locUVOffset >= 0) {
            glUniform2f(locUVOffset, uvOffsetX, uvOffsetY);
        }
    }

    // set tint (if uniform location valid)
    if (locTint >= 0) {
        glUniform4f(locTint, tintR, tintG, tintB, tintA);
    }

    // set model matrix (if uniform location valid)
    if (locModel >= 0) {
        Mat3 model = getModelMatrix();
        glUniformMatrix3fv(locModel, 1, GL_FALSE, model.data());
    }

    DrawQuad();
}

void Sprite::nextFrame() {
    if (atlasCols <= 0 || atlasRows <= 0) return;
    int total = atlasCols * atlasRows;
    frameIndex = (frameIndex + 1) % total;
    int col = frameIndex % atlasCols;
    int row = frameIndex / atlasCols;
    // reuse existing setter to update UVs and stored state
    const_cast<Sprite*>(this)->setFrame(col, row, atlasCols, atlasRows);
}

void Sprite::prevFrame() {
    if (atlasCols <= 0 || atlasRows <= 0) return;
    int total = atlasCols * atlasRows;
    frameIndex = (frameIndex - 1 + total) % total;
    int col = frameIndex % atlasCols;
    int row = frameIndex / atlasCols;
    const_cast<Sprite*>(this)->setFrame(col, row, atlasCols, atlasRows);
}