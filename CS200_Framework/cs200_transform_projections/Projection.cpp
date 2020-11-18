// Name       : Yoonki Kim
// Assignment : Transform Projection Programming Assignment
// Course     : CS200
// Term & Year: Fall 2020
#include "Projection.h"

Affine CameraToWorld(const Camera& cam)
{
    const Vector right = cam.Right();
    const Vector up = cam.Up();
    const Vector back = cam.Back();
    const Point pos = cam.Eye();
    return Affine(right, up, back, pos);
}

Affine WorldToCamera(const Camera& cam)
{
    return inverse(CameraToWorld(cam));
}

Matrix CameraToNDC(const Camera& cam)
{
    const Vector cam_geo = cam.ViewportGeometry();
    const float width = cam_geo.x;
    const float height = cam_geo.y;
    const float distance = cam_geo.z;

    const float near = cam.NearDistance();
    const float far = cam.FarDistance();

    Matrix cam_to_NDC;
    cam_to_NDC[0] = Hcoord{ 2 * distance / width, 0, 0, 0 };
    cam_to_NDC[1] = Hcoord{ 0, 2 * distance / height, 0, 0 };
    cam_to_NDC[2] = Hcoord{ 0, 0, (near + far) / (near - far), (2 * near * far) / (near - far) };
    cam_to_NDC[3] = Hcoord{ 0, 0, -1, 0 };

    return cam_to_NDC;
}
