// Name       : Yoonki Kim
// Assignment : Affine and Mesh Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "Affine.h"

Hcoord::Hcoord(float X, float Y, float Z, float W) : x{ X }, y{Y}, z{Z}, w{W} {}
Point::Point(float X, float Y, float Z) : Hcoord(X, Y, Z, 1) {}
Vector::Vector(float X, float Y, float Z) : Hcoord(X, Y, Z, 0) {}

Affine::Affine(void)
{
    row[0] = Hcoord{ 0, 0, 0, 0 };
    row[1] = Hcoord{ 0, 0, 0, 0 };
    row[2] = Hcoord{ 0, 0, 0, 0 };
    row[3] = Hcoord{ 0, 0, 0, 1 };
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
    row[0] = Hcoord{ Lx.x, Ly.x, Lz.x, D.x };
    row[1] = Hcoord{ Lx.y, Ly.y, Lz.y, D.y };
    row[2] = Hcoord{ Lx.z, Ly.z, Lz.z, D.z };
    row[3] = Hcoord{ Lx.w, Ly.w, Lz.w, D.w };
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
    const Hcoord result{ u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w };
    return result;
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
    const Hcoord result{ u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w };
    return result;
}

Hcoord operator-(const Hcoord& v)
{
    return Hcoord{ -v.x, -v.y, -v.z, -v.w };
}

Hcoord operator*(float r, const Hcoord& v)  
{
    const Hcoord result{ r * v.x, r * v.y, r * v.z, r * v.w };
    return result;
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
    Hcoord result;
    const Hcoord row_0{ A[0] };
    result.x = row_0.x * v.x + row_0.y * v.y + row_0.z * v.z + row_0.w * v.w;
    const Hcoord row_1{ A[1] };
    result.y = row_1.x * v.x + row_1.y * v.y + row_1.z * v.z + row_1.w * v.w;
    const Hcoord row_2{ A[2] };
    result.z = row_2.x * v.x + row_2.y * v.y + row_2.z * v.z + row_2.w * v.w;
    const Hcoord row_3{ A[3] };
    result.w = row_3.x * v.x + row_3.y * v.y + row_3.z * v.z + row_3.w * v.w;
    return result;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
    Matrix result;
    const Hcoord B_col_0{ B[0].x, B[1].x, B[2].x, B[3].x };
    const Hcoord B_col_1{ B[0].y, B[1].y, B[2].y, B[3].y };
    const Hcoord B_col_2{ B[0].z, B[1].z, B[2].z, B[3].z };
    const Hcoord B_col_3{ B[0].w, B[1].w, B[2].w, B[3].w };

    auto calc = [](Hcoord row, Hcoord col)
    {
        return row.x * col.x + row.y * col.y + row.z * col.z + row.w * col.w;
    };

    result[0] = Hcoord{ calc(A[0], B_col_0), calc(A[0], B_col_1), calc(A[0], B_col_2), calc(A[0], B_col_3) };
    result[1] = Hcoord{ calc(A[1], B_col_0), calc(A[1], B_col_1), calc(A[1], B_col_2), calc(A[1], B_col_3) };
    result[2] = Hcoord{ calc(A[2], B_col_0), calc(A[2], B_col_1), calc(A[2], B_col_2), calc(A[2], B_col_3) };
    result[3] = Hcoord{ calc(A[3], B_col_0), calc(A[3], B_col_1), calc(A[3], B_col_2), calc(A[3], B_col_3) };
    return result;
}

float dot(const Vector& u, const Vector& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

float abs(const Vector& v)
{
    return sqrt(dot(v, v));
}

Vector normalize(const Vector& v)
{
    const float length = abs(v);
    return Vector{ v.x / length, v.y / length, v.z / length };
}

Vector cross(const Vector& u, const Vector& v)
{
    return Vector{ u.y * v.z - u.z * v.y , u.z * v.x - u.x * v.z , u.x * v.y - u.y * v.x };
}

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
    Matrix result;
    result[0] = m1[0] + m2[0];
    result[1] = m1[1] + m2[1];
    result[2] = m1[2] + m2[2];
    result[3] = m1[3] + m2[3];
    return result;
}

Matrix operator*(const float val, Matrix& m)
{
    Matrix result;
    result[0] = val * m[0];
    result[1] = val * m[1];
    result[2] = val * m[2];
    result[3] = val * m[3];
    return result;
}

Affine rotate(float t, const Vector& v)
{
    const float cos_t = cos(t);
    const float sin_t = sin(t);

    Matrix identity_matrix;
    identity_matrix[0] = Hcoord(1, 0, 0, 0);
    identity_matrix[1] = Hcoord(0, 1, 0, 0);
    identity_matrix[2] = Hcoord(0, 0, 1, 0);
    identity_matrix[3] = Hcoord(0, 0, 0, 1);

    Matrix outer_product_matrix_v;
    outer_product_matrix_v[0] = Hcoord{ v.x * v.x, v.x * v.y, v.x * v.z, 0 };
    outer_product_matrix_v[1] = Hcoord{ v.y * v.x, v.y * v.y, v.y * v.z, 0 };
    outer_product_matrix_v[2] = Hcoord{ v.z * v.x, v.z * v.y, v.z * v.z, 0 };
    outer_product_matrix_v[3] = Hcoord{ 0, 0, 0, 0 };

    Matrix cross_matrix_v;
    cross_matrix_v[0] = Hcoord{ 0, -v.z, v.y, 0 };
    cross_matrix_v[1] = Hcoord{ v.z, 0, -v.x, 0 };
    cross_matrix_v[2] = Hcoord{ -v.y, v.x, 0, 0 };
    cross_matrix_v[3] = Hcoord{ 0, 0, 0, 0 };

    const float length_v = abs(v);

    Matrix result = cos_t * identity_matrix + ((1.f - cos_t) / (length_v * length_v)) * outer_product_matrix_v + (sin_t / length_v) * cross_matrix_v;
    result[3][3] = 1;
    return result;
}

Affine translate(const Vector& v)
{
    Matrix result;
    result[0] = Hcoord{ 1, 0, 0, v.x };
    result[1] = Hcoord{ 0, 1, 0, v.y };
    result[2] = Hcoord{ 0, 0, 1, v.z };
    result[3] = Hcoord{ 0, 0, 0, 1 };
    return result;
}

Affine scale(float r)
{
    Matrix result;
    result[0] = Hcoord{ r, 0, 0, 0 };
    result[1] = Hcoord{ 0, r, 0, 0 };
    result[2] = Hcoord{ 0, 0, r, 0 };
    result[3] = Hcoord{ 0, 0, 0, 1 };
    return result;
}

Affine scale(float rx, float ry, float rz)
{
    Affine result;
    result[0] = Hcoord{ rx, 0, 0, 0 };
    result[1] = Hcoord{ 0, ry, 0, 0 };
    result[2] = Hcoord{ 0, 0, rz, 0 };
    result[3] = Hcoord{ 0, 0, 0, 1 };
    return result;
}

Affine inverse(const Affine& A)
{
    auto det2x2 = [](float a11, float a12, float a21, float a22)
    {
        return a11 * a22 - a12 * a21;
    };

    Matrix M;
    M[0] = Hcoord{ det2x2(A[1][1], A[1][2], A[2][1], A[2][2]), det2x2(A[1][0], A[1][2], A[2][0], A[2][2]), det2x2(A[1][0], A[1][1], A[2][0], A[2][1]), 0 };
    M[1] = Hcoord{ det2x2(A[0][1], A[0][2], A[2][1], A[2][2]), det2x2(A[0][0], A[0][2], A[2][0], A[2][2]), det2x2(A[0][0], A[0][1], A[2][0], A[2][1]), 0 };
    M[2] = Hcoord{ det2x2(A[0][1], A[0][2], A[1][1], A[1][2]), det2x2(A[0][0], A[0][2], A[1][0], A[1][2]), det2x2(A[0][0], A[0][1], A[1][0], A[1][1]), 0 };
    M[3] = Hcoord{ 0, 0, 0, 1 };

    Matrix C = M;
    C[0][1] = -C[0][1];
    C[1][0] = -C[1][0];
    C[1][2] = -C[1][2];
    C[2][1] = -C[2][1];

    const Vector A_first_row{ A[0].x, A[0].y, A[0].z };
    const Vector C_first_row{ C[0].x, C[0].y, C[0].z };
    const float det_A = dot(A_first_row, C_first_row);

    Matrix C_flip;
    C_flip[0] = Hcoord{ C[0][0], C[1][0], C[2][0], C[3][0] };
    C_flip[1] = Hcoord{ C[0][1], C[1][1], C[2][1], C[3][1] };
    C_flip[2] = Hcoord{ C[0][2], C[1][2], C[2][2], C[3][2] };
    C_flip[3] = Hcoord{ C[0][3], C[1][3], C[2][3], C[3][3] };

    Matrix inverse_A = (1 / det_A) * C_flip;
    inverse_A[3][3] = 1.f;

    Matrix inverse_T;
    inverse_T[0] = Hcoord{ 1, 0, 0, -A[0][3] };
    inverse_T[1] = Hcoord{ 0, 1, 0, -A[1][3] };
    inverse_T[2] = Hcoord{ 0, 0, 1, -A[2][3] };
    inverse_T[3] = Hcoord{ 0, 0, 0, 1 };

    return inverse_A * inverse_T;
}