//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Defines the interface used for creating Delaunay triangulations
    //! from a set of 2D points.
    //**********************************************************************
    struct IDelaunay2 : public IRefCount
    {
        virtual void AddPoint(Vector2 &pt) = 0;
        virtual uint32 GetNumberTriangles() = 0;
        virtual void GetTriangle(uint32 index, Vector2 &v0, Vector2 &v1, Vector2 &v2) = 0;
        virtual void Open() = 0;
        virtual void Close() = 0;
    };

    CAUSTICAPI void CreateDelaunay2(IDelaunay2 **ppDelaunay, BBox2 &bb);
}
