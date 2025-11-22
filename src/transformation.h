#pragma once

#include <cstddef>

struct Mat3 {
    float m[9]; // column-major: m[col*3 + row]

    // Constructors / factory methods
    static Mat3 Identity();
    static Mat3 Translation(float tx, float ty);
    static Mat3 Rotation(float angleRad);
    static Mat3 Scale(float sx, float sy);
    static Mat3 TRS(float tx, float ty, float angleRad, float sx = 1.0f, float sy = 1.0f);

    // Orthographic projection that maps [left,right] x [bottom,top] -> NDC [-1,1] x [-1,1]
    static Mat3 Ortho(float left, float right, float bottom, float top);

    // Convenience: build an ortho matrix from screen size in pixels.
    // originTopLeft = true -> maps (0,0) top-left to NDC (-1,1) (typical screen coords, y down)
    // originTopLeft = false -> maps (0,0) bottom-left to NDC (-1,-1) (y up)
    static Mat3 OrthoFromScreen(float width, float height, bool originTopLeft = true);

    // multiplication (this * other)
    Mat3 operator*(const Mat3& other) const;

    // data pointer for glUniformMatrix3fv
    const float* data() const { return m; }
    float* data() { return m; }
};