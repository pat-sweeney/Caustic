//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Rendering\Caustic\IRenderMaterial.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IShaderInfo.h"
#include "Rendering\Caustic\IRenderMesh.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"

//**********************************************************************
// File: IRenderMesh.h
// Defines interface for Render mesh. Render mesh is the mirror of <IMesh>
// but defines the mesh as a vertex/index buffer pair.
//**********************************************************************

namespace Caustic
{
	//**********************************************************************
	// Interface: IRenderSubMesh
	// Defines the D3D counterpart to an ISubMesh object (i.e. the renderable
	// form of a mesh, that is, a mesh converted from a half-edge structure
	// to a vertex and index buffer).
	//**********************************************************************
	struct IRenderSubMesh : public IRefCount
	{
        //**********************************************************************
        // Method: GetMeshFlags
        // Returns:
        // Returns a set of flags associated with the mesh. See <EMeshFlags>
        // for flag definitions.
        //**********************************************************************
        virtual uint32 GetMeshFlags() = 0;

        //**********************************************************************
        // Method: SetMeshFlags
        // Set the flags associated with the mesh. See <EMeshFlags>
        // for flag definitions.
        //
        // Parameters:
        // flags - set of flags to apply
        //**********************************************************************
        virtual void SetMeshFlags(uint32 flags) = 0;

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
        // lights - List of lights to use
        //**********************************************************************
        virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) = 0;

        //**********************************************************************
        // Method: SetShader
        // Sets the shader to use when rendering this mesh
        //
        // Parameters:
        // pShader - shader
        //**********************************************************************
        virtual void SetShader(IShader *pShader) = 0;

        //**********************************************************************
        // Method: GetShader
        // Returns:
        // Returns the shader associated with this mesh
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() = 0;
	};

	//**********************************************************************
	// Interface: IRenderMesh
	// IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).
	//**********************************************************************
	struct IRenderMesh : public ISerialize
	{
        //**********************************************************************
        // Method: NumberSubMeshes
        // Returns:
        // Number of submeshes for this mesh
        //**********************************************************************
        virtual uint32 NumberSubMeshes() = 0;

        //**********************************************************************
        // Method: GetSubMesh
        // Retrieves the Nth submesh
        //
        // Parameters:
        // index - index of submesh to retrieve
        // ppSubMesh - Returns the Nth submesh
        //**********************************************************************
        virtual void GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh) = 0;

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
        // ppMaterial - returns the associated material
        //**********************************************************************
        virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) = 0;

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
        // lights - list of lights to use
        //**********************************************************************
        virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) = 0;
	};
}
