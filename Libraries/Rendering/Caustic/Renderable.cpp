//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "Sampler.h"
#include "Rendering\Caustic\RenderMaterial.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Renderable.h"
#include <d3d11.h>

//**********************************************************************
// File: Renderable.cpp
// Contains implementation for CRenderable
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Constructor: CRenderable
    // Creates a renderable object. A Renderable is a IRenderSubMesh
    // paired with a set of materials and a shader.
    //
    // Parameters:
    // pSubMesh - Render submesh
    // pFrontMaterial Material to apply to front facing polygons
    // pBackMaterial Material to apply to back facing polygons
    // mat - world matrix to apply
    //**********************************************************************
    CRenderable::CRenderable(IRenderSubMesh *pSubMesh,
        IRenderMaterial *pFrontMaterial,
        IRenderMaterial *pBackMaterial,
        DirectX::XMMATRIX &mat) :
        m_xform(mat)
    {
        m_spSubMesh = pSubMesh;
        m_spFrontMaterial = pFrontMaterial;
        m_spBackMaterial = pBackMaterial;
        m_passes = 1 << c_PassTransparent;
    }

    //**********************************************************************
    // Method: RenderMesh
    // Renders either the front or back faces of a mesh
    //
    // Parameters:
    // pGraphics - graphics device
    // lights - List of lights to apply
    // pRenderCtx - Rendering context
    // pRenderMaterial - Material to apply
    // cullmode - Culling mode to apply
    //**********************************************************************
    void CRenderable::RenderMesh(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights,
        IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D11_CULL_MODE cullmode)
    {
        CComPtr<ID3D11Device> spDevice = pGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spContext = pGraphics->GetContext();

        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = cullmode;
        rasDesc.DepthClipEnable = TRUE;
        bool showWireframe = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWireframe) ? true : false;
        rasDesc.FillMode = showWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
        spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        spContext->RSSetState(spRasterState);

        CRefObj<IShader> spShader;
        if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsColors)
        {
            pGraphics->GetShaderMgr()->FindShader(L"ColorNormal", &spShader);
        }
        else if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayUVsAsColors)
        {
            pGraphics->GetShaderMgr()->FindShader(L"ColorUVs", &spShader);
        }
        else
            pRenderMaterial->GetShader(&spShader);

        static int id = 0;
        std::any anyId(id);
        spShader->SetPSParam(L"objID", anyId);
        id++;

        // Setup the material
        pRenderMaterial->Render(pGraphics, lights, pRenderCtx, spShader);

        UINT offsets = 0;
        const MeshData &md = m_spSubMesh->GetMeshData();
        spContext->IASetVertexBuffers(0, 1, &md.m_spVB.p, &md.m_vertexSize, &offsets);

        if (md.m_numIndices > 0)
            spContext->IASetIndexBuffer(md.m_spIB, DXGI_FORMAT_R32_UINT, 0);

        spShader->BeginRender(pGraphics, m_spFrontMaterial, m_spBackMaterial, lights, &m_xform);

        if (pRenderCtx->PassBlendable())
        {
            CComPtr<ID3D11BlendState> spBlendState;
            D3D11_BLEND_DESC blendState;
            ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
            blendState.AlphaToCoverageEnable = false;
            blendState.IndependentBlendEnable = false;
            blendState.RenderTarget[0].BlendEnable = true;
            blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
            blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            spDevice->CreateBlendState(&blendState, &spBlendState);
            spContext->OMSetBlendState(spBlendState, 0, 0xffffffff);
        }

        spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        if (md.m_numIndices > 0)
            spContext->DrawIndexedInstanced(md.m_numIndices, 1, 0, 0, 0);
        else
            spContext->DrawInstanced(md.m_numVertices, 1, 0, 0);
        spShader->EndRender(pGraphics);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderable::Render>
    //**********************************************************************
    void CRenderable::Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx)
    {
        CComPtr<ID3D11Device> spDevice = pGraphics->GetDevice();
        CComPtr<ID3D11DeviceContext> spContext = pGraphics->GetContext();

        for (int j = 0; j < 2; j++)
        {
            if (j == 0 && m_spFrontMaterial)
            {
                RenderMesh(pGraphics, lights, pRenderCtx, m_spFrontMaterial, D3D11_CULL_MODE::D3D11_CULL_BACK);
            }
            else if (j == 1 && m_spBackMaterial)
            {
                RenderMesh(pGraphics, lights, pRenderCtx, m_spBackMaterial, D3D11_CULL_MODE::D3D11_CULL_FRONT);
            }
        }
        if ((pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ||
            (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals))
        {
            CRefObj<IShader> spShader;
            pGraphics->GetShaderMgr()->FindShader(L"DrawNormal", &spShader);
            UINT offset = 0;
            const MeshData &md = m_spSubMesh->GetMeshData(true);
            spContext->IASetVertexBuffers(0, 1, &md.m_spVB.p, &md.m_vertexSize, &offset);
            spShader->SetVSParam(L"normalScale", std::any(2.0f));
            float v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawFaceNormals", std::any(v));
            v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawNormals", std::any(v));

            spShader->BeginRender(pGraphics, m_spFrontMaterial, m_spBackMaterial, lights, nullptr);
            spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            spContext->DrawInstanced(md.m_numVertices, 1, 0, 0);
            spShader->EndRender(pGraphics);
        }
    }

    //**********************************************************************
    // Function: CreateRenderable
    // Creates a renderable object
    //
    // Parameters:
    // pSubMesh - Submesh to create renderable for
    // pFrontMaterial - material for front faces
    // pBackMaterial - material for back faces
    // mat - transform to apply
    //
    // Returns:
    // Returns the created renderable
    //**********************************************************************
    CAUSTICAPI CRefObj<IRenderable> CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat)
    {
        return CRefObj<IRenderable>(new CRenderable(pSubMesh, pFrontMaterial, pBackMaterial, mat));
    }
}
