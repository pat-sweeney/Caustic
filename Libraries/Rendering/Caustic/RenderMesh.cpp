//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "ConstructBuffers.h"
#include "RenderMesh.h"
#include "ShaderInfo.h"
#include <d3d12.h>

namespace Caustic
{
    //**********************************************************************
    // Method: Render
    // See <IRenderSubMesh::Render>
    //**********************************************************************
    void CRenderSubMesh::Render(IRenderer* pRenderer, std::vector<CRefObj<IPointLight>>& lights)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();
        ID3D11Device* pDevice = pRenderer->GetDevice();
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
    // Method: Render
    // See <IRenderSubMesh::Render>
    //**********************************************************************
    void CRenderSubMesh::Render(IRenderer* pRenderer, IShader *pShader, IRenderMaterial *pMaterial, std::vector<CRefObj<IPointLight>>& lights)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();
        ID3D11Device* pDevice = pRenderer->GetDevice();
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pShader->BeginRender(pRenderer, pMaterial, pMaterial, lights, nullptr);
        uint32 vertexSize = pShader->GetShaderInfo()->GetVertexSize();
        uint32 numVertices = m_VB.m_numVertices;
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &m_VB.m_spVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_VB.m_spIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        pContext->DrawIndexed(m_VB.m_numIndices, 0, 0);
        pShader->EndRender(pRenderer);
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
    void CRenderMesh::Render(IRenderer* pRenderer, std::vector<CRefObj<IPointLight>>& lights)
    {
		for (auto submesh : m_subMeshes)
            submesh->Render(pRenderer, lights);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderMesh::Render>
    //**********************************************************************
    void CRenderMesh::Render(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pMaterial, std::vector<CRefObj<IPointLight>>& lights)
    {
        for (auto submesh : m_subMeshes)
            submesh->Render(pRenderer, pShader, pMaterial, lights);
    }

    CAUSTICAPI CRefObj<IRenderSubMesh> CreatePointCloudSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, std::vector<CGeomVertex>& verts)
    {
        if (verts.size() == 0)
            return CRefObj<IRenderSubMesh>(nullptr);
        std::vector<CGeomVertex> triangles;
        std::vector<uint32> faces;
        for (size_t i = 0; i < verts.size(); i++)
        {
            Vector3 pos = verts[i].pos;
            Vector3 v(0.0f, 1.0f, 0.0f);
            Vector3 u = verts[i].norm.cross(v);
            u.Normalize();
            v = u.cross(verts[i].norm);
            v.Normalize();
            const float stepSize = 0.01f;
            CGeomVertex p0, p1, p2, p3;
            p0.pos = pos + u * stepSize + v * stepSize;
            p1.pos = pos - u * stepSize + v * stepSize;
            p2.pos = pos - u * stepSize - v * stepSize;
            p3.pos = pos + u * stepSize - v * stepSize;
            p0.uvs[0] = Vector2(1.0f, 1.0f);
            p1.uvs[0] = Vector2(0.0f, 1.0f);
            p2.uvs[0] = Vector2(0.0f, 0.0f);
            p3.uvs[0] = Vector2(1.0f, 0.0f);
            p0.norm = verts[i].norm;
            p1.norm = verts[i].norm;
            p2.norm = verts[i].norm;
            p3.norm = verts[i].norm;
            triangles.push_back(p0);
            triangles.push_back(p1);
            triangles.push_back(p2);
            triangles.push_back(p0);
            triangles.push_back(p2);
            triangles.push_back(p3);
            faces.push_back(int(6 * i + 0));
            faces.push_back(int(6 * i + 1));
            faces.push_back(int(6 * i + 2));
            faces.push_back(int(6 * i + 3));
            faces.push_back(int(6 * i + 4));
            faces.push_back(int(6 * i + 5));
        }
        
        CRefObj<ICausticFactory> spFactory = CreateCausticFactory();
        CRefObj<IRenderSubMesh> spSubMesh = spFactory->CreateRenderSubMesh();
        spSubMesh->SetBackMaterial(pBackMaterial);
        spSubMesh->SetFrontMaterial(pFrontMaterial);
        spSubMesh->SetShader(pShader);
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CRefObj<IShaderInfo> spShaderInfo = pShader->GetShaderInfo();
        MeshData meshData;
        BuildVertexBuffer(pRenderer, spShaderInfo, triangles, &meshData);
        BuildIndexBuffer(pRenderer, faces, &meshData);
        spSubMesh->SetMeshData(meshData);
        return spSubMesh;
    }
}
