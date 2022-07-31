//**********************************************************************
// Copyright Patrick Sweeney 2015-2016
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <queue>
#include <type_traits>
#include <dxgi1_6.h>
#include <atlbase.h>

export module Rendering.RendererMarshaller.RendererMarshaller;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.ITexture;
import Rendering.RendererMarshaller.IRendererMarshaller;

//**********************************************************************
// File: RendererMarshaller.h
// Contains the declaration for the <CRendererMarshaller> class.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CRenderQueue
    // Implements the render queue. Render commands from the client are stored
    // in this queue until the render thread picks them up and executes them.
    //
    // Module:
    // {Link:import Rendering.Caustic.RendererMarshaller;{Rendering/Caustic/RendererMarshaller.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.RendererMarshaller;{Rendering/Caustic/RendererMarshaller.ixx}}
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
        DWORD m_renderThreadID;
        std::function<void(IRenderer* pRenderer)> m_postCreate;
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> m_renderCallback;
        std::function<void(IRenderer* pRenderer)> m_prePresentCallback;

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
        virtual void Initialize(HWND hwnd, BBox2 &viewport,
            std::wstring& shaderFolder,
            std::function<void(IRenderer* pRenderer)> postCreate,
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback,
            bool startFrozen = false, int desktopIndex = 0) override;
        virtual void Shutdown() override;
        virtual CRefObj<ITexture> LoadTexture(const wchar_t* pPath) override;
        virtual CRefObj<ITexture> LoadVideoTexture(const wchar_t* pPath) override;
        virtual void SaveScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph) override;
        virtual void LoadScene(const wchar_t* pFilename, ISceneGraph* pSceneGraph) override;
        virtual CRefObj<IRenderer> GetRenderer() { return this; }

        //**********************************************************************
        // IRenderer methods
        //**********************************************************************
        virtual void DeviceWindowResized(uint32 width, uint32 height) override;
        virtual void SetViewport(float x0, float y0, float x1, float y1) override;
        virtual DWORD GetRenderThreadID() override { return m_renderThreadID; }
        virtual CRefObj<IRenderMesh> ToRenderMesh(IMesh* pMesh, IShader* pShader) override;
        virtual void ToRenderMaterials(IMesh* pMesh, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial) override;
        virtual bool IsRenderThread() override { return false; }
        virtual void RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait /* = false */) override;
        virtual bool EnableDepthTest(bool enable) override;
        virtual void Freeze() override;
        virtual void Unfreeze() override;
        virtual bool IsFrozen() override;
        virtual void AddRenderable(IRenderable* pRenderable) override;
        virtual CComPtr<ID3D11Device> GetDevice() override;
#ifdef _DEBUG
        virtual void BeginMarker(const wchar_t* pLabel) override;
        virtual void EndMarker() override;
#endif
        virtual CComPtr<IDXGIOutputDuplication> GetDuplication() override;
        virtual void DrawScreenQuadWithCustomShader(IShader* pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth = false) override;
        virtual void DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth = false) override;
        virtual void LoadShaders(const wchar_t* pFolder) override;
        virtual CComPtr<ID3D11DeviceContext> GetContext() override;
        virtual void ClearDepth() override;
        virtual CRefObj<ICamera> GetCamera() override;
        virtual CRefObj<IShaderMgr> GetShaderMgr() override { return m_spRenderer->GetShaderMgr(); }
        virtual void Setup(HWND hwnd, BBox2 &viewport, std::wstring& shaderFolder, bool createDebugDevice, bool startFrozen = false, int desktopIndex = 0) override;
        virtual void DrawMesh(IRenderSubMesh* pMesh, IMaterialAttrib* pMaterial, ITexture* pTexture, IShader* pShader, DirectX::XMMATRIX& mat) override; // Draws a mesh
        virtual void RenderLoop(
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override; // Renderer entry point
        virtual void RenderFrame(
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override; // Have renderer draw and present next frame
        virtual uint32 GetBackBufferWidth() override;
        virtual uint32 GetBackBufferHeight() override;
        virtual CComPtr<ID3D11Texture2D> GetBackBuffer() override;
        virtual void CopyFrameBackBuffer(IImage* pImage) override;
        virtual void SetCamera(ICamera* pCamera) override; // Sets camera
        virtual void AddPointLight(IPointLight* pLight) override;
        virtual CRefObj<IRenderCtx> GetRenderCtx() override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual void PushShadowmapRT(int whichShadowmap, int lightMapIndex, const Vector3& lightPos, const Vector3& lightDir) override;
        virtual void PopShadowmapRT() override;
        virtual void SelectShadowmap(int whichShadowmap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader) override;
        virtual CRefObj<ITexture> GetShadowmapTexture(int whichShadowmap) override;
        virtual void SetFinalRenderTarget(ID3D11Texture2D* pTexture) override;
        virtual void SetFinalRenderTargetUsingSharedTexture(IUnknown* pTexture) override;
    };
}
