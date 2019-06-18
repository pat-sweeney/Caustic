//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "Sampler.h"
#include "Rendering/Caustic/Material.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Renderable.h"

namespace Caustic
{
    //**********************************************************************
    // CRenderable creates a renderable object
    //
    // Creates a renderable. A renderable is a vertex buffer, (optionally) index buffer,
    // and the default shader. Objects are rendered in arbitrary order
    // (as opposed to objects in the scene graph which are rendered in order).
	//     pVB - Vertex buffer
	//     pNormalVB - vertex buffer for displaying normal vectors. Maybe nullptr
    //     pFrontMaterial Material to apply to front facing polygons
    //     pBackMaterial Material to apply to back facing polygons
	//	   mat - world matrix to apply
    //**********************************************************************
    CRenderable::CRenderable(CVertexBuffer *pVB, CVertexBuffer *pNormalVB,
                             IRenderMaterial *pFrontMaterial,
                             IRenderMaterial *pBackMaterial,
                             DirectX::XMMATRIX &mat) :
        m_xform(mat)
    {
		if (pVB != nullptr)
			m_vertexBuffer = *pVB;
		if (pNormalVB != nullptr)
			m_normalVB = *pNormalVB;
        m_spFrontMaterial = pFrontMaterial;
        m_spBackMaterial = pBackMaterial;
        m_passes = 1 << c_PassTransparent;
    }

#if 0
    CRenderable::CRenderable(IRe*pGraphics, ISubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat)
    {
        CComPtr<ID3D11Buffer> spVB;
        CComPtr<ID3D11Buffer> spIB;
        uint32 numVerts;
        uint32 numIndices;
		Caustic::CCausticFactory::Instance()->MeshToD3D(pGraphics, pSubMesh, 2, &spVB, &numVerts, 1, &spIB, &numIndices, nullptr, nullptr);
        uint32 numNormalVerts;
        CComPtr<ID3D11Buffer> spNormalVB;
		Caustic::CCausticFactory::Instance()->MeshToNormals(pGraphics, pSubMesh, &spNormalVB, &numNormalVerts);
        CRefObj<ITexture> spTexture;
        *this = CRenderable(
            spVB, numVerts, 
            spIB, numIndices, 
            spNormalVB, numNormalVerts,
            pFrontMaterial, pBackMaterial, mat);
    }
#endif
    //**********************************************************************
    // Renders either the front or back faces of a mesh
    //		pRenderer - rendering device
    //		lights - List of lights to apply
    //		pRenderCtx - Rendering context
    //		pRenderMaterial - Material to apply
    //		cullmode - Culling mode to apply
    //**********************************************************************
    void CRenderable::RenderMesh(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights,
		IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D12_CULL_MODE cullmode)
    {
        CComPtr<ID3D12Device> spDevice = pRenderer->GetDevice();

        //CComPtr<ID3D11RasterizerState> spRasterState;
        //D3D11_RASTERIZER_DESC rasDesc;
        //ZeroMemory(&rasDesc, sizeof(rasDesc));
        //rasDesc.CullMode = cullmode;
        //rasDesc.DepthClipEnable = TRUE;
        //bool showWireframe = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWireframe) ? true : false;
        //rasDesc.FillMode = showWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
        //pDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        //pContext->RSSetState(spRasterState);

        CRefObj<IShader> spShader;
        if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsColors)
        {
            pRenderer->GetShaderMgr()->FindShader(L"ColorNormal", &spShader);
        }
        else if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayUVsAsColors)
        {
			pRenderer->GetShaderMgr()->FindShader(L"ColorUVs", &spShader);
        }
        else
            pRenderMaterial->GetShader(&spShader);

        static int id = 0;
        std::any anyId(id);
        spShader->SetPSParam(L"objID", anyId);
        id++;

        // Setup the material
        pRenderMaterial->Render(pRenderer, lights, pRenderCtx, spShader);

		CComPtr<ID3D12GraphicsCommandList> spCommandList = pRenderer->GetCommandList();
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = m_vertexBuffer.m_spVB->GetGPUVirtualAddress();
		vbView.SizeInBytes = m_vertexBuffer.m_numVertices * m_vertexBuffer.m_vertexSize;
		vbView.StrideInBytes = m_vertexBuffer.m_vertexSize;
		spCommandList->IASetVertexBuffers(0, 1, &vbView);

		if (m_vertexBuffer.m_numIndices > 0)
		{
			D3D12_INDEX_BUFFER_VIEW ibView = {};
			ibView.BufferLocation = m_vertexBuffer.m_spIB->GetGPUVirtualAddress();
			ibView.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			ibView.SizeInBytes = m_vertexBuffer.m_numIndices * sizeof(uint16);
			spCommandList->IASetIndexBuffer(&ibView);
		}

		spShader->BeginRender(pRenderer, m_spFrontMaterial, m_spBackMaterial, lights, &m_xform);

        //if (pRenderCtx->PassBlendable())
        //{
        //    CComPtr<ID3D11BlendState> spBlendState;
        //    D3D11_BLEND_DESC blendState;
        //    ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
        //    blendState.AlphaToCoverageEnable = false;
        //    blendState.IndependentBlendEnable = false;
        //    blendState.RenderTarget[0].BlendEnable = true;
        //    blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        //    blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        //    blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        //    blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        //    blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        //    blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        //    blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        //    pDevice->CreateBlendState(&blendState, &spBlendState);
        //    pContext->OMSetBlendState(spBlendState, 0, 0xffffffff);
        //}

        spCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		if (m_vertexBuffer.m_numIndices > 0)
			spCommandList->DrawIndexedInstanced(m_vertexBuffer.m_numIndices, 1, 0, 0, 0);
		else
			spCommandList->DrawInstanced(m_vertexBuffer.m_numVertices, 1, 0, 0);
		spShader->EndRender(pRenderer);
    }

    //**********************************************************************
    // Render renders the renderable object
    // pGraphics Interface to the graphics renderer
    //**********************************************************************
    void CRenderable::Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx)
    {
        for (int j = 0; j < 2; j++)
        {
            if (j == 0 && m_spFrontMaterial)
            {
                RenderMesh(pRenderer, lights, pRenderCtx, m_spFrontMaterial, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
            }
            else if (j == 1 && m_spBackMaterial)
            {
                RenderMesh(pRenderer, lights, pRenderCtx, m_spBackMaterial, D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT);
            }
        }
        if ((pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ||
            (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals))
        {
            CRefObj<IShader> spShader;
            CShaderMgr::Instance()->FindShader(L"DrawNormal", &spShader);
            UINT offset = 0;
			CComPtr<ID3D12Device5> spDevice = pRenderer->GetDevice();
			CComPtr<ID3D12GraphicsCommandList4> spCommandList = pRenderer->GetCommandList();
			D3D12_VERTEX_BUFFER_VIEW vbView = {};
			vbView.BufferLocation = m_normalVB.m_spVB->GetGPUVirtualAddress();
			vbView.SizeInBytes = m_normalVB.m_numVertices * m_normalVB.m_vertexSize;
			vbView.StrideInBytes = m_normalVB.m_vertexSize;
			spCommandList->IASetVertexBuffers(0, 1, &vbView);
            spShader->SetVSParam(L"normalScale", std::any(2.0f));
            float v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawFaceNormals", std::any(v));
            v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawNormals", std::any(v));

            spShader->BeginRender(pRenderer, m_spFrontMaterial, m_spBackMaterial, lights);
            spCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            spCommandList->DrawInstanced(m_normalVB.m_numVertices, 1, 0, 0);
            spShader->EndRender(pRenderer);
        }
    }
}
