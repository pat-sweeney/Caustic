//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <queue>
#include <functional>
#include <algorithm>
#include <Windows.h>
#include <atlbase.h>
#include <Shlwapi.h>

module Rendering.RendererMarshaller.RendererMarshaller;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Geometry.Mesh.Mesh;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IVideoTexture;
import Rendering.RendererMarshaller.IRendererMarshaller;
import Rendering.SceneGraph.ISceneGraph;

//**********************************************************************
// File: RendererMarshaller.cpp
// Code related to the renderer marshaller. The RendererMarshaller
// marshalls rendering calls from some user thread to the render thread.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Method: CRendererMarshaller
    // Defines the implementation of <IRendererMarshaller>
    //**********************************************************************
    CRendererMarshaller::CRendererMarshaller() :
        m_exit(false),
        m_thread(nullptr)
    {
    }

    CRefObj<IRenderMesh> CRendererMarshaller::ToRenderMesh(IMesh* pMesh, IShader* pShader)
    {
        CRefObj<IRenderMesh> spRenderMesh;
        RunOnRenderer(
            [this, pMesh, pShader, &spRenderMesh](IRenderer *pRenderer)
            {
                spRenderMesh = pRenderer->ToRenderMesh(pMesh, pShader);
            }, true);
        return spRenderMesh;
    }

    void CRendererMarshaller::ToRenderMaterials(IMesh* pMesh, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial)
    {
        RunOnRenderer(
            [this, pMesh, pShader, pRenderMesh, pDefaultMaterial](IRenderer *pRenderer)
            {
                pRenderer->ToRenderMaterials(pMesh, pShader, pRenderMesh, pDefaultMaterial);
            }, false);
    }

    //**********************************************************************
    // Method: EnableDepthTest
    // See <IRenderer::EnableDepthTest>
    //**********************************************************************
    bool CRendererMarshaller::EnableDepthTest(bool enable)
    {
        bool enableDepth;
        RunOnRenderer(
            [this, enable, &enableDepth](IRenderer *pRenderer)
            {
                enableDepth = pRenderer->EnableDepthTest(enable);
            }, true);
        return enableDepth;
    }

    //**********************************************************************
    // Method: Freeze
    // See <IRenderer::Freeze>
    //**********************************************************************
    void CRendererMarshaller::Freeze()
    {
        RunOnRenderer(
            [this](IRenderer *pRenderer)
            {
                pRenderer->Freeze();
            }, false);
    }

    //**********************************************************************
    // Method: Unfreeze
    // See <IRenderer::Unfreeze>
    //**********************************************************************
    void CRendererMarshaller::Unfreeze()
    {
        // NOTE: We must run Unfreeze immediately (i.e. not from RunOnRenderer()
        // since the renderer is frozen and won't run the command... i.e. deadlock)
        m_spRenderer->Unfreeze();
    }

    //**********************************************************************
    // Method: IsFrozen
    // See <IRenderer::IsFrozen>
    //**********************************************************************
    bool CRendererMarshaller::IsFrozen()
    {
        return m_spRenderer->IsFrozen();
    }

    //**********************************************************************
    // Method: RenderThreadProc
    // Entry point for the render thread.
    //
    // Parameters:
    // lpParameter - CRendererMarshaller object
    //**********************************************************************
    DWORD WINAPI RenderThreadProc(LPVOID lpParameter)
    {
        CRendererMarshaller* pClientServer = (CRendererMarshaller*)lpParameter;
        pClientServer->MainLoop();
        return 0;
    }

    //**********************************************************************
    // Method: DeviceWindowResized
    // See <IRenderer::DeviceWindowResized>
    //**********************************************************************
    void CRendererMarshaller::DeviceWindowResized(uint32_t width, uint32_t height)
    {
        RunOnRenderer(
            [this, width, height](IRenderer* pRenderer)
            {
                pRenderer->DeviceWindowResized(width, height);
            }, false);
    }

    //**********************************************************************
    // Method: Initialize
    // See <IRenderer::Initialize>
    //**********************************************************************
    void CRendererMarshaller::Initialize(HWND hwnd, BBox2 &viewport, std::wstring& shaderFolder,
        std::function<void(IRenderer* pRenderer)> postCreate,
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
        std::function<void(IRenderer* pRenderer)> prePresentCallback,
        bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        m_spRenderer = CCausticFactory::Instance()->CreateRenderer(hwnd, viewport, shaderFolder, startFrozen, desktopIndex);
        InitializeCriticalSection(&m_renderQueue.m_cs);
        m_thread = CreateThread(nullptr, 0, RenderThreadProc, this, 0, nullptr);
        m_postCreate = postCreate;
        m_renderCallback = renderCallback;
        m_prePresentCallback = prePresentCallback;
    }

    //**********************************************************************
    // Method: Shutdown
    // See <IRenderer::Shutdown>
    //**********************************************************************
    void CRendererMarshaller::Shutdown()
    {
        m_exit = true;
        WaitForSingleObject(m_thread, INFINITE);
    }


    //**********************************************************************
    // Method: BeginShadowmapPass
    // See <IRenderer::BeginShadowmapPass>
    //**********************************************************************
    void CRendererMarshaller::BeginShadowmapPass(int whichShadowmap)
    {
        RunOnRenderer(
            [this, whichShadowmap](IRenderer* pRenderer)
            {
                pRenderer->BeginShadowmapPass(whichShadowmap);
            }, false);
    }

    //**********************************************************************
    // Method: EndShadowmapPass
    // See <IRenderer::EndShadowmapPass>
    //**********************************************************************
    void CRendererMarshaller::EndShadowmapPass(int whichShadowmap)
    {
        RunOnRenderer(
            [this, whichShadowmap](IRenderer* pRenderer)
            {
                pRenderer->EndShadowmapPass(whichShadowmap);
            }, false);
    }

    //**********************************************************************
    // Method: PushShadowmapRT
    // See <IRenderer::PushShadowmapRT>
    //**********************************************************************
    void CRendererMarshaller::PushShadowmapRT(int whichShadowmap, int lightMapIndex, const Vector3& lightPos, const Vector3& lightDir)
    {
        Vector3 lp = lightPos;
        Vector3 ld = lightDir;
        RunOnRenderer(
            [this, whichShadowmap, lightMapIndex, &lp, &ld](IRenderer *pRenderer)
            {
                pRenderer->PushShadowmapRT(whichShadowmap, lightMapIndex, lp, ld);
            }, false);
    }

    //**********************************************************************
    // Method: PopShadowmapRT
    // See <IRenderer::PopShadowmapRT>
    //**********************************************************************
    void CRendererMarshaller::PopShadowmapRT()
    {
        RunOnRenderer(
            [this](IRenderer *pRenderer)
            {
                pRenderer->PopShadowmapRT();
            }, false);
    }

    //**********************************************************************
    // Method: SelectShadowmap
    // See <IRenderer::SelectShadowmap>
    //**********************************************************************
    void CRendererMarshaller::SelectShadowmap(int whichShadowmap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader)
    {
        std::vector<CRefObj<ILight>> copyLights = lights;
        RunOnRenderer(
            [this, whichShadowmap, lightMapIndex, &copyLights, pShader](IRenderer *pRenderer)
            {
                pRenderer->SelectShadowmap(whichShadowmap, lightMapIndex, copyLights, pShader);
            }, false);
    }

    //**********************************************************************
    // Method: GetShadowmapTexture
    // See <IRenderer::GetShadowmapTexture>
    //**********************************************************************
    CRefObj<ITexture> CRendererMarshaller::GetShadowmapTexture(int whichShadowmap)
    {
        CRefObj<ITexture> spTexture;
        RunOnRenderer(
            [this, whichShadowmap, &spTexture](IRenderer *pRenderer)
            {
                spTexture = pRenderer->GetShadowmapTexture(whichShadowmap);
            }, true);
        return spTexture;
    }

    void CRendererMarshaller::RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait /* = false */)
    {
        if (GetRenderThreadID() == GetCurrentThreadId())
        {
            callback(m_spRenderer);
            return;
        }
        HANDLE evt;
        if (wait)
            evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        m_renderQueue.AddLambda(
            [this, evt, callback]()
            {
                callback(m_spRenderer);
                SetEvent(evt);
            }
        );
        if (wait)
        {
            WaitForSingleObject(evt, INFINITE);
            CloseHandle(evt);
        }
    }

    //**********************************************************************
    // Method: SaveScene
    // Saves the scene graph to the specified file.
    //
    // Parameters:
    // pFilename - name of file to save to
    // pSceneGraph - Scene graph to save
    //**********************************************************************
    void CRendererMarshaller::SaveScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        m_renderQueue.AddLambda(
            [this, pFilename, pSceneGraph, evt]()
            {
                CComPtr<IStream> spStream;
                CT(SHCreateStreamOnFile(pFilename, STGM_CREATE | STGM_WRITE, &spStream));
                pSceneGraph->Lock();
                pSceneGraph->Store(spStream);
                pSceneGraph->Unlock();
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }

    //**********************************************************************
    // Method: LoadScene
    // Loads the scene graph from the specified file.
    //
    // Parameters:
    // pFilename - name of file to load from
    // pSceneGraph - Scene graph to load
    //**********************************************************************
    void CRendererMarshaller::LoadScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        m_renderQueue.AddLambda(
            [this, pFilename, pSceneGraph, evt]()
            {
                CComPtr<IStream> spStream;
                SHCreateStreamOnFile(pFilename, STGM_READ, &spStream);
                pSceneGraph->Lock();
                pSceneGraph->Load(spStream);
                pSceneGraph->Unlock();
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }

    //**********************************************************************
    // Method: LoadTexture
    // See <IRenderer::LoadTexture>
    //**********************************************************************
    CRefObj<ITexture> CRendererMarshaller::LoadTexture(const wchar_t* pPath)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        CRefObj<ITexture> spTexture;
        m_renderQueue.AddLambda(
            [this, pPath, evt, &spTexture]()
            {
                spTexture = Caustic::CCausticFactory::Instance()->LoadTexture(pPath, m_spRenderer);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return spTexture;
    }

    //**********************************************************************
    // Method: LoadVideoTexture
    // See <IRenderer::LoadVideoTexture>
    //**********************************************************************
    CRefObj<IVideoTexture> CRendererMarshaller::LoadVideoTexture(const wchar_t* pPath)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        CRefObj<IVideoTexture> spTexture;
        m_renderQueue.AddLambda(
            [this, pPath, evt, &spTexture]()
            {
                spTexture = Caustic::CCausticFactory::Instance()->LoadVideoTexture(pPath, m_spRenderer);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return spTexture;
    }

    //**********************************************************************
    // Method: MainLoop
    // See <IRenderer::MainLoop>
    //**********************************************************************
    void CRendererMarshaller::MainLoop()
    {
        if (m_postCreate != nullptr)
            m_postCreate(m_spRenderer);
        m_renderThreadID = GetCurrentThreadId();
        while (!m_exit)
        {
            EnterCriticalSection(&m_renderQueue.m_cs);
            while (!m_renderQueue.m_queue.empty())
            {
                (m_renderQueue.m_queue.front())();
                m_renderQueue.m_queue.pop();
            }
            LeaveCriticalSection(&m_renderQueue.m_cs);
            m_spRenderer->RenderFrame(m_renderCallback, m_prePresentCallback);
        }
    }

    //**********************************************************************
    // Method: DrawScreenQuadWithCustomShader
    // See <IRenderer::DrawScreenQuadWithCustomShader>
    //**********************************************************************
    void CRendererMarshaller::DrawScreenQuadWithCustomShader(IShader* pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth /* = false */)
    {
        m_renderQueue.AddLambda(
            [this, pShader, minU, minV, maxU, maxV, pTexture, pSampler, disableDepth]()
            {
                m_spRenderer->DrawScreenQuadWithCustomShader(pShader, minU, minV, maxU, maxV, pTexture, pSampler, disableDepth);
            }
        );
    }

    //**********************************************************************
    // Method: DrawScreenQuad
    // See <IRenderer::DrawScreenQuad>
    //**********************************************************************
    void CRendererMarshaller::DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth /* = false */)
    {
        m_renderQueue.AddLambda(
            [this, minU, minV, maxU, maxV, pTexture, pSampler, disableDepth]()
            {
                m_spRenderer->DrawScreenQuad(minU, minV, maxU, maxV, pTexture, pSampler, disableDepth);
            }
        );
    }

    //**********************************************************************
    // Method: GetDevice
    // See <IRenderer::GetDevice>
    //**********************************************************************
    CComPtr<ID3D11Device> CRendererMarshaller::GetDevice()
    {
        return CComPtr<ID3D11Device>(nullptr); // We don't allow the client access to the D3D device
    }

    //**********************************************************************
    // Method: GetDuplication
    // See <IRenderer::GetDuplication>
    //**********************************************************************
    CComPtr<IDXGIOutputDuplication> CRendererMarshaller::GetDuplication()
    {
        return CComPtr<IDXGIOutputDuplication>(nullptr); // We don't allow the client access to the duplication service
    }

#ifdef _DEBUG
    //**********************************************************************
    // Method: BeginMarker
    // See <IRenderer::BeginMarker>
    //**********************************************************************
    void CRendererMarshaller::BeginMarker(const wchar_t* pLabel)
    {
        m_renderQueue.AddLambda(
            [this, pLabel]()
            {
                m_spRenderer->BeginMarker(pLabel);
            }
        );
    }

    //**********************************************************************
    // Method: EndMarker
    // See <IRenderer::EndMarker>
    //**********************************************************************
    void CRendererMarshaller::EndMarker()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->EndMarker();
            }
        );
    }

#ifdef SUPPORT_GRAPHICS_CAPTURE
    //**********************************************************************
    // Method: BeginCapture
    // See <IRenderer::BeginCapture>
    //**********************************************************************
    void CRendererMarshaller::BeginCapture()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->BeginCapture();
            }
        );
    }

    //**********************************************************************
    // Method: EndCapture
    // See <IRenderer::EndCapture>
    //**********************************************************************
    void CRendererMarshaller::EndCapture()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->EndCapture();
            }
        );
    }
#endif // SUPPORT_GRAPHICS_CAPTURE
#endif

    //**********************************************************************
    // Method: LoadShaders
    // See <IRenderer::LoadShaders>
    //**********************************************************************
    void CRendererMarshaller::LoadShaders(const wchar_t* pFolder)
    {
        m_renderQueue.AddLambda(
            [this, pFolder]()
            {
                m_spRenderer->LoadShaders(pFolder);
            }
        );
    }

    //**********************************************************************
    // Method: GetContext
    // See <IRenderer::GetContext>
    //**********************************************************************
    CComPtr<ID3D11DeviceContext> CRendererMarshaller::GetContext()
    {
        return CComPtr<ID3D11DeviceContext>(nullptr); // We don't allow the client access to the D3D device
    }

    //**********************************************************************
    // Method: GetCamera
    // See <IRenderer::GetCamera>
    //**********************************************************************
    CRefObj<ICamera> CRendererMarshaller::GetCamera()
    {
        return m_spRenderer->GetCamera();
    }

    //**********************************************************************
    // Method: AddRenderable
    // See <IRenderer::AddRenderable>
    //**********************************************************************
    void CRendererMarshaller::AddRenderable(IRenderable* pRenderable)
    {
        m_renderQueue.AddLambda(
            [this, pRenderable]()
            {
                m_spRenderer->AddRenderable(pRenderable);
            }
        );
    }

    //**********************************************************************
    // Method: Setup
    // See <IRenderer::Setup>
    //**********************************************************************
    void CRendererMarshaller::Setup(HWND hwnd, BBox2 &viewport, std::wstring& shaderFolder, bool createDebugDevice, bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        m_renderQueue.AddLambda(
            [this, hwnd, viewport, shaderFolder, createDebugDevice, startFrozen, desktopIndex]()
            {
                std::wstring sh = shaderFolder;
                BBox2 vp = viewport;
                m_spRenderer->Setup(hwnd, vp, sh, createDebugDevice, startFrozen, desktopIndex);
            }
        );
    }

    //**********************************************************************
    // Method: DrawLine
    // See <IRenderer::DrawLine>
    //**********************************************************************
    void CRendererMarshaller::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        m_renderQueue.AddLambda(
            [this, p1, p2, clr]()
            {
                m_spRenderer->DrawLine(p1, p2, clr);
            }
        );
    }

    //**********************************************************************
    // Method: DrawMesh
    // See <IRenderer::DrawMesh>
    //**********************************************************************
    void CRendererMarshaller::DrawMesh(IRenderSubMesh* pMesh, IMaterialAttrib* pMaterial, ITexture* pTexture, IShader* pShader, DirectX::XMMATRIX& mat)
    {
        if (pMesh)
            pMesh->AddRef();
        if (pMaterial)
            pMaterial->AddRef();
        if (pTexture)
            pTexture->AddRef();
        if (pShader)
            pShader->AddRef();
        m_renderQueue.AddLambda(
            [this, pMesh, pMaterial, pTexture, pShader, mat]()
            {
                DirectX::XMMATRIX m = mat;
                m_spRenderer->DrawMesh(pMesh, pMaterial, pTexture, pShader, m);
                if (pMesh)
                    pMesh->Release();
                if (pMaterial)
                    pMaterial->Release();
                if (pTexture)
                    pTexture->Release();
                if (pShader)
                    pShader->Release();
            }
        );
    }

    //**********************************************************************
    // Method: GetRenderCtx
    // See <IRenderer::GetRenderCtx>
    //**********************************************************************
    CRefObj<IRenderCtx> CRendererMarshaller::GetRenderCtx()
    {
        HANDLE event = CreateEvent(NULL, false, false, NULL);
        CRefObj<IRenderCtx> spRenderCtx;
        m_renderQueue.AddLambda(
            [this, &spRenderCtx, event]()
        {
            spRenderCtx = m_spRenderer->GetRenderCtx();
            SetEvent(event);
        }
        );
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
        return spRenderCtx;
    }

    //**********************************************************************
    // Method: ClearDepth
    // See <IRenderer::ClearDepth>
    //**********************************************************************
    void CRendererMarshaller::ClearDepth()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->ClearDepth();
            }
        );
    }

    //**********************************************************************
    // Method: AddPointLight
    // See <IRenderer::AddPointLight>
    //**********************************************************************
    void CRendererMarshaller::AddPointLight(IPointLight* pLight)
    {
        pLight->AddRef();
        m_renderQueue.AddLambda(
            [this, pLight]()
            {
                m_spRenderer->AddPointLight(pLight);
                pLight->Release();
            }
        );
    }

    //**********************************************************************
    // Method: RenderLoop
    // See <IRenderer::RenderLoop>. It is not valid for the client to call this method.
    //**********************************************************************
    void CRendererMarshaller::RenderLoop(
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
        std::function<void(IRenderer* pRenderer)> prePresentCallback
    )
    {
        throw new CausticException(E_FAIL, __FILE__, __LINE__); // Don't allow client to start render loop
    }

    //**********************************************************************
    // Method: RenderFrame
    // See <IRenderer::RenderFrame>
    //**********************************************************************
    void CRendererMarshaller::RenderFrame(
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
        std::function<void(IRenderer* pRenderer)> prePresentCallback
    )
    {
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> callback = renderCallback;
        std::function<void(IRenderer* pRenderer)> preCallback = prePresentCallback;
        m_renderQueue.AddLambda(
            [this, callback, preCallback]()
            {
                m_spRenderer->RenderFrame(callback, preCallback);
            }
        );
    }

    //**********************************************************************
    // Method: GetBackBufferWidth
    // See <IRenderer::GetBackBufferWidth>
    //**********************************************************************
    uint32_t CRendererMarshaller::GetBackBufferWidth()
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        uint32_t width;
        m_renderQueue.AddLambda(
            [this, evt, &width]()
            {
                width = m_spRenderer->GetBackBufferWidth();
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return width;
    }

    //**********************************************************************
    // Method: GetBackBufferHeight
    // See <IRenderer::GetBackBufferHeight>
    //**********************************************************************
    uint32_t CRendererMarshaller::GetBackBufferHeight()
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        uint32_t height;
        m_renderQueue.AddLambda(
            [this, evt, &height]()
            {
                height = m_spRenderer->GetBackBufferHeight();
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return height;
    }

    //**********************************************************************
    // Method: GetBackBuffer
    // See <IRenderer::GetBackBuffer>
    //**********************************************************************
    CComPtr<ID3D11Texture2D> CRendererMarshaller::GetBackBuffer()
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        CComPtr<ID3D11Texture2D> spTexture;
        m_renderQueue.AddLambda(
            [this, evt, &spTexture]()
            {
                spTexture = m_spRenderer->GetBackBuffer();
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return spTexture;
    }

    //**********************************************************************
    // Method: CopyFrameBackbuffer
    // See <IRenderer::CopyFrameBackbuffer>
    //**********************************************************************
    void CRendererMarshaller::CopyFrameBackBuffer(IImage* pImage)
    {
        RunOnRenderer(
            [this, pImage](IRenderer *pRenderer)
            {
                pRenderer->CopyFrameBackBuffer(pImage);
            }, false);
    }

    //**********************************************************************
    // Method: SetCamera
    // See <IRenderer::SetCamera>
    //**********************************************************************
    void CRendererMarshaller::SetCamera(ICamera* pCamera)
    {
        pCamera->AddRef();
        RunOnRenderer(
            [this, pCamera](IRenderer *pRenderer)
            {
                pRenderer->SetCamera(pCamera);
                pCamera->Release();
            }, false);
    }

    //**********************************************************************
    // Method: SetFinalRenderTarget
    // See <IRenderer::SetFinalRenderTarget>
    //**********************************************************************
    void CRendererMarshaller::SetFinalRenderTarget(ID3D11Texture2D* pTexture)
    {
        pTexture->AddRef();
        RunOnRenderer(
            [this, pTexture](IRenderer *pRenderer)
            {
                pRenderer->SetFinalRenderTarget(pTexture);
                pTexture->Release();
            }, false);
    }

    CRefObj<IRendererMarshaller> CreateRendererMarshallerInternal()
    {
        return CRefObj<IRendererMarshaller>(new CRendererMarshaller());
    }

    //**********************************************************************
    // Method: SetViewport
    // See <IRenderer::SetViewport>
    //**********************************************************************
    void CRendererMarshaller::SetViewport(float x0, float y0, float x1, float y1)
    {
        RunOnRenderer(
            [this, x0, y0, x1, y1](IRenderer* pRenderer)
            {
                pRenderer->SetViewport(x0, y0, x1, y1);
            }, false);
    }

    //**********************************************************************
    // Method: SetFinalRenderTarget
    // See <IRenderer::SetFinalRenderTargetUsingSharedTexture>
    //**********************************************************************
    void CRendererMarshaller::SetFinalRenderTargetUsingSharedTexture(IUnknown* pTexture)
    {
        pTexture->AddRef();
        RunOnRenderer(
            [this, pTexture](IRenderer *pRenderer)
            {
                pRenderer->SetFinalRenderTargetUsingSharedTexture(pTexture);
                pTexture->Release();
            }, false);
    }
}
