//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\ISerialize.h"
#include "Base\Math\Vector.h"
#include "Base\Math\BBox.h"
#include "Rendering\Caustic\RenderTypes.h"
#include "Rendering\Caustic\IMaterialAttrib.h"
#include <memory>
#include <atlbase.h>
#include <vector>
#include <d3d11.h>
#include <string>

//**********************************************************************
// File: IMesh.h
// Defines interface for mesh.
//**********************************************************************

namespace Caustic
{
    struct IRenderer;
    struct IShader;
    struct IRenderSubMesh;
    struct IRenderMesh;

    //**********************************************************************
    // Struct: CFaceVertex
    // Defines a vertex data relative to a given face.
    // See <CGeomVertex> for further information.
    //**********************************************************************
    struct CFaceVertex
    {
        Vector3 norm;       // Defines the normal
        Vector2 uvs[4];     // Defines UV coordinates
        int index;          // Index (used for serialization)
    };

    class CFace;
    
    //**********************************************************************
    // Class: CHalfEdge
	// Defines a single half edge on our graph
    //
    // Members:
    // m_pNext - Next half edge in edge loop
    // m_pPrev - Previous half edge in edge loop
    // m_pOpposite - Opposite half edge
    // m_pVertex - Vertex at head of edge
    // m_pFace - Face to the left of edge
    // m_smoothingGroup - Smoothing group this edge belongs to
    // index - 
    //**********************************************************************
    class CHalfEdge
    {
        CHalfEdge *m_pNext;            // Next half edge in edge loop
        CHalfEdge *m_pPrev;            // Previous half edge in edge loop
        CHalfEdge *m_pOpposite;        // Opposite half edge
        CGeomVertex *m_pVertex;        // Vertex at head of edge
        CFace *m_pFace;                // Face to the left of edge
        uint32 m_smoothingGroup;       // Smoothing group this edge belongs to
        int index;

        friend class CFace;
        friend class CSubMesh;
        friend class CMeshConstructor;
    public:
        //**********************************************************************
        // Method: GetNextEdge
        // Returns the next edge in our edge loop
        //**********************************************************************
        CHalfEdge *GetNextEdge() { return m_pNext; }

        //**********************************************************************
        // Method: GetPrevEdge
        // Returns the previous edge in our edge loop
        //**********************************************************************
        CHalfEdge *GetPrevEdge() { return m_pPrev; }

        //**********************************************************************
        // Method: GetOppositeEdge
        // Returns the half edge opposite to the current edge
        //**********************************************************************
        CHalfEdge *GetOppositeEdge() { return m_pOpposite; }

        //**********************************************************************
        // Method: GetHeadVertex
        // Returns the vertex at the head of this edge
        //**********************************************************************
        CGeomVertex *GetHeadVertex() { return m_pVertex; }

        //**********************************************************************
        // Method: GetTailVertex
        // Returns the vertex at the tail of this edge
        //**********************************************************************
        CGeomVertex *GetTailVertex() { return m_pPrev->m_pVertex; }

        //**********************************************************************
        // Method: GetFace
        // Returns the face this edge is a boundary of
        //
        // Returns:
        // Pointer to the current face
        //**********************************************************************
        CFace *GetFace() { return m_pFace; }

        //**********************************************************************
        // Method: GetSmoothingGroup
        // Returns the smoothing group this edge belongs to
        //
        // Returns:
        // ID of the smoothing group this edge belongs to
        //**********************************************************************
        uint32 GetSmoothingGroup() { return m_smoothingGroup; }
    };

    //**********************************************************************
    // Class: CFace
	// Defines a face in our CMesh
    //**********************************************************************
    class CFace
    {
        Vector3 m_normal;                      // Face's normal vector
        Vector3 m_center;                      // Center position of face
        CHalfEdge *m_pEdge;                    // First edge in this face
        std::vector<CGeomVertex*> m_vertices;
        std::vector<CFaceVertex> m_vertexData; // Data at each vertex specific to this face
        int index;
        
        friend class CHalfEdge;
        friend class CSubMesh;
        friend class CMeshConstructor;
    public:
        Vector3 GetNormal() { return m_normal; }
        Vector3 GetCenter() { return m_center; }
        CHalfEdge *GetEdge() { return m_pEdge; }
        uint32 GetNumberVertices() { return (uint32)m_vertices.size(); }
        CGeomVertex *GetVertex(uint32 idx) { return m_vertices[idx]; }

        void ComputeFaceNormal();
        bool IsDegenerate();
    };
    
    //**********************************************************************
    // Enum: EVertexFlags
	// Define flags describing what fields are available in a SVertex
    //**********************************************************************
    enum EVertexFlags
    {
        HasPosition = 0x01,    // Vertex's 'pos' field is valid
        HasNormal = 0x02,    // Vertex's 'norm' field is valid
        HasUV0 = 0x04,        // Vertex's 'uv[0]' field is valid
        HasUV1 = 0x08,        // Vertex's 'uv[1]' field is valid
        HasUV2 = 0x10,        // Vertex's 'uv[2]' field is valid
        HasUV3 = 0x20,        // Vertex's 'uv[3]' field is valid
    };

    //**********************************************************************
    // Enum: ETriangulateMethod
	// Defines the method used for converting a polygonal mesh into a trianglar mesh.
    //**********************************************************************
    enum ETriangulateMethod
    {
        EarClipping,   // Use "Ear-clipping" method (clip triangle off polygon, repeat)
        PointInsertion // Use "Point-Insertion" method (insert new vertices to form triangles)
    };

    const uint32 c_InvalidIndex = 0xffffffff;

    //**********************************************************************
    // Class: EMeshFlags
	// Defines various flags associated with an ISubMesh
    //
    // TwoSided - Mesh is considered to be two sided
    //**********************************************************************
    enum EMeshFlags
    {
        TwoSided = 0x1,
    };

    //**********************************************************************
    // Interface: ISubMesh
    // ISubMesh is used by clients for manipulating a submesh. A submesh contains
    // actual mesh data. This is different from a mesh object which is just
    // a collection of submesh objects.
    //**********************************************************************
    struct ISubMesh : public ISerialize
    {
        virtual void SetName(const char* pName) = 0;
        virtual std::string GetName() = 0;
        virtual uint32 GetNumberFaces() = 0;
        virtual CFace *GetFace(uint32 index) = 0;
        virtual uint32 GetNumberVertices() = 0;
        virtual CGeomVertex *GetVertex(uint32 index) = 0;
        virtual uint32 GetNumberEdges() = 0;
        virtual CHalfEdge *GetEdge(uint32 index) = 0;
        virtual EVertexFlags GetVertexFlags() = 0;
        virtual void SetVertexFlags(EVertexFlags flags) = 0;
        virtual EMeshFlags GetMeshFlags() = 0;
        virtual void SetMeshFlags(EMeshFlags flags) = 0;
        virtual uint32 GetMaterialID() = 0;
        virtual void SetMaterialID(uint32 materialID) = 0;
        virtual const BBox3 &GetBBox() = 0;

        //**********************************************************************
        // Method: Normalize
        // Rescales a mesh so fits inside a -0.5..+0.5 bounding box
        //
        // Parameters:
        // bbox - Bounding box of the mesh
        //**********************************************************************
        virtual void Normalize(const BBox3 &bbox) = 0;
        virtual void ComputeNormals() = 0;
        virtual uint32 VertexToIndex(CGeomVertex *pVertex) = 0;
        virtual uint32 FaceToIndex(CFace *pFace) = 0;
        virtual uint32 EdgeToIndex(CHalfEdge *pEdge) = 0;
        virtual void Triangulate(ETriangulateMethod method) = 0;
        virtual CRefObj<IRenderSubMesh> ToRenderSubMesh(IRenderer *pRenderer, IShader *pShader) = 0;
    };

    //**********************************************************************
    // Interface: IMesh
    // IMesh is simply a collection of submesh objects (ISubMesh).
	//
    // NOTE: This mesh is different from a renderable mesh. There is a function
    // MeshToD3D which will convert the mesh into a renderable mesh.
    //**********************************************************************
    struct IMesh : public ISerialize
    {
        virtual uint32 NumberSubMeshes() = 0;
        virtual CRefObj<ISubMesh> GetSubMesh(uint32 index) = 0;
        virtual void AddSubMesh(ISubMesh *pSubMesh) = 0;
        virtual void GetBBox(BBox3 *pBBox) = 0;
        virtual void Normalize() = 0;
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) = 0;
        virtual uint32 GetNumberMaterials() = 0;
        virtual CRefObj<IMaterialAttrib> GetMaterial(uint32 materialID) = 0;
        virtual void ComputeNormals() = 0;
		virtual CRefObj<IRenderMesh> ToRenderMesh(IRenderer *pRenderer, IShader *pShader) = 0;
        virtual void ToRenderMaterials(IRenderer* pRenderer, IShader* pShader, IRenderMesh* pRenderMesh) = 0;
    };
    

    CAUSTICAPI CRefObj<IMesh> CreateSurfaceRevolution(std::vector<Vector3> &pts, uint32 npts, uint32 subdivisions, float maxAngle);
    CAUSTICAPI CRefObj<IMesh> CreateEmptyMesh();
    CAUSTICAPI CRefObj<ISubMesh> CreateSubMesh(std::vector<CGeomVertex> &verts,
        std::vector<int> &faces,
        uint32 materialID);
    CAUSTICAPI CRefObj<ISubMesh> CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<Vector3> &vertNorms,
        std::vector<Vector2> &vertUVs,
        std::vector<int> &faces,
        EVertexFlags flags,
        uint32 materialID);
    CAUSTICAPI CRefObj<ISubMesh> CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<int> &faces,
        uint32 materialID);
    CAUSTICAPI CRefObj<ISubMesh> CreateEmptySubMesh();
    CAUSTICAPI CRefObj<IMesh> CreateCube();
    CAUSTICAPI CRefObj<IMesh> CreateSphere(uint32 subdivisions);
    CAUSTICAPI CRefObj<IMesh> CreateTetrahedron();
    CAUSTICAPI CRefObj<IMesh> CreateGrid(uint32 subdivisions);
    CAUSTICAPI CRefObj<IMesh> CreateGrid(uint32 width, uint32 height);
}
