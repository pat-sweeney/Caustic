//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\RendererMarshaller.h"
#include "Renderer.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include <queue>
#include <functional>
#include <algorithm>

namespace Caustic {
    CAUSTICAPI void CreateRendererMarshaller(IRendererMarshaller **ppClientServer)
    {
        std::unique_ptr<CRendererMarshaller> spClientServer(new CRendererMarshaller());
        *ppClientServer = spClientServer.release();
        (*ppClientServer)->AddRef();
    }

    CRendererMarshaller::CRendererMarshaller() :
        m_exit(false),
        m_thread(nullptr)
    {
    }

    DWORD WINAPI RenderThreadProc(LPVOID lpParameter)
    {
        CRendererMarshaller *pClientServer = (CRendererMarshaller*)lpParameter;
        pClientServer->MainLoop();
        return 0;
    }

    void CRendererMarshaller::Initialize(HWND hwnd)
    {
        Caustic::CreateRenderer(hwnd, &m_spRenderer);
        InitializeCriticalSection(&m_cs);
        m_thread = CreateThread(nullptr, 0, RenderThreadProc, this, 0, nullptr);
    }

    void CRendererMarshaller::Shutdown()
    {
        m_exit = true;
        WaitForSingleObject(m_thread, INFINITE);
    }

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
    }

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
    }
    
    void CRendererMarshaller::LoadTexture(const wchar_t *pPath, ITexture **ppTexture)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        AddLambda(
            [this, pPath, evt, ppTexture]()
            {
                Caustic::LoadTexture(pPath, m_spRenderer.p, ppTexture);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
    }

    void CRendererMarshaller::LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture)
    {
        HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        AddLambda(
            [this, pPath, evt, ppTexture]()
            {
                Caustic::LoadVideoTexture(pPath, m_spRenderer.p, ppTexture);
                SetEvent(evt);
            }
        );
        WaitForSingleObject(evt, INFINITE);
    }

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

    CComPtr<ID3D11Device> CRendererMarshaller::GetDevice()
    {
        return CComPtr<ID3D11Device>(nullptr); // We don't allow the client access to the D3D device
    }

    CComPtr<ID3D11DeviceContext> CRendererMarshaller::GetContext()
    {
        return CComPtr<ID3D11DeviceContext>(nullptr); // We don't allow the client access to the D3D device
    }

    CRefObj<ICamera> CRendererMarshaller::GetCamera()
    {
        return m_spRenderer->GetCamera();
    }

    void CRendererMarshaller::AddLambda(std::function<void()> func)
    {
        EnterCriticalSection(&m_cs);
        m_queue.push(func);
        LeaveCriticalSection(&m_cs);
    }

    void CRendererMarshaller::Setup(HWND hwnd, bool createDebugDevice)
    {
        AddLambda(
            [this, hwnd, createDebugDevice]()
            {
                m_spRenderer->Setup(hwnd, createDebugDevice);
            }
        );
    }

    void CRendererMarshaller::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        AddLambda(
            [this, p1, p2, clr]()
            {
                m_spRenderer->DrawLine(p1, p2, clr);
            }
        );
    }

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
    }

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

    void CRendererMarshaller::RenderLoop()
    {
        throw new CausticException(E_FAIL, __FILE__, __LINE__); // Don't allow client to start render loop
    }

    void CRendererMarshaller::RenderFrame()
    {
        AddLambda(
            [this]()
            {
                m_spRenderer->RenderFrame();
            }
        );
    }

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
