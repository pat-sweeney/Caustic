//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Base\Core\RefCount.h"
#include "Base\Math\Vector.h"
#include "IDelaunay.h"
#include <vector>

namespace Caustic
{
    struct Triangle
    {
        bool inuse;
        int i0, i1, i2;
    };

    //**********************************************************************
    //! \brief CDelaunay2 defines a class for creating a Delaunay triangulation
    //! from a set of 2D points
    //**********************************************************************
    class CDelaunay2 : public IDelaunay2, public CRefCount
    {
        std::vector<Vector2> points;
        std::vector<Triangle> triangles;
    public:
        CDelaunay2()
        {
            // First add the super triangle
            Vector2 v0(-FLT_MAX, -FLT_MAX);
            Vector2 v1(0.0f, FLT_MAX);
            Vector2 v2(FLT_MAX, -FLT_MAX);
            points.push_back(v0);
            points.push_back(v1);
            points.push_back(v2);
            Triangle tri;
            tri.i0 = 0;
            tri.i1 = 2;
            tri.i2 = 1;
            tri.inuse = true;
            triangles.push_back(tri);
        }

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IDelaunay2
        //**********************************************************************
        void AddPoint(Vector2 &pt);
    };
}
