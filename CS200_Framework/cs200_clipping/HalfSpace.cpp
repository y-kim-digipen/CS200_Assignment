// Name       : Yoonki Kim
// Assignment : Clipping Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "HalfSpace.h"
HalfSpace::HalfSpace(const Vector& m, const Point& C) : Hcoord(m)
{
    w = -(x * C.x + y * C.y + z * C.z);
}

HalfSpace::HalfSpace(const Point& A, const Point& B, const Point& C, const Point& P)
{
    Vector normal = cross(B - A, C - A);
    if(dot(normal, A - P) < 0)
    {
        normal = -normal;
    }
    x = normal.x;
    y = normal.y;
    z = normal.z;
    w = -(x * A.x + y * A.y + z * A.z);
}

Polyhedron::Polyhedron(bool cube)
{
    if (!cube)
        return;
    half_spaces.reserve(6);
    //front face
    half_spaces.emplace_back(HalfSpace{ 0, 0, -1, -1 });
    //back face
    half_spaces.emplace_back(HalfSpace{ 0, 0, 1, -1 });
    //up face
    half_spaces.emplace_back(HalfSpace{ 1, 0, 0, -1 });
    //down face
    half_spaces.emplace_back(HalfSpace{ -1, 0, 0, -1 });
    //right face
    half_spaces.emplace_back(HalfSpace{ 0, 1, 0, -1 });
    //left face
    half_spaces.emplace_back(HalfSpace{ 0, -1, 0, -1 });
}

Polyhedron::Polyhedron(float fov, float a, float N, float F)
{
    half_spaces.reserve(6);
    //front face
    half_spaces.emplace_back(HalfSpace{ 0, 0, 1, N });

    //back face
    half_spaces.emplace_back(HalfSpace{ 0, 0, -1, -F });

    const float tan_half_fov = tan(fov / 2.f);
    const float half_near_width = N * tan_half_fov;
    const float half_near_height = a * half_near_width;

    //up face
    const Vector up_orient = cross(Point(half_near_width, half_near_height, -N) - Point(), Point(-half_near_width, half_near_height, -N) - Point());
    half_spaces.emplace_back(HalfSpace(up_orient, Point(half_near_width, half_near_height, -N)));

    //down face
    const Vector down_orient = cross(Point(-half_near_width, -half_near_height, -N) - Point(), Point(half_near_width, -half_near_height, -N) - Point());
    half_spaces.emplace_back(HalfSpace(down_orient, Point(-half_near_width, -half_near_height, -N)));

    //right face
    const Vector right_orient = cross(Point(half_near_width, -half_near_height, -N) - Point(), Point(half_near_width, half_near_height, -N) - Point());
    half_spaces.emplace_back(HalfSpace(right_orient, Point(half_near_width, -half_near_height, -N)));

    //left face
    const Vector left_orient = cross(Point(-half_near_width, half_near_height, -N) - Point(), Point(-half_near_width, -half_near_height, -N) - Point());
    half_spaces.emplace_back(HalfSpace(left_orient, Point(-half_near_width, half_near_height, -N)));
}

bool contains(const Polyhedron& polyhedron, const Hcoord& P)
{
    bool is_contains = true;
    for(const auto & half_space : polyhedron.half_spaces)
    {
        is_contains &= dot(half_space, P) < 0;
    }
    return is_contains;
}
