#pragma once

#include "transformation.h"
#include "textureHandler.h"
#include <GL/glew.h>

// Minimal Sprite API
struct Sprite {
    Texture* texture = nullptr;

    float x = 0.0f;
    float y = 0.0f;
    float w = 100.0f;
    float h = 100.0f;
    float rotation = 0.0f;

    float tintR = 1.0f;
    float tintG = 1.0f;
    float tintB = 1.0f;
    float tintA = 1.0f;

    Sprite() = default;
    explicit Sprite(Texture* tex) : texture(tex) {}

    void setTexture(Texture* tex) { texture = tex; }
    void setPosition(float px, float py) { x = px; y = py; }
    void setSize(float width, float height) { w = width; h = height; }
    void setRotation(float rad) { rotation = rad; }
    void setTint(float r, float g, float b, float a = 1.0f) { tintR = r; tintG = g; tintB = b; tintA = a; }

    Mat3 getModelMatrix() const { return Mat3::TRS(x, y, rotation, w, h); }

    // Draw: shader must be in use; locModel and locTint are uniform locations or -1 to skip
    void draw(GLint locModel, GLint locTint) const;
};