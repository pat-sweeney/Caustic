//**********************************************************************
// Copyright Patrick Sweeney 2019-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <float.h>
#include <cinttypes>

module Rendering.Caustic.RenderMesh;
import Base.Core.Core;
import Base.Core.Error;
import Rendering.Caustic.ShaderInfo;
import Rendering.Caustic.ConstructBuffer;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.IShader;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.ILight;

namespace Caustic
{
    //**********************************************************************
    void CRenderSubMesh::RenderSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderMaterial *pMaterial,
        std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();

        // Set morph target params (always set to prevent stale values)
        pShader->SetVSParam(L"morphTargetCount", std::any((int)m_numMorphTargets));
        pShader->SetVSParam(L"morphNumVertices", std::any((int)m_morphNumVertices));
        pShader->SetVSParam(L"morphWeights0", std::any(Float4(m_morphWeights[0], m_morphWeights[1], m_morphWeights[2], m_morphWeights[3])));
        pShader->SetVSParam(L"morphWeights1", std::any(Float4(m_morphWeights[4], m_morphWeights[5], m_morphWeights[6], m_morphWeights[7])));

        pShader->BeginRender(pRenderer, pMaterial, lights, pWorld);

        // Manually bind morph StructuredBuffer SRV to VS slot 0
        // (engine's shader param system only supports PS/CS SRV binding)
        if (m_numMorphTargets > 0 && m_spMorphBuffer != nullptr)
        {
            CComPtr<ID3D11ShaderResourceView> spSRV = m_spMorphBuffer->GetSRView();
            ID3D11ShaderResourceView* pSRV = spSRV.p;
            pContext->VSSetShaderResources(0, 1, &pSRV);
        }
        else
        {
            ID3D11ShaderResourceView* pNullSRV = nullptr;
            pContext->VSSetShaderResources(0, 1, &pNullSRV);
        }

        uint32_t vertexSize = pShader->GetShaderInfo()->GetVertexSize();
        uint32_t numVertices = m_VB.m_numVertices;
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &m_VB.m_spVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_VB.m_spIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        pContext->DrawIndexed(m_VB.m_numIndices, 0, 0);

        // Unbind VS SRV to avoid hazards
        if (m_numMorphTargets > 0)
        {
            ID3D11ShaderResourceView* pNullSRV = nullptr;
            pContext->VSSetShaderResources(0, 1, &pNullSRV);
        }

        pShader->EndRender(pRenderer);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderSubMesh::Render>
    //**********************************************************************
    void CRenderSubMesh::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IRenderMaterial* pFrontMaterialOverride,
        IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights,
        DirectX::XMMATRIX *pWorld)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();
        ID3D11Device* pDevice = pRenderer->GetDevice();
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        CRefObj<IShader> spShader;
        if (pRenderer->GetRenderCtx()->GetCurrentPass() == c_PassShadow)
            spShader = pRenderer->GetShaderMgr()->FindShader(L"ShadowMap");
        else
        {
            spShader = (m_spFrontMaterial) ? m_spFrontMaterial->GetShader() : nullptr;
            if (spShader == nullptr)
            {
                auto spMaterialAttrib = (m_spFrontMaterial) ? m_spFrontMaterial->GetMaterial() : nullptr;
                if (spMaterialAttrib && spMaterialAttrib->GetScalar(L"pbrModel") > 0.0f)
                    spShader = pRenderer->GetShaderMgr()->FindShader(L"PBR");
                else
                {
                    CRefObj<IImage> spDiffuseTexture = (spMaterialAttrib) ? spMaterialAttrib->GetTexture(L"diffuseTexture") : nullptr;
                    spShader = pRenderer->GetShaderMgr()->FindShader((spDiffuseTexture == nullptr) ? L"Default" : L"Textured");
                }
            }
        }
        if (spShader)
        {
            //**********************************************************************
            // There are two ways we could handle backside materials:
            // 1) Render the geomtry twice. Once for the front, then again
            // for the back but switching the winding order.
            // 2) Allow the shader to switch its selection of textures and other
            // constants based on the VFACE setting.
            //
            // Originally I was going to go with option 2 since it would decrease
            // the number of geometry render calls. However, I ultimately decided
            // to go with option 1 since it simplifies the shaders and it isn't
            // very often where we have actual back face materials.
            //**********************************************************************
            auto spRenderMaterial = (pFrontMaterialOverride) ? pFrontMaterialOverride : m_spFrontMaterial.p;
            uint32_t currentPass = pRenderCtx->GetCurrentPass();
            auto spMaterialAttrib = (spRenderMaterial) ? spRenderMaterial->GetMaterial() : nullptr;
            if (spMaterialAttrib == nullptr)
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
            else if ((currentPass == c_PassTransparent) && spMaterialAttrib->GetIsTransparent())
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
            else if ((currentPass != c_PassTransparent) && !spMaterialAttrib->GetIsTransparent())
            {
                CComPtr<ID3D11RasterizerState> spRasterizerState;
                CComPtr<ID3D11RasterizerState> spOldRasterizerState;
                D3D11_RASTERIZER_DESC desc;
                desc.FrontCounterClockwise = false;
                desc.AntialiasedLineEnable = false;
                desc.DepthBias = 0;
                desc.DepthBiasClamp = 0.0f;
                desc.DepthClipEnable = true;
                desc.CullMode = spMaterialAttrib->GetCullMode();
                desc.FillMode = spMaterialAttrib->GetFillMode();
                desc.MultisampleEnable = true;
                desc.SlopeScaledDepthBias = 0.0f;
                desc.ScissorEnable = false;
                CT(pDevice->CreateRasterizerState(&desc, &spRasterizerState));
                pContext->RSGetState(&spOldRasterizerState);
                pContext->RSSetState(spRasterizerState);
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
                pContext->RSSetState(spOldRasterizerState);
            }
            if (pBackMaterialOverride || m_spBackMaterial)
            {
                auto spRenderMaterial = (pBackMaterialOverride) ? pBackMaterialOverride : m_spBackMaterial.p;
                uint32_t currentPass = pRenderCtx->GetCurrentPass();
                auto spMaterialAttrib = spRenderMaterial->GetMaterial();
                if (((currentPass == c_PassTransparent) && spMaterialAttrib->GetIsTransparent()) ||
                    ((currentPass != c_PassTransparent) && !spMaterialAttrib->GetIsTransparent()))
                {
                    CComPtr<ID3D11RasterizerState> spRasterizerState;
                    CComPtr<ID3D11RasterizerState> spOldRasterizerState;
                    D3D11_RASTERIZER_DESC desc;
                    desc.FrontCounterClockwise = false;
                    desc.AntialiasedLineEnable = false;
                    desc.DepthBias = 0;
                    desc.DepthBiasClamp = 0.0f;
                    desc.DepthClipEnable = true;
                    desc.CullMode = spMaterialAttrib->GetCullMode();
                    desc.FillMode = spMaterialAttrib->GetFillMode();
                    desc.MultisampleEnable = true;
                    desc.SlopeScaledDepthBias = 0.0f;
                    desc.ScissorEnable = false;
                    CT(pDevice->CreateRasterizerState(&desc, &spRasterizerState));
                    pContext->RSGetState(&spOldRasterizerState);
                    pContext->RSSetState(spRasterizerState);
                    RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
                    pContext->RSSetState(spOldRasterizerState);
                }
            }
        }
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderSubMesh::Render>
    //**********************************************************************
    void CRenderSubMesh::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IShader *pShader, IRenderMaterial *pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();
        ID3D11Device* pDevice = pRenderer->GetDevice();
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pShader->BeginRender(pRenderer, pMaterial, lights, pWorld);
        uint32_t vertexSize = pShader->GetShaderInfo()->GetVertexSize();
        uint32_t numVertices = m_VB.m_numVertices;
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &m_VB.m_spVB.p, &vertexSize, &offset);
        if (m_VB.m_numIndices == 0)
            pContext->Draw(m_VB.m_numVertices, 0);
        else
        {
            pContext->IASetIndexBuffer(m_VB.m_spIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
            pContext->DrawIndexed(m_VB.m_numIndices, 0, 0);
        }
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
    CRefObj<IRenderSubMesh> CRenderMesh::GetSubMesh(uint32_t index)
    {
        return m_subMeshes[index];
    }

    //**********************************************************************
    // Method: AddSubMesh
    // See <IRenderMesh::AddSubMesh>
    //**********************************************************************
    void CRenderMesh::AddSubMesh(IRenderSubMesh* pSubMesh)
    {
        m_subMeshes.push_back(pSubMesh);
    }

    //**********************************************************************
    // Method: GetBBox
    // See <IRenderMesh::GetBBox>
    //**********************************************************************
    void CRenderMesh::GetBBox(Caustic::BBox3* pBBox)
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
    void CRenderMesh::SetMaterials(std::vector<CRefObj<IMaterialAttrib>>& materials)
    {
        m_materials.clear();
        for (auto p : materials)
            m_materials.push_back(p);
    }

    //**********************************************************************
    // Method: GetMaterial
    // See <IRenderMesh::GetMaterial>
    //**********************************************************************
    CRefObj<IMaterialAttrib> CRenderMesh::GetMaterial(uint32_t materialID)
    {
        if (materialID < m_materials.size())
            return m_materials[materialID];
        return CRefObj<IMaterialAttrib>(nullptr);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderMesh::Render>
    //**********************************************************************
    void CRenderMesh::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IRenderMaterial* pFrontMaterialOverride, IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        for (auto submesh : m_subMeshes)
            submesh->Render(pRenderer, pRenderCtx, pFrontMaterialOverride, pBackMaterialOverride, lights, pWorld);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderMesh::Render>
    //**********************************************************************
    void CRenderMesh::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IShader* pShader, IRenderMaterial* pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        for (auto submesh : m_subMeshes)
            submesh->Render(pRenderer, pRenderCtx, pShader, pMaterial, lights, pWorld);
    }

    CRefObj<IRenderSubMesh> CreatePointCloudSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, std::vector<CGeomVertex>& verts, bool useUVs /* = false */)
    {
        if (verts.size() == 0)
            return CRefObj<IRenderSubMesh>(nullptr);
        std::vector<CGeomVertex> triangles;
        std::vector<uint32_t> faces;
        for (size_t i = 0; i < verts.size(); i++)
        {
            Vector3 pos = verts[i].pos;
            Vector3 v(0.0f, 1.0f, 0.0f);
            Vector3 u = verts[i].norm.Cross(v);
            u.Normalize();
            v = u.Cross(verts[i].norm);
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
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CRefObj<IShaderInfo> spShaderInfo = pShader->GetShaderInfo();
        MeshData meshData;
        BuildVertexBuffer(pRenderer, spShaderInfo, triangles, &meshData);
        BuildIndexBuffer(pRenderer, faces, &meshData);
        spSubMesh->SetMeshData(meshData);
        return spSubMesh;
    }

    //**********************************************************************
    // Method: SetMorphTargets
    // See <IRenderSubMesh::SetMorphTargets>
    //**********************************************************************
    void CRenderSubMesh::SetMorphTargets(IRenderer* pRenderer, std::vector<std::vector<MorphTargetDelta>>& targets)
    {
        m_numMorphTargets = (int)targets.size();
        if (m_numMorphTargets > c_MaxMorphTargets)
            m_numMorphTargets = c_MaxMorphTargets;

        if (m_numMorphTargets == 0 || targets[0].empty())
        {
            m_spMorphBuffer = nullptr;
            m_numMorphTargets = 0;
            m_morphNumVertices = 0;
            return;
        }

        m_morphNumVertices = (int)targets[0].size();

        // Pack all targets into a flat buffer: [target0_v0, target0_v1, ..., target1_v0, ...]
        uint32_t totalElems = m_numMorphTargets * m_morphNumVertices;
        std::vector<MorphTargetDelta> flatData(totalElems);
        for (int t = 0; t < m_numMorphTargets; t++)
        {
            int numVerts = (int)targets[t].size();
            for (int v = 0; v < m_morphNumVertices; v++)
            {
                if (v < numVerts)
                    flatData[t * m_morphNumVertices + v] = targets[t][v];
                else
                    flatData[t * m_morphNumVertices + v] = { Vector3(0,0,0), Vector3(0,0,0) };
            }
        }

        // Create StructuredBuffer (each element = MorphTargetDelta = 2x float3 = 24 bytes)
        m_spMorphBuffer = CreateGPUBuffer(pRenderer, EBufferType::StructuredBuffer,
            totalElems, sizeof(MorphTargetDelta), D3D11_BIND_SHADER_RESOURCE);
        m_spMorphBuffer->CopyFromCPU(pRenderer, (uint8_t*)flatData.data());
    }

    //**********************************************************************
    // Method: SetMorphWeights
    // See <IRenderSubMesh::SetMorphWeights>
    //**********************************************************************
    void CRenderSubMesh::SetMorphWeights(float* weights, int count)
    {
        memset(m_morphWeights, 0, sizeof(m_morphWeights));
        int numWeights = (count < c_MaxMorphTargets) ? count : c_MaxMorphTargets;
        for (int i = 0; i < numWeights; i++)
            m_morphWeights[i] = weights[i];
    }
}
