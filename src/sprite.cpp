#include "sprite.h"
#include "quad.h"
#include "spriteBatch.h"
#include <GL/glew.h>

void Sprite::draw(GLint locModel, GLint locTint) const {
    // bind texture if present
    if (texture && texture->getID()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getID());
    }

    // set UV transform (if uniform locations valid)
    GLint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    if (prog) {
        GLint locUVScale = glGetUniformLocation(prog, "uUVScale");

        if (locUVScale >= 0) glUniform2f(locUVScale, uvScaleX, uvScaleY);
        GLint locUVOffset = glGetUniformLocation(prog, "uUVOffset");

        if (locUVOffset >= 0) glUniform2f(locUVOffset, uvOffsetX, uvOffsetY);
    }

    if (locTint >= 0) glUniform4f(locTint, tintR, tintG, tintB, tintA);

    // set model matrix (if uniform location valid)
    if (locModel >= 0) {
        Mat3 model = getModelMatrix();
        glUniformMatrix3fv(locModel, 1, GL_FALSE, model.data());
    }

    DrawQuad();
}

void Sprite::submitToBatch(SpriteBatch& batch) const {
    if (!texture) return;

    batch.draw(texture, x, y, w, h, 
               uvOffsetX, uvOffsetY, 
               uvOffsetX + uvScaleX, uvOffsetY + uvScaleY, 
               tintR, tintG, tintB, tintA);
}

void Sprite::setFrame(int col, int row, int cols, int rows) {
    if (cols <= 0 || rows <= 0) return;
    uvScaleX = 1.0f / cols;
    uvScaleY = 1.0f / rows;
    uvOffsetX = col * uvScaleX;
    uvOffsetY = row * uvScaleY;
    atlasCols = cols;
    atlasRows = rows;
    frameIndex = row * cols + col;
}

void Sprite::setAtlasSize(int cols, int rows) {
    if (cols <= 0 || rows <= 0) {
        atlasCols = atlasRows = frameIndex = 0;
        return;
    }
    atlasCols = cols;
    atlasRows = rows;
    frameIndex %= (cols * rows);
}

void Sprite::nextFrame() {
    if (atlasCols <= 0 || atlasRows <= 0) return;
    frameIndex = (frameIndex + 1) % (atlasCols * atlasRows);
    setFrame(frameIndex % atlasCols, frameIndex / atlasCols, atlasCols, atlasRows);
}

void Sprite::prevFrame() {
    if (atlasCols <= 0 || atlasRows <= 0) return;
    int total = atlasCols * atlasRows;
    frameIndex = (frameIndex - 1 + total) % total;
    setFrame(frameIndex % atlasCols, frameIndex / atlasCols, atlasCols, atlasRows);
}