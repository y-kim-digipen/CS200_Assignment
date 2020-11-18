// Name       : Yoonki Kim
// Assignment : Camera Frustum Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include <cmath>
#include "Camera.h"

Camera::Camera(void) : Camera(Point{}, Vector{0, 0, -1}, Vector{0, 1, 0}, 90, 1, 0.1, 10) {}

Camera::Camera(const Point& E, const Vector& look, const Vector& rel, float fov, float aspect, float near, float far)
    : eye{ E }, near{ near }, far{ far }
{
    back = -normalize(look);
    right = normalize(cross(look, rel));
    up = cross(back, right);

    distance = this->near;

    width = 2 * tan(fov / 2) * distance;
    height = width / aspect;
}

Point Camera::Eye() const
{
    return eye;
}

Vector Camera::Right() const
{
    return right;
}

Vector Camera::Up() const
{
    return up;
}

Vector Camera::Back() const
{
    return back;
}

Vector Camera::ViewportGeometry() const
{
    return Vector{ width, height, distance };
}

float Camera::NearDistance() const
{
    return near;
}

float Camera::FarDistance() const
{
    return far;
}

Camera& Camera::Zoom(float factor)
{
    width *= factor;
    height *= factor;
    return *this;
}

Camera& Camera::Forward(float distance_increment)
{
    eye = eye + Vector{distance_increment * -back.x, distance_increment * -back.y, distance_increment * -back.z};
    return *this;
}

Camera& Camera::Yaw(float angle)
{
    const Affine yaw_mat = rotate(angle, up);

    back = yaw_mat * back;
    right = yaw_mat * right;
    return *this;
}

Camera& Camera::Pitch(float angle)
{
    const Affine pitch_mat = rotate(angle, right);

    up = pitch_mat * up;
    back = pitch_mat * back;
    return *this;
}

Camera& Camera::Roll(float angle)
{
    const Affine roll_mat = rotate(angle, back);

    up = roll_mat * up;
    right = roll_mat * right;
    return *this;
}
