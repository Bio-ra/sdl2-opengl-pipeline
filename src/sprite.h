#pragma once

#include "transformation.h"
#include "textureHandler.h"
#include <GL/glew.h>

class SpriteBatch;

// Simple Sprite holder for your 2D renderer.
// - Minimal API: position, size, rotation, tint, texture.
// - draw() uploads model matrix and tint, binds texture unit 0 and calls DrawQuad().
struct Sprite {
    Texture* texture = nullptr;

    // float x = 0.0f;
    // float y = 0.0f;
    // float w = 100.0f;
    // float h = 100.0f;
    // float rotation = 0.0f;

    // float tintR = 1.0f;
    // float tintG = 1.0f;
    // float tintB = 1.0f;
    // float tintA = 1.0f;

    // Default
    // float uvScaleX = 1.0f;
    // float uvScaleY = 1.0f;
    // float uvOffsetX = 0.0f;
    // float uvOffsetY = 0.0f;

    // optional atlas playback state
    // int atlasCols = 0;
    // int atlasRows = 0;
    // int frameIndex = 0; // 0..(atlasCols*atlasRows-1)

    float x = 0.0f, y = 0.0f, w = 100.0f, h = 100.0f, rotation = 0.0f;
    float tintR = 1.0f, tintG = 1.0f, tintB = 1.0f, tintA = 1.0f;
    float uvScaleX = 1.0f, uvScaleY = 1.0f, uvOffsetX = 0.0f, uvOffsetY = 0.0f;
    int atlasCols = 0, atlasRows = 0, frameIndex = 0;

    Sprite() = default;
    explicit Sprite(Texture* tex) : texture(tex) {}

    void setTexture(Texture* tex) { texture = tex; }
    void setPosition(float px, float py) { x = px; y = py; }
    void setSize(float width, float height) { w = width; h = height; }
    void setRotation(float rad) { rotation = rad; }
    void setTint(float r, float g, float b, float a = 1.0f) { 
        tintR = r; tintG = g; tintB = b; tintA = a; 
    }

    Mat3 getModelMatrix() const { return Mat3::TRS(x, y, rotation, w, h); }

    // Set UV transform directly
    void setUVTransform(float scaleX, float scaleY, float offsetX, float offsetY) {
        uvScaleX = scaleX; uvScaleY = scaleY; 
        uvOffsetX = offsetX; uvOffsetY = offsetY;
    }

    // Convenience: set frame from atlas coordinates.
    // col,row are zero-based indices; cols,rows are atlas dimensions.
    void setFrame(int col, int row, int cols, int rows);

    // Define atlas size without changing current frame index
    void setAtlasSize(int cols, int rows);
    //     if (cols <= 0 || rows <= 0) { atlasCols = atlasRows = 0; frameIndex = 0; return; }
    //     atlasCols = cols;
    //     atlasRows = rows;
    //     int total = atlasCols * atlasRows;
    //     frameIndex = frameIndex % total;
    // }

    // Advance to next/previous frame
    void nextFrame();
    void prevFrame();

    // draw the sprite.
    void draw(GLint locModel, GLint locTint) const;
    void submitToBatch(SpriteBatch& batch) const;
};