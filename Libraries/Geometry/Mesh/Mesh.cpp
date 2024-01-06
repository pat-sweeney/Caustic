//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <float.h>
#include <vector>

module Geometry.Mesh.Mesh;
import Base.Core.Core;
import Base.Core.Error;
import Base.Math.Ray;
import Geometry.Mesh.IMesh;

//**********************************************************************
// File: Mesh.cpp
// Contains the methods used to implement the CMesh object.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Method: ComputeFaceNormal
    // Computes the normal for a face using Newell's method (so convex
    // polygons are handled correctly)
    //**********************************************************************
    void CFace::ComputeFaceNormal()
    {
        m_normal.x = 0.0F;
        m_normal.y = 0.0F;
        m_normal.z = 0.0F;
        CHalfEdge *pEdge = m_pEdge;
        CHalfEdge *pStartEdge = pEdge;
        CGeomVertex *pCurVertex = pEdge->m_pVertex;
        CGeomVertex *pPrevVertex = pEdge->m_pPrev->m_pVertex;
        do {
            m_normal.x += ((pPrevVertex->pos.y - pCurVertex->pos.y) * (pPrevVertex->pos.z + pCurVertex->pos.z));
            m_normal.y += ((pPrevVertex->pos.z - pCurVertex->pos.z) * (pPrevVertex->pos.x + pCurVertex->pos.x));
            m_normal.z += ((pPrevVertex->pos.x - pCurVertex->pos.x) * (pPrevVertex->pos.y + pCurVertex->pos.y));
            pEdge = pEdge->m_pNext;
            pPrevVertex = pCurVertex;
            pCurVertex = pEdge->m_pVertex;
        } while (pEdge != pStartEdge);
        m_normal.Normalize();
    }

    //**********************************************************************
    // Method: IsDegenerate
    // Returns whether the face is degenerate (based on the normal vector)
    //**********************************************************************
    bool CFace::IsDegenerate()
    {
        return IsZero(m_normal.x) && IsZero(m_normal.y) && IsZero(m_normal.z);
    }

    //**********************************************************************
    // Constructor: CMesh
    // Default ctor for our mesh object. A mesh is simply a collection
    // of submeshes (submeshes define geometry and material attributes)
    //**********************************************************************
    CMesh::CMesh()
    {
    }

    //**********************************************************************
    // Method: RayIntersect
    // See <IMesh::RayIntersect>
    //**********************************************************************
    bool CMesh::RayIntersect(Ray3& ray, RayIntersect3* pIntersection, uint32_t* pMaterialID)
    {
        bool hitMesh = false;
        for (size_t i = 0; i < m_subMeshes.size(); i++)
            if (m_subMeshes[i]->RayIntersect(ray, pIntersection, pMaterialID))
                hitMesh = true;
        return hitMesh;
    }

    //**********************************************************************
    // Method: GetSubMesh
    // See <IMesh::GetSubMesh>
    //**********************************************************************
    CRefObj<ISubMesh> CMesh::GetSubMesh(uint32_t index)
    {
        return m_subMeshes[index];
    }

    //**********************************************************************
    // Method: AddSubMesh
    // See <IMesh::AddSubMesh>
    //**********************************************************************
    void CMesh::AddSubMesh(ISubMesh *pSubMesh)
    {
        m_subMeshes.push_back(CRefObj<ISubMesh>(pSubMesh));
    }

    //**********************************************************************
    // Method: GetBBox
    // See <IMesh::GetBBox>
    //**********************************************************************
    void CMesh::GetBBox(BBox3 *pBBox)
    {
        pBBox->minPt = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
        pBBox->maxPt = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (size_t i = 0; i < m_subMeshes.size(); i++)
        {
            BBox3 bb = m_subMeshes[i]->GetBBox();
            pBBox->AddPoint(bb.minPt.x, bb.minPt.y, bb.minPt.z);
            pBBox->AddPoint(bb.maxPt.x, bb.maxPt.y, bb.maxPt.z);
        }
    }

    //**********************************************************************
    // Method: Normalize
    // See <IMesh::Normalize>
    //**********************************************************************
    void CMesh::Normalize()
    {
        BBox3 bb;
        GetBBox(&bb);
        for (size_t i = 0; i < m_subMeshes.size(); i++)
            m_subMeshes[i]->Normalize(bb);
    }

    //**********************************************************************
    // Method: SetMaterials
    // See <IMesh::SetMaterials>
    //**********************************************************************
    void CMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials)
    {
        m_materials.clear();
        for (auto p : materials)
            m_materials.push_back(p);
    }

    //**********************************************************************
    // Method: GetNumberMaterials
    // See <IMesh::GetNumberMaterials>
    //**********************************************************************
    uint32_t CMesh::GetNumberMaterials()
    {
        return (uint32_t)m_materials.size();
    }

    //**********************************************************************
    // Method: GetMaterial
    // See <IMesh::GetMaterial>
    //**********************************************************************
    CRefObj<IMaterialAttrib> CMesh::GetMaterial(uint32_t materialID)
    {
        if (materialID < m_materials.size())
            return m_materials[materialID];
        return CRefObj<IMaterialAttrib>(nullptr);
    }

    //**********************************************************************
    // Method: Load
    // Loads a previously saved Mesh
    //
    // Parameters:
    // pStream - Loads the material from the specified stream
    //**********************************************************************
    void CMesh::Load(IStream *pStream)
    {
        uint32_t numSubMeshes;
        ULONG bytesWritten;
        CT(pStream->Read(&numSubMeshes, sizeof(numSubMeshes), &bytesWritten));
        for (uint32_t i = 0; i < numSubMeshes; i++)
        {
            CRefObj<ISubMesh> spSubMesh(new CSubMesh());
            spSubMesh->Load(pStream);
            m_subMeshes.push_back(spSubMesh);
        }
    }

    //**********************************************************************
    // Method: Store
    // Stores a mesh to a stream
    //
    // Parameters:
    // pStream - Stores the material to the specified stream
    //**********************************************************************
    void CMesh::Store(IStream *pStream)
    {
        uint32_t numSubMeshes = (uint32_t)NumberSubMeshes();
        DWORD bytesWritten;
        CT(pStream->Write(&numSubMeshes, sizeof(numSubMeshes), &bytesWritten));
        for (uint32_t i = 0; i < numSubMeshes; i++)
        {
            CRefObj<ISubMesh> spSubMesh = GetSubMesh(i);
            spSubMesh->Store(pStream);
        }
    }

    //**********************************************************************
    // Method: ComputeNormals
    // Computes normal vectors for each vertex on the mesh
    //**********************************************************************
    void CMesh::ComputeNormals()
    {
        for (auto pSubMesh : m_subMeshes)
            pSubMesh->ComputeNormals();
    }
}
