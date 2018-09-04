//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Base\Core\error.h"
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include "MeshConstructor.h"

namespace Caustic
{
    CAUSTICAPI void CreateMeshConstructor(IMeshConstructor **ppMeshConstructor)
    {
        std::unique_ptr<CMeshConstructor> spMeshConstructor(new CMeshConstructor());
        *ppMeshConstructor = spMeshConstructor.release();
        (*ppMeshConstructor)->AddRef();
    }

    CMeshConstructor::CMeshConstructor() :
        m_pCurFace(nullptr)
    {
    }

    void CMeshConstructor::MeshOpen()
    {
        CreateEmptyMesh(&m_spMesh);
    }

    void CMeshConstructor::MeshClose(IMesh **ppMesh)
    {
        *ppMesh = m_spMesh.p;
        (*ppMesh)->AddRef();
        m_spMesh = nullptr;
    }

    void CMeshConstructor::SubMeshOpen()
    {
        CT((m_spSubMesh == nullptr) ? S_OK : E_FAIL);
        m_spSubMesh = new CSubMesh();
        m_spSubMesh->AddRef();
    }

    void CMeshConstructor::SubMeshClose(ISubMesh **ppSubMesh)
    {
        if (ppSubMesh)
        {
            *ppSubMesh = m_spSubMesh.p;
            (*ppSubMesh)->AddRef();
        }
        if (m_spMesh.p)
        {
            m_spSubMesh->CheckConsistency();
            m_spMesh->AddSubMesh(m_spSubMesh.p);
        }
        m_spSubMesh = nullptr;
    }

    void CMeshConstructor::FaceOpen()
    {
        // Allocate new face
        m_pCurFace = m_spSubMesh->AllocateFace();
        m_pPrevEdge = nullptr;
        m_pPrevVertex = nullptr;
    }

    void CMeshConstructor::AddEdge(CGeomVertex *pTail, CGeomVertex *pHead)
    {
        // First see if the edge already exists
        CHalfEdge *pNewEdge = m_spSubMesh->FindEdge(pHead, pTail);
        if (pNewEdge == nullptr)
        {
            // Edge doesn't exist yet. Create the edge.
            pNewEdge = m_spSubMesh->AllocateEdge(pHead, pTail);
        }
        pNewEdge->m_pFace = m_pCurFace;
        m_spSubMesh->LinkEdges(m_pPrevEdge, pNewEdge);
        m_pPrevEdge = pNewEdge;
        if (m_pCurFace->m_pEdge == nullptr)
            m_pCurFace->m_pEdge = pNewEdge;
    }

    void CMeshConstructor::FaceClose()
    {
        // Add edge between first and last vertex
        AddEdge(m_pPrevVertex, m_pCurFace->m_vertices[0]);

        // Compute current face's center point
        m_pCurFace->m_center.x /= (float)m_pCurFace->m_vertices.size();
        m_pCurFace->m_center.y /= (float)m_pCurFace->m_vertices.size();
        m_pCurFace->m_center.z /= (float)m_pCurFace->m_vertices.size();

        // Link up first and last edge
        if (m_pCurFace->m_pEdge && m_pPrevEdge)
            m_spSubMesh->LinkEdges(m_pPrevEdge, m_pCurFace->m_pEdge);

        // Compute face normal vector.
        m_pCurFace->ComputeFaceNormal();

//#define DEBUG_LINKS
#ifdef DEBUG_LINKS
        wchar_t buf[1024];
        swprintf_s(buf, L"Face Verts: ");
        OutputDebugString(buf);
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf, L"%d ", m_pCurFace->GetVertex(i)->index);
            OutputDebugString(buf);
        }
        swprintf_s(buf, L"\n");
        OutputDebugString(buf);
        swprintf_s(buf, L"Face Verts Pos: ");
        OutputDebugString(buf);
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf, L"%f %f %f, ", m_pCurFace->GetVertex(i)->pos.x, m_pCurFace->GetVertex(i)->pos.y, m_pCurFace->GetVertex(i)->pos.z);
            OutputDebugString(buf);
        }
        swprintf_s(buf, L"\n");
        OutputDebugString(buf);
        swprintf_s(buf,L"Face Edges: \n");
        OutputDebugString(buf);
        CHalfEdge *pEdge = m_pCurFace->GetEdge();
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf,L"  %d: Prev:%d  Next:%d  Opposite:%d  OppPrev:%d  OppNext:%d  Face:%d\n",
                pEdge->index, pEdge->m_pPrev->index, pEdge->m_pNext->index, 
                pEdge->m_pOpposite->index,
                pEdge->m_pOpposite->m_pPrev->index,
                pEdge->m_pOpposite->m_pNext->index,
                pEdge->m_pFace->index);
            OutputDebugString(buf);
            pEdge = pEdge->m_pNext;
        }
        swprintf_s(buf,L"\n");
        OutputDebugString(buf);
#endif
        m_pCurFace = nullptr;
        return;
    }

    void CMeshConstructor::VertexAdd(Vector3 &pos, Vector3 &normal, Vector2 &uv)
    {
        if (m_pCurFace == nullptr)
            CT(E_FAIL); // Face isn't open

        CGeomVertex *pVertex = m_spSubMesh->FindVertex(pos, nullptr, &uv);
        if (pVertex == nullptr)
            pVertex = m_spSubMesh->AllocateGeomVertex(pos, normal, uv);
        m_pCurFace->m_vertices.push_back(pVertex);

        // Update running average of where current face's center point is
        m_pCurFace->m_center.x += pVertex->pos.x;
        m_pCurFace->m_center.y += pVertex->pos.y;
        m_pCurFace->m_center.z += pVertex->pos.z;
        
        // If this is NOT the first vertex being added to the face
        // then create an edge between the last vertex and the new vertex.
        if (m_pPrevVertex)
            AddEdge(m_pPrevVertex, pVertex);
        m_pPrevVertex = pVertex;
        return;
    }
}
