#include "camera.h"

// ctor
Camera2D::Camera2D(float px, float py, float rot, float z)
    : x(px), y(py), rotation(rot), zoom(z) { }

// position
void Camera2D::setPosition(float px, float py) { x = px; y = py; }
void Camera2D::move(float dx, float dy) { x += dx; y += dy; }

// rotation
void Camera2D::setRotation(float r) { rotation = r; }
void Camera2D::rotate(float dr) { rotation += dr; }

// zoom
void Camera2D::setZoom(float z) { if (z > 0.0f) zoom = z; }
void Camera2D::zoomBy(float factor) { if (factor > 0.0f) zoom *= factor; }

// view matrix = S(1/zoom) * R(-rotation) * T(-pos)
Mat3 Camera2D::getViewMatrix() const {
    // Mat3 s = Mat3::Scale(1.0f / zoom, 1.0f / zoom);
    // Mat3 r = Mat3::Rotation(-rotation);
    // Mat3 t = Mat3::Translation(-x, -y);
    // return s * r * t;
    return Mat3::Scale(1.0f / zoom, 1.0f / zoom) * 
           Mat3::Rotation(-rotation) * 
           Mat3::Translation(-x, -y);
}

Mat3 Camera2D::getProjectionFromScreen(float width, float height, bool originTopLeft) const {
    return Mat3::OrthoFromScreen(width, height, originTopLeft);
}


Mat3 Camera2D::getViewProjection(float width, float height, bool centerOrigin, bool includeZoom) const {

    // use camera zoom unless includeZoom == false
    float effectiveZoom = includeZoom ? this->zoom : 1.0f;

    // return proj * s * r * t;
    return getProjectionFromScreen(width, height, centerOrigin) * 
           Mat3::Scale(1.0f / effectiveZoom, 1.0f / effectiveZoom) * 
           Mat3::Rotation(-rotation) * 
           Mat3::Translation(-x, -y);
}