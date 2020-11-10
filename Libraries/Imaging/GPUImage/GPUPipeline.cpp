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
#include <memory>
#include <Windows.h>
#include "GPUPipelineImpl.h"
#include <any>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Method: CreateSourceNode
    // See <IGPUPipeline::CreateSourceNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSourceNode> CGPUPipeline::CreateSourceNode(IImage *pImage, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineSourceNode> spSource(new CGPUPipelineSourceNode(outputWidth, outputHeight, format));
        if (pImage)
            spSource->SetSource(this, pImage);
        m_sourceNodes.push_back(CRefObj<IGPUPipelineSourceNode>(spSource.get()));
        return CRefObj<IGPUPipelineSourceNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreateSinkNode
    // See <IGPUPipeline::CreateSinkNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSinkNode> CGPUPipeline::CreateSinkNode(IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineSinkNode> spSource(new CGPUPipelineSinkNode(outputWidth, outputHeight, format));
        m_sinkNodes.push_back(CRefObj<IGPUPipelineSinkNode>(spSource.get()));
        return CRefObj<IGPUPipelineSinkNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreateNode
    // See <IGPUPipeline::CreateNode>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipeline::CreateNode(IShader *pShader, uint32 outputWidth, uint32 outputHeight, DXGI_FORMAT format)
    {
        std::unique_ptr<CGPUPipelineNode> spSource(new CGPUPipelineNode(outputWidth, outputHeight, format));
        spSource->SetShader(pShader);
        m_nodes.push_back(CRefObj<IGPUPipelineNode>(spSource.get()));
        return CRefObj<IGPUPipelineNode>(spSource.release());
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipeline::Process>
    //**********************************************************************
    void CGPUPipeline::Process()
    {
        for (CRefObj<IGPUPipelineSinkNode> &pCurNode : m_sinkNodes)
        {
            pCurNode->GetOutputTexture(this);
        }
    }
    
    //**********************************************************************
    // Method: Process
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
        Process(pPipeline);
        return m_spOutputTexture;
    }

    //**********************************************************************
    // Method: GetOutputTexture
    // See <IGPUPipelineNode::GetOutputTexture>
    //**********************************************************************
    void CGPUPipeline::RenderQuad(IShader *pShader)
    {
        UINT offset = 0;
        UINT vertexSize = sizeof(GPUVertex);
        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = D3D11_CULL_NONE;
        rasDesc.DepthClipEnable = TRUE;
        rasDesc.FillMode = D3D11_FILL_SOLID;
        CComPtr<ID3D11Device> spDevice = m_spRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = m_spRenderer->GetContext();
        spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        spCtx->RSSetState(spRasterState);
        spCtx->IASetVertexBuffers(0, 1, &m_spFullQuadVB.p, &vertexSize, &offset);
        spCtx->IASetIndexBuffer(m_spFullQuadIB, DXGI_FORMAT_R32_UINT, 0);
        std::vector<CRefObj<ILight>> lights;
        pShader->BeginRender(m_spRenderer, nullptr, nullptr, lights, nullptr);
        spCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        spCtx->DrawIndexed(6, 0, 0);
        pShader->EndRender(m_spRenderer);
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipelineNode::Process>
    //**********************************************************************
    void CGPUPipelineNodeBase::Process(IGPUPipeline *pPipeline)
    {
        CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
        CComPtr<ID3D11Device> spDevice = spRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spRenderer->GetContext();

#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(spCtx->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
#endif
#ifdef _DEBUG
        wchar_t buf[1024];
        swprintf_s(buf, L"%d", rand());
        std::wstring s(buf);
        spCtx2->BeginEventInt((std::wstring(L"GPUPipeline:")+m_spShader->Name()+L"-"+s).c_str(), 0);
#endif

        if (m_spOutputTexture == nullptr)
            m_spOutputTexture = CCausticFactory::Instance()->CreateTexture(spRenderer, m_width, m_height, m_outputFormat, m_cpuFlags, m_bindFlags);
        
        // Get the input textures from the earlier nodes in the pipeline
        std::vector<CRefObj<ITexture>> textures;
        for (auto spSourceNode : m_sourceNodes)
        {
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
                m_spShader->SetPSParam(spSourceNode.first, std::any(spTexture));
                if (spSourceNode.second.second.length() > 0)
                {
                    CRefObj<ISampler> spSampler = Caustic::CCausticFactory::Instance()->CreateSampler(spRenderer, spTexture);
                    spSampler->SetAddressU(spRenderer, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER);
                    spSampler->SetAddressV(spRenderer, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER);
                    m_spShader->SetPSParam(spSourceNode.second.second, std::any(CSamplerRef(spSampler)));
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

            // Draw full screen quad using shader
            pPipeline->RenderQuad(m_spShader);

            spCtx->RSSetViewports(numViewports, &oldViewport);
            spCtx->OMSetRenderTargets(1, &spOldRTV.p, spOldDSV);
        }
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: SetSource
    // See <IGPUPipelineSourceNode::SetSource>
    //**********************************************************************
    void CGPUPipelineSourceNode::SetSource(IGPUPipeline *pPipeline, IImage *pSource)
    {
        CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
        m_spOutputTexture = Caustic::CreateTexture(spRenderer, pSource, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
    }

    //**********************************************************************
    // Method: GetOutputTexture
    // See <IGPUPipelineSourceNode::GetOutputTexture>
    //**********************************************************************
    CRefObj<ITexture> CGPUPipelineSourceNode::GetOutputTexture(IGPUPipeline *pPipeline)
    {
        return m_spOutputTexture;
    }

    //**********************************************************************
    // Method: Process
    // See <IGPUPipelineNode::Process>
    //**********************************************************************
    void CGPUPipelineSinkNode::Process(IGPUPipeline *pPipeline)
    {
        CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
        CComPtr<ID3D11Device> spDevice = spRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spRenderer->GetContext();
        CRefObj<ITexture> spTexture = m_sourceNodes.begin()->second.first->GetOutputTexture(pPipeline);
        if (m_width == 0 || m_height == 0)
        {
            m_width = spTexture->GetWidth();
            m_height = spTexture->GetHeight();
        }
        m_spOutputTexture = Caustic::CCausticFactory::Instance()->CreateTexture(spRenderer, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, m_cpuFlags, m_bindFlags);
        spCtx->CopyResource(m_spOutputTexture->GetD3DTexture(), spTexture->GetD3DTexture());
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
        CComPtr<ID3D11Texture2D> spTexture = m_spOutputTexture->GetD3DTexture();
        spCtx->Map(spTexture, 0, D3D11_MAP::D3D11_MAP_READ, 0, &mapinfo);
        BYTE *pSrcRow = (BYTE*)mapinfo.pData;
        BYTE *pDstRow = spImage->GetData();
        for (uint32 y = 0; y < spImage->GetHeight(); y++)
        {
            BYTE *pSrcCol = pSrcRow;
            BYTE *pDstCol = pDstRow;
            for (uint32 x = 0; x < spImage->GetHeight(); x++)
            {
                pDstCol[0] = pSrcCol[0];
                pDstCol[1] = pSrcCol[1];
                pDstCol[2] = pSrcCol[2];
                pDstCol[3] = pSrcCol[3];
                pSrcCol += 4;
                pDstCol += 4;
            }
            pSrcRow += mapinfo.RowPitch;
            pDstRow += spImage->GetStride();
        }
        spCtx->Unmap(spTexture, 0);
        return CRefObj<IImage>(spImage.Detach());
    }
}
