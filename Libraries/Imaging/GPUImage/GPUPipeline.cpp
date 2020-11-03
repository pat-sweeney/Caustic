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

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Variable: s_defaultVSLayout
    // s_defaultVSLayout defines the default layout for our default vertex
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_VSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    // Method: CreateSourceNode
    // See <IGPUPipeline::CreateSourceNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSourceNode> CGPUPipeline::CreateSourceNode(IImage *pImage)
    {
        std::unique_ptr<CGPUPipelineSourceNode> spSource(new CGPUPipelineSourceNode());
        spSource->SetSource(this, pImage);
        m_sourceNodes.push_back(CRefObj<IGPUPipelineSourceNode>(spSource.get()));
        return CRefObj<IGPUPipelineSourceNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreateSinkNode
    // See <IGPUPipeline::CreateSinkNode>
    //**********************************************************************
    CRefObj<IGPUPipelineSinkNode> CGPUPipeline::CreateSinkNode(uint32 numInputs, IShader *pShader)
    {
        std::unique_ptr<CGPUPipelineSinkNode> spSource(new CGPUPipelineSinkNode(numInputs));
        m_sinkNodes.push_back(CRefObj<IGPUPipelineSinkNode>(spSource.get()));
        return CRefObj<IGPUPipelineSinkNode>(spSource.release());
    }

    //**********************************************************************
    // Method: CreateNode
    // See <IGPUPipeline::CreateNode>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipeline::CreateNode(uint32 numInputs, IShader *pShader)
    {
        std::unique_ptr<CGPUPipelineNode> spSource(new CGPUPipelineNode(numInputs));
        m_nodes.push_back(CRefObj<IGPUPipelineNode>(spSource.get()));
        return CRefObj<IGPUPipelineNode>(spSource.release());
    }

    struct SPredefinedNodes
    {
        const wchar_t *m_pNodeName;
        bool m_flipUVs;
    } _SPredefinedNodes[] = {
        { g_GaussianVerticalFilter, true },
        { g_GaussianHorizontalFilter, false },
        { g_RawCopyFilter, false }
    };

    //**********************************************************************
    // Method: CreatePredefinedNode
    // See <IGPUPipeline::CreatePredefinedNode>
    //**********************************************************************
    CRefObj<IGPUPipelineNode> CGPUPipeline::CreatePredefinedNode(wchar_t *pNodeName)
    {
        for (SPredefinedNodes &pNode : _SPredefinedNodes)
        {
            if (_wcsicmp(pNode.m_pNodeName, pNodeName) == 0)
            {
                std::unique_ptr<CGPUPipelineNode> spNode(new CGPUPipelineNode(1));
                CRefObj<IShader> spShader = GetRenderer()->GetShaderMgr()->FindShader(pNodeName);
                spNode->SetShader(spShader);
                m_nodes.push_back(CRefObj<IGPUPipelineNode>(spNode.get()));
                return CRefObj<IGPUPipelineNode>(spNode.release());
            }
        }
        CT(E_INVALIDARG); // Couldn't find node
        return CRefObj<IGPUPipelineNode>(nullptr);
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
        float pts[4];
        float uvs[2];
    };

    //**********************************************************************
    void CGPUPipeline::Initialize(IRenderer *pRenderer)
    {
        if (pRenderer == nullptr)
			pRenderer = Caustic::CCausticFactory::Instance()->CreateRenderer(nullptr, std::wstring(L""));
        m_spRenderer = pRenderer;

        // Make sure the shaders required are loaded
        m_spRenderer->LoadShaders(L"");

        // Setup vertex buffer
        CD3D11_BUFFER_DESC vbdesc(sizeof(GPUVertex) * 4, D3D11_BIND_VERTEX_BUFFER);
        GPUVertex quadPts[5] = {
            { -1.0f, -1.0f, 0.9f, 1.0f, 0.0f, 0.0f },
            { -1.0f, +1.0f, 0.9f, 1.0f, 0.0f, 1.0f, },
            { +1.0f, +1.0f, 0.9f, 1.0f, 1.0f, 1.0f, },
            { +1.0f, -1.0f, 0.9f, 1.0f, 1.0f, 0.0f },
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
    CRefObj<IGPUPipelineNode> CGPUPipelineNodeBase::GetInput(uint32 index)
    {
        return CRefObj<IGPUPipelineNode>(m_sourceNodes[index]);
    }

    //**********************************************************************
    // Method: SetInput
    // See <IGPUPipelineNode::SetInput>
    //**********************************************************************
    void CGPUPipelineNodeBase::SetInput(uint32 index, IGPUPipelineNode *pNode)
    {
        m_sourceNodes[index] = pNode;
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
        spCtx->IASetVertexBuffers(0, 1, &m_spFullQuadVB, &vertexSize, &offset);
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

        // Get the input textures from the earlier nodes in the pipeline
        std::vector<CRefObj<ITexture>> textures;
        std::vector<CSamplerRef> samplers;
        for (int i = 0; i < m_sourceNodes.size(); i++)
        {
            CRefObj<ITexture> spTexture = m_sourceNodes[i]->GetOutputTexture(pPipeline);
            textures.push_back(spTexture);
            CRefObj<ISampler> spSampler = Caustic::CCausticFactory::Instance()->CreateSampler(spRenderer, spTexture);
            samplers.push_back(CSamplerRef(spSampler));
        }

        if (m_spShader)
        {
            // Set the input from earlier nodes as our source textures in our shader
            for (int i = 0; i < textures.size(); i++)
            {
                wchar_t buf[1024];
                swprintf_s(buf, L"sourceSampler%d", i + 1);
                m_spShader->SetPSParam(buf, std::any(samplers[i]));
                swprintf_s(buf, L"sourceTexture%d", i + 1);
                m_spShader->SetPSParam(buf, std::any(textures[i]));
            }

            // Create our output texture and assign it as our render target
            if (m_width == 0 || m_height == 0)
            {
                m_width = textures[0]->GetWidth();
                m_height = textures[0]->GetHeight();
            }
            m_spOutputTexture = Caustic::CCausticFactory::Instance()->CreateTexture(spRenderer, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, m_cpuFlags, m_bindFlags);
            CComPtr<ID3D11RenderTargetView> spRTView;
            CT(spDevice->CreateRenderTargetView(m_spOutputTexture->GetD3DTexture(), nullptr, &spRTView));

            // Setup render target
            spCtx->OMSetRenderTargets(1, &spRTView, nullptr);
            FLOAT bgClr[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            spCtx->ClearRenderTargetView(spRTView, bgClr);

            // Draw full screen quad using shader
            pPipeline->RenderQuad(m_spShader);
        }
    }

    //**********************************************************************
    // Method: SetSource
    // See <IGPUPipelineSourceNode::SetSource>
    //**********************************************************************
    void CGPUPipelineSourceNode::SetSource(IGPUPipeline *pPipeline, IImage *pSource)
    {
        CRefObj<IRenderer> spRenderer = pPipeline->GetRenderer();
        m_spOutputTexture = Caustic::CCausticFactory::Instance()->CreateTexture(spRenderer, pSource->GetWidth(), pSource->GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
        CComPtr<ID3D11DeviceContext> spCtx = spRenderer->GetContext();
        CComPtr<ID3D11Texture2D> spTexture = m_spOutputTexture->GetD3DTexture();
        D3D11_MAPPED_SUBRESOURCE mapinfo;
        spCtx->Map(spTexture, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapinfo);
        BYTE *pSrcRow = pSource->GetData();
        BYTE *pDstRow = (BYTE*)mapinfo.pData;
        for (uint32 y = 0; y < pSource->GetHeight(); y++)
        {
            BYTE *pSrcCol = pSrcRow;
            BYTE *pDstCol = pDstRow;
            for (uint32 x = 0; x < pSource->GetHeight(); x++)
            {
                pDstCol[0] = pSrcCol[0];
                pDstCol[1] = pSrcCol[1];
                pDstCol[2] = pSrcCol[2];
                pDstCol[3] = pSrcCol[3];
                pSrcCol += 4;
                pDstCol += 4;
            }
            pSrcRow += pSource->GetStride();
            pDstRow += mapinfo.RowPitch;
        }
        spCtx->Unmap(spTexture, 0);
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
        _ASSERT(m_sourceNodes.size() == 1 && m_sourceNodes[0] != nullptr);
        CRefObj<ITexture> spTexture = m_sourceNodes[0]->GetOutputTexture(pPipeline);
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
