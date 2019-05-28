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
		CoInitialize(nullptr);
        pClientServer->MainLoop();
		CoUninitialize();
        return 0;
    }

	void CRendererMarshaller::MainLoop()
	{
		CCausticFactory::Instance()->CreateRenderer(m_hwnd, m_shaderFolder, &m_spRenderer);
		while (!m_exit)
		{
			m_spRenderer->BeginFrame(m_renderCallback);
			EnterCriticalSection(&m_cs);
			while (!m_queue.empty())
			{
				(m_queue.front())();
				m_queue.pop();
			}
			LeaveCriticalSection(&m_cs);
			m_spRenderer->EndFrame();
		}
	}

	void CRendererMarshaller::Initialize(HWND hwnd, std::wstring &shaderFolder, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        InitializeCriticalSection(&m_cs);
		m_hwnd = hwnd;
		m_shaderFolder = shaderFolder;
		m_renderCallback = renderCallback;
        m_thread = CreateThread(nullptr, 0, RenderThreadProc, this, 0, nullptr);
    }

    void CRendererMarshaller::Shutdown()
    {
        m_exit = true;
        WaitForSingleObject(m_thread, INFINITE);
    }

	void CRendererMarshaller::SetMaxCmdLength()
	{
	}

	void CRendererMarshaller::GetRenderer(IRenderer **ppRenderer)
	{
		*ppRenderer = this;
		(*ppRenderer)->AddRef();
	}

	void CRendererMarshaller::LoadTexture(const wchar_t *pPath, ITexture **ppTexture)
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		AddLambda(
			[this, pPath, evt, ppTexture]()
			{
				Caustic::CCausticFactory::Instance()->LoadTexture(pPath, m_spRenderer, ppTexture);
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
	}

	void CRendererMarshaller::LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture)
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		AddLambda(
			[this, pPath, evt, ppTexture]()
			{
				Caustic::CCausticFactory::Instance()->LoadVideoTexture(pPath, m_spRenderer, ppTexture);
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
	}

	void CRendererMarshaller::SetSceneGraph(ISceneGraph *pSceneGraph)
	{
		if (pSceneGraph)
			pSceneGraph->AddRef();
		AddLambda(
			[this, pSceneGraph]()
			{
//				m_spRenderer->SetSceneGraph(pSceneGraph);
				if (pSceneGraph)
					pSceneGraph->Release();
			}
		);
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
		CloseHandle(evt);
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
		CloseHandle(evt);
	}

	DXGI_SAMPLE_DESC CRendererMarshaller::GetSampleDesc()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		DXGI_SAMPLE_DESC desc = {};
		AddLambda(
			[&]()
			{
				desc = m_spRenderer->GetSampleDesc();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return desc;
	}

	DXGI_FORMAT CRendererMarshaller::GetFormat()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		AddLambda(
			[&]()
			{
				format = m_spRenderer->GetFormat();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return format;
	}
	
	CComPtr<ID3D12RootSignature> CRendererMarshaller::GetRootSignature()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CComPtr<ID3D12RootSignature> spRootSignature;
		AddLambda(
			[&]()
			{
				spRootSignature = m_spRenderer->GetRootSignature();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return spRootSignature;
	}

	CComPtr<ID3D12Device5> CRendererMarshaller::GetDevice()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CComPtr<ID3D12Device5> spDevice;
		AddLambda(
			[&]()
			{
				spDevice = m_spRenderer->GetDevice();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return spDevice;
	}

	CRefObj<ICamera> CRendererMarshaller::GetCamera()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CRefObj<ICamera> spCamera;
		AddLambda(
			[&]()
			{
				spCamera = m_spRenderer->GetCamera();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return spCamera;
	}

	uint32 CRendererMarshaller::GetFrameIndex()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		uint32 frameIndex;
		AddLambda(
			[&]()
			{
				frameIndex = m_spRenderer->GetFrameIndex();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return frameIndex;
	}

	CComPtr<ID3D12GraphicsCommandList4> CRendererMarshaller::GetCommandList()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CComPtr< ID3D12GraphicsCommandList4> spCmdList;
		AddLambda(
			[&]()
			{
				spCmdList = m_spRenderer->GetCommandList();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return spCmdList;
	}

	void CRendererMarshaller::SetCamera(ICamera *pCamera)
	{
		pCamera->AddRef();
		AddLambda(
			[&]()
			{
				m_spRenderer->SetCamera(pCamera);
				pCamera->Release();
			}
		);
	}

	void CRendererMarshaller::Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice)
	{
		AddLambda(
			[&]()
			{
				m_spRenderer->Setup(hwnd, shaderFolder, createDebugDevice);
			}
		);
	}

	CRefObj<IShaderMgr> CRendererMarshaller::GetShaderMgr()
	{
		HANDLE evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		CRefObj<IShaderMgr> spShaderMgr;
		AddLambda(
			[&]()
			{
				spShaderMgr = m_spRenderer->GetShaderMgr();
				SetEvent(evt);
			}
		);
		WaitForSingleObject(evt, INFINITE);
		CloseHandle(evt);
		return spShaderMgr;
	}

	void CRendererMarshaller::RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
	{
		AddLambda(
			[&]()
			{
				m_spRenderer->RenderFrame(renderCallback);
			}
		);
	}

	void CRendererMarshaller::AddPointLight(IPointLight *pLight)
	{
		pLight->AddRef();
		AddLambda(
			[&]()
			{
				m_spRenderer->AddPointLight(pLight);
				pLight->Release();
			}
		);
	}

	void CRendererMarshaller::GetRenderCtx(IRenderCtx **ppCtx)
	{
		AddLambda(
			[&]()
			{
				m_spRenderer->GetRenderCtx(ppCtx);
			}
		);
	}

	void CRendererMarshaller::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
	{
		AddLambda(
			[&]()
			{
				m_spRenderer->DrawLine(p1, p2, clr);
			}
		);
	}

    void CRendererMarshaller::AddLambda(std::function<void()> func)
    {
        EnterCriticalSection(&m_cs);
        m_queue.push(func);
        LeaveCriticalSection(&m_cs);
    }

#if 0
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
#endif
	void CRendererMarshaller::CallOnRenderThread(std::function<void(IRenderer *pRenderer)> func, bool wait)
	{
		HANDLE evt;
		if (wait)
			evt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		AddLambda(
			[&]()
			{
				func(m_spRenderer);
				if (wait)
					SetEvent(evt);
			}
		);
		if (wait)
		{
			WaitForSingleObject(evt, INFINITE);
			CloseHandle(evt);
		}
	}
}
