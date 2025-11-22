#include "transformation.h"
#include <cmath>

// Identity
Mat3 Mat3::Identity() {
    Mat3 r{};
    r.m[0] = 1.0f; r.m[1] = 0.0f; r.m[2] = 0.0f;
    r.m[3] = 0.0f; r.m[4] = 1.0f; r.m[5] = 0.0f;
    r.m[6] = 0.0f; r.m[7] = 0.0f; r.m[8] = 1.0f;
    return r;
}

// Translation (tx,ty)
Mat3 Mat3::Translation(float tx, float ty) {
    Mat3 r = Identity();
    r.m[6] = tx; // m02
    r.m[7] = ty; // m12
    return r;
}

// Rotation (angle in radians), counter-clockwise
Mat3 Mat3::Rotation(float angleRad) {
    float c = std::cos(angleRad);
    float s = std::sin(angleRad);
    Mat3 r{};
    r.m[0] =  c; r.m[1] =  s; r.m[2] = 0.0f;
    r.m[3] = -s; r.m[4] =  c; r.m[5] = 0.0f;
    r.m[6] =  0; r.m[7] =  0; r.m[8] = 1.0f;
    return r;
}

// Scale (sx,sy)
Mat3 Mat3::Scale(float sx, float sy) {
    Mat3 r{};
    r.m[0] = sx;  r.m[1] = 0.0f; r.m[2] = 0.0f;
    r.m[3] = 0.0f; r.m[4] = sy;  r.m[5] = 0.0f;
    r.m[6] = 0.0f; r.m[7] = 0.0f; r.m[8] = 1.0f;
    return r;
}

// TRS: Translation * Rotation * Scale (applies scale, then rotation, then translation)
Mat3 Mat3::TRS(float tx, float ty, float angleRad, float sx, float sy) {
    return Translation(tx, ty) * Rotation(angleRad) * Scale(sx, sy);
}

// Ortho projection mapping [left,right]x[bottom,top] -> NDC [-1,1]x[-1,1]
Mat3 Mat3::Ortho(float left, float right, float bottom, float top) {
    float sx = 2.0f / (right - left);
    float sy = 2.0f / (top - bottom);
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);

    Mat3 r{};
    r.m[0] = sx;  r.m[1] = 0.0f; r.m[2] = 0.0f;
    r.m[3] = 0.0f; r.m[4] = sy;  r.m[5] = 0.0f;
    r.m[6] = tx;  r.m[7] = ty;  r.m[8] = 1.0f;
    return r;
}

// Convenience: build ortho from screen dimensions
Mat3 Mat3::OrthoFromScreen(float width, float height, bool originTopLeft) {
    if (originTopLeft) {
        return Ortho(0.0f, width, height, 0.0f);
    } else {
        return Ortho(0.0f, width, 0.0f, height);
    }
}

// Matrix multiply (column-major)
Mat3 Mat3::operator*(const Mat3& other) const {
    Mat3 res{};
    for (int c = 0; c < 3; ++c) {
        for (int r = 0; r < 3; ++r) {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k) {
                float a = this->m[k*3 + r];   // (r,k)
                float b = other.m[c*3 + k];   // (k,c)
                sum += a * b;
            }
            res.m[c*3 + r] = sum;
        }
    }
    return res;
}