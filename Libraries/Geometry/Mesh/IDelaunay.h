//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

namespace Caustic
{
    //**********************************************************************
    // Interface: IDelaunay2
	// Defines the interface used for creating Delaunay triangulations
    // from a set of 2D points.
    //**********************************************************************
    struct IDelaunay2 : public IRefCount
    {
		//**********************************************************************
		// Method: Open
		// Opens the triangulation so points can be added
		//**********************************************************************
		virtual void Open() = 0;

		//**********************************************************************
		// Method: AddPoint
		// Opens the triangulation so points can be added
		//
		// Parameters:
		// pt - point to add to triangulation
		// uv - UV coordinates of the point
		// isBoundary - True if point is part of the triangulation boundary
		//**********************************************************************
		virtual void AddPoint(Vector2 &pt, Vector2 &uv, bool isBoundary) = 0;

		//**********************************************************************
		// Method: Close
		// Closes the triangulation and performs the Delaunay triangulation on
		// the set of points
		//**********************************************************************
		virtual void Close() = 0;

		//**********************************************************************
		// Method: GetNumberTriangles
		// Returns the number of triangles in the triangulation
		//**********************************************************************
		virtual uint32 GetNumberTriangles() = 0;

		//**********************************************************************
		// Method: GetTriangle
		// Returns the Nth triangle
		//
		// Parameters:
		// index - index of triangle to be retrieved
		// v0 - first point in triangle
		// v1 - second point in triangle
		// v2 - third point in triangle
		// isExterior - flags indicating whether the returned points are part of the
		// triangulations boundary polygon
		//**********************************************************************
		virtual void GetTriangle(uint32 index, Vector2 &v0, Vector2 &v1, Vector2 &v2, bool isExterior[3]) = 0;

		//**********************************************************************
		// Method: WritePLY
		// Helper function for writing the triangulation out as a .PLY file
		//**********************************************************************
		virtual void WritePLY() = 0;
    };

    CAUSTICAPI CRefObj<IDelaunay2> CreateDelaunay2(BBox2 &bb);
}
