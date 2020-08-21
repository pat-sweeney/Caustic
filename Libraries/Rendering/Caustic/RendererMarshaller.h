//**********************************************************************
// Copyright Patrick Sweeney 2015-2016
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\RefCount.h"
#include <functional>
#include <queue>
#include <type_traits>

//**********************************************************************
// File: RendererMarshaller.h
// Contains the declaration for the <CRendererMarshaller> class.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderQueue
    // Implements the render queue. Render commands from the client are stored
    // in this queue until the render thread picks them up and executes them.
    //
    // Header:
    // [Link:Rendering/Caustic/RendererMarshaller.h]
    //**********************************************************************
    struct CRenderQueue
    {
        CRITICAL_SECTION m_cs;
        std::queue<std::function<void()>> m_queue;

        //**********************************************************************
        // Method: AddLambda
        // Adds the specified lambda into the render's instruction queue
        //**********************************************************************
        void AddLambda(std::function<void()> func)
        {
            EnterCriticalSection(&m_cs);
            m_queue.push(func);
            LeaveCriticalSection(&m_cs);
        }
    };

    //**********************************************************************
    // Class: CRendererMarshaller
    // Implements <IRendererMarshaller>
    //
    // Members:
    // m_cs - Critical section used to control access to the render queue
    // m_spRenderer - Server side renderer
    // m_queue - Queue of client side renderer requests
    // m_exit - Flag to indicate whether the renderer is exiting
    // m_thread - Handle to the renderer thread
    //
    // Header:
    // [Link:Rendering/Caustic/RendererMarshaller.h]
    //**********************************************************************
    class CRendererMarshaller :
        public IRenderer,
        public IRendererMarshaller,
        public CRefCount
    {
        CRefObj<IRenderer> m_spRenderer;
        CRenderQueue m_renderQueue;
        bool m_exit;
        HANDLE m_thread;
        std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> m_renderCallback;

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
        virtual CRefObj<ITexture> LoadTexture(const wchar_t *pPath) override;
        virtual CRefObj<ITexture> LoadVideoTexture(const wchar_t *pPath) override;
        virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;
        virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;
        virtual void RunOnRenderer(std::function<void(IRenderer*, void* clientData)> callback, void* clientData) override;
        virtual CRefObj<IRenderer> GetRenderer() { return m_spRenderer; }

        //**********************************************************************
        // IGraphics methods
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() override;
        virtual CComPtr<ID3D11DeviceContext> GetContext() override;
        virtual CRefObj<ICamera> GetCamera() override;
        virtual CRefObj<IShaderMgr> GetShaderMgr() override { return m_spRenderer->GetShaderMgr(); }

        //**********************************************************************
        // IRenderer methods
        //**********************************************************************
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) override;
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) override; // Draws a mesh
        virtual void RenderLoop(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override; // Renderer entry point
        virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override; // Have renderer draw and present next frame
        virtual void SetCamera(ICamera *pCamera) override; // Sets camera
        virtual void AddPointLight(IPointLight *pLight) override;
        virtual CRefObj<IRenderCtx> GetRenderCtx() override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual CRefObj<IGraphics> GetGraphics() override { return CRefObj<IGraphics>(nullptr); }
        virtual void PushShadowmapRT(int whichShadowmap, bool clear, Vector3& lightPos) override;
        virtual void PopShadowmapRT() override;
        virtual void SelectShadowmap(int whichShadowmap) override {}
    };
}
