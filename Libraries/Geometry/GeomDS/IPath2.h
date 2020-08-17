//**********************************************************************
// Copyright Patrick Sweeney 1995-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Math\vector.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
	enum EPathElem
	{
		Null,
		MoveTo,
		LineTo,
		CurveTo,
		ClosePath,
		SmoothClosePath
	};

	struct PathElem
	{
		Caustic::EPathElem elemtype;
		Vector2 pt[3];
	public:
		Caustic::EPathElem ElemType() { return elemtype; }
		Caustic::EPathElem ElemType() const { return elemtype; }
		const Vector2* Point() { return &pt[0]; }
		const Vector2* Point() const { return &pt[0]; }
		const Vector2* ControlPoint(int index) { return &pt[index]; }
		const Vector2* ControlPoint(int index) const { return &pt[index]; }
	};

	//**********************************************************************
	// Interface: IPath2
	// Defines a 2D Path (collection of bezier curves and lines)
	//**********************************************************************
	struct IPath2 : public IRefCount
	{
		//**********************************************************************
		// Method: GetNumerElems
		// Returns the number of PathElems in the path
		//
		// Returns:
		// Number of PathElems comprising the path
		//**********************************************************************
		virtual uint32 GetNumberElems() = 0;

		//**********************************************************************
		// Method: GetPathElem
		// Returns the Ith path element in the path
		//
		// Returns:
		// Pointer to the Ith path element
		//**********************************************************************
		virtual const PathElem* GetElement(int indx) = 0;

		//**********************************************************************
		// Method: GetBBox
		// Returns the path's bounding box. The path's bounding box includes
		// the convex hull of any curves. For a tighter fit first flatten the path
		// via [IPath2::Flatten].
		//
		// Returns:
		// Path's bounding box
		//**********************************************************************
		virtual BBox2 GetBBox() = 0;

		//**********************************************************************
		// Method: AddPathElem
		// Ads a new path element to the end of the path
		//
		// Parameters:
		// elemType - type of element to add
		// p0-p3 - points to use. These are dependent on the type of element requested via 'elemType'
		//**********************************************************************
		virtual void AddPathElem(EPathElem elemType, Vector2* p0 = nullptr, Vector2* p1 = nullptr, Vector2* p2 = nullptr) = 0;

		//**********************************************************************
		// Method: AppendPathElems
		// Appends a series of new path elements to the end of the path
		//
		// Parameters:
		// elemType - type of element to add
		// ... - varargs of elemType, x, y, ..., elemType, x, y, ...
		//   Each vararg elemType is expected to be followed by the correct number
		// of x,y coordinate pairs for the specified element type.
		// For instance the following call is valid:
		//    AppendPathElem(EPathElem::MoveTo, 0.0f, 0.0f, EPathElem::CurveTo, 0.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f, EPathElem::ClosePath, EPathElem::Null);
		//**********************************************************************
		virtual void AppendPathElems(EPathElem elemType, ...) = 0;
		
		//**********************************************************************
		// Method: Flatten
		// Flattens the path. This results in a path that only contains line segments
		//
		// Parameters:
		// flatness - tolerance for what is considered flat
		// bbox - returns the new path's bounding box
		//
		// Returns:
		// Pointer to the Ith path element
		//**********************************************************************
		virtual CRefObj<IPath2> Flatten(float flatness, BBox2* bb = nullptr) = 0;

		//**********************************************************************
		// Method: PointOnPath
		// Determines whether a 2D point lies on the path (within some tolerance)
		//
		// Parameters:
		// pt - Point to test
		// closestpt - returns the closest point
		// tolerance - tolerance
		//
		// Returns:
		// True if point is on path with given tolerance. False otherwise.
		//**********************************************************************
		virtual bool PointOnPath(Vector2& pt, Vector2 * closestpt, float tolerance) = 0;

		//**********************************************************************
		// Method: PathInsideBbox
		// Determines whether the path lies within the specified bounding box.
		//
		// Parameters:
		// bbox - bounding box to test if path is inside of
		//
		// Returns:
		// True if path is inside specified bounding box. False otherwise.
		//**********************************************************************
		virtual bool PathInsideBbox(BBox2& bbox) = 0;
	};

	CAUSTICAPI CRefObj<IPath2> CreatePath2();
}
