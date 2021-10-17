//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Math.Point;
#include <limits>

namespace Caustic
{
    //**********************************************************************
    // Struct: IBBox2
    // Defines a integer axis-aligned bounding box (in 2D)
    //
    // Members:
    // <Point2> p1 - Minimum point defining one coroner of the bbox
    // <Point2> p2 - Maximum point defining one coroner of the bbox
    //
    // Header:
    // {Link:#include "Base/Math/IBBox.h"{Base/Math/IBBox.h}}
    //**********************************************************************
    struct IBBox2
    {
        Point2 p1, p2;

        //**********************************************************************
        // Constructor: IBBox2
        // Default constructor
        //**********************************************************************
        IBBox2()
        {
            p1 = Point2(_CRT_INT_MAX, _CRT_INT_MAX);
            p2 = Point2(-_CRT_INT_MAX, -_CRT_INT_MAX);
        }

        IBBox2(Point2& topleft, Point2& bottomright)
        {
            p1 = topleft;
            p2 = bottomright;
        }

        IBBox2(int32 topleftx, int32 toplefty, int32 bottomrightx, int32 bottomrighty) :
            p1(topleftx, toplefty),
            p2(bottomrightx, bottomrighty)
        {
        }

        //**********************************************************************
        // Constructor: IBBox2
        // Constructs a <IBBox2> from another <IBBox2>
        //
        // Parameters:
        // bbox - Bounding box to construct from
        //**********************************************************************
        IBBox2(const IBBox2& bbox)
        {
            p1 = bbox.p1;
            p2 = bbox.p2;
        }

        //**********************************************************************
        // Method: Empty
        // Returns:
        // Returns true if bbox is empty. False otherwise.
        //**********************************************************************
        bool Empty()
        {
            if (p1.x == _CRT_INT_MAX && p1.y == _CRT_INT_MAX &&
                p2.x == -_CRT_INT_MAX && p2.y == -_CRT_INT_MAX)
                return true;
            return false;
        }

        //**********************************************************************
        // Method: AddPoint
        // Adds a point to a bbox.
        //
        // Parameters:
        // p - point to add
        //**********************************************************************
        void AddPoint(const Point2& p)
        {
            if (p.x < p1.x)
                p1.x = p.x;
            if (p.x > p2.x)
                p2.x = p.x;
            if (p.y < p1.y)
                p1.y = p.y;
            if (p.y > p2.y)
                p2.y = p.y;
            return;
        }

        //**********************************************************************
        // Method: AddPoint
        // Adds a point to a bbox.
        //
        // Parameters:
        // x - X coordinate of point to add
        // y - Y coordinate of point to add
        //**********************************************************************
        void AddPoint(int32 x, int32 y)
        {
            if (x < p1.x)
                p1.x = x;
            if (x > p2.x)
                p2.x = x;
            if (y < p1.y)
                p1.y = y;
            if (y > p2.y)
                p2.y = y;
            return;
        }

        //**********************************************************************
        // Method: PointInside
        // Test if a specified point is inside the bounding box
        //
        // Parameters:
        // p - Point to test
        //
        // Returns:
        // Returns true if point is inside bounding box. False otherwise.
        //**********************************************************************
        bool PointInside(const Point2& p)
        {
            return (p.x >= p1.x && p.x <= p2.x &&
                    p.y >= p1.y && p.y <= p2.y) ? true : false;
        }

        //**********************************************************************
        // Method: PointInside
        // Test if a specified point within some specified tolerance is inside the bounding box
        //
        // Parameters:
        // p - Point to test
        // tolerance - amount to expand bounding box by for test purposes
        //
        // Returns:
        // Returns true if point is inside bounding box. False otherwise.
        //**********************************************************************
        bool PointInside(const Point2& p, int32 tolerance)
        {
            return (p.x >= p1.x - tolerance && p.x <= p2.x + tolerance &&
                    p.y >= p1.y - tolerance && p.y <= p2.y + tolerance) ? true : false;
        }

        //**********************************************************************
        // Method: Normalize
        // Adjusts the coordinates of the bbox such that the minimum X and Y
        // are in p1 and the maximum X and Y are in coordinate p2
        //**********************************************************************
        void Normalize()
        {
            if (p1.x > p2.x)
            {
                int32 tmp = p1.x;
                p1.x = p2.x;
                p2.x = tmp;
            }
            if (p1.y > p2.y)
            {
                int32 tmp = p1.y;
                p1.y = p2.y;
                p2.y = tmp;
            }
            return;
        }

        //**********************************************************************
        // Method: Intersection
        // Computes the intersection of two bounding boxes
        //
        // Parameters:
        // bbox - Bounding box to intersect against
        // result - Returns the resulting bbox intersection
        //
        // Returns:
        // Returns true if bounding boxes intersect. False otherwise.
        //**********************************************************************
        bool Intersection(IBBox2& bbox, IBBox2* pResult)
        {
            IBBox2 b1, b2;

            b1 = *this;
            b2 = bbox;
            b1.Normalize();
            b2.Normalize();
            if (b2.p2.x < b1.p1.x || b2.p1.x > b1.p2.x || b2.p2.y < b1.p1.y || b2.p1.y > b1.p2.y)
                return false;
            if (pResult)
            {
                pResult->p1.x = (b2.p1.x > b1.p1.x) ? b2.p1.x : b1.p1.x;
                pResult->p1.y = (b2.p1.y > b1.p1.y) ? b2.p1.y : b1.p1.y;
                pResult->p2.x = (b2.p2.x < b1.p2.x) ? b2.p2.x : b1.p2.x;
                pResult->p2.y = (b2.p2.y < b1.p2.y) ? b2.p2.y : b1.p2.y;
            }
            return true;
        }

        //**********************************************************************
        // Method: LineIntersects
        // Determines whether the specified line intersects the bounding box.
        //
        // Parameters:
        // _p1 - first point of line segment to test against
        // _p2 - second point of line segment to test against
        //
        // Returns:
        // Returns true if line intersects bounding box. False otherwise.
        //**********************************************************************
        bool LineIntersects(Point2& _p1, Point2& _p2)
        {
            Point2 b1, b2;
            int numer, denom;
            float t;

            if ((_p1.x >= p1.x && _p1.x <= p2.x && _p1.y >= p1.y && _p1.y <= p2.y) ||
                (_p2.x >= p1.x && _p2.x <= p2.x && _p2.y >= p1.y && _p2.y <= p2.y))
                return true;
            //
            // Next check if line is trivially outside (completely to the left, right, top, or bottom)
            //
            if ((_p1.x < p1.x && _p2.x < p1.x) ||
                (_p1.x > p2.x && _p2.x > p2.x) ||
                (_p1.y < p1.y && _p2.y < p1.y) ||
                (_p1.y > p2.y && _p2.y > p2.y))
                return false;
            //
            // Line may be partially visible so clip against bbox.
            // First clip against left edge of box.
            //
            b1.x = p1.x; b1.y = p1.y;
            b2.x = p1.x; b2.y = p2.y;
            denom = -(b2.y - b1.y) * (_p2.x - _p1.x);
            if (denom != 0)
            {
                numer = (_p1.x - b1.x) * (_p2.y - _p1.y) - (_p1.y - b1.y) * (_p2.x - _p1.x);
                t = (float)numer / (float)denom;
                if (t >= 0.0F && t <= 1.0F)
                    return true;
            }
            //
            // Clip against top edge
            //
            b1.x = p1.x; b1.y = p1.y;
            b2.x = p2.x; b2.y = p1.y;
            denom = (b2.x - b1.x) * (_p2.y - _p1.y);
            if (denom != 0)
            {
                numer = (_p1.x - b1.x) * (_p2.y - _p1.y) - (_p1.y - b1.y) * (_p2.x - _p1.x);
                t = (float)numer / (float)denom;
                if (t >= 0.0F && t <= 1.0F)
                    return true;
            }
            //
            // Clip against right edge
            //
            b1.x = p2.x; b1.y = p1.y;
            b2.x = p2.x; b2.y = p2.y;
            denom = -(b2.y - b1.y) * (_p2.x - _p1.x);
            if (denom != 0)
            {
                numer = (_p1.x - b1.x) * (_p2.y - _p1.y) - (_p1.y - b1.y) * (_p2.x - _p1.x);
                t = (float)numer / (float)denom;
                if (t >= 0.0F && t <= 1.0F)
                    return true;
            }
            //
            // Clip against bottom edge
            //
            b1.x = p1.x; b1.y = p2.y;
            b2.x = p2.x; b2.y = p2.y;
            denom = (b2.x - b1.x) * (_p2.y - _p1.y);
            if (denom != 0)
            {
                numer = (_p1.x - b1.x) * (_p2.y - _p1.y) - (_p1.y - b1.y) * (_p2.x - _p1.x);
                t = (float)numer / (float)denom;
                if (t >= 0.0F && t <= 1.0F)
                    return true;
            }
            return false;
        }
    };
}
