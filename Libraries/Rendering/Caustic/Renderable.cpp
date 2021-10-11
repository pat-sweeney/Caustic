//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
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
        m_passes = 1 << Caustic::c_PassOpaque;
    }

    //**********************************************************************
    // Method: RenderMesh
    // Renders either the front or back faces of a mesh
    //
    // Parameters:
    // pRenderer - graphics device
    // lights - List of lights to apply
    // pRenderCtx - Rendering context
    // pRenderMaterial - Material to apply
    // cullmode - Culling mode to apply
    //**********************************************************************
    void CRenderable::RenderMesh(IRenderer *pRenderer, std::vector<CRefObj<ILight>> &lights,
        IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D11_CULL_MODE cullmode)
    {
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spContext = pRenderer->GetContext();

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
            spShader = pRenderer->GetShaderMgr()->FindShader(L"ColorNormal");
        else if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayUVsAsColors)
            spShader = pRenderer->GetShaderMgr()->FindShader(L"ColorUVs");
        else
            spShader = pRenderMaterial->GetShader();

        static int id = 0;
        std::any anyId(id);
        spShader->SetPSParam(L"objID", anyId);
        id++;

        // Setup the material
        pRenderMaterial->Render(pRenderer, lights, pRenderCtx, spShader);

        UINT offsets = 0;
        const MeshData &md = m_spSubMesh->GetMeshData();
        spContext->IASetVertexBuffers(0, 1, &md.m_spVB.p, &md.m_vertexSize, &offsets);

        if (md.m_numIndices > 0)
            spContext->IASetIndexBuffer(md.m_spIB, DXGI_FORMAT_R32_UINT, 0);

        spShader->BeginRender(pRenderer, m_spFrontMaterial /*, m_spBackMaterial */, lights, &m_xform);

        spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        if (md.m_numIndices > 0)
            spContext->DrawIndexedInstanced(md.m_numIndices, 1, 0, 0, 0);
        else
            spContext->DrawInstanced(md.m_numVertices, 1, 0, 0);
        spShader->EndRender(pRenderer);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderable::Render>
    //**********************************************************************
    void CRenderable::Render(IRenderer *pRenderer, std::vector<CRefObj<ILight>> &lights, IRenderCtx *pRenderCtx)
    {
        CComPtr<ID3D11Device> spDevice = pRenderer->GetDevice();
        CComPtr<ID3D11DeviceContext> spContext = pRenderer->GetContext();

        if (m_spFrontMaterial)
        {
            RenderMesh(pRenderer, lights, pRenderCtx, m_spFrontMaterial, D3D11_CULL_MODE::D3D11_CULL_BACK);
        }
        if (m_spBackMaterial)
        {
            RenderMesh(pRenderer, lights, pRenderCtx, m_spBackMaterial, D3D11_CULL_MODE::D3D11_CULL_FRONT);
        }
        if ((pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ||
            (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals))
        {
            CRefObj<IShader> spShader = pRenderer->GetShaderMgr()->FindShader(L"DrawNormal");
            UINT offset = 0;
            const MeshData &md = m_spSubMesh->GetMeshData(true);
            spContext->IASetVertexBuffers(0, 1, &md.m_spVB.p, &md.m_vertexSize, &offset);
            spShader->SetVSParam(L"normalScale", std::any(2.0f));
            float v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawFaceNormals", std::any(v));
            v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawNormals", std::any(v));

            spShader->BeginRender(pRenderer, m_spFrontMaterial /*, m_spBackMaterial */, lights, nullptr);
            spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            spContext->DrawInstanced(md.m_numVertices, 1, 0, 0);
            spShader->EndRender(pRenderer);
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
