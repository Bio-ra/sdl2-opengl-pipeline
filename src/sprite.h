#pragma once

#include "transformation.h"
#include "textureHandler.h"
#include <GL/glew.h>

// Simple Sprite holder for your 2D renderer.
// - Minimal API: position, size, rotation, tint, texture.
// - draw() uploads model matrix and tint, binds texture unit 0 and calls DrawQuad().
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

    // Default
    float uvScaleX = 1.0f;
    float uvScaleY = 1.0f;
    float uvOffsetX = 0.0f;
    float uvOffsetY = 0.0f;

    // optional atlas playback state
    int atlasCols = 0;
    int atlasRows = 0;
    int frameIndex = 0; // 0..(atlasCols*atlasRows-1)

    Sprite() = default;
    explicit Sprite(Texture* tex) : texture(tex) {}

    void setTexture(Texture* tex) { texture = tex; }
    void setPosition(float px, float py) { x = px; y = py; }
    void setSize(float width, float height) { w = width; h = height; }
    void setRotation(float rad) { rotation = rad; }
    void setTint(float r, float g, float b, float a = 1.0f) { tintR = r; tintG = g; tintB = b; tintA = a; }

    Mat3 getModelMatrix() const { return Mat3::TRS(x, y, rotation, w, h); }

    // Set UV transform directly
    void setUVTransform(float scaleX, float scaleY, float offsetX, float offsetY) {
        uvScaleX = scaleX; uvScaleY = scaleY; uvOffsetX = offsetX; uvOffsetY = offsetY;
    }

    // Convenience: set frame from atlas coordinates.
    // col,row are zero-based indices; cols,rows are atlas dimensions.
    void setFrame(int col, int row, int cols, int rows) {
        if (cols <= 0 || rows <= 0) return;
        uvScaleX = 1.0f / float(cols);
        uvScaleY = 1.0f / float(rows);
        uvOffsetX = col * uvScaleX;
        uvOffsetY = row * uvScaleY;
        // store atlas info for playback helpers
        atlasCols = cols;
        atlasRows = rows;
        frameIndex = row * cols + col;
    }

    // Define atlas size without changing current frame index
    void setAtlasSize(int cols, int rows) {
        if (cols <= 0 || rows <= 0) { atlasCols = atlasRows = 0; frameIndex = 0; return; }
        atlasCols = cols;
        atlasRows = rows;
        int total = atlasCols * atlasRows;
        frameIndex = frameIndex % total;
    }

    // Advance to next/previous frame (wraps) and update UVs
    void nextFrame();
    void prevFrame();

    // Draw the sprite.
    // - locModel: uniform location of uModel in the currently bound shader (or -1 to skip)
    // - locTint: uniform location of uTint in the currently bound shader (or -1 to skip)
    // Assumes shader is already in use and that the shader's sampler is set to unit 0.
    void draw(GLint locModel, GLint locTint) const;
};