#pragma once
//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\GeomDS\kdtree.h"
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.BlockAllocator;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\PathTrace.h"
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
        std::string m_name;
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

        //**********************************************************************
        // Method: FindEdge
        // Returns the edge from pTail->pHead. If the edge does
        // not exist then nullptr is returned.
        //
        // Parameters:
        // pHead - starting vertex for edge
        // pTail -ending vertex for edge
        //
        // Returns:
        // Found edge
        //**********************************************************************
        CHalfEdge *FindEdge(CGeomVertex *pHead, CGeomVertex *pTail);

        //**********************************************************************
        // Method: FindNextEdgeAroundVertex
        // Returns the next edge around the vertex
        // in the given direction (clockwise or counter-clockwise) that is either
        // entering or leaving the vertex.
        //
        // Parameters:
        // pVertex - Vertex around which to search
        // pEdge - starting edge for search
        // entering - true if edge is pointing towards 'pVert'. false otherwise
        // clockwise - indicates whether to search clockwise around vertex. counter-clockwise otherwise.
        //
        // Returns:
        // Next edge around vertex.
        //**********************************************************************
        CHalfEdge *FindNextEdgeAroundVertex(CGeomVertex *pVert, CHalfEdge *pEdge, bool entering, bool clockwise);

        //**********************************************************************
        // Method: FindVertex
        // Returns a pointer to a vertex in our mesh
        // that matches the specified values
        //
        // Parameters:
        // pos - position of vertex
        // pNorm - normal at vertex. maybe nullptr
        // pUV - texture coordinates at vertex. maybe nullptr
        //
        // Returns:
        // Found vertex. nullptr if there is no matching vertex
        //**********************************************************************
        CGeomVertex *FindVertex(Caustic::Vector3 &pos, Caustic::Vector3 *pNorm, Caustic::Vector2 *pUV);

        //**********************************************************************
        // Method: AllocateFace
        // Allocates a new face and places in our face list.
        // 
        // Returns:
        // Returns the allocate face object
        //**********************************************************************
        CFace *AllocateFace();

        //**********************************************************************
        // Method: AllocateEdge
        // Allocates a new edge going from
        // pTail->pHead. NOTE: This function does not insert
        // the edge into our edge list. That occurs during the call
        // to LinkEdges().
        //
        // Parameters:
        // pHead - starting vertex for the edge
        // pTail - ending vertex for the edge
        //
        // Returns:
        // Returns new edge
        //**********************************************************************
        CHalfEdge *AllocateEdge(CGeomVertex *pHead, CGeomVertex *pTail);

        //**********************************************************************
        // Method: AllocateGeomVertex
        // Allocates a new vertex given a position,
        // normal, and UVs and places it in our vertex list.
        //
        // Parameters:
        // pos - position of vertex
        // normal - normal vector at the vertex
        // uv - texture coordinates at the vertex
        //
        // Returns:
        // Newly created vertex object
        //**********************************************************************
        CGeomVertex *AllocateGeomVertex(Caustic::Vector3 &pos, Vector3 &normal, Vector2 &uv);
        CFaceVertex *AllocateFaceVertex(CFace *face, Caustic::Vector3 &normal, Caustic::Vector2 &uv);

        //**********************************************************************
        // Method: LinkEdges
        // Links two edges (full edges) together
        // We start with the picture at the top. The solid lines represent
        // HalfEdge's, while the dotted lines represent their m_pPrev/m_pNext
        // links. We are trying to add newEdge and link it to oldEdge (an existing
        // edge)
        // === Text
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
        // ===
        //
        // Parameters:
        // pPrev - previous edge
        // pCur - current edge
        //**********************************************************************
        void LinkEdges(CHalfEdge *pPrev, CHalfEdge *pCur);

        void TriangulateViaEarClipping();
        void TriangulateViaPointInsertion();

        //**********************************************************************
        // Method: BuildVertexBuffer
        // Creates a vertex buffer using the specified submesh.
        //
        // Parameters:
        // pRenderer - Graphics device
        // pShaderInfo - Information about the shader
        // vertexReferenced - List of vertices that are referenced
        // MeshData - place to store created vertex buffer
        //**********************************************************************
        void BuildVertexBuffer(IRenderer *pRenderer, IShaderInfo *pShaderInfo, std::vector<int> &vertexReferenced, MeshData *pMeshData);

        //**********************************************************************
        // Method: BuildIndexBuffer
        // Creates a index buffer using the specified submesh.
        //
        // Parameters:
        // pRenderer - Graphics device
        // vertexReferenced - List of vertices that are referenced
        // MeshData - place to store created index buffer
        //**********************************************************************
        void BuildIndexBuffer(IRenderer *pRenderer, std::vector<int> &vertexReferenced, MeshData *pMeshData);

        void BuildReferencedVertexList(std::vector<int> &vertexReferenced);

        //**********************************************************************
        // Method: CheckConsistency
        // Checks whether the links between edges in the mesh are consistent
        //**********************************************************************
        void CheckConsistency();
    public:
        friend class CMeshConstructor;

        CSubMesh() :
            m_materialID(0),
            m_vertexFlags(EVertexFlags(HasPosition | HasNormal | HasUV0)),
            m_meshFlags(EMeshFlags(0))
        {
            m_spKDTree = CreateKDTree();
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

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISubMesh
        //**********************************************************************
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial, std::vector<CRefObj<IMaterialAttrib>> materials);
        virtual void SetName(const char* pName) override { m_name = std::string(pName); }
        virtual std::string GetName() override { return m_name; }
        virtual uint32 GetNumberFaces() override { return (uint32)m_faces.size(); }
        virtual CFace *GetFace(uint32 index) override { return m_faces[index]; }
        virtual uint32 GetNumberVertices() override { return (uint32)m_vertices.size(); }
        virtual CGeomVertex *GetVertex(uint32 index) override { return m_vertices[index]; }
        virtual uint32 GetNumberEdges() override { return (uint32)m_edges.size(); }
        virtual CHalfEdge *GetEdge(uint32 index) override { return m_edges[index]; }
        virtual EVertexFlags GetVertexFlags() override { return m_vertexFlags; }
        virtual void SetVertexFlags(EVertexFlags flags) override { m_vertexFlags = flags; }
        virtual EMeshFlags GetMeshFlags() override { return m_meshFlags; }
        virtual void SetMeshFlags(EMeshFlags  flags) override { m_meshFlags = flags; }
        virtual uint32 GetMaterialID() override { return m_materialID; }
        virtual void SetMaterialID(uint32 materialID) override { m_materialID = materialID; }
        virtual const BBox3 &GetBBox() override { return m_bbox; }
        virtual void Normalize(const BBox3 &bbox) override; // See <ISubMesh::Normalize>
        virtual void ComputeNormals() override;
        virtual uint32 VertexToIndex(CGeomVertex *pVertex) override;
        virtual uint32 FaceToIndex(CFace *pFace) override;
        virtual uint32 EdgeToIndex(CHalfEdge *pEdge) override;
        virtual void Triangulate(ETriangulateMethod method) override;
        virtual CRefObj<IRenderSubMesh> ToRenderSubMesh(IRenderer *pRenderer, IShader *pShader) override;

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
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial);
        virtual uint32 NumberSubMeshes() override { return (uint32)m_subMeshes.size(); };
        virtual CRefObj<ISubMesh> GetSubMesh(uint32 index) override;
        virtual void AddSubMesh(ISubMesh *pSubMesh) override;
        virtual void GetBBox(Caustic::BBox3 *pBBox) override;
        virtual void Normalize() override;
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) override;
        virtual uint32 GetNumberMaterials() override;
        virtual CRefObj<IMaterialAttrib> GetMaterial(uint32 materialID) override;
        virtual void ComputeNormals() override;
        virtual CRefObj<IRenderMesh> ToRenderMesh(IRenderer* pRenderer, IShader* pShader) override;
        virtual void ToRenderMaterials(IRenderer* pRenderer, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial) override;

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;
    };
};
