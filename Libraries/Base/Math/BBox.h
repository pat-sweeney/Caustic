//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include <float.h>
#include "Base\Math\Vector.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Defines a axis-aligned bounding box (in 2D)
    //**********************************************************************
    struct BBox2
    {
        Vector2 minPt; //!< Minimum point defining one corner of the bbox
        Vector2 maxPt; //!< Maximum point defining one corner of the bbox

        BBox2();
        BBox2(Vector2 &topLeft, Vector2 &bottomRight);
        bool Empty();
        void AddPoint(float x, float y);
        void AddPoint(Vector2 &pt);
        bool PointInside(const Vector2 &p);
        bool PointInside(const Vector2 &p, float tolerance);
    };

    //**********************************************************************
    //! \brief Defines a axis-aligned bounding box (in 3D)
    //!
    //! The bounding box is assumed to be built as follows:
    //!
    //!           +---------------+ P2
    //!          /|              /|
    //!         / |             / |
    //!        /  |            /  |
    //!       /   |           /   |
    //!      +---------------+    |
    //!      |    |          |    |
    //!   ^  |    +----------|----+
    //!   |  |   /           |   / ^
    //!   |Y |  /            |  / /
    //!   |  | /             | / / Z
    //!   |  |/              |/ /
    //!      +---------------+ 
    //!    P1                
    //!        ---------->X
    //!
    //! This code also assumes that the bounding volume
    //! is aligned with the coordinate system axis.
    //**********************************************************************
    struct BBox3
    {
        Vector3 minPt; //!< Minimum point defining one corner of the bbox
        Vector3 maxPt; //!< Maximum point defining one corner of the bbox

        BBox3();
        BBox3(Vector3 &frontTopLeft, Vector3 &backBottomRight);
        bool Empty();
        void AddPoint(float x, float y, float z);
        void AddPoint(Vector3 &pt);
        bool PointInside(const Vector3 &p);
        bool PointInside(const Vector3 &p, float tolerance);
    };
}
