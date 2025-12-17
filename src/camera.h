#pragma once

#include "transformation.h"

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

    // view matrix: transforms world -> camera space 
    Mat3 getViewMatrix() const;

    // projection helper that wraps Mat3::OrthoFromScreen
    Mat3 getProjectionFromScreen(float width, float height, bool originTopLeft = true) const;

    // combined projection * view
    Mat3 getViewProjection(float width, float height, bool centerOrigin, bool includeZoom = true) const;
};