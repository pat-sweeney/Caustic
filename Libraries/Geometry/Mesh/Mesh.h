#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\GeomDS\kdtree.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\BlockAllocator.h"
#include "Rendering\Caustic\Caustic.h"
#include <vector>
#include <map>

namespace Caustic
{
    class CHalfEdge;

    //**********************************************************************
    // Class: CSubMesh
    // Defines a submesh. A submesh is a child of a mesh object
    // and contains the actual mesh data (collection of vertices and edges). 
    //**********************************************************************
    class CSubMesh : public ISubMesh, public CRefCount
    {
        std::vector<CGeomVertex*> m_vertices;
        std::vector<CFaceVertex*> m_faceVertices;
        std::vector<CFace*> m_faces;
        std::vector<CHalfEdge*> m_edges;
        std::map<uint64, CHalfEdge*> m_vertToEdge;
		CRefObj<IKDTree> m_spKDTree;
		Caustic::BBox3 m_bbox;
        uint32 m_materialID;
        EVertexFlags m_vertexFlags;
        EMeshFlags m_meshFlags;
        static bool s_allocatorInitialized;
        static CBlockAllocator<CGeomVertex> m_vertexAllocator;
        static CBlockAllocator<CFaceVertex> m_faceVertexAllocator;
        static CBlockAllocator<CHalfEdge> m_edgeAllocator;
        static CBlockAllocator<CFace> m_faceAllocator;

        CHalfEdge *FindEdge(CGeomVertex *pHead, CGeomVertex *pTail);
        CHalfEdge *FindNextEdgeAroundVertex(CGeomVertex *pVert, CHalfEdge *pEdge, bool entering, bool clockwise);
        CGeomVertex *FindVertex(Caustic::Vector3 &pos, Caustic::Vector3 *pNorm, Caustic::Vector2 *pUV);
        CFace *AllocateFace();
        CHalfEdge *AllocateEdge(CGeomVertex *pHead, CGeomVertex *pTail);
        CGeomVertex *AllocateGeomVertex(Caustic::Vector3 &pos, Vector3 &normal, Vector2 &uv);
        CFaceVertex *AllocateFaceVertex(CFace *face, Caustic::Vector3 &normal, Caustic::Vector2 &uv);
        void LinkEdges(CHalfEdge *pPrev, CHalfEdge *pCur);
        void TriangulateViaEarClipping();
        void TriangulateViaPointInsertion();
        void BuildVertexBuffer(IGraphics *pGraphics, IShaderInfo *pShaderInfo, std::vector<int> &vertexReferenced, MeshData *pMeshData);
        void BuildIndexBuffer(IGraphics *pGraphics, std::vector<int> &vertexReferenced, MeshData *pMeshData);
        void BuildReferencedVertexList(std::vector<int> &vertexReferenced);
    public:
        friend class CMeshConstructor;

        CSubMesh() :
            m_materialID(0),
            m_vertexFlags(EVertexFlags(HasPosition | HasNormal | HasUV0)),
            m_meshFlags(EMeshFlags(0))
        {
            CreateKDTree(&m_spKDTree);
        }

        CSubMesh(int approxNumVertices, int approxNumEdges, int approxNumFaces)
        {
            if (!s_allocatorInitialized)
            {
                m_vertexAllocator.CBlockAllocator<CGeomVertex>::CBlockAllocator(approxNumVertices);
                m_edgeAllocator.CBlockAllocator<CHalfEdge>::CBlockAllocator(approxNumEdges);
                m_faceAllocator.CBlockAllocator<CFace>::CBlockAllocator(approxNumFaces);
                m_faceVertexAllocator.CBlockAllocator<CFaceVertex>::CBlockAllocator(approxNumVertices);
                s_allocatorInitialized = true;
            }
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
        CFace *GetFace(uint32 index) override { return m_faces[index]; }
        uint32 GetNumberVertices() override { return (uint32)m_vertices.size(); }
        CGeomVertex *GetVertex(uint32 index) override { return m_vertices[index]; }
        uint32 GetNumberEdges() override { return (uint32)m_edges.size(); }
        CHalfEdge *GetEdge(uint32 index) override { return m_edges[index]; }
        virtual EVertexFlags GetVertexFlags() override { return m_vertexFlags; }
        virtual void SetVertexFlags(EVertexFlags flags) override { m_vertexFlags = flags; }
        virtual EMeshFlags GetMeshFlags() override { return m_meshFlags; }
        virtual void SetMeshFlags(EMeshFlags  flags) override { m_meshFlags = flags; }
        virtual uint32 GetMaterialID() override { return m_materialID; }
        virtual void SetMaterialID(uint32 materialID) override { m_materialID = materialID; }
        virtual const BBox3 &GetBBox() override { return m_bbox; }

        //**********************************************************************
        // Method: Normalize
        // See <ISubMesh::Normalize>
        //**********************************************************************
        virtual void Normalize(const BBox3 &bbox) override;

        virtual void ComputeNormals() override;
        virtual uint32 VertexToIndex(CGeomVertex *pVertex) override;
        virtual uint32 FaceToIndex(CFace *pFace) override;
        virtual uint32 EdgeToIndex(CHalfEdge *pEdge) override;

        //**********************************************************************
        // Method: Triangulate
        // Converts a mesh from arbitrary polygons into
        // a triangulated mesh.
        //
        // Parameters:
        // method - method to use for triangulation
        //**********************************************************************
        virtual void Triangulate(ETriangulateMethod method) override;
        virtual void ToRenderSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderSubMesh **ppRenderSubMesh) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };

    //**********************************************************************
    // Class: CMesh
    // Defines a mesh. A mesh object is simply a collection of submesh
    // objects (ISubMesh). To convert this mesh into a renderable form the client
    // should call <MeshToD3D>.
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
        virtual void ToRenderMesh(IRenderer* pRenderer, IShader* pShader, IRenderMesh** ppRenderMesh) override;
        virtual void ToRenderMaterials(IRenderer* pRenderer, IShader* pShader, IRenderMesh* pRenderMesh) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };
};
