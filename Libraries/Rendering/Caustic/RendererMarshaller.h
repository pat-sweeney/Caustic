//**********************************************************************
// Copyright Patrick Sweeney 2015-2016
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "IRendererMarshaller.h"
#include "Base\Core\RefCount.h"
#include <functional>
#include <queue>
#include <type_traits>
#include <string>

namespace Caustic
{
    //**********************************************************************
    //! \brief CRendererMarshaller marshals function calls from a user thread
    //! to the render thread.
    //**********************************************************************
    class CRendererMarshaller :
        public IRendererMarshaller,
        public CRefCount
    {
        CRITICAL_SECTION m_cs;
        CRefObj<IRenderer> m_spRenderer;
        std::queue<std::function<void()> > m_queue;
        bool m_exit;
        HANDLE m_thread;
		HWND m_hwnd;
		std::wstring m_shaderFolder;
		std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> m_renderCallback;
		CRefObj<ISceneGraph> m_spScene;

        void AddLambda(std::function<void()> func);
    public:
        CRendererMarshaller();
        void MainLoop();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRendererMarshaller methods
        //**********************************************************************
		virtual void Initialize(HWND hwnd, std::wstring &shaderFolder, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override;
		virtual void Shutdown() override;
		virtual void SetMaxCmdLength() override;
		virtual void GetRenderer(IRenderer **ppRenderer) override;
		virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) override;
		virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) override;
		virtual void SetSceneGraph(ISceneGraph *pSceneGraph) override;
		virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;
		virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;

        //**********************************************************************
        // IRenderer methods
        //**********************************************************************
		virtual DXGI_SAMPLE_DESC GetSampleDesc() override;
		virtual DXGI_FORMAT GetFormat() override;
		virtual CComPtr<ID3D12RootSignature> GetRootSignature() override;
		virtual CComPtr<ID3D12Device5> GetDevice() override;
		virtual CRefObj<ICamera> GetCamera() override;
		virtual uint32 GetFrameIndex() override;
		virtual CComPtr<ID3D12GraphicsCommandList4> GetCommandList() override;
		virtual void SetCamera(ICamera *pCamera) override;
		virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) override;
		virtual CRefObj<IShaderMgr> GetShaderMgr() override;
		virtual void BeginFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override
		{
		}
		virtual void EndFrame() override
		{
		}
		virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override; // Have renderer draw and present next frame
		virtual void GetRenderCtx(IRenderCtx **ppCtx) override;
		virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
		virtual void DrawMesh(IRenderMesh *pMesh) override {}
		virtual void CallOnRenderThread(std::function<void(IRenderer *pRenderer)> func, bool wait) override;
	};
}
