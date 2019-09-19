//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "RenderMesh.h"
#include "ShaderInfo.h"
#include <d3d12.h>

namespace Caustic
{
    //**********************************************************************
    // Method: Render
    // See <IRenderSubMesh::Render>
    //**********************************************************************
    void CRenderSubMesh::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights)
	{
        ID3D11DeviceContext *pContext = pRenderer->GetContext();
        ID3D11Device *pDevice = pRenderer->GetDevice();
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_spShader->BeginRender(pRenderer, m_spFrontMaterial, m_spBackMaterial, lights, nullptr);
		uint32 vertexSize = m_spShader->GetShaderInfo()->GetVertexSize();
		uint32 numVertices = m_VB.m_numVertices;
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &m_VB.m_spVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_VB.m_spIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        pContext->DrawIndexed(m_VB.m_numIndices, 0, 0);
		m_spShader->EndRender(pRenderer);
	}

    //**********************************************************************
    // Constructor: CRenderMesh
    // Ctor for CRenderMesh
    //**********************************************************************
    CRenderMesh::CRenderMesh()
	{
	}
	
    //**********************************************************************
    // Method: GetSubMesh
    // See <IRenderMesh::GetSubMesh>
    //**********************************************************************
    void CRenderMesh::GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh)
	{
		*ppSubMesh = m_subMeshes[index];
		(*ppSubMesh)->AddRef();
	}

    //**********************************************************************
    // Method: AddSubMesh
    // See <IRenderMesh::AddSubMesh>
    //**********************************************************************
    void CRenderMesh::AddSubMesh(IRenderSubMesh *pSubMesh)
	{
		m_subMeshes.push_back(pSubMesh);
	}

    //**********************************************************************
    // Method: GetBBox
    // See <IRenderMesh::GetBBox>
    //**********************************************************************
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
	
    //**********************************************************************
    // Method: SetMaterials
    // See <IRenderMesh::SetMaterials>
    //**********************************************************************
    void CRenderMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials)
	{
		m_materials.clear();
		for (auto p : materials)
			m_materials.push_back(p);
	}

    //**********************************************************************
    // Method: GetMaterial
    // See <IRenderMesh::GetMaterial>
    //**********************************************************************
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

    //**********************************************************************
    // Method: Render
    // See <IRenderMesh::Render>
    //**********************************************************************
    void CRenderMesh::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights)
	{
		for (auto submesh : m_subMeshes)
			submesh->Render(pRenderer, lights);
	}
}
