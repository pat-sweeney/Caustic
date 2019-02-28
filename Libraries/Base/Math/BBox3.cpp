//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    BBox3::BBox3() :
        minPt(FLT_MAX, FLT_MAX, FLT_MAX),
        maxPt(-FLT_MAX, -FLT_MAX, -FLT_MAX)
    {
    }

    BBox3::BBox3(Vector3 &frontTopLeft, Vector3 &backBottomRight)
    {
        minPt = frontTopLeft;
        maxPt = backBottomRight;
    }

    bool BBox3::Empty()
    {
        if (minPt.x == FLT_MAX && minPt.y == FLT_MAX && minPt.z == FLT_MAX &&
            maxPt.x == -FLT_MAX && maxPt.y == -FLT_MAX && maxPt.z == -FLT_MAX)
            return true;
        return false;
    }

    void BBox3::AddPoint(float x, float y, float z)
    {
        if (x < minPt.x)
            minPt.x = x;
        if (x > maxPt.x)
            maxPt.x = x;
        if (y < minPt.y)
            minPt.y = y;
        if (y > maxPt.y)
            maxPt.y = y;
        if (z < minPt.z)
            minPt.z = z;
        if (z > maxPt.z)
            maxPt.z = z;
    }

    void BBox3::AddPoint(Vector3 &pt)
    {
        if (pt.x < minPt.x)
            minPt.x = pt.x;
        if (pt.x > maxPt.x)
            maxPt.x = pt.x;
        if (pt.y < minPt.y)
            minPt.y = pt.y;
        if (pt.y > maxPt.y)
            maxPt.y = pt.y;
        if (pt.z < minPt.z)
            minPt.z = pt.z;
        if (pt.z > maxPt.z)
            maxPt.z = pt.z;
    }

    bool BBox3::PointInside(const Vector3 &p)
    {
        return (p.x >= minPt.x && p.x <= maxPt.x &&
            p.y >= minPt.y && p.y <= maxPt.y &&
            p.z >= minPt.z && p.z <= maxPt.z) ? true : false;
    }

    bool BBox3::PointInside(const Vector3 &p, float tolerance)
    {
        return (p.x >= minPt.x - tolerance && p.x <= maxPt.x + tolerance &&
            p.y >= minPt.y - tolerance && p.y <= maxPt.y + tolerance &&
            p.z >= minPt.z - tolerance && p.z <= maxPt.z + tolerance) ? true : false;
    }
}
