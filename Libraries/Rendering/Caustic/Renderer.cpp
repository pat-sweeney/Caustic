//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Renderer.h"
#include <vector>
#include <any>
#include "Base\Core\CritSec.h"
#include <d3d11.h>
namespace Caustic {
#include "DefaultVS.h"
#include "DefaultVS.tbl"
#include "DrawNormalPS.h"
#include "DrawNormalPS.tbl"
#include "DrawNormalVS.h"
#include "DrawNormalVS.tbl"
#include "ColorNormalPS.h"
#include "ColorNormalPS.tbl"
#include "ColorUVsPS.h"
#include "ColorUVsPS.tbl"
#include "DefaultPS.h"
#include "DefaultPS.tbl"
#include "TexturedPS.h"
#include "TexturedPS.tbl"
#include "LineVS.h"
#include "LineVS.tbl"
#include "LinePS.h"
#include "LinePS.tbl"
#ifdef SUPPORT_SHADOW_MAPPING
#include "ShadowMapPS.h"
#include "ShadowMapPS.tbl"
#include "ShadowMapVS.h"
#include "ShadowMapVS.tbl"
#endif // SUPPORT_SHADOW_MAPPING
    //#include "InfinitePlaneVS.h"
//#include "InfinitePlaneVS.tbl"
//#include "InfinitePlanePS.h"
//#include "InfinitePlanePS.tbl"
#ifdef SUPPORT_FULLQUAD
#include "FullQuadVS.h"
#include "FullQuadVS.tbl"
#include "FullQuadPS.h"
#include "FullQuadPS.tbl"
#endif // SUPPORT_FULLQUAD
#include "NormalPS.h"
#include "NormalPS.tbl"
}
#include "Rendering\SceneGraph\SceneGraph.h"
#include <algorithm>

namespace Caustic
{
    //**********************************************************************
    // \brief s_defaultVSLayout defines the default layout for our default vertex
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_defaultVSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    // \brief s_lineVSLayout defines the default layout for our line vertex
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_lineVSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    // \brief s_InfinitePlaneVSLayout defines the default layout for our infinite plane
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_InfinitePlaneVSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    // \brief s_drawNormalVSLayout defines the default layout for our line vertex
    //**********************************************************************
    D3D11_INPUT_ELEMENT_DESC s_drawNormalVSLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //**********************************************************************
    // \brief CRenderer ctor
    //**********************************************************************
    CRenderer::CRenderer() :
        m_waitForShutdown(false, true),
        m_exitThread(false)
    {
    }

    //**********************************************************************
    CRenderer::~CRenderer()
    {
        m_exitThread = true;
        m_waitForShutdown.Wait(INFINITE);
    }

    //**********************************************************************
    // \brief Setup is called at the start of the application to initialize
    // the server side of our renderer
    // \param[in] hwnd HWND to use for drawing
    //**********************************************************************
    void CRenderer::InitializeD3D(HWND hwnd)
    {
        CGraphicsBase::InitializeD3D(hwnd);

        // Create texture for rendering object IDs
        CD3D11_TEXTURE2D_DESC texObjID(DXGI_FORMAT_R32_UINT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        CT(m_spDevice->CreateTexture2D(&texObjID, NULL, &m_spObjIDTexture));
        D3D11_RENDER_TARGET_VIEW_DESC objIDRVDesc;
        objIDRVDesc.Format = texObjID.Format;
        objIDRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        objIDRVDesc.Texture2D.MipSlice = 0;
        CT(m_spDevice->CreateRenderTargetView(m_spObjIDTexture, &objIDRVDesc, &m_spObjIDRTView));
    }

    //**********************************************************************
    // \brief Setup is called at the start of the application to initialize
    // the server side of our renderer
    // \param[in] hwnd HWND to use for drawing
    //**********************************************************************
    void CRenderer::Setup(HWND hwnd, bool createDebugDevice)
    {
        CGraphicsBase::Setup(hwnd, createDebugDevice);

        // Create our default shaders
        CRefObj<IShader> spShader;
        CreateShader(this, L"Default",
            g_DefaultPS_ParamTable, _countof(g_DefaultPS_ParamTable),
            g_DefaultVS_ParamTable, _countof(g_DefaultVS_ParamTable),
            g_DefaultPS, sizeof(g_DefaultPS),
            g_DefaultVS, sizeof(g_DefaultVS),
            s_defaultVSLayout, _countof(s_defaultVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"Default", spShader.p);

#ifdef SUPPORT_SHADOW_MAPPING
        spShader = nullptr;
        CreateShader(this, L"ShadowMap",
            g_ShadowMapPS_ParamTable, _countof(g_ShadowMapPS_ParamTable),
            g_ShadowMapVS_ParamTable, _countof(g_ShadowMapVS_ParamTable),
            g_ShadowMapPS, sizeof(g_ShadowMapPS),
            g_ShadowMapVS, sizeof(g_ShadowMapVS),
            s_ShadowMapVSLayout, _countof(s_ShadowMapVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"ShadowMap", spShader.p);
#endif // SUPPORT_SHADOW_MAPPING

        spShader = nullptr;
        CreateShader(this, L"DrawNormal",
            g_DrawNormalPS_ParamTable, _countof(g_DrawNormalPS_ParamTable),
            g_DrawNormalVS_ParamTable, _countof(g_DrawNormalVS_ParamTable),
            g_DrawNormalPS, sizeof(g_DrawNormalPS),
            g_DrawNormalVS, sizeof(g_DrawNormalVS),
            s_drawNormalVSLayout, _countof(s_drawNormalVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"DrawNormal", spShader.p);

        spShader = nullptr;
        CreateShader(this, L"ColorNormal",
            g_ColorNormalPS_ParamTable, _countof(g_ColorNormalPS_ParamTable),
            g_DefaultVS_ParamTable, _countof(g_DefaultVS_ParamTable),
            g_ColorNormalPS, sizeof(g_ColorNormalPS),
            g_DefaultVS, sizeof(g_DefaultVS),
            s_defaultVSLayout, _countof(s_defaultVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"ColorNormal", spShader.p);

        spShader = nullptr;
        CreateShader(this, L"ColorUVs",
            g_ColorUVsPS_ParamTable, _countof(g_ColorUVsPS_ParamTable),
            g_DefaultVS_ParamTable, _countof(g_DefaultVS_ParamTable),
            g_ColorUVsPS, sizeof(g_ColorUVsPS),
            g_DefaultVS, sizeof(g_DefaultVS),
            s_defaultVSLayout, _countof(s_defaultVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"ColorUVs", spShader.p);

        spShader = nullptr;
        CreateShader(this, L"Textured",
            g_TexturedPS_ParamTable, _countof(g_TexturedPS_ParamTable),
            g_DefaultVS_ParamTable, _countof(g_DefaultVS_ParamTable),
            g_TexturedPS, sizeof(g_TexturedPS),
            g_DefaultVS, sizeof(g_DefaultVS),
            s_defaultVSLayout, _countof(s_defaultVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"Textured", spShader.p);

        spShader = nullptr;
        CreateShader(this, L"Normal",
            g_NormalPS_ParamTable, _countof(g_NormalPS_ParamTable),
            g_DefaultVS_ParamTable, _countof(g_DefaultVS_ParamTable),
            g_NormalPS, sizeof(g_NormalPS),
            g_DefaultVS, sizeof(g_DefaultVS),
            s_defaultVSLayout, _countof(s_defaultVSLayout), &spShader);
        CShaderMgr::GetInstance()->RegisterShader(L"Normal", spShader.p);

        //**********************************************************************
        // Create vertex buffer used to draw lines
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(SVertex_3) * 2, D3D11_BIND_VERTEX_BUFFER);
            SVertex_3 *pVertexBuffer = new SVertex_3[2];
            pVertexBuffer[0].m_pos[0] = 0.0f;
            pVertexBuffer[0].m_pos[1] = 0.0f;
            pVertexBuffer[0].m_pos[2] = 0.0f;
            pVertexBuffer[1].m_pos[0] = 1.0f;
            pVertexBuffer[1].m_pos[1] = 1.0f;
            pVertexBuffer[1].m_pos[2] = 1.0f;
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = pVertexBuffer;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spLineVB));
        }

        // Get Shader for lines
        CreateShader(this, L"Line",
            g_LinePS_ParamTable, _countof(g_LinePS_ParamTable),
            g_LineVS_ParamTable, _countof(g_LineVS_ParamTable),
            g_LinePS, sizeof(g_LinePS),
            g_LineVS, sizeof(g_LineVS),
            s_lineVSLayout, _countof(s_lineVSLayout), &m_spLineShader);
        CShaderMgr::GetInstance()->RegisterShader(L"Line", m_spLineShader.p);

        //**********************************************************************
        // Create vertex buffer used to draw infinite plane
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(SVertex_5) * 5, D3D11_BIND_VERTEX_BUFFER);
            SVertex_5 planePts[5] = {
                { 0.0f, 0.0f, 0.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f, 0.0f },
                { -1.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, -1.0f, 0.0f },
            };
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = planePts;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spInfinitePlaneVB));
        }

        //**********************************************************************
        // Create index buffer used to draw infinite plane
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(uint32) * 12, D3D11_BIND_INDEX_BUFFER);
            uint32 planeIndices[4][3] = {
                { 0, 1, 2 },
                { 0, 2, 3 },
                { 0, 3, 4 },
                { 0, 4, 1 }
            };
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = planeIndices;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spInfinitePlaneIB));
        }

        // Get Shader for plane
//        CreateShader(this, L"Plane",
//            g_InfinitePlanePS_ParamTable, _countof(g_InfinitePlanePS_ParamTable),
//            g_InfinitePlaneVS_ParamTable, _countof(g_InfinitePlaneVS_ParamTable),
//            g_InfinitePlanePS, sizeof(g_InfinitePlanePS),
//            g_InfinitePlaneVS, sizeof(g_InfinitePlaneVS),
//            s_InfinitePlaneVSLayout, _countof(s_InfinitePlaneVSLayout), &m_spInfinitePlaneShader);
//        CShaderMgr::GetInstance()->RegisterShader(L"Plane", m_spInfinitePlaneShader.p);

#ifdef SUPPORT_FULLQUAD
        //**********************************************************************
        // Create vertex buffer used to draw fullscreen quad
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC vbdesc(sizeof(SVertex_5) * 4, D3D11_BIND_VERTEX_BUFFER);
            SVertex_5 quadPts[5] = {
                { -1.0f, -1.0f, 0.9f, 1.0f },
                { -1.0f, +1.0f, 0.9f, 1.0f },
                { +1.0f, +1.0f, 0.9f, 1.0f },
                { +1.0f, -1.0f, 0.9f, 1.0f },
            };
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = quadPts;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&vbdesc, &data, &m_spFullQuadVB));

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
            CT(m_spDevice->CreateBuffer(&ibdesc, &data, &m_spFullQuadIB));

            // Get Shader for plane
            CreateShader(this, L"FullQuad",
                g_FullQuadPS_ParamTable, _countof(g_FullQuadPS_ParamTable),
                g_FullQuadVS_ParamTable, _countof(g_FullQuadVS_ParamTable),
                g_FullQuadPS, sizeof(g_FullQuadPS),
                g_FullQuadVS, sizeof(g_FullQuadVS),
                s_InfinitePlaneVSLayout, _countof(s_InfinitePlaneVSLayout), &m_spFullQuadShader);
            CShaderMgr::GetInstance()->RegisterShader(L"FullQuad", m_spFullQuadShader.p);
        }
#endif // SUPPORT_FULLQUAD
    }

    //**********************************************************************
    // \brief DrawMesh draws a single mesh
    // \param[in] pMesh Mesh to render
    // \param[in] pMaterial Material definition for mesh (maybe nullptr)
    // \param[in] pTexture Texture to use when rendering (maybe nullptr)
    // \param[in] pShader Shader to use when rendering (maybe nullptr)
    // \param[in] mat Transformation matrix to apply to mesh
    //
    // CRendererServer::DrawMesh() draws the specified mesh.
    //**********************************************************************
    void CRenderer::DrawMesh(ISubMesh *pSubMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat)
    {
        CRefObj<IRenderMaterial> spFrontMaterial;
		CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader, &spFrontMaterial);
        spFrontMaterial->SetDiffuseTexture(this, pTexture);
        CRefObj<IRenderMaterial> spBackMaterial;
        if (pSubMesh->GetMeshFlags() & EMeshFlags::TwoSided)
        {
			CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader, &spBackMaterial);
            spBackMaterial->SetDiffuseTexture(this, pTexture);
        }
        CRenderable renderable(this, pSubMesh, spFrontMaterial.p, spBackMaterial.p, mat);
        m_singleObjs.push_back(renderable);
    }

    //**********************************************************************
    void CRenderer::AddPointLight(IPointLight *pLight)
    {
        m_lights.push_back(CRefObj<IPointLight>(pLight));
    }

    //**********************************************************************
    void CRenderer::GetRenderCtx(IRenderCtx **ppCtx)
    {
        (*ppCtx) = m_spRenderCtx.p;
        if (m_spRenderCtx.p)
            (*ppCtx)->AddRef();
    }

    void CRenderer::DrawInfinitePlane()
    {
#ifdef SUPPORT_FULLQUAD
        UINT offset = 0;
        UINT vertexSize = sizeof(SVertex_5);
        ID3D11DeviceContext *pContext = GetContext();
        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = D3D11_CULL_NONE;
        rasDesc.DepthClipEnable = TRUE;
        rasDesc.FillMode = D3D11_FILL_SOLID;
        m_spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        pContext->RSSetState(spRasterState);
        pContext->IASetVertexBuffers(0, 1, &m_spFullQuadVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_spFullQuadIB.p, DXGI_FORMAT_R32_UINT, 0);
        Vector3 eye;
        GetCamera()->GetPosition(&eye, nullptr, nullptr, nullptr, nullptr, nullptr);
        Float3 vEye(eye.x, eye.y, eye.z);
        m_spFullQuadShader->SetPSParam(L"eye", std::any(vEye));
        m_spFullQuadShader->BeginRender(this);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(6, 0, 0);
        m_spFullQuadShader->EndRender(this);
#endif // SUPPORT_FULLQUAD
    }

    void CRenderer::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        UINT offset = 0;
        UINT vertexSize = sizeof(SVertex_3);
        ID3D11DeviceContext *pContext = GetContext();
        pContext->IASetVertexBuffers(0, 1, &m_spLineVB.p, &vertexSize, &offset);
        Matrix m;
        m.x[0] = p2.x - p1.x;    m.x[4] = 0.0f;            m.x[8] = 0.0f;            m.x[12] = 0.0f;
        m.x[1] = 0.0f;            m.x[5] = p2.y - p1.y;    m.x[9] = 0.0f;            m.x[13] = 0.0f;
        m.x[2] = 0.0f;            m.x[6] = 0.0f;            m.x[10] = p2.z - p1.z;    m.x[14] = 0.0f;
        m.x[3] = p1.x;            m.x[7] = p1.y;            m.x[11] = p1.z;            m.x[15] = 1.0f;
        m_spLineShader->SetVSParam(L"endpoints", std::any(m));
        Float4 color(clr.x, clr.y, clr.z, clr.w);
        m_spLineShader->SetPSParam(L"color", std::any(color));
        m_spLineShader->BeginRender(this);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->Draw(2, 0);
        m_spLineShader->EndRender(this);
    }

    void CRenderer::SetSceneGraph(ISceneGraph *pSceneGraph)
    {
        m_spSceneGraph = pSceneGraph;
    }

    void CRenderer::GetGraphics(IGraphics **ppGraphics)
    {
        *ppGraphics = this;
        (*ppGraphics)->AddRef();
    }

    void CRenderer::DrawSceneObjects(int pass)
    {
        if (m_spSceneGraph.p)
        {
            SceneCtx sceneCtx;
            sceneCtx.m_CurrentPass = pass;
            m_spSceneGraph->Render(this, m_spRenderCtx.p, &sceneCtx);
        }

        // Render any single objects
        for (size_t i = 0; i < m_singleObjs.size(); i++)
        {
            if (m_singleObjs[i].m_passes & (1 << pass))
                m_singleObjs[i].Render(this, m_lights, m_spRenderCtx.p);
        }
    }

    void CRenderer::RenderScene()
    {
        DrawInfinitePlane();
        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWorldAxis)
        {
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 10.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 10.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayLightDir &&
            m_lights.size() > 0)
        {
            for (size_t i = 0; i < m_lights.size(); i++)
                DrawLine(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        for (uint32 pass = c_PassFirst; pass <= c_PassLast; pass++)
        {
#ifdef SUPPORT_OBJID
            CRenderCtx *pCtx = (CRenderCtx*)m_spRenderCtx.p;
            pCtx->m_currentPass = pass;
            pCtx->m_passBlendable = true;
            if (pass == c_PassObjID)
            {
                pCtx->m_passBlendable = false;
                // Setup render target
                m_spContext->OMSetRenderTargets(1, &m_spObjIDRTView.p, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
#ifdef SUPPORT_SHADOW_MAPPING
            if (pass == c_PassShadow)
            {
                int numShadowPasses = (m_lights.size() < c_MaxShadowMaps) ? m_lights.size() : c_MaxShadowMaps;
                for (int i = 0; i < numShadowPasses; i++)
                {
                    m_spContext->OMSetRenderTargets(1, &m_spShadowRTView[i].p, m_spStencilView);
                    FLOAT bgClr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                    m_spContext->ClearRenderTargetView(m_spShadowRTView[i], bgClr);
                    m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
                    // Reset the camera to be from the lights perspective
                    CRefObj<ICamera> spCamera;
                    CreateCamera(true, &spCamera);
                    spCamera->SetPosition(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
                    this->SetCamera(spCamera.p);
                    DrawSceneObjects(pass);
                    // Restore default render targets
                    m_spContext->OMSetRenderTargets(1, &m_spRTView.p, m_spStencilView);
                }
            }
            else
#endif // SUPPORT_SHADOW_MAPPING
            if (pass == c_PassTransparent)
            {
                std::vector<int> order;
                order.resize(m_singleObjs.size());
                std::sort(m_singleObjs.begin(), m_singleObjs.end(),
                    [&](CRenderable &left, CRenderable &right)->bool
                    {
                        Vector3 cameraPos;
                        GetCamera()->GetPosition(&cameraPos, nullptr, nullptr, nullptr, nullptr, nullptr);
                        float dist1 = (left.GetPos() - cameraPos).Length();
                        float dist2 = (right.GetPos() - cameraPos).Length();
                        if (dist1 < dist2)
                            return true;
                        return false;
                    }
                );
                DrawSceneObjects(pass);
            }

#ifdef SUPPORT_OBJID
            if (pass == c_PassObjID)
            {
                // Restore default render targets
                m_spContext->OMSetRenderTargets(1, &m_spRTView.p, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
        }
    }

    //**********************************************************************
    // \brief RenderFrame is typically called from the render loop to render
    // the next frame.
    //**********************************************************************
    void CRenderer::RenderFrame()
    {
        ID3D11RenderTargetView *pView = m_spRTView;
        m_spContext->OMSetRenderTargets(1, &pView, nullptr);

        FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
        m_spContext->ClearRenderTargetView(pView, bgClr);
        m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        CD3D11_DEPTH_STENCIL_DESC depthDesc(D3D11_DEFAULT);
        depthDesc.DepthEnable = true;
        depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
        CComPtr<ID3D11DepthStencilState> spDepthStencilState;
        CT(m_spDevice->CreateDepthStencilState(&depthDesc, &spDepthStencilState));
        m_spContext->OMSetDepthStencilState(spDepthStencilState, 1);

        m_spContext->OMSetRenderTargets(1, &pView, m_spStencilView);

        RenderScene();
        m_spSwapChain->Present(1, 0);
    }

    //**********************************************************************
    // \brief RenderLoop is our main rendering loop
    //**********************************************************************
    void CRenderer::RenderLoop()
    {
        while (!m_exitThread)
        {
            RenderFrame();
        }
        m_waitForShutdown.Set();
    }

    CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics)
    {
        _ASSERT(ppGraphics);
        std::unique_ptr<CGraphics> spGraphics(new CGraphics());
        spGraphics->Setup(hwnd, true);

        CRefObj<ICamera> spCamera;
        CCausticFactory::Instance()->CreateCamera(true, &spCamera);
        spGraphics->SetCamera(spCamera.p);

        *ppGraphics = spGraphics.release();
        (*ppGraphics)->AddRef();
    }

    CAUSTICAPI void CreateRenderer(HWND hwnd, IRenderer **ppRenderer)
    {
        _ASSERT(ppRenderer);
        std::unique_ptr<CRenderer> spRenderer(new CRenderer());
        spRenderer->Setup(hwnd, true);

        CRefObj<ICamera> spCamera;
		CCausticFactory::Instance()->CreateCamera(true, &spCamera);
        spRenderer->SetCamera(spCamera.p);

        *ppRenderer = spRenderer.release();
        (*ppRenderer)->AddRef();
    }

    //**********************************************************************
    // \brief SetCamera assigns a camera to the graphics device
    // \param[in] pCamera Camera for renderer to use
    //**********************************************************************
    void CGraphicsBase::SetCamera(ICamera *pCamera)
    {
        m_spCamera = pCamera;
    }

    //**********************************************************************
    // \brief Setup is called at the start of the application to initialize
    // the server side of our renderer
    // \param[in] hwnd HWND to use for drawing
    //**********************************************************************
    void CGraphicsBase::InitializeD3D(HWND hwnd)
    {
        std::unique_ptr<CRenderCtx> spCtx(new CRenderCtx());
        m_spRenderCtx = spCtx.release();

        CComPtr<ID3D11Texture2D> m_spBackBuffer;
        CT(m_spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_spBackBuffer)));
        D3D11_TEXTURE2D_DESC m_BBDesc;
        m_spBackBuffer->GetDesc(&m_BBDesc);
        CT(m_spDevice->CreateRenderTargetView(m_spBackBuffer, nullptr, &m_spRTView));

#ifdef SUPPORT_SHADOW_MAPPING
        // Create texture for rendering shadow map
        for (int i = 0; i < c_MaxShadowMaps; i++)
        {
            CD3D11_TEXTURE2D_DESC texObjID(DXGI_FORMAT_R32_FLOAT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&texObjID, NULL, &m_spShadowTexture[i]));
            D3D11_RENDER_TARGET_VIEW_DESC rtdesc;
            rtdesc.Format = texObjID.Format;
            rtdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtdesc.Texture2D.MipSlice = 0;
            CT(m_spDevice->CreateRenderTargetView(m_spShadowTexture[i], &rtdesc, &m_spShadowRTView[i]));
        }
#endif // SUPPORT_SHADOW_MAPPING

        // Create depth buffer
        CD3D11_TEXTURE2D_DESC texDesc2D(DXGI_FORMAT_D24_UNORM_S8_UINT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
        CT(m_spDevice->CreateTexture2D(&texDesc2D, NULL, &m_spDepthStencilBuffer));

        // Create the depth buffer ressource view
        CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
        CT(m_spDevice->CreateDepthStencilView(m_spDepthStencilBuffer, &stencilDesc, &m_spStencilView));

        RECT rect;
        ::GetClientRect(hwnd, &rect);
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(viewport));
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)(rect.right - rect.left);
        viewport.Height = (float)(rect.bottom - rect.top);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        m_spContext->RSSetViewports(1, &viewport);
    }

    //**********************************************************************
    // \brief Setup is called at the start of the application to initialize
    // the server side of our renderer
    // \param[in] hwnd HWND to use for drawing
    //**********************************************************************
    void CGraphicsBase::Setup(HWND hwnd, bool createDebugDevice)
    {
        DXGI_SWAP_CHAIN_DESC desc = { 0 };
        desc.BufferDesc.Width = 0;
        desc.BufferDesc.Height = 0;
        desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BufferDesc.RefreshRate.Numerator = 1;
        desc.BufferDesc.RefreshRate.Denominator = 30;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2;
        desc.OutputWindow = hwnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.Flags = 0;

        CT(D3D11CreateDeviceAndSwapChain(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // software module
            D3D11_CREATE_DEVICE_DEBUG, // flags
            nullptr, // pFeatureLevels
            0, // numFeatureLevels
            D3D11_SDK_VERSION,
            &desc, &m_spSwapChain, &m_spDevice, &m_featureLevel, &m_spContext));

        InitializeD3D(hwnd);

        // Create a default camera
		CCausticFactory::Instance()->CreateCamera(false, &m_spCamera);
    }
}
