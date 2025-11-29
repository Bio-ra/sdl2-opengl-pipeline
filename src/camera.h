#pragma once

#include "transformation.h"

// Minimal 2D camera for your renderer (pixel coordinates).
// Position = world-space center (pixels). rotation = radians. zoom = scale (>0).
struct Camera2D {
    float x = 0.0f, y = 0.0f, rotation = 0.0f, zoom = 1.0f;

    Camera2D() = default;
    Camera2D(float px, float py, float rot = 0.0f, float z = 1.0f);

    void setPosition(float px, float py);
    void move(float dx, float dy);

    void setRotation(float r);
    void rotate(float dr);

    void setZoom(float z);
    void zoomBy(float factor);

    // View matrix: transforms world -> camera space (inverse camera transform)
    Mat3 getViewMatrix() const;

    // Projection helper that wraps Mat3::OrthoFromScreen
    Mat3 getProjectionFromScreen(float width, float height, bool originTopLeft = true) const;

    // Convenience: combined projection * view
    Mat3 getViewProjection(float width, float height, bool centerOrigin, bool includeZoom = true) const;
};