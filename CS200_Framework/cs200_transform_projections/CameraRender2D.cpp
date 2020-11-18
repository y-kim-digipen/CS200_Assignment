// Name       : Yoonki Kim
// Assignment : Transform Projection Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "CameraRender2D.h"
#include "Projection.h"

CameraRender2D::CameraRender2D(Render& r) : render(r)
{
    const Camera default_cam{};
    world2camera = WorldToCamera(default_cam);
    camera2ndc = CameraToNDC(default_cam);
}

CameraRender2D::~CameraRender2D()
{
    cam_vertices.clear();
    ndc_vertices.clear();
}

void CameraRender2D::SetCamera(const Camera& cam)
{
    world2camera = WorldToCamera(cam);
    camera2ndc = CameraToNDC(cam);
}

void CameraRender2D::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
    cam_vertices.clear();
    ndc_vertices.clear();

    const auto vtx_cnt = m.VertexCount();
    cam_vertices.resize(vtx_cnt);
    ndc_vertices.resize(vtx_cnt);

    for (auto idx = 0; idx < vtx_cnt; ++idx)
    {
        const Hcoord cam_vertex = world2camera * (A * m.GetVertex(idx));
        cam_vertices[idx] = Point{ cam_vertex.x / cam_vertex.w, cam_vertex.y / cam_vertex.w, cam_vertex.z / cam_vertex.w };

        const Hcoord ndc_vertex = camera2ndc * cam_vertices[idx];
        ndc_vertices[idx] = Point{ ndc_vertex.x / ndc_vertex.w, ndc_vertex.y / ndc_vertex.w, ndc_vertex.z / ndc_vertex.w};
    }

    const auto edge_cnt = m.EdgeCount();
    render.SetColor(color);

    for (auto idx = 0; idx < edge_cnt; ++idx)
    {
        const auto edge = m.GetEdge(idx);
        if(cam_vertices[edge.index1].z < 0 && cam_vertices[edge.index2].z < 0)
        {
            render.DrawLine(ndc_vertices[edge.index1], ndc_vertices[edge.index2]);
        }
    }
}

void CameraRender2D::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
    const Affine cam_to_world = inverse(world2camera);
    const Vector light_vec = Vector{ cam_to_world[0][2], cam_to_world[1][2], cam_to_world[2][2] };
    const Point eye = Point(cam_to_world[0][3], cam_to_world[1][3], cam_to_world[2][3]);

    cam_vertices.clear();
    ndc_vertices.clear();

    const auto vtx_cnt = m.VertexCount();
    cam_vertices.resize(vtx_cnt);
    ndc_vertices.resize(vtx_cnt);

    for (auto idx = 0; idx < vtx_cnt; ++idx)
    {
        const Hcoord cam_vertex = world2camera * (A * m.GetVertex(idx));
        cam_vertices[idx] = Point{ cam_vertex.x / cam_vertex.w, cam_vertex.y / cam_vertex.w, cam_vertex.z / cam_vertex.w };

        const Hcoord ndc_vertex = camera2ndc * cam_vertices[idx];
        ndc_vertices[idx] = Point{ ndc_vertex.x / ndc_vertex.w, ndc_vertex.y / ndc_vertex.w, ndc_vertex.z / ndc_vertex.w };
    }

    const auto face_cnt = m.FaceCount();
    for (auto idx = 0; idx < face_cnt; ++idx)
    {
        const auto face = m.GetFace(idx);

        Vector orient_vec = cross(
            cam_to_world * cam_vertices[face.index2] - cam_to_world * cam_vertices[face.index1],
            cam_to_world * cam_vertices[face.index3] - cam_to_world * cam_vertices[face.index1]);

        Vector eye_to_vtx1 = eye - (cam_to_world * cam_vertices[face.index1]);

        const bool is_visible = dot(eye_to_vtx1, orient_vec) >= 0;

        if (is_visible && (cam_vertices[face.index1].z < 0 && cam_vertices[face.index2].z < 0 && cam_vertices[face.index3].z < 0))
        {
            const float diffuse_shading_factor = (dot(light_vec, orient_vec)) / (abs(light_vec) * abs(orient_vec));

            render.SetColor(diffuse_shading_factor * color);
            render.FillTriangle(ndc_vertices[face.index1], ndc_vertices[face.index2], ndc_vertices[face.index3]);
        }
    }
}
