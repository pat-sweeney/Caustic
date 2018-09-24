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
        bool inuse;     //!< Flag indicating whether triangle is a good (true) or bad (false) triangle
        int v0, v1, v2; //!< Indices into CDelaunay2.points defining the triangle's vertices
        int e0, e1, e2; //!< Indices into CDelaunay2.edges defining the triangle's edges

        Triangle() {}
        Triangle(int _v0, int _v1, int _v2, int _e0, int _e1, int _e2, bool used) :
            v0(_v0), v1(_v1), v2(_v2),
            e0(_e0), e1(_e1), e2(_e2),
            inuse(used)
        {
        }
    };

    struct Edge
    {
        int v0, v1; //!< Indices into CDelaunay2.points defining the triangle's edges

        Edge() {}
        Edge(int i0, int i1)
        {
            v0 = i0;
            v1 = i1;
        }
    };

    //**********************************************************************
    //! \brief CDelaunay2 defines a class for creating a Delaunay triangulation
    //! from a set of 2D points
    //**********************************************************************
    class CDelaunay2 : public IDelaunay2, public CRefCount
    {
        std::vector<Vector2> m_points;
        std::map<std::tuple<int, int>, int> m_edgeMap; // Maps vertex index pair into edge index
        std::vector<Edge> m_edges;
        std::vector<Triangle> m_triangles;
        int m_numTriangles;

        int FindOrAddEdge(int v0, int v1);
    public:
        CDelaunay2(BBox2 &bb);

        //**********************************************************************
        // IAddRef
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IDelaunay2
        //**********************************************************************
        virtual void Open() override {}
        virtual void AddPoint(Vector2 &pt) override;
        virtual void Close() override;

        virtual uint32 GetNumberTriangles() override 
        { 
            return m_numTriangles;
        }

        virtual void GetTriangle(uint32 index, Vector2 &v0, Vector2 &v1, Vector2 &v2) override
        {
            v0 = m_points[m_triangles[index].v0];
            v1 = m_points[m_triangles[index].v1];
            v2 = m_points[m_triangles[index].v2];
        }
    };
}
