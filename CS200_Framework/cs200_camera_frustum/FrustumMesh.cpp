// Name       : Yoonki Kim
// Assignment : Camera Frustum Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "FrustumMesh.h"
#include <utility>

const Mesh::Edge FrustumMesh::edges[16]
{
    {0, 1}, {0, 2}, {0, 3}, {0, 4},
    {1, 2}, {2, 3}, {3, 4}, {4, 1},
    {1, 5}, {2, 6}, {3, 7}, {4, 8},
    {5, 6}, {6, 7}, {7, 8}, {8, 5},
};
const Mesh::Face FrustumMesh::faces[12]
{
    {1, 2, 3}, {1, 3, 4},
    {2, 1, 6}, {1, 5 ,6},
    {6, 7, 2}, {2, 7, 3},
    {7, 8, 3}, {3, 8, 4},
    {8, 1, 4}, {8, 5, 1},
    {5, 7, 6}, {8, 7, 5},
};

FrustumMesh::FrustumMesh(float fov, float a, float n, float f)
{
    vertices[0] = Point{ 0, 0, 0 };

    std::pair<float, float> near_wh;
    std::pair<float, float> far_wh;
    near_wh.first = 2 * n * tan(fov / 2.f);
    near_wh.second = near_wh.first / a;

    far_wh.first = 2 * f * tan(fov / 2.f);
    far_wh.second = far_wh.first / a;

    vertices[1] = Point{ -near_wh.first / 2, -near_wh.second / 2, -n };
    vertices[2] = Point{ +near_wh.first / 2, -near_wh.second / 2, -n };
    vertices[3] = Point{ +near_wh.first / 2, +near_wh.second / 2, -n };
    vertices[4] = Point{ -near_wh.first / 2, +near_wh.second / 2, -n };

    vertices[5] = Point{ -far_wh.first / 2, -far_wh.second / 2, -f };
    vertices[6] = Point{ +far_wh.first / 2, -far_wh.second / 2, -f };
    vertices[7] = Point{ +far_wh.first / 2, +far_wh.second / 2, -f };
    vertices[8] = Point{ -far_wh.first / 2, +far_wh.second / 2, -f };
}

int FrustumMesh::VertexCount()
{
    return sizeof(vertices)/sizeof(Point);
}

Point FrustumMesh::GetVertex(int i)
{
    return vertices[i];
}

Vector FrustumMesh::Dimensions()
{
    return Vector{ 2, 2, 2 };
}

Point FrustumMesh::Center()
{
    return Point{};
}

int FrustumMesh::FaceCount()
{
    return sizeof(faces) / sizeof(Face);
}

Mesh::Face FrustumMesh::GetFace(int i)
{
    return faces[i];
}

int FrustumMesh::EdgeCount()
{
    return sizeof(edges) / sizeof(Edge);
}

Mesh::Edge FrustumMesh::GetEdge(int i)
{
    return edges[i];
}
