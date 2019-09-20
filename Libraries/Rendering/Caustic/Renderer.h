//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
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
#include <atlbase.h>
#include <d3d11.h>

//**********************************************************************
// File: Renderer.h
// Contains the declarations related to the CRenderer class which implements <IRenderer>
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Group: Constants
    // c_RenderCmd_DrawMesh - Command ID for rendering a mesh
    // c_RenderCmd_SetCamera  - Command ID for setting the camera
    //
    // Group: Pass Flags
    // c_PassFirst - first pass
    // c_PassObjID - renders pass writing object IDs to output texture
    // c_PassShadow - pass that renders shadow maps
    // c_PassOpaque - pass rendering the opaque objects
    // c_PassTransparent - pass rendering transparent objects
    // c_PassEnvironment - pass rendering environment maps
    // c_PassLast - last pass
    // c_PassAllPasses - combination of all the pass flags
    //**********************************************************************

    const int c_RenderCmd_DrawMesh = 0; // Command ID for rendering a mesh
    const int c_RenderCmd_SetCamera = 1; // Command ID for setting the camera

    const int c_PassFirst = 0;
    const int c_PassObjID = 0;
    const int c_PassShadow = 1;
    const int c_PassOpaque = 2;
    const int c_PassTransparent = 3;
    const int c_PassEnvironment = 4;
    const int c_PassLast = c_PassEnvironment;
    const int c_PassAllPasses = (1 << c_PassLast) - 1;

    struct IRenderMaterial;

    //**********************************************************************
    // Class: CLineVertex
    // Vertex structure for drawing lines
    //**********************************************************************
    struct CLineVertex
    {
        float x, y, z;
    };

    //**********************************************************************
    // Class: CLight
    // Class implementing <IPointLight>
    //**********************************************************************
    class CLight : public IPointLight, public CRefCount
    {
    public:
        void Render(IGraphics * /*pGraphics*/) {}
    };

    //**********************************************************************
    // Class: CRenderCtx
    // Class implementing <IRenderCtx>
    //**********************************************************************
    class CRenderCtx : public IRenderCtx, public CRefCount
    {
        uint32 m_debugFlags;
        float m_NormalScale;
        int m_currentPass;
        bool m_passBlendable;

    public:
        friend class CRenderer;
        friend class CRenderable;

        CRenderCtx() :
            m_debugFlags(0),
            m_NormalScale(1.0f),
            m_currentPass(0),
            m_passBlendable(false)
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
        virtual bool PassBlendable() { return m_passBlendable; }
    };

    const int c_MaxShadowMaps = 4;

    //**********************************************************************
    // Class: CGraphicsBase
    // Defines the data shared between our CGraphics and CRenderer objects
    //**********************************************************************
    class CGraphicsBase : public CRefCount
    {
    protected:
        CComPtr<ID3D11Device> m_spDevice;                   // D3D Device
        CComPtr<ID3D11DeviceContext> m_spContext;           // D3D Device context
        CComPtr<IDXGISwapChain> m_spSwapChain;              // D3D Swap chain
        D3D_FEATURE_LEVEL m_featureLevel;                   // D3D feature level
        CRefObj<ICamera> m_spCamera;                        // Camera to use for rendering
        CRefObj<IRenderCtx> m_spRenderCtx;                  // D3D Render context
        CComPtr<ID3D11RenderTargetView> m_spRTView;         // Render target view
        CComPtr<ID3D11DepthStencilView> m_spStencilView;    // Stencil view
        CComPtr<ID3D11Texture2D> m_spDepthStencilBuffer;    // Our depth map
        D3D11_TEXTURE2D_DESC m_BBDesc;                      // Description of our back buffer

        friend CAUSTICAPI void CreateGraphics(HWND hwnd, IGraphics **ppGraphics);

        void InitializeD3D(HWND hwnd);
        void Setup(HWND hwnd, bool createDebugDevice);
        void SetCamera(ICamera *pCamera);
        CComPtr<ID3D11Device> GetDevice() { return m_spDevice; }
        CComPtr<ID3D11DeviceContext> GetContext() { return m_spContext; }
        CRefObj<ICamera> GetCamera() { return m_spCamera; }
    };

    //**********************************************************************
    // Class: CGraphics
    // See <IGraphics>
    //**********************************************************************
    class CGraphics : public CGraphicsBase, public IGraphics
    {
    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IGraphics
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() override { return CGraphicsBase::GetDevice(); }
        virtual CComPtr<ID3D11DeviceContext> GetContext() override { return CGraphicsBase::GetContext(); }
        virtual CRefObj<ICamera> GetCamera() override { return CGraphicsBase::GetCamera(); }
        virtual void SetCamera(ICamera *pCamera) override { CGraphicsBase::SetCamera(pCamera); }
    };

    //**********************************************************************
    // Class: CRenderer
    // See <IRenderer>
    //**********************************************************************
    class CRenderer : 
        public CGraphicsBase,
        public IRenderer
    {
        CRefObj<ISceneGraph> m_spSceneGraph;
        std::vector<CRenderable> m_singleObjs;                              // List of individual renderable objects (outside scene graph)
        std::vector<CRefObj<IPointLight>> m_lights;                         // List of lights in this scene
        CComPtr<ID3D11Texture2D> m_spObjIDTexture;                          // Texture for rendering object IDs
        CComPtr<ID3D11RenderTargetView> m_spObjIDRTView;                    // Render target view for m_spObjIDTexture
        CComPtr<ID3D11Texture2D> m_spShadowTexture[c_MaxShadowMaps];        // Texture for shadow map
        CComPtr<ID3D11RenderTargetView> m_spShadowRTView[c_MaxShadowMaps];  // Render target view for m_spShadowTexture

        CEvent m_waitForShutdown;                       // Event to control shutdown (waits for render thread to exit)
        bool m_exitThread;                              // Controls whether we are exiting the render thread
        CComPtr<ID3D11Buffer> m_spLineVB;               // Vertex buffer used to draw lines
        CRefObj<IShader> m_spLineShader;                // Shader used to draw lines
        CComPtr<ID3D11Buffer> m_spInfinitePlaneVB;      // Vertex buffer used to draw ground plane
        CComPtr<ID3D11Buffer> m_spInfinitePlaneIB;      // Index buffer used to draw ground plane
        CRefObj<IShader> m_spInfinitePlaneShader;       // Shader used to draw ground plane
#ifdef SUPPORT_FULLQUAD
        CComPtr<ID3D11Buffer> m_spFullQuadVB;           // Vertex buffer used for drawing full screen quad
        CComPtr<ID3D11Buffer> m_spFullQuadIB;           // Index buffer used for drawing full screen quad
        CRefObj<IShader> m_spFullQuadShader;            // Shader used for drawing full screen quad
#endif // SUPPORT_FULLQUAD

        void LoadDefaultShaders(const wchar_t *pFolder);
        void LoadShaderBlob(std::wstring &filename, ID3DBlob **ppBlob);
        void LoadShaderInfo(std::wstring &filename, IShaderInfo **ppShaderInfo);
        void RenderScene();
        void DrawSceneObjects(int pass);
    public:
        explicit CRenderer();
        virtual ~CRenderer();
        void RenderLoop();
        void RenderFrame();
        void InitializeD3D(HWND hwnd);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CGraphicsBase::AddRef(); }
        virtual uint32 Release() override { return CGraphicsBase::Release(); }

        //**********************************************************************
        // IGraphics
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() override { return CGraphicsBase::GetDevice(); }
        virtual CComPtr<ID3D11DeviceContext> GetContext() override { return CGraphicsBase::GetContext(); }
        virtual CRefObj<ICamera> GetCamera() override { return CGraphicsBase::GetCamera(); }
        virtual void SetCamera(ICamera *pCamera) override { CGraphicsBase::SetCamera(pCamera); }

        //**********************************************************************
        // IRenderer
        //**********************************************************************
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) override;
        virtual void DrawMesh(IRenderSubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) override;
        virtual void AddPointLight(IPointLight *pLight) override;
        virtual void GetRenderCtx(IRenderCtx **ppCtx) override;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual void GetGraphics(IGraphics **ppGraphics) override;
        void DrawInfinitePlane();
    };
}
