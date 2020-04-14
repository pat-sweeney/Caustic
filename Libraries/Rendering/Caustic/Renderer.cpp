//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Renderer.h"
#include "Renderable.h"
#include "ShaderInfo.h"
#include <vector>
#include <any>
#include "Base\Core\CritSec.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include "Rendering\SceneGraph\ISceneGraph.h"
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
        m_waitForShutdown(false, true),
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
        CGraphicsBase::Setup(hwnd, createDebugDevice);

        LoadDefaultShaders(shaderFolder.c_str());

        IShaderMgr *pShaderMgr = CShaderMgr::Instance();
        pShaderMgr->FindShader(L"Line", &m_spLineShader);

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

        //**********************************************************************
        // Create vertex buffer used to draw infinite plane
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(CLineVertex) * 5, D3D11_BIND_VERTEX_BUFFER);
            CLineVertex planePts[5] = {
                { 0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { -1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, -1.0f },
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
        }
#endif // SUPPORT_FULLQUAD
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
    // ppShaderInfo - Returns the new shader info object
    //**********************************************************************
    void CRenderer::LoadShaderInfo(std::wstring &filename, IShaderInfo **ppShaderInfo)
    {
        CCausticFactory::Instance()->CreateShaderInfo(filename.c_str(), ppShaderInfo);
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
        IShaderMgr *pShaderMgr = CShaderMgr::Instance();
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
                LoadShaderInfo(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L".shi", &spShaderInfo);
                if (spShaderInfo->HasShader(EShaderType::TypePixelShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_PS.cso", &spPixelShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeVertexShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_VS.cso", &spVertexShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeComputeShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_CS.cso", &spComputeShaderBlob);
                CCausticFactory::Instance()->CreateShader(this, shaderName.c_str(), spVertexShaderBlob, spPixelShaderBlob, spComputeShaderBlob, spShaderInfo, &spShader);
                pShaderMgr->RegisterShader(shaderName.c_str(), spShader);
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
		CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader, &spFrontMaterial);
        spFrontMaterial->SetTexture(this, L"diffuseTexture", pTexture, EShaderAccess::PixelShader);
        CRefObj<IRenderMaterial> spBackMaterial;
        if (pSubMesh->GetMeshFlags() & EMeshFlags::TwoSided)
        {
			CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader, &spBackMaterial);
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
        m_lights.push_back(CRefObj<IPointLight>(pLight));
    }

    //**********************************************************************
    // Method: GetRenderCtx
    // See <IRenderer::GetRenderCtx>
    //**********************************************************************
    void CRenderer::GetRenderCtx(IRenderCtx **ppCtx)
    {
        (*ppCtx) = m_spRenderCtx;
        if (m_spRenderCtx)
            (*ppCtx)->AddRef();
    }

    //**********************************************************************
    // Method: DrawInfinitePlane
    // See <IRenderer::DrawInfinitePlane>
    //**********************************************************************
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
        pContext->IASetVertexBuffers(0, 1, &m_spFullQuadVB, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_spFullQuadIB, DXGI_FORMAT_R32_UINT, 0);
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

    //**********************************************************************
    // Method: DrawLine
    // See <IRenderer::DrawLine>
    //**********************************************************************
    void CRenderer::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        UINT offset = 0;
        UINT vertexSize = sizeof(CLineVertex);
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
        std::vector<CRefObj<IPointLight>> lights;
        m_spLineShader->BeginRender(this, nullptr, nullptr, lights, nullptr);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->Draw(2, 0);
        m_spLineShader->EndRender(this);
    }

    //**********************************************************************
    // Method: GetGraphics
    // See <IRenderer::GetGraphics>
    //**********************************************************************
    void CRenderer::GetGraphics(IGraphics **ppGraphics)
    {
        *ppGraphics = this;
        (*ppGraphics)->AddRef();
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
        for (size_t i = 0; i < m_singleObjs.size(); i++)
        {
            if (m_singleObjs[i].m_passes & (1 << pass))
                m_singleObjs[i].Render(this, m_lights, m_spRenderCtx);
        }
    }

    //**********************************************************************
    // Method: RenderScene
    // Renders current scene (both scene graph and any renderables currently
    // attached to the renderer)
    //**********************************************************************
    void CRenderer::RenderScene(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
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
            CRenderCtx *pCtx = (CRenderCtx*)m_spRenderCtx.p;
            pCtx->m_currentPass = pass;
            pCtx->m_passBlendable = true;
            if (pass == c_PassOpaque)
                DrawSceneObjects(pass, renderCallback);
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
#ifdef SUPPORT_SHADOW_MAPPING
            else if (pass == c_PassShadow)
            {
                int numShadowPasses = (m_lights.size() < c_MaxShadowMaps) ? m_lights.size() : c_MaxShadowMaps;
                for (int i = 0; i < numShadowPasses; i++)
                {
                    m_spContext->OMSetRenderTargets(1, &m_spShadowRTView[i], m_spStencilView);
                    FLOAT bgClr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                    m_spContext->ClearRenderTargetView(m_spShadowRTView[i], bgClr);
                    m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
                    // Reset the camera to be from the lights perspective
                    CRefObj<ICamera> spCamera;
                    CreateCamera(true, &spCamera);
                    spCamera->SetPosition(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
                    this->SetCamera(spCamera);
                    DrawSceneObjects(pass, renderCallback);
                    // Restore default render targets
                    m_spContext->OMSetRenderTargets(1, &m_spRTView, m_spStencilView);
                }
            }
#endif // SUPPORT_SHADOW_MAPPING
            else if (pass == c_PassTransparent)
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
                DrawSceneObjects(pass, renderCallback);
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
        while (!m_exitThread)
        {
            RenderFrame(renderCallback);
        }
        m_waitForShutdown.Set();
    }

    //**********************************************************************
    // Function: CreateGraphics
    // Creates a graphics device bound to the specified window
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // ppGraphics - Returns the created device
    //**********************************************************************
    CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics)
    {
        _ASSERT(ppGraphics);
        std::unique_ptr<CGraphics> spGraphics(new CGraphics());
        spGraphics->Setup(hwnd, true);

        CRefObj<ICamera> spCamera;
        CCausticFactory::Instance()->CreateCamera(true, &spCamera);
        spGraphics->SetCamera(spCamera);

        *ppGraphics = spGraphics.release();
        (*ppGraphics)->AddRef();
    }

    //**********************************************************************
    // Function: CreateRenderer
    // Creates a renderer
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // shaderFolder - path to directory containing shaders
    // ppRenderer - Returns the created renderer
    //**********************************************************************
    CAUSTICAPI void CreateRenderer(HWND hwnd, std::wstring &shaderFolder, IRenderer **ppRenderer)
    {
        _ASSERT(ppRenderer);
        std::unique_ptr<CRenderer> spRenderer(new CRenderer());
        spRenderer->Setup(hwnd, shaderFolder, true);

        CRefObj<ICamera> spCamera;
		CCausticFactory::Instance()->CreateCamera(true, &spCamera);
        spRenderer->SetCamera(spCamera);

        *ppRenderer = spRenderer.release();
        (*ppRenderer)->AddRef();
    }

    //**********************************************************************
    // Method: SetCamera
    // See <IRenderer::SetCamera>
    //**********************************************************************
    void CGraphicsBase::SetCamera(ICamera *pCamera)
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
		CCausticFactory::Instance()->CreateCamera(false, &m_spCamera);
    }
}
