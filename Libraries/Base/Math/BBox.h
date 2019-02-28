//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include <float.h>
#include "Base\Math\Vector.h"

namespace Caustic
{
    //**********************************************************************
    // Struct: BBox2
	// Defines a axis-aligned bounding box (in 2D)
    //**********************************************************************
    struct BBox2
    {
        Vector2 minPt; // Minimum point defining one corner of the bbox
        Vector2 maxPt; // Maximum point defining one corner of the bbox

        BBox2();
        BBox2(Vector2 &topLeft, Vector2 &bottomRight);
        bool Empty();
        void AddPoint(float x, float y);
        void AddPoint(Vector2 &pt);
        bool PointInside(const Vector2 &p);
        bool PointInside(const Vector2 &p, float tolerance);
    };

    //**********************************************************************
    // Struct: BBox3
	// Defines a axis-aligned bounding box (in 3D)
    //
    // The bounding box is assumed to be built as follows:
    //--- Text ------------------------------------
    //           +---------------+ P2
    //          /|              /|
    //         / |             / |
    //        /  |            /  |
    //       /   |           /   |
    //      +---------------+    |
    //      |    |          |    |
    //   ^  |    +----------|----+
    //   |  |   /           |   / ^
    //   |Y |  /            |  / /
    //   |  | /             | / / Z
    //   |  |/              |/ /
    //      +---------------+ 
    //    P1                
    //        ---------->X
	//-------------------------------------------
	//
    // This code also assumes that the bounding volume
    // is aligned with the coordinate system axis.
    //**********************************************************************
    struct BBox3
    {
        Vector3 minPt; // Minimum point defining one corner of the bbox
        Vector3 maxPt; // Maximum point defining one corner of the bbox

		//**********************************************************************
		// Constructor: BBox3
		// Default constructor
		//**********************************************************************
		BBox3();

		//**********************************************************************
		// Constructor: BBox3
		//
		// Parameters:
		// frontTopLeft - top left corner for bounding box
		// backBottomRight - lower right corner for bounding box
		//**********************************************************************
		BBox3(Vector3 &frontTopLeft, Vector3 &backBottomRight);

		//**********************************************************************
		// Method: Empty
		// Determines if bbox is empty
		//
		// Returns:
		// Returns true if bbox is empty. False otherwise.
		//**********************************************************************
		bool Empty();

		//**********************************************************************
		// Method: AddPoint
		// Adds a point to a bounding box. If the point is outside the bbox, the bbox
		// is updated.
		//
		// Parameters:
		// x - X coordinate
		// y - Y coordinate
		// z - Z coordinate
		//**********************************************************************
		void AddPoint(float x, float y, float z);

		//**********************************************************************
		// Method: AddPoint
		// Adds a point to a bounding box. If the point is outside the bbox, the bbox
		// is updated.
		//
		// Parameters:
		// pt - point
		//**********************************************************************
		void AddPoint(Vector3 &pt);

		//**********************************************************************
		// Method: PointInside
		// Tests if the specified point is inside the bounding box
		//
		// Parameters:
		// p - point to test
		//
		// Returns:
		// True if point is inside bbox. False otherwise.
		//**********************************************************************
		bool PointInside(const Vector3 &p);

		//**********************************************************************
		// Method: PointInside
		// Tests if the specified point +/- tolerance is inside the bounding box
		//
		// Parameters:
		// p - point to test
		// tolerance - Tolerance
		//
		// Returns:
		// True if point is inside bbox. False otherwise.
		//**********************************************************************
		bool PointInside(const Vector3 &p, float tolerance);
    };
}
