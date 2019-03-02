//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
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


namespace Caustic
{
#include "QuadVS.h"
#include "QuadVS.tbl"
#include "GaussianPS.h"
#include "GaussianPS.tbl"
#include "RawCopyPS.h"
#include "RawCopyPS.tbl"

    //**********************************************************************
    //! \brief s_defaultVSLayout defines the default layout for our default vertex
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_VSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    //! \brief Creates a new shader using the specified byte code and list
    //! of shader parameters.
    //! \param[in] pShaderCode Pointer to byte array containing the shader executable
    //! \param[in] shaderCodeSize Size of pShaderCode in bytes
    //! \param[in] pShaderParams List of parameters to set on the shader
    //! \param[in] shaderParamSize Number of parameters in pShaderParams
    //! \param[out] ppShader Newly created shader object
    //**********************************************************************
    void CGPUPipeline::CreateShader(BYTE *pShaderCode, uint32 shaderCodeSize, ShaderDefs *pShaderParams, uint32 shaderParamSize, IShader **ppShader)
    {
        CRefObj<IShader> spShader;
        Caustic::CreateShader(m_spGraphics.p, nullptr,
            pShaderParams, shaderParamSize,
            g_QuadVS_ParamTable, _countof(g_QuadVS_ParamTable),
            pShaderCode, shaderCodeSize,
            g_QuadVS, sizeof(g_QuadVS),
            s_VSLayout, _countof(s_VSLayout), &spShader);
        *ppShader = spShader.Detach();
    }

    //**********************************************************************
    //! \brief Creates a source node. Source nodes read IImage and make them
    //! available to the rest of the GPU imaging pipeline.
    //! \param[in] pImage Image to use as source data
    //! \param[out] ppNewNode Returns the new source node
    //**********************************************************************
    void CGPUPipeline::CreateSourceNode(IImage *pImage, IGPUImageSourceNode **ppNewNode)
    {
        std::unique_ptr<CGPUImageSourceNode> spSource(new CGPUImageSourceNode());
        spSource->SetSource(this, pImage);
        m_sourceNodes.push_back(CRefObj<IGPUImageSourceNode>(spSource.get()));
        *ppNewNode = spSource.release();
        (*ppNewNode)->AddRef();
    }

    //**********************************************************************
    //! \brief Creates a sink node. Sink nodes are the final stage in a GPU
    //! imaging pipeline and are responsible for converting the D3D texture
    //! back into an IImage.
    //! \param[in] numInputs Number of source textures that feed this node
    //! \param[in] pShader Shader to run before returning result
    //! \param[out] ppNewNode Returns the newly created node
    //**********************************************************************
    void CGPUPipeline::CreateSinkNode(uint32 numInputs, IShader *pShader, IGPUImageSinkNode **ppNewNode)
    {
        std::unique_ptr<CGPUImageSinkNode> spSource(new CGPUImageSinkNode(numInputs));
        m_sinkNodes.push_back(CRefObj<IGPUImageSinkNode>(spSource.get()));
        *ppNewNode = spSource.release();
        (*ppNewNode)->AddRef();
    }

    //**********************************************************************
    //! \brief Creates an internal GPU pipeline node. These nodes do the bulk
    //! of the processing.
    //! \param[in] numInputs Number of source texture that feed this node
    //! \param[in] pShader Pixel shader to run on this data
    //! \param[out] ppNewNode Returns the new node
    //**********************************************************************
    void CGPUPipeline::CreateNode(uint32 numInputs, IShader *pShader, IGPUImageNode **ppNewNode)
    {
        std::unique_ptr<CGPUImageNode> spSource(new CGPUImageNode(numInputs));
        m_nodes.push_back(CRefObj<IGPUImageNode>(spSource.get()));
        *ppNewNode = spSource.release();
        (*ppNewNode)->AddRef();
    }

    struct SPredefinedNodes
    {
        const wchar_t *m_pNodeName;
        bool m_flipUVs;
        BYTE *m_pShaderCode;
        uint32 m_shaderSize; // Number of bytes in m_pShaderCode
        ShaderDefs *m_pParams;
        uint32 m_paramSize; // Number of parameters in m_pParams
    } _SPredefinedNodes[] = {
        { g_GaussianVerticalFilter, true, (BYTE*)g_GaussianPS, sizeof(g_GaussianPS), g_GaussianPS_ParamTable, _countof(g_GaussianPS_ParamTable) },
        { g_GaussianHorizontalFilter, false, (BYTE*)g_GaussianPS, sizeof(g_GaussianPS), g_GaussianPS_ParamTable, _countof(g_GaussianPS_ParamTable) },
        { g_RawCopyFilter, false, (BYTE*)g_RawCopyPS, sizeof(g_RawCopyPS), g_RawCopyPS_ParamTable, _countof(g_RawCopyPS_ParamTable) }
    };

    //**********************************************************************
    //! \brief Creates an predefined GPU pipeline node. These nodes are
    //! already associated with some shader (for instance a Gaussian blur filter)
    //! \param[in] pShaderName Name of GPU graph node to create
    //! \param[out] Returns the newly created node
    //**********************************************************************
    void CGPUPipeline::CreatePredefinedNode(wchar_t *pShaderName, IGPUImageNode **ppNewNode)
    {
        *ppNewNode = nullptr;
        for (SPredefinedNodes &pNode : _SPredefinedNodes)
        {
            if (_wcsicmp(pNode.m_pNodeName, pShaderName) == 0)
            {
                std::unique_ptr<CGPUImageNode> spNode(new CGPUImageNode(1));
                CRefObj<IShader> spShader;
                CreateShader(pNode.m_pShaderCode, pNode.m_shaderSize, pNode.m_pParams, pNode.m_paramSize, &spShader);
                spNode->SetShader(spShader.p);
                m_nodes.push_back(CRefObj<IGPUImageNode>(spNode.get()));
                *ppNewNode = spNode.release();
                (*ppNewNode)->AddRef();
                break;
            }
        }
    }

    //**********************************************************************
    //! \brief Starts processing of a GPU graph. Data is processed using a
    //! a pull model starting at each sink node.
    //**********************************************************************
    void CGPUPipeline::Process()
    {
        for (CRefObj<IGPUImageSinkNode> &pCurNode : m_sinkNodes)
        {
            pCurNode->GetOutputTexture(this, nullptr);
        }
    }
    
    //**********************************************************************
    //! \brief Returns the graphics device associated with this pipeline
    //! \param[out] ppDevice Returns the IGraphics device associated with this pipeline
    //**********************************************************************
    void CGPUPipeline::GetGraphics(IGraphics **ppDevice)
    {
        *ppDevice = m_spGraphics.p;
        (*ppDevice)->AddRef();
    }

    //**********************************************************************
    //! \brief Defines a vertex used by the quad rendering
    //**********************************************************************
    struct GPUVertex
    {
        float pts[4];
        float uvs[2];
    };

    //**********************************************************************
    //! \brief Setups our pipeline
    //! \param[in] pGraphics Graphics device to use. If null then a new device is created
    //**********************************************************************
    void CGPUPipeline::Initialize(IGraphics *pGraphics)
    {
        if (pGraphics == nullptr)
			Caustic::CCausticFactory::Instance()->CreateGraphics(nullptr, &pGraphics);
        m_spGraphics = pGraphics;
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
        CComPtr<ID3D11Device> spDevice = m_spGraphics->GetDevice();
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
    //! \brief Allocates a new pipeline object
    //! \param[in] pGraphics Specifies the IGraphics object to be used. If null a new graphics device is created.
    //! \param[out] pPipeline Returns the newly created pipeline object
    //**********************************************************************
    void CreateGPUPipeline(IGraphics *pGraphics, IGPUPipeline **ppPipeline)
    {
        std::unique_ptr<CGPUPipeline> spPipeline(new CGPUPipeline());
        spPipeline->Initialize(pGraphics);
        *ppPipeline = spPipeline.release();
        (*ppPipeline)->AddRef();
    }

    //**********************************************************************
    //! \brief Sets the shader associated with this graph node
    //! \param[in] pShader Specifies the shader to use for this node
    //**********************************************************************
    void CGPUImageBase::SetShader(IShader *pShader)
    {
        m_spShader = pShader;
    }

    //**********************************************************************
    //! \brief Returns the shader associated with this graph node
    //! \param[out] ppShader Returns the shader associated with this node
    //**********************************************************************
    void CGPUImageBase::GetShader(IShader **ppShader)
    {
        *ppShader = m_spShader.p;
        (*ppShader)->AddRef();
    }
    
    //**********************************************************************
    //! \brief Returns the node that is the Nth input into this node
    //! \param[in] index Input slot to return
    //! \param[out] ppNode Returns the node used as input in slot 'index'
    //**********************************************************************
    void CGPUImageBase::GetInput(uint32 index, IGPUImageNode **ppNode)
    {
        *ppNode = m_sourceNodes[index];
        (*ppNode)->AddRef();
    }

    //**********************************************************************
    //! \brief Sets the Nth input node (i.e. this node gets input from the specified node)
    //! \param[in] index Slot pNode is used as input to
    //! \param[in] pNode Node to set as input in slot 'index'
    //**********************************************************************
    void CGPUImageBase::SetInput(uint32 index, IGPUImageNode *pNode)
    {
        m_sourceNodes[index] = pNode;
    }

    //**********************************************************************
    //! \brief Retrieves the output from this node (after processing has been performed)
    //! \param[in] pPipeline Pipeline this node is part of
    //! \param[out] ppTexture The output texture for this node
    //**********************************************************************
    void CGPUImageBase::GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture)
    {
        Process(pPipeline);
        if (ppTexture)
            *ppTexture = m_spOutputTexture.Detach();
    }

    //**********************************************************************
    //! \brief Renders a full screen quad on which the node's shader will be run
    //! \param[in] pShader Renders a quad using the specified pixel shader
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
        CComPtr<ID3D11Device> spDevice = m_spGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = m_spGraphics->GetContext();
        spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        spCtx->RSSetState(spRasterState);
        spCtx->IASetVertexBuffers(0, 1, &m_spFullQuadVB.p, &vertexSize, &offset);
        spCtx->IASetIndexBuffer(m_spFullQuadIB.p, DXGI_FORMAT_R32_UINT, 0);
        pShader->BeginRender(m_spGraphics.p);
        spCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        spCtx->DrawIndexed(6, 0, 0);
        pShader->EndRender(m_spGraphics.p);
    }

    //**********************************************************************
    //! \brief Process first obtains the inputs for this node and then runs
    //! the node's shader resulting in a final output texture
    //! \param[in] pPipeline Pipeline this node is a part of
    //**********************************************************************
    void CGPUImageBase::Process(IGPUPipeline *pPipeline)
    {
        CRefObj<IGraphics> spGraphics;
        pPipeline->GetGraphics(&spGraphics);
        CComPtr<ID3D11Device> spDevice = spGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spGraphics->GetContext();

        // Get the input textures from the earlier nodes in the pipeline
        std::vector<CRefObj<ITexture>> textures;
        std::vector<CSamplerRef> samplers;
        for (int i = 0; i < m_sourceNodes.size(); i++)
        {
            CRefObj<ITexture> spTexture;
            m_sourceNodes[i]->GetOutputTexture(pPipeline, &spTexture);
            textures.push_back(spTexture);
            CRefObj<ISampler> spSampler;
			Caustic::CCausticFactory::Instance()->CreateSampler(spGraphics.p, spTexture.p, &spSampler);
            samplers.push_back(CSamplerRef(spSampler.p));
        }

        if (m_spShader.p)
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
			Caustic::CCausticFactory::Instance()->CreateTexture(spGraphics.p, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, m_cpuFlags, m_bindFlags, &m_spOutputTexture);
            CComPtr<ID3D11RenderTargetView> spRTView;
            CT(spDevice->CreateRenderTargetView(m_spOutputTexture->GetD3DTexture(), nullptr, &spRTView));

            // Setup render target
            spCtx->OMSetRenderTargets(1, &spRTView.p, nullptr);
            FLOAT bgClr[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            spCtx->ClearRenderTargetView(spRTView, bgClr);

            // Draw full screen quad using shader
            pPipeline->RenderQuad(m_spShader.p);
        }
    }

    //**********************************************************************
    //! \brief Assigns an image to be used as a source texture
    //! \param[in] pPipeline Pipeline this node is a part of
    //! \param[in] pSource Image to use as source data into this node
    //**********************************************************************
    void CGPUImageSourceNode::SetSource(IGPUPipeline *pPipeline, IImage *pSource)
    {
        CRefObj<IGraphics> spGraphics;
        pPipeline->GetGraphics(&spGraphics);
		Caustic::CCausticFactory::Instance()->CreateTexture(spGraphics.p, pSource->GetWidth(), pSource->GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_CPU_ACCESS_WRITE, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, &m_spOutputTexture);
        CComPtr<ID3D11DeviceContext> spCtx = spGraphics->GetContext();
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
    //! \brief Retrieves the output texture
    //! \param[in] pPipeline Pipeline this node is a part of
    //! \param[out] ppTexture Returns the texture this node outputs
    //**********************************************************************
    void CGPUImageSourceNode::GetOutputTexture(IGPUPipeline *pPipeline, ITexture **ppTexture)
    {
        if (ppTexture)
        {
            *ppTexture = m_spOutputTexture.p;
            (*ppTexture)->AddRef();
        }
    }

    //**********************************************************************
    void CGPUImageSinkNode::Process(IGPUPipeline *pPipeline)
    {
        CRefObj<IGraphics> spGraphics;
        pPipeline->GetGraphics(&spGraphics);
        CComPtr<ID3D11Device> spDevice = spGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spGraphics->GetContext();
        _ASSERT(m_sourceNodes.size() == 1 && m_sourceNodes[0] != nullptr);
        CRefObj<ITexture> spTexture;
        m_sourceNodes[0]->GetOutputTexture(pPipeline, &spTexture);
        if (m_width == 0 || m_height == 0)
        {
            m_width = spTexture->GetWidth();
            m_height = spTexture->GetHeight();
        }
		Caustic::CCausticFactory::Instance()->CreateTexture(spGraphics.p, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, m_cpuFlags, m_bindFlags, &m_spOutputTexture);
        spCtx->CopyResource(m_spOutputTexture->GetD3DTexture(), spTexture->GetD3DTexture());
    }

    //**********************************************************************
    //! \brief Retrieves the output texture as an image
    //! \param[in] pPipeline Pipeline this node is a part of
    //! \param[out] ppSource Returns the output from this node as an Image (CPU based)
    //**********************************************************************
    void CGPUImageSinkNode::GetOutput(IGPUPipeline *pPipeline, IImage **ppSource)
    {
        CRefObj<IGraphics> spGraphics;
        pPipeline->GetGraphics(&spGraphics);
        CComPtr<ID3D11Device> spDevice = spGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spCtx = spGraphics->GetContext();
        spCtx->Flush();
        CRefObj<IImage> spImage;
        Caustic::CreateImage(m_width, m_height, &spImage);
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
        *ppSource = spImage.Detach();
    }
}
