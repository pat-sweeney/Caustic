//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Rendering\Caustic\Caustic.h"
#include "ConstructBuffers.h"
#include <d3d12.h>
import Rendering.Caustic.ShaderInfo;
import Rendering.Caustic.RenderMesh;

namespace Caustic
{
    //**********************************************************************
    void CRenderSubMesh::RenderSubMesh(IRenderer *pRenderer, IShader *pShader, IRenderMaterial *pMaterial,
        std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld)
    {
        ID3D11DeviceContext* pContext = pRenderer->GetContext();
        pShader->BeginRender(pRenderer, pMaterial, lights, pWorld);
        uint32 vertexSize = pShader->GetShaderInfo()->GetVertexSize();
        uint32 numVertices = m_VB.m_numVertices;
        UINT offset = 0;
        pContext->IASetVertexBuffers(0, 1, &m_VB.m_spVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_VB.m_spIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        pContext->DrawIndexed(m_VB.m_numIndices, 0, 0);
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
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        CRefObj<IShader> spShader;
        if (pRenderer->GetRenderCtx()->GetCurrentPass() == c_PassShadow)
            spShader = pRenderer->GetShaderMgr()->FindShader(L"ShadowMap");
        else
        {
            spShader = m_spFrontMaterial->GetShader();
            if (spShader == nullptr)
            {
                auto spMaterialAttrib = m_spFrontMaterial->GetMaterial();
                CRefObj<IImage> spDiffuseTexture = (spMaterialAttrib) ? spMaterialAttrib->GetTexture(L"diffuseTexture") : nullptr;
                spShader = pRenderer->GetShaderMgr()->FindShader((spDiffuseTexture == nullptr) ? L"Default" : L"Textured");
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
            uint32 currentPass = pRenderCtx->GetCurrentPass();
            auto spMaterialAttrib = spRenderMaterial->GetMaterial();
            if (spMaterialAttrib == nullptr)
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
            else if ((currentPass == c_PassTransparent) && spMaterialAttrib->GetIsTransparent())
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
            else if ((currentPass != c_PassTransparent) && !spMaterialAttrib->GetIsTransparent())
                RenderSubMesh(pRenderer, spShader, spRenderMaterial, lights, pWorld);
            if (pBackMaterialOverride || m_spBackMaterial)
            {
                auto spRenderMaterial = (pBackMaterialOverride) ? pBackMaterialOverride : m_spBackMaterial.p;
                uint32 currentPass = pRenderCtx->GetCurrentPass();
                auto spMaterialAttrib = spRenderMaterial->GetMaterial();
                if (((currentPass == c_PassTransparent) && spMaterialAttrib->GetIsTransparent()) ||
                    ((currentPass != c_PassTransparent) && !spMaterialAttrib->GetIsTransparent()))
                {
                    CComPtr<ID3D11RasterizerState> spRasterizerState;
                    CComPtr<ID3D11RasterizerState> spOldRasterizerState;
                    D3D11_RASTERIZER_DESC desc;
                    desc.FrontCounterClockwise = false;
                    desc.CullMode = D3D11_CULL_BACK;
                    desc.AntialiasedLineEnable = false;
                    desc.DepthBias = 0;
                    desc.DepthBiasClamp = 0.0f;
                    desc.DepthClipEnable = true;
                    desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
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
        uint32 vertexSize = pShader->GetShaderInfo()->GetVertexSize();
        uint32 numVertices = m_VB.m_numVertices;
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
    CRefObj<IRenderSubMesh> CRenderMesh::GetSubMesh(uint32 index)
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
    CRefObj<IMaterialAttrib> CRenderMesh::GetMaterial(uint32 materialID)
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
        std::vector<uint32> faces;
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
}
