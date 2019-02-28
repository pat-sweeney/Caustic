//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\Material.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Computes the normal for a face using Newell's method (so convex
    //! polygons are handled correctly)
    //**********************************************************************
    void CFace::ComputeFaceNormal()
    {
        m_normal.x = 0.0F;
        m_normal.y = 0.0F;
        m_normal.z = 0.0F;
        CHalfEdge *pEdge = m_pEdge;
        CHalfEdge *pStartEdge = pEdge;
        do {
            CGeomVertex *pCurVertex = pEdge->GetHeadVertex();
            CGeomVertex *pPrevVertex = pEdge->GetTailVertex();
            m_normal.x += ((pPrevVertex->pos.y - pCurVertex->pos.y) * (pPrevVertex->pos.z + pCurVertex->pos.z));
            m_normal.y += ((pPrevVertex->pos.z - pCurVertex->pos.z) * (pPrevVertex->pos.x + pCurVertex->pos.x));
            m_normal.z += ((pPrevVertex->pos.x - pCurVertex->pos.x) * (pPrevVertex->pos.y + pCurVertex->pos.y));
            pEdge = pEdge->GetNextEdge();
        } while (pEdge != pStartEdge);
        m_normal.Normalize();
    }

    //**********************************************************************
    //! \brief Returns whether the face is degenerate (based on the normal vector)
    //**********************************************************************
    bool CFace::IsDegenerate()
    {
        return IsZero(m_normal.x) && IsZero(m_normal.y) && IsZero(m_normal.z);
    }

    //**********************************************************************
    //! \brief Default ctor for our mesh object. A mesh is simply a collection
    //! of submeshes (submeshes define geometry and material attributes)
    //**********************************************************************
    CMesh::CMesh()
    {
    }

    //**********************************************************************
    //! \brief Returns the Nth submesh
    //! @param [in] index Index of submesh to retrieve
    //! @param [out] ppSubMesh Returns the Nth submesh
    //**********************************************************************
    void CMesh::GetSubMesh(uint32 index, ISubMesh **ppSubMesh)
    {
        *ppSubMesh = m_subMeshes[index].p;
        (*ppSubMesh)->AddRef();
    }

    //**********************************************************************
    //! \brief Adds a submesh to the mesh
    //! @param [in] pSubMesh Adds the specified submesh to our mesh
    //**********************************************************************
    void CMesh::AddSubMesh(ISubMesh *pSubMesh)
    {
        m_subMeshes.push_back(CRefObj<ISubMesh>(pSubMesh));
    }

    //**********************************************************************
    //! \brief Returns the bounding box for the mesh
    //! @param [in] pBBox Returns the bounding box of the mesh
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
    //! \brief Rescales a mesh so that it has unit size
    //**********************************************************************
    void CMesh::Normalize()
    {
        BBox3 bb;
        GetBBox(&bb);
        for (size_t i = 0; i < m_subMeshes.size(); i++)
            m_subMeshes[i]->Normalize(bb);
    }

    //**********************************************************************
    //! \brief Sets the mesh's material list
    //! @param [in] materials Assigns the mesh the specified list of materials
    //**********************************************************************
    void CMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials)
    {
        m_materials.clear();
        for (auto p : materials)
            m_materials.push_back(p);
    }

    //**********************************************************************
    //! \brief Returns the Nth material
    //! @param [in] materialID Index of material to retrieve
    //! @param [out] ppMaterial Returns the Nth material
    //**********************************************************************
    void CMesh::GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial)
    {
        if (materialID < m_materials.size())
        {
            (*ppMaterial) = m_materials[materialID].p;
            (*ppMaterial)->AddRef();
        }
        else
            *ppMaterial = nullptr;
    }

    //**********************************************************************
    //! \brief Loads a previously save Mesh
    //! @param [in] pStream Loads the material from the specified stream
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
            m_subMeshes.push_back(spSubMesh.p);
        }
    }

    //**********************************************************************
    //! \brief Stores a mesh to a stream
    //! @param [in] pStream Stores the material to the specified stream
    //**********************************************************************
    void CMesh::Store(IStream *pStream)
    {
        uint32 numSubMeshes = (uint32)NumberSubMeshes();
        DWORD bytesWritten;
        CT(pStream->Write(&numSubMeshes, sizeof(numSubMeshes), &bytesWritten));
        for (uint32 i = 0; i < numSubMeshes; i++)
        {
            CRefObj<ISubMesh> spSubMesh;
            GetSubMesh(i, &spSubMesh);
            spSubMesh->Store(pStream);
        }
    }

    //**********************************************************************
    //! \brief Computes normal vectors for each vertex on the mesh
    //**********************************************************************
    void CMesh::ComputeNormals()
    {
        for (auto pSubMesh : m_subMeshes)
            pSubMesh->ComputeNormals();
    }
    
    //**********************************************************************
    //! \brief Returns an empty mesh
    //! @param [out] ppMesh Returns the newly created mesh object
    //**********************************************************************
    CAUSTICAPI void CreateEmptyMesh(IMesh **ppMesh)
    {
        std::unique_ptr<CMesh> pMesh(new CMesh());
        *ppMesh = pMesh.release();
        (*ppMesh)->AddRef();
    }
}
