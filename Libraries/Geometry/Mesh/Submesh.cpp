//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\MeshConstructor.h"
#include "Base\Core\error.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Normalize rescales a mesh so fits inside a -0.5..+0.5 bounding box
    //!
    //! \param bbox Bounding box of the mesh
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
    //! \Brief This method converts a mesh from arbitrary polygons into
    //! a triangulated mesh.
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
    //! \Brief This method will compute all the vertex normals
    //! by averaging the surrounding face normals.
    //! 
    //! This method will compute all the vertex normals
    //! on the mesh by summing and normalizing the face vertices
    //! around each vertex (via Newell's method). NOTE: This makes
    //! an implicit assumption that each face is planar. This is not
    //! always the case and can thus result in incorrect normals (for
    //! instance, the faces from CreateSphere() will not be planar).
    //! To prevent incorrect normals, it is first recommended that
    //! the mesh be triangulated first via CSubMesh::Triangulate().
    //**********************************************************************
    void CSubMesh::ComputeNormals()
    {
        for (size_t i = 0; i < m_vertices.size(); i++)
            m_vertices[i]->norm = Vector3(0.0f, 0.0f, 0.0f);
        for (size_t i = 0; i < m_faces.size(); i++)
        {
            CFace *pFace = m_faces[i].get();
            pFace->ComputeFaceNormal();
            Vector3 faceNorm = pFace->GetNormal();
            for (size_t j = 0; j < pFace->m_vertices.size(); j++)
                pFace->m_vertices[j]->norm += faceNorm;
        }
        for (size_t i = 0; i < m_vertices.size(); i++)
            m_vertices[i]->norm.Normalize();
    }

    uint32 CSubMesh::VertexToIndex(CGeomVertex *pVertex)
    {
        return pVertex->index;
//        uint32 index = c_InvalidIndex;
//        for (uint32 j = 0; j < (uint32)m_vertices.size(); j++)
//        {
//            if (m_vertices[j].get() == pVertex)
//            {
//                index = j;
//                break;
//            }
//        }
//        return index;
    }

    uint32 CSubMesh::EdgeToIndex(CHalfEdge *pEdge)
    {
        return pEdge->index;
//        uint32 index = c_InvalidIndex;
//        for (uint32 i = 0; i < (uint32)m_edges.size(); i++)
//            if (m_edges[i].get() == pEdge)
//            {
//                index = i;
//                break;
//            }
//        return index;
    };
    
    uint32 CSubMesh::FaceToIndex(CFace *pFace)
    {
        return pFace->index;
//        uint32 index = c_InvalidIndex;
//        for (uint32 j = 0; j < (uint32)m_faces.size(); j++)
//        {
//            if (m_faces[j].get() == pFace)
//            {
//                index = j;
//                break;
//            }
//        }
//        return index;
    };

    CAUSTICAPI void CreateEmptySubMesh(ISubMesh **ppSubMesh)
    {
        std::unique_ptr<CSubMesh> pSubMesh(new CSubMesh());
        *ppSubMesh = pSubMesh.release();
        (*ppSubMesh)->AddRef();
    }

    CAUSTICAPI void CreateSubMesh(std::vector<CGeomVertex> &verts,
        std::vector<int> &faces,
        uint32 materialID,
        ISubMesh **ppSubMesh)
    {
        CMeshConstructor meshConstructor;
        meshConstructor.SubMeshOpen();
        for (size_t i = 0; i < faces.size();)
        {
            meshConstructor.FaceOpen();
            Vector3 norm(0.0f, 0.0f, 0.0f);
            uint32 vIndex = faces[i++];
            Caustic::Vector3 n0 = verts[vIndex].norm;
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            vIndex = faces[i++];
            Caustic::Vector3 n1 = verts[vIndex].norm;
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            vIndex = faces[i++];
            Caustic::Vector3 n2 = verts[vIndex].norm;
            meshConstructor.VertexAdd(verts[vIndex].pos, verts[vIndex].norm, verts[vIndex].uvs[0]);
            meshConstructor.FaceClose();
            // Check if normals agree
        }
        CRefObj<ISubMesh> spSubMesh;
        meshConstructor.SubMeshClose(&spSubMesh);
        spSubMesh->SetMaterialID(materialID);
        *ppSubMesh = spSubMesh.Detach();
    }

    CAUSTICAPI void CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<int> &faces,
        uint32 materialID,
        ISubMesh **ppSubMesh)
    {
        std::vector<Vector3> norms;
        std::vector<Vector2> uvs;
        CreateSubMesh(vertPos, norms, uvs, faces, EVertexFlags::HasPosition, materialID, ppSubMesh);
    }

    CAUSTICAPI void CreateSubMesh(std::vector<Vector3> &vertPos,
                                  std::vector<Vector3> &vertNorms,
                                  std::vector<Vector2> &vertUVs, 
                                  std::vector<int> &faces, 
                                  EVertexFlags flags,
                                  uint32 materialID,
                                  ISubMesh **ppSubMesh)
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
        CRefObj<ISubMesh> spSubMesh;
        meshConstructor.SubMeshClose(&spSubMesh);
        spSubMesh->SetVertexFlags(flags);
        spSubMesh->SetMaterialID(materialID);
        *ppSubMesh = spSubMesh.Detach();
    }

    //**********************************************************************
    //! \brief AllocateFace allocates a new face and places in our face list.
    //**********************************************************************
    CFace *CSubMesh::AllocateFace()
    {
        std::unique_ptr<CFace> spFace(new CFace());
        spFace->index = (int)m_faces.size();
        spFace->m_center = Vector3(0.0f, 0.0f, 0.0f);
        m_faces.emplace_back(spFace.release());
        return m_faces[m_faces.size() - 1].get();
    }

    //**********************************************************************
    //! \brief AllocateVertex allocates a new vertex given a position,
    //! normal, and UVs and places it in our vertex list.
    //**********************************************************************
    CGeomVertex *CSubMesh::AllocateGeomVertex(Vector3 &pos, Vector3 &normal, Vector2 &uv)
    {
        std::unique_ptr<CGeomVertex> spVert(new CGeomVertex());
        spVert->pos = pos;
        spVert->norm = normal;
        spVert->uvs[0] = uv;
        int index = spVert->index = (int)m_vertices.size();
        m_vertices.emplace_back(spVert.release());
        CGeomVertex *pVertex = m_vertices[index].get();
        m_spKDTree->AddPoint(pos, pVertex);
        m_bbox.AddPoint(pos);
        return pVertex;
    }

    //**********************************************************************
    CFaceVertex *CSubMesh::AllocateFaceVertex(CFace * /*face*/, Vector3 &normal, Vector2 &uv)
    {
        std::unique_ptr<CFaceVertex> spVert(new CFaceVertex());
        spVert->norm = normal;
        spVert->uvs[0] = uv;
        int index = spVert->index = (int)m_vertices.size();
        m_faceVertices.emplace_back(spVert.release());
        CFaceVertex *pVertex = m_faceVertices[index].get();
      // m_spKDTree->AddPoint(pos, pVertex);
      // m_bbox.AddPoint(pos);
        return pVertex;
    }

    //**********************************************************************
    //! \brief AllocateEdge allocates a new edge going from
    //! pTail->pHead. NOTE: This function does not insert
    //! the edge into our edge list. That occurs during the call
    //! to LinkEdges().
    //**********************************************************************
    CHalfEdge *CSubMesh::AllocateEdge(CGeomVertex *pHead, CGeomVertex *pTail)
    {
        std::unique_ptr<CHalfEdge> spLeftHalf(new CHalfEdge());
        std::unique_ptr<CHalfEdge> spRightHalf(new CHalfEdge());
        spLeftHalf->m_pOpposite = spRightHalf.get();
        spLeftHalf->m_pFace = nullptr;
        spLeftHalf->m_pNext = spRightHalf.get();
        spLeftHalf->m_pPrev = spRightHalf.get();
        spRightHalf->m_pOpposite = spLeftHalf.get();
        spRightHalf->m_pNext = spLeftHalf.get();
        spRightHalf->m_pPrev = spLeftHalf.get();
        spRightHalf->m_pFace = nullptr;
        spLeftHalf->m_pVertex = pHead;
        spRightHalf->m_pVertex = pTail;
        spLeftHalf->m_smoothingGroup = 0;
        spRightHalf->m_smoothingGroup = 0;
        spRightHalf->index = (int)m_edges.size();
        spLeftHalf->index = (int)m_edges.size()+1;
        m_edges.emplace_back(spRightHalf.release());
        m_edges.emplace_back(spLeftHalf.release());
        std::tuple<CGeomVertex*, CGeomVertex*> t = std::make_tuple(pTail, pHead);
        m_vertToEdge[t] = spLeftHalf.get();
        t = std::make_tuple(pHead, pTail);
        m_vertToEdge[t] = spRightHalf.get();
        return m_edges[m_edges.size() - 1].get();
    }

    //**********************************************************************
    //! \brief FindEdge returns the edge from pTail->pHead. If the edge does
    //! not exist then nullptr is returned.
    //**********************************************************************
    CHalfEdge *CSubMesh::FindEdge(CGeomVertex *pHead, CGeomVertex *pTail)
    {
        std::tuple<CGeomVertex*, CGeomVertex*> t = std::make_tuple(pTail, pHead);
        return m_vertToEdge[t];
//        for (int i = (int)m_edges.size() - 1; i >= 0; i--)
//        {
//            CHalfEdge *pHalf = m_edges[i].get();
//            if (pHalf->m_pVertex == pHead && pHalf->m_pPrev->m_pVertex == pTail)
//                return pHalf;
//        }
//        return nullptr;
    }

    //**********************************************************************
    //! \brief FindVertex will return a pointer to a vertex in our mesh
    //! that matches the specified values
    //**********************************************************************
    CGeomVertex *CSubMesh::FindVertex(Vector3 *pPos, Vector3 *pNorm, Vector2 *pUV)
    {
        void *data;
        if (m_spKDTree->FindPoint(*pPos, [pNorm, pUV](void *data) -> bool {
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
    //! \brief FindNextEdgeAroundVertex returns the next edge around the vertex
    //! in the given direction (clockwise or counter-clockwise) that is either
    //! entering or leaving the vertex.
    //**********************************************************************
    CHalfEdge *CSubMesh::FindNextEdgeAroundVertex(CGeomVertex *pVert, CHalfEdge *pEdge, bool entering, bool clockwise)
    {
        if (pEdge == nullptr)
        {
            // Just return any edge
            for (size_t i = 0; i < m_edges.size(); i++)
            {
                CHalfEdge *pNext = m_edges[i].get();
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
    //! \brief LinkEdges links two edges (full edges) together
    // We start with the picture at the top. The solid lines represent
    // HalfEdge's, while the dotted lines represent their m_pPrev/m_pNext
    // links. We are trying to add newEdge and link it to oldEdge (an existing
    // edge)
    //            ^|
    //        ...>|| <.......
    //        v   |V        .
    //  ---------> O pHead  .
    //  <---------          .
    //  oldEdge ^............
    //
    //            ^|
    //        ...>|| <.......
    //        v   |V        .
    //  ---------> O pHead  .
    //  <--------- ^|       .
    //  oldEdge    ||<.......
    //     newEdge ||
    //        ....>||<.......
    //        v    |V       .
    //  ---------> O pTail  .
    //  <---------          .
    //         ^.............
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

    void CSubMesh::CheckConsistency()
    {
        for (size_t i = 0; i < m_faces.size(); i++)
        {
            CFace *pFace = m_faces[i].get();
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
            std::unique_ptr<CGeomVertex> spVertex(new CGeomVertex());
            CT(pStream->Read(spVertex.get(), sizeof(CGeomVertex), &bytesRead));
            m_vertices.emplace_back(spVertex.release());
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
            CHalfEdge *pEdge = m_edges[i].get();
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pNext = m_edges[index].get();
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pPrev = m_edges[index].get();
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pOpposite = m_edges[index].get();
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pVertex = m_vertices[index].get();
            CT(pStream->Read(&index, sizeof(index), &bytesRead));
            pEdge->m_pFace = m_faces[index].get();
            CT(pStream->Read(&pEdge->m_smoothingGroup, sizeof(pEdge->m_smoothingGroup), &bytesRead));
            CT(pStream->Read(&pEdge->index, sizeof(pEdge->index), &bytesRead));
        }
        
        // Read faces
        for (uint32 i = 0; i < numFaces; i++)
        {
            CFace *pFace = m_faces[i].get();
            CT(pStream->Read(&pFace->m_normal, sizeof(pFace->m_normal), &bytesRead));
            CT(pStream->Read(&pFace->m_center, sizeof(pFace->m_center), &bytesRead));
            uint32 edgeIndex;
            CT(pStream->Read(&edgeIndex, sizeof(edgeIndex), &bytesRead));
            pFace->m_pEdge = m_edges[edgeIndex].get();

            uint32 numVertices;
            CT(pStream->Read(&numVertices, sizeof(numVertices), &bytesRead));
            for (uint32 j = 0; j < numVertices; j++)
            {
                uint32 vertIndex;
                CT(pStream->Read(&vertIndex, sizeof(vertIndex), &bytesRead));
                pFace->m_vertices.push_back(m_vertices[vertIndex].get());
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

    void CSubMesh::Store(IStream *pStream)
    {
        ULONG bytesWritten;
        const int c_Version = 1;
        CT(pStream->Write(&c_Version, sizeof(c_Version), &bytesWritten));

        // Write out vertices
        uint32 numVerts = (uint32)m_vertices.size();
        CT(pStream->Write(&numVerts, sizeof(numVerts), &bytesWritten));
        for (uint32 i = 0; i < (uint32)numVerts; i++)
            CT(pStream->Write(m_vertices[i].get(), sizeof(CGeomVertex), &bytesWritten));
        
        // Write out edges
        uint32 numEdges = (uint32)m_edges.size();
        CT(pStream->Write(&numEdges, sizeof(numEdges), &bytesWritten));
        uint32 numFaces = (uint32)m_faces.size();
        CT(pStream->Write(&numFaces, sizeof(numFaces), &bytesWritten));
        for (uint32 i = 0; i < (uint32)m_edges.size(); i++)
        {
            CHalfEdge *pEdge = m_edges[i].get();
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
            CFace *pFace = m_faces[i].get();
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
