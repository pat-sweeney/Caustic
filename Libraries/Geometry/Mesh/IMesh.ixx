//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <atlbase.h>
#include <vector>
#include <d3d11.h>
#include <string>

export module Geometry.Mesh.IMesh;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Base.Math.Ray;
import Geometry.Mesh.RenderTypes;
import Geometry.Mesh.MaterialAttrib;
import Geometry.Mesh.IMaterialAttrib;

//**********************************************************************
// File: IMesh.h
// Defines interface for mesh.
//**********************************************************************

export namespace Caustic
{
    struct IRenderer;
    struct IShader;

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
    public: // Temporary work around due to module issue where friending doesn't appear to work properly
        CHalfEdge *m_pNext;            // Next half edge in edge loop
        CHalfEdge *m_pPrev;            // Previous half edge in edge loop
        CHalfEdge *m_pOpposite;        // Opposite half edge
        CGeomVertex *m_pVertex;        // Vertex at head of edge
        CFace *m_pFace;                // Face to the left of edge
        uint32_t m_smoothingGroup;       // Smoothing group this edge belongs to
        int index;

        //friend class CFace;
        //friend class CSubMesh;
        //friend class CMeshConstructor;
    public:
        //**********************************************************************
        // Method: GetNextEdge
        // Returns the next edge in our edge loop
        //**********************************************************************
        CHalfEdge* GetNextEdge() { return m_pNext; }

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
        CGeomVertex* GetHeadVertex() { return m_pVertex; }

        //**********************************************************************
        // Method: GetTailVertex
        // Returns the vertex at the tail of this edge
        //**********************************************************************
        CGeomVertex* GetTailVertex() { return m_pPrev->m_pVertex; }

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
        uint32_t GetSmoothingGroup() { return m_smoothingGroup; }
    };

    //**********************************************************************
    // Class: CFace
    // Defines a face in our CMesh
    //**********************************************************************
    class CFace
    {
    public: // Temporary work around due to module issue where friending doesn't appear to work properly
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
        //**********************************************************************
        // Method: GetNormal
        // Returns the normal vector for a face
        //**********************************************************************
        Vector3 GetNormal() { return m_normal; }

        //**********************************************************************
        // Method: GetCenter
        // Returns the center point for a face
        //**********************************************************************
        Vector3 GetCenter() { return m_center; }

        //**********************************************************************
        // Method: GetEdge
        // Returns a pointer to once of the edges in the edge loop that makes
        // up the face.
        //**********************************************************************
        CHalfEdge *GetEdge() { return m_pEdge; }

        //**********************************************************************
        // Method: GetNumberVertices
        // Returns the number of vertices in the face.
        //**********************************************************************
        uint32_t GetNumberVertices() { return (uint32_t)m_vertices.size(); }

        //**********************************************************************
        // Method: GetVertex
        // Returns the Nth vertex in a face
        // 
        // Parameters:
        // idx - index of vertex
        //**********************************************************************
        CGeomVertex *GetVertex(uint32_t idx) { return m_vertices[idx]; }


        //**********************************************************************
        // Method: ComputeFaceNormal
        // Computes the normal for the face using Newell's method
        //**********************************************************************
        void ComputeFaceNormal();

        //**********************************************************************
        // Method: IsDegenerate
        // Returns true if the face is degenerate. Otherwise, returns false.
        //**********************************************************************
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

    const uint32_t c_InvalidIndex = 0xffffffff;

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
        //**********************************************************************
        // Method: RayIntersect
        // Computes the intersection between a ray and a mesh.
        //
        // Parameters:
        // ray - Ray to intersect against
        // pIntersection - Returns the point of intersection if ray intersects mesh
        // pMaterial - Material of face that ray intersects
        // materials - list of possible materials
        // 
        // Returns:
        // Returns true if ray intersects mesh. False otherwise.
        //**********************************************************************
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, uint32_t* pMaterialID) = 0;

        //**********************************************************************
        // Method: SetName
        // Sets the name of the submesh
        //
        // Parameters:
        // pName - name to assign
        //**********************************************************************
        virtual void SetName(const char* pName) = 0;

        //**********************************************************************
        // Method: GetName
        // Returns the name of the submesh
        //**********************************************************************
        virtual std::string GetName() = 0;

        //**********************************************************************
        // Method: GetNumberFaces
        // Returns the number of faces in the submesh
        //**********************************************************************
        virtual uint32_t GetNumberFaces() = 0;

        //**********************************************************************
        // Method: GetFace
        // Returns a pointer to the requested face
        // 
        // Parameters:
        // index - index of face
        //**********************************************************************
        virtual CFace *GetFace(uint32_t index) = 0;

        //**********************************************************************
        // Method: GetNumberVertices
        // Returns number of vertices in submesh
        //**********************************************************************
        virtual uint32_t GetNumberVertices() = 0;

        //**********************************************************************
        // Method: GetVertex
        // Returns a requested vertex
        // 
        // Parameters:
        // index - index of requested vertex
        //**********************************************************************
        virtual CGeomVertex *GetVertex(uint32_t index) = 0;

        //**********************************************************************
        // Method: GetNumberEdges
        // Returns number of edges in submesh
        //**********************************************************************
        virtual uint32_t GetNumberEdges() = 0;

        //**********************************************************************
        // Method: GetEdge
        // Returns the requested edge
        // 
        // Parameters:
        // index - index of requested edge
        //**********************************************************************
        virtual CHalfEdge *GetEdge(uint32_t index) = 0;

        //**********************************************************************
        // Method: GetVertexFlags
        // Returns vertex flags (which describe what fields a vertex contains)
        //**********************************************************************
        virtual EVertexFlags GetVertexFlags() = 0;

        //**********************************************************************
        // Method: SetVertexFlags
        // Sets which fields a vertex contains
        // 
        // Parameters:
        // flags - flags indicating which fields a vertex contains
        //**********************************************************************
        virtual void SetVertexFlags(EVertexFlags flags) = 0;

        //**********************************************************************
        // Method: GetMeshFlags
        // Returns mesh flags (set of flags describing attributes of the submesh)
        //**********************************************************************
        virtual EMeshFlags GetMeshFlags() = 0;

        //**********************************************************************
        // Method: SetMeshFlags
        // Sets the mesh flags on the submesh
        // 
        // Parameters:
        // flags - list of flags describing attributes of the submesh
        //**********************************************************************
        virtual void SetMeshFlags(EMeshFlags flags) = 0;

        //**********************************************************************
        // Method: GetMaterialID
        // Returns the material ID associated with the submesh
        //**********************************************************************
        virtual uint32_t GetMaterialID() = 0;

        //**********************************************************************
        // Method: SetMaterialID
        // Sets the material ID for the submesh
        // 
        // Parameters:
        // materialID - ID of material
        //**********************************************************************
        virtual void SetMaterialID(uint32_t materialID) = 0;

        //**********************************************************************
        // Method: GetBBOX
        // Returns the bounding box of the submesh
        //**********************************************************************
        virtual const BBox3 &GetBBox() = 0;

        //**********************************************************************
        // Method: Normalize
        // Rescales a mesh so fits inside a -0.5..+0.5 bounding box
        //
        // Parameters:
        // bbox - Bounding box of the mesh
        //**********************************************************************
        virtual void Normalize(const BBox3 &bbox) = 0;

        //**********************************************************************
        // Method: ComputeNormals
        // This method will compute all the vertex normals
        // on the mesh by summing and normalizing the face vertices
        // around each vertex (via Newell's method). NOTE: This makes
        // an implicit assumption that each face is planar. This is not
        // always the case and can thus result in incorrect normals (for
        // instance, the faces from CreateSphere() will not be planar).
        // To prevent incorrect normals, it is first recommended that
        // the mesh be triangulated first via CSubMesh::Triangulate().
        //**********************************************************************
        virtual void ComputeNormals() = 0;

        //**********************************************************************
        // Method: VertexToIndex
        // Given a vertex this returns the index of that vertex
        // 
        // Parameters:
        // pVertex - vertex to convert into index
        //**********************************************************************
        virtual uint32_t VertexToIndex(CGeomVertex *pVertex) = 0;

        //**********************************************************************
        // Method: FaceToIndex
        // Given a face this returns the index of that face
        // 
        // Parameters:
        // pFace - face to convert into index
        //**********************************************************************
        virtual uint32_t FaceToIndex(CFace *pFace) = 0;

        //**********************************************************************
        // Method: EdgeToIndex
        // Given a edge this returns the index of that edge
        // 
        // Parameters:
        // pEdge - edge to convert into index
        //**********************************************************************
        virtual uint32_t EdgeToIndex(CHalfEdge *pEdge) = 0;

        //**********************************************************************
        // Method: Triangulate
        // Converts a mesh from arbitrary polygons into
        // a triangulated mesh.
        //
        // Parameters:
        // method - method to use for triangulation
        //**********************************************************************
        virtual void Triangulate(ETriangulateMethod method) = 0;

        //**********************************************************************
        // Method: ToMeshData
        // Converts a CSubMesh object into a renderable form
        //
        // Parameters:
        // pDevice - device
        // vertexLayout - description of each vertex's layout
        // vertexSize - size of each vertex in bytes
        //
        // Returns:
        // Returns renderable form of the mesh
        //**********************************************************************
        virtual MeshData ToMeshData(ID3D11Device* pDevice, std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout, uint32_t vertexSize) = 0;
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
        //**********************************************************************
        // Method: RayIntersect
        // Computes the intersection of a ray with a mesh
        //
        // Parameters:
        // ray - ray direction to trace
        // pIntersection - returns the intersection point if a ray intersect occurs
        // pMaterialID - returns the material for the part of the mesh interested
        //**********************************************************************
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, uint32_t* pMaterialID) = 0;
         
        //**********************************************************************
        // Method: NumberSubMeshes
        // Returns number of submeshes in the mesh
        //**********************************************************************
        virtual uint32_t NumberSubMeshes() = 0;

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
        virtual CRefObj<ISubMesh> GetSubMesh(uint32_t index) = 0;

        //**********************************************************************
        // Method: AddSubMesh
        // Adds a submesh to the mesh
        //
        // Parameters:
        // pSubMesh - Adds the specified submesh to our mesh
        //**********************************************************************
        virtual void AddSubMesh(ISubMesh *pSubMesh) = 0;

        //**********************************************************************
        // Method: GetBBox
        // Returns the bounding box for the mesh
        //
        // Parameters:
        // pBBox - Returns the bounding box of the mesh
        //**********************************************************************
        virtual void GetBBox(BBox3 *pBBox) = 0;

        //**********************************************************************
        // Method: Normalize
        // Rescales a mesh so that it has unit size
        //**********************************************************************
        virtual void Normalize() = 0;

        //**********************************************************************
        // Method: SetMaterials
        // Sets the mesh's material list
        //
        // Parameters:
        // materials - Assigns the mesh the specified list of materials
        //**********************************************************************
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) = 0;

        //**********************************************************************
        // Method: GetNumberMaterials
        // Returns number of materials currently assigned to mesh
        //
        // Returns:
        // Returns number of materials
        //**********************************************************************
        virtual uint32_t GetNumberMaterials() = 0;

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
        virtual CRefObj<IMaterialAttrib> GetMaterial(uint32_t materialID) = 0;

        //**********************************************************************
        // Method: ComputeNormals
        // Computes normal vectors for each vertex on the mesh
        //**********************************************************************
        virtual void ComputeNormals() = 0;

        //**********************************************************************
        // Method: ToMeshData
        // Converts a CMesh object into a renderable form
        //
        // Parameters:
        // pDevice - device
        // vertexLayout - description of each vertex's layout
        // vertexSize - size of each vertex in bytes
        //
        // Returns:
        // Returns list of MeshData associated with each submesh
        //**********************************************************************
        virtual std::vector<MeshData> ToMeshData(ID3D11Device* pDevice, std::vector<D3D11_INPUT_ELEMENT_DESC>& vertexLayout, uint32_t vertexSize) = 0;
    };
}
