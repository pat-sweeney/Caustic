//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Base\Math\Helper.h"
#include "Base\Math\BBox.h"
#include <map>
#include "Delaunay.h"

namespace Caustic
{
    CDelaunay2::CDelaunay2(BBox2 &bb)
    {
        // First add the super triangle
        Vector2 v0(bb.minPt.x, bb.minPt.y);
        Vector2 v1(bb.maxPt.x, bb.minPt.y);
        Vector2 v2(bb.minPt.x, bb.maxPt.y);
        Vector2 v3(bb.maxPt.x, bb.maxPt.y);
        m_points.push_back(v0);
        m_points.push_back(v1);
        m_points.push_back(v2);
        m_points.push_back(v3);
        int e0 = FindOrAddEdge(0, 1);
        int e1 = FindOrAddEdge(1, 2);
        int e2 = FindOrAddEdge(2, 0);
        int e3 = FindOrAddEdge(1, 3);
        int e4 = FindOrAddEdge(3, 2);
        m_triangles.push_back(Triangle(0, 1, 2, e0, e1, e2, true));
        m_triangles.push_back(Triangle(1, 3, 2, e3, e4, e1, true));
        m_numTriangles = 2;
    }

    int CDelaunay2::FindOrAddEdge(int v0, int v1)
    {
        int ei0;
        std::map<std::tuple<int, int>, int>::iterator it = m_edgeMap.find(std::make_tuple(v0, v1));
        if (it != m_edgeMap.end())
            ei0 = it->second;
        else
        {
            m_edges.push_back(Edge(v0, v1));
            ei0 = (int)m_edges.size() - 1;
            m_edgeMap.insert(std::make_pair(std::make_tuple(v0, v1), ei0));
            m_edgeMap.insert(std::make_pair(std::make_tuple(v1, v0), ei0));
        }
        return ei0;
    }

    //**********************************************************************
    //! \brief Adds a new point to our current Delaunay triangulation using
    //! the Bowyer-Watson algorithm.
    //**********************************************************************
    void CDelaunay2::AddPoint(Vector2 &pt)
    {
        //**********************************************************************
        // First determine which triangles now fail the Delaunay property
        // when the new point is added
        //**********************************************************************
        std::vector<int> badTriangles;
        for (int i = 0; i < m_triangles.size(); i++)
        {
            if (!m_triangles[i].inuse)
                continue;
            if (PointInTriangleCircumcircle(m_points[m_triangles[i].v0], m_points[m_triangles[i].v1], m_points[m_triangles[i].v2], pt))
                badTriangles.push_back(i);
        }

        //**********************************************************************
        // Next determine which edges make up the polygon defining the hole
        // caused by removing the bad triangles
        //**********************************************************************
        std::vector<int> edgeUseCount;
        edgeUseCount.resize(m_edges.size(), 0);
        for (int j = 0; j < (int)badTriangles.size(); j++)
        {
            Triangle &t = m_triangles[badTriangles[j]];
            edgeUseCount[t.e0]++;
            edgeUseCount[t.e1]++;
            edgeUseCount[t.e2]++;
            t.inuse = false; // Mark this triangle as bad
            m_numTriangles--;
        }

        //**********************************************************************
        // Now find all the edges whose count == 1, that is they are used by only
        // 1 "bad" triangle. These edges define the boundary of our hole (created
        // by removing the bad triangles)
        //**********************************************************************
        m_points.push_back(pt);
        int ptIndex = (int)m_points.size() - 1;
        for (int j = 0; j < (int)edgeUseCount.size(); j++)
        {
            if (edgeUseCount[j] == 1)
            {
                // Create a new triangle between each edge and new point
                int ei0 = FindOrAddEdge(m_edges[j].v0, ptIndex);
                int ei1 = FindOrAddEdge(m_edges[j].v1, ptIndex);
                m_triangles.push_back(Triangle(m_edges[j].v0, m_edges[j].v1, ptIndex, j, ei1, ei0, true));
                m_numTriangles++;
            }
        }
    }
    
    void CDelaunay2::Close()
    {
        for (int i = 0, j = 0; i < (int)m_triangles.size(); i++)
        {
            if (!m_triangles[i].inuse)
                continue;
            if (i != j)
                m_triangles[j++] = m_triangles[i];
        }
    }
    
    CAUSTICAPI void CreateDelaunay2(IDelaunay2 **ppDelaunay, BBox2 &bb)
    {
        std::unique_ptr<CDelaunay2> spDelaunay(new CDelaunay2(bb));
        *ppDelaunay = spDelaunay.release();
        (*ppDelaunay)->AddRef();
    }
}
