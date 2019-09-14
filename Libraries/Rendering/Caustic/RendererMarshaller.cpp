//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\RendererMarshaller.h"
#include "Renderer.h"
#include "Rendering\SceneGraph\SceneGraph.h"
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
    // Parameters:
    // ppClientServer - Returns the newly created RendererMarshaller.
    //**********************************************************************
    CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppClientServer)
    {
        std::unique_ptr<CRendererMarshaller> spClientServer(new CRendererMarshaller());
        *ppClientServer = spClientServer.release();
        (*ppClientServer)->AddRef();
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
    void CRendererMarshaller::Initialize(HWND hwnd)
    {
		CCausticFactory::Instance()->CreateRenderer(hwnd, &m_spRenderer);
        InitializeCriticalSection(&m_cs);
        m_thread = CreateThread(nullptr, 0, RenderThreadProc, this, 0, nullptr);
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
        AddLambda(
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
        AddLambda(
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
    void CRendererMarshaller::LoadTexture(const wchar_t *pPath, ITexture **ppTexture)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        AddLambda(
            [this, pPath, evt, ppTexture]()
            {
                Caustic::CCausticFactory::Instance()->LoadTexture(pPath, m_spRenderer.p, ppTexture);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }

    //**********************************************************************
    // Method: LoadVideoTexture
    // See <IRenderer::LoadVideoTexture>
    //**********************************************************************
    void CRendererMarshaller::LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        AddLambda(
            [this, pPath, evt, ppTexture]()
            {
				Caustic::CCausticFactory::Instance()->LoadVideoTexture(pPath, m_spRenderer.p, ppTexture);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
        CloseHandle(evt);
    }

    //**********************************************************************
    // Method: SetSceneGraph
    // See <IRenderer::SetSceneGraph>
    //**********************************************************************
    void CRendererMarshaller::SetSceneGraph(ISceneGraph *pSceneGraph)
    {
        if (pSceneGraph)
            pSceneGraph->AddRef();
        AddLambda(
            [this, pSceneGraph]()
            {
                m_spRenderer->SetSceneGraph(pSceneGraph);
                if (pSceneGraph)
                    pSceneGraph->Release();
            }
        );
    }

    //**********************************************************************
    // Method: MainLoop
    // See <IRenderer::MainLoop>
    //**********************************************************************
    void CRendererMarshaller::MainLoop()
    {
        while (!m_exit)
        {
            EnterCriticalSection(&m_cs);
            while (!m_queue.empty())
            {
                (m_queue.front())();
                m_queue.pop();
            }
            LeaveCriticalSection(&m_cs);
            m_spRenderer->RenderFrame();
        }
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
    // Method: AddLambda
    // Adds the specified lambda into the render's instruction queue
    //**********************************************************************
    void CRendererMarshaller::AddLambda(std::function<void()> func)
    {
        EnterCriticalSection(&m_cs);
        m_queue.push(func);
        LeaveCriticalSection(&m_cs);
    }

    //**********************************************************************
    // Method: Setup
    // See <IRenderer::Setup>
    //**********************************************************************
    void CRendererMarshaller::Setup(HWND hwnd, bool createDebugDevice)
    {
        AddLambda(
            [this, hwnd, createDebugDevice]()
            {
                m_spRenderer->Setup(hwnd, createDebugDevice);
            }
        );
    }

    //**********************************************************************
    // Method: DrawLine
    // See <IRenderer::DrawLine>
    //**********************************************************************
    void CRendererMarshaller::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        AddLambda(
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
    void CRendererMarshaller::DrawMesh(ISubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat)
    {
        if (pMesh)
            pMesh->AddRef();
        if (pMaterial)
            pMaterial->AddRef();
        if (pTexture)
            pTexture->AddRef();
        if (pShader)
            pShader->AddRef();
        AddLambda(
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
    void CRendererMarshaller::GetRenderCtx(IRenderCtx **ppCtx)
    {
        HANDLE event = CreateEvent(NULL, false, false, NULL);
        AddLambda(
            [this, ppCtx, event]()
            {
                m_spRenderer->GetRenderCtx(ppCtx);
                SetEvent(event);
            }
        );
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    //**********************************************************************
    // Method: AddPointLight
    // See <IRenderer::AddPointLight>
    //**********************************************************************
    void CRendererMarshaller::AddPointLight(IPointLight *pLight)
    {
        pLight->AddRef();
        AddLambda(
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
    void CRendererMarshaller::RenderLoop()
    {
        throw new CausticException(E_FAIL, __FILE__, __LINE__); // Don't allow client to start render loop
    }

    //**********************************************************************
    // Method: RenderFrame
    // See <IRenderer::RenderFrame>
    //**********************************************************************
    void CRendererMarshaller::RenderFrame()
    {
        AddLambda(
            [this]()
            {
                m_spRenderer->RenderFrame();
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
        AddLambda(
            [this, pCamera]()
            {
                m_spRenderer->SetCamera(pCamera);
                pCamera->Release();
            }
        );
    }
}
