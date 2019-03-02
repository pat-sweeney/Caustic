//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Renderer.h"
#include "Sampler.h"
#include "Geometry\Mesh\Material.h"
#include "Rendering\Caustic\CausticFactory.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Creates a renderable object
    //!
    //! Creates a renderable. A renderable is a vertex buffer, index buffer,
    //! and the default shader.
    //! \param[in] pGraphics Our graphics renderer
    //! \param[in] pSubMesh Submesh to generate index and vertex buffers from
    //! \param[in] pMaterial Material to apply to mesh
    //! \param[in] pTexture Texture to apply to mesh
    //! \param[out] ppRenderable Returns the newly created IRenderable
    //**********************************************************************
    CAUSTICAPI void CreateRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, IShader *pShader, IRenderable **ppRenderable)
    {
        if (pSubMesh->GetNumberVertices() == 0 || pSubMesh->GetNumberEdges() == 0 || pSubMesh->GetNumberFaces() == 0)
        {
            *ppRenderable = nullptr;
            return;
        }
        CRefObj<IRenderMaterial> spFrontMaterial;
		CCausticFactory::Instance()->CreateRenderMaterial(pGraphics, pMaterial, pShader, &spFrontMaterial);
        CRefObj<IRenderMaterial> spBackMaterial;
        if (pSubMesh->GetMeshFlags() & EMeshFlags::TwoSided)
        {
			CCausticFactory::Instance()->CreateRenderMaterial(pGraphics, pMaterial, pShader, &spBackMaterial);
        }

        DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
        std::unique_ptr<CRenderable> spRenderable(new CRenderable(pGraphics, pSubMesh, spFrontMaterial.p, spBackMaterial.p, mat));
        *ppRenderable = spRenderable.release();
        (*ppRenderable)->AddRef();
    }

    //**********************************************************************
    //! \brief Creates a renderable object
    //**********************************************************************
    CAUSTICAPI void CreateRenderable(ID3D11Buffer *pVB, uint32 numVertices,
        ID3D11Buffer *pIB, uint32 numIndices,
        IRenderMaterial *pFrontMaterial,
        IRenderMaterial *pBackMaterial,
        DirectX::XMMATRIX &mat,
        IRenderable **ppRenderable)
    {
        std::unique_ptr<CRenderable> spRenderable(new CRenderable(pVB, numVertices, pIB, numIndices, nullptr, 0, pFrontMaterial, pBackMaterial, mat));
        *ppRenderable = spRenderable.release();
        (*ppRenderable)->AddRef();
    }

    //**********************************************************************
    //! \brief CRenderable creates an empty renderable object
    //!
    //! Creates an empty renderable. This method should only be called by
    //! the Caustic APIs (it is used during Store/Load)
    //**********************************************************************
    CAUSTICAPI void CreateRenderable(IRenderable **ppRenderable)
    {
        std::unique_ptr<CRenderable> spRenderable(new CRenderable());
        *ppRenderable = spRenderable.release();
        (*ppRenderable)->AddRef();
    }

    //**********************************************************************
    //! \brief CRenderable creates a renderable object
    //!
    //! Creates a renderable. A renderable is a vertex buffer, index buffer,
    //! and the default shader. Objects are rendered in arbitrary order
    //! (as opposed to objects in the scene graph which are rendered in order).
    //! \param[in] pGraphics Our graphics renderer
    //! \param[in] pVB Vertex buffer
    //! \param[in] numVertices Number of vertices in pVB
    //! \param[in] pIB Index buffer
    //! \param[in] numIndices Number of indices in pIB
    //! \param[in] pFrontMaterial Material to apply to front facing polygons
    //! \param[in] pBackMaterial Material to apply to back facing polygons
    //**********************************************************************
    CRenderable::CRenderable(ID3D11Buffer *pVB, uint32 numVertices,
                             ID3D11Buffer *pIB, uint32 numIndices,
                             ID3D11Buffer *pNormalVB, uint32 numNormalVertices,
                             IRenderMaterial *pFrontMaterial,
                             IRenderMaterial *pBackMaterial,
                             DirectX::XMMATRIX &mat) :
        m_numIndices(numIndices),
        m_numVerts(numVertices),
        m_numNormalVerts(numNormalVertices),
        m_xform(mat)
    {
        m_spVB = pVB;
        m_spIB = pIB;
        m_spNormalVB = pNormalVB;
        m_spFrontMaterial = pFrontMaterial;
        m_spBackMaterial = pBackMaterial;
        m_passes = 1 << c_PassTransparent;
    }

    CRenderable::CRenderable(IGraphics *pGraphics, ISubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat)
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

    //**********************************************************************
    //! \brief Renders either the front or back faces of a mesh
    //! \param[in] pGraphics Interface to the graphics renderer
    //! \param[in] lights List of lights to apply
    //! \param[in] pRenderCtx Rendering context
    //! \param[in] pRenderMaterial Material to apply
    //! \param[in] cullmode Culling mode to apply
    //**********************************************************************
    void CRenderable::RenderMesh(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial,
        D3D11_CULL_MODE cullmode)
    {
        ID3D11DeviceContext *pContext = pGraphics->GetContext();
        ID3D11Device *pDevice = pGraphics->GetDevice();

        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = cullmode;
        rasDesc.DepthClipEnable = TRUE;
        bool showWireframe = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWireframe) ? true : false;
        rasDesc.FillMode = showWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
        pDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        pContext->RSSetState(spRasterState);

        CRefObj<IShader> spShader;
        if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsColors)
        {
            CShaderMgr::GetInstance()->FindShader(L"ColorNormal", &spShader);
        }
        else if (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayUVsAsColors)
        {
            CShaderMgr::GetInstance()->FindShader(L"ColorUVs", &spShader);
        }
        else
            pRenderMaterial->GetShader(&spShader);

        static int id = 0;
        std::any anyId(id);
        spShader->SetPSParam(L"objID", anyId);
        id++;

        // Setup the material
        pRenderMaterial->Render(pGraphics, lights, pRenderCtx, spShader.p);

        UINT offset = 0;
        UINT vertexSize = sizeof(SVertex_2);
        pContext->IASetVertexBuffers(0, 1, &m_spVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_spIB, DXGI_FORMAT_R32_UINT, 0);
        spShader->BeginRender(pGraphics, &m_xform);

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
            pDevice->CreateBlendState(&blendState, &spBlendState);
            pContext->OMSetBlendState(spBlendState, 0, 0xffffffff);
        }

        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(m_numIndices, 0, 0);
        spShader->EndRender(pGraphics);
    }

    //**********************************************************************
    //! \brief Render renders the renderable object
    //! \param[in] pGraphics Interface to the graphics renderer
    //**********************************************************************
    void CRenderable::Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx)
    {
        for (int j = 0; j < 2; j++)
        {
            if (j == 0 && m_spFrontMaterial.p)
            {
                RenderMesh(pGraphics, lights, pRenderCtx, m_spFrontMaterial.p, D3D11_CULL_BACK);
            }
            else if (j == 1 && m_spBackMaterial.p)
            {
                RenderMesh(pGraphics, lights, pRenderCtx, m_spBackMaterial.p, D3D11_CULL_FRONT);
            }
        }
        if ((pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ||
            (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals))
        {
            CRefObj<IShader> spShader;
            CShaderMgr::GetInstance()->FindShader(L"DrawNormal", &spShader);
            UINT offset = 0;
            UINT vertexSize = sizeof(SVertex_4);
            ID3D11DeviceContext *pContext = pGraphics->GetContext();
            pContext->IASetVertexBuffers(0, 1, &m_spNormalVB.p, &vertexSize, &offset);
            spShader->SetVSParam(L"normalScale", std::any(2.0f));
            float v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayFaceNormals) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawFaceNormals", std::any(v));
            v = (pRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayNormalsAsLines) ? 1.0f : 0.0f;
            spShader->SetVSParam(L"drawNormals", std::any(v));

            spShader->BeginRender(pGraphics);
            pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            pContext->Draw(m_numNormalVerts, 0);
            spShader->EndRender(pGraphics);
        }
    }
}
