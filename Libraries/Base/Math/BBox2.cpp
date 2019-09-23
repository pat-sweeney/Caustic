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
    BBox2::BBox2() :
        minPt(FLT_MAX, FLT_MAX),
        maxPt(-FLT_MAX, -FLT_MAX)
    {
    }

    BBox2::BBox2(Vector2 &topLeft, Vector2 &bottomRight)
    {
        minPt = topLeft;
        maxPt = bottomRight;
    }

    bool BBox2::Empty()
    {
        if (minPt.x == FLT_MAX && minPt.y == FLT_MAX &&
            maxPt.x == -FLT_MAX && maxPt.y == -FLT_MAX)
            return true;
        return false;
    }

    void BBox2::AddPoint(float x, float y)
    {
        if (x < minPt.x)
            minPt.x = x;
        if (x > maxPt.x)
            maxPt.x = x;
        if (y < minPt.y)
            minPt.y = y;
        if (y > maxPt.y)
            maxPt.y = y;
    }

    void BBox2::AddPoint(Vector2 &pt)
    {
        if (pt.x < minPt.x)
            minPt.x = pt.x;
        if (pt.x > maxPt.x)
            maxPt.x = pt.x;
        if (pt.y < minPt.y)
            minPt.y = pt.y;
        if (pt.y > maxPt.y)
            maxPt.y = pt.y;
    }

    bool BBox2::PointInside(const Vector2 &p)
    {
        return (p.x >= minPt.x && p.x <= maxPt.x &&
            p.y >= minPt.y && p.y <= maxPt.y) ? true : false;
    }

    bool BBox2::PointInside(const Vector2 &p, float tolerance)
    {
        return (p.x >= minPt.x - tolerance && p.x <= maxPt.x + tolerance &&
            p.y >= minPt.y - tolerance && p.y <= maxPt.y + tolerance) ? true : false;
    }
}
