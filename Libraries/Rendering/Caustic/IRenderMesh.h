//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\IRenderMaterial.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IShaderInfo.h"
#include "Rendering\Caustic\IRenderMesh.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\BBox.h"

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
        virtual uint32 GetMeshFlags() = 0;
        virtual void SetMeshFlags(uint32 flags) = 0;
        virtual void SetFrontMaterial(IRenderMaterial *pFrontMaterial) = 0;
		virtual CRefObj<IRenderMaterial> GetFrontMaterial() = 0;
		virtual void SetBackMaterial(IRenderMaterial *pBackMaterial) = 0;
		virtual CRefObj<IRenderMaterial> GetBackMaterial() = 0;
        virtual void SetMeshData(MeshData &meshData, bool hasNormals = false) = 0;
        virtual const MeshData &GetMeshData(bool withNormals = false) = 0;
        virtual void SetBBox(BBox3 *pBBox) = 0;
        virtual void GetBBox(BBox3 *pBBox) = 0;
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) = 0;
		virtual void SetShader(IShader *pShader) = 0;
		virtual CRefObj<IShader> GetShader() = 0;
	};

	//**********************************************************************
	// Interface: IRenderMesh
	// IRenderMesh is simply a collection of submesh objects (IRenderSubMesh).
	//**********************************************************************
	struct IRenderMesh : public ISerialize
	{
		virtual uint32 NumberSubMeshes() = 0;
		virtual void GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh) = 0;
		virtual void AddSubMesh(IRenderSubMesh *pSubMesh) = 0;
		virtual void GetBBox(BBox3 *pBBox) = 0;
		virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) = 0;
		virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) = 0;
		virtual void ComputeNormals() = 0;
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) = 0;
	};
}
