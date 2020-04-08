//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
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

        //**********************************************************************
        // Constructor: Ray2
        // Default constructor for a 2D ray
        //**********************************************************************
        Ray2() {}

        //**********************************************************************
        // Constructor: Ray2
        // Constructor for a 2D from a point/direction vector
        // 
        // Parameters:
        // _pos - starting point for ray
        // _dir - direction of ray
        //**********************************************************************
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

        //**********************************************************************
        // Constructor: Ray3
        // Defines a 3D ray
        //**********************************************************************
        Ray3() {}

        //**********************************************************************
        // Constructor: Ray3
        // Defines a 3D ray
        // 
        // Parameters:
        // _pos - starting position for ray
        // _dir - direction of ray
        //**********************************************************************
        Ray3(Vector3 &_pos, Vector3 &_dir)
        {
            pos = _pos;
            dir = _dir;
        }

        //**********************************************************************
        // Method: Intersect
        // Computes intersection of ray with 3D bounding box
        // 
        // Parameters:
        // bbox - BBox to intersect against
        // pInvTm - Matrix to convert from ray's coordinate space to unit space (generally world=>object)
        // pIntersectInfo - Results of intersection test. Maybe nullptr.
        //
        // Returns:
        // True if ray intersects bounding box. False otherwise.
        //**********************************************************************
        bool Intersect(const BBox3 &bbox, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
      
	//**********************************************************************
	// Method: Intersect
        // Calculates intersection of a ray with a cone
	//
	// Calculates the intersection of a ray with a cone. The cone is defined
	// by a top and bottom radius (which maybe something other than 0.0f and
	// 1.0f resulting in a tapered cone). The equation for the cone is:
	//
        // --- Text ---
	//        b = bottom radius
	//        t = top radius
	//        h = height of cone
	//        x^2 + y^2 = (((z + 1) * t - (z - 1) * b) / 2)^2
        // ---
	//
	// Parameters:
	// bottomRadius - Radius of the bottom of the cone
	// topRadius - Radius of the top of the cone
	// height - Height of cone
	// pInvTm - Matrix to convert from ray's coordinate space to unit space (generally world=>object)
	// pIntersectInfo - Returns the interesection info. Maybe nullptr.
        //
        // Returns:
        // True if ray intersects cone. False otherwise.
	//**********************************************************************
        bool Intersect(float bottomRadius, float topRadius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
      
	//**********************************************************************
	// Method: Intersect
        // Calculates the intersection of a ray with an sphere. If pIntersectInfo
	// is not nullptr then the intersection data is returned via this pointer.
        //
        // Parameters:
	// radius - Radius of sphere
	// pInvTm - Matrix to convert from ray's coordinate space to unit space (generally world=>object)
	// pIntersectInfo - Returns the interesection info. Maybe nullptr.
        //
        // Returns:
	// True if ray interesects bbox. False otherwise.
	//**********************************************************************
        bool Intersect(float radius, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
      
	//**********************************************************************
	// Method: Intersect
	// Calculates the intersection of a ray with an cylinder. If pIntersectInfo
	// is not nullptr then the intersection data is returned via this pointer.
        //
        // Parameters:
	// radius - Radius of top/bottom of cylinder
	// height - Height of cylinder
	// pInvTm - Matrix to convert from ray's coordinate space to unit space (generally world=>object)
	// pIntersectInfo - Returns the interesection info. Maybe nullptr.
        //
        // Returns:
	// \return true if ray interesects bbox, otherwise false
	//**********************************************************************
        bool Intersect(float radius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo);
    };
}
