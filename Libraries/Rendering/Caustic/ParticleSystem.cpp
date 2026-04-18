//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <DirectXMath.h>
#include <vector>
#include <cinttypes>

module Rendering.Caustic.ParticleSystem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IParticleSystem;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ILight;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.Shader;

namespace Caustic
{
    struct BillboardVertex
    {
        float x, y; // [-0.5, +0.5] corner offsets
    };

    CParticleSystem::CParticleSystem(uint32_t maxParticles)
        : m_maxParticles(maxParticles),
        m_frameCounter(0),
        m_emitPos(0.0f, 0.0f, 0.0f),
        m_emissionRate(100.0f),
        m_emitAccumulator(0.0f),
        m_particleLifetime(3.0f),
        m_initialSpeed(5.0f),
        m_emitSpread(0.5f),
        m_particleSize(0.1f),
        m_particleColor(1.0f, 1.0f, 1.0f, 1.0f),
        m_gravity(0.0f, -9.8f, 0.0f),
        m_wind(0.0f, 0.0f, 0.0f),
        m_initialized(false)
    {
    }

    void CParticleSystem::InitGPUResources(IRenderer* pRenderer)
    {
        if (m_initialized)
            return;

        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();

        // Create particle structured buffer with both UAV and SRV
        {
            uint32_t stride = sizeof(GPUParticle);
            uint32_t bufSize = m_maxParticles * stride;
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = bufSize;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            desc.StructureByteStride = stride;

            // Initialize all particles as dead (age = -1)
            std::vector<GPUParticle> initData(m_maxParticles);
            for (auto& p : initData)
            {
                p.pos[0] = p.pos[1] = p.pos[2] = 0.0f;
                p.age = -1.0f;
                p.vel[0] = p.vel[1] = p.vel[2] = 0.0f;
                p.lifetime = 1.0f;
                p.size = 0.0f;
                p.pad = 0.0f;
                p.color[0] = p.color[1] = p.color[2] = p.color[3] = 0.0f;
            }
            D3D11_SUBRESOURCE_DATA srd = {};
            srd.pSysMem = initData.data();
            CT(spDevice->CreateBuffer(&desc, &srd, &m_spParticleBuffer));

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Format = DXGI_FORMAT_UNKNOWN;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.NumElements = m_maxParticles;
            CT(spDevice->CreateUnorderedAccessView(m_spParticleBuffer, &uavDesc, &m_spParticleUAV));

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = DXGI_FORMAT_UNKNOWN;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            srvDesc.Buffer.NumElements = m_maxParticles;
            CT(spDevice->CreateShaderResourceView(m_spParticleBuffer, &srvDesc, &m_spParticleSRV));
        }

        // Create emit counter buffer (RWStructuredBuffer<uint> with 1 element)
        {
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = sizeof(uint32_t);
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            desc.StructureByteStride = sizeof(uint32_t);
            CT(spDevice->CreateBuffer(&desc, nullptr, &m_spEmitCounterBuffer));

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Format = DXGI_FORMAT_UNKNOWN;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.NumElements = 1;
            CT(spDevice->CreateUnorderedAccessView(m_spEmitCounterBuffer, &uavDesc, &m_spEmitCounterUAV));
        }

        // Create billboard quad VB (4 corner vertices)
        {
            BillboardVertex verts[4] = {
                { -0.5f, -0.5f },
                { -0.5f, +0.5f },
                { +0.5f, +0.5f },
                { +0.5f, -0.5f }
            };
            CD3D11_BUFFER_DESC vbDesc(sizeof(verts), D3D11_BIND_VERTEX_BUFFER);
            D3D11_SUBRESOURCE_DATA vbData = { verts, 0, 0 };
            CT(spDevice->CreateBuffer(&vbDesc, &vbData, &m_spQuadVB));
        }

        // Create billboard quad IB (2 triangles)
        {
            uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
            CD3D11_BUFFER_DESC ibDesc(sizeof(indices), D3D11_BIND_INDEX_BUFFER);
            D3D11_SUBRESOURCE_DATA ibData = { indices, 0, 0 };
            CT(spDevice->CreateBuffer(&ibDesc, &ibData, &m_spQuadIB));
        }

        // Create additive blend state (SRC_ALPHA / ONE) for particle rendering
        {
            D3D11_BLEND_DESC blendDesc = {};
            blendDesc.RenderTarget[0].BlendEnable = TRUE;
            blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            CT(spDevice->CreateBlendState(&blendDesc, &m_spAdditiveBlend));
        }

        // Create depth-stencil state: depth test ON, depth write OFF
        {
            D3D11_DEPTH_STENCIL_DESC dsDesc = {};
            dsDesc.DepthEnable = TRUE;
            dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
            dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
            dsDesc.StencilEnable = FALSE;
            CT(spDevice->CreateDepthStencilState(&dsDesc, &m_spDepthNoWrite));
        }

        // Find shaders
        auto spShaderMgr = pRenderer->GetShaderMgr();
        try { m_spSimulateShader = spShaderMgr->FindShader(L"ParticleSimulate"); } catch (...) {}
        try { m_spBillboardShader = spShaderMgr->FindShader(L"ParticleBillboard"); } catch (...) {}

        m_initialized = true;
    }

    void CParticleSystem::Update(IRenderer* pRenderer, float deltaTime)
    {
        InitGPUResources(pRenderer);
        if (m_spSimulateShader == nullptr)
            return;

        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();

        // Compute how many particles to emit this frame
        m_emitAccumulator += m_emissionRate * deltaTime;
        uint32_t emitCount = (uint32_t)m_emitAccumulator;
        m_emitAccumulator -= (float)emitCount;

        // Upload emit count to the counter buffer
        spCtx->UpdateSubresource(m_spEmitCounterBuffer, 0, nullptr, &emitCount, 0, 0);

        // Bind particle buffer and emit counter as UAVs before engine dispatch.
        // Only scalar CS params are set (no buffer params), so the engine's
        // PushBuffers/PopBuffers won't touch these UAV slots.
        ID3D11UnorderedAccessView* uavs[2] = { m_spParticleUAV, m_spEmitCounterUAV };
        UINT initCounts[2] = { (UINT)-1, (UINT)-1 };
        spCtx->CSSetUnorderedAccessViews(0, 2, uavs, initCounts);

        // Set compute constants (scalars only — no buffer params)
        m_spSimulateShader->SetCSParamFloat(L"deltaTime", deltaTime);
        m_spSimulateShader->SetCSParamFloat(L"gravityX", m_gravity.x);
        m_spSimulateShader->SetCSParamFloat(L"gravityY", m_gravity.y);
        m_spSimulateShader->SetCSParamFloat(L"gravityZ", m_gravity.z);
        m_spSimulateShader->SetCSParamFloat(L"windX", m_wind.x);
        m_spSimulateShader->SetCSParamFloat(L"windY", m_wind.y);
        m_spSimulateShader->SetCSParamFloat(L"windZ", m_wind.z);
        m_spSimulateShader->SetCSParamInt(L"maxParticles", (int)m_maxParticles);
        m_spSimulateShader->SetCSParamInt(L"emitCount", (int)emitCount);
        m_spSimulateShader->SetCSParamFloat(L"emitPosX", m_emitPos.x);
        m_spSimulateShader->SetCSParamFloat(L"emitPosY", m_emitPos.y);
        m_spSimulateShader->SetCSParamFloat(L"emitPosZ", m_emitPos.z);
        m_spSimulateShader->SetCSParamFloat(L"emitSpread", m_emitSpread);
        m_spSimulateShader->SetCSParamFloat(L"emitSpeed", m_initialSpeed);
        m_spSimulateShader->SetCSParamFloat(L"emitLifetime", m_particleLifetime);
        m_spSimulateShader->SetCSParamFloat(L"emitSize", m_particleSize);
        m_spSimulateShader->SetCSParamFloat(L"emitColorR", m_particleColor.x);
        m_spSimulateShader->SetCSParamFloat(L"emitColorG", m_particleColor.y);
        m_spSimulateShader->SetCSParamFloat(L"emitColorB", m_particleColor.z);
        m_spSimulateShader->SetCSParamFloat(L"emitColorA", m_particleColor.w);
        m_spSimulateShader->SetCSParamInt(L"frameIndex", (int)m_frameCounter++);

        // Dispatch via engine: BeginRender sets CS, pushes constants, dispatches,
        // then pops (no-op since no buffer params). UAVs bound above survive.
        uint32_t numGroups = (m_maxParticles + 255) / 256;
        m_spSimulateShader->Dispatch(pRenderer, (int)numGroups, 1, 1);

        // Unbind UAVs
        ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr, nullptr };
        spCtx->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);
    }

    void CParticleSystem::Render(IRenderer* pRenderer)
    {
        InitGPUResources(pRenderer);
        if (m_spBillboardShader == nullptr)
            return;

        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();

        // Save current blend and depth-stencil state
        CComPtr<ID3D11BlendState> spOldBlend;
        float oldBlendFactor[4];
        UINT oldSampleMask;
        spCtx->OMGetBlendState(&spOldBlend, oldBlendFactor, &oldSampleMask);
        CComPtr<ID3D11DepthStencilState> spOldDS;
        UINT oldStencilRef;
        spCtx->OMGetDepthStencilState(&spOldDS, &oldStencilRef);

        // Set additive blend + depth no-write
        spCtx->OMSetBlendState(m_spAdditiveBlend, nullptr, 0xffffffff);
        spCtx->OMSetDepthStencilState(m_spDepthNoWrite, 0);

        // Bind particle SRV to VS slot 0 (manual, since engine only supports PS/CS)
        spCtx->VSSetShaderResources(0, 1, &m_spParticleSRV.p);

        // Set billboard shader params
        DirectX::XMFLOAT4X4 f4x4;
        DirectX::XMStoreFloat4x4(&f4x4, pRenderer->GetCamera()->GetView());
        Matrix viewMat(reinterpret_cast<float*>(&f4x4));
        DirectX::XMStoreFloat4x4(&f4x4, pRenderer->GetCamera()->GetProjection());
        Matrix projMat(reinterpret_cast<float*>(&f4x4));

        // Extract camera right and up from view matrix for billboarding
        DirectX::XMMATRIX xmView = pRenderer->GetCamera()->GetView();
        DirectX::XMFLOAT4X4 viewF;
        DirectX::XMStoreFloat4x4(&viewF, xmView);
        Float3 camRight(viewF._11, viewF._21, viewF._31);
        Float3 camUp(viewF._12, viewF._22, viewF._32);

        m_spBillboardShader->SetVSParam(L"viewMatrix", std::any(viewMat));
        m_spBillboardShader->SetVSParam(L"projMatrix", std::any(projMat));
        m_spBillboardShader->SetVSParam(L"camRight", std::any(camRight));
        m_spBillboardShader->SetVSParam(L"camUp", std::any(camUp));

        // Bind quad VB/IB
        UINT stride = sizeof(BillboardVertex);
        UINT offset = 0;
        spCtx->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &stride, &offset);
        spCtx->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
        spCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Draw all particle slots; dead particles output degenerate quads in VS
        std::vector<CRefObj<ILight>> emptyLights;
        m_spBillboardShader->BeginRender(pRenderer, nullptr, emptyLights, nullptr);
        spCtx->DrawIndexedInstanced(6, m_maxParticles, 0, 0, 0);
        m_spBillboardShader->EndRender(pRenderer);

        // Unbind VS SRV
        ID3D11ShaderResourceView* nullSRV = nullptr;
        spCtx->VSSetShaderResources(0, 1, &nullSRV);

        // Restore state
        spCtx->OMSetBlendState(spOldBlend, oldBlendFactor, oldSampleMask);
        spCtx->OMSetDepthStencilState(spOldDS, oldStencilRef);
    }

    CRefObj<IParticleSystem> CreateParticleSystem(IRenderer* pRenderer, uint32_t maxParticles)
    {
        return CRefObj<IParticleSystem>(new CParticleSystem(maxParticles));
    }
}
