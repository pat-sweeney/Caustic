//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "RenderMesh.h"
#include <d3d12.h>

namespace Caustic
{
	void CRenderSubMesh::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights)
	{
		CComPtr<ID3D12GraphicsCommandList> spCommandList = pRenderer->GetCommandList();
		spCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_spShader->BeginRender(pRenderer, lights);
		uint32 vertexSize = m_spShader->GetShaderInfo()->GetVertexSize();
		uint32 numVertices = GetNumberVertices();
		D3D12_VERTEX_BUFFER_VIEW vbView;
		vbView.BufferLocation = m_spVB->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSize;
		vbView.SizeInBytes = (UINT)(numVertices * vertexSize);
		spCommandList->IASetVertexBuffers(0, 1, &vbView);
		D3D12_INDEX_BUFFER_VIEW ibView;
		ibView.BufferLocation = m_spIB->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R32_UINT;
		uint32 numIndices = GetNumberIndices();
		ibView.SizeInBytes = numIndices * sizeof(uint32);
		spCommandList->IASetIndexBuffer(&ibView);
		spCommandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
		m_spShader->EndRender(pRenderer);
	}

	CRenderMesh::CRenderMesh()
	{
	}
	
	void CRenderMesh::GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh)
	{
		*ppSubMesh = m_subMeshes[index];
		(*ppSubMesh)->AddRef();
	}

	void CRenderMesh::AddSubMesh(IRenderSubMesh *pSubMesh)
	{
		m_subMeshes.push_back(pSubMesh);
	}

	void CRenderMesh::GetBBox(Caustic::BBox3 *pBBox)
	{
		pBBox->minPt = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		pBBox->maxPt = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (size_t i = 0; i < m_subMeshes.size(); i++)
		{
			BBox3 bb;
			m_subMeshes[i]->GetBBox(&bb);
			pBBox->AddPoint(bb.minPt.x, bb.minPt.y, bb.minPt.z);
			pBBox->AddPoint(bb.maxPt.x, bb.maxPt.y, bb.maxPt.z);
		}
	}
	
	void CRenderMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials)
	{
		m_materials.clear();
		for (auto p : materials)
			m_materials.push_back(p);
	}

	void CRenderMesh::GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial)
	{
		if (materialID < m_materials.size())
		{
			(*ppMaterial) = m_materials[materialID];
			(*ppMaterial)->AddRef();
		}
		else
			*ppMaterial = nullptr;
	}

	void CRenderMesh::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights)
	{
		for (auto submesh : m_subMeshes)
			submesh->Render(pRenderer, lights);
	}
}
