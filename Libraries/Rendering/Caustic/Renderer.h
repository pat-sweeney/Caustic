//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Geometry\Mesh\Mesh.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\CritSec.h"
#include "Base\Core\Event.h"
#include "Shader.h"
#include <memory>
#include <any>
#include <vector>
#include <stack>
#include <atlbase.h>
#include <d3d11.h>
#include <d3d11_4.h>

//**********************************************************************
// File: Renderer.h
// Contains the declarations related to the CRenderer class which implements <IRenderer>
//**********************************************************************

namespace Caustic
{

    struct IRenderMaterial;

    //**********************************************************************
    // Class: CLineVertex
    // Vertex structure for drawing lines
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    struct CLineVertex
    {
        float x, y, z;
    };

    //**********************************************************************
    // Class: CQuadVertex
    // Vertex structure for drawing screen space quads
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    struct CQuadVertex
    {
        float x, y, z;
        float u, v;
    };

    //**********************************************************************
    // Class: CLight
    // Class implementing <IPointLight>
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    class CLight : public IPointLight, public CRefCount
    {
    public:
        void Render(IRenderer* /*pRenderer*/) {}
    };

    //**********************************************************************
    // Class: CRenderCtx
    // Class implementing <IRenderCtx>
    //
    // Members:
    // m_debugFlags - current set of debug flags (used for rendering debug information)
    // m_NormalScale - scale factor to apply to lines when drawing vertex/face normals
    // m_currentPass - current pass (transparent, opaque, shadow, ...)
    // m_passBlendable - indicates whether alpha blending is enabled on the current pass
    // m_currentEpoch - current epoch for keeping track of changes. Each time something changes this number is updated
    // m_mostRecentEpoch - most recent epoch we have found while traversing the render graph
    // c_MaxShadowMaps - maximum number of shadow maps that can be active during a frame
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    class CRenderCtx : public IRenderCtx, public CRefCount
    {
        uint32 m_debugFlags;
        float m_NormalScale;
        int m_currentPass;
        bool m_passBlendable;
        uint32 m_currentEpoch;
        uint32 m_mostRecentEpoch;

    public:
        friend class CRenderer;
        friend class CRenderable;

        CRenderCtx() :
            m_debugFlags(0),
            m_NormalScale(1.0f),
            m_currentPass(0),
            m_passBlendable(false),
            m_currentEpoch(0),
            m_mostRecentEpoch(0)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderCtx
        //**********************************************************************
        virtual void SetDebugFlags(uint32 flags) override { m_debugFlags = flags; }
        virtual uint32 GetDebugFlags() override { return m_debugFlags; }
        virtual void SetNormalScale(float normalScale) override { m_NormalScale = normalScale; };
        virtual float GetNormalScale() override { return m_NormalScale; }
        virtual uint32 GetCurrentPass() override { return m_currentPass; }
        virtual bool PassBlendable() override { return m_passBlendable; }
        virtual uint32 GetEpoch() override { return m_currentEpoch; }
        virtual uint32 IncrementEpoch() override { return ++m_currentEpoch; }
        virtual uint32 GetMostRecentEpoch() override { return m_mostRecentEpoch; }
        virtual void SetMostRecentEpoch(uint32 v) override { m_mostRecentEpoch = v; }
    };

    //**********************************************************************
    // Class: CGraphicsBase
    // Defines the data shared between our CGraphics and CRenderer objects
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    class CGraphicsBase : public CRefCount
    {
    protected:
        CComPtr<ID3D11Device> m_spDevice;                   // D3D Device
        CComPtr<ID3D11DeviceContext> m_spContext;           // D3D Device context
        CComPtr<IDXGISwapChain> m_spSwapChain;              // D3D Swap chain
        D3D_FEATURE_LEVEL m_featureLevel;                   // D3D feature level
        CComPtr<IDXGIOutputDuplication> m_spDuplication;    // Windows Duplication service
        CComPtr<ID3D11Texture2D> m_spBackBuffer;
        CComPtr<ID3D11Texture2D> m_spCPUBackBuffer;         // Version of back buffer with CPU access
        CRefObj<ICamera> m_spCamera;                        // Camera to use for rendering
        CRefObj<IRenderCtx> m_spRenderCtx;                  // D3D Render context
        CComPtr<ID3D11RenderTargetView> m_spRTView;         // Render target view
        CComPtr<ID3D11DepthStencilView> m_spStencilView;    // Stencil view
        CComPtr<ID3D11Texture2D> m_spDepthStencilBuffer;    // Our depth map
        D3D11_TEXTURE2D_DESC m_BBDesc;                      // Description of our back buffer
        CRefObj<IShaderMgr> m_spShaderMgr;                  // Our shader manager
        CComPtr<ID3D11Texture2D> m_spShadowTexture[c_MaxShadowMaps];        // Texture for shadow map
        CComPtr<ID3D11ShaderResourceView> m_spShadowSRView[c_MaxShadowMaps];  // Shader resource view for m_spShadowTexture
        CComPtr<ID3D11DepthStencilView> m_spShadowMapStencilView[c_MaxShadowMaps];
        int m_shadowMapWidth[c_MaxShadowMaps];
        int m_shadowMapHeight[c_MaxShadowMaps];
        D3D11_VIEWPORT m_viewport;

        friend CAUSTICAPI CRefObj<IRenderer> CreateGraphics(HWND hwnd);

        virtual void InitializeD3D(HWND hwnd);
        void Setup(HWND hwnd, bool createDebugDevice, int desktopIndex);
        void SetCamera(ICamera* pCamera);
        CComPtr<ID3D11Device> GetDevice() { return m_spDevice; }
        CComPtr<IDXGIOutputDuplication> GetDuplication() { return m_spDuplication; }
        CComPtr<ID3D11DeviceContext> GetContext() { return m_spContext; }
        CRefObj<ICamera> GetCamera() { return m_spCamera; }
        CRefObj<IShaderMgr> GetShaderMgr() { return m_spShaderMgr; }
    };

    //**********************************************************************
    struct ShadowMapRenderState
    {
        CRefObj<ICamera> m_spOldCamera;
        CComPtr<ID3D11RenderTargetView> m_spOldRT;
        CComPtr<ID3D11DepthStencilView> m_spOldStencil;
        D3D11_VIEWPORT m_viewport;
    };

    //**********************************************************************
    // Class: CRenderer
    // Implementation of <IRenderer>
    //
    // Members:
    // m_renderThreadId - Thread ID this thread is running on. That is the only valid thread to make calls to this object from.
    // m_spShaderMgr - Our shader manager
    // m_singleObjs - List of individual renderable objects (outside scene graph)
    // m_lights - List of lights in this scene
    // m_spObjIDTexture - Texture for rendering object IDs
    // m_spObjIDRTView - Render target view for m_spObjIDTexture
    // m_spShadowTexture[c_MaxShadowMaps] - Texture for shadow map
    // m_spShadowRTView[c_MaxShadowMaps] - Render target view for m_spShadowTexture
    // m_waitForShutdown - Event to control shutdown (waits for render thread to exit)
    // m_exitThread - Controls whether we are exiting the render thread
    // m_spLineVB - Vertex buffer used to draw lines
    // m_spLineShader - Shader used to draw lines
    // m_spQuadVB - Vertex buffer used to draw screen space quads
    // m_spQuadShader - Shader used to draw screen space quads
    //
    // Header:
    // [Link:Rendering/Caustic/Renderer.h]
    //**********************************************************************
    class CRenderer : 
        public CGraphicsBase,
        public IRenderer
    {
    protected:
        HANDLE m_freezeEvent;
        int m_freeze;
        std::stack<ShadowMapRenderState> m_cameras;         // Cameras from shadow mapping
        DWORD m_renderThreadId;                             // Render thread's ID
        std::vector<CRefObj<IRenderable>> m_singleObjs;              // List of individual renderable objects (outside scene graph)
        std::vector<CRefObj<ILight>> m_lights;              // List of lights in this scene
        CComPtr<ID3D11Texture2D> m_spObjIDTexture;          // Texture for rendering object IDs
        CComPtr<ID3D11RenderTargetView> m_spObjIDRTView;    // Render target view for m_spObjIDTexture
        CEvent m_waitForShutdown;                           // Event to control shutdown (waits for render thread to exit)
        bool m_exitThread;                                  // Controls whether we are exiting the render thread
        CComPtr<ID3D11Buffer> m_spLineVB;                   // Vertex buffer used to draw lines
        CRefObj<IShader> m_spLineShader;                    // Shader used to draw lines
        CComPtr<ID3D11Buffer> m_spQuadVB;                   // Vertex buffer used to draw screen space quads
        CComPtr<ID3D11Buffer> m_spQuadIB;                   // Index buffer used to draw screen space quads
        CRefObj<IShader> m_spQuadShader;                    // Shader used to draw screen space quads
        CComPtr<ID3D11RasterizerState> m_spRasterizerState;
        bool m_depthTestEnabled;

#ifdef DIAGNOSTICS
        void CheckThread()
        {
            if (m_renderThreadId != GetCurrentThreadId())
            {
                OutputDebugString(L"ERROR! Renderer called from non-renderer thread! Clients should be calling the marshalled version of the renderer\n");
                CT(E_UNEXPECTED);
            }
        }
#define CHECKTHREAD CheckThread()
#else
#define CHECKTHREAD
#endif
        void LoadShaderBlob(std::wstring &filename, ID3DBlob **ppBlob);
        CRefObj<IShaderInfo> LoadShaderInfo(std::wstring &filename);
        void RenderScene(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback);
        void DrawSceneObjects(int pass, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback);
        void SetShadowmapViewport(int whichShadowMap, int lightMapIndex);
    public:
        explicit CRenderer();
        virtual ~CRenderer();
        virtual void InitializeD3D(HWND hwnd) override;
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CGraphicsBase::AddRef(); }
        virtual uint32 Release() override { return CGraphicsBase::Release(); }

        //**********************************************************************
        // IRenderer
        //**********************************************************************
#ifdef _DEBUG
        virtual void BeginMarker(const wchar_t* pLabel) override
        {
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
            spCtx2->BeginEventInt(pLabel, 0);
        }
        virtual void EndMarker() override
        {
            CHECKTHREAD;
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
            spCtx2->EndEvent();
        }
#endif
        virtual bool EnableDepthTest(bool enable) override;
        virtual CComPtr<ID3D11Device> GetDevice() override { CHECKTHREAD;  return CGraphicsBase::GetDevice(); }
        virtual CComPtr<IDXGIOutputDuplication> GetDuplication() override { CHECKTHREAD;  return m_spDuplication; }
        virtual void Freeze() override;
        virtual void Unfreeze() override;
        virtual void RenderLoop(std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override;
        virtual void RenderFrame(
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override;
        virtual uint32 GetBackBufferWidth() override { return m_BBDesc.Width; }
        virtual uint32 GetBackBufferHeight() override { return m_BBDesc.Height; }
        virtual void CopyFrameBackBuffer(IImage* pImage) override;
        virtual void DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler *pSampler, bool disableDepth = false) override;
        virtual void DrawScreenQuadWithCustomShader(IShader *pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth = false) override;
        virtual void LoadShaders(const wchar_t* pFolder) override;
        virtual CComPtr<ID3D11DeviceContext> GetContext() override { CHECKTHREAD; return CGraphicsBase::GetContext(); }
        virtual CRefObj<ICamera> GetCamera() override { CHECKTHREAD; return CGraphicsBase::GetCamera(); }
        virtual void SetCamera(ICamera* pCamera) override { CHECKTHREAD; CGraphicsBase::SetCamera(pCamera); }
        virtual CRefObj<IShaderMgr> GetShaderMgr() override { CHECKTHREAD; return CGraphicsBase::GetShaderMgr(); }
        virtual void AddRenderable(IRenderable* pRenderable) override;
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice, bool startFrozen = false, int desktopIndex = 0) override;
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) override;
        virtual void AddPointLight(IPointLight *pLight) override;
        virtual CRefObj<IRenderCtx> GetRenderCtx() override;
        virtual void ClearDepth() override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual void PushShadowmapRT(int whichShadowmap, int lightMapIndex, Vector3& lightPos, Vector3& lightDir) override;
        virtual void PopShadowmapRT() override;
        virtual void SelectShadowmap(int whichShadowMap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader) override;
        virtual CRefObj<ITexture> GetShadowmapTexture(int whichShadowMap) override;
    };
}
