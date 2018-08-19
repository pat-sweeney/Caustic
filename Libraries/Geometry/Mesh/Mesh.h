#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\GeomDS\kdtree.h"
#include "Base\Core\RefCount.h"
#include <vector>
#include <map>

namespace Caustic
{
    class CHalfEdge;

    //**********************************************************************
    //! \brief CSubMesh defines a submesh object
    //! 
    //! CSubMesh defines a submesh. A submesh is a child of a mesh object
    //! and contains the actual mesh data (collection of vertices and edges). 
    //**********************************************************************
    class CSubMesh : public ISubMesh, public CRefCount
    {
        std::vector<std::unique_ptr<CGeomVertex>> m_vertices;
        std::vector<std::unique_ptr<CFaceVertex>> m_faceVertices;
        std::vector<std::unique_ptr<CFace>> m_faces;
        std::vector<std::unique_ptr<CHalfEdge>> m_edges;
        std::map<std::tuple<CGeomVertex*, CGeomVertex*>, CHalfEdge*> m_vertToEdge;
		CRefObj<IKDTree> m_spKDTree;
		Caustic::BBox3 m_bbox;
        uint32 m_materialID;
        EVertexFlags m_vertexFlags;
        EMeshFlags m_meshFlags;

        CHalfEdge *FindEdge(CGeomVertex *pHead, CGeomVertex *pTail);
        CHalfEdge *FindNextEdgeAroundVertex(CGeomVertex *pVert, CHalfEdge *pEdge, bool entering, bool clockwise);
        CGeomVertex *FindVertex(Caustic::Vector3 *pPos, Caustic::Vector3 *pNorm, Caustic::Vector2 *pUV);
        CFace *AllocateFace();
        CHalfEdge *AllocateEdge(CGeomVertex *pHead, CGeomVertex *pTail);
        CGeomVertex *AllocateGeomVertex(Caustic::Vector3 &pos, Vector3 &normal, Vector2 &uv);
        CFaceVertex *AllocateFaceVertex(CFace *face, Caustic::Vector3 &normal, Caustic::Vector2 &uv);
        void LinkEdges(CHalfEdge *pPrev, CHalfEdge *pCur);
        void TriangulateViaEarClipping();
        void TriangulateViaPointInsertion();
    public:
        friend class CMeshConstructor;

        CSubMesh() :
            m_materialID(0),
            m_vertexFlags(EVertexFlags(HasPosition | HasNormal | HasUV0)),
            m_meshFlags(EMeshFlags(0))
        {
			CreateKDTree(&m_spKDTree);
        }

        void CheckConsistency();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISubMesh
        //**********************************************************************
        uint32 GetNumberFaces() override { return (uint32)m_faces.size(); }
        CFace *GetFace(uint32 index) override { return m_faces[index].get(); }
        uint32 GetNumberVertices() override { return (uint32)m_vertices.size(); }
        CGeomVertex *GetVertex(uint32 index) override { return m_vertices[index].get(); }
        uint32 GetNumberEdges() override { return (uint32)m_edges.size(); }
        CHalfEdge *GetEdge(uint32 index) override { return m_edges[index].get(); }
        virtual EVertexFlags GetVertexFlags() override { return m_vertexFlags; }
        virtual void SetVertexFlags(EVertexFlags flags) override { m_vertexFlags = flags; }
        virtual EMeshFlags GetMeshFlags() override { return m_meshFlags; }
        virtual void SetMeshFlags(EMeshFlags  flags) override { m_meshFlags = flags; }
        virtual uint32 GetMaterialID() override { return m_materialID; }
        virtual void SetMaterialID(uint32 materialID) override { m_materialID = materialID; }
        virtual const BBox3 &GetBBox() override { return m_bbox; }
        virtual void Normalize(const BBox3 &bbox) override;
        virtual void ComputeNormals() override;
        virtual uint32 VertexToIndex(CGeomVertex *pVertex) override;
        virtual uint32 FaceToIndex(CFace *pFace) override;
        virtual uint32 EdgeToIndex(CHalfEdge *pEdge) override;
        virtual void Triangulate(ETriangulateMethod method) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };

    //**********************************************************************
    //! \brief CMesh defines a mesh object
    //! 
    //! CMesh defines a mesh. A mesh object is simply a collection of submesh
    //! objects (ISubMesh). To convert this mesh into a renderable form the client
    //! should call \ref MeshToD3D .
    //**********************************************************************
    class CMesh : public IMesh, public CRefCount
    {
        std::vector<CRefObj<ISubMesh>> m_subMeshes;
        std::vector<CRefObj<IMaterialAttrib>> m_materials;
    public:
        CMesh();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }
        
        //**********************************************************************
        // IMesh
        //**********************************************************************
        virtual uint32 NumberSubMeshes() override { return (uint32)m_subMeshes.size(); };
        virtual void GetSubMesh(uint32 index, ISubMesh **ppSubMesh) override;
        virtual void AddSubMesh(ISubMesh *pSubMesh) override;
        virtual void GetBBox(Caustic::BBox3 *pBBox) override;
        virtual void Normalize() override;
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) override;
        virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) override;
        virtual void ComputeNormals() override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };
};
