// Name       : Yoonki Kim
// Assignment : Z Buffer Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "RasterUtilities.h"
#include <cmath>
#include <array>

struct vec2
{
    vec2(float x_, float y_) : x(x_), y(y_) {}
    vec2(int x, int y) : vec2(static_cast<float>(x), static_cast<float>(y)) {}
    float x, y;
};
struct Equation {
    Equation(vec2 start_p, vec2 end_p) noexcept :
        a(start_p.y - end_p.y), b(end_p.x - start_p.x), c(start_p.x* end_p.y - end_p.x * start_p.y),
        tl((a != 0.f) ? (a > 0.f ? true : false) : (b < 0.f ? true : false))
    {};

    [[nodiscard]] float Evaluate(vec2 point) const noexcept {
        return a * point.x + b * point.y + c;
    }

    float a, b, c;
    bool tl;
};
struct TriangleEquations {
    TriangleEquations(const Equation l_e0, const Equation l_e1, const Equation l_e2) noexcept : e0(l_e0), e1(l_e1), e2(l_e2) {};
    Equation e0, e1, e2;
};
struct TriangleBoundingBox
{
    TriangleBoundingBox() = delete;
    TriangleBoundingBox(const vec2 p0, const vec2 p1, const vec2 p2) noexcept {
        min.x = floor(p0.x < (p1.x < p2.x ? p1.x : p2.x) ? p0.x : (p1.x < p2.x ? p1.x : p2.x));
        min.y = floor(p0.y < (p1.y < p2.y ? p1.y : p2.y) ? p0.y : (p1.y < p2.y ? p1.y : p2.y));

        max.x = ceil(p0.x > (p1.x > p2.x ? p1.x : p2.x) ? p0.x : (p1.x > p2.x ? p1.x : p2.x));
        max.y = ceil(p0.y > (p1.y > p2.y ? p1.y : p2.y) ? p0.y : (p1.y > p2.y ? p1.y : p2.y));
    };
    vec2 min{ 0, 0 }, max{ 0, 0 };
};
struct PlaneEquation
{
    PlaneEquation(Point P, Point Q, Point R) noexcept
    {
        const Vector PtoQ = Q - P;
        const Vector PtoR = R - P;
        const Vector orient = cross(PtoQ, PtoR);
        a = orient.x;
        b = orient.y;
        c = orient.z;
        d = a * P.x + b * P.y + c * P.z;
    }

    [[nodiscard]] float GetZVal(vec2 pos) const
    {
        const float z = -(a * pos.x + b * pos.y - d) / c;
        return z;
    }
    [[nodiscard]] Vector GetOrient() const { return Vector{a, b, c}; }
    [[nodiscard]] vec2 GetIncrements() const
    {
        return vec2{ -a / c, -b / c };
    }

private:
    float a, b, c, d;
};


namespace helper
{
    Equation EdgeEquationTopLeft(const vec2 start_p, const vec2 end_p) noexcept;
    TriangleEquations TriangleEdgeEquation(vec2 p0, vec2 p1, vec2 p2) noexcept;
    bool PointInTriangle(TriangleEquations equations, vec2 p) noexcept;
    bool PointInEdgeTopLeft(Equation e, vec2 p) noexcept;
}

void ClearBuffers(Raster& r, float z)
{
    const auto width = r.Width(), height = r.Height();

    for(auto y = 0; y < height; ++y)
    {
        r.GotoPoint(0, y);
        for(auto x = 0; x < width; ++x)
        {
            r.WritePixel();
            r.WriteZ(z);
            r.IncrementX();
        }
    }
}

void FillTriangle(Raster& r, const Hcoord& P, const Hcoord& Q, const Hcoord& R)
{
    auto to_point = [](Hcoord p)
    {
        return Point(p.x / p.w, p.y / p.w, p.z / p.w);
    };

    std::array<Point, 3> vertices = { to_point(P), to_point(Q), to_point(R) };

    const PlaneEquation equations_3d(vertices[0], vertices[1], vertices[2]);

    //if triangle is back-face, change vertices order to front-face
    if (dot(Vector{ 0, 0, -1 }, equations_3d.GetOrient()) >= 0)
    {
        std::swap(vertices[1], vertices[2]);
    }

    const std::array<vec2, 3> vertices_2d
    {
        vec2{vertices[0].x, vertices[0].y},
        vec2{vertices[1].x, vertices[1].y},
        vec2{vertices[2].x, vertices[2].y},
    };

    const TriangleBoundingBox bounding_box(vertices_2d[0], vertices_2d[1], vertices_2d[2]);
    const TriangleEquations equations_2D = helper::TriangleEdgeEquation(vertices_2d[0], vertices_2d[1], vertices_2d[2]);

    float eval0 = equations_2D.e0.Evaluate(bounding_box.min);
    float eval1 = equations_2D.e1.Evaluate(bounding_box.min);
    float eval2 = equations_2D.e2.Evaluate(bounding_box.min);

    float eval_z = equations_3d.GetZVal(bounding_box.min);

    const vec2 z_eval_increments = equations_3d.GetIncrements();

    const std::pair<int, int> bounding_box_min = { static_cast<int>(bounding_box.min.x), static_cast<int>(bounding_box.min.y) };
    const std::pair<int, int> bounding_box_max = { static_cast<int>(bounding_box.max.x), static_cast<int>(bounding_box.max.y) };

    for(int j = bounding_box_min.second; j < bounding_box_max.second; ++j)
    {
        float h_eval0 = eval0;
        float h_eval1 = eval1;
        float h_eval2 = eval2;

        float z_val = eval_z;

        r.GotoPoint(bounding_box_min.first, j);
        for (int i = bounding_box_min.first; i < bounding_box_max.first; ++i)
        {
            const bool eval0InTheEdge = h_eval0 > 0 || (h_eval0 == 0.f && equations_2D.e0.tl == true);
            const bool eval1InTheEdge = h_eval1 > 0 || (h_eval1 == 0.f && equations_2D.e1.tl == true);
            const bool eval2InTheEdge = h_eval2 > 0 || (h_eval2 == 0.f && equations_2D.e2.tl == true);

            if (eval0InTheEdge && eval1InTheEdge && eval2InTheEdge) 
            {
                if(z_val < r.GetZ())
                {
                    r.WriteZ(z_val);
                    r.WritePixel();
                }
            }

            h_eval0 += equations_2D.e0.a;
            h_eval1 += equations_2D.e1.a;
            h_eval2 += equations_2D.e2.a;

            z_val += z_eval_increments.x;

            r.IncrementX();
        }
        eval0 += equations_2D.e0.b;
        eval1 += equations_2D.e1.b;
        eval2 += equations_2D.e2.b;

        eval_z += z_eval_increments.y;
    }
}

namespace helper
{
    Equation EdgeEquationTopLeft(const vec2 start_p, const vec2 end_p) noexcept
    {
        return Equation(start_p, end_p);
    }

    TriangleEquations TriangleEdgeEquation(vec2 p0, vec2 p1, vec2 p2) noexcept
    {
        const Equation e0 = EdgeEquationTopLeft(p1, p2);
        const Equation e1 = EdgeEquationTopLeft(p2, p0);
        const Equation e2 = EdgeEquationTopLeft(p0, p1);
        return TriangleEquations{ e0, e1, e2 };
    }

    bool PointInTriangle(TriangleEquations equations, vec2 p) noexcept
    {
        if (PointInEdgeTopLeft(equations.e0, p) && PointInEdgeTopLeft(equations.e1, p) && PointInEdgeTopLeft(equations.e2, p))
            return true;
        return false;
    }

    bool PointInEdgeTopLeft(Equation e, vec2 p) noexcept
    {
        const float eval = e.a * p.x + e.b * p.y + e.c;
        return (eval > 0 || (eval == 0.f && e.tl == true)) ? true : false;
    }
}