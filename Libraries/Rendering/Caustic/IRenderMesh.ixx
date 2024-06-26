//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

export module Rendering.Caustic.IRenderMesh;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.BBox;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IShaderInfo;
import Rendering.Caustic.IShader;
import Geometry.Mesh.RenderTypes;
import Geometry.Mesh.IMaterialAttrib;

//**********************************************************************
// File: IRenderMesh.ixx
// Defines interface for Render mesh. Render mesh is the mirror of <IMesh>
// but defines the mesh as a vertex/index buffer pair.
//**********************************************************************

export namespace Caustic
{
    struct IRenderer;

    //**********************************************************************
    // Interface: IRenderSubMesh
    // Defines the D3D counterpart to an ISubMesh object (i.e. the renderable
    // form of a mesh, that is, a mesh converted from a half-edge structure
    // to a vertex and index buffer).
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderMesh;{Rendering/Caustic/IRenderMesh.ixx}}
    //**********************************************************************
    struct IRenderSubMesh : public IRefCount
    {
        virtual void SetName(const char* pName) = 0;
        virtual std::string GetName() = 0;

        //**********************************************************************
        // Method: GetMeshFlags
        // Returns:
        // Returns a set of flags associated with the mesh. See <EMeshFlags>
        // for flag definitions.
        //**********************************************************************
        virtual uint32_t GetMeshFlags() = 0;

        //**********************************************************************
        // Method: SetMeshFlags
        // Set the flags associated with the mesh. See <EMeshFlags>
        // for flag definitions.
        //
        // Parameters:
        // flags - set of flags to apply
        //**********************************************************************
        virtual void SetMeshFlags(uint32_t flags) = 0;

        //**********************************************************************
        // Method: SetFrontMaterial
        // Sets the material to use when rendering front faces
        //
        // Parameters:
        // pFrontMaterial - material to use when rendering front faces
        //**********************************************************************
        virtual void SetFrontMaterial(IRenderMaterial *pFrontMaterial) = 0;

        //**********************************************************************
        // Method: GetFrontMaterial
        // Returns:
        // Material to use when rendering front faces
        //**********************************************************************
        virtual CRefObj<IRenderMaterial> GetFrontMaterial() = 0;

        //**********************************************************************
        // Method: SetBackMaterial
        // Sets the material to use when rendering back faces
        //
        // Parameters:
        // pBackMaterial - material to use when rendering back faces
        //**********************************************************************
        virtual void SetBackMaterial(IRenderMaterial *pBackMaterial) = 0;

        //**********************************************************************
        // Method: GetBackMaterial
        // Returns:
        // Material to use when rendering back faces
        //**********************************************************************
        virtual CRefObj<IRenderMaterial> GetBackMaterial() = 0;

        //**********************************************************************
        // Method: SetMeshData
        // Assigns mesh data to the render mesh
        //
        // Parameters:
        // meshData - mesh data for this render mesh
        // hasNormals - flag indicating whether the mesh data contains geometry for the normal vectors
        //**********************************************************************
        virtual void SetMeshData(MeshData &meshData, bool hasNormals = false) = 0;

        //**********************************************************************
        // Method: GetMeshData
        // Returns the mesh data assigned to the render mesh
        //
        // Parameters:
        // withNormals - if true then returns the mesh data associated with geometry containing normals. Otherwise returns the mesh data for the normal geometry.
        //
        // Returns:
        // The MeshData associated with the render mesh
        //**********************************************************************
        virtual const MeshData &GetMeshData(bool withNormals = false) = 0;

        //**********************************************************************
        // Method: SetBBox
        // Sets the bounding box of the mesh
        //
        // Parameters:
        // pBBox - bbox for the mesh data
        //**********************************************************************
        virtual void SetBBox(BBox3 *pBBox) = 0;

        //**********************************************************************
        // Method: GetBBox
        // Returns the bounding box of the mesh
        //
        // Parameters:
        // pBBox - Returns the bbox for the mesh data
        //**********************************************************************
        virtual void GetBBox(BBox3 *pBBox) = 0;

        //**********************************************************************
        // Method: Render
        // Renders the mesh
        //
        // Parameters:
        // pRenderer - Renderer to use for rendering the mesh
        // pRenderCtx - render context
        // pFrontMaterialOverride - material to use to override mesh's material. Maybe nullptr.
        // pBackMaterialOverride - material to use to override mesh's material. Maybe nullptr.
        // lights - List of lights to use
        // pWorld - world matrix. Maybe nullptr
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx *pRenderCtx, IRenderMaterial* pFrontMaterialOverride, IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) = 0;

        //**********************************************************************
        // Method: Render
        // Renders the mesh
        //
        // Parameters:
        // pRenderer - Renderer to use for rendering the mesh
        // pRenderCtx - render context
        // pShader - Shader to use
        // pMaterial - Material to use
        // lights - List of lights to use
        // pWorld - world matrix. Maybe nullptr
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx *pRenderCtx, IShader* pShader, IRenderMaterial* pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) = 0;
    };

    //**********************************************************************
    // Interface: IRenderMesh
    // IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).
    //
    // Module:
    // {Link:import Rendering.Caustic.IRenderMesh;{Rendering/Caustic/IRenderMesh.ixx}}
    //**********************************************************************
    struct IRenderMesh : public ISerialize
    {
        //**********************************************************************
        // Method: NumberSubMeshes
        // Returns:
        // Number of submeshes for this mesh
        //**********************************************************************
        virtual uint32_t NumberSubMeshes() = 0;

        //**********************************************************************
        // Method: GetSubMesh
        // Retrieves the Nth submesh
        //
        // Parameters:
        // index - index of submesh to retrieve
        //
        // Returns:
        // Returns the Nth submesh
        //**********************************************************************
        virtual CRefObj<IRenderSubMesh> GetSubMesh(uint32_t index) = 0;

        //**********************************************************************
        // Method: AddSubMesh
        // Adds a submesh to the mesh
        //
        // Parameters:
        // pSubMesh - Submesh to add
        //**********************************************************************
        virtual void AddSubMesh(IRenderSubMesh *pSubMesh) = 0;

        //**********************************************************************
        // Method: GetBBox
        // Gets the mesh's bounding box
        //
        // Parameters:
        // pBBox - Returns the mesh's bounding box
        //**********************************************************************
        virtual void GetBBox(BBox3 *pBBox) = 0;

        //**********************************************************************
        // Method: SetMaterials
        // Assigns list of materials that submesh's use
        //
        // Parameters:
        // materials - list of materials
        //**********************************************************************
        virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) = 0;

        //**********************************************************************
        // Method: GetMaterial
        // Retrieves a material
        //
        // Parameters:
        // materialID - ID of which material to retrieve
        //
        // Returns:
        // Returns the associated material
        //**********************************************************************
        virtual CRefObj<IMaterialAttrib> GetMaterial(uint32_t materialID) = 0;

        //**********************************************************************
        // Method: ComputeNormals
        // Computes the normals for the mesh
        //**********************************************************************
        virtual void ComputeNormals() = 0;

        //**********************************************************************
        // Method: Render
        // Renders the mesh
        //
        // Parameters:
        // pRenderer - renderer to use
        // pRenderCtx - render context
        // pFrontMaterialOverride - material to use to override mesh's material. Maybe nullptr.
        // pBackMaterialOverride - material to use to override mesh's material. Maybe nullptr.
        // lights - list of lights to use
        // pWorld - world matrix
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx *pRenderCtx, IRenderMaterial* pFrontMaterialOverride, IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) = 0;

        //**********************************************************************
        // Method: Render
        // Renders the mesh
        //
        // Parameters:
        // pRenderer - renderer to use
        // pShader - shader to use
        // pMaterial - material to use
        // lights - list of lights to use
        // pWorld - world matrix
        //**********************************************************************
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IShader *pShader, IRenderMaterial*pMaterial, std::vector<CRefObj<ILight>> &lights, DirectX::XMMATRIX* pWorld) = 0;
    };

    CRefObj<IRenderSubMesh> BuildRenderSubMesh(IRenderer* pRenderer,
        std::vector<CGeomFace>& faces, std::vector<CGeomVertex>& verts,
        IShader* pShader);
    CRefObj<IRenderSubMesh> CreatePointCloudSubMesh(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, std::vector<CGeomVertex>& verts, bool useUVs = false);
}
