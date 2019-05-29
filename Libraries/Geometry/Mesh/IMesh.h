//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\ISerialize.h"
#include "Base\Math\Vector.h"
#include "Base\Math\BBox.h"
#include "Rendering/Caustic/IMaterialAttrib.h"
#include <memory>
#include <atlbase.h>
#include <vector>
#include <d3d12.h>

namespace Caustic
{
	struct IRenderMesh;
	struct IRenderSubMesh;
	struct IRenderer;
	struct IShader;

    //**********************************************************************
    // CGeomVertex defines a vertex in our mesh. 
    //
    // Defines a vertex on our mesh. We have two types of vertex data:
    // 1) data that is specific to a vertex relative to a given face
    // 2) data that is relative to a vertex regardless of face
    // The reason for this split is so that positional information
    // can be shared across the mesh (useful for performing operations
    // on the geometry) while data that can change from face to face
    // is stored separately (such as normal information).
    // Typically most mesh data is stored in CGeomVertex. Only faces
    // at material boundaries or smoothing groups contain CFaceVertex data.
    //**********************************************************************
    struct CGeomVertex
    {
        Vector3 pos;        // Defines the position
        Vector3 norm;       // Defines the normal
        Vector2 uvs[4];     // Defines UV coordinates
        int index;          // Index (used for serialization)
    };

    //**********************************************************************
    // CFaceVertex defines a vertex data relative to a given face.
    // See CGeomVertex for further information.
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
        // Returns the next edge in our edge loop
        CHalfEdge *GetNextEdge() { return m_pNext; }
        // Returns the previous edge in our edge loop
        CHalfEdge *GetPrevEdge() { return m_pPrev; }
        // Returns the half edge opposite to the current edge
        CHalfEdge *GetOppositeEdge() { return m_pOpposite; }
        // Returns the vertex at the head of this edge
        CGeomVertex *GetHeadVertex() { return m_pVertex; }
        // Returns the vertex at the tail of this edge
        CGeomVertex *GetTailVertex() { return m_pPrev->m_pVertex; }
        // Returns the face this edge is a boundary of
        CFace *GetFace() { return m_pFace; }
        // Returns the smoothing group this edge belongs to
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
    // Enum: EMeshFlags
	// Defines various flags associated with an ISubMesh
    //**********************************************************************
    enum EMeshFlags
    {
        TwoSided = 0x1, // Mesh is considered to be two sided
    };

    //**********************************************************************
    // Interface: ISubMesh
    // ISubMesh is used by clients for manipulating a submesh. A submesh contains
    // actual mesh data. This is different from a mesh object which is just
    // a collection of submesh objects.
    //**********************************************************************
    struct ISubMesh : public ISerialize
    {
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
        virtual void Normalize(const BBox3 &bbox) = 0;
        virtual void ComputeNormals() = 0;
        virtual uint32 VertexToIndex(CGeomVertex *pVertex) = 0;
        virtual uint32 FaceToIndex(CFace *pFace) = 0;
        virtual uint32 EdgeToIndex(CHalfEdge *pEdge) = 0;
        virtual void Triangulate(ETriangulateMethod method) = 0;
		virtual void ToRenderSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderSubMesh **ppRenderMesh) = 0;
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
        virtual void GetSubMesh(uint32 index, ISubMesh **ppSubMesh) = 0;
        virtual void AddSubMesh(ISubMesh *pSubMesh) = 0;
        virtual void GetBBox(BBox3 *pBBox) = 0;
        virtual void Normalize() = 0;
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) = 0;
        virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) = 0;
        virtual void ComputeNormals() = 0;
		virtual void ToRenderMesh(IRenderer *pRenderer, IShader *pShader, IRenderMesh **ppRenderMesh) = 0;
	};
    

    CAUSTICAPI void CreateSurfaceRevolution(std::vector<Vector3> &pts, uint32 npts, uint32 subdivisions, float maxAngle, IMesh **ppMesh);
    CAUSTICAPI void CreateEmptyMesh(IMesh **ppMesh);
    CAUSTICAPI void CreateSubMesh(std::vector<CGeomVertex> &verts,
        std::vector<int> &faces,
        uint32 materialID,
        ISubMesh **ppSubMesh);
    CAUSTICAPI void CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<Vector3> &vertNorms,
        std::vector<Vector2> &vertUVs,
        std::vector<int> &faces,
        EVertexFlags flags,
        uint32 materialID,
        ISubMesh **ppSubMesh);
    CAUSTICAPI void CreateSubMesh(std::vector<Vector3> &vertPos,
        std::vector<int> &faces,
        uint32 materialID,
        ISubMesh **ppSubMesh);
    CAUSTICAPI void CreateEmptySubMesh(ISubMesh **ppSubMesh);
    CAUSTICAPI void CreateCube(IMesh **ppMesh);
    CAUSTICAPI void CreateSphere(uint32 subdivisions, IMesh **ppMesh);
    CAUSTICAPI void CreateTetrahedron(IMesh **ppMesh);
    CAUSTICAPI void CreateGrid(uint32 subdivisions, IMesh **ppMesh);
}
