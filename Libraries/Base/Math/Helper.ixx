//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Base.Math.Helper;
import Base.Core.Core;
import Base.Math.BBox;
import Base.Math.Vector;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Function: isqrt
    // Computes the sqrt of an integer via Newtons method without using floating point.
    // See https://en.wikipedia.org/wiki/Integer_square_root for details.
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    inline uint32 isqrt(uint32 n)
    {
        uint32 next = n >> 1;
        uint32 cur;

        if (n <= 1)
            return n;
        while (true)
        {
            cur = next;
            next = (next + n / next) >> 1;
            if (next >= cur)
                break;
        }
        return cur;
    }

    //**********************************************************************
    // Function: DistancePointToLine
    // Determines the distance of a given point from a line
    //
    // Parameters:
    // p0 - first point defining the line segment
    // p1 - second point defining the line segment
    // pt - point to test
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    float DistancePointToLine(Caustic::Vector2& p0, Caustic::Vector2& p1, Caustic::Vector2& pt);

    //**********************************************************************
    // Function: Determinant2x2
    // Computes the determinant of a 2x2 matrix.
    //
    // Parameters:
    // Matrix is defined as:
    // ---- Text ---
    //  | a00 a01 |
    //  | a10 a11 |
    // -------------
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    extern float Determinant2x2(float a00, float a01, float a10, float a11);

    //**********************************************************************
    // Function: Determinant3x3
    // Computes the determinant of a 3x3 matrix.
    //
    // Parameters:
    // Matrix is defined as:
    // ---- Text ---
    //  | a00 a01 a02 |
    //  | a10 a11 a12 |
    //  | a20 a21 a22 |
    // -------------
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    extern float Determinant3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);

    //**********************************************************************
    // Function: CircumCircle
    // Given 3 points of a triangle this function will compute the center and radius of the circle passing through those points
    //
    // Parameters:
    // p0 - first point on circle
    // p1 - second point on circle
    // p2 - third point on circle
    // pCircumCenter - returns the center of the circumcircle
    // pRadius - returns the radius of the circumcircle
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    extern void CircumCircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 *pCircumCenter, float *pRadius);

    //**********************************************************************
    // Function: PointInTriangleCircumcircle
    // Tests if a point is inside the specified circumcircle
    //
    // Parameters:
    // p0 - first point on circle
    // p1 - second point on circle
    // p2 - third point on circle
    // pt - Point to test
    //
    // Returns:
    // True if point is in circumcircle. False otherwise.
    //
    // Header:
    // {Link:#include "Base/Math/Helper.h"{Base/Math/Helper.h}}
    //**********************************************************************
    extern bool PointInTriangleCircumcircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &pt);
}
