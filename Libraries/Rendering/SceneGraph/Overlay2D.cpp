//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <any>
#include <d3d11.h>
#include <atlbase.h>

module Rendering.SceneGraph.SceneOverlay2DElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Sampler;
import Rendering.Caustic.IShader;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ICausticFactory;
import Rendering.SceneGraph.SceneFactory;

namespace Caustic
{
    struct OverlayVertex
    {
        float pts[4];
        float uvs[2];
    };

    void CSceneOverlay2DElem::CreateQuad(IRenderer* pRenderer)
    {
        CD3D11_BUFFER_DESC vbdesc(sizeof(OverlayVertex) * 4, D3D11_BIND_VERTEX_BUFFER);
        OverlayVertex quadPts[4] = {
            { 0.0f, 0.0f, 0.9f, 1.0f, 0.0f, 0.0f },
            { 0.0f, +1.0f, 0.9f, 1.0f, 0.0f, 1.0f, },
            { +1.0f, +1.0f, 0.9f, 1.0f, 1.0f, 1.0f, },
            { +1.0f, 0.0f, 0.9f, 1.0f, 1.0f, 0.0f },
        };
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = quadPts;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CT(spDevice->CreateBuffer(&vbdesc, &data, &m_spQuadVB));

        //**********************************************************************
        // Create index buffer used to draw full quad
        //**********************************************************************
        CD3D11_BUFFER_DESC ibdesc(sizeof(uint32_t) * 6, D3D11_BIND_INDEX_BUFFER);
        uint32_t quadIndices[2][3] = {
            { 0, 2, 1 },
            { 0, 3, 2 },
        };
        data.pSysMem = quadIndices;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(spDevice->CreateBuffer(&ibdesc, &data, &m_spQuadIB));

        if (m_spShader == nullptr)
            m_spShader = pRenderer->GetShaderMgr()->FindShader(L"Overlay2D");
        auto spCausticFactory = Caustic::CreateCausticFactory();
        m_spSampler = spCausticFactory->CreateSampler(pRenderer, m_spTexture);
    }
    
    void CSceneOverlay2DElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
        
        if (!m_initialized)
        {
            m_initialized = true;
            CreateQuad(pRenderer);
        }

        Float2 scale(m_bounds.maxPt.x - m_bounds.minPt.x, m_bounds.maxPt.y - m_bounds.minPt.y);
        Float2 trans(m_bounds.minPt.x, m_bounds.minPt.y);
        m_spShader->SetVSParam(L"scale", std::any(scale));
        m_spShader->SetVSParam(L"offset", std::any(trans));
        m_spShader->SetPSParam(L"diffuseSampler", std::any(CSamplerRef(m_spSampler)));
        m_spShader->SetPSParam(L"diffuseTexture", std::any(m_spTexture));

        UINT offset = 0;
        UINT vertexSize = sizeof(OverlayVertex);
        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = D3D11_CULL_NONE;
        rasDesc.DepthClipEnable = TRUE;
        rasDesc.FillMode = D3D11_FILL_SOLID;
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        spCtx->RSSetState(spRasterState);
        spCtx->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &vertexSize, &offset);
        spCtx->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
        std::vector<CRefObj<ILight>> lights;
        m_spShader->BeginRender(pRenderer, nullptr, lights, nullptr);
        spCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        spCtx->DrawIndexed(6, 0, 0);
        m_spShader->EndRender(pRenderer);
    }
    
    void CSceneOverlay2DElem::SetTexture(ITexture *pTexture)
    {
        m_spTexture = pTexture;
    }
};
