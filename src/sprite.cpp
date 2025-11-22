#include "sprite.h"
#include "quad.h"
#include <GL/glew.h>

void Sprite::draw(GLint locModel, GLint locTint) const {
    if (texture && texture->getID() != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getID());
    }

    if (locTint >= 0) {
        glUniform4f(locTint, tintR, tintG, tintB, tintA);
    }

    if (locModel >= 0) {
        Mat3 model = getModelMatrix();
        glUniformMatrix3fv(locModel, 1, GL_FALSE, model.data());
    }

    DrawQuad();
}