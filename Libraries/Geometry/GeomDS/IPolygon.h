//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.IRefCount;
import Base.Core.Core;
import Base.Math.Vector;
import Base.Math.BBox;

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
		//
		// Returns:
		// Returns the new simplified polygon
		//**********************************************************************
		virtual CRefObj<IPolygon2> Simplify(float err, float maxLen) = 0;

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

    CRefObj<IPolygon2> CreatePolygon2();
}
