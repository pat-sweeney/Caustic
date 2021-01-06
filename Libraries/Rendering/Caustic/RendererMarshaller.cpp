//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\RendererMarshaller.h"
#include "Renderer.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include <queue>
#include <functional>
#include <algorithm>

//**********************************************************************
// File: RendererMarshaller.cpp
// Code related to the renderer marshaller. The RendererMarshaller
// marshalls rendering calls from some user thread to the render thread.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateRendererMarshaller
    // Global function for creating the RendererMarshaller. This method should generally
    // not be called. Use the ICausticFactory to create new Caustic objects.
    //
    // Returns:
    // Returns the newly created RendererMarshaller.
    //**********************************************************************
    CAUSTICAPI CRefObj<IRendererMarshaller> CreateRendererMarshaller()
    {
        return CRefObj<IRendererMarshaller>(new CRendererMarshaller());
    }

    //**********************************************************************
    // Method: CRendererMarshaller
    // Defines the implementation of <IRendererMarshaller>
    //**********************************************************************
    CRendererMarshaller::CRendererMarshaller() :
        m_exit(false),
        m_thread(nullptr)
    {
    }

    //**********************************************************************
    // Method: Freeze
    // See <IRenderer::Freeze>
    //**********************************************************************
    void CRendererMarshaller::Freeze()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->Freeze();
            }
        );
    }
    //**********************************************************************
    // Method: Unfreeze
    // See <IRenderer::Unfreeze>
    //**********************************************************************
    void CRendererMarshaller::Unfreeze()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->Unfreeze();
            }
        );
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
        CRendererMarshaller *pClientServer = (CRendererMarshaller*)lpParameter;
        pClientServer->MainLoop();
        return 0;
    }

    //**********************************************************************
    // Method: Initialize
    // See <IRenderer::Initialize>
    //**********************************************************************
    void CRendererMarshaller::Initialize(HWND hwnd, std::wstring &shaderFolder, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback, bool startFrozen /* = false */)
    {
        m_spRenderer = CCausticFactory::Instance()->CreateRenderer(hwnd, shaderFolder, startFrozen);
        InitializeCriticalSection(&m_renderQueue.m_cs);
        m_thread = CreateThread(nullptr, 0, RenderThreadProc, this, 0, nullptr);
        m_renderCallback = renderCallback;
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
    // Method: PushShadowmapRT
    // See <IRenderer::PushShadowmapRT>
    //**********************************************************************
    void CRendererMarshaller::PushShadowmapRT(int whichShadowmap, int lightMapIndex, Vector3& lightPos, Vector3 &lightDir)
    {
        Vector3 lp = lightPos;
        Vector3 ld = lightDir;
        m_renderQueue.AddLambda(
            [this, whichShadowmap, lightMapIndex, &lp, &ld]()
            {
                m_spRenderer->PushShadowmapRT(whichShadowmap, lightMapIndex, lp, ld);
            }
        );
    }
    
    //**********************************************************************
    // Method: PopShadowmapRT
    // See <IRenderer::PopShadowmapRT>
    //**********************************************************************
    void CRendererMarshaller::PopShadowmapRT()
    {
        m_renderQueue.AddLambda(
            [this]()
            {
                m_spRenderer->PopShadowmapRT();
            }
        );
    }
    
    //**********************************************************************
    // Method: SelectShadowmap
    // See <IRenderer::SelectShadowmap>
    //**********************************************************************
    void CRendererMarshaller::SelectShadowmap(int whichShadowmap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader)
    {
        std::vector<CRefObj<ILight>> copyLights = lights;
        m_renderQueue.AddLambda(
            [this, whichShadowmap, lightMapIndex, &copyLights, pShader]()
            {
                m_spRenderer->SelectShadowmap(whichShadowmap, lightMapIndex, copyLights, pShader);
            }
        );
    }

    //**********************************************************************
    // Method: GetShadowmapTexture
    // See <IRenderer::GetShadowmapTexture>
    //**********************************************************************
    CRefObj<ITexture> CRendererMarshaller::GetShadowmapTexture(int whichShadowmap)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        CRefObj<ITexture> spTexture;
        m_renderQueue.AddLambda(
            [this, evt, whichShadowmap , &spTexture]()
            {
                spTexture = m_spRenderer->GetShadowmapTexture(whichShadowmap);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
        return spTexture;
    }

    void CRendererMarshaller::RunOnRenderer(std::function<void(IRenderer*, void* clientData)> callback, void* clientData)
    {
        m_renderQueue.AddLambda(
            [this, callback, clientData]()
            {
                callback(m_spRenderer, clientData);
            }
        );
    }

    //**********************************************************************
    // Method: SaveScene
    // Saves the scene graph to the specified file.
    //
    // Parameters:
    // pFilename - name of file to save to
    // pSceneGraph - Scene graph to save
    //**********************************************************************
    void CRendererMarshaller::SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph)
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
    void CRendererMarshaller::LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph)
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
    CRefObj<ITexture> CRendererMarshaller::LoadTexture(const wchar_t *pPath)
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
    CRefObj<ITexture> CRendererMarshaller::LoadVideoTexture(const wchar_t *pPath)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        CRefObj<ITexture> spTexture;
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
        while (!m_exit)
        {
            EnterCriticalSection(&m_renderQueue.m_cs);
            while (!m_renderQueue.m_queue.empty())
            {
                (m_renderQueue.m_queue.front())();
                m_renderQueue.m_queue.pop();
            }
            LeaveCriticalSection(&m_renderQueue.m_cs);
            m_spRenderer->RenderFrame(m_renderCallback);
        }
    }

    //**********************************************************************
    // Method: DrawScreenQuad
    // See <IRenderer::DrawScreenQuad>
    //**********************************************************************
    void CRendererMarshaller::DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler *pSampler)
    {
        m_renderQueue.AddLambda(
            [this, minU, minV, maxU, maxV, pTexture, pSampler]()
            {
                m_spRenderer->DrawScreenQuad(minU, minV, maxU, maxV, pTexture, pSampler);
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
    void CRendererMarshaller::Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice, bool startFrozen /* = false */)
    {
        m_renderQueue.AddLambda(
            [this, hwnd, shaderFolder, createDebugDevice, startFrozen]()
            {
                std::wstring sh = shaderFolder;
                m_spRenderer->Setup(hwnd, sh, createDebugDevice, startFrozen);
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
    void CRendererMarshaller::DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat)
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
    void CRendererMarshaller::AddPointLight(IPointLight *pLight)
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
    void CRendererMarshaller::RenderLoop(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        throw new CausticException(E_FAIL, __FILE__, __LINE__); // Don't allow client to start render loop
    }

    //**********************************************************************
    // Method: RenderFrame
    // See <IRenderer::RenderFrame>
    //**********************************************************************
    void CRendererMarshaller::RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> callback = renderCallback;
        m_renderQueue.AddLambda(
            [this, callback]()
            {
                m_spRenderer->RenderFrame(callback);
            }
        );
    }

    //**********************************************************************
    // Method: SetCamera
    // See <IRenderer::SetCamera>
    //**********************************************************************
    void CRendererMarshaller::SetCamera(ICamera *pCamera)
    {
        pCamera->AddRef();
        m_renderQueue.AddLambda(
            [this, pCamera]()
            {
                m_spRenderer->SetCamera(pCamera);
                pCamera->Release();
            }
        );
    }
}
