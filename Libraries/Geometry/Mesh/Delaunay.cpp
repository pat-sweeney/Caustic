//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Base\Math\Helper.h"
#include "Base\Math\BBox.h"
#include "Imaging\Image\Image.h"
#include <map>
#include "Delaunay.h"
#include <stack>

namespace Caustic
{
    CDelaunay2::CDelaunay2(BBox2 &bb)
    {
        // First add the super triangle
        Vector2 v0(bb.minPt.x, bb.minPt.y);
        Vector2 v1(bb.maxPt.x, bb.minPt.y);
        Vector2 v2(bb.minPt.x, bb.maxPt.y);
        Vector2 v3(bb.maxPt.x, bb.maxPt.y);
        Vector2 uv(0.0f, 0.0f);
        m_points.push_back(Vertex(v0, uv, c_SuperTriangleVertex));
        m_points.push_back(Vertex(v1, uv, c_SuperTriangleVertex));
        m_points.push_back(Vertex(v2, uv, c_SuperTriangleVertex));
        m_points.push_back(Vertex(v3, uv, c_SuperTriangleVertex));
        int e0 = FindOrAddEdge(0, 1, 0, false);
        int e1 = FindOrAddEdge(1, 2, 0, false);
        int e2 = FindOrAddEdge(2, 0, 0, false);
        int e3 = FindOrAddEdge(1, 3, 1, false);
        int e4 = FindOrAddEdge(3, 2, 1, false);
        int e5 = FindOrAddEdge(2, 1, 1, false);
        m_triangles.push_back(Triangle(0, 1, 2, e0, e1, e2, c_ExteriorTriangle));
        m_triangles.push_back(Triangle(1, 3, 2, e3, e4, e5, c_ExteriorTriangle));
        m_numTriangles = 2;
    }

    int CDelaunay2::FindOrAddEdge(int v0, int v1, int tri, bool isBoundaryEdge)
    {
        int ei0;
        std::map<std::tuple<int, int>, int>::iterator it = m_edgeMap.find(std::make_tuple(v0, v1));
        if (it != m_edgeMap.end())
        {
            ei0 = it->second;
            if (m_edges[ei0].v0 == v0)
                m_edges[ei0].t0 = tri;
            else
                m_edges[ei0].t1 = tri;
        }
        else
        {
            m_edges.push_back(Edge(v0, v1, tri, -1));
            ei0 = (int)m_edges.size() - 1;
            m_edgeMap.insert(std::make_pair(std::make_tuple(v0, v1), ei0));
            m_edgeMap.insert(std::make_pair(std::make_tuple(v1, v0), ei0));
        }
        if (isBoundaryEdge)
            m_edges[ei0].flags |= c_BoundaryEdge;
        return ei0;
    }

    //**********************************************************************
    //! \brief Adds a new point to our current Delaunay triangulation using
    //! the Bowyer-Watson algorithm.
    //**********************************************************************
    void CDelaunay2::AddPoint(Vector2 &pt, Vector2 &uv, bool isBoundary)
    {
        m_points.push_back(Vertex(pt, uv, (isBoundary) ? c_BoundaryVertex : c_InteriorVertex));
    }

    //**********************************************************************
    //! \brief Pre-allocates all the edges that form our meshes boundary.
    //! We assume that boundary edges are added in counter-clockwise order
    //! around the mesh.
    //**********************************************************************
    void CDelaunay2::CreateBoundaryEdges()
    {
        int firstBoundaryVertex = -1;
        int lastBoundaryVertex = -1;
        for (int i = 0; i < (int)m_points.size(); i++)
        {
            if (m_points[i].flags & c_BoundaryVertex)
            {
                if (lastBoundaryVertex != -1)
                    FindOrAddEdge(i, lastBoundaryVertex, -1, true);
                else
                    firstBoundaryVertex = i;
                lastBoundaryVertex = i;
            }
        }
        if (firstBoundaryVertex != -1 && lastBoundaryVertex != -1 && firstBoundaryVertex != lastBoundaryVertex)
            FindOrAddEdge(firstBoundaryVertex, lastBoundaryVertex, -1, true);
    }

    //**********************************************************************
    //! \brief Computes the Delaunay triangulation of our mesh. We first compute
    //! a triangulation based on the boundary vertices. We then remove the exterior
    //! triangles (super triangles) and then add the interior vertices.
    //**********************************************************************
    void CDelaunay2::ComputeTriangulation()
    {
        CreateBoundaryEdges();
        TriangulatePoints(c_BoundaryVertex);
        RemoveExteriorTriangles();
        TriangulatePoints(c_InteriorVertex);
    }

#ifdef DEBUGGING_AIDS
#include "Imaging\Image\Image.h"
    static int frame = 0;
    void CDelaunay2::DrawTriangulation(int type, int currentTri)
    {
        if (frame == 267)
            DebugBreak();
        CRefObj<IImage> spImage;
        CreateImage(1024, 1024, &spImage);
        uint8 blue[4] = { 0, 0, 255, 255 };
        uint8 red[4] = { 255, 0, 0, 255 };
        // Draw current triangle in green and boundary edge in yellow
        uint8 green[4] = { 0, 255, 0, 255 };
        uint8 yellow[4] = { 255, 255, 0, 255 };
        uint8 cyan[4] = { 0, 255, 255, 255 };
        uint8 grey[4] = { 0, 0, 0, 255 };
        for (int i = 0; i < (int)m_triangles.size(); i++)
        {
            if (type == 0)
            {
                if (m_triangles[i].flags & c_TriangleBad)
                    continue;
                uint8 *color = red;
                m_triangles[i].flags &= ~c_RemovedTriangle;
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e0].v0].pos,
                    m_points[m_edges[m_triangles[i].e0].v1].pos,
                    color);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e1].v0].pos,
                    m_points[m_edges[m_triangles[i].e1].v1].pos,
                    color);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e2].v0].pos,
                    m_points[m_edges[m_triangles[i].e2].v1].pos,
                    color);
                for (int j = -3; j < 3; j++)
                    for (int k = -3; k < 3; k++)
                        spImage->SetPixel((int)m_points[currentTri].pos.x + j, (int)m_points[currentTri].pos.y + k, yellow);
            }
            else if (type == 1)
            {
                if (m_triangles[i].flags & c_TriangleBad)
                    continue;
                if (m_triangles[i].flags & c_RemovedTriangle)
                    continue;
                m_triangles[i].flags &= ~c_RemovedTriangle;
                Vector2 mid;
                mid.x = (m_points[m_edges[m_triangles[i].e0].v0].pos.x + m_points[m_edges[m_triangles[i].e0].v1].pos.x) / 2.0f;
                mid.y = (m_points[m_edges[m_triangles[i].e0].v0].pos.y + m_points[m_edges[m_triangles[i].e0].v1].pos.y) / 2.0f;
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e0].v0].pos,
                    mid,
                    red);
                spImage->DrawLine(
                    mid,
                    m_points[m_edges[m_triangles[i].e0].v1].pos,
                    green);
                mid.x = (m_points[m_edges[m_triangles[i].e1].v0].pos.x + m_points[m_edges[m_triangles[i].e1].v1].pos.x) / 2.0f;
                mid.y = (m_points[m_edges[m_triangles[i].e1].v0].pos.y + m_points[m_edges[m_triangles[i].e1].v1].pos.y) / 2.0f;
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e1].v0].pos,
                    mid,
                    red);
                spImage->DrawLine(
                    mid,
                    m_points[m_edges[m_triangles[i].e1].v1].pos,
                    green);
                mid.x = (m_points[m_edges[m_triangles[i].e2].v0].pos.x + m_points[m_edges[m_triangles[i].e2].v1].pos.x) / 2.0f;
                mid.y = (m_points[m_edges[m_triangles[i].e2].v0].pos.y + m_points[m_edges[m_triangles[i].e2].v1].pos.y) / 2.0f;
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e2].v0].pos,
                    mid,
                    red);
                spImage->DrawLine(
                    mid,
                    m_points[m_edges[m_triangles[i].e2].v1].pos,
                    green);
                spImage->SetPixel((int)m_points[currentTri].pos.x, (int)m_points[currentTri].pos.y, yellow);
            }
            else if (type == 2)
            {
                if (m_triangles[i].flags & c_TriangleBad)
                    continue;
                uint8 *color;
                if (m_triangles[i].v0 == currentTri ||
                    m_triangles[i].v1 == currentTri ||
                    m_triangles[i].v2 == currentTri)
                    color = green;
                else
                    color = red;
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e0].v0].pos,
                    m_points[m_edges[m_triangles[i].e0].v1].pos,
                    color);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e1].v0].pos,
                    m_points[m_edges[m_triangles[i].e1].v1].pos,
                    color);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e2].v0].pos,
                    m_points[m_edges[m_triangles[i].e2].v1].pos,
                    color);
                spImage->SetPixel((int)m_points[currentTri].pos.x, (int)m_points[currentTri].pos.y, yellow);
            }
        }
        wchar_t fn[1024];
        swprintf_s(fn, L"d:\\images\\frame-%d.png", frame++);
        StoreImage(fn, spImage.p);
    }
    void CDelaunay2::DrawTriangulation(bool skipExterior, int currentTri, int nextTri, int crossEdge)
    {
        CRefObj<IImage> spImage;
        CreateImage(1024, 1024, &spImage);
        uint8 blue[4] = { 0, 0, 255, 255 };
        uint8 red[4] = { 255, 0, 0, 255 };
        // Draw current triangle in green and boundary edge in yellow
        uint8 green[4] = { 0, 255, 0, 255 };
        uint8 yellow[4] = { 255, 255, 0, 255 };
        uint8 cyan[4] = { 0, 255, 255, 255 };
        uint8 grey[4] = { 180, 180, 180, 255 };
        for (int i = 0; i < (int)m_triangles.size(); i++)
        {
            if (currentTri == -2 && nextTri > 0)
            {
                for (int i2 = -3; i2<3; i2++)
                    for (int j2 = -3; j2<3; j2++)
                        spImage->SetPixel((int)m_points[nextTri].pos.x + i2, (int)m_points[nextTri].pos.y + j2, yellow);
            }
            if (currentTri == -2 && (m_triangles[i].flags & c_RemovedTriangle))
            {
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e0].v0].pos,
                    m_points[m_edges[m_triangles[i].e0].v1].pos,
                    grey);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e1].v0].pos,
                    m_points[m_edges[m_triangles[i].e1].v1].pos,
                    grey);
                spImage->DrawLine(
                    m_points[m_edges[m_triangles[i].e2].v0].pos,
                    m_points[m_edges[m_triangles[i].e2].v1].pos,
                    grey);
                m_triangles[i].flags &= ~c_RemovedTriangle;
                continue;
            }
            if (m_triangles[i].flags & c_TriangleBad)
                continue;
            if (skipExterior && (m_triangles[i].flags & c_ExteriorTriangle))
                continue;
            if (i != currentTri && i != nextTri && currentTri != -1 && nextTri != -1)
                continue;
            spImage->DrawLine(
                m_points[m_edges[m_triangles[i].e0].v0].pos,
                m_points[m_edges[m_triangles[i].e0].v1].pos,
                (m_triangles[i].e0 == crossEdge) ? yellow :
                ((m_edges[m_triangles[i].e0].t0 == -1 || m_edges[m_triangles[i].e0].t1 == -1)?green :
                 ((i == nextTri) ? cyan :
                  ((i == currentTri) ? green :
                   ((m_edges[m_triangles[i].e0].flags & c_BoundaryEdge) ? blue : red)))));
            spImage->DrawLine(
                m_points[m_edges[m_triangles[i].e1].v0].pos,
                m_points[m_edges[m_triangles[i].e1].v1].pos,
                (m_triangles[i].e1 == crossEdge) ? yellow :
                ((m_edges[m_triangles[i].e1].t0 == -1 || m_edges[m_triangles[i].e1].t1 == -1) ? green :
                ((i == nextTri) ? cyan :
                ((i == currentTri) ? green :
                    ((m_edges[m_triangles[i].e1].flags & c_BoundaryEdge) ? blue : red)))));
            spImage->DrawLine(
                m_points[m_edges[m_triangles[i].e2].v0].pos,
                m_points[m_edges[m_triangles[i].e2].v1].pos,
                (m_triangles[i].e2 == crossEdge) ? yellow :
                ((m_edges[m_triangles[i].e2].t0 == -1 || m_edges[m_triangles[i].e2].t0 == -1) ? green :
                ((i == nextTri) ? cyan :
                ((i == currentTri) ? green :
                    ((m_edges[m_triangles[i].e2].flags & c_BoundaryEdge) ? blue : red)))));
        }
        wchar_t fn[1024];
        swprintf_s(fn, L"d:\\images\\frame-%d.png", frame++);
        StoreImage(fn, spImage.p);
    }
#endif

    //**********************************************************************
    //! \brief Adds the set of vertex points that match the specified vertex
    //! flag to the current triangulation.
    //**********************************************************************
    void CDelaunay2::TriangulatePoints(uint8 flag)
    {
        //**********************************************************************
        // Next add each of our points to the triangulation
        //**********************************************************************
        for (int ptIndex = 0; ptIndex < (int)m_points.size(); ptIndex++)
        {
#ifdef DEBUGGING_AIDS
            //DrawTriangulation(flag & c_InteriorVertex);
#endif
            if (!(m_points[ptIndex].flags & flag))
                continue;

            //for (int i = 0; i < m_triangles.size(); i++)
            //{
            //    if (m_triangles[i].flags & c_TriangleBad)
            //        continue;
            //    _ASSERT(m_edges[m_triangles[i].e0].t0 != -1);
            //    _ASSERT(m_edges[m_triangles[i].e0].t1 != -1);
            //    _ASSERT(m_edges[m_triangles[i].e1].t0 != -1);
            //    _ASSERT(m_edges[m_triangles[i].e1].t1 != -1);
            //    _ASSERT(m_edges[m_triangles[i].e2].t0 != -1);
            //    _ASSERT(m_edges[m_triangles[i].e2].t1 != -1);
            //}

            //**********************************************************************
            // First determine which triangles now fail the Delaunay property
            // when the new point is added
            //**********************************************************************
            std::vector<int> badTriangles;
            for (int i = 0; i < m_triangles.size(); i++)
            {
                if (m_triangles[i].flags & c_TriangleBad)
                    continue;
                if (PointInTriangleCircumcircle(m_points[m_triangles[i].v0].pos, m_points[m_triangles[i].v1].pos, m_points[m_triangles[i].v2].pos, m_points[ptIndex].pos))
                {
                    // Next make sure that all three vertices of the triangle are not boundary vertices.
                    // This can happen when two of the triangle edges are inside the boundary and one edge
                    // is on the boundary.  Alternatively, it can happen if two edges are boundary edges and
                    // the third edge is a non-boundary edge. For instance, if edge e0 is a boundary edge, 
                    // and e1 & e2 are interior (non-boundary) edges, but vertex v2 is on the boundary, we can
                    // end up classifying the triangle as bad, and incorrectly remove it.
                    //
                    //     v0    e0   v1
                    //    ---X-------X-----
                    //        \     /
                    //      e1 \   /  e2
                    //          \ /
                    //   --------X----------
                    //           v2
                    int numBoundaryVertices = 0;
                    numBoundaryVertices += (m_points[m_triangles[i].v0].flags & c_BoundaryVertex) ? 1 : 0;
                    numBoundaryVertices += (m_points[m_triangles[i].v1].flags & c_BoundaryVertex) ? 1 : 0;
                    numBoundaryVertices += (m_points[m_triangles[i].v2].flags & c_BoundaryVertex) ? 1 : 0;
                    if (numBoundaryVertices < 3)
                    {
                        badTriangles.push_back(i);
                        m_triangles[i].flags |= c_RemovedTriangle;
                    }
                }
            }
            DrawTriangulation(0, ptIndex);
            DrawTriangulation(1, ptIndex);

            //**********************************************************************
            // Next determine which edges make up the polygon defining the hole
            // caused by removing the bad triangles
            //**********************************************************************
            std::vector<int> edgeUseCount;
            edgeUseCount.resize(m_edges.size(), 0);
            for (int i = 0; i < (int)badTriangles.size(); i++)
            {
                int triIndex = badTriangles[i];
                Triangle &t = m_triangles[triIndex];
                edgeUseCount[t.e0]++;
                edgeUseCount[t.e1]++;
                edgeUseCount[t.e2]++;
                t.flags |= c_TriangleBad;
                if (m_edges[t.e0].t0 == triIndex)
                    m_edges[t.e0].t0 = -1;
                else
                {
                    _ASSERT(m_edges[t.e0].t1 == -1 || m_edges[t.e0].t1 == triIndex);
                    m_edges[t.e0].t1 = -1;
                }
                if (m_edges[t.e1].t0 == triIndex)
                    m_edges[t.e1].t0 = -1;
                else
                {
                    _ASSERT(m_edges[t.e1].t1 == -1 || m_edges[t.e1].t1 == triIndex);
                    m_edges[t.e1].t1 = -1;
                }
                if (m_edges[t.e2].t0 == triIndex)
                    m_edges[t.e2].t0 = -1;
                else
                {
                    _ASSERT(m_edges[t.e2].t1 == -1 || m_edges[t.e2].t1 == triIndex);
                    m_edges[t.e2].t1 = -1;
                }
                m_numTriangles--;
            }

#ifdef DEBUGGING_AIDS
#if 0
            CRefObj<IImage> spImage;
            CreateImage(1024, 1024, &spImage);
            uint8 red[4] = { 255, 0, 0, 255 };
            uint8 green[4] = { 0, 255, 0, 255 };
            uint8 yellow[4] = { 0, 255, 255, 255 };
            for (int i = 0; i < (int)edgeUseCount.size(); i++)
            {
                if (edgeUseCount[i] == 1)
                {
                    spImage->DrawLine(
                        m_points[m_edges[i].v0].pos,
                        m_points[m_edges[i].v1].pos,
                        red);
                }
                else if (edgeUseCount[i] == 2)
                {
                    spImage->DrawLine(
                        m_points[m_edges[i].v0].pos,
                        m_points[m_edges[i].v1].pos,
                        green);
                }
            }
            for (int i = -3; i<3; i++)
                for (int j = -3; j<3; j++)
                    spImage->SetPixel((int)m_points[ptIndex].pos.x+i, (int)m_points[ptIndex].pos.y+j, yellow);
            wchar_t fn[1024];
            swprintf_s(fn, L"d:\\images\\frame-%d.png", frame++);
            StoreImage(fn, spImage.p);
            //DrawTriangulation(true);
#endif
#endif
            //**********************************************************************
            // Now find all the edges whose count == 1, that is they are used by only
            // 1 "bad" triangle. These edges define the boundary of our hole (created
            // by removing the bad triangles)
            //**********************************************************************
            for (int i = 0; i < (int)edgeUseCount.size(); i++)
            {
                if (edgeUseCount[i] == 1)
                {
                    // First determine if we are to left or right of edge m_edges[i].v0=>m_edges[i].v1
                    // We need to make sure that all our triangles are created in counter clockwise manner.
                    Vector2 edge0 = m_points[m_edges[i].v1].pos - m_points[m_edges[i].v0].pos;
                    Vector2 edge1 = m_points[ptIndex].pos - m_points[m_edges[i].v0].pos;
                    float dir = edge0.Cross(edge1);
                    int v0, v1;
                    if (IsLess(dir, 0.0f))
                    {
                        v0 = m_edges[i].v0;
                        v1 = m_edges[i].v1;
                    }
                    else
                    {
                        v1 = m_edges[i].v0;
                        v0 = m_edges[i].v1;
                    }

                    // Create a new triangle between each edge and new point
                    int triIndex = (int)m_triangles.size();
                    int ei0 = FindOrAddEdge(ptIndex, v0, triIndex, false);
                    int ei1 = FindOrAddEdge(v1, ptIndex, triIndex, false);
                    if (IsGreaterEq(dir, 0.0f))
                        m_edges[i].t0 = triIndex;
                    else
                        m_edges[i].t1 = triIndex;
                    m_triangles.push_back(Triangle(v0, v1, ptIndex, i, ei1, ei0, 0));
#ifdef DEBUGGING_AIDS
                    {
                        wchar_t buf[1024];
                        swprintf_s(buf, L"Creating Triangle: v0:%3d v1:%3d v2:%3d\te0:%3d e1:%3d e2:%3d\n",
                            v0, v1, ptIndex, i, ei1, ei0);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   v0.x:%f  v0.y=%f\n",
                            m_points[v0].pos.x, m_points[v0].pos.y);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   v1.x:%f  v1.y=%f\n",
                            m_points[v1].pos.x, m_points[v1].pos.y);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   ptIndex.x:%f  ptIndex.y=%f \n",
                            m_points[ptIndex].pos.x, m_points[ptIndex].pos.y);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   e0:%3d v0:%3d v1:%3d t0:%3d t1:%3d\n", i, v0, v1, m_edges[i].t0, m_edges[i].t1);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   e1:%3d v0:%3d v1:%3d t0:%3d t1:%3d\n", ei0, m_edges[ei0].v0, m_edges[ei0].v1, m_edges[ei0].t0, m_edges[ei0].t1);
                        OutputDebugString(buf);
                        swprintf_s(buf, L"                   e2:%3d v0:%3d v1:%3d t0:%3d t1:%3d\n", ei1, m_edges[ei1].v0, m_edges[ei1].v1, m_edges[ei1].t0, m_edges[ei1].t1);
                        OutputDebugString(buf);
                    }
#endif
                    m_numTriangles++;
                }
            }
            DrawTriangulation(2, ptIndex);
        }
    }

    void CDelaunay2::RemoveExteriorTriangles()
    {
        std::stack<int> exteriorTriangles;

        // Place each triangle that is part of the super triangles into our
        // exterior triangle stack.
        for (int i = 0; i < (int)m_triangles.size(); i++)
        {
            if (m_triangles[i].flags & c_TriangleBad)
                continue;
            if (m_triangles[i].v0 <= 3 || m_triangles[i].v1 <= 3 || m_triangles[i].v2 <= 3) // Part of super triangle
            {
                m_triangles[i].flags |= c_ExteriorTriangle | c_TriangleBad;
#ifdef DEBUGGING_AIDS
                //DrawTriangulation(true);
#endif
                exteriorTriangles.push(i);
            }
        }

        // While there are exterior triangles, walk their edges and add any additional
        // exterior triangles to the stack
        while (!exteriorTriangles.empty())
        {
            int tri = exteriorTriangles.top();
            exteriorTriangles.pop();
            // Check each of our edges.
            auto CheckEdge = [&](int edge)
            {
                if (!(m_edges[edge].flags & c_BoundaryEdge))
                {
                    int nextTri = (m_edges[edge].t0 != tri) ? m_edges[edge].t0 : m_edges[edge].t1;
#ifdef DEBUGGING_AIDS
                    {
                        wchar_t buf[1024];
                        swprintf_s(buf, L"Stepping across edge: %d  into Triangle: %d\n", edge, nextTri);
                        OutputDebugString(buf);
                    }
#endif
                    if (!(m_triangles[nextTri].flags & c_ExteriorTriangle))
                    {
#ifdef DEBUGGING_AIDS
                        //DrawTriangulation(true);
                        //DrawTriangulation(true, tri, nextTri, edge);
#endif
                        m_triangles[nextTri].flags |= c_ExteriorTriangle | c_TriangleBad;
                        exteriorTriangles.push(nextTri);
                    }
                }
            };
            CheckEdge(m_triangles[tri].e0);
            CheckEdge(m_triangles[tri].e1);
            CheckEdge(m_triangles[tri].e2);
        }
    }

    void CDelaunay2::Close()
    {
        ComputeTriangulation();
        int j = 0;
        for (int i = 0; i < (int)m_triangles.size(); i++)
        {
            if (m_triangles[i].flags & c_TriangleBad)
                continue;
            if (m_triangles[i].flags & c_ExteriorTriangle)
                continue;
            if (m_triangles[i].v0 <= 3 || m_triangles[i].v1 <= 3 || m_triangles[i].v2 <= 3) // Part of super triangle
                continue;

            if (i != j)
                m_triangles[j++] = m_triangles[i];
        }
        m_triangles.resize(j);
        m_numTriangles = (uint32)m_triangles.size();
    }

    static void WriteString(HANDLE h, const char *pData, ...)
    {
        va_list args;
        va_start(args, pData);
        char buffer[1024];
        vsprintf_s(buffer, pData, args);
        va_end(args);
        DWORD bytesWritten;
        WriteFile(h, buffer, (DWORD)strlen(buffer), &bytesWritten, nullptr);
    }

    // This is a complete hack. I just needed a temporary way to export this as a mesh
    void CDelaunay2::WritePLY()
    {
        const wchar_t *pFilename = L"c:\\users\\patri\\output.ply";
        HANDLE h = CreateFile(pFilename, FILE_GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
        if (h == INVALID_HANDLE_VALUE)
            CT(HRESULT_FROM_WIN32(GetLastError()));
        WriteString(h, "ply\n");
        WriteString(h, "format ascii 1.0\n");
        WriteString(h, "element vertex %d\n", 3 * m_numTriangles);
        WriteString(h, "property float x\n");
        WriteString(h, "property float y\n");
        WriteString(h, "property float z\n");
        WriteString(h, "property float tx\n");
        WriteString(h, "property float ty\n");
        WriteString(h, "element face %d\n", m_numTriangles);
        WriteString(h, "property list uchar int vertex_indices\n");
        WriteString(h, "end_header\n");
        for (int i = 0; i < (int)m_numTriangles; i++)
        {
            WriteString(h, "%f %f %f %f %f\n", m_points[m_triangles[i].v0].pos.x, m_points[m_triangles[i].v0].pos.y, 0.0f,
                m_points[m_triangles[i].v0].uv.x, m_points[m_triangles[i].v0].uv.y);
            WriteString(h, "%f %f %f %f %f\n", m_points[m_triangles[i].v1].pos.x, m_points[m_triangles[i].v1].pos.y, 0.0f,
                m_points[m_triangles[i].v1].uv.x, m_points[m_triangles[i].v1].uv.y);
            WriteString(h, "%f %f %f %f %f\n", m_points[m_triangles[i].v2].pos.x, m_points[m_triangles[i].v2].pos.y, 0.0f,
                m_points[m_triangles[i].v2].uv.x, m_points[m_triangles[i].v2].uv.y);
        }
        for (int i = 0; i < (int)m_numTriangles; i++)
        {
            WriteString(h, "3 %d %d %d\n", i * 3 + 0, i * 3 + 1, i * 3 + 2);
        }
        CloseHandle(h);
    }

    CAUSTICAPI void CreateDelaunay2(IDelaunay2 **ppDelaunay, BBox2 &bb)
    {
        std::unique_ptr<CDelaunay2> spDelaunay(new CDelaunay2(bb));
        *ppDelaunay = spDelaunay.release();
        (*ppDelaunay)->AddRef();
    }
}
