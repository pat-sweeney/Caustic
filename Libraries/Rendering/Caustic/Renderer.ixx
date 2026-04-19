//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <any>
#include <vector>
#include <stack>
#include <chrono>
#include <atlbase.h>
#include <d3d11.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <string>
#ifdef SUPPORT_GRAPHICS_CAPTURE
#include <DXGItype.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <DXProgrammableCapture.h>
#endif // SUPPORT_GRAPHICS_CAPTURE

export module Rendering.Caustic.Renderer;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Event;
import Base.Core.CritSec;
import Base.Math.BBox;
import Geometry.Mesh.Mesh;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderInfo;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.IDecal;
import Rendering.Caustic.IParticleSystem;
import Imaging.Color;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.IPointLight;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.RendererFlags;

//**********************************************************************
// File: Renderer.ixx
// Contains the declarations related to the CRenderer class which implements <IRenderer>
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CLineVertex
    // Vertex structure for drawing lines
    //
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
    //**********************************************************************
    struct CLineVertex
    {
        float x, y, z;
    };

    //**********************************************************************
    // Class: CQuadVertex
    // Vertex structure for drawing screen space quads
    //
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
    //**********************************************************************
    class CLight : public IPointLight, public CRefCount
    {
    public:
//        void Render([[maybe_unused]]IRenderer* pRenderer) {}
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
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
    //**********************************************************************
    class CRenderCtx : public IRenderCtx, public CRefCount
    {
        uint32_t m_debugFlags;
        float m_NormalScale;
        int m_currentPass;
        bool m_passBlendable;
        uint32_t m_currentEpoch;
        uint32_t m_mostRecentEpoch;

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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderCtx
        //**********************************************************************
        virtual void SetDebugFlags(uint32_t flags) override { m_debugFlags = flags; }
        virtual uint32_t GetDebugFlags() override { return m_debugFlags; }
        virtual void SetNormalScale(float normalScale) override { m_NormalScale = normalScale; };
        virtual float GetNormalScale() override { return m_NormalScale; }
        virtual uint32_t GetCurrentPass() override { return m_currentPass; }
        virtual bool PassBlendable() override { return m_passBlendable; }
        virtual uint32_t GetEpoch() override { return m_currentEpoch; }
        virtual uint32_t IncrementEpoch() override { return ++m_currentEpoch; }
        virtual uint32_t GetMostRecentEpoch() override { return m_mostRecentEpoch; }
        virtual void SetMostRecentEpoch(uint32_t v) override { m_mostRecentEpoch = v; }
    };

    //**********************************************************************
    // Class: CGraphicsBase
    // Defines the data shared between our CGraphics and CRenderer objects
    //
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
    //**********************************************************************
    class CGraphicsBase : public CRefCount
    {
    protected:
        HWND m_hwnd;
        BBox2 m_finalViewport;
#ifdef SUPPORT_GRAPHICS_CAPTURE
        IDXGraphicsAnalysis* m_spGraphicsAnalysis;
#endif
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
        CComPtr<ID3D11RenderTargetView> m_spFinalRTView;    // Render target view if final RT override is set
        CComPtr<ID3D11DepthStencilView> m_spFinalStencilView; // Stencil view to use if final RT override is set
        CComPtr<ID3D11Texture2D> m_spFinalDepthStencilBuffer; // Depth map if final RT override is set
        CComPtr<ID3D11DepthStencilView> m_spStencilView;    // Stencil view
        CComPtr<ID3D11Texture2D> m_spDepthStencilBuffer;    // Our depth map
        CComPtr<ID3D11ShaderResourceView> m_spDepthSRView;  // SRV for reading depth in post-processing
        CRefObj<ITexture> m_spDepthTextureObj;               // ITexture wrapper for depth in shader binding
        D3D11_TEXTURE2D_DESC m_BBDesc;                      // Description of our back buffer

        // HDR post-processing render targets
        CComPtr<ID3D11Texture2D> m_spHDRTexture;            // HDR scene color RT (R16G16B16A16_FLOAT)
        CComPtr<ID3D11RenderTargetView> m_spHDRRTView;      // RTV for HDR scene render
        CRefObj<ITexture> m_spHDRTextureObj;                 // ITexture wrapper for shader binding
        CComPtr<ID3D11Texture2D> m_spPostProcessRT[2];      // Ping-pong post-processing RTs
        CComPtr<ID3D11RenderTargetView> m_spPostProcessRTV[2]; // RTVs for ping-pong
        CRefObj<ITexture> m_spPostProcessTexObj[2];          // ITexture wrappers for shader binding
        // Bloom downsample chain (half-res, quarter-res, eighth-res, sixteenth-res)
        static const int c_BloomMipCount = 4;
        CComPtr<ID3D11Texture2D> m_spBloomMipTexture[c_BloomMipCount];
        CComPtr<ID3D11RenderTargetView> m_spBloomMipRTV[c_BloomMipCount];
        CRefObj<ITexture> m_spBloomMipTexObj[c_BloomMipCount]; // ITexture wrappers for shader binding
        // Normal buffer for SSR (written as MRT from PBR pass)
        CRefObj<ITexture> m_spNormalBufferObj;
        CComPtr<ID3D11RenderTargetView> m_spNormalRTV;
        CRefObj<IShaderMgr> m_spShaderMgr;                  // Our shader manager
        CComPtr<ID3D11Texture2D> m_spShadowTexture[c_MaxShadowMaps];        // Texture for shadow map
        CComPtr<ID3D11ShaderResourceView> m_spShadowSRView[c_MaxShadowMaps];  // Shader resource view for m_spShadowTexture
        CComPtr<ID3D11DepthStencilView> m_spShadowMapStencilView[c_MaxShadowMaps];
        int m_shadowMapWidth[c_MaxShadowMaps];
        int m_shadowMapHeight[c_MaxShadowMaps];
        int m_shadowMapLightWidth[c_MaxShadowMaps];
        int m_shadowMapLightHeight[c_MaxShadowMaps];
        RECT m_viewRect;
        D3D11_VIEWPORT m_viewport;

        void AdjustViewport();
        void DeviceWindowResizedInternal(uint32_t width, uint32_t height);
        virtual void InitializeD3D(HWND hwnd, BBox2 &viewport);
        void Setup(HWND hwnd, BBox2 &viewport, bool createDebugDevice, int desktopIndex);
        void SetCamera(ICamera* pCamera);
        CComPtr<ID3D11Device> GetDevice() { return m_spDevice; }
        CComPtr<IDXGIOutputDuplication> GetDuplication() { return m_spDuplication; }
        CComPtr<ID3D11DeviceContext> GetContext() { return m_spContext; }
        CRefObj<ICamera> GetCamera() { return m_spCamera; }
        CRefObj<IShaderMgr> GetShaderMgr() { return m_spShaderMgr; }
#ifdef SUPPORT_GRAPHICS_CAPTURE
        void BeginCapture() { m_spGraphicsAnalysis->BeginCapture(); }
        void EndCapture() { m_spGraphicsAnalysis->EndCapture(); }
#endif // SUPPORT_GRAPHICS_CAPTURE
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
    // Struct: CascadeShadowData
    // Stores per-cascade light view-projection matrices and split depths
    // for cascaded shadow mapping.
    //**********************************************************************
    struct CascadeShadowData
    {
        DirectX::XMMATRIX cascadeViewProj[c_NumCascades];
        float cascadeSplitDepths[c_NumCascades];
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
    // Module:
    // {Link:import Rendering.Caustic.Renderer;{Rendering/Caustic/Renderer.ixx}}
    //**********************************************************************
    class CRenderer : 
        public CGraphicsBase,
        public IRenderer
    {
    protected:
        HANDLE m_freezeEvent;
        int m_freeze;
        std::stack<ShadowMapRenderState> m_shadowMapRenderState; // Render state changes due to shadow mapping
        CascadeShadowData m_cascadeData; // Cascade shadow map data for the current frame
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
        CComPtr<ID3D11Buffer> m_spCubeVB;                   // Vertex buffer for decal unit cube
        CComPtr<ID3D11Buffer> m_spCubeIB;                   // Index buffer for decal unit cube
        CRefObj<IShader> m_spQuadShader;                    // Shader used to draw screen space quads
        CComPtr<ID3D11RasterizerState> m_spRasterizerState;
        bool m_depthTestEnabled;
        // Post-processing shaders
        CRefObj<IShader> m_spBloomExtractShader;
        CRefObj<IShader> m_spBloomBlurShader;
        CRefObj<IShader> m_spBloomCompositeShader;
        CRefObj<IShader> m_spFXAAShader;
        CRefObj<IShader> m_spSSAOShader;
        CRefObj<IShader> m_spSSAOBlurShader;
        bool m_postProcessEnabled;
        bool m_bloomEnabled;
        bool m_fxaaEnabled;
        bool m_ssaoEnabled;
        float m_bloomThreshold;
        float m_bloomIntensity;
        float m_exposure;

        // Point-light shadow cubemap
        static const int c_MaxPointShadowLights = 1;
        static const int c_PointShadowMapSize = 512;
        CRefObj<ITexture> m_spPointShadowCubemap[c_MaxPointShadowLights];
        int m_numPointShadowLights;

        // Spot-light shadow maps
        static const int c_MaxSpotShadowLights = 4;
        static const int c_SpotShadowMapSize = 1024;
        CRefObj<ITexture> m_spSpotShadowMap[c_MaxSpotShadowLights];
        CComPtr<ID3D11DepthStencilView> m_spSpotShadowDSV[c_MaxSpotShadowLights];
        DirectX::XMMATRIX m_spotShadowViewProj[c_MaxSpotShadowLights];
        int m_numSpotShadowLights;

        // IBL (Image-Based Lighting)
        CRefObj<ITexture> m_spEnvironmentMap;       // User-provided HDR cubemap
        CRefObj<ITexture> m_spIrradianceMap;        // Diffuse irradiance cubemap (32x32)
        CRefObj<ITexture> m_spPrefilteredMap;       // Specular pre-filtered cubemap (128x128, 5 mips)
        CRefObj<ITexture> m_spBRDFLUT;              // BRDF integration LUT (512x512)
        CRefObj<IShader> m_spBRDFLUTShader;
        CRefObj<IShader> m_spIrradianceShader;
        CRefObj<IShader> m_spPrefilterShader;
        bool m_iblDirty;                            // True when env map changed, need to regenerate

        // Tiled forward lighting
        CRefObj<IShader> m_spTileCullShader;
        CRefObj<IGPUBuffer> m_spLightBuffer;        // StructuredBuffer<LightData> for all lights
        CRefObj<IGPUBuffer> m_spTileLightBuffer;    // RWStructuredBuffer<uint> for per-tile light indices
        bool m_tiledLightingEnabled;

        // SSR (Screen-Space Reflections)
        CRefObj<IShader> m_spSSRShader;
        bool m_ssrEnabled;

        // Volumetric fog
        CRefObj<IShader> m_spFogShader;
        CRefObj<IShader> m_spFogCompositeShader;
        CRefObj<ITexture> m_spFogRT;                // Half-res fog render target
        CComPtr<ID3D11RenderTargetView> m_spFogRTV;
        bool m_fogEnabled;
        uint32_t m_fogFrameCounter;
        float m_fogDensity;
        FRGBColor m_fogColor;
        float m_fogHeightFalloff;
        float m_fogScattering;      // Henyey-Greenstein g parameter
        float m_fogMaxDistance;
        float m_fogStartHeight;

        // Decals
        CRefObj<IShader> m_spDecalShader;
        std::vector<CRefObj<IDecal>> m_decals;

        // Particles
        std::vector<CRefObj<IParticleSystem>> m_particleSystems;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;
        bool m_hasLastFrameTime = false;

        // Screen-space subsurface scattering
        CRefObj<IShader> m_spSSSBlurShader;
        CComPtr<ID3D11RenderTargetView> m_spSSSMaskRTV;
        CRefObj<ITexture> m_spSSSMaskTextureObj;
        bool m_sssEnabled;
        float m_sssWidth;
        FRGBColor m_sssColor;
        CComPtr<ID3D11BlendState> m_spDecalBlendState;
        CComPtr<ID3D11RasterizerState> m_spDecalRastState;
        CRefObj<ITexture> m_spDepthCopy;            // Copy of depth for SRV during decal pass

        // Temporal Anti-Aliasing (TAA)
        CRefObj<IShader> m_spMotionVectorShader;
        CRefObj<IShader> m_spTAAResolveShader;
        CComPtr<ID3D11RenderTargetView> m_spMotionVectorRTV;
        CRefObj<ITexture> m_spMotionVectorTexObj;
        CComPtr<ID3D11RenderTargetView> m_spTAAHistoryRTV[2];
        CRefObj<ITexture> m_spTAAHistoryTexObj[2];
        int m_taaHistoryIndex;              // Ping-pong index: read from [index], write to [1-index]
        bool m_taaHistoryValid;             // False on first frame or after resize
        DirectX::XMMATRIX m_prevJitteredViewProj;   // Previous frame's jittered viewProj
        DirectX::XMMATRIX m_jitteredProjection;     // Current frame jittered projection
        uint32_t m_taaFrameIndex;           // Halton sequence frame counter
        EAntiAliasMode m_aaMode;
        float m_taaBlendFactor;

        void CheckThread()
        {
#ifdef DIAGNOSTICS
            if (m_renderThreadId != GetCurrentThreadId())
            {
                OutputDebugString(L"ERROR! Renderer called from non-renderer thread! Clients should be calling the marshalled version of the renderer\n");
                CT(E_UNEXPECTED);
            }
#endif
        }
        void LoadShaderBlob(const std::wstring &filename, ID3DBlob **ppBlob);
        CRefObj<IShaderInfo> LoadShaderInfo(const std::wstring &filename);
        void RenderScene(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback);
        void DrawSceneObjects(int pass, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback);
        void SetShadowmapViewport(int whichShadowMap, int lightMapIndex);
        void ComputeCascadeSplits(float nearClip, float farClip, float splitDepths[c_NumCascades]);
        void ComputeCascadeViewProj(ICamera* pCamera, const Vector3& lightDir, float nearSplit, float farSplit, DirectX::XMMATRIX& outViewProj);
        void RunPostProcessing();
        void RenderPointShadows(int pass, std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback);
        void RenderSpotShadows(int pass, std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback);
        void GenerateIBLMaps();
        void DispatchTileLightCull();
        bool IsBoxInFrustum(const BBox3& bbox, const DirectX::XMVECTOR frustumPlanes[6]);
        bool m_frustumCullingEnabled;
    public:
        explicit CRenderer();
        virtual ~CRenderer();
        virtual void InitializeD3D(HWND hwnd, BBox2 &viewport) override;
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CGraphicsBase::AddRef(); }
        virtual uint32_t Release() override { return CGraphicsBase::Release(); }

        //**********************************************************************
        // IRenderer
        //**********************************************************************
        virtual void DeviceWindowResized(uint32_t width, uint32_t height) override;
        virtual void SetViewport(float x0, float y0, float x1, float y1) override;
        virtual DWORD GetRenderThreadID() override { return m_renderThreadId; }
        virtual CRefObj<IRenderMesh> ToRenderMesh(IMesh* pMesh, IShader* pShader) override;
        virtual void ToRenderMaterials(IMesh* pMesh, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial) override;
        virtual bool IsRenderThread() override { return true; }
        virtual void RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait /* = false */) override;
#ifdef _DEBUG
        virtual void BeginMarker(const wchar_t* pLabel) override
        {
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
            spCtx2->BeginEventInt(pLabel, 0);
        }
        virtual void EndMarker() override
        {
            CheckThread();
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
            spCtx2->EndEvent();
        }

#ifdef SUPPORT_GRAPHICS_CAPTURE
        virtual void BeginCapture() override
        {
            CGraphicsBase::BeginCapture();
        }

        virtual void EndCapture() override
        {
            CGraphicsBase::EndCapture();
        }
#endif // SUPPORT_GRAPHICS_CAPTURE

#endif
        virtual bool EnableDepthTest(bool enable) override;
        virtual CComPtr<ID3D11Device> GetDevice() override { CheckThread();  return CGraphicsBase::GetDevice(); }
        virtual CComPtr<IDXGIOutputDuplication> GetDuplication() override { CheckThread();  return m_spDuplication; }
        virtual void Freeze() override;
        virtual void Unfreeze() override;
        virtual bool IsFrozen() override;
        virtual void RenderLoop(std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override;
        virtual void RenderFrame(
            std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
            std::function<void(IRenderer* pRenderer)> prePresentCallback
        ) override;
        virtual uint32_t GetBackBufferWidth() override { return m_BBDesc.Width; }
        virtual uint32_t GetBackBufferHeight() override { return m_BBDesc.Height; }
        virtual CComPtr<ID3D11Texture2D> GetBackBuffer() { return m_spBackBuffer; }
        virtual void CopyFrameBackBuffer(IImage* pImage) override;
        virtual void DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler *pSampler, bool disableDepth = false) override;
        virtual void DrawScreenQuadWithCustomShader(IShader *pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth = false) override;
        virtual void LoadShaders(const wchar_t* pFolder) override;
        virtual CComPtr<ID3D11DeviceContext> GetContext() override { CheckThread(); return CGraphicsBase::GetContext(); }
        virtual CRefObj<ICamera> GetCamera() override { CheckThread(); return CGraphicsBase::GetCamera(); }
        virtual void SetCamera(ICamera* pCamera) override { CheckThread(); CGraphicsBase::SetCamera(pCamera); }
        virtual CRefObj<IShaderMgr> GetShaderMgr() override { CheckThread(); return CGraphicsBase::GetShaderMgr(); }
        virtual void AddRenderable(IRenderable* pRenderable) override;
        virtual void Setup(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool createDebugDevice, bool startFrozen = false, int desktopIndex = 0) override;
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) override;
        virtual void AddPointLight(IPointLight *pLight) override;
        virtual CRefObj<IRenderCtx> GetRenderCtx() override;
        virtual void ClearDepth() override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual void BeginShadowmapPass(int whichShadowmap) override;
        virtual void EndShadowmapPass(int whichShadowmap) override;
        virtual void PushShadowmapRT(int whichShadowmap, int lightMapIndex, const Vector3& lightPos, const Vector3& lightDir) override;
        virtual void PopShadowmapRT() override;
        virtual void SelectShadowmap(int whichShadowMap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader) override;
        virtual CRefObj<ITexture> GetShadowmapTexture(int whichShadowMap) override;
        virtual void SetFinalRenderTarget(ID3D11Texture2D* pTexture) override;
        virtual void SetFinalRenderTargetUsingSharedTexture(IUnknown* pTexture) override;
        virtual void SetPostProcessEnabled(bool enabled) override { m_postProcessEnabled = enabled; }
        virtual void SetBloomEnabled(bool enabled) override { m_bloomEnabled = enabled; }
        virtual void SetBloomParams(float threshold, float intensity) override { m_bloomThreshold = threshold; m_bloomIntensity = intensity; }
        virtual void SetFXAAEnabled(bool enabled) override { m_fxaaEnabled = enabled; m_aaMode = enabled ? EAntiAliasMode::FXAA : EAntiAliasMode::None; }
        virtual void SetAntiAliasMode(EAntiAliasMode mode) override
        {
            m_aaMode = mode;
            m_fxaaEnabled = (mode == EAntiAliasMode::FXAA);
            if (mode != EAntiAliasMode::TAA)
                m_taaHistoryValid = false;
        }
        virtual void SetSSAOEnabled(bool enabled) override { m_ssaoEnabled = enabled; }
        virtual void SetExposure(float exposure) override { m_exposure = exposure; }
        virtual void SetEnvironmentMap(ITexture* pCubemap) override
        {
            m_spEnvironmentMap = CRefObj<ITexture>(pCubemap);
            m_iblDirty = true;
        }
        virtual void SetTiledLightingEnabled(bool enabled) override { m_tiledLightingEnabled = enabled; }
        virtual void SetSSREnabled(bool enabled) override { m_ssrEnabled = enabled; }
        virtual void SetFrustumCullingEnabled(bool enabled) override { m_frustumCullingEnabled = enabled; }
        virtual void SetFogEnabled(bool enabled) override { m_fogEnabled = enabled; }
        virtual void SetFogParams(float density, FRGBColor& color, float heightFalloff,
            float scattering, float maxDistance, float startHeight) override
        {
            m_fogDensity = density;
            m_fogColor = color;
            m_fogHeightFalloff = heightFalloff;
            m_fogScattering = scattering;
            m_fogMaxDistance = maxDistance;
            m_fogStartHeight = startHeight;
        }
        virtual void SetSSSEnabled(bool enabled) override { m_sssEnabled = enabled; }
        virtual void SetSSSParams(float width, FRGBColor& color) override
        {
            m_sssWidth = width;
            m_sssColor = color;
        }
        virtual void AddDecal(IDecal* pDecal) override { m_decals.push_back(CRefObj<IDecal>(pDecal)); }
        virtual void RemoveDecal(IDecal* pDecal) override
        {
            for (auto it = m_decals.begin(); it != m_decals.end(); ++it)
            {
                if (it->p == pDecal)
                {
                    m_decals.erase(it);
                    return;
                }
            }
        }
        virtual void AddParticleSystem(IParticleSystem* pParticleSystem) override { m_particleSystems.push_back(CRefObj<IParticleSystem>(pParticleSystem)); }
        virtual void RemoveParticleSystem(IParticleSystem* pParticleSystem) override
        {
            for (auto it = m_particleSystems.begin(); it != m_particleSystems.end(); ++it)
            {
                if (it->p == pParticleSystem)
                {
                    m_particleSystems.erase(it);
                    return;
                }
            }
        }
    };
}
