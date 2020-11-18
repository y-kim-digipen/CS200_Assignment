#pragma once
#include "cs200_affine_mesh/Mesh.h"

class Triangle2DMesh : public Mesh
{
public:
    ~Triangle2DMesh(void) {}
    int VertexCount(void) override
    {
        return 3;
    };
    Point GetVertex(int i) override
    {
        return vertices[i];
    };
    Vector Dimensions(void) override
    {
        return Vector{ 2, 2, 0 };
    };
    Point Center(void) override
    {
        return Point{ -1 / 3, 1/3, -1 };
    };
    int FaceCount(void)
    {
        return 1;
    };
    Face GetFace(int i)
    {
        return faces[i];
    };
    int EdgeCount(void)
    {
        return 3;
    };
    Edge GetEdge(int i)
    {
        return edges[i];
    };

private:
    const Point vertices[3] = 
    {
        Point{-1, 1, -1}, Point{1, 1, -1}, Point{-1, -1, -1},
    };
    const Face faces[1] = 
    {
        {1, 0, 2}
    };
    const Edge edges[3]
    {
        {0, 1}, {0, 2}, {1, 2}
    };
};