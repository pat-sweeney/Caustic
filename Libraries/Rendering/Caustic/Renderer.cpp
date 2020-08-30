//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Texture.h"
#include "Renderer.h"
#include "Renderable.h"
#include "ShaderInfo.h"
#include <vector>
#include <any>
#include "Base\Core\CritSec.h"
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <algorithm>

//**********************************************************************
// File: Renderer.cpp
// Contains code for the main renderer
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Constructor: CRenderer
    //**********************************************************************
    CRenderer::CRenderer() :
        m_waitForShutdown(true, true),
        m_exitThread(false)
    {
    }

    //**********************************************************************
    // Destructor: ~CRenderer
    //**********************************************************************
    CRenderer::~CRenderer()
    {
        m_exitThread = true;
        m_waitForShutdown.Wait(INFINITE);
    }

    //**********************************************************************
    // Method: InitializeD3D
    // Initializes the server side renderer. Clients should call this method indirectly (via <IRenderer::Setup>)
    // at application startup.
    //
    // Parameters:
    // hwnd - HWND to use for drawing
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
    // Method: Setup
    // See <IRenderer::Setup>.
    //**********************************************************************
    void CRenderer::Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice)
    {
        m_renderThreadId = GetCurrentThreadId();
        CGraphicsBase::Setup(hwnd, createDebugDevice);

        if (shaderFolder.empty())
            shaderFolder = std::wstring(DEFAULT_SHADER_PATH);
        m_spShaderMgr = CRefObj<IShaderMgr>(new CShaderMgr());
        LoadDefaultShaders(shaderFolder.c_str());

        m_spLineShader = m_spShaderMgr->FindShader(L"Line");

        //**********************************************************************
        // Create vertex buffer used to draw lines
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(CLineVertex) * 2, D3D11_BIND_VERTEX_BUFFER);
            CLineVertex *pVertexBuffer = new CLineVertex[2];
            pVertexBuffer[0].x = 0.0f;
            pVertexBuffer[0].y = 0.0f;
            pVertexBuffer[0].z = 0.0f;
            pVertexBuffer[1].x = 1.0f;
            pVertexBuffer[1].y = 1.0f;
            pVertexBuffer[1].z = 1.0f;
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = pVertexBuffer;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spLineVB));
        }
    }

    //**********************************************************************
    // Method: LoadShaderBlob
    // Loads the shader into a blob from the specified file.
    //
    // Parameters:
    // filename - Name of file to load from
    // ppBlob - Returns the created shader blob
    //**********************************************************************
    void CRenderer::LoadShaderBlob(std::wstring &filename, ID3DBlob **ppBlob)
    {
        HANDLE f = ::CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
        if (f != INVALID_HANDLE_VALUE)
        {
            DWORD dwSize = GetFileSize(f, nullptr);
            CT(D3DCreateBlob(dwSize, ppBlob));
            DWORD bytesRead;
            ReadFile(f, (*ppBlob)->GetBufferPointer(), dwSize, &bytesRead, nullptr);
            CloseHandle(f);
        }
    }

    //**********************************************************************
    // Method: LoadShaderInfo
    // Loads the shader info (*.shi) file (describes the layout and parameters of a given shader)
    //
    // Parameters:
    // filename - Name of shader info file (*.shi)
    //
    // Returns:
    // Returns the new shader info object
    //**********************************************************************
    CRefObj<IShaderInfo> CRenderer::LoadShaderInfo(std::wstring &filename)
    {
        return CCausticFactory::Instance()->CreateShaderInfo(filename.c_str());
    }

    //**********************************************************************
    // Method: LoadDefaultShaders
    // Loads all the shaders found in the specified folder.
    //
    // Parameters:
    // pFolder - path to shaders to load
    //**********************************************************************
    void CRenderer::LoadDefaultShaders(const wchar_t *pFolder)
    {
        WIN32_FIND_DATA findData;
        std::wstring fn(pFolder);
        fn += L"\\*.shi";
        HANDLE h = ::FindFirstFile(fn.c_str(), &findData);
        if (h == INVALID_HANDLE_VALUE)
            CT(E_FAIL);
        while (true)
        {
            std::wstring fn(findData.cFileName);
            std::size_t found = fn.rfind(L".shi");
            if (found != std::wstring::npos)
            {
                CComPtr<ID3DBlob> spPixelShaderBlob;
                CComPtr<ID3DBlob> spVertexShaderBlob;
                CComPtr<ID3DBlob> spComputeShaderBlob;
                CRefObj<IShaderInfo> spShaderInfo;
                CRefObj<IShader> spShader;
                std::wstring shaderName(fn.substr(0, found));
                spShaderInfo = LoadShaderInfo(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L".shi");
                if (spShaderInfo->HasShader(EShaderType::TypePixelShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_PS.cso", &spPixelShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeVertexShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_VS.cso", &spVertexShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeComputeShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_CS.cso", &spComputeShaderBlob);
                spShader = CCausticFactory::Instance()->CreateShader(this, shaderName.c_str(), spVertexShaderBlob, spPixelShaderBlob, spComputeShaderBlob, spShaderInfo);
                m_spShaderMgr->RegisterShader(shaderName.c_str(), spShader);
            }
            if (!::FindNextFile(h, &findData))
                break;
        }
    }

    //**********************************************************************
    // Method: DrawMesh
    // See <IRenderer::DrawMesh>
    //**********************************************************************
    void CRenderer::DrawMesh(IRenderSubMesh *pSubMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat)
    {
        CRefObj<IRenderMaterial> spFrontMaterial;
        spFrontMaterial = CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader);
        spFrontMaterial->SetTexture(this, L"diffuseTexture", pTexture, EShaderAccess::PixelShader);
        CRefObj<IRenderMaterial> spBackMaterial;
        if (pSubMesh->GetMeshFlags() & EMeshFlags::TwoSided)
        {
            spBackMaterial = CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader);
            spBackMaterial->SetTexture(this, L"diffuseTexture", pTexture, EShaderAccess::PixelShader);
        }
        CRenderable renderable(pSubMesh, spFrontMaterial, spBackMaterial, mat);
        m_singleObjs.push_back(renderable);
    }

    //**********************************************************************
    // Method: AddPointLight
    // See <IRenderer::AddPointLight>
    //**********************************************************************
    void CRenderer::AddPointLight(IPointLight *pLight)
    {
        CHECKTHREAD;
        m_lights.push_back(CRefObj<ILight>(pLight));
    }

    //**********************************************************************
    // Method: GetRenderCtx
    // See <IRenderer::GetRenderCtx>
    //**********************************************************************
    CRefObj<IRenderCtx> CRenderer::GetRenderCtx()
    {
        CHECKTHREAD;
        return CRefObj<IRenderCtx>(m_spRenderCtx);
    }

    //**********************************************************************
    // Method: DrawLine
    // See <IRenderer::DrawLine>
    //**********************************************************************
    void CRenderer::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        CHECKTHREAD;
        UINT offset = 0;
        UINT vertexSize = sizeof(CLineVertex);
        ID3D11DeviceContext *pContext = GetContext();
        pContext->IASetVertexBuffers(0, 1, &m_spLineVB.p, &vertexSize, &offset);
        Matrix m;
        m.x[0] = p2.x - p1.x;    m.x[4] = 0.0f;           m.x[8] = 0.0f;           m.x[12] = 0.0f;
        m.x[1] = 0.0f;           m.x[5] = p2.y - p1.y;    m.x[9] = 0.0f;           m.x[13] = 0.0f;
        m.x[2] = 0.0f;           m.x[6] = 0.0f;           m.x[10] = p2.z - p1.z;   m.x[14] = 0.0f;
        m.x[3] = p1.x;           m.x[7] = p1.y;           m.x[11] = p1.z;          m.x[15] = 1.0f;
        m_spLineShader->SetVSParam(L"endpoints", std::any(m));
        Float4 color(clr.x, clr.y, clr.z, clr.w);
        m_spLineShader->SetPSParam(L"color", std::any(color));
        std::vector<CRefObj<ILight>> lights;
        m_spLineShader->BeginRender(this, nullptr, nullptr, lights, nullptr);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->Draw(2, 0);
        m_spLineShader->EndRender(this);
    }

    //**********************************************************************
    // Method: DrawSceneObjects
    // First calls the renderCallback provided. Then renders each Renderable.
    //
    // Parameters:
    // pass - which pass are we rendering
    // renderCallback - Render callback
    //**********************************************************************
    void CRenderer::DrawSceneObjects(int pass, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        if (renderCallback)
            (renderCallback)(this, m_spRenderCtx, pass);

        // Render any single objects
        if (pass == c_PassShadow)
        {
            int totalLights = 0; // We will only take the first 16 lights
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                if (totalLights >= c_MaxLights)
                    break;
                if (m_lights[i]->GetType() != ELightType::DirectionalLight)
                    continue; // For the moment skip all light types except directional

                if (m_lights[i]->GetCastsShadows())
                {
                    PushShadowmapRT(c_HiResShadowMap, totalLights++, m_lights[i]->GetPosition(), m_lights[i]->GetDirection());
                    for (size_t i = 0; i < m_singleObjs.size(); i++)
                    {
                        if (m_singleObjs[i].m_passes & (1 << pass))
                            m_singleObjs[i].Render(this, m_lights, m_spRenderCtx);
                    }
                    PopShadowmapRT();
                }
            }
        }
        else
        {
            for (size_t i = 0; i < m_singleObjs.size(); i++)
            {
                if (m_singleObjs[i].m_passes & (1 << pass))
                    m_singleObjs[i].Render(this, m_lights, m_spRenderCtx);
            }
        }
    }
    
    //**********************************************************************
    CRefObj<ITexture> CRenderer::GetShadowmapTexture(int whichShadowMap)
    {
        return CRefObj<ITexture>(new CTexture(m_spShadowTexture[whichShadowMap], m_spShadowSRView[whichShadowMap]));
    }

    //**********************************************************************
    void CRenderer::SetShadowmapViewport(int whichShadowMap, int lightMapIndex)
    {
        int lx = lightMapIndex % 4;
        int ly = lightMapIndex / 4;
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        m_viewport.TopLeftX = float(lx * m_shadowMapWidth[whichShadowMap] / 4);
        m_viewport.TopLeftY = float(ly * m_shadowMapHeight[whichShadowMap] / 4);
        m_viewport.Width = (float)(m_shadowMapWidth[whichShadowMap] / 4);
        m_viewport.Height = (float)(m_shadowMapHeight[whichShadowMap] / 4);
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_spContext->RSSetViewports(1, &m_viewport);
    }

    //**********************************************************************
    // The total size of our shadow map is 8192x8192. This texture is further
    // divided into 3 sections of hires, midres, and lowres shadow maps.
    // There are 8 hi-res maps, 
    //      2048         1024    1024
    // +---------------+-------+-------+
    // |               |       |       |
    // |               |   2   |   3   |
    // |               |       |       |
    // |       1       +-------+-------+
    // |               |       |       |
    // |               |   4   |   5   |
    // |               |       |       |
    // +-------+-------+-------+---+---+
    // |       |       |   |   |   |   |
    // |   6   |   7   +---+---+---+---+
    // |       |       |   |   |   |   |
    // |-------+-------+---+---+---+---+
    // |       |       |   |   |   |   |
    // |   8   |   9   |---+---+---+---+
    // |       |       |   |   |   |   |
    // +-------+-------+---+---+---+---+
    void CRenderer::SelectShadowmap(int whichShadowMap, int lightMapIndex, IShader *pShader)
    {
        pShader->SetPSParam(L"shadowMapTexture", std::any(m_spShadowTexture[whichShadowMap]));
        int lx = lightMapIndex % 4;
        int ly = lightMapIndex / 4;
        Float4 bounds(
            float(lx * m_shadowMapWidth[whichShadowMap] / 4),
            float(ly * m_shadowMapHeight[whichShadowMap] / 4),
            (float)(m_shadowMapWidth[whichShadowMap] / 4),
            (float)(m_shadowMapHeight[whichShadowMap] / 4)
            );
        pShader->SetPSParam(L"shadowMapBounds", std::any(bounds));
    }

    //**********************************************************************
    void CRenderer::PushShadowmapRT(int whichShadowMap, int lightMapIndex, Vector3& lightPos, Vector3 &lightDir)
    {
        ShadowMapRenderState rs;
        m_spContext->OMGetRenderTargets(1, &rs.m_spOldRT, &rs.m_spOldStencil);
        rs.m_spOldCamera = m_spCamera;
        rs.m_viewport = m_viewport;
        m_cameras.push(rs);
        m_spContext->OMSetRenderTargets(0, nullptr, m_spShadowMapStencilView[whichShadowMap].p);
        FLOAT bgClr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_spContext->ClearDepthStencilView(m_spShadowMapStencilView[whichShadowMap], D3D11_CLEAR_DEPTH, 1.0f, 0);

        // Reset the camera to be from the lights perspective
        CRefObj<ICamera> spCamera = CreateCamera(true);
        spCamera->SetPosition(lightPos, lightDir, Vector3(0.0f, 1.0f, 0.0f));
        this->SetCamera(spCamera);
        SetShadowmapViewport(whichShadowMap, lightMapIndex);
    }
    
    void CRenderer::PopShadowmapRT()
    {
        // Restore default render targets
        ShadowMapRenderState rs = m_cameras.top();
        this->SetCamera(rs.m_spOldCamera);
        m_viewport = rs.m_viewport;
        m_spContext->RSSetViewports(1, &m_viewport);
        m_cameras.pop();
        m_spContext->OMSetRenderTargets(1, &rs.m_spOldRT.p, rs.m_spOldStencil);
    }
    
    //**********************************************************************
    // Method: RenderScene
    // Renders current scene (both scene graph and any renderables currently
    // attached to the renderer)
    //**********************************************************************
    void CRenderer::RenderScene(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
#ifdef _DEBUG
        m_spRenderCtx->SetDebugFlags(RenderCtxFlags::c_DisplayWorldAxis);
#endif
        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWorldAxis)
        {
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 100.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 100.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayLightDir &&
            m_lights.size() > 0)
        {
            for (size_t i = 0; i < m_lights.size(); i++)
                DrawLine(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        for (uint32 pass = c_PassFirst; pass <= c_PassLast; pass++)
        {
            CRenderCtx *pCtx = (CRenderCtx*)m_spRenderCtx.p;
#ifdef _DEBUG
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
#endif
            pCtx->m_currentPass = pass;
            pCtx->m_passBlendable = true;
            if (pass == c_PassOpaque)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"OpaquePass", 0);
#endif
                DrawSceneObjects(pass, renderCallback);
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }
#ifdef SUPPORT_OBJID
            else if (pass == c_PassObjID)
            {
                pCtx->m_passBlendable = false;
                // Setup render target
                m_spContext->OMSetRenderTargets(1, &m_spObjIDRTView, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
            else if (pass == c_PassShadow)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"ShadowPass", 0);
#endif
                DrawSceneObjects(pass, renderCallback);
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }
            else if (pass == c_PassTransparent)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"TransparentPass", 0);
#endif
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
                DrawSceneObjects(pass, renderCallback);
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }

#ifdef SUPPORT_OBJID
            if (pass == c_PassObjID)
            {
                // Restore default render targets
                m_spContext->OMSetRenderTargets(1, &m_spRTView, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
        }
    }

    //**********************************************************************
    // Method: RenderFrame
    // See <IRenderer::RenderFrame>
    //**********************************************************************
    void CRenderer::RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
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

        RenderScene(renderCallback);
        m_spSwapChain->Present(1, 0);
    }

    //**********************************************************************
    // Method: RenderLoop
    // See <IRenderer::RenderLoop>
    //**********************************************************************
    void CRenderer::RenderLoop(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        m_waitForShutdown.Clear();
        while (!m_exitThread)
        {
            RenderFrame(renderCallback);
        }
        m_waitForShutdown.Set();
    }

    //**********************************************************************
    // Function: CreateRenderer
    // Creates a renderer
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // shaderFolder - path to directory containing shaders
    //
    // Returns:
    // Returns the created renderer
    //**********************************************************************
    CAUSTICAPI CRefObj<IRenderer> CreateRenderer(HWND hwnd, std::wstring &shaderFolder)
    {
        std::unique_ptr<CRenderer> spRenderer(new CRenderer());
        spRenderer->Setup(hwnd, shaderFolder, true);

        CRefObj<ICamera> spCamera = CCausticFactory::Instance()->CreateCamera(true);
        spRenderer->SetCamera(spCamera);
        return CRefObj<IRenderer>(spRenderer.release());
    }

    //**********************************************************************
    // Method: SetCamera
    // See <IRenderer::SetCamera>
    //**********************************************************************
    void CGraphicsBase::SetCamera(ICamera* pCamera)
    {
        m_spCamera = pCamera;
    }

    //**********************************************************************
    // Method: InitializeD3D
    // Called at the start of the application to initialize the server side of our renderer.
    //
    // Parameters:
    // hwnd - HWND to use for drawing
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

        // Create texture for rendering shadow map
        for (int i = 0; i < c_MaxShadowMaps; i++)
        {
            int shadowMapWidth = 8192;
            int shadowMapHeight = 8192;
            switch (i)
            {
            case c_HiResShadowMap:
                shadowMapWidth = 8192;
                shadowMapHeight = 8192;
                break;
            case c_MidResShadowMap:
                shadowMapWidth = 4096;
                shadowMapHeight = 4096;
                break;
            case c_LowResShadowMap:
                shadowMapWidth = 512;
                shadowMapHeight = 512;
                break;
            }
            m_shadowMapWidth[i] = shadowMapWidth;
            m_shadowMapHeight[i] = shadowMapHeight;

            // Create shadow map texture
            CD3D11_TEXTURE2D_DESC texDesc(/*DXGI_FORMAT_R32_FLOAT*/DXGI_FORMAT_R32_TYPELESS, shadowMapWidth, shadowMapHeight,
                1, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&texDesc, NULL, &m_spShadowTexture[i]));

            // Create the view onto shadow map texture for use as depth buffer
            CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
            CT(m_spDevice->CreateDepthStencilView(m_spShadowTexture[i], &stencilDesc, &m_spShadowMapStencilView[i]));

            // Create the view onto shadow map texture for use as shader resource
            CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32_FLOAT);
            CT(m_spDevice->CreateShaderResourceView(m_spShadowTexture[i], &srvDesc, &m_spShadowSRView[i]));
        }

        // Create depth buffer
        CD3D11_TEXTURE2D_DESC texDesc2D(DXGI_FORMAT_D24_UNORM_S8_UINT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
        CT(m_spDevice->CreateTexture2D(&texDesc2D, NULL, &m_spDepthStencilBuffer));

        // Create the depth buffer ressource view
        CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
        CT(m_spDevice->CreateDepthStencilView(m_spDepthStencilBuffer, &stencilDesc, &m_spStencilView));

        RECT rect;
        ::GetClientRect(hwnd, &rect);
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;
        m_viewport.Width = (float)(rect.right - rect.left);
        m_viewport.Height = (float)(rect.bottom - rect.top);
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_spContext->RSSetViewports(1, &m_viewport);
    }

    //**********************************************************************
    // Method: Setup
    // See <IRenderer::Setup>
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
        m_spCamera = CCausticFactory::Instance()->CreateCamera(false);
    }
}
