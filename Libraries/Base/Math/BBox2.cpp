//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <float.h>

module Base.Math.BBox;
import Base.Core.Core;

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

    bool BBox2::LineInside(const Vector2& p1, const Vector2& p2)
    {
		//
		// First check if either end point is within the bounding box
		//
		if (PointInside(p1) || PointInside(p2))
			return true;

		//
		// Next check if line is trivially outside (completely to the left, right, top, or bottom)
		//
		if ((p1.x < minPt.x && p2.x < minPt.x) ||
			(p1.x > maxPt.x && p2.x > maxPt.x) ||
			(p1.y < minPt.y && p2.y < minPt.y) ||
			(p1.y > maxPt.y && p2.y > maxPt.y))
			return false;

		//
		// Line may be partially visible so clip against bbox.
		// First clip against left edge of box.
		//
		float numer = (p1.x - minPt.x) * (p2.y - p1.y) - (p1.y - minPt.y) * (p2.x - p1.x);
		float denom = -(maxPt.y - minPt.y) * (p2.x - p1.x);
		if (!Caustic::IsZero(denom))
		{
			float t = numer / denom;
			if (t >= 0.0F && t <= 1.0F)
				return true;
		}

		//
		// Clip against top edge
		//
		denom = (maxPt.x - minPt.x) * (p2.y - p1.y);
		if (!Caustic::IsZero(denom))
		{
			float t = numer / denom;
			if (t >= 0.0F && t <= 1.0F)
				return true;
		}

		//
		// Clip against right edge
		//
		denom = -(maxPt.y - minPt.y) * (p2.x - p1.x);
		if (!Caustic::IsZero(denom))
		{
			float t = numer / denom;
			if (t >= 0.0F && t <= 1.0F)
				return true;
		}

		//
		// Clip against bottom edge
		//
		denom = (maxPt.x - minPt.x) * (p2.y - p1.y);
		if (!Caustic::IsZero(denom))
		{
			float t = numer / denom;
			if (t >= 0.0F && t <= 1.0F)
				return true;
		}
		return false;
	}
}
