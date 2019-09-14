//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Math\vector.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
	//**********************************************************************
	// Interface: IPolygon2
	// Defines a 2D polygon
	//**********************************************************************
	struct IPolygon2 : public IRefCount
    {
		//**********************************************************************
		// Method: GetNumberPoints
		// Returns the number of points in the polygon
		//**********************************************************************
		virtual uint32 GetNumberPoints() = 0;

		//**********************************************************************
		// Method: GetPoint
		// Returns the requested point (by index)
		//
		// Parameters:
		// index - index of point to get
		// pt - Returns the found point
		//**********************************************************************
		virtual void GetPoint(uint32 index, Vector2 *pt) = 0;

		//**********************************************************************
		// Method: AddPoint
		// Adds a point to the polygon
		//
		// Parameters:
		// pt - Point to add
		//**********************************************************************
		virtual void AddPoint(Vector2 &pt) = 0;

		//**********************************************************************
		// Method: GetBBox
		// Returns the 2D bounding box of the polygon
		//
		// Parameters:
		// pBBox - Returns the polygon's bounding box
		//**********************************************************************
		virtual void GetBBox(BBox2 *pBBox) = 0;

		//**********************************************************************
		// Method: Simplify
		// Removes points from a polygon that allow the resulting
		// polygon to still be with 'err' units from the original polygon.
		// This function uses the Ramer-Douglas-Peucker algorithm:
        //
		// <https://en.wikipedia.org/wiki/Ramer–Douglas–Peucker_algorithm>
		//
		// Parameters:
		// err - Tolerance for deciding whether to remove a point
		// maxLen - Longest line segment allowed. If maxLen==0 then no limit.
		// ppResult - The new simplified polygon
		//**********************************************************************
		virtual void Simplify(float err, float maxLen, IPolygon2 **ppResult) = 0;

		//**********************************************************************
		// Method: ContainsPoint
		// Determines whether the specified point is inside the polygon.
		//
		// Parameters:
		// pos - Point to rest
		// pMinDist - Optional argument that returns the closest distance from the point to the polygon
		// pMaxDist - Optional argument that returns the furthest distance from the point to the polygon (Hausdorff distance)
		//**********************************************************************
		virtual bool ContainsPoint(Vector2 &pt, float *pMinDist = nullptr, float *pMaxDist = nullptr) = 0;
    };

    extern void CreatePolygon2(IPolygon2 **ppPolygon);
}
