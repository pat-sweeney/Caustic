//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base/Math/BBox.h"
#include "IRenderMesh.h"
#include <d3d12.h>

namespace Caustic
{
	class CRenderSubMesh : public IRenderSubMesh, public CRefCount
	{
		CComPtr<ID3D12Resource> m_spVB;
		CComPtr<ID3D12Resource> m_spIB;
		CRefObj<IRenderMaterial> m_spFrontMaterial;
		CRefObj<IRenderMaterial> m_spBackMaterial;
		CRefObj<IShader> m_spShader;
		uint32 m_numVerts;
		uint32 m_numIndices;
		Caustic::BBox3 m_bbox;
	public:
		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderSubMesh
		//**********************************************************************
		virtual void SetFrontMaterial(IRenderMaterial *pFrontMaterial) override { m_spFrontMaterial = pFrontMaterial; }
		virtual CRefObj<IRenderMaterial> GetFrontMaterial() override { return m_spFrontMaterial; }
		virtual void SetBackMaterial(IRenderMaterial *pBackMaterial) override { m_spBackMaterial = pBackMaterial; }
		virtual CRefObj<IRenderMaterial> GetBackMaterial() override { return m_spBackMaterial; }
		virtual void SetNumberVertices(uint32 numVertices) override { m_numVerts = numVertices; }
		virtual uint32 GetNumberVertices() override { return m_numVerts; }
		virtual void SetNumberIndices(uint32 numIndices) override { m_numIndices = numIndices; }
		virtual uint32 GetNumberIndices() override { return m_numIndices; }
		virtual void SetVertexBuffer(ID3D12Resource *pVB) override { m_spVB = pVB; }
		virtual CComPtr<ID3D12Resource> GetVertexBuffer() override { return m_spVB; }
		virtual void SetIndexBuffer(ID3D12Resource *pIB) override { m_spIB = pIB; }
		virtual CComPtr<ID3D12Resource> GetIndexBuffer() override { return m_spIB; }
		virtual void GetBBox(BBox3 *pBBox) override { *pBBox = m_bbox; };
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) override;
		virtual void SetShader(IShader *pShader) override { m_spShader = pShader; }
		virtual CRefObj<IShader> GetShader() override { return m_spShader; }
	};

	//**********************************************************************
	class CRenderMesh : public IRenderMesh, public CRefCount
	{
		std::vector<CRefObj<IRenderSubMesh>> m_subMeshes;
		std::vector<CRefObj<IMaterialAttrib>> m_materials;
	public:
		CRenderMesh();

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderMesh
		//**********************************************************************
		virtual uint32 NumberSubMeshes() override { return (uint32)m_subMeshes.size(); };
		virtual void GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh) override;
		virtual void AddSubMesh(IRenderSubMesh *pSubMesh) override;
		virtual void GetBBox(Caustic::BBox3 *pBBox) override;
		virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) override;
		virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) override;
		virtual void ComputeNormals() override {}
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights);

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override {}
		virtual void Store(IStream *pStream) override {}
	};
}
