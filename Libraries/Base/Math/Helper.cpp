//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    //**********************************************************************
    // Function: Determinant2x2
    // Computes the determinant of a 2x2matrix.
    //**********************************************************************
    float Determinant2x2(
        float a00, float a01,
        float a10, float a11)
    {
        return a00 * a11 - a10 * a01;
    }

    //**********************************************************************
    // Function: Determinant3x3
    // Computes the determinant of a 3x3 matrix.
    // This is a temporary function. At some point I should just add
    // general support for 3x3 matrices, but would rather have this simple
    // function instead of a half implemented Matrix3x3 class.
    //**********************************************************************
    float Determinant3x3(
        float a00, float a01, float a02,
        float a10, float a11, float a12,
        float a20, float a21, float a22)
    {
        return a00 * Determinant2x2(a11, a12, a21, a22) - a01 * Determinant2x2(a10, a12, a20, a22) + a02 * Determinant2x2(a10, a11, a20, a21);
    }

    //**********************************************************************
    // Function: CircumCircle
    // Computes the circumcircle for a given triangle.
    // For complete details on the math see: http://mathworld.wolfram.com/Circumcircle.html
    //**********************************************************************
    void CircumCircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 *pCircumCenter, float *pRadius)
    {
        float p0dotp0 = p0.Dot(p0);
        float p1dotp1 = p1.Dot(p1);
        float p2dotp2 = p2.Dot(p2);
        float a = Determinant3x3(
            p0.x, p0.y, 1.0f,
            p1.x, p1.y, 1.0f,
            p2.x, p2.y, 1.0f);
        float Bx = -Determinant3x3(
            p0dotp0, p0.y, 1.0f,
            p1dotp1, p1.y, 1.0f,
            p2dotp2, p2.y, 1.0f);
        float By = Determinant3x3(
            p0dotp0, p0.x, 1.0f,
            p1dotp1, p1.x, 1.0f,
            p2dotp2, p2.x, 1.0f);
        float c = -Determinant3x3(
            p0dotp0, p0.x, p0.y,
            p1dotp1, p1.x, p1.y,
            p2dotp2, p2.x, p2.y);
        pCircumCenter->x = -Bx / (2 * a);
        pCircumCenter->y = -By / (2 * a);
        *pRadius = sqrtf(Bx * Bx + By * By - 4 * a * c) / (2 * (float)fabs(a));
    }

    //**********************************************************************
    // Function: PointInTriangleCircumcircle
    // Tests whether the specified point
    // is inside the circumcircle for the given triangle.
    //
    // Parameters:
    // p0 - First point in the triangle
    // p1 - Second point in the triangle
    // p2 - Third point in the triangle
    // pt - Point to test
    //
    // Returns:
    // True if point is inside the circumcircle
    //**********************************************************************
    bool PointInTriangleCircumcircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &pt)
    {
        Vector2 center;
        float radius;
        CircumCircle(p0, p1, p2, &center, &radius);
        float dist = (pt - center).Length();
        return (dist <= radius) ? true : false;
    }
}
