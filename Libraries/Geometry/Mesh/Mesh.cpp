//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\MaterialAttrib.h"
#include "Rendering\Caustic\PathTrace.h"
#include "Base\Math\Ray.h"

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
        CGeomVertex *pCurVertex = pEdge->GetHeadVertex();
        CGeomVertex *pPrevVertex = pEdge->GetTailVertex();
        do {
            m_normal.x += ((pPrevVertex->pos.y - pCurVertex->pos.y) * (pPrevVertex->pos.z + pCurVertex->pos.z));
            m_normal.y += ((pPrevVertex->pos.z - pCurVertex->pos.z) * (pPrevVertex->pos.x + pCurVertex->pos.x));
            m_normal.z += ((pPrevVertex->pos.x - pCurVertex->pos.x) * (pPrevVertex->pos.y + pCurVertex->pos.y));
            pEdge = pEdge->GetNextEdge();
            pPrevVertex = pCurVertex;
            pCurVertex = pEdge->GetHeadVertex();
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
    // Computes the intersection of a ray with a mesh
    //
    // Parameters:
    // pCtx - path tracing context
    // ray - ray direction to trace
    // pRadiance - Returns the radiance returned along 'ray'
    //**********************************************************************
    bool CMesh::RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial)
    {
        bool hitMesh = false;
        for (size_t i = 0; i < m_subMeshes.size(); i++)
            if (m_subMeshes[i]->RayIntersect(ray, pIntersection, pMaterial, m_materials))
                hitMesh = true;
        return hitMesh;
    }

    //**********************************************************************
    // Method: GetSubMesh
    // Returns the Nth submesh
    //
    // Parameters:
    // index - Index of submesh to retrieve
    //
    // Returns:
    // Returns the Nth submesh
    //**********************************************************************
    CRefObj<ISubMesh> CMesh::GetSubMesh(uint32 index)
    {
        return m_subMeshes[index];
    }

    //**********************************************************************
    // Method: AddSubMesh
    // Adds a submesh to the mesh
    //
    // Parameters:
    // pSubMesh - Adds the specified submesh to our mesh
    //**********************************************************************
    void CMesh::AddSubMesh(ISubMesh *pSubMesh)
    {
        m_subMeshes.push_back(CRefObj<ISubMesh>(pSubMesh));
    }

    //**********************************************************************
    // Method: GetBBox
    // Returns the bounding box for the mesh
    //
    // Parameters:
    // pBBox - Returns the bounding box of the mesh
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
    // Rescales a mesh so that it has unit size
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
    // Sets the mesh's material list
    //
    // Parameters:
    // materials - Assigns the mesh the specified list of materials
    //**********************************************************************
    void CMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials)
    {
        m_materials.clear();
        for (auto p : materials)
            m_materials.push_back(p);
    }

    //**********************************************************************
    // Method: GetNumberMaterials
    // Returns number of materials currently assigned to mesh
    //
    // Returns:
    // Returns number of materials
    //**********************************************************************
    uint32 CMesh::GetNumberMaterials()
    {
        return (uint32)m_materials.size();
    }

    //**********************************************************************
    // Method: GetMaterial
    // Returns the Nth material
    //
    // Parameters:
    // materialID - Index of material to retrieve
    //
    // Returns:
    // Returns the Nth material
    //**********************************************************************
    CRefObj<IMaterialAttrib> CMesh::GetMaterial(uint32 materialID)
    {
        if (materialID < m_materials.size())
            return m_materials[materialID];
        return CRefObj<IMaterialAttrib>(nullptr);
    }

    //**********************************************************************
    // Method: Load
    // Loads a previously save Mesh
    //
    // Parameters:
    // pStream - Loads the material from the specified stream
    //**********************************************************************
    void CMesh::Load(IStream *pStream)
    {
        uint32 numSubMeshes;
        ULONG bytesWritten;
        CT(pStream->Read(&numSubMeshes, sizeof(numSubMeshes), &bytesWritten));
        for (uint32 i = 0; i < numSubMeshes; i++)
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
        uint32 numSubMeshes = (uint32)NumberSubMeshes();
        DWORD bytesWritten;
        CT(pStream->Write(&numSubMeshes, sizeof(numSubMeshes), &bytesWritten));
        for (uint32 i = 0; i < numSubMeshes; i++)
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
    
    //**********************************************************************
    // Function: CreateEmptyMesh
    // Returns an empty mesh
    //
    // Returns:
    // Returns the newly created mesh object
    //**********************************************************************
    CAUSTICAPI CRefObj<IMesh> CreateEmptyMesh()
    {
        return CRefObj<IMesh>(new CMesh());
    }
}
