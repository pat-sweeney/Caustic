//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Base\Math\Helper.h"

export module Base.Math.Point;
import Base.Core.Core;

export namespace Caustic
{
    //**********************************************************************
    // Structure: Point2
    // Defines a simple point with 2 components
    //
    // Members:
    // int32 x - x component
    // int32 y - y component
    //
    // Module:
    // {Link:import Base.Math.Point;{Base/Math/Point.ixx}}
    //**********************************************************************
    struct Point2
    {
        int32 x, y;

        //**********************************************************************
        // Constructor: Point2
        // Default contructor
        //**********************************************************************
        Point2() {}

        //**********************************************************************
        // Constructor: Point2
        // Contructor for a 2D vector
        //
        // Parameters:
        // _x - X coordinate
        // _y - Y coordinate
        //**********************************************************************
        Point2(int32 _x, int32 _y) { x = _x; y = _y; }

        //**********************************************************************
        // Method: IsEq
        // Returns whether two points are equivalent
        //
        // Parameters:
        // p - Point to compare against
        //
        // Returns:
        // True if points are equivalent. False otherwise.
        //**********************************************************************
        bool IsEq(const Point2& p)
        {
            if (p.x == x && p.y == y)
                return true;
            return false;
        }

        //**********************************************************************
        // Method: Length
        // Returns the L2 norm for the vector
        //
        // Returns:
        // Length of vector
        //**********************************************************************
        uint32 Length() { return isqrt((uint32)(x * x + y * y)); }

        Point2 operator-() { return Point2(-x, -y); }
        Point2 operator-(Point2& rhs) { return Point2(x - rhs.x, y - rhs.y); }
        Point2 operator+(Point2& rhs) { return Point2(x + rhs.x, y + rhs.y); }
        Point2 operator*(int f) { return Point2(x * f, y * f); }
        bool operator==(Point2& rhs) { return this->IsEq(rhs); }
        bool operator!=(Point2& rhs) { return !this->IsEq(rhs); }
    };
}