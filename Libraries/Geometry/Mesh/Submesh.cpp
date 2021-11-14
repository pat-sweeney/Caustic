//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.Error;
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\MeshConstructor.h"
import Base.Math.Ray;
#include "Rendering\Caustic\PathTrace.h"
#include <Windows.h>

//**********************************************************************
// File: SubMesh.cpp
// Contains the methods used to implement the CSubMesh object.
//**********************************************************************
namespace Caustic
{
    bool CSubMesh::s_allocatorInitialized = false;
    CBlockAllocator<CGeomVertex> CSubMesh::m_vertexAllocator(100000);
    CBlockAllocator<CFaceVertex> CSubMesh::m_faceVertexAllocator(100000);
    CBlockAllocator<CHalfEdge> CSubMesh::m_edgeAllocator(100000);
    CBlockAllocator<CFace> CSubMesh::m_faceAllocator(100000);

    //**********************************************************************
    // Method: RayIntersect
    // See <ISubMesh::RayIntersect>
    //**********************************************************************
    bool CSubMesh::RayIntersect(Ray3& ray, RayIntersect3* pIntersection, uint32* pMaterialID)
    {
        bool hitMesh = false;
        uint32 minMaterial = 0;
        for (size_t i = 0; i < m_faces.size(); i++)
        {
            _ASSERT(m_faces[i]->m_vertices.size() == 3);
            RayIntersect3 rayInfo;
            if (ray.Intersect(
                m_faces[i]->m_vertices[0]->pos,
                m_faces[i]->m_vertices[1]->pos,
                m_faces[i]->m_vertices[2]->pos, &rayInfo))
            {
                if (rayInfo.hitTime < pIntersection->hitTime)
                {
                    minMaterial = m_materialID;
                    *pIntersection = rayInfo;
                    hitMesh = true;
                }
            }
        }
        if (hitMesh)
            *pMaterialID = minMaterial;
        return hitMesh;
    }

    //**********************************************************************
    // Method: Normalize
    // See <ISubMesh::Normalize>
    //**********************************************************************
    void CSubMesh::Normalize(const BBox3 &bbox)
    {
        BBox3 bb;
        float minx = bbox.minPt.x;
        float miny = bbox.minPt.y;
        float minz = bbox.minPt.z;
        float maxx = bbox.maxPt.x;
        float maxy = bbox.maxPt.y;
        float maxz = bbox.maxPt.z;
        float cx = (minx + maxx) / 2.0f;
        float cy = (miny + maxy) / 2.0f;
        float cz = (minz + maxz) / 2.0f;
        minx -= cx;
        miny -= cy;
        minz -= cz;
        maxx -= cx;
        maxy -= cy;
        maxz -= cz;
        float dx = maxx - minx;
        float maxdelta = dx;
        float dy = maxy - miny;
        if (dy > maxdelta) maxdelta = dy;
        float dz = maxz - minz;
        if (dz > maxdelta) maxdelta = dz;
        float scalex = dx / maxdelta;
        float scaley = dy / maxdelta;
        float scalez = dz / maxdelta;
        uint32 numVerts = GetNumberVertices();
        for (uint32 i = 0; i < numVerts; i++)
        {
            CGeomVertex *pv = GetVertex(i);
            // Convert vert to -0.5..+0.5 range and then rescale to maintain aspect ratio
            if (!IsZero(dx))
                pv->pos.x = scalex * ((pv->pos.x - minx) / dx - 0.5f);
            if (!IsZero(dy))
                pv->pos.y = scaley * ((pv->pos.y - miny) / dy - 0.5f);
            if (!IsZero(dz))
                pv->pos.z = scalez * ((pv->pos.z - minz) / dz - 0.5f);
        }
        for (uint32 j = 0; j < GetNumberFaces(); j++)
        {
            CFace *pFace = GetFace(j);
            if (!IsZero(dx))
                pFace->m_center.x = scalex * ((pFace->m_center.x - minx) / dx - 0.5f);
            if (!IsZero(dy))
                pFace->m_center.y = scaley * ((pFace->m_center.y - miny) / dy - 0.5f);
            if (!IsZero(dz))
                pFace->m_center.z = scalez * ((pFace->m_center.z - minz) / dz - 0.5f);
        }
    }
    
    //**********************************************************************
    void CSubMesh::TriangulateViaEarClipping()
    {
        _ASSERT(FALSE); // Not yet implemented
    }

    //**********************************************************************
    void CSubMesh::TriangulateViaPointInsertion()
    {
        _ASSERT(FALSE); // Not yet implemented
    }

    //**********************************************************************
    // Method: Triangulate
    // See <ISubMesh::Triangulate>
    //**********************************************************************
    void CSubMesh::Triangulate(ETriangulateMethod method)
    {
        switch (method)
        {
        case ETriangulateMethod::EarClipping:
            TriangulateViaEarClipping();
            break;
        case ETriangulateMethod::PointInsertion:
            TriangulateViaPointInsertion();
            break;
        }
    }

    //**********************************************************************
    // Method: ComputeNormals
    // See <ISubMesh::ComputeNormals>
    //**********************************************************************
    void CSubMesh::ComputeNormals()
    {
        for (size_t i = 0; i < m_vertices.size(); i++)
            m_vertices[i]->norm = Vector3(0.0f, 0.0f, 0.0f);
        for (size_t i = 0; i < m_faces.size(); i++)
        {
            CFace *pFace = m_faces[i];
            pFace->ComputeFaceNormal();
            Vector3 faceNorm = pFace->GetNormal();
            for (size_t j = 0; j < pFace->m_vertices.size(); j++)
                pFace->m_vertices[j]->norm += faceNorm;
        }
        for (size_t i = 0; i < m_vertices.size(); i++)
            m_vertices[i]->norm.Normalize();
    }

    //**********************************************************************
    // Method: VertexToIndex
    // See <ISubMesh::VertexToIndex>
    //**********************************************************************
    uint32 CSubMesh::VertexToIndex(CGeomVertex *pVertex)
    {
        return pVertex->index;
    }

    //**********************************************************************
    // Method: EdgeToIndex
    // See <ISubMesh::EdgeToIndex>
    //**********************************************************************
    uint32 CSubMesh::EdgeToIndex(CHalfEdge *pEdge)
    {
        return pEdge->index;
    };
    
    //**********************************************************************
    // Method: FaceToIndex
    // See <ISubMesh::FaceToIndex>
    //**********************************************************************
    uint32 CSubMesh::FaceToIndex(CFace *pFace)
    {
        return pFace->index;
    };

    CRefObj<ISubMesh> CreateEmptySubMesh()
    {
        return CRefObj<ISubMesh>(new CSubMesh());
    }

    CRefObj<ISubMesh> CreateSubMesh(std::vector<CGeomVertex> &verts, std::vector<int> &faces, uint32 materialID)
    {
        CMeshConstructor meshConstructor;
        meshConstructor.SubMeshOpen();
        for (size_t i = 0; i < faces.size();)
        {
            meshConstructor.FaceOpen();
            uint32 vIndex = faces[i++];
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            vIndex = faces[i++];
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            vIndex = faces[i++];
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            meshConstructor.FaceClose();
        }
        CRefObj<ISubMesh> spSubMesh = meshConstructor.SubMeshClose();
        spSubMesh->SetMaterialID(materialID);
        return spSubMesh;
    }

    CRefObj<ISubMesh> CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<int> &faces,
        uint32 materialID)
    {
        std::vector<Vector3> norms;
        std::vector<Vector2> uvs;
        return CreateSubMesh(vertPos, norms, uvs, faces, EVertexFlags::HasPosition, materialID);
    }

    CRefObj<ISubMesh> CreateSubMesh(std::vector<Vector3> &vertPos,
                                  std::vector<Vector3> &vertNorms,
                                  std::vector<Vector2> &vertUVs, 
                                  std::vector<int> &faces, 
                                  EVertexFlags flags,
                                  uint32 materialID)
    {
        CMeshConstructor meshConstructor;
        meshConstructor.SubMeshOpen();
        for (size_t i = 0; i < faces.size(); i += 3)
        {
            meshConstructor.FaceOpen();
            uint32 vIndex = faces[i];
            Vector3 norm = (vIndex < vertNorms.size()) ? vertNorms[vIndex] : Vector3(0.0f, 0.0f, 0.0f);
            Vector2 uv = (vIndex < vertUVs.size()) ? vertUVs[vIndex] : Vector2(0.0f, 0.0f);
            meshConstructor.VertexAdd(vertPos[vIndex], norm, uv);
            vIndex = faces[i + 1];
            norm = (vIndex < vertNorms.size()) ? vertNorms[vIndex] : Vector3(0.0f, 0.0f, 0.0f);
            uv = (vIndex < vertUVs.size()) ? vertUVs[vIndex] : Vector2(0.0f, 0.0f);
            meshConstructor.VertexAdd(vertPos[vIndex], norm, uv);
            vIndex = faces[i + 2];
            norm = (vIndex < vertNorms.size()) ? vertNorms[vIndex] : Vector3(0.0f, 0.0f, 0.0f);
            uv = (vIndex < vertUVs.size()) ? vertUVs[vIndex] : Vector2(0.0f, 0.0f);
            meshConstructor.VertexAdd(vertPos[vIndex], norm, uv);
            meshConstructor.FaceClose();
        }
        CRefObj<ISubMesh> spSubMesh = meshConstructor.SubMeshClose();
        spSubMesh->SetVertexFlags(flags);
        spSubMesh->SetMaterialID(materialID);
        return spSubMesh;
    }

    //**********************************************************************
    // Method: AllocateFace
    // See <CSubMesh::AllocateFace>
    //**********************************************************************
    CFace *CSubMesh::AllocateFace()
    {
        CFace *pFace = m_faceAllocator.Allocate();
        pFace->index = (int)m_faces.size();
        pFace->m_center = Vector3(0.0f, 0.0f, 0.0f);
        m_faces.push_back(pFace);
        return m_faces[m_faces.size() - 1];
    }

    //**********************************************************************
    // Method: AllocateGeomVertex
    // See <CSubMesh::AllocateGeomVertex>
    //**********************************************************************
    CGeomVertex *CSubMesh::AllocateGeomVertex(Vector3 &pos, Vector3 &normal, Vector2 &uv)
    {
        CGeomVertex *pVert = m_vertexAllocator.Allocate();
        pVert->pos = pos;
        pVert->norm = normal;
        pVert->uvs[0] = uv;
        int index = pVert->index = (int)m_vertices.size();
        m_vertices.push_back(pVert);
        CGeomVertex *pVertex = m_vertices[index];
        m_spKDTree->AddPoint(pos, pVertex);
        m_bbox.AddPoint(pos);
        return pVertex;
    }

    //**********************************************************************
    CFaceVertex *CSubMesh::AllocateFaceVertex(CFace * /*face*/, Vector3 &normal, Vector2 &uv)
    {
        CFaceVertex *pVert = m_faceVertexAllocator.Allocate();
        pVert->norm = normal;
        pVert->uvs[0] = uv;
        int index = pVert->index = (int)m_vertices.size();
        m_faceVertices.push_back(pVert);
        return m_faceVertices[index];
    }

    //**********************************************************************
    // Method: AllocateEdge
    // See <CSubMesh::AllocateEdge>
    //**********************************************************************
    CHalfEdge *CSubMesh::AllocateEdge(CGeomVertex *pHead, CGeomVertex *pTail)
    {
        CHalfEdge *pLeftHalf = m_edgeAllocator.Allocate();
        CHalfEdge *pRightHalf = m_edgeAllocator.Allocate();
        pLeftHalf->m_pOpposite = pRightHalf;
        pLeftHalf->m_pFace = nullptr;
        pLeftHalf->m_pNext = pRightHalf;
        pLeftHalf->m_pPrev = pRightHalf;
        pRightHalf->m_pOpposite = pLeftHalf;
        pRightHalf->m_pNext = pLeftHalf;
        pRightHalf->m_pPrev = pLeftHalf;
        pRightHalf->m_pFace = nullptr;
        pLeftHalf->m_pVertex = pHead;
        pRightHalf->m_pVertex = pTail;
        pLeftHalf->m_smoothingGroup = 0;
        pRightHalf->m_smoothingGroup = 0;
        pRightHalf->index = (int)m_edges.size();
        pLeftHalf->index = (int)m_edges.size()+1;
        m_edges.push_back(pRightHalf);
        m_edges.push_back(pLeftHalf);
        
        // Use cantor pairing to come up with hash value
        // For complete description of cantor pairing see:
        // https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
        uint64 addr1 = (uint64)pHead;
        uint64 addr2 = (uint64)pTail;
        uint64 value = ((addr1 + addr2) * (addr1 + addr2 + 1)) / 2 + addr2;

        m_vertToEdge[value] = pLeftHalf;
        return m_edges[m_edges.size() - 1];
    }

    //**********************************************************************
    // Method: FindEdge
    // See <CSubMesh::FindEdge>
    //**********************************************************************
    CHalfEdge *CSubMesh::FindEdge(CGeomVertex *pHead, CGeomVertex *pTail)
    {
        // Use cantor pairing to come up with hash value
        // For complete description of cantor pairing see:
        // https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
        uint64 addr1 = (uint64)pHead;
        uint64 addr2 = (uint64)pTail;
        uint64 value = ((addr1 + addr2) * (addr1 + addr2 + 1)) / 2 + addr2;
        CHalfEdge *pEdge = m_vertToEdge[value];
        if (pEdge == nullptr || pEdge->GetHeadVertex() == pHead)
            return pEdge;
        return pEdge->GetOppositeEdge();
    }

    //**********************************************************************
    // Method: FindVertex
    // See <CSubMesh::FindVertex>
    //**********************************************************************
    CGeomVertex *CSubMesh::FindVertex(Vector3 &pos, Vector3 *pNorm, Vector2 *pUV)
    {
        void *data;
        if (m_spKDTree->FindPoint(pos, [pNorm, pUV](void *data) -> bool {
                CGeomVertex *pVertex = (CGeomVertex*)data;
                if (pNorm && !pVertex->norm.IsEq(*pNorm))
                    return false;
                if (pUV && !pVertex->uvs[0].IsEq(*pUV))
                    return false;
                return true;
            }, &data))
        {
            return (CGeomVertex*)data;
        }
        return nullptr;
    }

    //**********************************************************************
    // Method: FindNextEdgeAroundVertex
    // See <CSubMesh::FindNextEdgeAroundVertex>
    //**********************************************************************
    CHalfEdge *CSubMesh::FindNextEdgeAroundVertex(CGeomVertex *pVert, CHalfEdge *pEdge, bool entering, bool clockwise)
    {
        if (pEdge == nullptr)
        {
            // Just return any edge
            for (size_t i = 0; i < m_edges.size(); i++)
            {
                CHalfEdge *pNext = m_edges[i];
                if (entering && pNext->m_pVertex == pVert)
                    return pNext;
                if (!entering && pNext->m_pOpposite->m_pVertex == pVert)
                    return pNext->m_pOpposite;
            }
            return nullptr;
        }
        if (entering)
        {
            _ASSERTE(pEdge->m_pVertex == pVert);
            if (clockwise)
            {
                _ASSERTE(pEdge->m_pOpposite);
                _ASSERTE(pEdge->m_pNext);
                return pEdge->m_pNext->m_pOpposite;
            }
            _ASSERTE(pEdge->m_pOpposite);
            return pEdge->m_pOpposite->m_pPrev;
        }
        else
        {
            _ASSERTE(pEdge->m_pOpposite);
            _ASSERTE(pEdge->m_pOpposite->m_pVertex == pVert);
            if (clockwise)
            {
                _ASSERTE(pEdge->m_pOpposite);
                return pEdge->m_pOpposite->m_pNext;
            }
            _ASSERTE(pEdge->m_pPrev);
            return pEdge->m_pPrev->m_pOpposite;
        }
    }

    //**********************************************************************
    // Method: LinkEdges
    // See <CSubMesh::LinkEdges>
    //**********************************************************************
    void CSubMesh::LinkEdges(CHalfEdge *pPrev, CHalfEdge *pCur)
    {
        if (pPrev == nullptr)
            return;
        pPrev->m_pNext->m_pPrev = pCur->m_pPrev;
        pCur->m_pPrev->m_pNext = pPrev->m_pNext;
        pPrev->m_pNext = pCur;
        pCur->m_pPrev = pPrev;
    }

    //**********************************************************************
    // Method: CheckConsistency
    // See <CSubMesh::CheckConsistency>
    //**********************************************************************
    void CSubMesh::CheckConsistency()
    {
        for (size_t i = 0; i < m_faces.size(); i++)
        {
            CFace *pFace = m_faces[i];
            std::vector<bool> touchVertex(m_vertices.size(), false);
            CHalfEdge *pEdge = pFace->m_pEdge;
            CHalfEdge *pFirst = pEdge;
            CHalfEdge *pPrev = pFace->m_pEdge->m_pPrev;
            CGeomVertex *pPrevVert = pPrev->m_pVertex;
            do {
                int vertIndex = VertexToIndex(pEdge->m_pVertex);
                _ASSERT(!touchVertex[vertIndex]);
                touchVertex[vertIndex] = true;
                _ASSERT(pEdge->m_pPrev == pPrev);
                _ASSERT(pPrev->m_pNext == pEdge);
                _ASSERT(pPrev->m_pVertex == pPrevVert);
                _ASSERT(pEdge->m_pFace == pFace);
                pPrevVert = pEdge->m_pVertex;
                pPrev = pEdge;
                pEdge = pEdge->m_pNext;
            } while (pEdge != pFirst);
            for (size_t j = 0; j < pFace->m_vertices.size(); j++)
                _ASSERT(touchVertex[pFace->m_vertices[j]->index]);
        }
    }

    //**********************************************************************
    // Method: Load
    // See <ISubMesh::Load>
    //**********************************************************************
    void CSubMesh::Load(IStream *pStream)
    {
        ULONG bytesRead;
        int version;
        CT(pStream->Read(&version, sizeof(version), &bytesRead));
        _ASSERT(version == 1);

        // Read vertices
        uint32 numVerts;
        CT(pStream->Read(&numVerts, sizeof(numVerts), &bytesRead));
        for (uint32 i = 0; i < numVerts; i++)
        {
            CGeomVertex *pVertex = m_vertexAllocator.Allocate();
            CT(pStream->Read(pVertex, sizeof(CGeomVertex), &bytesRead));
            m_vertices.push_back(pVertex);
        }

        // Read edges
        uint32 numEdges;
        CT(pStream->Read(&numEdges, sizeof(numEdges), &bytesRead));
        for (uint32 i = 0; i < (uint32)numEdges; i++)
            m_edges.emplace_back(new CHalfEdge());
        uint32 numFaces;
        CT(pStream->Read(&numFaces, sizeof(numFaces), &bytesRead));
        for (uint32 i = 0; i < (uint32)numFaces; i++)
            m_faces.emplace_back(new CFace());
        for (uint32 i = 0; i < (uint32)numEdges; i++)
        {
            uint32 index;
            CHalfEdge *pEdge = m_edges[i];
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pNext = m_edges[index];
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pPrev = m_edges[index];
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pOpposite = m_edges[index];
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pVertex = m_vertices[index];
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pFace = m_faces[index];
            CT(pStream->Read(&pEdge->m_smoothingGroup, sizeof(pEdge->m_smoothingGroup), &bytesRead));
            CT(pStream->Read(&pEdge->index, sizeof(pEdge->index), &bytesRead));
        }
        
        // Read faces
        for (uint32 i = 0; i < numFaces; i++)
        {
            CFace *pFace = m_faces[i];
            CT(pStream->Read(&pFace->m_normal, sizeof(pFace->m_normal), &bytesRead));
            CT(pStream->Read(&pFace->m_center, sizeof(pFace->m_center), &bytesRead));
            uint32 edgeIndex;
            CT(pStream->Read(&edgeIndex, sizeof(edgeIndex), &bytesRead));
            pFace->m_pEdge = m_edges[edgeIndex];

            uint32 numVertices;
            CT(pStream->Read(&numVertices, sizeof(numVertices), &bytesRead));
            for (uint32 j = 0; j < numVertices; j++)
            {
                uint32 vertIndex;
                CT(pStream->Read(&vertIndex, sizeof(vertIndex), &bytesRead));
                pFace->m_vertices.push_back(m_vertices[vertIndex]);
            }
            CT(pStream->Read(&pFace->index, sizeof(pFace->index), &bytesRead));
        }

        // Read miscellaneous fields
        CT(pStream->Read(&m_bbox, sizeof(m_bbox), &bytesRead));
        CT(pStream->Read(&m_materialID, sizeof(m_materialID), &bytesRead));
        uint32 flags;
        CT(pStream->Read(&flags, sizeof(flags), &bytesRead));
        m_vertexFlags = (EVertexFlags)flags;
        CT(pStream->Read(&flags, sizeof(flags), &bytesRead));
        m_meshFlags = (EMeshFlags)flags;
    }

    //**********************************************************************
    // Method: Load
    // See <ISubMesh::Store>
    //**********************************************************************
    void CSubMesh::Store(IStream *pStream)
    {
        ULONG bytesWritten;
        const int c_Version = 1;
        CT(pStream->Write(&c_Version, sizeof(c_Version), &bytesWritten));

        // Write out vertices
        uint32 numVerts = (uint32)m_vertices.size();
        CT(pStream->Write(&numVerts, sizeof(numVerts), &bytesWritten));
        for (uint32 i = 0; i < (uint32)numVerts; i++)
            CT(pStream->Write(m_vertices[i], sizeof(CGeomVertex), &bytesWritten));
        
        // Write out edges
        uint32 numEdges = (uint32)m_edges.size();
        CT(pStream->Write(&numEdges, sizeof(numEdges), &bytesWritten));
        uint32 numFaces = (uint32)m_faces.size();
        CT(pStream->Write(&numFaces, sizeof(numFaces), &bytesWritten));
        for (uint32 i = 0; i < (uint32)m_edges.size(); i++)
        {
            CHalfEdge *pEdge = m_edges[i];
            uint32 index = EdgeToIndex(pEdge->m_pNext);
            CT(pStream->Write(&index, sizeof(index), &bytesWritten));
            index = EdgeToIndex(pEdge->m_pPrev);
            CT(pStream->Write(&index, sizeof(index), &bytesWritten));
            index = EdgeToIndex(pEdge->m_pOpposite);
            CT(pStream->Write(&index, sizeof(index), &bytesWritten));
            index = VertexToIndex(pEdge->m_pVertex);
            CT(pStream->Write(&index, sizeof(index), &bytesWritten));
            index = FaceToIndex(pEdge->m_pFace);
            CT(pStream->Write(&index, sizeof(index), &bytesWritten));
            CT(pStream->Write(&pEdge->m_smoothingGroup, sizeof(pEdge->m_smoothingGroup), &bytesWritten));
            CT(pStream->Write(&pEdge->index, sizeof(pEdge->index), &bytesWritten));
        }

        // Write out faces
        for (uint32 i = 0; i < (uint32)numFaces; i++)
        {
            CFace *pFace = m_faces[i];
            CT(pStream->Write(&pFace->m_normal, sizeof(pFace->m_normal), &bytesWritten));
            CT(pStream->Write(&pFace->m_center, sizeof(pFace->m_center), &bytesWritten));
            uint32 edgeIndex = EdgeToIndex(pFace->m_pEdge);
            CT(pStream->Write(&edgeIndex, sizeof(edgeIndex), &bytesWritten));
            uint32 numVertices = (uint32)pFace->m_vertices.size();
            CT(pStream->Write(&numVertices, sizeof(numVertices), &bytesWritten));
            for (uint32 j = 0; j < numVertices; j++)
            {
                uint32 vertIndex = VertexToIndex(pFace->m_vertices[j]);
                CT(pStream->Write(&vertIndex, sizeof(vertIndex), &bytesWritten));
            }
            CT(pStream->Write(&pFace->index, sizeof(pFace->index), &bytesWritten));
        }

        // Write other miscellaneous fields
        CT(pStream->Write(&m_bbox, sizeof(m_bbox), &bytesWritten));
        CT(pStream->Write(&m_materialID, sizeof(m_materialID), &bytesWritten));
        uint32 flags = m_vertexFlags;
        CT(pStream->Write(&flags, sizeof(flags), &bytesWritten));
        flags = m_meshFlags;
        CT(pStream->Write(&flags, sizeof(flags), &bytesWritten));
    }
}
