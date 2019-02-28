//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"
#include "Base\Math\Matrix.h"

namespace Caustic
{
	//**********************************************************************
	// Structure: RayIntersect2
	// Defines results of a 2D ray intersection
	//**********************************************************************
	struct RayIntersect2
    {
        float hitTime;      // Value of 't' on line equation where ray intersects line
        Vector2 hitPt;      // Point on line where ray intersects
        float hitTimeRay;   // Value of 'u' on ray equation where ray intersects line
        Vector2 hitPtRay;   // Point on ray where line intersects
    };

	//**********************************************************************
	// Structure: Ray2
	// Defines a 2 dimensional ray
	//**********************************************************************
    struct Ray2
    {
        Vector2 pos;
        Vector2 dir;

        Ray2() {}
        Ray2(Vector2 &_pos, Vector2 &_dir)
        {
            pos = _pos;
            dir = _dir;
        }

		//**********************************************************************
		// Method: IntersectWithLine
		// Calculates the intersection of a 2D ray with a line.
		// --- Text ---
		//     R.pos + R.dir * t = p0 + (p1 - p0) * u
		// taking cross product of R.dir with both sides gives:
		//     (R.pos + R.dir * t) x R.dir = (p0 + (p1 - p0) * u) x R.dir
		//     ((p1 - p0) * u x R.dir) = (R.pos - p0) x R.dir
		// given that (R.dir * t) x R.dir == 0
		//     u = ((R.pos - p0) x R.dir) / ((p1 - p0)x R.dir)
		//     t = -(R.pos - p0) x(p1 - p0) / (-(p1 - p0) xR.dir)
		// ---
		//
		// Parameters:
		// p0 - start of line segment
		// p1 - end of line segment
		// pIntersectInfo - Returns intersection results
		//
		// Returns:
		// Returns true if ray intersects line. False otherwise.
		//**********************************************************************
		bool IntersectWithLine(Vector2 &p0, Vector2 &p1, RayIntersect2 *pIntersectInfo);
    };

	//**********************************************************************
	// Structure: RayIntersect3
	// Defines results of a 3D ray intersection
	//**********************************************************************
	struct RayIntersect3
    {
        float hitTime;
        Vector3 hitPt;
        Vector3 hitNorm;
    };

    //**********************************************************************
    // Structure: Ray3
	// Defines a 3 dimensional ray
	//**********************************************************************
    struct Ray3
    {
        Vector3 pos;
        Vector3 dir;

        Ray3() {}
        Ray3(Vector3 &_pos, Vector3 &_dir)
        {
            pos = _pos;
            dir = _dir;
        }

        bool Intersect(const BBox3 &bbox, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
        bool Intersect(float bottomRadius, float topRadius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
        bool Intersect(float radius, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
        bool Intersect(float radius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
    };
}
