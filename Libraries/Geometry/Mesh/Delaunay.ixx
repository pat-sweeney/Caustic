//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <map>

#if 1
#define DEBUGGING_AIDS // Determine whether to dump debug
#endif

export module Geometry.Mesh.Delaunay;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.Mesh.IDelaunay;

//**********************************************************************
// File: Delaunay.ixx
// Header with class/methods for performing Delaunay triangulation
//**********************************************************************

export namespace Caustic
{
    const int c_TriangleBad = 0x1;      // Flag indicating whether triangle was removed due to violating Delaunay property
    const int c_ExteriorTriangle = 0x2; // Flag indicating whether triangle is a part of the exterior (super triangle)
    const int c_RemovedTriangle = 0x4;

    struct Triangle
    {
        uint8 flags;    // Miscellaneous triangle flags
        int v0, v1, v2; // Indices into CDelaunay2.points defining the triangle's vertices
        int e0, e1, e2; // Indices into CDelaunay2.edges defining the triangle's edges

        Triangle() {}
        Triangle(int _v0, int _v1, int _v2, int _e0, int _e1, int _e2, uint8 triFlags) :
            v0(_v0), v1(_v1), v2(_v2),
            e0(_e0), e1(_e1), e2(_e2),
            flags(triFlags)
        {
        }
    };

    const int c_NormalEdge = 0x0;
    const int c_BoundaryEdge = 0x1;
    struct Edge
    {
        uint8 flags;
        int v0, v1; // Indices into CDelaunay2.points defining the triangle's edges
        int t0, t1; // Triangles sharing this edge

        Edge() : flags(0) {}
        Edge(int i0, int i1, int tri0, int tri1) : flags(0)
        {
            v0 = i0;
            v1 = i1;
            t0 = tri0;
            t1 = tri1;
        }
    };

    //**********************************************************************
    // Class: CDelaunay2
    // Defines a class for creating a Delaunay triangulation
    // from a set of 2D points
    //**********************************************************************
    class CDelaunay2 : public IDelaunay2, public CRefCount
    {
        std::vector<DelaunayVertex> m_points;
        std::map<std::tuple<int, int>, int> m_edgeMap; // Maps vertex index pair into edge index
        std::vector<Edge> m_edges;
        std::vector<Triangle> m_triangles;
        int m_numTriangles;

        int FindOrAddEdge(int v0, int v1, int tri, int edgeFlag);
        void CreateBoundaryEdges();
        void ComputeTriangulation();
        void TriangulatePoints(uint8 flag);
    public:
        CDelaunay2(DelaunayVertex* pVertices, int numVertices, int* pIndices, int numIndices);
        CDelaunay2(BBox2 &bb);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IDelaunay2
        //**********************************************************************
        virtual void Open() override {}
        virtual void AddPoint(Vector2 &pt, Vector2 &uv, bool isBoundary) override;
        virtual void Close(bool removeSuperTriangles) override;
        virtual void AddFixedMesh(DelaunayVertex* pVertices, int numVertices, int* pIndices, int numIndices) override;

        virtual uint32 GetNumberTriangles() override 
        { 
            return m_numTriangles;
        }

        virtual void GetTriangleIndices(uint32 index, uint32& i0, uint32& i1, uint32& i2, bool isExterior[3]) override
        {
            i0 = m_triangles[index].v0;
            i1 = m_triangles[index].v1;
            i2 = m_triangles[index].v2;
            isExterior[0] = (m_edges[m_triangles[index].e0].flags & c_BoundaryEdge) ? true : false;
            isExterior[1] = (m_edges[m_triangles[index].e1].flags & c_BoundaryEdge) ? true : false;
            isExterior[2] = (m_edges[m_triangles[index].e2].flags & c_BoundaryEdge) ? true : false;
        }

        virtual void GetTriangle(uint32 index, Vector2 &v0, Vector2 &v1, Vector2 &v2, bool isExterior[3]) override
        {
            v0 = m_points[m_triangles[index].v0].pos;
            v1 = m_points[m_triangles[index].v1].pos;
            v2 = m_points[m_triangles[index].v2].pos;
            isExterior[0] = (m_edges[m_triangles[index].e0].flags & c_BoundaryEdge) ? true : false;
            isExterior[1] = (m_edges[m_triangles[index].e1].flags & c_BoundaryEdge) ? true : false;
            isExterior[2] = (m_edges[m_triangles[index].e2].flags & c_BoundaryEdge) ? true : false;
        }

        virtual void WritePLY() override;
        void RemoveExteriorTriangles();
    };
}
