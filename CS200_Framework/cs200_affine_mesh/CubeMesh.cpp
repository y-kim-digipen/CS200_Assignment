// Name       : Yoonki Kim
// Assignment : Affine and Mesh Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "CubeMesh.h"

const Point CubeMesh::vertices[8]
{
    Point{-1,1,-1}, Point{1, 1 ,-1}, Point{1, 1 ,1}, Point{-1, 1, 1},
    Point{-1,-1,-1}, Point{1, -1, -1}, Point{1, -1, 1}, Point{-1, -1, 1},
};
const Mesh::Face CubeMesh::faces[12]
{
    {3, 1, 0}, {1, 3, 2},
    {0, 1, 4}, {4, 1, 5},
    {4, 7, 0}, {0, 7, 3},
    {7, 2, 3}, {2, 7, 6},
    {1, 2, 5}, {5, 2, 6},
    {4, 5, 7}, {7, 5, 6},
};
const Mesh::Edge CubeMesh::edges[12]
{
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {0, 4}, {1, 5}, {2, 6}, {3, 7},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
};

int CubeMesh::VertexCount(void)
{
    return 8;
}

Point CubeMesh::GetVertex(int i)
{
    return *(vertices + i);
}

Vector CubeMesh::Dimensions(void)
{
    return Vector(2, 2, 2);
}

Point CubeMesh::Center(void)
{
    return Point{ 0, 0, 0 };
}

int CubeMesh::FaceCount(void)
{
    return 12;
}

Mesh::Face CubeMesh::GetFace(int i)
{
    return *(faces + i);
}

int CubeMesh::EdgeCount(void)
{
    return 12;
}

Mesh::Edge CubeMesh::GetEdge(int i)
{
    return *(edges + i);
}
