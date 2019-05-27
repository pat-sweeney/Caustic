//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base/Core/IRefCount.h"
#include "Base/Math/BBox.h"

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
		virtual void SetFrontMaterial(IRenderMaterial *pFrontMaterial) = 0;
		virtual CRefObj<IRenderMaterial> GetFrontMaterial() = 0;
		virtual void SetBackMaterial(IRenderMaterial *pBackMaterial) = 0;
		virtual CRefObj<IRenderMaterial> GetBackMaterial() = 0;
		virtual void SetNumberVertices(uint32 numVertices) = 0;
		virtual uint32 GetNumberVertices() = 0;
		virtual void SetNumberIndices(uint32 numIndices) = 0;
		virtual uint32 GetNumberIndices() = 0;
		virtual void SetVertexBuffer(ID3D12Resource *pVB) = 0;
		virtual CComPtr<ID3D12Resource> GetVertexBuffer() = 0;
		virtual void SetIndexBuffer(ID3D12Resource *pIB) = 0;
		virtual CComPtr<ID3D12Resource> GetIndexBuffer() = 0;
		virtual void GetBBox(BBox3 *pBBox) = 0;
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
	};
}
