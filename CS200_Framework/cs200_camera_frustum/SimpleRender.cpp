// Name       : Yoonki Kim
// Assignment : Camera Frustum Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "SimpleRender.h"

SimpleRender::SimpleRender(Render& r) : render(r)
{
    PersProj[0] = Hcoord{ 1, 0, 0, 0 };
    PersProj[1] = Hcoord{ 0, 1, 0, 0 };
    PersProj[2] = Hcoord{ 0, 0, 1.f / 11, -5.f / 11 };
    PersProj[3] = Hcoord{ 0, 0, -2.f / 11, 10.f / 11 };
}

SimpleRender::~SimpleRender(void)
{
    world_vertices.clear();
    proj_vertices.clear();
}

void SimpleRender::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
    world_vertices.clear();
    proj_vertices.clear();

    const auto vtx_cnt = m.VertexCount();
    world_vertices.resize(vtx_cnt);
    proj_vertices.resize(vtx_cnt);

    for (auto idx = 0; idx < vtx_cnt; ++idx)
    {
        world_vertices[idx] = A * m.GetVertex(idx);

        //w might not be one
        const Hcoord proj_vertices_temp = PersProj * world_vertices[idx];
        proj_vertices[idx] = Hcoord{ proj_vertices_temp.x / proj_vertices_temp.w,
            proj_vertices_temp.y / proj_vertices_temp.w, proj_vertices_temp.z / proj_vertices_temp.w, 1 };
    }

    const auto edge_cnt = m.EdgeCount();
    render.SetColor(color);

    for (auto idx = 0; idx < edge_cnt; ++idx)
    {
        const auto edge = m.GetEdge(idx);
        render.DrawLine(proj_vertices[edge.index1], proj_vertices[edge.index2]);
    }
}

void SimpleRender::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
    const Vector light_vec{ 0, 0, 1 };
    const Point eye{ 0.f, 0.f, 5.f };

    world_vertices.clear();
    proj_vertices.clear();

    const auto vtx_cnt = m.VertexCount();
    world_vertices.resize(vtx_cnt);
    proj_vertices.resize(vtx_cnt);

    for (auto idx = 0; idx < vtx_cnt; ++idx)
    {
        world_vertices[idx] = A * m.GetVertex(idx);

        //w might not be one
        const Hcoord proj_vertices_temp = PersProj * world_vertices[idx];
        proj_vertices[idx] = Hcoord{ proj_vertices_temp.x / proj_vertices_temp.w,
            proj_vertices_temp.y / proj_vertices_temp.w, proj_vertices_temp.z / proj_vertices_temp.w, 1 };
    }

    const auto face_cnt = m.FaceCount();
    for (auto idx = 0; idx < face_cnt; ++idx)
    {
        const auto face = m.GetFace(idx);

        Vector orient_vec = cross(
            world_vertices[face.index2] - world_vertices[face.index1],
            world_vertices[face.index3] - world_vertices[face.index1]);

        Vector eye_to_vtx1 = eye - world_vertices[face.index1];

        const bool is_visible = dot(eye_to_vtx1, orient_vec) >= 0;
        if (is_visible)
        {
            const float diffuse_shading_factor = (dot(light_vec, orient_vec)) / (abs(light_vec) * abs(orient_vec));

            render.SetColor(diffuse_shading_factor * color);
            render.FillTriangle(proj_vertices[face.index1], proj_vertices[face.index2], proj_vertices[face.index3]);
        }
    }
}