//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\Shader.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Sampler.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Cameras\CameraBase\ICamera.h"
#include <memory>
#include <Windows.h>
#include "GPUPipelineImpl.h"
#include <any>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>
#include <varargs.h>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Method: CreateSourceNode
    // See <IGPUPipeline::CreateSourceNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSourceNode> CGPUPipeline::CreateSourceNode(const wchar_t* pName, IImage *pImage, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineSourceNode> spSource(new CGPUPipelineSourceNode(outputWidth, outputHeight, format));
        spSource->SetSource(this, pImage);
        spSource->SetName(pName);
        m_sourceNodes.push_back(CRefObj<IGPUPipelineSourceNode>(spSource.get()));
        return CRefObj<IGPUPipelineSourceNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreateSinkNode
    // See <IGPUPipeline::CreateSinkNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSinkNode> CGPUPipeline::CreateSinkNode(const wchar_t *pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineSinkNode> spSource(new CGPUPipelineSinkNode(outputWidth, outputHeight, format));
        spSource->SetName(pName);
        spSource->SetShader(pShader);
        m_sinkNodes.push_back(CRefObj<IGPUPipelineSinkNode>(spSource.get()));
        return CRefObj<IGPUPipelineSinkNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreatePredefinedNode
    // See <IGPUPipeline::CreatePredefinedNode>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipeline::CreatePredefinedNode(const wchar_t* pName, ...)
    {
        if (wcscmp(pName, c_CustomNode_DepthMeshNode) == 0)
        {
            va_list argptr;
            va_start(argptr, pName);
            uint32 depthWidth = (uint32)va_arg(argptr, unsigned int);
            uint32 depthHeight = (uint32)va_arg(argptr, unsigned int);
            uint32 colorWidth = (uint32)va_arg(argptr, unsigned int);
            uint32 colorHeight = (uint32)va_arg(argptr, unsigned int);
            CRefObj<ITexture> spRayTex = va_arg(argptr, CRefObj<ITexture>);
            Matrix4x4 extrinsics = va_arg(argptr, Matrix4x4);
            CameraIntrinsics intrinsics = va_arg(argptr, CameraIntrinsics);
            float minDepth = (float)va_arg(argptr, double);
            float maxDepth = (float)va_arg(argptr, double);
            DXGI_FORMAT outputFormat = va_arg(argptr, DXGI_FORMAT);
            const wchar_t* pShaderName = va_arg(argptr, const wchar_t*);
            va_end(argptr);

            auto spShader = m_spRenderer->GetShaderMgr()->FindShader(pShaderName);
            std::unique_ptr<CGPUPipelineDepthMeshNode> spSource(
                new CGPUPipelineDepthMeshNode(m_spRenderer, spShader, depthWidth, depthHeight,
                    colorWidth, colorHeight, spRayTex, extrinsics, intrinsics, minDepth, maxDepth,
                    outputFormat));
            m_nodes.push_back(CRefObj<IGPUPipelineNode>(spSource.get()));
            return CRefObj<IGPUPipelineNode>(spSource.release());
        }
        if (wcscmp(pName, c_CustomNode_Color2Depth) == 0)
        {
            va_list argptr;
            va_start(argptr, pName);
            uint32 depthWidth = (uint32)va_arg(argptr, unsigned int);
            uint32 depthHeight = (uint32)va_arg(argptr, unsigned int);
            uint32 colorWidth = (uint32)va_arg(argptr, unsigned int);
            uint32 colorHeight = (uint32)va_arg(argptr, unsigned int);
            CRefObj<ITexture> spRayTex = va_arg(argptr, CRefObj<ITexture>);
            Matrix4x4 extrinsics = va_arg(argptr, Matrix4x4);
            CameraIntrinsics intrinsics = va_arg(argptr, CameraIntrinsics);
            float minDepth = (float)va_arg(argptr, double);
            float maxDepth = (float)va_arg(argptr, double);
            DXGI_FORMAT outputFormat = va_arg(argptr, DXGI_FORMAT);
            const wchar_t* pShaderName = va_arg(argptr, const wchar_t*);
            va_end(argptr);

            auto spShader = m_spRenderer->GetShaderMgr()->FindShader(pShaderName);
            std::unique_ptr<CGPUPipelineDepthMeshNode> spSource(
                new CGPUPipelineDepthMeshNode(m_spRenderer, spShader, depthWidth, depthHeight,
                    colorWidth, colorHeight, spRayTex, extrinsics, intrinsics, minDepth, maxDepth,
                    outputFormat));
            m_nodes.push_back(CRefObj<IGPUPipelineNode>(spSource.get()));
            return CRefObj<IGPUPipelineNode>(spSource.release());
        }
        CT(E_INVALIDARG);
        return CRefObj<IGPUPipelineNode>(nullptr);
    }

    //**********************************************************************
    // Method: CreateNode
    // See <IGPUPipeline::CreateNode>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipeline::CreateNode(const wchar_t* pName, IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineNode> spSource(new CGPUPipelineNode(outputWidth, outputHeight, format));
        spSource->SetName(pName);
        spSource->SetShader(pShader);
        m_nodes.push_back(CRefObj<IGPUPipelineNode>(spSource.get()));
        return CRefObj<IGPUPipelineNode>(spSource.release());
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipeline::Process>
    //**********************************************************************
    void CGPUPipeline::Process(IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        for (CRefObj<IGPUPipelineSinkNode> &pCurNode : m_sinkNodes)
        { 
            if (pCurNode->IsEnabled())
                pCurNode->Process(this, pRenderer, pRenderCtx);
        }
    }
    
    //**********************************************************************
    // Method: GetRenderer
    // See <IGPUPipeline::GetRenderer>
    //**********************************************************************
    CRefObj<IRenderer> CGPUPipeline::GetRenderer()
    {
        return m_spRenderer;
    }

    //**********************************************************************
    // Struct: GPUVertex
    // GPU vertex used for the fullscreen quad.
    //**********************************************************************
    struct GPUVertex
    {
        float pts[3];
        float uvs[2];
    };

    //**********************************************************************
    void CGPUPipeline::Initialize(IRenderer *pRenderer)
    {
        if (pRenderer == nullptr)
			pRenderer = Caustic::CCausticFactory::Instance()->CreateRenderer(nullptr, std::wstring(L""));
        m_spRenderer = pRenderer;

        // Make sure the shaders required are loaded
        std::wstring path = GetCausticShaderDirectory();
        m_spRenderer->LoadShaders(path.c_str());

        // Setup vertex buffer
        CD3D11_BUFFER_DESC vbdesc(sizeof(GPUVertex) * 4, D3D11_BIND_VERTEX_BUFFER);
        GPUVertex quadPts[4] = {
            { -1.0f, -1.0f, 0.9f, 0.0f, 1.0f },
            { -1.0f, +1.0f, 0.9f, 0.0f, 0.0f, },
            { +1.0f, +1.0f, 0.9f, 1.0f, 0.0f, },
            { +1.0f, -1.0f, 0.9f, 1.0f, 1.0f },
        };
        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = quadPts;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CComPtr<ID3D11Device> spDevice = m_spRenderer->GetDevice();
        CT(spDevice->CreateBuffer(&vbdesc, &data, &m_spFullQuadVB));

        //**********************************************************************
        // Create index buffer used to draw full quad
        //**********************************************************************
        CD3D11_BUFFER_DESC ibdesc(sizeof(uint32) * 6, D3D11_BIND_INDEX_BUFFER);
        uint32 quadIndices[2][3] = {
            { 0, 2, 1 },
            { 0, 3, 2 },
        };
        data.pSysMem = quadIndices;
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;
        CT(spDevice->CreateBuffer(&ibdesc, &data, &m_spFullQuadIB));
    }

    //**********************************************************************
    // Function: CreateCPUPipeline
    // Allocates a new GPUPipeline object
    //
    // Parameters:
    // pRenderer - renderer to use
    //**********************************************************************
    CRefObj<IGPUPipeline> CreateGPUPipeline(IRenderer *pRenderer)
    {
        std::unique_ptr<CGPUPipeline> spPipeline(new CGPUPipeline());
        spPipeline->Initialize(pRenderer);
        return CRefObj<IGPUPipeline>(spPipeline.release());
    }

    //**********************************************************************
    // Method: SetShader
    // See <IGPUPipelineNode::SetShader>
    //**********************************************************************
    void CGPUPipelineNodeBase::SetShader(IShader *pShader)
    {
        m_spShader = pShader;
    }

    //**********************************************************************
    // Method: GetShader
    // See <IGPUPipelineNode::GetShader>
    //**********************************************************************
    CRefObj<IShader> CGPUPipelineNodeBase::GetShader()
    {
        return CRefObj<IShader>(m_spShader);
    }
    
    //**********************************************************************
    // Method: GetInput
    // See <IGPUPipelineNode::GetInput>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipelineNodeBase::GetInput(const wchar_t* pName)
    {
        return CRefObj<IGPUPipelineNode>(m_sourceNodes[pName].first);
    }

    //**********************************************************************
    // Method: SetInput
    // See <IGPUPipelineNode::SetInput>
    //**********************************************************************
    void CGPUPipelineNodeBase::SetInput(const wchar_t* pName, const wchar_t* pSamplerName, IGPUPipelineNode *pNode)
    {
        m_sourceNodes[pName] = std::make_pair(pNode, std::wstring((pSamplerName) ? pSamplerName : L""));
    }

    //**********************************************************************
    // Method: GetOutputTexture
    // See <IGPUPipelineNode::GetOutputTexture>
    //**********************************************************************
    CRefObj<ITexture> CGPUPipelineNodeBase::GetOutputTexture(IGPUPipeline *pPipeline)
    {
        return m_spOutputTexture;
    }

    //**********************************************************************
    // Method: RenderQuad
    // See <IGPUPipelineNode::RenderQuad>
    //**********************************************************************
    void CGPUPipeline::RenderQuad(IShader *pShader)
    {
        UINT offset = 0;
        UINT vertexSize = sizeof(GPUVertex);

        CComPtr<ID3D11Device> spDevice = m_spRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = m_spRenderer->GetContext();
        spCtx->IASetVertexBuffers(0, 1, &m_spFullQuadVB.p, &vertexSize, &offset);
        spCtx->IASetIndexBuffer(m_spFullQuadIB, DXGI_FORMAT_R32_UINT, 0);
        std::vector<CRefObj<ILight>> lights;
        pShader->BeginRender(m_spRenderer, nullptr, lights, nullptr);
        spCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        spCtx->DrawIndexed(6, 0, 0);
        pShader->EndRender(m_spRenderer);
    }

    void CGPUPipelineNodeBase::ProcessInternal(IGPUPipeline* pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx,
        std::function<void()> customRender)
    {
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();

        if (m_epoch == pPipeline->GetCurrentEpoch())
            return;
        m_epoch = pPipeline->GetCurrentEpoch();

#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(spCtx->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        wchar_t buf[1024];
        swprintf_s(buf, L"%d", rand());
        std::wstring s(buf);
        spCtx2->BeginEventInt((std::wstring(L"GPUPipeline:") + m_spShader->Name() + L"-" + s).c_str(), 0);
#endif
        CComPtr<ID3D11RasterizerState> spOldRasterState;
        spCtx->RSGetState(&spOldRasterState);

        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = D3D11_CULL_NONE;
        rasDesc.DepthClipEnable = TRUE;
        rasDesc.FillMode = D3D11_FILL_SOLID;
        spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        spCtx->RSSetState(spRasterState);

        if (m_spOutputTexture == nullptr)
            m_spOutputTexture = CCausticFactory::Instance()->CreateTexture(pRenderer, m_width, m_height, m_outputFormat, m_cpuFlags, m_bindFlags);

        if (!IsEnabled())
            return;

        // Get the input textures from the earlier nodes in the pipeline
        std::vector<CRefObj<ITexture>> textures;
        for (auto spSourceNode : m_sourceNodes)
        {
            spSourceNode.second.first->Process(pPipeline, pRenderer, pRenderCtx);
            CRefObj<ITexture> spTexture = spSourceNode.second.first->GetOutputTexture(pPipeline);
            textures.push_back(spTexture);
        }

        if (m_spShader)
        {
            // Set the input from earlier nodes as our source textures in our shader
            int i = 0;
            for (auto spSourceNode : m_sourceNodes)
            {
                CRefObj<ITexture> spTexture = textures[i++];
                if (spTexture)
                {
                    m_spShader->SetPSParam(spSourceNode.first, std::any(spTexture));
                    m_spShader->SetVSParam(spSourceNode.first, std::any(spTexture));
                    std::wstring wName = std::wstring(L"_") + spSourceNode.first + std::wstring(L"Width");
                    m_spShader->SetPSParam(wName, std::any(float(spTexture->GetWidth())));
                    m_spShader->SetVSParam(wName, std::any(float(spTexture->GetWidth())));
                    std::wstring hName = std::wstring(L"_") + spSourceNode.first + std::wstring(L"Height");
                    m_spShader->SetPSParam(hName, std::any(float(spTexture->GetHeight())));
                    m_spShader->SetVSParam(hName, std::any(float(spTexture->GetHeight())));
                    if (spSourceNode.second.second.length() > 0)
                    {
                        CRefObj<ISampler> spSampler = Caustic::CCausticFactory::Instance()->CreateSampler(pRenderer, spTexture);
                        spSampler->SetAddressU(pRenderer, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER);
                        spSampler->SetAddressV(pRenderer, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER);
                        m_spShader->SetVSParam(spSourceNode.second.second, std::any(CSamplerRef(spSampler)));
                        m_spShader->SetPSParam(spSourceNode.second.second, std::any(CSamplerRef(spSampler)));
                    }
                }
            }

            CComPtr<ID3D11RenderTargetView> spRTView;
            CT(spDevice->CreateRenderTargetView(m_spOutputTexture->GetD3DTexture(), nullptr, &spRTView));

            CComPtr<ID3D11RenderTargetView> spOldRTV;
            CComPtr<ID3D11DepthStencilView> spOldDSV;
            spCtx->OMGetRenderTargets(1, &spOldRTV, &spOldDSV);

            // Setup render target
            spCtx->OMSetRenderTargets(1, &spRTView.p, nullptr);

            UINT numViewports = 1;
            D3D11_VIEWPORT oldViewport;
            spCtx->RSGetViewports(&numViewports, &oldViewport);

            // Set viewport
            D3D11_VIEWPORT viewport;
            ZeroMemory(&viewport, sizeof(viewport));
            viewport.TopLeftX = 0;
            viewport.TopLeftY = 0;
            viewport.Width = (float)m_spOutputTexture->GetWidth();
            viewport.Height = (float)m_spOutputTexture->GetHeight();
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            spCtx->RSSetViewports(1, &viewport);

            FLOAT bgClr[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            spCtx->ClearRenderTargetView(spRTView, bgClr);

            std::any w(viewport.Width);
            m_spShader->SetPSParam(L"_imageWidth", w);
            m_spShader->SetVSParam(L"_imageWidth", w);
            std::any h(viewport.Height);
            m_spShader->SetPSParam(L"_imageHeight", h);
            m_spShader->SetVSParam(L"_imageHeight", h);

            // Draw full screen quad using shader
            if (customRender != nullptr)
                customRender();
            else
                pPipeline->RenderQuad(m_spShader);

            spCtx->RSSetViewports(numViewports, &oldViewport);
            spCtx->OMSetRenderTargets(1, &spOldRTV.p, spOldDSV);
        }
        spCtx->RSSetState(spOldRasterState);
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipelineNode::Process>
    //**********************************************************************
    void CGPUPipelineNodeBase::Process(IGPUPipeline *pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        ProcessInternal(pPipeline, pRenderer, pRenderCtx, nullptr);
    }

    //**********************************************************************
    CGPUPipelineDepthMeshNode::CGPUPipelineDepthMeshNode(
        IRenderer* pRenderer, IShader* pShader,
        uint32 depthInputWidth, uint32 depthInputHeight,
        uint32 outputColorWidth, uint32 outputColorHeight,
        CRefObj<ITexture> spRayTex, Matrix4x4& extrinsics, CameraIntrinsics& intrinsics,
        float minDepth, float maxDepth,
        DXGI_FORMAT format) :
        CGPUPipelineNodeBase(outputColorWidth, outputColorHeight, format),
        m_epoch(0)
    {
        CRefObj<ICausticFactory> spFactory = Caustic::CreateCausticFactory();
        SetShader(pShader);
        m_spMesh = Caustic::CreateDepthGridMesh(pRenderer, depthInputWidth, depthInputHeight, pShader);
        m_cpuFlags = (D3D11_CPU_ACCESS_FLAG)0;
        m_bindFlags = (D3D11_BIND_FLAG)(D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);

        pRenderer->Freeze();
        std::any raymap = std::any(spRayTex);
        pShader->SetVSParam(L"rayTexture", raymap);
        pShader->SetPSParam(L"rayTexture", raymap);
        std::any f1 = std::any(intrinsics.codx);
        std::any f2 = std::any(intrinsics.cody);
        std::any f3 = std::any(intrinsics.cx);
        std::any f4 = std::any(intrinsics.cy);
        std::any f5 = std::any(intrinsics.fx);
        std::any f6 = std::any(intrinsics.fy);
        std::any f7 = std::any(intrinsics.k1);
        std::any f8 = std::any(intrinsics.k2);
        std::any f9 = std::any(intrinsics.k3);
        std::any f10 = std::any(intrinsics.k4);
        std::any f11 = std::any(intrinsics.k5);
        std::any f12 = std::any(intrinsics.k6);
        std::any f13 = std::any(intrinsics.metricRadius);
        std::any f14 = std::any(intrinsics.p1);
        std::any f15 = std::any(intrinsics.p2);
        pShader->SetVSParam(L"codx", f1);
        pShader->SetVSParam(L"cody", f2);
        pShader->SetVSParam(L"cx", f3);
        pShader->SetVSParam(L"cy", f4);
        pShader->SetVSParam(L"fx", f5);
        pShader->SetVSParam(L"fy", f6);
        pShader->SetVSParam(L"k1", f7);
        pShader->SetVSParam(L"k2", f8);
        pShader->SetVSParam(L"k3", f9);
        pShader->SetVSParam(L"k4", f10);
        pShader->SetVSParam(L"k5", f11);
        pShader->SetVSParam(L"k6", f12);
        pShader->SetVSParam(L"metricRadius", f13);
        pShader->SetVSParam(L"p1", f14);
        pShader->SetVSParam(L"p2", f15);
        std::any v(Int(intrinsics.type));
        pShader->SetVSParam(L"type", v);
        Matrix m(extrinsics);
        std::any mat = std::any(m);
        pShader->SetVSParam(L"colorExt", mat);
        std::any dw(Int((int)depthInputWidth));
        pShader->SetVSParam(L"depthWidth", dw);
        std::any dh(Int((int)depthInputHeight));
        pShader->SetVSParam(L"depthHeight", dh);
        std::any cw(Int((int)outputColorWidth));
        pShader->SetVSParam(L"colorWidth", cw);
        std::any ch(Int((int)outputColorHeight));
        pShader->SetVSParam(L"colorHeight", ch);
        std::any mind(minDepth);
        pShader->SetVSParam(L"minDepth", mind);
        pShader->SetPSParam(L"minDepth", mind);
        std::any maxd(maxDepth);
        pShader->SetVSParam(L"maxDepth", maxd);
        pShader->SetPSParam(L"maxDepth", maxd);
        pRenderer->Unfreeze();
    }

    //**********************************************************************
    // Method: Process
    // See <CGPUPipelineDepthMeshNode::Process>
    //**********************************************************************
    void CGPUPipelineDepthMeshNode::Process(IGPUPipeline* pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        ProcessInternal(pPipeline, pRenderer, pRenderCtx,
            [&]() {
                DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
                std::vector<CRefObj<ILight>> lights;
                m_spMesh->Render(pRenderer, pRenderCtx, m_spShader, nullptr, lights, nullptr);
            });
    }
    
    //**********************************************************************
    // Method: SetSource
    // See <IGPUPipelineSourceNode::SetSource>
    //**********************************************************************
    void CGPUPipelineSourceNode::SetSource(IGPUPipeline *pPipeline, IImage *pSource)
    {
        if (pSource)
        {
            CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
            if (m_spSourceImage[0] == nullptr)
            {
                DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
                switch (pSource->GetBPP())
                {
                case 1: format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM; break;
                case 8: format = DXGI_FORMAT::DXGI_FORMAT_R8_UNORM; break;
                case 16: format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT; break;
                case 24: format = (pSource->GetRGBOrder()) ? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM; break;
                case 32: format = (pSource->GetRGBOrder()) ? DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM; break;
                case 128: format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT; break;
                }
                for (int i = 0; i < c_NumberBackBuffers; i++)
                    m_spSourceImage[i] = CreateTexture(spRenderer, pSource->GetWidth(), pSource->GetHeight(), format, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
            }
            m_index = (m_index == c_NumberBackBuffers - 1) ? 0 : m_index + 1;
            m_spSourceImage[m_index]->CopyFromImage(spRenderer, pSource);
        }
    }

    //**********************************************************************
    // Method: GetOutputTexture
    // See <IGPUPipelineSourceNode::GetOutputTexture>
    //**********************************************************************
    CRefObj<ITexture> CGPUPipelineSourceNode::GetOutputTexture(IGPUPipeline *pPipeline)
    {
        return m_spSourceImage[m_index];
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipelineNode::Process>
    //**********************************************************************
    void CGPUPipelineSinkNode::Process(IGPUPipeline *pPipeline, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        CComPtr<ID3D11DeviceContext> spCtx = pRenderer->GetContext();
        assert(m_outputFormat == DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM || m_outputFormat == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
        if (m_spOutputStagedTexture == nullptr)
            m_spOutputStagedTexture = Caustic::CCausticFactory::Instance()->CreateTexture(pRenderer, m_width, m_height, m_outputFormat, D3D11_CPU_ACCESS_READ, (D3D11_BIND_FLAG)0);
        
        // First run the sink nodes shader
        CGPUPipelineNodeBase::Process(pPipeline, pRenderer, pRenderCtx);

        spCtx->CopyResource(m_spOutputStagedTexture->GetD3DTexture(), m_spOutputTexture->GetD3DTexture());
    }

    //**********************************************************************
    // Method: GetOutput
    // See <IGPUPipelineSinkNode::GetOutput>
    //**********************************************************************
    CRefObj<IImage> CGPUPipelineSinkNode::GetOutput(IGPUPipeline *pPipeline)
    {
        CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
        CComPtr<ID3D11Device> spDevice = spRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spRenderer->GetContext();
        spCtx->Flush();
        CRefObj<IImage> spImage = CreateImage(m_width, m_height, 32);
        D3D11_MAPPED_SUBRESOURCE mapinfo;
        CComPtr<ID3D11Texture2D> spTexture = m_spOutputStagedTexture->GetD3DTexture();
        spCtx->Map(spTexture, 0, D3D11_MAP::D3D11_MAP_READ, 0, &mapinfo);
        BYTE *pSrcRow = (BYTE*)mapinfo.pData;
        BYTE *pDstRow = spImage->GetData();
        for (uint32 y = 0; y < spImage->GetHeight(); y++)
        {
            BYTE *pSrcCol = pSrcRow;
            BYTE *pDstCol = pDstRow;
            memcpy(pDstCol, pSrcCol, spImage->GetWidth() * 4);
            pSrcRow += mapinfo.RowPitch;
            pDstRow += spImage->GetStride();
        }
        spCtx->Unmap(spTexture, 0);
        return CRefObj<IImage>(spImage.Detach());
    }
}
